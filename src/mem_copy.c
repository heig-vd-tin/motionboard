/**
 * @title   MotionBoard project v2.0
 * @file    mem_copy.c
 * @brief   Used to copy flash code to ram space
 * @author  Yves Chevallier <yves.chevallier@kalios.ch>
 * @svn     $Id: mem_copy.c 490 2008-11-15 14:24:53Z yves $
 */


////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "global.h"

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    { 
       *DestAddr++ = *SourceAddr++;
    }
    return;
}

//===========================================================================
// End of file.
//===========================================================================
