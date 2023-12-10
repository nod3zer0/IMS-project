#include "simlib.h"
class ElectricCar : public Process {
public:
  ElectricCar(int *Boxes, bool *shift, double afternoonShift,
              double morningShift, Stat *BateryUsage, Stat *DeliveryTime,
              Stat *OverTime, Store *LoadingStations, Stat *ShiftEndsEarly);
  void Behavior();

private:
  int *Boxes;
  bool *shift;
  double afternoonShift;
  double morningShift;
  const int carCapacity = 104;
  Stat *BateryUsage;
  Stat *DeliveryTime;
  Stat *OverTime;
  Store *LoadingStations;
  Stat *ShiftEndsEarly;
};