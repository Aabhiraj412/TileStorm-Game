# 🎮 TileStorm

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C/C++](https://img.shields.io/badge/language-C%2FC%2B%2B-blue.svg)](https://en.cppreference.com/)
[![Raylib](https://img.shields.io/badge/graphics-Raylib-red.svg)](https://www.raylib.com/)

TileStorm is a fast-paced, time-limited tile-matching game built using the [Raylib](https://www.raylib.com/) graphics library. The game challenges players to match 3 or more tiles of the same type in a grid-based board within 60 seconds.

This repository contains **two implementations**:
- ✅ A **modular C version** with organized components (`functions.c`, `screens.c`, `global.h`)
- ✅ A **C++ single-file version** (`main.cpp`) for simplified compilation and quick testing

---

## 🧩 Features

- 🎯 **8x8 dynamic tile grid** with colorful tile types
- ⏰ **Time-limited gameplay** (60 seconds of intense matching)
- 🎬 **Real-time animations** and falling effects
- 🔍 **Smart match detection** (horizontal and vertical)
- 🏆 **Score tracking** with persistent high score saving
- 🖱️ **Interactive UI** featuring:
  - Hover highlighting (gray outline)
  - Selection highlighting (light red background)
  - Restart option upon game end

---

## 📂 Project Structure

```
.
├── Assets/
│   └── 123.jpg              # Background image
├── Components/
│   ├── functions.c          # Core game logic (C version)
│   ├── screens.c            # Drawing screens (C version)
│   └── functions.h          # Function declarations
├── global.h                 # Constants, structs, shared includes
├── main.c                   # Entry point for modular C version
├── main.cpp                 # Single-file C++ version
└── README.md               # This file
```

---

## 🛠️ Requirements

- **[Raylib](https://github.com/raysan5/raylib)** - Install via package manager or build from source
- **C/C++ compiler** (gcc/g++ recommended)
- **Linux environment** (tested on Kali and Ubuntu)

### Installing Raylib

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install libraylib-dev
```

**Arch Linux:**
```bash
sudo pacman -S raylib
```

**From source:**
```bash
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

---

## 🚀 How to Compile & Run

> You can build and run **either** version based on your preference.

### ▶️ C++ Version (Single File)
```bash
g++ main.cpp -o tilestorm -lraylib -lm -ldl -lpthread -lGL -lX11
./tilestorm
```

### ▶️ C Version (Modular Structure)
```bash
gcc main.c ./Components/functions.c ./Components/screens.c -o tilestorm -lraylib -lm -ldl -lpthread -lGL -lX11
./tilestorm
```

### Alternative: Using Makefile (Optional)
Create a `Makefile` for easier compilation:

```makefile
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c99
CXXFLAGS = -Wall -Wextra -std=c++11
LIBS = -lraylib -lm -ldl -lpthread -lGL -lX11

# C version
tilestorm-c: main.c Components/functions.c Components/screens.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# C++ version
tilestorm-cpp: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f tilestorm-c tilestorm-cpp

.PHONY: clean
```

---

## 🎮 How to Play

1. **Objective**: Match 3 or more tiles of the same color/type
2. **Controls**: 
   - Click to select a tile
   - Click an adjacent tile to swap
   - Valid matches will automatically clear and new tiles will fall
3. **Scoring**: Earn points for each successful match
4. **Time Limit**: You have 60 seconds to achieve the highest score possible
5. **Restart**: Click the restart button when the game ends

---

## 📊 Game Mechanics

- **Grid Size**: 8x8 tiles
- **Tile Types**: Multiple colorful tile varieties
- **Match Rules**: 3+ consecutive tiles (horizontal or vertical)
- **Gravity**: Tiles fall down when matches are cleared
- **High Score**: Automatically saved to `.highscore` file

---

## 🖼️ Screenshots

> Add gameplay screenshots or GIFs here

### 🎮 Gameplay Preview

![TileStorm Gameplay (CPP)](Assets/CPP%20Preview.png)
![TileStorm Gameplay (C)](Assets/C%20Preview.png)


---

## 📌 Technical Notes

- High score is stored in `.highscore` file in the project root directory
- The game uses Raylib's built-in timing functions for smooth 60 FPS gameplay
- Both versions share the same core game logic and mechanics
- Cross-platform compatible (tested on Linux, should work on Windows/macOS with proper Raylib setup)

---

## 🚧 Known Issues & Future Enhancements

- [ ] Add sound effects and background music
- [ ] Implement power-ups and special tiles
- [ ] Add difficulty levels
- [ ] Create mobile-friendly touch controls
- [ ] Add particle effects for matches

---

## 🤝 Contributing

Contributions are welcome! Please feel free to:
- Report bugs by opening an issue
- Suggest new features
- Submit pull requests with improvements

---

## 👨‍💻 Author

**Abhiraj Dixit**
- Made with ❤️ and lots of ☕
- For feedback or contributions, feel free to open an issue or PR

---

## 📝 License

This project is open-source and available under the [MIT License](LICENSE).

---

## 🙏 Acknowledgments

- [Raylib](https://www.raylib.com/) for the fantastic graphics library
- The open-source community for inspiration and support

---

*⭐ If you found this project helpful, please consider giving it a star!*