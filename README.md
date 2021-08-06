# Asteroids
A version of Asteroids built in SFML that only allocates memory statically, similar to older games from the Playstation era.

## Assets
All assets were purchased for usage from [here](https://kpdwyer.itch.io/lo-fi-stellar-skirmish) and [here](https://s4m-ur4i.itch.io/huge-pixelart-asset-pack).

## Installation

### Requirements

- Windows 10
- [Visual Studio 15 2017](https://visualstudio.microsoft.com/vs/older-downloads/)
- [SFML 2.5.1](https://www.sfml-dev.org/download/sfml/2.5.1/) (Other versions may work as well)

### Install

Install and set up SFML according to the instructions for Visual Studio [here](https://www.sfml-dev.org/tutorials/2.5/start-vc.php). Make sure to link SFML's graphics, windows, and system libraries, as well as other dependencies. Make sure that the Additional Dependencies under the Linker -> Input section of the project properties includes `winmm.lib;opengl32.lib;sfml-graphics-s.lib;sfml-window-s.lib;sfml-system-s.lib;freetype.lib`. Since these are statically linked, you will also need to include `SFML_STATIC` as a Preprocessor definition in the C/C++ -> Preprocessor section of the project properties.

Once you do all this, SFML should be properly set up to run with Visual Studio 2017. Build the project with `Ctrl+Shift+B` to create an executable, or run it directly with `Ctrl+F5`. 

### Other installation methods

There are many other ways to compile and run SFML projects, but I will not be going into them, since they depend greatly on the user's. If you wish to compile the project yourself, you can find a number of compilation methods for SFML [here](https://www.sfml-dev.org/tutorials/2.5/). If you are having trouble building the game yourself, you can go to the releases page to find a download for the game.

### How to play

| Action | Key |
| :--- | ---: |
| Accelerate | W or ↑ |
| Turn Right | D or → |
| Turn Left | A or ← |
| Shoot | S or ↓ or Space |
| Restart | R |
