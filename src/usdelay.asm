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
;;; @file     usdelay.asm                                                       
;;; @language ASCII/C                                                           
;;; @svn      $Id: regul.c 117 2007-03-03 17:41:08Z ychevall@heig-vd.ch $       
;;;                                                                             
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

       .def _DSP28x_usDelay
       .sect "ramfuncs"

       .global  __DSP28x_usDelay

_DSP28x_usDelay:
        SUB    ACC,#1
        BF     _DSP28x_usDelay,GEQ    ;; Loop if ACC >= 0
        LRETR 

