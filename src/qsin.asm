; Tylor Series Approximation in the first quardrant:
; sin(x) = 3.1406625*x+0.02026367*x^2-5.325196*x^3+0.5446778*x^4
; +1.800293*x^5, where 'x' is the normalized radians

        .def _qsin  ;External ref

K5      .set 0x6480             ; Scaled to Q13
K4      .set 0x52FF             ; Scaled to Q20
K3      .set 0xAACC             ; Scaled to Q12
K2      .set 0x45B8             ; Scaled to Q15
K1      .set 0x7338             ; Scaled to Q14

_qsin:
        SETC    SXM,OVM         ; ACC=x
        MOV     ACC,AL<<16      ; AH='x', AL=0
        CLRC    TC
        ABSTC   ACC             ; TC= sign(x), AH=abs(x)

        LSL     ACC,#1          ; Convert to first quadrant (0 to pi/2)
        ABS     ACC
        SFR     ACC,#1

        MOVL    XT,ACC          ; XT=x in Q31 and in first quardrant

        MPY     ACC,T,#K1
        ADD     ACC,#K2<<14     ; ACC=K1*x+K2 in Q29
        QMPYL   ACC,XT,ACC      ; ACC=(K1*x+K2)*x in Q28
        ADD     AH,#K3          ; ACC=(K1*x+K2)*x+K3 in Q28
        QMPYL   ACC,XT,ACC      ; ACC=((K1*x+K2)*x+K3)*x in Q27
        ADD     ACC,#K4<<7      ; ACC=((K1*x+K2)*x+K3)*x+K4 in Q27
        QMPYL   ACC,XT,ACC      ; ACC=(((K1*x+K2)*x+K3)*x+K4)*x in Q26
        ADD     ACC,#K5<<13     ; ACC=(((K1*x+K2)*x+K3)*x+K4)*x+K5 in Q26
        QMPYL   ACC,XT,ACC      ; ACC=((((K1*x+K2)*x+K3)*x+K4)*x+K5)*x in Q25
        LSL     ACC,#6          ; in Q31
        ABS     ACC             ; Saturate to 0x7fff
        NEGTC   ACC             ; ACC=-sin(x), if TC=1
        MOV     AL,AH
        CLRC 	OVM
        LRETR
