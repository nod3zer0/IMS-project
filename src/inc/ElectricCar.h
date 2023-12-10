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
  const int loading_mean = 40.5;
  const int loading_sd = 9.056;
  const int delivery_mean = 93.22;
  const int delivery_sd = 24.845;
  const int return_mean = 43.2;
  const int return_sd = 10.95;
  const int spead_mean = 64.22;
  const int spead_sd = 13.381;
};