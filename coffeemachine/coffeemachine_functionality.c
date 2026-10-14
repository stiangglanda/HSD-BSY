// coffeemachine_functionality.c - Implementation of the
//                   functionality of our coffee machine

#include "coffeemachine_functionality.h"

// Status store

unsigned coins_ = 0;
unsigned machine_status_ = 0x0000;


void waterContainerStatus(int status)
{
  if (!status)
  {
    machine_status_ |= WATER_EMPTY;
    display("--------------------------------------------------");
    display("please refill water container");
    display("--------------------------------------------------");
    return;
  }
  display("water refilled");
  machine_status_ &= ~WATER_EMPTY;
}

//----------------------------------------------------------------------
void coffeeContainerStatus(int status)
{
  if (!status)
  {
    machine_status_ |= COFFEE_EMPTY;
    display("--------------------------------------------------");
    display("please refill coffee container");
    display("--------------------------------------------------");
    return;
  }
  display("coffee refilled");
  machine_status_ &= ~COFFEE_EMPTY;
}

//----------------------------------------------------------------------
void milkContainerStatus(int status)
{
  if (!status)
  {
    machine_status_ |= MILK_EMPTY;
    display("--------------------------------------------------");
    display("please refill milk container");
    display("--------------------------------------------------");
    return;
  }
  display("milk refilled");
  machine_status_ &= ~MILK_EMPTY;
}

//----------------------------------------------------------------------
void sugarContainerStatus(int status)
{
  if (!status)
  {
    machine_status_ |= SUGAR_EMPTY;
    display("--------------------------------------------------");
    display("please refill sugar container");
    display("--------------------------------------------------");
    return;
  }
  display("sugar refilled");
  machine_status_ &= ~SUGAR_EMPTY;
}

//----------------------------------------------------------------------
void mugInserted()
{
  machine_status_ |= MUG_INSERTED;
  display("mug inserted, please choose your coffee");
}

//----------------------------------------------------------------------
void mugRemoved()
{
  machine_status_ &= ~MUG_INSERTED;
  display("mug removed");
}

//----------------------------------------------------------------------
void keyPressed(unsigned short keymask)
{
  unsigned int check_choice = WATER_EMPTY | COFFEE_EMPTY; // always need water and coffee

  if (machine_status_ & MACHINE_OPENED)
  {
    changePrices(keymask);
    return;
  }

  if (!(keymask & BREW)) // "brew coffee" was not pressed.. ignoring
    return;
  if (keymask & MILK)
    check_choice |= MILK_EMPTY;
  if (keymask & SUGAR)
    check_choice |= SUGAR_EMPTY;

  if (check_choice & machine_status_) // check if anything is empty
  {
    display("Sorry, not all ingredients available"); // yes, I know, this is against
              // the requirements but I was just too lazy to give a detailed message
    return;
  }
  if (!(machine_status_ & MUG_INSERTED))
  {  
    display("Sorry, you have to insert your mug first");
    return;
  }
  grindCoffee(COFFEE_AMOUNT);
  if (keymask & SUGAR)
    insertSugar(SUGAR_AMOUNT);
  if (keymask & MILK)
    insertMilk(MILK_AMOUNT);
  if (keymask & STRONG)
    brewCoffee(WATER_AMOUNT / 2);
  else
    brewCoffee(WATER_AMOUNT);
  keepCoins();
  coins_ = 0;
  // price display and price-checking not implemented... something for
  // you to play around with.
  display("Something for you to play around with:");
  display("Checking the price and displaying the price");
  display("is not implemented yet");

}

//----------------------------------------------------------------------
void coinInserted(unsigned value)
{
  display("coin inserted");
  coins_ += value;
}

//----------------------------------------------------------------------
void machineOpened()
{
  display("machine open - changing to price-balance mode");
  machine_status_ |= MACHINE_OPENED;
}

//----------------------------------------------------------------------
void machineClosed()
{
  display("machine closed - normal coffee mode again");
  machine_status_ &= ~MACHINE_OPENED;
}

//----------------------------------------------------------------------
void changePrices(unsigned keymask)
{
  display("not yet implemented, that's one for you to play around");
  display("with the program to get a feeling for it");
}
