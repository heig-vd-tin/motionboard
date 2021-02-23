////////////////////////////////////////////////////////////////////////////////
///  _   _        _                            _                                
/// | | | |      |_|                          | |                               
/// | |_| |_____  _ _____           _     _ __| |                               
/// | |_  | ___ || |  _  \  _____  \ \  / // _  |                               
/// | | | | ____|| | |_| | (_____)  \ \/ /( (_| |                               
/// |_| |_|_____)|_|___  |           \__/  \____|                               
///                  __| | Haute Ecole d'Ingenieurs                             
///                 |___/  et de Gestion - Vaud                                 
///                                                                             
/// @title    Logiciel de contrôle de moteur pour la carte "motionboard"        
/// @context  Coupe suisse de robotique 2007                                    
/// @author   Y. Chevallier <nowox@kalios.ch>                                   
/// @file     smb_cmd_list.h                                                    
/// @language ASCII/C                                                           
/// @svn      $Id: main.c 105 2005-12-14 16:53:46Z Canard $                     
/// @desc     Ce fichier contient les adresses des registres de la carte        
///           moteur.                                                           
////////////////////////////////////////////////////////////////////////////////
#ifndef __scmb_523dfkhlj34h__
#define __scmb_523dfkhlj34h__
                              
////////////////////////////////////////////////////////////////////////////////
/// Command list                                                                
////////////////////////////////////////////////////////////////////////////////
#define SCMB_REG_BROADCAST          0x00
#define SCMB_REG_POWER_ON           0x01  //!< You must write 0x5F in this register
#define SCMB_REG_POWER_OFF          0x02  //!< You must write 0xF5 in this register
                                  //0x03
#define SCMB_REG_SYNC               0x04
#define SCMB_REG_RESET_POSITION_A   0x05
#define SCMB_REG_RESET_POSITION_B   0x06
#define SCMB_REG_LATCH_POSITIONS    0x08
                                  //0x09
#define SCMB_REG_COUPLING           0x0A  
                                  //0x0B
#define SCMB_REG_SMOOTH_STOP        0x0C
#define SCMB_REG_BB_BUSY            0x0D
                                  //0x0E
                                  //0x0F
#define SCMB_REG_MODE_A             0x10
#define SCMB_REG_CURRENT_MAX_A      0x11
#define SCMB_REG_SYNC_A             0x12
#define SCMB_REG_POS_CORRECTION_A   0x13
                                  //0x15
                                  //0x16
                                  //0x17
                                  //0x18
                                  //0x19
                                  //0x1A
        		          //0x1B
                                  //0x1C
                                  //0x1D
                                  //0x1E
                                  //0x1F
#define SCMB_REG_MODE_B             0x20
#define SCMB_REG_CURRENT_MAX_B      0x21
#define SCMB_REG_SYNC_B             0x22   
#define SCMB_REG_POS_CORRECTION_B   0x23
                                  //0x25
                                  //0x26
                                  //0x27
                                  //0x28
                                  //0x29
                                  //0x2A
        			  //0x2B
                                  //0x2C
                                  //0x2D
                                  //0x2E
                                  //0x2F
                                  //0x30
                                  //0x31
                                  //0x32
                                  //0x33
                                  //0x34
                                  //0x35
                                  //0x36
                                  //0x37
                                  //0x38
                                  //0x39
                                  //0x3A
        			  //0x3B
                                  //0x3C
                                  //0x3D
                                  //0x3E
                                  //0x3F
                                  //0x40
                                  //0x41
                                  //0x42
                                  //0x43
                                  //0x44
                                  //0x45
                                  //0x46
                                  //0x47
                                  //0x48
                                  //0x49
                                  //0x4A
        			  //0x4B
                                  //0x4C
                                  //0x4D
                                  //0x4E
                                  //0x4F
#define SCMB_REG_TORQUE_A           0x50
#define SCMB_REG_VELOCITY_A         0x51
#define SCMB_REG_POSITION_A         0x52
#define SCMB_REG_ZERO_A             0x53
#define SCMB_REG_LATCHED_POSITION_A 0x54
#define SCMB_REG_READ_POWER_A       0x55
                                  //0x56
                                  //0x57
                                  //0x58
                                  //0x59
#define SCMB_REG_BB_ACC_A           0x5A
#define SCMB_REG_BB_DEC_A     	    0x5B
#define SCMB_REG_BB_VEL_A     	    0x5C
#define SCMB_REG_BB_MOV_A           0x5D
#define SCMB_REG_BB_BUSY_A          0x5E
                                  //0x5F
#define SCMB_REG_TORQUE_B           0x60
#define SCMB_REG_VELOCITY_B         0x61
#define SCMB_REG_POSITION_B         0x62
#define SCMB_REG_ZERO_B             0x63
#define SCMB_REG_BB_BUSY_B          0x64
#define SCMB_REG_LATCHED_POSITION_B 0x65
#define SCMB_REG_READ_POWER_B       0x66
                                  //0x67
                                  //0x68
                                  //0x69
#define SCMB_REG_BB_ACC_B           0x6A
#define SCMB_REG_BB_DEC_B     	    0x6B
#define SCMB_REG_BB_VEL_B     	    0x6C
#define SCMB_REG_BB_MOV_B           0x6D
                                  //0x6E
                                  //0x6F
                                  //0x70
                                  //0x71
                                  //0x72
                                  //0x73
                                  //0x74
                                  //0x75
                                  //0x76
                                  //0x77
                                  //0x78
                                  //0x79
                                  //0x7A
        			              //0x7B
                                  //0x7C
                                  //0x7D
                                  //0x7E
                                  //0x7F
#define SCMB_REG_XYP_READ_X         0x80
#define SCMB_REG_XYP_READ_Y         0x81
                                  //0x82
                                  //0x83
                                  //0x84
                                  //0x85
                                  //0x86
                                  //0x87
                                  //0x88
                                  //0x89
                                  //0x8A
        			              //0x8B
                                  //0x8C
                                  //0x8D
                                  //0x8E
                                  //0x8F
                                  //0x90
                                  //0x91
                                  //0x92
                                  //0x93
                                  //0x94
                                  //0x95
                                  //0x96
                                  //0x97
                                  //0x98
                                  //0x99
                                  //0x9A
        			  //0x9B
                                  //0x9C
                                  //0x9D
                                  //0x9E
                                  //0x9F
                                  //0xA0
                                  //0xA1
                                  //0xA2
                                  //0xA3
                                  //0xA4
                                  //0xA5
                                  //0xA6
                                  //0xA7
                                  //0xA8
                                  //0xA9
                                  //0xAA
        			  //0xAB
                                  //0xAC
                                  //0xAD
                                  //0xAE
                                  //0xAF
                                  //0xB0
                                  //0xB1
                                  //0xB2
                                  //0xB3
                                  //0xB4
                                  //0xB5
                                  //0xB6
                                  //0xB7
                                  //0xB8
                                  //0xB9
                                  //0xBA
        			  //0xBB
                                  //0xBC
                                  //0xBD
                                  //0xBE
                                  //0xBF
                                  //0xC0
                                  //0xC1
                                  //0xC2
                                  //0xC3
                                  //0xC4
                                  //0xC5
                                  //0xC6
                                  //0xC7
                                  //0xC8
                                  //0xC9
                                  //0xCA
        			  //0xCB
                                  //0xCC
                                  //0xCD
                                  //0xCE
                                  //0xCF
#define SCMB_REG_TQ_KP_A            0xD0
#define SCMB_REG_TQ_GI_A            0xD1
#define SCMB_REG_SP_KP_A            0xD2
#define SCMB_REG_SP_GI_A            0xD3
#define SCMB_REG_POS_KP_A           0xD4
#define SCMB_REG_POS_GI_A           0xD5
                                  //0xD6
                                  //0xD7
                                  //0xD8
                                  //0xD9
                                  //0xDA
        			  //0xDB
                                  //0xDC
                                  //0xDD
                                  //0xDE
                                  //0xDF
#define SCMB_REG_TQ_KP_B            0xE0
#define SCMB_REG_TQ_GI_B            0xE1
#define SCMB_REG_SP_KP_B            0xE2
#define SCMB_REG_SP_GI_B            0xE3
#define SCMB_REG_POS_KP_B           0xE4
#define SCMB_REG_POS_GI_B           0xE5
                                  //0xE6
                                  //0xE7
                                  //0xE8
                                  //0xE9
                                  //0xEA
        			  //0xEB
                                  //0xEC
                                  //0xED
                                  //0xEE
                                  //0xEF
                                  //0xF0
                                  //0xF1
                                  //0xF2
                                  //0xF3
                                  //0xF4
                                  //0xF5
                                  //0xF6
                                  //0xF7
                                  //0xF8
                                  //0xF9
                                  //0xFA
        			  //0xFB
                                  //0xFC
                                  //0xFD
                                  //0xFE
#define SCMB_REG_SELECT_REGISTER    0xFF
#endif
              
////////////////////////////////////////////////////////////////////////////////
/// End of file.                                                                
////////////////////////////////////////////////////////////////////////////////

