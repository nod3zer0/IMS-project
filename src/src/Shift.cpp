#include "inc/Shift.h"
#include "inc/ElectricCar.h"
#include "inc/GasCar.h"

Shift::Shift(Stat *AfternoonShift, Stat *MorningShift, int *Boxes,
             int afternoonBoxes, int morningBoxes, bool *shift,
             Stat *DeliveryTime, Stat *FuelUsage, Stat *BateryUsage,
             Stat *OverTime, Store *LoadingStations, int numberOfElectricCars,
             int numberOfGasCars, Stat *ShiftEndsEarly)
    : Process() {
  this->AfternoonShift = AfternoonShift;
  this->MorningShift = MorningShift;
  this->Boxes = Boxes;
  this->afternoonBoxes = afternoonBoxes;
  this->morningBoxes = morningBoxes;
  this->shift = shift;
  this->DeliveryTime = DeliveryTime;
  this->FuelUsage = FuelUsage;
  this->BateryUsage = BateryUsage;
  this->OverTime = OverTime;
  this->LoadingStations = LoadingStations;
  this->numberOfElectricCars = numberOfElectricCars;
  this->numberOfGasCars = numberOfGasCars;
  this->ShiftEndsEarly = ShiftEndsEarly;
}

void Shift::Behavior() {
  *shift = true;
  for (int i = 0; i < numberOfElectricCars; i++) { // start electric cars
    (new ElectricCar(Boxes, shift, afternoonShift, morningShift, BateryUsage,
                     DeliveryTime, OverTime, LoadingStations, ShiftEndsEarly))
        ->Activate();
  }

  for (int i = 0; i < numberOfGasCars; i++) { // start gas cars
    (new GasCar(Boxes, shift, afternoonShift, morningShift, FuelUsage,
                DeliveryTime, OverTime, LoadingStations, ShiftEndsEarly))
        ->Activate();
  }

  Wait(morningShift); // wait for morning shift to end

  if (*Boxes > 0) {
    Print("Dopoledne zbyva %d beden\n", *Boxes);
    Print("Rozvezlo se %d beden\n", morningBoxes - *Boxes);

  } else {
    Print("Dopoledne se rozvezlo vše\n");
    Print("Rozvezlo se %d beden\n", morningBoxes - *Boxes);
  }
  (*MorningShift)(morningShift);
  *Boxes = afternoonBoxes; // set new boxes for afternoon shift

  Wait(afternoonShift); // wait for afternoon shift to end
  if (*Boxes > 0) {
    Print("Odpoledne zbyva %d beden\n", *Boxes);
    Print("Rozvezlo se %d beden\n", afternoonBoxes - *Boxes);
  } else {
    Print("Odpoledne se Rozvezlo vše\n");
    Print("Rozvezlo se %d beden\n", afternoonBoxes - *Boxes);
  }
  (*AfternoonShift)(afternoonShift);
  *shift = false; // shift is over
}