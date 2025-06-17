#include <asf.h>
int main(void)
{
	uint8_t A = 0, B = 0; // A 端子, B 端子の値
	uint8_t rotary_q1 = 0, rotary_q2 = 0, prev_rotary_q1 = 0;
	// チャタリング除去用
	DDRD &= ~(1 << DDD2) & ~(1 << DDD3); // 入出力ポート設定
	DDRB |= (1 << DDB5);
	PORTB &= ~(1 << PORTB5);

	while (1)
	{
		A = (PIND & (1 << PIND2)) ? 1 : 0; // A 端子読み込み
		B = (PIND & (1 << PIND3)) ? 1 : 0; // B 端子読み込み
		if(A == 0 && B == 0){
			rotary_q1 = 0;
			rotary_q2 = rotary_q2;
		}else if(A == 0 && B == 1){
			rotary_q1 = rotary_q1;
			rotary_q2 = 1;
		}else if(A == 1 && B == 0){
			rotary_q1 = rotary_q1;
			rotary_q2 = 0;
		}else if(A == 1 && B == 1){
			rotary_q1 = 1;
			rotary_q2 = rotary_q2;
		}		

		// rotary_q1,rotary_q2 に変換（チャタリング除去）
		if (rotary_q1 != prev_rotary_q1)
		{ // rotary_q1 が変化して
			if (rotary_q1 == 1)
			{ // 立ち上がったら
				if (rotary_q2 == 0)
				{                         // 右回転なら
					PORTB |= 1 << PORTB5; // 点灯
				}
				else
				{     // 左回転なら
					PORTB &= ~(1 << PORTB5); // 消灯
				}
			}
			prev_rotary_q1 = rotary_q1; // 状態を更新
		}
	}
	return 0;
}