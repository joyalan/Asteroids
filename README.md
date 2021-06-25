# Asteroids
A version of Asteroids built in SFML that does not use the `new` or `delete` keywords to manage memory. The program also uses custom data structures for memory management rather than use STL data structures like vectors or maps.

## Assets
All assets were purchased for usage from https://kpdwyer.itch.io/lo-fi-stellar-skirmish and https://s4m-ur4i.itch.io/huge-pixelart-asset-pack.

## Installation

### Requirements

- [Visual Studio 15 2017] {https://visualstudio.microsoft.com/vs/older-downloads/}
- [SFML 2.5.1] {https://www.sfml-dev.org/download/sfml/2.5.1/} (Other versions may work as well)

### Install

Install and set up SFML according to the instructions for Visual Studio [here] {https://www.sfml-dev.org/tutorials/2.5/start-vc.php}. Make sure to link SFML's graphics, windows, and system libraries, as well as other dependencies. Make sure that the Additional Dependencies under the Linker -> Input section of the project properties includes `winmm.lib;opengl32.lib;sfml-graphics-s.lib;sfml-window-s.lib;sfml-system-s.lib;freetype.lib`. Since these are statically linked, you will also need to include `SFML_STATIC` as a Preprocessor definition in the C/C++ -> Preprocessor section of the project properties.

Once you do all this, SFML should be properly set up to run with Visual Studio 2017. Build the project with `Ctrl+Shift+B` to create an executable, or run it directly with `Ctrl+F5`. 

### How to play

| Action | Key |
| :--- | ---: |
| Accelerate | W or ↑ |
| Turn Right | D or → |
| Turn Left | A or ← |
| Shoot | S or ↓ or Space |
| Restart | R |
