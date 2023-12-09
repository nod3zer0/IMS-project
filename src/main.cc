////////////////////////////////////////////////////////////////////////////
// Model model2.cc                SIMLIB/C++
//
// Simple test model of queuing system
//

#include "simlib.h"
#include <stdlib.h> /* abs */

// global objects:

int numberOfElectricCars = 1;
int numberOfGasCars = 2;
int carCapacity = 300;
double morningShift = 5 * 60;
double afternoonShift = 4 * 60;
bool smena = true;

Facility Loading("nakladani");
Store LoadingStations("nakladaci stanice", 2);

Stat *DeliveryTime;
Stat *MorningShift;
Stat *AfternoonShift;

int *Pastry;

class Car : public Process {
  void Behavior() { // --- behavoir specification ---
    if (!smena) {
      Cancel();
    }
    Enter(LoadingStations, 1);
    if (*Pastry <= 0) {
      while (*Pastry <= 0 && smena) {
        Wait(1);
      }
      if (!smena) {
        Cancel();
      }
    } else {

      if (*Pastry <= carCapacity) {
        *Pastry = 0;
      } else {
        *Pastry -= carCapacity;
      }
    }
    double loading = Uniform(15, 20);
    Leave(LoadingStations, 1);
    double delivery = Uniform(60, 90);
    double returnTime = Uniform(10, 20);
    Wait(loading);    // nakladani
    Wait(delivery);   // rozvoz
    Wait(returnTime); // navrat
    (*DeliveryTime)(loading + delivery + returnTime);
    (new Car)->Activate();
  }
};

class Shift : public Process {
  void Behavior() { // --- behavoir specification ---

    smena = true;
    for (int i = 0; i < numberOfElectricCars + numberOfGasCars; i++) {
      (new Car)->Activate();
    }

    Wait(morningShift);

    if (*Pastry > 0) {
      printf("Dopoledne zbyva %d peciva\n", *Pastry);
      (*MorningShift)(morningShift);
    } else {
      printf("Dopoledne se rozvezlo vše\n", *Pastry);
      (*MorningShift)(DeliveryTime->Sum() /
                      (numberOfElectricCars + numberOfGasCars));
    }

    *Pastry += 5000;
    printf("Zbyva casu dopoledne (v minutách): %f\n",
           morningShift - MorningShift->Sum());
    Wait(afternoonShift);
    if (*Pastry > 0) {
      printf("Odpoledne zbyva %d peciva\n", *Pastry);
      (*AfternoonShift)(afternoonShift);
    } else {
      printf("Odpoledne se Rozvezlo vše\n", *Pastry);
      (*AfternoonShift)(
          abs((DeliveryTime->Sum() / (numberOfElectricCars + numberOfGasCars)) -
              MorningShift->Sum()));
    }
    printf("Zbyva casu odpoledne (v minutach): %f\n",
           afternoonShift - AfternoonShift->Sum());
    smena = false;
  }
};

int main() { // experiment description

  DeliveryTime = new Stat("Delivery time");
  MorningShift = new Stat("Morning shift");
  AfternoonShift = new Stat("Afternoon shift");

  Pastry = new int(2000);

  Print(" model2 - SIMLIB/C++ example\n");
  SetOutput("model2.out");
  Init(0, 1000);           // experiment initialization for time 0..1000
  (new Shift)->Activate(); // customer generator
  Run();                   // simulation

  printf("Celkem doba rozvozu v minutách na proplacení %f\n",
         DeliveryTime->Sum());
  printf("Průměrná doba rozvozu v minutách na auto %f\n",
         DeliveryTime->Sum() / (numberOfElectricCars + numberOfGasCars));

  DeliveryTime->Output();
  MorningShift->Output();
  AfternoonShift->Output();
  return 0;
}