INIT:
	    LDS R18, PRR
	    ANDI R18, (~(1 << 0))
	    STS PRR, R18

	    LDS R18, ACSR
	    ANDI R18, (~(1<<6))
	    ANDI R18, (~(1<<7))
	    STS ACSR, R18

	    LDS R18, ADCSRB
	    ANDI R18, (~(1<<6))
	    STS ADCSRB, R18

	    LDS R18, ADCSRA
	    ORI R18, (1 << 7)
	    STS ADCSRA, R18

    	LDS R18, DIDR0
	    ORI R18, (1 << 0)
	    ORI R18, (1 << 1)
	    ORI R18, (1 << 2)
	    ORI R18, (1 << 3)
	    ORI R18, (1 << 4)
	    ORI R18, (1 << 5)
	    STS DIDR0, R18
	
    	LDS R18, DIDR1
	    ORI R18, (1 << 0)
	    ORI R18, (1 << 1)
	    STS DIDR1, R18
	
	    LDI R18, 0b00111111
	    OUT DDRB, R18
    START:
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP

	    ;全体の回数を保存するレジスタ
	    LDI R20, 4
	    ;ビットの桁数を保存するレジスタ
	    LDI R21, 0b00010000
	    ;出力する状態を保存するレジスタ
	    LDI R22, 0b00000000
	    OUT PORTB, R22
    LOOP:
	    LSR R21
		OR R22, R21
		
	    ; 1回目 0b00001000
	    ; 2回目 0b00000100

	    OUT PORTB, R22
	
	    ;待ち処理 3回分 50ns * 3 = 150ns
	    NOP
	    NOP
	    NOP

	    IN R18, ACSR
	    ANDI R18, (1 << 5)
	    BRNE DOWN
		SUBI R20, 1
		BRNE LOOP
	    RJMP START
    DOWN:
	    MOV R23, R21
	    COM R23
	    AND R22, R23

    	;例: 2回目の処理
	    ; R22 0b000001100;
	    ; R23 0b111111011;
	    ;     0b000001000;
		;待ち処理
	    NOP
	    NOP
	    NOP

	    SUBI R20, 1
	    BRNE LOOP
	    RJMP START