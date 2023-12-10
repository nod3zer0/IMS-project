#include "inc/GasCar.h"

GasCar::GasCar(int *Boxes, bool *shift, double afternoonShift,
               double morningShift, Stat *FuelUsage, Stat *DeliveryTime,
               Stat *OverTime, Store *LoadingStations, Stat *ShiftEndsEarly)
    : Process() {
  this->Boxes = Boxes;
  this->shift = shift;
  this->afternoonShift = afternoonShift;
  this->morningShift = morningShift;
  this->FuelUsage = FuelUsage;
  this->DeliveryTime = DeliveryTime;
  this->OverTime = OverTime;
  this->LoadingStations = LoadingStations;
  this->ShiftEndsEarly = ShiftEndsEarly;
}

void GasCar::Behavior() {

  if (!*shift) { // if shift is over cancel
    double overtime = Time - (morningShift + afternoonShift);
    if (overtime > 0)
      (*OverTime)(overtime);

    Cancel();
  }

  if (*Boxes <= 0) { // if there are no boxes wait until there are
    double shiftEndsEarly = Time - morningShift - afternoonShift;
    if (shiftEndsEarly < 0)
      (*ShiftEndsEarly)(-shiftEndsEarly);
    while (_WaitUntil(*Boxes > 0 || !*shift))
      ;

    if (!*shift) { // if shift is over cancel
      double overtime = Time - (morningShift + afternoonShift);
      if (overtime > 0)
        (*OverTime)(overtime);

      Cancel();
    }
  }
  Enter(*LoadingStations, 1);
  if (*Boxes <= carCapacity) { // if there is less boxes than car capacity
                               // take all
    *Boxes = 0;
  } else { // else take car capacity
    *Boxes -= carCapacity;
  }

  double loading =
      Uniform(loading_mean - loading_sd, loading_mean + loading_sd);
  Wait(loading); // loading
  Leave(*LoadingStations, 1);
  double delivery =
      Uniform(delivery_mean - delivery_sd, delivery_mean + delivery_sd);
  Wait(delivery); // delivery
  double returnTime = Uniform(return_mean - return_sd, return_mean + return_sd);
  Wait(returnTime); // return
  (*DeliveryTime)(loading + delivery + returnTime);
  (*FuelUsage)(
      (((loading + delivery + returnTime) / 60 * Uniform(30, 90)) / 100) *
      Uniform(8, 12)); // 10l 100km
  (new GasCar(this->Boxes, this->shift, this->afternoonShift,
              this->morningShift, this->FuelUsage, this->DeliveryTime,
              this->OverTime, this->LoadingStations, this->ShiftEndsEarly))
      ->Activate();
}

GasCarStart::GasCarStart(int *Boxes, bool *shift, double afternoonShift,
                         double morningShift, Stat *FuelUsage,
                         Stat *DeliveryTime, Stat *OverTime,
                         Store *LoadingStations, Stat *ShiftEndsEarly)
    : Process() {
  this->Boxes = Boxes;
  this->shift = shift;
  this->afternoonShift = afternoonShift;
  this->morningShift = morningShift;
  this->FuelUsage = FuelUsage;
  this->DeliveryTime = DeliveryTime;
  this->OverTime = OverTime;
  this->LoadingStations = LoadingStations;
  this->ShiftEndsEarly = ShiftEndsEarly;
}

void GasCarStart::Behavior() {
  if ((Random() * numberOfDaysUntilGasEmpty) >=
      numberOfDaysUntilGasEmpty - 1) { // if gas is empty
    Wait(Uniform(15, 20));
  }
  (new GasCar(this->Boxes, this->shift, this->afternoonShift,
              this->morningShift, this->FuelUsage, this->DeliveryTime,
              this->OverTime, this->LoadingStations, this->ShiftEndsEarly))
      ->Activate();
}