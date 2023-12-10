#include "simlib.h"
class Shift : public Process {
public:
  Shift(int *Boxes, int afternoonBoxes, int morningBoxes, bool *shift,
        Stat *DeliveryTime, Stat *FuelUsage, Stat *BateryUsage, Stat *OverTime,
        Store *LoadingStations, int numberOfElectricCars, int numberOfGasCars,
        Stat *ShiftEndsEarly, int morningShift, int afternoonShift);
  void Behavior();

private:
  int *Boxes;
  int afternoonBoxes;
  int morningBoxes;
  int numberOfElectricCars;
  int numberOfGasCars;
  bool *shift;
  double morningShift;
  double afternoonShift;
  Stat *DeliveryTime;
  Stat *FuelUsage;
  Stat *BateryUsage;
  Stat *OverTime;
  Store *LoadingStations;
  Stat *ShiftEndsEarly;
};