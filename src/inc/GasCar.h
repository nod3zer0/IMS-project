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
  const int loading_mean = 42.22;
  const int loading_sd = 9.235;
  const int delivery_mean = 93.22;
  const int delivery_sd = 24.845;
  const int return_mean = 43.2;
  const int return_sd = 10.95;
  const int spead_mean = 64.22;
  const int spead_sd = 13.381;
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