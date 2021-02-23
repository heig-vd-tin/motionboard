// TI File $Revision: /main/5 $
// Checkin $Date: August 2, 2006   16:54:21 $
//###########################################################################
//
// FILE:   DSP280x_GlobalPrototypes.h
//
// TITLE:  Global prototypes for DSP280x Examples
// 
//###########################################################################
// $TI Release: DSP280x, DSP2801x Header Files V1.41 $
// $Release Date: August 7th, 2006 $
//###########################################################################
#ifndef DSP280X_GLOBALPROTOTYPES_H
#define DSP280X_GLOBALPROTOTYPES_H

extern void InitSysCtrl(void);
extern void InitPeripheralClocks(void);
extern void DisableDog(void);
extern void InitPll(Uint16 pllcr, Uint16 clkindiv);
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);
extern void EnableInterrupts(void); 
extern void InitFlash(void);
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);


//---------------------------------------------------------------------------
// External symbols created by the linker cmd file
// DSP28 examples will use these to relocate code from one LOAD location 
// in either Flash or XINTF to a different RUN location in internal
// RAM
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

#endif   

//===========================================================================
// End of file.
//===========================================================================

