// coffeemachine_functionality.h - declarations and
//              constants needed by our implementation

#ifndef __coffeemachine_functionality_h__
#define __coffeemachine_functionality_h__

#include "coffeemachine_api.h"

// Constants

#define WATER_EMPTY    0x0001
#define COFFEE_EMPTY   0x0002
#define MILK_EMPTY     0x0004
#define SUGAR_EMPTY    0x0008
#define MUG_INSERTED   0x0010
#define MACHINE_OPENED 0x0020

#define STRONG   BUTTON_1
#define MILK     BUTTON_2
#define SUGAR    BUTTON_3
#define BREW     BUTTON_4

#define COFFEE_AMOUNT    5
#define WATER_AMOUNT   125
#define SUGAR_AMOUNT     1
#define MILK_AMOUNT     30

// declarations of program-internal functions

void changePrices(unsigned keymask);



#endif // __coffeemachine_functionality_h__

