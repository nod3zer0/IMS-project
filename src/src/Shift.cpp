#include "inc/Shift.h"
#include "inc/ElectricCar.h"
#include "inc/GasCar.h"

Shift::Shift(Stat *AfternoonShift, Stat *MorningShift, int *Boxes,
             int afternoonBoxes, int morningBoxes, bool *shift,
             Stat *DeliveryTime, Stat *FuelUsage, Stat *BateryUsage,
             Stat *OverTime, Store *LoadingStations, int numberOfElectricCars,
             int numberOfGasCars)
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
}

void Shift::Behavior() {
  *shift = true;
  for (int i = 0; i < numberOfElectricCars; i++) { // start electric cars
    (new ElectricCar(Boxes, shift, afternoonShift, morningShift, BateryUsage,
                     DeliveryTime, OverTime, LoadingStations))
        ->Activate();
  }

  for (int i = 0; i < numberOfGasCars; i++) { // start gas cars
    (new GasCar(Boxes, shift, afternoonShift, morningShift, FuelUsage,
                DeliveryTime, OverTime, LoadingStations))
        ->Activate();
  }

  Wait(morningShift); // wait for morning shift to end

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

  *Boxes = afternoonBoxes; // set new boxes for afternoon shift

  printf("Zbyva casu dopoledne (v minutách): %f\n",
         morningShift - MorningShift->Sum());
  Wait(afternoonShift); // wait for afternoon shift to end
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
  *shift = false; // shift is over
}