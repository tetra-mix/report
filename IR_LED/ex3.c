// データの受信
#include <asf.h>
void start(void);
void timer0_ctcmode_init(uint8_t top);
void timer1_ctcmode_init(uint16_t top);
void io_init(void);
int8_t receive_bit(uint8_t intv);
#define LEADER_LMIN (uint8_t) // 9ms x 90%
#define LEADER_LMAX (uint8_t) // 9ms x 110%
#define LEADER_HMIN (uint8_t) // 4.5ms x 90%
#define LEADER_HMAX (uint8_t) // 4.5ms x 110%
#define D1_TMIN (uint8_t)     // 2.25ms x 90%
#define D1_TMAX (uint8_t)     // 2.25ms x 110%
#define D0_TMIN (uint8_t)     // 1.125ms x 90%
#define D0_TMAX (uint8_t)     // 1.125ms x 110%
#define TIME_OUT (uint8_t)    // 10ms
#define CUSTOM1_CODE (uint8_t)0x00
#define CUSTOM2_CODE (uint8_t)0xFF
#define OFF_CODE 0x01 // OFF ボタン
#define LIGHT_CODE    // ON(明)ボタン
#define DARK_CODE     // ON(暗)ボタン
enum
{
    LEADER_LOW,
    LEADER_HIGH,
    START_READING,
    CUSTOM1,
    CUSTOM2,
    DATA1,
    DATA2,
    END,
    Error
} State,
    NEXT_State;
uint8_t bit_pos = 0, recv_custom1_code = 0, recv_custom2_code = 0, recv_data1_code = 0,
        recv_data2_code = 0;
int8_t bit;

int main(void)
{
    io_init();
    start();
    sei();
    while (1)
        ;
    return 0;
}
void io_init(void) // I/O ポート設定
{
    DDRB = ;  // LED(PB5)を出力に設定
    PORTB = ; // LED 消灯
    DDRD = ;  // 外部割り込み端子（INT1）を入力に設定
    return;
}

void start(void) // 受信スタート
{
    EICRA = ; // 外部割り込み（INT1）立ち下がり設定
    EIMSK = ; // 外部割り込み（INT1）有効
    NEXT_State = LEADER_LOW;
    return;
}
ISR(INT1_vect)
{
    uint8_t intv = 0;
    State = NEXT_State; // ステート更新
    switch (State)
    {
    case LEADER_LOW:                   // 立ち下がりエッジ
        TCNT0 = 0;                     // タイムカウンタリセット
        timer0_ctcmode_init(TIME_OUT); // タイムアウト設定
        TIMSK0 = 0b00000010;           // 割り込み有効（コンペアマッチ A）
        EICRA = 0b00001100;            // 立ち上がり割り込みをセット
        NEXT_State = LEADER_HIGH;      // 次の状態へ
        break;
    case LEADER_HIGH: // 立ち上がりエッジ
        intv = TCNT0; // タイムカウンタ読み出し
        if (LEADER_LMIN <= intv && intv <= LEADER_LMAX)
        {                               // L レベルが範囲内なら
            TCNT0 = 0;                  // タイムアウトカウンタリセット
            EICRA = ;                   // 立ち下がり割り込みをセット
            NEXT_State = START_READING; // 次は START_READING
        }
        else
        { // 範囲外ならリスタート
            start();
        }
        break;
    case START_READING: // 立ち下がりエッジ
        intv = TCNT0;   // タイムカウンタ読み出し
        if (LEADER_HMIN <= intv && intv <= LEADER_HMAX)
        {                          // H レベルが範囲内なら
            TCNT0 = 0;             // タイムカウンタリセット
            recv_custom1_code = 0; // カスタムコード１初期設定
            bit_pos = ;            // ビット位置設定（最下位ビットから）
            NEXT_State = CUSTOM1;  // 次は CUSTOM1
        }
        else
        { // 範囲外ならリスタート
            start();
        }
        break;
    case CUSTOM1:                // 立ち下がりエッジ
        intv = TCNT0;            // タイムカウンタ読み出し
        TCNT0 = 0;               // タイムカウンタリセット
        bit = receive_bit(intv); // １ビット受信
        if (bit < 0)
        { // 受信エラーならリスタート
            start();
        }
        else if (bit == 1)
        {
            recv_custom1_code |= bit_pos; // カスタムコードにセット
        }
        bit_pos <<= 1; // ビット位置をずらす
        if (bit_pos == 0)
        {                         // 8bit 受信したら
            recv_custom2_code = ; // カスタムコード２初期設定
            bit_pos = ;           // ビット位置設定（最下位ビットから）
            NEXT_State = CUSTOM2; // 次は CUSTOM2
        }
        break;
    case CUSTOM2:                // 立ち下がりエッジ
        intv = TCNT0;            // タイムカウンタ読み出し
        TCNT0 = 0;               // タイムカウンタリセット
        bit = receive_bit(intv); // １ビット受信
        if (bit < 0)
        { // 受信エラーならリスタート
            start();
        }
        else if (bit == 1)
        {
            recv_custom2_code |= bit_pos; // 反転カスタムコードにセット
        }
        bit_pos <<= 1; // ビット位置をずらす
        if (bit_pos == 0)
        { // 8bit 受信したら
            if ((recv_custom1_code ^ recv_custom2_code) != 0b11111111)
            {
                start(); // データが一致しないならリスタート
            }
            else
            {
                recv_data1_code = ; // データコード１初期設定
                bit_pos = ;         // ビット位置設定（最下位ビットから）
                NEXT_State = DATA1; // 次は DATA1
            }
        }
        break;
    case DATA1: // 立ち下がりエッジ
    case DATA2:
    {
        else
        { // 8 ビット目で受信データが正しいなら
            State = END;
            データ受信終了
            NEXT_State = Error;
            次のデータがあれば受信エラー
        }
    }
    case END:
        break;
    case Error: // 33bit 目があれば受信エラー
        start();
        break;
    }
}

ISR(TIMER0_COMPA_vect) // タイムアウト検出
{
    if (State == END)
    {                        // 受信完了
        TIMSK0 = 0b00000000; // タイマ０割り込み停止
        if (recv_data1_code == OFF_CODE)
        {                        // OFF スイッチ
            TIMSK1 = 0b00000000; // タイマ１割込停止、LED 消灯
            PORTB = 0b00000000;
        }
        else if (recv_data1_code == LIGHT_CODE)
        { // オン（明）スイッチ
            PORTB ^= 0b00100000;
            TCNT1 = 0;
            timer1_ctcmode_init(); // 0.5 秒間隔点滅
            TIMSK1 = 0b00000010;   // 割り込み有効（コンペアマッチ A 割り込み）
        }
        else if (recv_data1_code == DARK_CODE)
        { // オン（暗）スイッチ
            PORTB ^= 0b00100000;
            TCNT1 = 0;
            timer1_ctcmode_init(); // １秒間隔点滅
            TIMSK1 = 0b00000010;   // 割り込み有効（コンペアマッチ A 割り込み）
        }
        start();
    }
    else
    { // タイムアウトエラー
        start();
    }
}
ISR(TIMER1_COMPA_vect) // 受信処理
{
    PORTB ^= 0b00100000; // LED 反転(PB5)
}
int8_t receive_bit(uint8_t intv) // データ受信、チェック
{
    if (D0_TMIN <= intv && intv <= D0_TMAX)
        return 0; // データ０受信
    else if ()
        return 1; // データ１受信
    else
        return -1; // 受信エラー
}
void timer0_ctcmode_init(uint8_t top)
{
    OCR0A = top;         // タイマ／カウンタ 0 最大値
    TCCR0A = 0b00000010; // CTC モード
    TCCR0B = ;           // CS02 CS01 CS00 プリスケーラ
}
void timer1_ctcmode_init(uint16_t top)
{
    OCR1A = top;         // タイマ／カウンタ 1 最大値
    TCCR1A = 0b00000000; // CTC モード
    TCCR1B = ;           // CS12:CS11:CS10 プリスケーラ
}