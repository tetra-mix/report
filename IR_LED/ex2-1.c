// リーダコードの受信
#include <asf.h>
void io_init(void);
void start(void);
void timer0_ctcmode_init(uint8_t top);

// コメント
// 9ms = 9*10^3μsの時で考える
// 最大の1024分周時 0.05*1024=51.2μs
// この時のトップ値は 9*10^3/51.2 = 175.78
//
// 次に大きい256分周時 0.05*256=12.8μs
// この時のトップ値は 9*10^3/12.8 = 703.125
// これは8ビットを超えているので分周は1024で設定する必要がある

// リーダコード許容範囲（カウンタ値）プリスケーラ設定 1024 分周
#define LEADER_LMIN (uint8_t)158 // 9ms x 90%
#define LEADER_LMAX (uint8_t)193 // 9ms x 110%
#define LEADER_HMIN (uint8_t)79	 // 4.5ms x 90%
#define LEADER_HMAX (uint8_t)97	 // 4.5ms x 110%
#define TIME_OUT (uint8_t)195	 // 10ms

// ステート定義
enum
{
	LEADER_LOW,
	LEADER_HIGH,
	START_READING
} State,
NEXT_State;

int main(void)
{
	io_init(); // I/O ポート初期設定
	start();   // 受信スタート
	sei();
	while (1)
	;
	return 0;
}

void start(void) // 受信スタート
{
	EICRA = 0b00001000; // 外部割り込み（INT1）を立ち下がりに設定
	// 立下り時の設定
	// 2ビット目 ISC10 = 0
	// 3ビット目 ISC11 = 1

	EIMSK = 0b00000010; // 外部割り込み有効
	// 1ビット目 INT1 割り込み有効

	NEXT_State = LEADER_LOW; // 次の状態は LEADER_HIGH
	return;
}

ISR(INT1_vect) // 外部割り込みサブルーチン
{
	uint8_t intv = 0;	// パルス幅
	State = NEXT_State; // ステート更新
	switch (State)
	{
		case LEADER_LOW:
		TCNT0 = 0;					   // タイムカウンタリセット
		timer0_ctcmode_init(TIME_OUT); // タイムアウト設定（タイマ／カウンタ０）
		TIMSK0 = 0b00000010;		   // タイムアウト有効（コンペアマッチ A 割り込み）
		EICRA = 0b00001100;			   // 外部割り込みを立ち上がりにセット
		NEXT_State = LEADER_HIGH;	   // 次の状態へ
		break;
		case LEADER_HIGH:
		intv = TCNT0; // タイムカウンタ読み込み
		if (LEADER_LMIN <= intv && intv <= LEADER_LMAX)
		{								// L レベルが範囲内なら
			TCNT0 = 0;					// タイムカウンタリセット
			EICRA = 0b00001000;			// 外部割り込みを立ち下がりにセット
			NEXT_State = START_READING; // 次の状態へ
		}
		else // 範囲外ならリスタート
		start();
		break;
		case START_READING:
		intv = TCNT0; // タイムカウンタ読み込み
		if (LEADER_HMIN <= intv && intv <= LEADER_HMAX)
		{						  // H レベルが範囲内なら
			EIMSK = 0b000000000;  // 外部割り込み停止
			TIMSK0 = 0b000000000; // タイムアウト停止（タイマ０割込み）
			PORTB ^= 0b00100000;  // 受信完了、LED 反転
			start();
		}
		else // 範囲外ならリスタート
		start();
		break;
	}
	return;
}

void io_init(void) // I/O ポート設定
{
	DDRB = 0b00100000;  // LED(PB5)を出力に設定
	PORTB = 0b00000000; // LED 消灯
	DDRD = 0b00000000;  // 外部割り込み（INT1）を入力に設定
	// PD2,INT0が共通
	// PD3,INT1が共有
	return;
}

ISR(TIMER0_COMPA_vect) // タイムアウト検出
{
	start();
	return;
}
// タイムアウト設定
// タイマ／カウンタ０ CTC モード
// top : カウンタ最大値

void timer0_ctcmode_init(uint8_t top)
{
	OCR0A = top;		 // タイマ／カウンタ 0 最大値
	TCCR0A = 0b00000010; // CTC モード
	TCCR0B = 0b00000101;			 // CTC モード、プリスケーラ設定1024分周
	//以下のように設定
	//0ビット目 CS00 1
	//1ビット目 CS01 0
	//2ビット目 CS02 1
	return;
}