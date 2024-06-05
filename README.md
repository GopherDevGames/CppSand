# About
This is a small SDL2 Based Sand Simulator.
This project uses the [SDL2 Graphics Library](https://www.libsdl.org/license.php)
# Controls
Choose a particle using the buttons 1-9 on your keyboard.

1 - Sand

2 - Water

Hold the left mouse button to place said particle.
# Compilation
This project requires you install the [Meson Build System](https://mesonbuild.com/index.html)

Run **release.sh** to build for linux, or **release_windows.sh** to crossbuild for windows from Linux.

The **debug.sh** file compiles and runs the game in debug-mode using GDB

# Project Structure and Methedology
The project is seperated into multiple files.

`main.ccp` - Instantiates the **SandRenderer** class

`SandRenderer.h` - Implements the **SandRenderer** class, handles rendering and input using SDL2, instantiates and runs the **SandSim** class every frame

`SandSim.h` - Houses the logic behind the simulation.

`Globals.h` - Holds various constants, not the best way to do it but having all of the important data in one file greatly increases the ease of adding and modyfing particle data.

While most of the files are simple `.h` files, splitting them into multiple `.cpp` files might be required to make maintaining them a bit easier.

## About Code

The main plan behind this project is to minimize technical debt, while keeping the core project extendable and portable.

Additionally, due to the sheer amount of particles on screen (777k on a 720x1080 screen with 1x scaling), tiny optimizations in core logic loops and avoidance of creating/deleting data on the heap (and to a lesser extent, on the stack) is very helpful.
