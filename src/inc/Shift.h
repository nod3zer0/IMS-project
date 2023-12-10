#include "simlib.h"
class Shift : public Process {
public:
  Shift(int *Boxes, int afternoonBoxes, int morningBoxes, bool *shift,
        Stat *DeliveryTime, Stat *FuelUsage, Stat *BateryUsage, Stat *OverTime,
        Store *LoadingStations, int numberOfElectricCars, int numberOfGasCars,
        Stat *ShiftEndsEarly);
  void Behavior();

private:
  int *Boxes;
  int afternoonBoxes;
  int morningBoxes;
  int numberOfElectricCars;
  int numberOfGasCars;
  bool *shift;
  const double morningShift = 5 * 60;
  const double afternoonShift = 4 * 60;
  Stat *DeliveryTime;
  Stat *FuelUsage;
  Stat *BateryUsage;
  Stat *OverTime;
  Store *LoadingStations;
  Stat *ShiftEndsEarly;
};