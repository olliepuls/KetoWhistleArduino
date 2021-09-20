# KetoWhistleArduino
Arduino Code for ENGN4221 S2 2021 KetoWhistle Project

## Installation & Usage Instructions

### VS Code + PlatformIO 

This code was developed using VS code and Platformio due to its flexibility and large selection of libraries.

To use this code, clone the repository, install [VS Code](https://code.visualstudio.com/download) and install the [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) and C/C++ extensions within VS Code.

### Build & Upload
Development was carried out on an Arduino Nano Every and Arduino Nano BLE (for final design). These boards have a number of non-compatible features and commands. Please switch between the `nano_every` and `main` branches for the Every and BLE code, respectively.

Open the KetoWhistle folder as a platformIO project and run Build and Upload commands with the Arduino Nano BLE connected. These commands can be run from the VS code Command Palette (Ctrl+Shift+P).

## Repository Structure

The src, lib, include and test directories are as usual for a platformIO project. Read the README files in these repositories for further explanations. The files in the src directory contain code to control the modules of the KetoWhistle. They are named after the module which they control. These libvraries of functions are included as headers in the `main.cpp` script and can be used as needed.

### Subsystem Test Scripts

Tests for individual modules are written as functions in their respective .cpp file. They are named using the following convention: `<module>_test_setup()` and `<module>_test_loop()`. To run these test functions, replace contents of the `setup()` and `loop()` functions in `main.cpp` with these test functions. The modules should function as intended in the test scripts if wired correctly.
