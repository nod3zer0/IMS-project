#include "simlib.h"
class GasCar : public Process {
public:
  GasCar(int *Boxes, bool *shift, double afternoonShift, double morningShift,
         Stat *FuelUsage, Stat *DeliveryTime, Stat *OverTime,
         Store *LoadingStations, Stat *ShiftEndsEarly);
  void Behavior();

private:
  int *Boxes;
  bool *shift;
  double afternoonShift;
  double morningShift;
  const int carCapacity = 104;
  Stat *FuelUsage;
  Stat *DeliveryTime;
  Stat *OverTime;
  Store *LoadingStations;
  Stat *ShiftEndsEarly;
};

class GasCarStart : public Process {
public:
  GasCarStart(int *Boxes, bool *shift, double afternoonShift,
              double morningShift, Stat *FuelUsage, Stat *DeliveryTime,
              Stat *OverTime, Store *LoadingStations, Stat *ShiftEndsEarly);
  void Behavior();

private:
  int *Boxes;
  bool *shift;
  double afternoonShift;
  double morningShift;
  const int numberOfDaysUntilGasEmpty = 5;
  Stat *FuelUsage;
  Stat *DeliveryTime;
  Stat *OverTime;
  Store *LoadingStations;
  Stat *ShiftEndsEarly;
};