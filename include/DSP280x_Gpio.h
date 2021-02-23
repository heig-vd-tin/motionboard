// TI File $Revision: /main/3 $
// Checkin $Date: April 24, 2006   12:51:13 $
//###########################################################################
//
// FILE:   DSP280x_Gpio.h
//
// TITLE:  DSP280x General Purpose I/O Definitions.
//
//###########################################################################
// $TI Release: DSP280x, DSP2801x Header Files V1.41 $
// $Release Date: August 7th, 2006 $
//###########################################################################

#ifndef DSP280x_GPIO_H
#define DSP280x_GPIO_H


#ifdef __cplusplus
extern "C" {
#endif

// GPxDIR GPIOx
#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

// GPxPUD GPIOx
#define EN_PULLUP   0
#define DIS_PULLUP  1

// GPxMUXx GPIOx
#define MUX_GPIO0_GPIO0     0
#define MUX_GPIO0_EPWM1A    1

#define MUX_GPIO1_GPIO1     0
#define MUX_GPIO1_EPWM1B    1
#define MUX_GPIO1_SPISIMOD  2

#define MUX_GPIO2_GPIO2     0
#define MUX_GPIO2_EPWM2A    1

#define MUX_GPIO3_GPIO3     0
#define MUX_GPIO3_EPWM2B    1
#define MUX_GPIO3_SPISOMID  2

#define MUX_GPIO4_GPIO4     0
#define MUX_GPIO4_EPWM3A    1

#define MUX_GPIO5_GPIO5		0
#define MUX_GPIO5_EPWM3B	1
#define MUX_GPIO5_SPICLKD	2
#define MUX_GPIO5_ECAP1		3 

#define MUX_GPIO6_GPIO6		0
#define MUX_GPIO6_EPWM4A	1
#define MUX_GPIO6_EPWMSYNCI	2
#define MUX_GPIO6_EPWMSYNCO 3

#define MUX_GPIO7_GPIO7		0
#define MUX_GPIO7_EPWM4B	1
#define MUX_GPIO7_SPISTED	2
#define MUX_GPIO7_ECAP2		3

#define MUX_GPIO8_GPIO8		0
#define MUX_GPIO8_EPWM5A	1
#define MUX_GPIO8_CANTXB	2
#define MUX_GPIO8_ADCSOCAO	3

#define MUX_GPIO9_GPIO9		0
#define MUX_GPIO9_EPWM5B	1
#define MUX_GPIO9_SCITXDB	2
#define MUX_GPIO9_ECAP3		3

#define MUX_GPIO10_GPIO10	0
#define MUX_GPIO10_EPWM6A	1
#define MUX_GPIO10_CANRXB	2
#define MUX_GPIO10_ADCSOCBO 3

#define MUX_GPIO11_GPIO11	0
#define MUX_GPIO11_EPWM6B	1
#define MUX_GPIO11_SCIRXDB	2
#define MUX_GPIO11_ECAP4	3

#define MUX_GPIO12_GPIO12	0
#define MUX_GPIO12_TZ1		1
#define MUX_GPIO12_CANTXB	2
#define MUX_GPIO12_SPISIMOB 3

#define MUX_GPIO13_GPIO13	0
#define MUX_GPIO13_TZ2		1
#define MUX_GPIO13_CANRXB	2
#define MUX_GPIO13_SPISOMIB 3

#define MUX_GPIO14_GPIO14	0
#define MUX_GPIO14_TZ3		1
#define MUX_GPIO14_SCITXDB	2
#define MUX_GPIO14_SPICLKB	3

#define MUX_GPIO15_GPIO15   0
#define MUX_GPIO15_TZ4		1
#define MUX_GPIO15_SCIRXDB	2
#define MUX_GPIO15_SPISTEB	3

#define MUX_GPIO16_GPIO16   0
#define MUX_GPIO16_SPISIMOA	1
#define MUX_GPIO16_TZ5	    3

#define MUX_GPIO17_GPIO17   0
#define MUX_GPIO17_SPISOMIA	1
#define MUX_GPIO17_TZ6  	3

#define MUX_GPIO18_GPIO18   0
#define MUX_GPIO18_SPICLKA	1
#define MUX_GPIO18_SCITXDB	2

#define MUX_GPIO19_GPIO19   0
#define MUX_GPIO19_SPITEA	1
#define MUX_GPIO19_SCIRXDB	2

#define MUX_GPIO20_GPIO20   0
#define MUX_GPIO20_EQEP1A   1
#define MUX_GPIO20_SPISIMOC 2
#define MUX_GPIO20_CANTXB   3

#define MUX_GPIO21_GPIO21   0
#define MUX_GPIO21_EQEP1B   1
#define MUX_GPIO21_SPISOMIC 2
#define MUX_GPIO21_CANRXB   3

#define MUX_GPIO22_GPIO22   0
#define MUX_GPIO22_EQEP1S   1
#define MUX_GPIO22_SPICLKC  2
#define MUX_GPIO22_SCITXDB  3

#define MUX_GPIO23_GPIO23   0
#define MUX_GPIO23_EQEP1I   1
#define MUX_GPIO23_SPISTEC  2
#define MUX_GPIO23_SCIRXDB  3

#define MUX_GPIO24_GPIO24   0
#define MUX_GPIO24_ECAP1    1
#define MUX_GPIO24_EQEP2A   2
#define MUX_GPIO24_SPISIMOB 3

#define MUX_GPIO25_GPIO25   0
#define MUX_GPIO25_ECAP2    1
#define MUX_GPIO25_EQEP2B   2
#define MUX_GPIO25_SPISOMIB 3

#define MUX_GPIO26_GPIO26   0
#define MUX_GPIO26_ECAP3    1
#define MUX_GPIO26_EQEP2I   2
#define MUX_GPIO26_SPICLKB  3

#define MUX_GPIO27_GPIO27   0
#define MUX_GPIO27_ECAP4    1
#define MUX_GPIO27_EQEP2S   2
#define MUX_GPIO27_SPISTEB  3

#define MUX_GPIO28_GPIO28   0
#define MUX_GPIO28_SCIRXDA  1
#define MUX_GPIO28_TZ5      3

#define MUX_GPIO29_GPIO29   0
#define MUX_GPIO29_SCITXDA  1
#define MUX_GPIO29_TZ6      3

#define MUX_GPIO30_GPIO30   0
#define MUX_GPIO30_CANRXA   1

#define MUX_GPIO31_GPIO31   0
#define MUX_GPIO31_CANTXA   1

#define MUX_GPIO34_GPIO34   0

// GPxQSELx
#define GPXQSELX_SYNC2SYSCLKOUT 	0
#define GPXQSELX_USING_3_SAMPLES 	1
#define GPXQSELX_USING_6_SAMPLES	2
#define GPXQSELX_ASYNC				3 

//----------------------------------------------------
// GPIO A control register bit definitions */                                    
struct GPACTRL_BITS {        // bits   description
   Uint16 QUALPRD0:8;        // 7:0    Qual period 
   Uint16 QUALPRD1:8;        // 15:8   Qual period 
   Uint16 QUALPRD2:8;        // 23:16  Qual period 
   Uint16 QUALPRD3:8;        // 31:24  Qual period  
};

union GPACTRL_REG {
   Uint32              all;
   struct GPACTRL_BITS bit;
};

//----------------------------------------------------
// GPIO B control register bit definitions */                                    
struct GPBCTRL_BITS {        // bits   description
   Uint16 QUALPRD0:8;        // 7:0    Qual period
   Uint16 rsvd1:8;           // 15:8   reserved 
   Uint16 rsvd2:16;          // 31:16  reserved   
};

union GPBCTRL_REG {
   Uint32              all;
   struct GPBCTRL_BITS bit;
};

//----------------------------------------------------
// GPIO A Qual/MUX select register bit definitions */                                    
struct GPA1_BITS {            // bits   description
   Uint16 GPIO0:2;            // 1:0    GPIO0   
   Uint16 GPIO1:2;            // 3:2    GPIO1 
   Uint16 GPIO2:2;            // 5:4    GPIO2 
   Uint16 GPIO3:2;            // 7:6    GPIO3    
   Uint16 GPIO4:2;            // 9:8    GPIO4 
   Uint16 GPIO5:2;            // 11:10  GPIO5 
   Uint16 GPIO6:2;            // 13:12  GPIO6    
   Uint16 GPIO7:2;            // 15:14  GPIO7 
   Uint16 GPIO8:2;            // 17:16  GPIO8 
   Uint16 GPIO9:2;            // 19:18  GPIO9    
   Uint16 GPIO10:2;           // 21:20  GPIO10 
   Uint16 GPIO11:2;           // 23:22  GPIO11       
   Uint16 GPIO12:2;           // 25:24  GPIO12 
   Uint16 GPIO13:2;           // 27:26  GPIO13 
   Uint16 GPIO14:2;           // 29:28  GPIO14 
   Uint16 GPIO15:2;           // 31:30  GPIO15 
};


struct GPA2_BITS {            // bits   description
   Uint16 GPIO16:2;           // 1:0    GPIO16   
   Uint16 GPIO17:2;           // 3:2    GPIO17 
   Uint16 GPIO18:2;           // 5:4    GPIO18 
   Uint16 GPIO19:2;           // 7:6    GPIO19   
   Uint16 GPIO20:2;           // 9:8    GPIO20
   Uint16 GPIO21:2;           // 11:10  GPIO21 
   Uint16 GPIO22:2;           // 13:12  GPIO22    
   Uint16 GPIO23:2;           // 15:14  GPIO23
   Uint16 GPIO24:2;           // 17:16  GPIO24 
   Uint16 GPIO25:2;           // 19:18  GPIO25    
   Uint16 GPIO26:2;           // 21:20  GPIO26 
   Uint16 GPIO27:2;           // 23:22  GPIO27       
   Uint16 GPIO28:2;           // 25:24  GPIO28 
   Uint16 GPIO29:2;           // 27:26  GPIO29 
   Uint16 GPIO30:2;           // 29:28  GPIO30 
   Uint16 GPIO31:2;           // 31:30  GPIO31 
};

struct GPB1_BITS {            // bits   description
   Uint16 GPIO32:2;           // 1:0    GPIO32   
   Uint16 GPIO33:2;           // 3:2    GPIO33
   Uint16 GPIO34:2;           // 5:4    GPIO34   
   Uint16 rsvd1:2;            // 7:6    reserved  
   Uint16 rsvd2:8;            // 15:8   reserved
   Uint16 rsvd3:16;           // 31:16  reserved
};  

struct GPB2_BITS  {           // bits   description
   Uint16 rsvd1:16;           // 15:0   reserved
   Uint16 rsvd2:16;           // 31:16  reserved
};



union GPA1_REG {
   Uint32              all;
   struct GPA1_BITS    bit;
};

union GPA2_REG {
   Uint32              all;
   struct GPA2_BITS    bit;
};      

union GPB1_REG {
   Uint32              all;
   struct GPB1_BITS    bit;
};

union GPB2_REG {
   Uint32              all;
   struct GPB2_BITS    bit;
};


//----------------------------------------------------
// GPIO A DIR/TOGGLE/SET/CLEAR register bit definitions */                                    
struct GPADAT_BITS {          // bits   description
   Uint16 GPIO0:1;            // 0      GPIO0   
   Uint16 GPIO1:1;            // 1      GPIO1 
   Uint16 GPIO2:1;            // 2      GPIO2 
   Uint16 GPIO3:1;            // 3      GPIO3    
   Uint16 GPIO4:1;            // 4      GPIO4 
   Uint16 GPIO5:1;            // 5      GPIO5 
   Uint16 GPIO6:1;            // 6      GPIO6    
   Uint16 GPIO7:1;            // 7      GPIO7 
   Uint16 GPIO8:1;            // 8      GPIO8 
   Uint16 GPIO9:1;            // 9      GPIO9    
   Uint16 GPIO10:1;           // 10     GPIO10 
   Uint16 GPIO11:1;           // 11     GPIO11       
   Uint16 GPIO12:1;           // 12     GPIO12 
   Uint16 GPIO13:1;           // 13     GPIO13 
   Uint16 GPIO14:1;           // 14     GPIO14 
   Uint16 GPIO15:1;           // 15     GPIO15 
   Uint16 GPIO16:1;           // 16     GPIO16   
   Uint16 GPIO17:1;           // 17     GPIO17 
   Uint16 GPIO18:1;           // 18     GPIO18 
   Uint16 GPIO19:1;           // 19     GPIO19   
   Uint16 GPIO20:1;           // 20     GPIO20
   Uint16 GPIO21:1;           // 21     GPIO21 
   Uint16 GPIO22:1;           // 22     GPIO22    
   Uint16 GPIO23:1;           // 23     GPIO23
   Uint16 GPIO24:1;           // 24     GPIO24 
   Uint16 GPIO25:1;           // 25     GPIO25    
   Uint16 GPIO26:1;           // 26     GPIO26 
   Uint16 GPIO27:1;           // 27     GPIO27       
   Uint16 GPIO28:1;           // 28     GPIO28 
   Uint16 GPIO29:1;           // 29     GPIO29 
   Uint16 GPIO30:1;           // 30     GPIO30 
   Uint16 GPIO31:1;           // 31     GPIO31 
};

struct GPBDAT_BITS {          // bits   description
   Uint16 GPIO32:1;           // 0      GPIO32   
   Uint16 GPIO33:1;           // 1      GPIO33
   Uint16 GPIO34:1;           // 2      GPIO34   
   Uint16 rsvd1:13;           // 15:3   reserved
   Uint16 rsvd2:16;           // 31:16  reserved
};

union GPADAT_REG {
   Uint32              all;
   struct GPADAT_BITS  bit;
};

union GPBDAT_REG {
   Uint32              all;
   struct GPBDAT_BITS  bit;
};


// GPIO35 is an internal pin that is not pinned out
// for HALT low power mode, enable the internal pull-up
// to avoid extran current draw from this pin.

struct GPBPUD_BITS {          // bits   description
   Uint16 GPIO32:1;           // 0      GPIO32   
   Uint16 GPIO33:1;           // 1      GPIO33
   Uint16 GPIO34:1;           // 2      GPIO34   
   Uint16 GPIO35:1;           // 3      GPIO35  
   Uint16 rsvd1:12;           // 15:4   reserved
   Uint16 rsvd2:16;           // 31:16  reserved
};

union GPBPUD_REG {
   Uint32              all;
   struct GPBPUD_BITS  bit;
};




//----------------------------------------------------
// GPIO XINT1/XINT2/XNMI select register bit definitions */                                    
struct GPIOXINT_BITS {        // bits   description
    Uint16 GPIOSEL:5;         // 4:0    Select GPIO interrupt input source
    Uint16 rsvd1:11;          // 15:5   reserved
};

union GPIOXINT_REG {
   Uint16                all;
   struct GPIOXINT_BITS  bit;
};


struct GPIO_CTRL_REGS {
   union  GPACTRL_REG  GPACTRL;   // GPIO A Control Register (GPIO0 to 31)
   union  GPA1_REG     GPAQSEL1;  // GPIO A Qualifier Select 1 Register (GPIO0 to 15)
   union  GPA2_REG     GPAQSEL2;  // GPIO A Qualifier Select 2 Register (GPIO16 to 31)
   union  GPA1_REG     GPAMUX1;   // GPIO A Mux 1 Register (GPIO0 to 15)
   union  GPA2_REG     GPAMUX2;   // GPIO A Mux 2 Register (GPIO16 to 31)
   union  GPADAT_REG   GPADIR;    // GPIO A Direction Register (GPIO0 to 31)
   union  GPADAT_REG   GPAPUD;    // GPIO A Pull Up Disable Register (GPIO0 to 31)
   Uint32              rsvd1;
   union  GPBCTRL_REG  GPBCTRL;   // GPIO B Control Register (GPIO32 to 63)
   union  GPB1_REG     GPBQSEL1;  // GPIO B Qualifier Select 1 Register (GPIO32 to 47)
   union  GPB2_REG     GPBQSEL2;  // GPIO B Qualifier Select 2 Register (GPIO48 to 63)
   union  GPB1_REG     GPBMUX1;   // GPIO B Mux 1 Register (GPIO32 to 47)
   union  GPB2_REG     GPBMUX2;   // GPIO B Mux 2 Register (GPIO48 to 63)
   union  GPBDAT_REG   GPBDIR;    // GPIO B Direction Register (GPIO32 to 63)
   union  GPBPUD_REG   GPBPUD;    // GPIO B Pull Up Disable Register (GPIO32 to 63)
   Uint16              rsvd2[33];
};

struct GPIO_DATA_REGS {
   union  GPADAT_REG       GPADAT;       // GPIO Data Register (GPIO0 to 31)
   union  GPADAT_REG       GPASET;       // GPIO Data Set Register (GPIO0 to 31)
   union  GPADAT_REG       GPACLEAR;     // GPIO Data Clear Register (GPIO0 to 31)
   union  GPADAT_REG       GPATOGGLE;    // GPIO Data Toggle Register (GPIO0 to 31) 
   union  GPBPUD_REG       GPBDAT;       // GPIO Data Register (GPIO32 to 63)
   union  GPBDAT_REG       GPBSET;       // GPIO Data Set Register (GPIO32 to 63)
   union  GPBDAT_REG       GPBCLEAR;     // GPIO Data Clear Register (GPIO32 to 63)
   union  GPBDAT_REG       GPBTOGGLE;    // GPIO Data Toggle Register (GPIO32 to 63)
   Uint16                  rsvd1[16];
};

struct GPIO_INT_REGS {
   union  GPIOXINT_REG     GPIOXINT1SEL; // XINT1 GPIO Input Selection
   union  GPIOXINT_REG     GPIOXINT2SEL; // XINT2 GPIO Input Selection
   union  GPIOXINT_REG     GPIOXNMISEL;  // XNMI_XINT13 GPIO Input Selection
   Uint16                  rsvd2[5];
   union  GPADAT_REG       GPIOLPMSEL;   // Low power modes GP I/O input select
};      
 

//---------------------------------------------------------------------------
// GPI/O External References & Function Declarations:
//
extern volatile struct GPIO_CTRL_REGS GpioCtrlRegs;
extern volatile struct GPIO_DATA_REGS GpioDataRegs;
extern volatile struct GPIO_INT_REGS GpioIntRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP280x_GPIO_H definition

//===========================================================================
// End of file.
//===========================================================================
