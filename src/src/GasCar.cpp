#include "inc/GasCar.h"

GasCar::GasCar(int *Boxes, bool *shift, double afternoonShift,
               double morningShift, Stat *FuelUsage, Stat *DeliveryTime,
               Stat *OverTime, Store *LoadingStations)
    : Process() {
  this->Boxes = Boxes;
  this->shift = shift;
  this->afternoonShift = afternoonShift;
  this->morningShift = morningShift;
  this->FuelUsage = FuelUsage;
  this->DeliveryTime = DeliveryTime;
  this->OverTime = OverTime;
  this->LoadingStations = LoadingStations;
}

void GasCar::Behavior() {
  if (!*shift) { // if shift is over cancel

    double Overtime = Time - morningShift - afternoonShift;
    if (Overtime > 0)
      (*OverTime)(Overtime);

    Cancel();
  }

  if (*Boxes <= 0) { // if there is no boxes wait until there is
    while (_WaitUntil(*Boxes > 0 || !*shift))
      ;
    if (!*shift) { // if shift is over cancel
      double Overtime = Time - morningShift - afternoonShift;
      if (Overtime > 0)
        (*OverTime)(Overtime);
      Cancel();
    }
  } else {

    if (*Boxes <= carCapacity) { // if there is less boxes than car capacity
                                 // take all
      *Boxes = 0;
    } else { // else take car capacity
      *Boxes -= carCapacity;
    }
  }

  Enter(*LoadingStations, 1);
  double loading = Uniform(30, 50);
  Wait(loading); // loading
  Leave(*LoadingStations, 1);
  double delivery = Uniform(60, 2 * 60);
  Wait(delivery); // delivery
  double returnTime = Uniform(30, 60);
  Wait(returnTime); // return
  (*DeliveryTime)(loading + delivery + returnTime);
  (*FuelUsage)(
      (((loading + delivery + returnTime) / 60 * Uniform(30, 90)) / 100) *
      Uniform(8, 12)); // 10l 100km
  (new GasCar(this->Boxes, this->shift, this->afternoonShift,
              this->morningShift, this->FuelUsage, this->DeliveryTime,
              this->OverTime, this->LoadingStations))
      ->Activate();
}

GasCarStart::GasCarStart(int *Boxes, bool *shift, double afternoonShift,
                         double morningShift, Stat *FuelUsage,
                         Stat *DeliveryTime, Stat *OverTime,
                         Store *LoadingStations)
    : Process() {
  this->Boxes = Boxes;
  this->shift = shift;
  this->afternoonShift = afternoonShift;
  this->morningShift = morningShift;
  this->FuelUsage = FuelUsage;
  this->DeliveryTime = DeliveryTime;
  this->OverTime = OverTime;
  this->LoadingStations = LoadingStations;
}

void GasCarStart::Behavior() {
  if ((Random() * numberOfDaysUntilGasEmpty) >=
      numberOfDaysUntilGasEmpty - 1) { // if gas is empty
    (new GasCar(this->Boxes, this->shift, this->afternoonShift,
                this->morningShift, this->FuelUsage, this->DeliveryTime,
                this->OverTime, this->LoadingStations))
        ->Activate();
  }
}