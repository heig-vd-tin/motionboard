;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;  _   _        _                            _                                
;;; | | | |      |_|                          | |                               
;;; | |_| |_____  _ _____           _     _ __| |                               
;;; | |_  | ___ || |  _  \  _____  \ \  / // _  |                               
;;; | | | | ____|| | |_| | (_____)  \ \/ /( (_| |                               
;;; |_| |_|_____)|_|___  |           \__/  \____|                               
;;;                  __| | Haute Ecole d'Ingenieurs                             
;;;                 |___/  et de Gestion - Vaud                                 
;;;                                                                             
;;; @title    Logiciel de contrôle de moteur pour la carte "motionboard"        
;;; @context  Coupe suisse de robotique 2007                                    
;;; @author   Y. Chevallier <nowox@kalios.ch>                                   
;;; @file     qcos.asm                                                          
;;; @language ASCII/ASM                                                         
;;; @svn      $Id: qcos.asm 155 2007-03-22 15:40:55Z ychevall@heig-vd.ch $      
;;;                                                                             
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  
                     
        .def _qcos  ;External ref
    
K5      .set 0x6480             ; Scaled to Q13
K4      .set 0x52FF             ; Scaled to Q20
K3      .set 0xAACC             ; Scaled to Q12
K2      .set 0x45B8             ; Scaled to Q15
K1      .set 0x7338             ; Scaled to Q14

_qcos:
        SETC   SXM,OVM 
        MOV    ACC,AL<<16       ; AH = AL
        ABS    ACC              ; ACC = abs(x) in Q15
        NEG    ACC              ; ACC = -x
        ADD    AH,#04000h       ; ACC = (PI/2 - x)
        CLRC   TC
        ABSTC  ACC              ;abs(PI/2-x)and store sign in TC

tylor:
        MOVL   XT,ACC           ;T=abs(PI/2-x) in Q15 and in first quardrant
        MPY     ACC,T,#K1           
        ADD     ACC,#K2<<14     ; ACC=K1*x+K2 in Q29
        QMPYL   ACC,XT,ACC      ; ACC=(K1*x+K2)*x in Q28
        ADD     AH,#K3          ; ACC=(K1*x+K2)*x+K3 in Q28
        QMPYL   ACC,XT,ACC      ; ACC=((K1*x+K2)*x+K3)*x in Q27
        ADD     ACC,#K4<<7      ; ACC=((K1*x+K2)*x+K3)*x+K4 in Q27
        QMPYL   ACC,XT,ACC      ; ACC=(((K1*x+K2)*x+K3)*x+K4)*x in Q26
        ADD     ACC,#K5<<13     ; ACC=(((K1*x+K2)*x+K3)*x+K4)*x+K5 in Q26
        QMPYL   ACC,XT,ACC      ; ACC=((((K1*x+K2)*x+K3)*x+K4)*x+K5)*x in Q25
        LSL     ACC,6           ; in Q31
        ABS     ACC             ; Saturate to 0x7fff
        NEGTC   ACC             ; ACC=-sin(x), if TC=1        
        MOV     AL,AH    
        CLRC 	OVM   
        LRETR
       
   
