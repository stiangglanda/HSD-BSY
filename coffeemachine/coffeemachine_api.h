// coffeemachine_api.h - declarations for users of the API

#ifndef __coffeemachine_api_h__
#define __coffeemachine_api_h__

#define BUTTON_1       0x01
#define BUTTON_2       0x02
#define BUTTON_3       0x04
#define BUTTON_4       0x08


// declarations of functions that the API Coffee-O'Prog V 0.7 provides

void activateCoffeeMachine();
void grindCoffee(unsigned amount);
void insertSugar(unsigned amount);
void insertMilk(unsigned amount);
void brewCoffee(unsigned amount);
void returnCoins();
void keepCoins();
void display(char message[]);

// declarations of callback functions that are needed by the
// API Coffee-O'Prog V 0.7.

void waterContainerStatus(int status);
void coffeeContainerStatus(int status);
void milkContainerStatus(int status);
void sugarContainerStatus(int status);
void mugInserted();
void mugRemoved();
void keyPressed(unsigned short keymask);
void coinInserted(unsigned value);
void machineOpened();
void machineClosed();


// declarations of internal API functions

void dispatch(unsigned char kbd_input);
void displayHelp();

// declarations of internal constants

#define _EMPTY_ 0
#define _FULL_  1

#define _INITIAL_WATER_AMOUNT_     500
#define _INITIAL_COFFEE_AMOUNT_     50
#define _INITIAL_MILK_AMOUNT_       90
#define _INITIAL_SUGAR_AMOUNT_       7

// status flags

#define _FRONT_OPEN_       0x0001
#define _MUG_INSERTED_     0x0002

#endif // __coffeemachine_api_h__

