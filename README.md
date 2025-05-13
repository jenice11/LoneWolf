# C++ LoneWolf Text Adventure Game 🐺

## Project Overview
This project is a text-based adventure game implemented in C++. It is based on Lone Wolf - Flight from the Dark with some changes in story and simplified mechanics. 
The game has a branching storyline where players navigate through different scenes, engage in combat, collect items.

## Future Plans
- Save/load game functionality
- Character skill
- Storyline continuation

## Built With
- **C++20**
- **CMake** for build system

### Building the Project

```bash
git clone https://github.com/jenice11/LoneWolf.git
cd LoneWolf
cmake -B build -S .
cmake --build build --config Release
```

Run
```bash
./build/bin/Release/LoneWolf
```

### Using Visual Studio with CMake
The repository includes a CMakeSettings.json file for Visual Studio integration with both Debug and Release configurations.

1. Open the project folder in Visual Studio
2. Select your preferred configuration (x64-Debug or x64-Release)
3. Build and run directly from Visual Studio

### Disclaimer
This project is purely for personal and educational purposes only. 