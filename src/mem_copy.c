/**
 *  Used to copy flash code to ram space
 */
#include "global.h"

void MemCopy(Uint16* SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while (SourceAddr < SourceEndAddr) {
        *DestAddr++ = *SourceAddr++;
    }
    return;
}