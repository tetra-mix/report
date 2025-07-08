//V- AIN1 PD7#include <asf.h>
#include <asf.h>

void io_init(void);
void timer0_ctcmode_init(uint8_t top);
void timer1_ctcmode_init(uint16_t top);
void comparator_init(void);

int main(void)
{
	comparator_init();
	io_init();                // IOポート設定
	timer0_ctcmode_init(249); // タイマ0設定
	
	sei();
	while (1)
	;
	return 0;
}

void io_init(void) // IOポート設定
{
	DDRD |= (1 << DDD0);     // PD0を出力設定
	DDRB |= (1 << DDB5); //LED用出力設定
	PORTD &= ~(1 << PORTD0); // PD0にLOWを出力
	PORTB &= ~(1 << PORTB5);
}

ISR(TIMER0_COMPA_vect) // タイマ0コンペアマッチA割り込みサービスルーチン
{
	PORTD |= (1 << PORTD0);
	ACSR |= 1 << ACIE; // コンパレータ割込み有効（ノイズ対策）
}

ISR(TIMER0_COMPB_vect) // タイマ0コンペアマッチB割り込みサービスルーチン
{
	PORTD &= ~(1 << PORTD0);
}

// ↓追加
ISR(TIMER1_COMPA_vect)
{
	PORTB &= ~(1 << PORTB5); //LEDを消灯
	TIMSK1 = 0b00000000; //タイマ1の割り込みを不可に
}

ISR(ANALOG_COMP_vect) // アナログコンパレータ割り込みサービスルーチン
{
	PORTD &= ~ (1 << PORTD0);
	ACSR &= ~(1 << ACIE); // コンパレータ割込み無効（ノイズ対策）

	uint16_t width = 34;
	if(TCNT0 >= width){
		PORTB |= (1 << PORTB5); //LEDを点灯
		timer1_ctcmode_init( 19531 ); // タイマ1設定 ←追加
	}
}

void comparator_init(void)
{
	DDRD &= ~(1 << PORTD6); // V+ AIN0 PD6
	DDRD &= ~(1 << PORTD7); //V- AIN1 PD7
	
	ADCSRB &= ~(1 << ACME); // AIN1 セレクト
	
	// *** 割り込み設定 ***
	ACSR |= (1 << ACIE); // 割り込みを有効にする
	ACSR &= ~(1 << ACIS0);
	ACSR |= (1 << ACIS1);
	// AIN0 セレクト、//AIN0 ＜ AIN1 で割り込み
	// アナログコンパレータの割り込みは出力のところを見ている。
	// 今の条件 最初: AIN0 > AIN1 出力 HIGH
	//    割り込み時: AIN0 < AIN1 出力 LOW
	// 出力がHIGHからLOWに変わるから設定はFalling Output Edgeにする

	ACSR &= ~(1 << ACBG);
	ACSR &= ~(1 << ACD);

}

// タイマ0 CTC モード
void timer0_ctcmode_init(uint8_t top)
{
	OCR0A = top;        // 比較値A（TOP値）を設定
	OCR0B = OCR0A >> 1; // 比較値Bを設定（中央値）
	TCCR0A = 0b00000010;
	// ++ WGM01:WGM00 CTCモード top=OCR0A
	TCCR0B = 0b00000010;
	// |+++ CS12:CS11:CS10 8分周
	// +--- WGM02 CTCモード top=OCR0A
	// WGM02 → 0
	// CS12:CS11:CS10 → 010 8分周
	TCNT0 = 0; // カウンタクリア
	TIMSK0 = 0b00000110;
	// |+- OCIE0A //比較値A 割り込み許可
	// +-- OCIE0B //比較値B 割り込み許可
}

// ↓設定 タイマ1
void timer1_ctcmode_init(uint16_t top)
{
	OCR1A = top;        // 比較値A（TOP値）を設定
	// CTCモード WGM13:WGM12:WGM11:WGM10 = 0:1:0:0
	TCCR1A &= ~(1 << WGM10);
	TCCR1A &= ~(1 << WGM11);
	TCCR1B |= (1 << WGM12);
	TCCR1B &= ~(1 << WGM13);
	// 分周の設定
	// CS12:CS11:CS10 = 1:0:1 1024分周
	// 1024分周の時top値は 10^5/51.2=19531
	// それ以外の場合はオーバーフローしてしまう
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B |= (1 << CS12);
	
	TCNT1 = 0; // カウンタクリア
	TIMSK1 = 0b00000010;
	// |+- OCIE0A //比較値A 割り込み許可
}