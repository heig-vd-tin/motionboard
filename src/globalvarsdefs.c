#include "device.h"

#pragma DATA_SECTION(AdcRegs, "AdcRegsFile");
volatile struct ADC_REGS AdcRegs;

#pragma DATA_SECTION(AdcMirror, "AdcMirrorFile");
volatile struct ADC_RESULT_MIRROR_REGS AdcMirror;

#pragma DATA_SECTION(CpuTimer0Regs, "CpuTimer0RegsFile");
volatile struct CPUTIMER_REGS CpuTimer0Regs;

#pragma DATA_SECTION(CpuTimer1Regs, "CpuTimer1RegsFile");
volatile struct CPUTIMER_REGS CpuTimer1Regs;

#pragma DATA_SECTION(CpuTimer2Regs, "CpuTimer2RegsFile");
volatile struct CPUTIMER_REGS CpuTimer2Regs;

#pragma DATA_SECTION(CsmPwl, "CsmPwlFile");
volatile struct CSM_PWL CsmPwl;

#pragma DATA_SECTION(CsmRegs, "CsmRegsFile");
volatile struct CSM_REGS CsmRegs;

#pragma DATA_SECTION(DevEmuRegs, "DevEmuRegsFile");
volatile struct DEV_EMU_REGS DevEmuRegs;

#pragma DATA_SECTION(ECanaRegs, "ECanaRegsFile");
volatile struct ECAN_REGS ECanaRegs;

#pragma DATA_SECTION(ECanaMboxes, "ECanaMboxesFile");
volatile struct ECAN_MBOXES ECanaMboxes;

#pragma DATA_SECTION(ECanaLAMRegs, "ECanaLAMRegsFile");
volatile struct LAM_REGS ECanaLAMRegs;

#pragma DATA_SECTION(ECanaMOTSRegs, "ECanaMOTSRegsFile");
volatile struct MOTS_REGS ECanaMOTSRegs;

#pragma DATA_SECTION(ECanaMOTORegs, "ECanaMOTORegsFile");
volatile struct MOTO_REGS ECanaMOTORegs;

#pragma DATA_SECTION(ECanbRegs, "ECanbRegsFile");
volatile struct ECAN_REGS ECanbRegs;

#pragma DATA_SECTION(ECanbMboxes, "ECanbMboxesFile");
volatile struct ECAN_MBOXES ECanbMboxes;

#pragma DATA_SECTION(ECanbLAMRegs, "ECanbLAMRegsFile");
volatile struct LAM_REGS ECanbLAMRegs;

#pragma DATA_SECTION(ECanbMOTSRegs, "ECanbMOTSRegsFile");
volatile struct MOTS_REGS ECanbMOTSRegs;

#pragma DATA_SECTION(ECanbMOTORegs, "ECanbMOTORegsFile");
volatile struct MOTO_REGS ECanbMOTORegs;

#pragma DATA_SECTION(EPwm1Regs, "EPwm1RegsFile");
volatile struct EPWM_REGS EPwm1Regs;

#pragma DATA_SECTION(EPwm2Regs, "EPwm2RegsFile");
volatile struct EPWM_REGS EPwm2Regs;

#pragma DATA_SECTION(EPwm3Regs, "EPwm3RegsFile");
volatile struct EPWM_REGS EPwm3Regs;

#pragma DATA_SECTION(EPwm4Regs, "EPwm4RegsFile");
volatile struct EPWM_REGS EPwm4Regs;

#pragma DATA_SECTION(EPwm5Regs, "EPwm5RegsFile");
volatile struct EPWM_REGS EPwm5Regs;

#pragma DATA_SECTION(EPwm6Regs, "EPwm6RegsFile");
volatile struct EPWM_REGS EPwm6Regs;

#pragma DATA_SECTION(ECap1Regs, "ECap1RegsFile");
volatile struct ECAP_REGS ECap1Regs;

#pragma DATA_SECTION(ECap2Regs, "ECap2RegsFile");
volatile struct ECAP_REGS ECap2Regs;

#pragma DATA_SECTION(ECap3Regs, "ECap3RegsFile");
volatile struct ECAP_REGS ECap3Regs;

#pragma DATA_SECTION(ECap4Regs, "ECap4RegsFile");
volatile struct ECAP_REGS ECap4Regs;

#pragma DATA_SECTION(EQep1Regs, "EQep1RegsFile");
volatile struct EQEP_REGS EQep1Regs;

#pragma DATA_SECTION(EQep2Regs, "EQep2RegsFile");
volatile struct EQEP_REGS EQep2Regs;

#pragma DATA_SECTION(GpioCtrlRegs, "GpioCtrlRegsFile");
volatile struct GPIO_CTRL_REGS GpioCtrlRegs;

#pragma DATA_SECTION(GpioDataRegs, "GpioDataRegsFile");
volatile struct GPIO_DATA_REGS GpioDataRegs;

#pragma DATA_SECTION(GpioIntRegs, "GpioIntRegsFile");
volatile struct GPIO_INT_REGS GpioIntRegs;

#pragma DATA_SECTION(I2caRegs, "I2caRegsFile");
volatile struct I2C_REGS I2caRegs;

#pragma DATA_SECTION(PieCtrlRegs, "PieCtrlRegsFile");
volatile struct PIE_CTRL_REGS PieCtrlRegs;

#pragma DATA_SECTION(PieVectTable, "PieVectTableFile");
struct PIE_VECT_TABLE PieVectTable;

#pragma DATA_SECTION(SciaRegs, "SciaRegsFile");
volatile struct SCI_REGS SciaRegs;

#pragma DATA_SECTION(ScibRegs, "ScibRegsFile");
volatile struct SCI_REGS ScibRegs;

#pragma DATA_SECTION(SpiaRegs, "SpiaRegsFile");
volatile struct SPI_REGS SpiaRegs;

#pragma DATA_SECTION(SpibRegs, "SpibRegsFile");
volatile struct SPI_REGS SpibRegs;

#pragma DATA_SECTION(SpicRegs, "SpicRegsFile");
volatile struct SPI_REGS SpicRegs;

#pragma DATA_SECTION(SpidRegs, "SpidRegsFile");
volatile struct SPI_REGS SpidRegs;

#pragma DATA_SECTION(SysCtrlRegs, "SysCtrlRegsFile");
volatile struct SYS_CTRL_REGS SysCtrlRegs;

#pragma DATA_SECTION(FlashRegs, "FlashRegsFile");
volatile struct FLASH_REGS FlashRegs;

#pragma DATA_SECTION(XIntruptRegs, "XIntruptRegsFile");
volatile struct XINTRUPT_REGS XIntruptRegs;
