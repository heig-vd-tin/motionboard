#include "IQmathLib.h"

int _IQshift(_iq x)
{
    _iq val1, val2, val3;
    int shift = 0;
    val2 = x;
    do {
        shift++;
        val1 = val2;
        val2 = val1 << 1;
        val3 = val2 >> 1;
    } while (val3 == val1);
    return shift - 1;
}

int _IQ16shift(_iq x)
{
    volatile _iq val;
    int shift = 0;
    val = x;
    if (val == 0) return 0;
    if (val < 65535) {
        do {
            shift++;
            val = val << 1;
        } while (val <= 65535);
        return shift - 1;
    }
    do {
        shift--;
        val = val >> 1;
    } while (val >= 65535);
    return shift;
}

int shiftA, shiftB;
_iq result, aa, bb;
_iq tmpp, tmpp2, tmpp3;

////////////////////////////////////////////////////////////////////////////////
/// Multiply A and B with Normalization and Saturation
////////////////////////////////////////////////////////////////////////////////
_iq _IQmpyS(_iq xa, int qa, _iq xb, int qb)
{
    shiftA = _IQ16shift(xa);
    shiftB = _IQ16shift(xb);
    aa = (shiftA > 0) ? xa << shiftA : xa >> (shiftA - 2 * shiftA);
    bb = (shiftB > 0) ? xb << shiftB : xb >> (shiftB - 2 * shiftB);
    return _IQmpy(aa, bb) << (30 - shiftA - shiftB - qa - qb);
}
