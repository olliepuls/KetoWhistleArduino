# KetoWhistleArduino
Arduino Code for ENGN4221 S2 2021 KetoWhistle Project

## Installation & Usage Instructions

### VS Code + PlatformIO (Preferred)

This code was developed using VS code and Platformio due to its flexibility and large selection of libraries.

To use this code, clone the repository, install [VS Code](https://code.visualstudio.com/download) and install the [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) and C/C++ extensions within VS Code.

Open the KetoWhistle folder as a platformIO project and run Build and Upload commands with the Arduino Nano BLE connected. 

### Arduino IDE

If you are not comfortable with VS Code/PlatformIO you can copy the code into the Arduino IDE. Note that there will be files in the *./Ketowhistle/lib/* folder that are required for proper execution of the scripts.

Other library dependencies that will need to be installed can be found in the *./KetoWhistle/platformio.ini* file.

## Repository Structure

The src, lib and test directories are as usual for a platformIO project. Read the README files in these repositories for further explanations.

### Subsystem Test Scripts

Tests for individual subsystems can be found in the *./KetoWhistle/src/test_scripts/* folder. The names of the `setup` and `loop` functions of these scripts have been modified slightly to avoid conflicts with the main script. To run a test script, comment out/replace the setup/loop code in *main.cpp* and replace it with the test script setup & loop functions. Then Build and Upload as normal.