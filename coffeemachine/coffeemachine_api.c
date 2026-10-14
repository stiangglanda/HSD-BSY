//----------------------------------------------------------------------
// coffeemachine_api.c - Implementation of the Coffee-O'Prog V 0.7 API

#include <stdio.h>
#include <stdlib.h>
#include "coffeemachine_api.h"

// internal variables

unsigned _status_ = 0x0000;
unsigned _water_ =    _INITIAL_WATER_AMOUNT_;
unsigned _coffee_ =   _INITIAL_COFFEE_AMOUNT_;
unsigned _milk_ =     _INITIAL_MILK_AMOUNT_;
unsigned _sugar_ =    _INITIAL_SUGAR_AMOUNT_;



//----------------------------------------------------------------------
// The main function that activates the machine. Here the endless loop
// for the keyboard input dispatcher of the simulator is located.

void activateCoffeeMachine()
{
  unsigned char kbd_input = '\0';

  for(;;)
  {
    printf("Coffee Machine> ");
    kbd_input = getchar();
    getchar(); // eat the newline character and ignore it
    dispatch(kbd_input);
  }
}

//----------------------------------------------------------------------
void grindCoffee(unsigned amount)
{
  if (_coffee_ >= 0) // see assumption in the SRD
  {
    printf("***** grinding %u grams of coffee\n",amount);
    _coffee_ = (amount <= _coffee_) ? (_coffee_ - amount) : 0;
    if (!_coffee_)
      coffeeContainerStatus(_EMPTY_);
    return;
  }
  printf("*** Oops - you tried to grind coffee, but there was none left!\n");
}

//----------------------------------------------------------------------
void insertSugar(unsigned amount)
{
  if (_sugar_ >= 0) // see assumption in the SRD
  {
    printf("***** inserting %u spoons of sugar\n",amount);
    _sugar_ = (amount <= _sugar_) ? (_sugar_ - amount) : 0;
    if (!_sugar_)
      sugarContainerStatus(_EMPTY_);
    return;
  }
  printf("*** Oops - you tried to put some sugar in the coffee, but there was none left!\n");
}

//----------------------------------------------------------------------
void insertMilk(unsigned amount)
{
  if (_milk_ >= 0) // see assumption in the SRD
  {
    printf("***** inserting %u milliliters of milk\n",amount);
    _milk_ = (amount <= _milk_) ? (_milk_ - amount) : 0;
    if (!_milk_)
      milkContainerStatus(_EMPTY_);
    return;
  }
  printf("*** Oops - you tried to put some milk in the coffee, but there was none left!\n");
}

//----------------------------------------------------------------------
void brewCoffee(unsigned amount)
{
  if (_water_ >= 0) // see assumption in the SRD
  {
    printf("***** brewing coffee with %u milliliters of water\n",amount);
    _water_ = (amount <= _water_) ? (_water_ - amount) : 0;
    if (!_water_)
      waterContainerStatus(_EMPTY_);
    return;
  }
  printf("*** Oops - you tried to brew coffee, but there was no water left!\n");
}

//----------------------------------------------------------------------
void returnCoins()
{
  printf("ok, you get your coins back\n");
}

//----------------------------------------------------------------------
void keepCoins()
{
  printf("thanks for the coins, enjoy your coffee\n");
}

//----------------------------------------------------------------------
void display(char message[])
{
  printf("### %s\n",message);
}


//----------------------------------------------------------------------
// dispatches the keyboard input

void dispatch(unsigned char kbd_input)
{
  switch(kbd_input)
  {
    case 'h':
      displayHelp();
      break;
    case 'P':
      if (!(_status_ & _FRONT_OPEN_))
      {
        _status_ |= _FRONT_OPEN_;
        machineOpened();
      }
      break;
    case 'p':
      if (_status_ & _FRONT_OPEN_)
      {
        _status_ &= ~_FRONT_OPEN_;
        machineClosed();
      }
      break;
    case 'i':
      coinInserted(1); // I know, the URD says 1, 5 and 10 S coins.. I was just
                       // too lazy to implement it.
      break;
    case 'm':
      if (!(_status_ & _MUG_INSERTED_))
      {
        _status_ |= _MUG_INSERTED_;
        mugInserted();
      }
      break;
    case 'r':
      if (_status_ & _MUG_INSERTED_)
      {
        _status_ &= ~_MUG_INSERTED_;
        mugRemoved();
      }
      break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      keyPressed((unsigned short)(kbd_input - '0'));
      break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
      keyPressed((unsigned short)(kbd_input - 'a' + 10));
      break;
    case 'W':
      if (_water_ <= 0)
      {
        _water_ = _INITIAL_WATER_AMOUNT_;
        waterContainerStatus(_FULL_);
      }
      break;
    case 'C':
      if (_coffee_ <= 0)
      {
        _coffee_ = _INITIAL_WATER_AMOUNT_;
        coffeeContainerStatus(_FULL_);
      }
      break;
    case 'M':
      if (_milk_ <= 0)
      {
        _milk_ = _INITIAL_WATER_AMOUNT_;
        milkContainerStatus(_FULL_);
      }
      break;
    case 'S':
      if (_sugar_ <= 0)
      {
        _sugar_ = _INITIAL_WATER_AMOUNT_;
        sugarContainerStatus(_FULL_);
      }
      break;
    case 'q':
      exit(0);
      break;
    default:
      printf("unknown keyboard input: %c\n",kbd_input);
      break;
  }
}

//----------------------------------------------------------------------
// displays help for operating the simulator

void displayHelp()
{
  printf("\n");
  printf("The following input is supported:\n");
  printf("h ... displays this help text\n");
  printf("P ... open front panel\n");
  printf("p ... close front panel\n");
  printf("i ... insert coin\n");
  printf("m ... insert mug\n");
  printf("r ... remove mug\n");
  printf("1 ... press button 1 (strong)\n");
  printf("2 ... press button 2 (milk)\n");
  printf("4 ... press button 3 (sugar)\n");
  printf("8 ... press button 4 (brew coffee)\n");
  printf("      if you want to press more than one button, just add the values in hex!!\n");
  printf("      e.g. if you want strong coffee with milk this is b\n");
  printf("W ... fill in water\n");
  printf("C ... fill in coffee\n");
  printf("M ... fill in milk\n");
  printf("S ... fill in sugar\n");
  printf("q ... quit\n");
  printf("\n");
}

