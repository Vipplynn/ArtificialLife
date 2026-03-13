# Artificial Life — Evolved Creature Simulation

A from-scratch implementation of an artificial life system in C. Creatures composed of nodes and springs evolve locomotion strategies over successive generations using a genetic algorithm. No physics engine, no ML framework, no game engine — every system is hand-written.

A companion bare-metal port targets an ARM microcontroller, driving a TFT display over SPI with no OS, no standard library, and a fixed memory budget.

---

## Demo

https://github.com/user-attachments/assets/deb6fbdb-9614-4162-baf1-202b080cc464


---

## Projects

### 1. Evolution Simulation (Desktop)

A real-time 2D simulation running on Linux via SDL3. A population of creatures is evaluated for locomotion fitness each generation. The best performers are selected, mutated, and carried forward. Over hundreds of generations, creatures develop stable and sometimes unexpected movement strategies.

**What is built from scratch:**
- Verlet integration physics engine
- Spring and node creature body system
- Muscle springs driven by sine-wave oscillation
- Genetic encoding of creature morphology and timing
- Selection, mutation, and generational loop
- SDL3 rendering with a custom filled-circle rasterizer

**Status:**
| Milestone | Task Description | Status |
| :--- | :--- | :--- |
| **1. Foundation** | Initialize SDL3 window, establish the game loop, and implement basic shape rendering (circles and lines). | Complete |
| **2. Physics Engine** | Develop Verlet integration for node movement, implement spring constraints, gravity, and ground collision detection. | Complete |
| **3. Creature Definition** | Define creature data structures (nodes/connections) and muscle contraction timings encoded as a genome. | Complete |
| **4. Animation** | Hardcode a test genome to verify that the muscle timings produce movement in the physics world. | Complete |
| **5. Fitness Evaluation** | Implement a function to measure the horizontal distance traveled over a fixed timeframe. | Complete |
| **6. Population Generation** | Write the logic to spawn a population of creatures with randomized genomic data. | Complete |
| **7. Genetic Algorithm** | Create selection and mutation logic to preserve the best performers and generate varied descendants. | Complete |
| **8. Evolution Loop** | Automate the cycle of simulation, evaluation, and reproduction to observe emergent behavior. | Complete |


---

### 2. Bare-Metal Physics Port (Microcontroller)

A stripped-down port of the physics engine targeting an ARM Cortex-M microcontroller, emulated via QEMU. The goal is to render a single physically simulated creature on a small TFT display driven over SPI — with no OS, no malloc, and no standard library.

This project is a deliberate exercise in constraint-driven engineering: fitting a real physics simulation into tens of kilobytes of SRAM, managing the main loop directly with hardware timers, and implementing peripheral communication at the register level.

**What is built from scratch:**
- Bare-metal ARM startup code and linker script
- SPI driver for TFT display (register-level, no HAL)
- Fixed-size memory pool replacing dynamic allocation
- Timer interrupt for consistent physics timestep
- Minimal framebuffer renderer

**Status:**
| Component | Status |
|---|---|
| QEMU bare-metal boot | In progress |
| SPI / TFT driver | Planned |
| Fixed memory pool allocator | Planned |
| Physics engine port | Planned |
| Creature rendering on display | Planned |

---

## How the Evolution Works

Each creature is defined by a genome — a compact array of floats encoding node positions, spring rest lengths, spring stiffness values, and muscle timing parameters. A population of randomly generated creatures is simulated in sequence. Each creature runs for a fixed number of physics steps and is scored by horizontal distance traveled. The top performers are retained, copied, and mutated — small random perturbations applied to genome values. This cycle repeats across generations.

No neural network is involved. Behavior emerges entirely from the physical interaction of the creature's body structure and its muscle oscillation pattern. A creature does not learn — it is replaced by a better one.

---

## Why the Behavior is Interesting

The simulation frequently discovers solutions that were not anticipated. Early generations produce incoherent flailing. Later generations develop asymmetric gaits, bouncing strategies, or body configurations that exploit the physics in unexpected ways. Occasionally a creature finds a degenerate solution — spinning, falling forward, or exploiting a numerical instability — faster than any walking strategy. These moments are unscripted and a direct consequence of optimizing for a simple fitness metric without constraints on how that fitness is achieved.

---

## Building (Desktop)

**Dependencies:**

```bash
sudo apt install build-essential cmake git
sudo apt install libwayland-dev libxkbcommon-dev wayland-protocols \
  libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev \
  libxss-dev libxfixes-dev libxtst-dev libxinerama-dev \
  libdbus-1-dev libudev-dev libgles2-mesa-dev libegl1-mesa-dev
```

**Build:**

```bash
git clone --recurse-submodules https://github.com/yourusername/artificial
cd artificial
mkdir build && cd build
cmake ..
make
./Debug/program
```

---

## Project Structure

```
artificial/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── build/
├── include/
│   ├── creature.h
│   ├── draw.h
│   └── physics.h
├── src/
│   ├── creature.c
│   ├── draw.c
│   ├── main.c
│   └── physics.c
└── vendored
    └── SDL/
```

---

## References

- Karl Sims, *Evolved Virtual Creatures* (1994) — the foundational paper this project is inspired by
- Jakub Červený, *Verlet Integration* — physics integration method used
- ARM Cortex-M Technical Reference Manual — bare-metal port reference

---

## License

MIT
