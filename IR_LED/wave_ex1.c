#include <asf.h>
void io_init(void);
void timer0_ctcmode_init(uint8_t top);
void timer0_top(uint8_t top);

enum Led_Type
{
    ON,
    OFF
};
enum Led_Type LED_state, NEXT_LED_state;
#define LED_ON 175.4  // LED ON時間 (タイマカウンタ0 CTCモード TOP値)
#define LED_OFF 2 // LED OFF時間 (タイマカウンタ0 CTCモード TOP値)

int main(void)
{
    io_init(); // IOポート設定
    LED_state = OFF;
    timer0_ctcmode_init(LED_OFF); // タイマカウンタ0 CTCモード割り込み設定
    NEXT_LED_state = ON;
    sei(); // 割り込み許可
    while (1)
        ; // 通常処理
    return 0;
}

ISR(TIMER0_COMPA_vect) // 搬送波送信
{
    LED_state = NEXT_LED_state;
    if (LED_state == ON)
    {
        PORTD |= 0b00000001; // LEDをON
        timer0_top(LED_ON);     // ON時間の設定
        NEXT_LED_state = OFF;   // 次はOFF状態
    }
    else
    { // if LED_state is OFF
    }
}

void io_init(void) // IOポート設定
{
    // +---- PD0を出力にセット（LED出力）
    DDRD = 0b00000001;
    // +---- LEDを消灯
    PORTD = 0b00000001;
    return;
}

void timer0_top(uint8_t top)
{
    OCR0A = top; // タイマ／カウンタ0の最大値を設定
}
// タイマ／カウンタ0 ＣＴＣモード割り込み設定
// Clear Timer on Compare Match Mode
//

void timer0_ctcmode_init(uint8_t top)
{
    TCNT0 = 0;   // カウンタリセット
    OCR0A = top; // タイマ／カウンタ0の最大値を設定
    // ++-------COM0A1 COM0A0 OC0A端子出力 OFF
    // || ++ WGM01 WGM00 CTCモード
    TCCR0A = 0b00000010;
    // +------- FOC0A 0(CTCモード)
    // | +--- WGM02 CTCモード
    // | |+++ CS02 CS01 CS00 1分周
    TCCR0B = 0b00000001;
    // +- OCIE0A タイマ／カウンタ0コンペアマッチA 割り込み有効
    TIMSK0 = 0b00000010;
}


