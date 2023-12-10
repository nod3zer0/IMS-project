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

  double loading = Uniform(30, 50);
  Wait(loading); // laoding
  Leave(*LoadingStations, 1);
  double delivery = Uniform(60, 2 * 60);
  Wait(delivery); // delivery
  double returnTime = Uniform(30, 60);
  Wait(returnTime); // return
  (*DeliveryTime)(loading + delivery + returnTime);
  (*BateryUsage)(
      ((((loading + delivery + returnTime) / 60 * Uniform(30, 90))) / 100) *
      Uniform(16, 24)); // 20kw  100 km
  (new ElectricCar(this->Boxes, this->shift, this->afternoonShift,
                   this->morningShift, this->BateryUsage, this->DeliveryTime,
                   this->OverTime, this->LoadingStations, this->ShiftEndsEarly))
      ->Activate();
}
