# Arduino Snake Game 🐍

<p align="center">
  <img src="./Assets/gameplay.gif" alt="Snake Game Gameplay" width="90%" height="90%" />
</p>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#hardware-requirements">Hardware</a> •
  <a href="#installation">Installation</a> •
  <a href="#usage">Usage</a> •
  <a href="#configuration">Configuration</a> •
  <a href="#troubleshooting">Troubleshooting</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white" alt="Arduino">
  <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge" alt="License">
  <img src="https://img.shields.io/badge/Version-1.1-blue.svg?style=for-the-badge" alt="Version">
</p>

---

## 📖 Overview

A faithful recreation of the classic Nokia Snake game for Arduino microcontrollers. This project combines retro gaming nostalgia with modern embedded programming practices, featuring smooth OLED graphics, responsive controls, persistent high scores, and progressively challenging gameplay.

## ✨ Features

- 🎮 **Classic Snake Gameplay** - Faithful recreation of the Nokia original
- 🏆 **Persistent High Scores** - Saved to EEPROM, survives power cycles
- 🎊 **Win Condition** - Reach maximum length (150 segments) to win!
- 📈 **Progressive Difficulty** - Game speeds up every 5 points
- 🎯 **Smart Collision Detection** - Wall, self-collision, and food detection
- 💾 **Ultra Memory Efficient** - PROGMEM optimization frees up RAM
- 🖥️ **Flicker-Free Display** - Full buffer rendering for smooth 24 FPS graphics
- 🕹️ **Responsive Controls** - Input debouncing and anti-reverse logic
- 🔄 **Instant Restart** - Press any button to play again
- 📊 **Real-time Stats** - Current score and high score displayed while playing
- ⭐ **High Score Celebration** - Special message when you beat your record
- 🔧 **High Score Reset** - Dedicated button to clear records

## 🆕 What's New in v1.1

### Performance & Memory
- ⚡ **Full buffer mode** - Switched to `U8G2_F` for completely flicker-free rendering
- 🧠 **PROGMEM strings** - All text moved to flash memory, saving precious RAM
- 🎯 **Frame-rate control** - Fixed 42ms draw intervals (24 FPS) for consistent visuals
- 💾 **Input debouncing** - 50ms debounce prevents accidental double-inputs

### New Features
- 🏆 **EEPROM high scores** - Your best score is saved permanently
- 🎊 **Win screen** - Special celebration for reaching max length
- 🔄 **Reset button** - Pin 4 clears high scores
- ⭐ **New record indicator** - Get notified when you beat your best

### Gameplay Improvements
- 🚀 **Increased max length** - Now 150 segments (up from 100)
- ⚙️ **Better difficulty curve** - Speed increases every 5 points (was 3)
- 🎮 **Refined starting speed** - 500ms starting speed for better balance
- 🎯 **Optimized grid** - 6x6 pixel cells for smoother gameplay (was 4x4)

## 🛠️ Hardware Requirements

### Components List

| Component | Specification | Quantity |
|-----------|--------------|----------|
| Arduino Board | Uno/Nano (ATmega328P) | 1 |
| OLED Display | SH1106 128x64 (I2C) | 1 |
| Push Buttons | Momentary tactile switches | 5 |
| Breadboard | Standard size | 1 |
| Jumper Wires | Male-to-male | ~12 |

**Estimated Cost:** $12-15 USD

### Pin Configuration

```
┌─────────────────┐
│   OLED Display  │
│   (SH1106 I2C)  │
└─────────────────┘
    │   │   │   │
   SDA SCL VCC GND
    │   │   │   │
    A4  A5  5V  GND ← Arduino
    
┌──────────────────────┐
│   Push Buttons       │
├──────────────────────┤
│ UP      → Pin 6      │
│ LEFT    → Pin 7      │
│ DOWN    → Pin 9      │
│ RIGHT   → Pin 10     │
│ RESET   → Pin 4      │ ← NEW!
│ (All buttons → GND)  │
└──────────────────────┘
```

## 📦 Software Dependencies

### Required Libraries

- **U8g2lib** - High-performance OLED display driver
  - **Installation:** Arduino IDE → Tools → Manage Libraries → Search "U8g2" → Install
  - **Version:** Latest stable (tested with v2.35+)
  - **Author:** olikraus
- **EEPROM** - Built-in Arduino library (no installation needed)

## 🚀 Installation

Choose your preferred installation method:

### Method 1: Download from Releases (Recommended for Beginners)

1. **Download the Latest Release**
   - Go to [Releases](https://github.com/aydakikio/arduino_snake/releases)
   - Download the latest `snake_arduino_v1.1.ino` file
   - Save it to a folder named `snake_arduino_v1.1`

2. **Install U8g2 Library**
   - Open Arduino IDE
   - Navigate to `Tools` → `Manage Libraries`
   - Search for "U8g2" by oliver
   - Click `Install`
   - Restart Arduino IDE

3. **Hardware Setup**
   - Connect OLED display to I2C pins (A4/SDA, A5/SCL)
   - Wire push buttons to pins 4, 6, 7, 9, and 10
   - Connect 5V and GND to all components

4. **Upload Code**
   - Open `snake_arduino_v1.1.ino` in Arduino IDE
   - Select your board: `Tools` → `Board` → `Arduino Uno/Nano`
   - Select your port: `Tools` → `Port` → (your Arduino port)
   - Click Upload (Ctrl+U / ⌘+U)

5. **Play!**
   - Game starts automatically after upload
   - Use directional buttons to control the snake
   - Eat food to grow and increase score
   - Beat your high score!

### Method 2: Clone from Git

1. **Clone the Repository**
   ```bash
   git clone https://github.com/aydakikio/arduino_snake.git
   cd arduino_snake
   ```

2. **Install U8g2 Library** (same as Method 1, step 2)

3. **Follow steps 3-5 from Method 1**

> **Note:** Arduino IDE requires `.ino` files to be in a folder with the same name

### Detailed Wiring Diagram

<p align="center">
  <img src="./Assets/wiring_diagram.png" alt="Detailed Wiring Diagram" width="80%" height="80%" />
</p>

## 🎮 Usage

### Game Controls

| Button | Action |
|--------|--------|
| UP | Move snake upward |
| DOWN | Move snake downward |
| LEFT | Move snake left |
| RIGHT | Move snake right |
| RESET | Reset high score (during gameplay) |
| ANY (Game Over) | Restart game |

### Gameplay Rules

1. **Objective:** Eat food (small squares) to grow your snake and increase score
2. **Speed:** Game gets faster every 5 points (500ms → 100ms minimum)
3. **High Score:** Your best score is automatically saved and displayed
4. **Win Condition:** Reach 150 segments to win the game!
5. **Game Over:** Hitting walls or yourself ends the game
6. **Scoring:** +1 point per food eaten
7. **No Reverse:** Can't make 180° turns (prevents accidental death)

### High Score Management

- **Automatic Save:** High scores are saved immediately when achieved
- **Persistent Storage:** Scores survive power cycles and resets
- **Reset Option:** Hold the RESET button (pin 4) to clear high scores
- **New Record Alert:** See "NEW HIGH SCORE!" message on game over screen

## ⚙️ Configuration

### Gameplay Tuning

Customize game behavior in the source code:

```cpp
// Game difficulty settings
#define SNAKE_MAX 150           // Maximum snake size (win condition)
#define GRID_SIZE 6             // Pixel size of grid cells
int game_speed = 500;           // Starting speed (ms per move)
int min_speed = 100;            // Minimum speed cap

// Speed progression (in main loop)
if (score % 5 == 0 && game_speed > min_speed) {
  game_speed -= 15;  // Speed increase per milestone
}
```

### Performance Tuning

```cpp
#define INPUT_DEBOUNCE 50       // Button debounce time (ms)
#define DRAW_INTERVAL 42        // Frame time (ms) - ~24 FPS
```

### Hardware Customization

**Change Button Pins:**
```cpp
#define BTN_RIGHT 10   // Modify these to match your wiring
#define BTN_DOWN 9
#define BTN_LEFT 7
#define BTN_UP 6
#define BTN_RESET 4    // High score reset button
```

**Switch Display Controller:**
```cpp
// For SSD1306 displays, replace line 3 with:
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
```

**Adjust EEPROM Addresses:**
```cpp
#define EEPROM_HIGH_SCORE_ADDR 0    // High score storage address
#define EEPROM_MAGIC_ADDR 2         // Validation byte address
#define EEPROM_MAGIC_VALUE 42       // Validation value
```

## 🏗️ Code Architecture

### System Design

```
┌─────────────────────────────────────┐
│         Main Game Loop              │
├─────────────────────────────────────┤
│  1. Handle Input (debounced)        │
│  2. Update Game State (timing)      │
│  3. Move Snake (grid-based)         │
│  4. Check Collisions (walls/self)   │
│  5. Process Food (growth/spawn)     │
│  6. Update High Score (EEPROM)      │
│  7. Render Display (full buffer)    │
└─────────────────────────────────────┘
```

### Key Components

**Data Structures:**
```cpp
struct Point {
  int8_t x, y;  // Grid-aligned coordinates (optimized)
};

Point snake[SNAKE_MAX];  // Snake body segments (150 max)
Point food;              // Food position
```

**Core Systems:**
- **Display:** U8G2 full-buffer rendering for flicker-free graphics
- **Input:** Debounced button reading with direction validation
- **Timing:** Non-blocking `millis()` based updates with frame limiting
- **Physics:** Grid-snapped movement with precise collision detection
- **Storage:** EEPROM persistence with magic byte validation
- **Memory:** PROGMEM strings to minimize RAM usage

### Performance Metrics

| Metric | Value | Change from v1.0 |
|--------|-------|------------------|
| RAM Usage | ~950 bytes | +150 bytes (buffer) |
| Flash Usage | ~16KB | +1KB (features) |
| Frame Rate | 24 FPS (capped) | Fixed timing |
| Input Latency | <50ms | Improved |
| Max Snake Length | 150 segments | +50% |
| Display Buffer | Full (1024 bytes) | Upgraded |

## 🐛 Troubleshooting

### Display Issues

**Problem:** OLED screen stays blank
- ✅ Verify I2C connections (SDA→A4, SCL→A5)
- ✅ Check power connections (VCC→5V, GND→GND)
- ✅ Confirm display controller type (SH1106 vs SSD1306)
- ✅ Test I2C scanner sketch to verify address
- ✅ Try swapping SDA/SCL wires
- ✅ Ensure U8g2 library is installed correctly

**Problem:** Display flickers or artifacts
- ✅ Verify you're using `_F_` buffer mode (full buffer)
- ✅ Check power supply stability (use quality USB cable)
- ✅ Add 100µF capacitor across display power pins

**Problem:** Display is rotated/mirrored
- Change `U8G2_R0` to `U8G2_R1`, `U8G2_R2`, or `U8G2_R3` in line 3

### Control Issues

**Problem:** Buttons not responding
- ✅ Verify correct pin numbers in code match hardware
- ✅ Check button wiring (should connect pin to GND)
- ✅ Test with multimeter or LED to confirm button function
- ✅ Ensure INPUT_PULLUP is enabled in code

**Problem:** Snake moves on its own or erratically
- Check for loose button connections
- Verify no short circuits between button pins
- Increase `INPUT_DEBOUNCE` value to 100ms
- Add 0.1µF capacitors across buttons

**Problem:** Reset button doesn't clear high score
- Hold button for 1+ second
- Check pin 4 connection
- Verify delay(500) in reset code executed

### High Score Issues

**Problem:** High score doesn't save
- ✅ Verify EEPROM_MAGIC_VALUE is being written
- ✅ Check that `saveHighScore()` is called after new records
- ✅ Ensure Arduino has EEPROM support (Uno/Nano do)
- ✅ Try clearing EEPROM with example sketch first

**Problem:** High score corrupted or wrong value
- Call `resetHighScore()` manually
- Check EEPROM addresses don't conflict with other projects
- Verify `EEPROM.get()` and `EEPROM.put()` used correctly

**Problem:** Can't reset high score
- Hold RESET button (pin 4) during gameplay
- Verify button is wired to pin 4 and GND
- Check `digitalRead(BTN_RESET)` returns LOW when pressed

### Game Logic Issues

**Problem:** Food spawns inside snake
- This is prevented by algorithm, but if occurring:
- Check `generateFood()` validation loop
- Verify `snake_length` is tracked correctly
- Ensure random seed initialized in `setup()`

**Problem:** Game runs too slow/fast
- Adjust `game_speed` variable (higher = slower)
- Check `DRAW_INTERVAL` isn't too long
- Verify `millis()` timing isn't blocked

**Problem:** Can't reach win condition
- Check `SNAKE_MAX` definition (should be 150)
- Verify win condition in main loop: `if (snake_length >= SNAKE_MAX)`
- Ensure food continues spawning until max length

### Memory Issues

**Problem:** Arduino resets randomly or behaves erratically
- ✅ RAM overflow - reduce `SNAKE_MAX` if needed
- ✅ Stack overflow - check for infinite recursion
- ✅ Use PROGMEM for all constant strings
- ✅ Monitor RAM usage with `freeMemory()` function

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/AmazingFeature`)
3. **Commit** your changes (`git commit -m 'Add some AmazingFeature'`)
4. **Push** to the branch (`git push origin feature/AmazingFeature`)
5. **Open** a Pull Request

### Contribution Ideas
- Add sound effects with piezo buzzer
- Implement different difficulty modes
- Add two-player mode
- Create custom food types with bonuses
- Add obstacles or maze levels
- Port to other display types

Please ensure code follows existing style and includes comments.

## 📝 Changelog

### v1.1 (Current)
- ✨ Added persistent high score storage (EEPROM)
- ✨ Added win condition (max length 150)
- ✨ Added high score reset button
- ⚡ Upgraded to full buffer mode for flicker-free display
- 🧠 Optimized memory with PROGMEM strings
- 🎯 Improved frame-rate control (fixed 24 FPS)
- 🎮 Better input debouncing (50ms)
- 📈 Adjusted difficulty curve (every 5 points)
- 🔧 Increased grid size to 6x6 pixels
- 🐛 Fixed Serial debug code leftover

### v1.0
- 🎮 Initial release
- Basic snake gameplay
- Progressive difficulty
- Collision detection
- Score tracking

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

You are free to use, modify, and distribute this code for personal and commercial projects.

## 🙏 Acknowledgments

- **U8g2 Library** by [olikraus](https://github.com/olikraus/u8g2) - Excellent OLED driver
- **Nokia** - For the original Snake game inspiration
- **Arduino Community** - For extensive documentation and support
- **Contributors** - Thank you to everyone who suggested improvements!

## 📬 Contact & Support

- **Issues:** Found a bug? [Open an issue](https://github.com/aydakikio/arduino_snake/issues)
- **Discussions:** Have questions? [Start a discussion](https://github.com/aydakikio/arduino_snake/discussions)

---
<p align="center">
  <a href="#arduino-snake-game-">Back to top ↑</a>
</p>
