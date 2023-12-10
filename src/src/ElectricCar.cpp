#include "inc/ElectricCar.h"

ElectricCar::ElectricCar(int *Boxes, bool *shift, double afternoonShift,
                         double morningShift, Stat *BateryUsage,
                         Stat *DeliveryTime, Stat *OverTime,
                         Store *LoadingStations, Stat *ShiftEndsEarly)
    : Process() {
  this->Boxes = Boxes;
  this->shift = shift;
  this->afternoonShift = afternoonShift;
  this->morningShift = morningShift;
  this->BateryUsage = BateryUsage;
  this->DeliveryTime = DeliveryTime;
  this->OverTime = OverTime;
  this->LoadingStations = LoadingStations;
  this->ShiftEndsEarly = ShiftEndsEarly;
}

void ElectricCar::Behavior() {
  if (!*shift) { // if shift is over cancel
    double overtime = Time - morningShift - afternoonShift;
    if (overtime > 0)
      (*OverTime)(overtime);

    Cancel();
  }

  if (*Boxes <= 0) { // if there is no boxes wait until there is
    double shiftEndsEarly = Time - morningShift - afternoonShift;
    if (shiftEndsEarly < 0)
      (*ShiftEndsEarly)(-shiftEndsEarly);
    while (_WaitUntil(*Boxes > 0 || !*shift))
      ;
    if (!*shift) { // if shift is over cancel
      double overtime = Time - morningShift - afternoonShift;
      if (overtime > 0)
        (*OverTime)(overtime);
      Cancel();
    }
  }
  Enter(*LoadingStations, 1);
  if (*Boxes <=
      carCapacity) { // if there is less boxes than car capacity take all
    *Boxes = 0;
  } else {
    *Boxes -= carCapacity; // else take car capacity
  }

  double loading =
      Uniform(loading_mean - loading_sd, loading_mean + loading_sd);
  Wait(loading); // laoding
  Leave(*LoadingStations, 1);
  double delivery =
      Uniform(delivery_mean - delivery_sd, delivery_mean + delivery_sd);
  Wait(delivery); // delivery
  double returnTime = Uniform(return_mean - return_sd, return_mean + return_sd);
  Wait(returnTime); // return
  (*DeliveryTime)(loading + delivery + returnTime);
  (*BateryUsage)(((((loading + delivery + returnTime) / 60 *
                    Uniform(spead_mean - spead_sd, spead_mean + spead_sd))) /
                  100) *
                 Uniform(16, 24)); // 20kw  100 km
  (new ElectricCar(this->Boxes, this->shift, this->afternoonShift,
                   this->morningShift, this->BateryUsage, this->DeliveryTime,
                   this->OverTime, this->LoadingStations, this->ShiftEndsEarly))
      ->Activate();
}
