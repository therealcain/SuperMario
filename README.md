# Super Mario
This project is created with Modern C++17 and SFML ( Simple and Fast Multimedia Library )

[![Video Sample](https://img.youtube.com/vi/5ow_ju4iNLo/0.jpg)](https://www.youtube.com/watch?v=5ow_ju4iNLo)

# Features
* Mario Player - currently in godmode
* Entities:
* - Blocks:
* - - Empty
* - - Brick
* - - Question Mark
* - Cloud
* - Coin
* Enemies:
* - Goomba
* - Spiny
* Miscellaneous:
* - Mushroom
* - Flower

# How to Compile
CMake file is designed only to Ubuntu.

Open up the Terminal in the project directory, and type this commands:
```
mkdir build; cd build
cmake ..
make
```
And the game will be generated to the bin folder

In case there is any sort of errors make sure you have the latest or installed `make`,`cmake` and `libsfml` binaries.
