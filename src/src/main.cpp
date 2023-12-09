////////////////////////////////////////////////////////////////////////////
// Model model2.cc                SIMLIB/C++
//
// Simple test model of queuing system
//

#include "inc/argument_helper_functions.h"
#include "simlib.h"
#include <ctime>
#include <stdlib.h> /* abs */

// global objects:

int morningBoxes = 104 * 10;
int afternoonBoxes = 104 * 10;
int numberOfElectricCars = 1;
int numberOfGasCars = 2;
int carCapacity = 104;
double morningShift = 5 * 60;
double afternoonShift = 4 * 60;
bool smena = true;
bool numberOfDaysUntilGasEmpty = 5;
int solarPanelOutput = 50; // kW

Facility Loading("nakladani");
Store LoadingStations("nakladaci stanice", 3);

Stat *DeliveryTime;
Stat *FuelUsage;
Stat *BateryUsage;
Stat *MorningShift;
Stat *AfternoonShift;

int *Boxes;

class Car : public Process {
  void Behavior() { // --- behavoir specification ---
    if (!smena) {
      Cancel();
    }
    Enter(LoadingStations, 1);
    if (*Boxes <= 0) {
      while (_WaitUntil(*Boxes > 0 || !smena))
        ;
      if (!smena) {
        Cancel();
      }
    } else {

      if (*Boxes <= carCapacity) {
        *Boxes = 0;
      } else {
        *Boxes -= carCapacity;
      }
    }
    double loading = Uniform(30, 50);
    Wait(loading); // nakladani
    Leave(LoadingStations, 1);
    double delivery = Uniform(60, 2 * 60);
    Wait(delivery); // rozvoz
    double returnTime = Uniform(30, 60);
    Wait(returnTime); // navrat
    (*DeliveryTime)(loading + delivery + returnTime);
    (*BateryUsage)(
        ((((loading + delivery + returnTime) / 60 * Uniform(30, 90))) / 100) *
        Uniform(16, 24)); // 20kw na 100kilometr
    (new Car)->Activate();
  }
};

class GasCar : public Process {
  void Behavior() { // --- behavoir specification ---
    if (!smena) {
      Cancel();
    }
    Enter(LoadingStations, 1);
    if (*Boxes <= 0) {
      while (_WaitUntil(*Boxes > 0 || !smena))
        ;
      if (!smena) {
        Cancel();
      }
    } else {

      if (*Boxes <= carCapacity) {
        *Boxes = 0;
      } else {
        *Boxes -= carCapacity;
      }
    }
    double loading = Uniform(30, 50);

    Wait(loading); // nakladani
    Leave(LoadingStations, 1);
    double delivery = Uniform(60, 2 * 60);
    Wait(delivery); // rozvoz
    double returnTime = Uniform(30, 60);
    Wait(returnTime); // navrat
    (*DeliveryTime)(loading + delivery + returnTime);
    (*FuelUsage)(
        (((loading + delivery + returnTime) / 60 * Uniform(30, 90)) / 100) *
        Uniform(8, 12));
    // spotřeba paliva normální rozložení mezi
    // 50kmh a 90kmh (10l na 100km)
    (new GasCar)->Activate();
  }
};

class GasCarStart : public Process {
  void Behavior() { // --- behavoir specification ---
    if (int(Random() * numberOfDaysUntilGasEmpty) ==
        numberOfDaysUntilGasEmpty) {
      double refueling = Uniform(20, 80);
      Wait(refueling);
    }
    (new GasCar)->Activate();
  }
};

class Shift : public Process {
  void Behavior() { // --- behavoir specification ---

    smena = true;
    for (int i = 0; i < numberOfElectricCars; i++) {
      (new Car)->Activate();
    }

    for (int i = 0; i < numberOfGasCars; i++) {
      (new GasCarStart)->Activate();
    }

    Wait(morningShift);

    if (*Boxes > 0) {
      printf("Dopoledne zbyva %d beden\n", *Boxes);
      printf("Rozvezlo se %d beden\n", morningBoxes - *Boxes);
      (*MorningShift)(morningShift);
    } else {
      printf("Dopoledne se rozvezlo vše\n");
      printf("Rozvezlo se %d beden\n", morningBoxes - *Boxes);
      (*MorningShift)(DeliveryTime->Sum() /
                      (numberOfElectricCars + numberOfGasCars));
    }

    *Boxes = afternoonBoxes;

    printf("Zbyva casu dopoledne (v minutách): %f\n",
           morningShift - MorningShift->Sum());
    Wait(afternoonShift);
    if (*Boxes > 0) {
      printf("Odpoledne zbyva %d beden\n", *Boxes);
      printf("Rozvezlo se %d beden\n", afternoonBoxes - *Boxes);
      (*AfternoonShift)(afternoonShift);
    } else {
      printf("Odpoledne se Rozvezlo vše\n");
      printf("Rozvezlo se %d beden\n", afternoonBoxes - *Boxes);
      (*AfternoonShift)(
          abs((DeliveryTime->Sum() / (numberOfElectricCars + numberOfGasCars)) -
              MorningShift->Sum()));
    }
    printf("Zbyva casu odpoledne (v minutach): %f\n",
           afternoonShift - AfternoonShift->Sum());
    smena = false;
  }
};

int main(int argc, const char **argv) { // experiment description

  argsT args = parseArguments(argc, argv);

  if (args.err) {
    fprintf(stderr, "Error while parsing arguments\n");
    return 1;
  }

  morningBoxes = args.morning_boxes;
  afternoonBoxes = args.afternoon_boxes;
  numberOfElectricCars = args.electric_cars;
  numberOfGasCars = args.gas_cars;

  RandomSeed(time(nullptr));
  DeliveryTime = new Stat("Delivery time");
  MorningShift = new Stat("Morning shift");
  AfternoonShift = new Stat("Afternoon shift");
  FuelUsage = new Stat("Fuel usage");
  BateryUsage = new Stat("Batery usage");

  Boxes = new int(morningBoxes);

  Print(" model2 - SIMLIB/C++ example\n");
  SetOutput("model2.out");
  Init(0, morningShift + afternoonShift +
              100);        // experiment initialization for time 0..1000
  (new Shift)->Activate(); // customer generator
  Run();                   // simulation

  printf("Celkem doba rozvozu v minutách na proplacení %f\n",
         DeliveryTime->Sum());
  printf("Průměrná doba rozvozu v minutách na auto %f\n",
         DeliveryTime->Sum() / (numberOfElectricCars + numberOfGasCars));

  double solarPanelOutputNow = solarPanelOutput * Uniform(0, 1);
  printf("Vyrobeno solárnímy panely: %f\n", solarPanelOutputNow);

  printf("Celkem spotřebováno paliva %f\n", FuelUsage->Sum());

  printf("Celkem spotřebováno energie %f\n", BateryUsage->Sum());

  printf("Spotřebovaná energie (odečteny solární panely) %f\n",
         BateryUsage->Sum() - solarPanelOutputNow);

  DeliveryTime->Output();
  MorningShift->Output();
  AfternoonShift->Output();
  FuelUsage->Output();
  BateryUsage->Output();
  return 0;
}