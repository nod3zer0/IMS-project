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
  int baterryCapacity = 450;
  const int kilometersPerCharge = 450;
  const int loading_mean = 42.22;
  const int loading_sd = 9.235;
  const int delivery_mean = 133.67;
  const int delivery_sd = 16.22;
  const int return_mean = 37.66;
  const int return_sd = 7.48;
  const int spead_mean = 64.22;
  const int spead_sd = 13.381;
};