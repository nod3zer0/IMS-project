# IMS: Bakery Delivery Fleet Simulation

A **discrete-event simulation** in **C++** built on the **SIMLIB** library (IMS course project, FI MUNI). It models the delivery operation of a bakery in Křižanov and answers operational questions: how do fleet composition, box volume, and shift length affect delivery time, energy/fuel consumption, and overtime?

## Model

- **Processes**: `Shift` (morning/afternoon) schedules delivery rounds; `ElectricCar` and `GasCar` processes pick up boxes at loading stations and deliver them.
- **Resources**: loading stations modeled as a SIMLIB `Store` (default capacity 2) — cars queue for loading capacity.
- **Energy**: electric cars draw from battery capacity; solar panel output (with random fluctuation) offsets consumed energy.
- **Collected statistics**: total/average delivery time, fuel usage, battery usage, overtime, and early shift terminations — reported as both `Print` output and SIMLIB `Stat` records.

## Experiments

Fully parameterized via CLI for sweep experiments:

```sh
./ims --electric_cars 2 --gas_cars 1 --morning_boxes 1040 \
      --afternoon_boxes 1040 --solar_panel_output 50
```

Options cover fleet mix (`--electric_cars`, `--gas_cars`), workload (`--morning_boxes`, `--afternoon_boxes`), shift lengths, and solar output; defaults encode the observed real-world configuration.

## Build

```sh
make            # on machines with SIMLIB installed
make -f MakefileLocal   # builds with the bundled SIMLIB copy
```

## Documentation

- `docs/petri nets.drawio` / `docs/petri-net.png` — the Petri-net model behind the simulation.
- `docs/ims_doc.docx` — project documentation.
