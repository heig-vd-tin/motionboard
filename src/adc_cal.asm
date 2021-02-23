;
; @title   MotionBoard project v2.0
; @file    adc_cal.c
; @brief   ADC calibration (original code from TI)
; @author  Yves Chevallier <yves.chevallier@kalios.ch>
; @svn     $Id: adc_cal.asm 490 2008-11-15 14:24:53Z yves $
;

    .def _ADC_cal
	.asg "0x711C",   ADCREFSEL_LOC

;-----------------------------------------------
; _ADC_cal
;-----------------------------------------------
;-----------------------------------------------
; This is the ADC cal routine.This routine is programmed into 
; reserved memory by the factory. 0xAAAA and 0xBBBB are place- 
; holders for calibration data.  
;The actual values programmed by TI are device specific. 
;
; This function assumes that the clocks have been
; enabled to the ADC module.
;-----------------------------------------------

    .sect ".adc_cal"

_ADC_cal
    MOVW  DP,   #ADCREFSEL_LOC >> 6
    MOV   @28,  #0xAAAA            ; actual value may not be 0xAAAA
    MOV   @29,  #0xBBBB             ; actual value may not be 0xBBBB
    LRETR
;eof ----------
