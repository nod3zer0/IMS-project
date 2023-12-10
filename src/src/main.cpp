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

int main(int argc, const char **argv) { // experiment description

  // default values
  int morningBoxes = 104 * 10;
  int afternoonBoxes = 104 * 10;
  int numberOfElectricCars = 1;
  int numberOfGasCars = 2;
  double morningShift = 5 * 60;
  double afternoonShift = 4 * 60;
  bool shift = true;
  int solarPanelOutput = 50; // kW

  Store LoadingStations("nakladaci stanice", 3);

  Stat *DeliveryTime;
  Stat *FuelUsage;
  Stat *BateryUsage;
  Stat *MorningShift;
  Stat *AfternoonShift;
  Stat *OverTime;
  Stat *ShiftEndsEarly;

  DeliveryTime = new Stat("Delivery time");
  MorningShift = new Stat("Morning shift");
  AfternoonShift = new Stat("Afternoon shift");
  FuelUsage = new Stat("Fuel usage");
  BateryUsage = new Stat("Batery usage");
  OverTime = new Stat("Overtime");
  ShiftEndsEarly = new Stat("Shift ends early");

  int *Boxes;

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

  Boxes = new int(morningBoxes);

  Print("Model pekárny v Křižanov\n");
  SetOutput("model.out");
  Init(0, morningShift + afternoonShift + 100);
  (new Shift(MorningShift, AfternoonShift, Boxes, afternoonBoxes, morningBoxes,
             &shift, DeliveryTime, FuelUsage, BateryUsage, OverTime,
             &LoadingStations, numberOfElectricCars, numberOfGasCars,
             ShiftEndsEarly))
      ->Activate();
  Run(); // simulation

  Print("Celkem doba rozvozu v minutách: %f v hodinách: %f\n",
        DeliveryTime->Sum() + OverTime->Sum(),
        (DeliveryTime->Sum() + OverTime->Sum()) / 60);
  Print("Průměrná doba rozvozu v minutách na auto: %f v hodinách: %f\n",
        (DeliveryTime->Sum() + OverTime->Sum()) /
            (numberOfElectricCars + numberOfGasCars),
        ((DeliveryTime->Sum() + OverTime->Sum()) /
         (numberOfElectricCars + numberOfGasCars)) /
            60);

  double solarPanelOutputNow = solarPanelOutput * Uniform(0, 1);
  Print("Vyrobeno solárnímy panely: %f Kw\n", solarPanelOutputNow);

  Print("Celkem spotřebováno paliva: %f l\n", FuelUsage->Sum());

  Print("Celkem spotřebováno energie: %f Kw\n", BateryUsage->Sum());

  Print("Spotřebovaná energie (odečteny solární panely): %f Kw\n",
        BateryUsage->Sum() - solarPanelOutputNow);

  Print("Celkem přesčasů: %f minut\n", OverTime->Sum());
  Print("Celkem brzkých ukončení směny: %f minut\n", ShiftEndsEarly->Sum());

  DeliveryTime->Output();
  MorningShift->Output();
  AfternoonShift->Output();
  FuelUsage->Output();
  BateryUsage->Output();
  OverTime->Output();
  ShiftEndsEarly->Output();
  return 0;
}