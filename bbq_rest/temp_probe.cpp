#include <math.h>
#include <string.h>
#include "temp_probe.h"
//Did not find atomic.h
//#include <util/atomic.h>
//#include <digitalWriteFast.h>

TempProbe::TempProbe(const unsigned char pin) :
  _pin(pin), _tempStatus(TSTATUS_NONE)
{
}

