#include <asf.h>

void io_init(void);
void timer0_ctcmode_init(uint8_t top);

int main(void)
{
    io_init();                // IOポート設定
    timer0_ctcmode_init(249); // タイマ0設定
    sei();
    while (1);
    return 0;
}

void io_init(void) // IOポート設定
{
    DDRD |= (1 << DDD0);     // PD0を出力設定
    PORTD &= ~(1 << PORTD0); // PD0にLOWを出力
}

ISR(TIMER0_COMPA_vect) // タイマ0コンペアマッチA割り込みサービスルーチン
{
    PORTD |= (1 << PORTD0);
}

ISR(TIMER0_COMPB_vect) // タイマ0コンペアマッチB割り込みサービスルーチン
{
    PORTD &= ~(1 << PORTD0);
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