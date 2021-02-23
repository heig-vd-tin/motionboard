MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */
   
   BEGIN      : origin = 0x000000, length = 0x000002             
   RAMM0      : origin = 0x000002, length = 0x0003FE
   RAMLX      : origin = 0x008000, length = 0x002000    
   RESET      : origin = 0x3FFFC0, length = 0x000002
   BOOTROM    : origin = 0x3FF000, length = 0x000FC0               

         
PAGE 1 : 

   BOOT_RSVD   : origin = 0x000400, length = 0x000080     /* Part of M1, BOOT rom will use this for stack */
   RAMM1       : origin = 0x000480, length = 0x000380     /* on-chip RAM block M1 */
/*   RAML1       : origin = 0x009000, length = 0x001000     */
}
 
 
SECTIONS
{
   /* Setup for "boot to SARAM" mode: 
      The codestart section (found in DSP28_CodeStartBranch.asm)
      re-directs execution to the start of user code.  */
   codestart        : > BEGIN,     PAGE = 0
   ramfuncs         : > RAMM0      PAGE = 0  
   .text            : > RAMLX,     PAGE = 0
   .cinit           : > RAMM0,     PAGE = 0
   .pinit           : > RAMM0,     PAGE = 0
   .switch          : > RAMM0,     PAGE = 0
   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */
   
   .stack           : > RAMM1,     PAGE = 1
   .ebss            : > RAMLX,     PAGE = 0
   .econst          : > RAMLX,     PAGE = 0      
   .esysmem         : > RAMM1,     PAGE = 1


   IQmath           : > RAMLX,     PAGE = 0
   IQmathTables     : > BOOTROM,   type = NOLOAD, PAGE = 0

     
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/

