////////////////////////////////////////////////////////////////////////////
// Model model2.cc                SIMLIB/C++
//
// Simple test model of queuing system
//

#include "inc/Shift.h"
#include "inc/argument_helper_functions.h"
#include "simlib.h"
#include <ctime>
#include <math.h>
#include <stdlib.h> /* abs */

// global objects:

int morningBoxes = 104 * 10;
int afternoonBoxes = 104 * 10;
int numberOfElectricCars = 1;
int numberOfGasCars = 2;
int carCapacity = 104;
double morningShift = 5 * 60;
double afternoonShift = 4 * 60;
bool shift = true;
int numberOfDaysUntilGasEmpty = 5;
int solarPanelOutput = 50; // kW

Facility Loading("nakladani");
Store LoadingStations("nakladaci stanice", 3);

Stat *DeliveryTime;
Stat *FuelUsage;
Stat *BateryUsage;
Stat *MorningShift;
Stat *AfternoonShift;
Stat *OverTime;

int *Boxes;

// class ElectricCar : public Process {
//   void Behavior() { // --- behavoir specification ---

//     if (!shift) { // if shift is over cancel
//       double Overtime = Time - morningShift - afternoonShift;
//       if (Overtime > 0)
//         (*OverTime)(Overtime);
//       Cancel();
//     }

//     if (*Boxes <= 0) { // if there is no boxes wait until there is
//       while (_WaitUntil(*Boxes > 0 || !shift))
//         ;
//       if (!shift) { // if shift is over cancel
//         double Overtime = Time - morningShift - afternoonShift;
//         if (Overtime > 0)
//           (*OverTime)(Overtime);
//         Cancel();
//       }
//     } else {

//       if (*Boxes <=
//           carCapacity) { // if there is less boxes than car capacity take all
//         *Boxes = 0;
//       } else {
//         *Boxes -= carCapacity; // else take car capacity
//       }
//     }
//     Enter(LoadingStations, 1);
//     double loading = Uniform(30, 50);
//     Wait(loading); // laoding
//     Leave(LoadingStations, 1);
//     double delivery = Uniform(60, 2 * 60);
//     Wait(delivery); // delivery
//     double returnTime = Uniform(30, 60);
//     Wait(returnTime); // return
//     (*DeliveryTime)(loading + delivery + returnTime);
//     (*BateryUsage)(
//         ((((loading + delivery + returnTime) / 60 * Uniform(30, 90))) / 100)
//         * Uniform(16, 24)); // 20kw  100 km
//     (new ElectricCar)->Activate();
//   }
// };

// class GasCar : public Process {
//   void Behavior() {
//     if (!shift) { // if shift is over cancel

//       double Overtime = Time - morningShift - afternoonShift;
//       if (Overtime > 0)
//         (*OverTime)(Overtime);

//       Cancel();
//     }

//     if (*Boxes <= 0) { // if there is no boxes wait until there is
//       while (_WaitUntil(*Boxes > 0 || !shift))
//         ;
//       if (!shift) { // if shift is over cancel
//         double Overtime = Time - morningShift - afternoonShift;
//         if (Overtime > 0)
//           (*OverTime)(Overtime);
//         Cancel();
//       }
//     } else {

//       if (*Boxes <= carCapacity) { // if there is less boxes than car
//       capacity
//                                    // take all
//         *Boxes = 0;
//       } else { // else take car capacity
//         *Boxes -= carCapacity;
//       }
//     }

//     Enter(LoadingStations, 1);
//     double loading = Uniform(30, 50);
//     Wait(loading); // loading
//     Leave(LoadingStations, 1);
//     double delivery = Uniform(60, 2 * 60);
//     Wait(delivery); // delivery
//     double returnTime = Uniform(30, 60);
//     Wait(returnTime); // return
//     (*DeliveryTime)(loading + delivery + returnTime);
//     (*FuelUsage)(
//         (((loading + delivery + returnTime) / 60 * Uniform(30, 90)) / 100) *
//         Uniform(8, 12)); // 10l 100km
//     (new GasCar)->Activate();
//   }
// };

// class GasCarStart : public Process {
//   void Behavior() {
//     if ((Random() * numberOfDaysUntilGasEmpty) >=
//         numberOfDaysUntilGasEmpty - 1) { // car will last in average 5
//                                          // days with one tank
//       double refueling = Uniform(20, 80);
//       Wait(refueling);
//     }
//     (new GasCar)->Activate();
//   }
// };

// class Shift : public Process {
//   void Behavior() {

//     shift = true;
//     for (int i = 0; i < numberOfElectricCars; i++) { // start electric cars
//       (new ElectricCar)->Activate();
//     }

//     for (int i = 0; i < numberOfGasCars; i++) { // start gas cars
//       (new GasCarStart)->Activate();
//     }

//     Wait(morningShift); // wait for morning shift to end

//     if (*Boxes > 0) {
//       printf("Dopoledne zbyva %d beden\n", *Boxes);
//       printf("Rozvezlo se %d beden\n", morningBoxes - *Boxes);
//       (*MorningShift)(morningShift);
//     } else {
//       printf("Dopoledne se rozvezlo vše\n");
//       printf("Rozvezlo se %d beden\n", morningBoxes - *Boxes);
//       (*MorningShift)(DeliveryTime->Sum() /
//                       (numberOfElectricCars + numberOfGasCars));
//     }

//     *Boxes = afternoonBoxes; // set new boxes for afternoon shift

//     printf("Zbyva casu dopoledne (v minutách): %f\n",
//            morningShift - MorningShift->Sum());
//     Wait(afternoonShift); // wait for afternoon shift to end
//     if (*Boxes > 0) {
//       printf("Odpoledne zbyva %d beden\n", *Boxes);
//       printf("Rozvezlo se %d beden\n", afternoonBoxes - *Boxes);
//       (*AfternoonShift)(afternoonShift);
//     } else {
//       printf("Odpoledne se Rozvezlo vše\n");
//       printf("Rozvezlo se %d beden\n", afternoonBoxes - *Boxes);
//       (*AfternoonShift)(
//           abs((DeliveryTime->Sum() / (numberOfElectricCars +
//           numberOfGasCars)) -
//               MorningShift->Sum()));
//     }
//     printf("Zbyva casu odpoledne (v minutach): %f\n",
//            afternoonShift - AfternoonShift->Sum());
//     shift = false; // shift is over
//   }
// };

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
  OverTime = new Stat("Overtime");

  Boxes = new int(morningBoxes);

  Print(" model2 - SIMLIB/C++ example\n");
  SetOutput("model2.out");
  Init(0, morningShift + afternoonShift + 100);
  (new Shift(MorningShift, AfternoonShift, Boxes, afternoonBoxes, morningBoxes,
             &shift, DeliveryTime, FuelUsage, BateryUsage, OverTime,
             &LoadingStations, numberOfElectricCars, numberOfGasCars))
      ->Activate();
  Run(); // simulation

  printf("Celkem doba rozvozu v minutách: %f\n", DeliveryTime->Sum());
  printf("Průměrná doba rozvozu v minutách na auto: %f\n",
         DeliveryTime->Sum() / (numberOfElectricCars + numberOfGasCars));

  double solarPanelOutputNow = solarPanelOutput * Uniform(0, 1);
  printf("Vyrobeno solárnímy panely: %f Kw\n", solarPanelOutputNow);

  printf("Celkem spotřebováno paliva: %f l\n", FuelUsage->Sum());

  printf("Celkem spotřebováno energie: %f Kw\n", BateryUsage->Sum());

  printf("Spotřebovaná energie (odečteny solární panely): %f Kw\n",
         BateryUsage->Sum() - solarPanelOutputNow);

  printf("Celkem přesčasů: %f minut\n", OverTime->Sum());

  DeliveryTime->Output();
  MorningShift->Output();
  AfternoonShift->Output();
  FuelUsage->Output();
  BateryUsage->Output();
  OverTime->Output();
  return 0;
}