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
  Stat *OverTime;
  Stat *ShiftEndsEarly;

  DeliveryTime = new Stat("Čas rozvozu");
  FuelUsage = new Stat("Spotřeba paliva");
  BateryUsage = new Stat("Spotřeba energie");
  OverTime = new Stat("Přesčasy");
  ShiftEndsEarly = new Stat("Brzké ukončení směny");

  int *Boxes;

  argsT args = parseArguments(argc, argv);

  if (args.err) {
    fprintf(stderr, "Error while parsing arguments\n");
    return 1;
  }

  if (args.help) {
    printf("Použití: %s [MOŽNOSTI]\n", argv[0]);
    printf("Možnosti:\n");
    printf("  --electric_cars <number>  Počet elektrických aut\n");
    printf("  --gas_cars <number>       Počet aut na benzín\n");
    printf("  --morning_boxes <number>  Počet krabic v ranní směně\n");
    printf("  --afternoon_boxes <number>Počet krabic v odpolední směně\n");
    printf("  --solar_panel_output <number> Výkon solárních panelů v kW\n");
    printf("  --morning_shift <number>  Délka ranní směny v minutách\n");
    printf("  --afternoon_shift <number>Délka odpolední směny v minutách\n");
    printf("  --help                    Vytiskne tuto nápovědu\n");
  }

  if (args.morning_boxes != -1)
    morningBoxes = args.morning_boxes;
  if (args.afternoon_boxes != -1)
    afternoonBoxes = args.afternoon_boxes;
  if (args.electric_cars != -1)
    numberOfElectricCars = args.electric_cars;
  if (args.gas_cars != -1)
    numberOfGasCars = args.gas_cars;
  if (args.solar_panel_output != -1)
    solarPanelOutput = args.solar_panel_output;
  if (args.morning_shift != -1)
    morningShift = args.morning_shift;
  if (args.afternoon_shift != -1)
    afternoonShift = args.afternoon_shift;

  RandomSeed(time(nullptr));

  Boxes = new int(morningBoxes);

  Print("Model pekárny v Křižanov\n");
  SetOutput("model.out");
  Init(0, morningShift + afternoonShift + 100);
  (new Shift(Boxes, afternoonBoxes, morningBoxes, &shift, DeliveryTime,
             FuelUsage, BateryUsage, OverTime, &LoadingStations,
             numberOfElectricCars, numberOfGasCars, ShiftEndsEarly,
             morningShift, afternoonShift))
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

  double solarPanelOutputNow =
      Uniform(solarPanelOutput - solarPanelOutput * 0.065,
              solarPanelOutput + solarPanelOutput * 0.065);
  Print("Vyrobeno solárnímy panely: %f Kw\n", solarPanelOutputNow);

  Print("Celkem spotřebováno paliva: %f l\n", FuelUsage->Sum());

  Print("Celkem spotřebováno energie: %f Kw\n", BateryUsage->Sum());

  Print("Spotřebovaná energie (odečteny solární panely): %f Kw\n",
        BateryUsage->Sum() - solarPanelOutputNow);

  Print("Celkem přesčasů: %f minut\n", OverTime->Sum());
  Print("Celkem brzkých ukončení směny: %f minut\n", ShiftEndsEarly->Sum());

  DeliveryTime->Output();
  FuelUsage->Output();
  BateryUsage->Output();
  OverTime->Output();
  ShiftEndsEarly->Output();
  return 0;
}