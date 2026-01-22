# 🎮 SB Keyboard to Macro Keyboard Converter 🎛️  

## 📌 Overview  
This project uses an **Arduino Leonardo** (or compatible board) with a **USB Host Shield** to convert a standard **USB keyboard** into a **macro keyboard**.  
The Arduino reads key presses from the external keyboard and sends specific **commands, shortcuts, or macros** to the computer.

## 🐧 Linux Optimization Branch  
This branch includes **Linux-specific optimizations** with enhanced macro mappings for Linux desktop environments and terminal applications.

## ⚡ Performance Optimizations  
✅ **Non-blocking Timing** – Replaced all `delay()` calls with `millis()`-based state machine  
✅ **Improved Responsiveness** – USB processing continues during macro execution  
✅ **Efficient State Management** – Centralized macro execution with minimal memory overhead  
✅ **Better Performance** – No CPU wasted in blocking delay loops  

## 🖥️ Ubuntu/GNOME Window Management  
Enhanced with **Ubuntu/GNOME desktop shortcuts** for productivity:

| 🔢 Function Key | 🎯 GNOME Action | 📝 Description |
|-----------------|----------------|----------------|
| **F1** | Switch to workspace 1 | `Super+Home` |
| **F2** | Switch to workspace 2 | `Super+End` |
| **F3** | Previous workspace | `Super+Ctrl+Left` |
| **F4** | Next workspace | `Super+Ctrl+Right` |
| **F5** | Maximize window | `Super+Up` |
| **F6** | Show desktop | `Super+D` |

## 💻 VSCode Integration  
Enhanced with **developer-focused macros** for VSCode productivity (F7-F12):

| 🔢 Function Key | 🎯 VSCode Action | 📝 Description |
|------------------|-------------------|----------------|
| **F7** | `Shift+Alt+F` | Format Document |
| **F8** | `Ctrl+J` | Toggle Terminal/Panel |
| **F9** | `Ctrl+Shift+P` | Command Palette |
| **F10** | `Ctrl+P` | Quick Open File |
| **F11** | `Ctrl+/` | Toggle Line Comment |
| **F12** | `Ctrl+K S` | Save All Files |

## 🎯 Enhanced Macro System  
✅ **Ubuntu Window Management** – F1-F6 provide complete GNOME desktop control  
✅ **Two-stage Macros** – Support for complex sequences (like Ctrl+K then S)  
✅ **Developer Tools** – Essential VSCode shortcuts at your fingertips (F7-F12)  
✅ **Non-blocking Execution** – All macros use millis() timing for responsive operation  
✅ **Memory Efficient** – Minimal RAM overhead with maximum functionality |  

## 🔥 Features  
✅ **USB Keyboard Input** – Connects an external keyboard via the USB Host Shield.  
✅ **Macro Execution** – Sends custom key combinations instead of default key presses.  
✅ **Num Lock Indicator** – Uses an LED to show the Num Lock state.  
✅ **Custom Key Mappings** – Supports function keys and macros (e.g., `Ctrl + C`, `Ctrl + V`).  
✅ **Multi-key Support** – Handles modifiers like **Shift, Ctrl, and Alt**.  
✅ **Linux Optimizations** – Special mappings for Linux desktop and terminal shortcuts.  

## 🛠️ Hardware Used  

The current setup uses the following components:  
- 🔹 **Arduino Leonardo (Compatible Board)** – [Robocraze Leonardo R3 Board](https://robocraze.com/products/leonardo-r3-board-compatible-with-arduino)  
- 🔹 **USB Host Shield** – [Robocraze USB Host Shield](https://robocraze.com/products/uno-usb-host-shield-for-arduino)  

💡 *These are the best purchase options I found and personally bought. However, please do your own research before purchasing.*  

---

## ⚠️ Disclaimers  

🔴 **Use at Your Own Risk:**  
I am **not responsible** for any **fraud, hardware damage**, or issues with your **computer, microcontroller, or HID device**. Proceed at your own risk.  

🔵 **Hardcoded Macros:**  
The macros in this project are **hardcoded into the Arduino**, meaning **any keyboard should work fine**. However, **there is no room for dynamic macro addition** in this setup.  

🟢 **Expanding Functionality:**  
1️⃣ **SD Card + JSON Configuration:**  
- Adding an **SD card reader** would allow users to store macros in a JSON file.  
- This way, macros could be **modified dynamically** without reprogramming the Arduino.  

2️⃣ **QMK Firmware on Pro Micro or Raspberry Pi Pico:**  
- Switching to **QMK firmware** on a **Pro Micro** or **Pi Pico** would provide full programmability.  
- Users could dynamically reassign macros and key bindings via software.  

🟡 **PlatformIO Notice:**  
📌 This code was originally written on **PlatformIO**, so ensure that you include the **necessary header files** if using a different environment.  

---

## 🚀 Installation  

### 1️⃣ Connect the Hardware  
🔌 Attach the **USB Host Shield** to the **Arduino Leonardo**.  
⌨️ Connect the **USB keyboard** to the **USB Host Shield**.  
💡 Connect an **LED to pin 12** (optional, for Num Lock status).  

### 2️⃣ Upload the Code  
🖥️ Open the provided `.ino` file in the **Arduino IDE**.  
📍 Select **Board**: `Arduino Leonardo`.  
🔍 Select the correct **Port**.  
⬆️ Click **Upload**.  

---

## ⚙️ How It Works  
🖥️ The **Arduino Leonardo** acts as a USB Host using the `USBHost` library.  
🕹️ When a key is pressed on the external keyboard:  
- 🔍 The Arduino **detects the key**.  
- 🎭 If it is a **macro key**, the Arduino sends a **predefined macro**.  
- 💬 Otherwise, it sends the **normal key press** to the computer.  
- 💡 The **Num Lock key** toggles an **LED on pin 12**.  
- 🎯 **Special keys execute additional functions:**  
  - 🔄 `Ctrl + C` (Copy) and `Ctrl + V` (Paste) are automatically mapped.  
  - ⏬ `Page Down (F13)` and ⏭️ `End (F13)` execute additional functions.  

---

## ⌨️ Complete Key Mappings 

### 🔤 Alphabet Keys & Special Macros
| 🔢 Key | Normal | Shift | 🎯 Special Action (Linux Mode) |
|---------|---------|--------|--------------------------------|
| **A** | `a` | `A` | Normal typing |
| **B** | ❌ | ❌ | `Alt + F4` (Close window) |
| **C** | ❌ | ❌ | `Ctrl + C` (Copy) |
| **D** | `d` | `D` | Normal typing |
| **E** | `e` | `E` | Normal typing |
| **F** | `f` | `F` | Normal typing |
| **G** | `g` | `G` | Normal typing |
| **H** | `h` | `H` | Normal typing |
| **I** | `i` | `I` | Normal typing |
| **J** | `j` | `J` | Normal typing |
| **K** | `k` | `K` | Normal typing |
| **L** | `l` | `L` | Normal typing |
| **M** | `m` | `M` | Normal typing |
| **N** | `n` | `N` | Normal typing |
| **O** | `o` | `O` | Normal typing |
| **P** | `p` | `P` | Normal typing |
| **Q** | `q` | `Q` | Normal typing |
| **R** | `r` | `R` | Normal typing |
| **S** | `s` | `S` | Normal typing |
| **T** | `t` | `T` | Normal typing |
| **U** | `u` | `U` | Normal typing |
| **V** | ❌ | ❌ | `Ctrl + V` (Paste) |
| **W** | `w` | `W` | Normal typing |
| **X** | `x` | `X` | Normal typing |
| **Y** | `y` | `Y` | Normal typing |
| **Z** | `z` | `Z` | Normal typing |

### 🔢 Number Keys
| 🔢 Key | Normal | Shift | 🎯 Action |
|---------|---------|--------|-----------|
| **1** | `1` | `!` | Normal typing |
| **2** | `2` | `@` | Normal typing |
| **3** | `3` | `#` | Normal typing |
| **4** | `4` | `$` | Normal typing |
| **5** | `5` | `%` | Normal typing |
| **6** | `6` | `^` | Normal typing |
| **7** | `7` | `&` | Normal typing |
| **8** | `8` | `*` | Normal typing |
| **9** | `9` | `(` | Normal typing |
| **0** | `0` | `)` | Normal typing |

### 🔤 Symbol Keys
| 🔢 Key | Normal | Shift | Action |
|---------|---------|--------|--------|
| **-** | `-` | `_` | Normal typing |
| **=** | `=` | `+` | Normal typing |
| **[** | `[` | `{` | Normal typing |
| **]** | `]` | `}` | Normal typing |
| **\** | `\` | `|` | Normal typing |
| **;** | `;` | `:` | Normal typing |
| **'** | `'` | `"` | Normal typing |
| **`** | `` ` `` | `~` | Normal typing |
| **,** | `,` | `<` | Normal typing |
| **.** | `.` | `>` | Normal typing |
| **/** | `/` | `?` | Normal typing |

### ⌨️ Special Function Keys
| 🔢 Key | 🎯 Action | 📝 Description |
|---------|-----------|----------------|
| **Space** | `F24` | **Special output key** (remains F24) |
| **Enter** | `Enter` | New line |
| **Backspace** | `Backspace` | Delete previous character |
| **Tab** | `Tab` | Tab character |
| **Esc** | `Esc` | Escape key |
| **Num Lock** | LED Toggle | Toggles Num Lock LED & state |

### 🎛️ Function Keys (F1-F12)
| 🔢 Function Key | 🎯 Ubuntu/GNOME Action | 🎯 VSCode Action | 📝 Description |
|----------------|----------------------|-------------------|----------------|
| **F1** | `Super+Home` | - | **Switch to workspace 1** |
| **F2** | `Super+End` | - | **Switch to workspace 2** |
| **F3** | `Super+Ctrl+Left` | - | **Previous workspace** |
| **F4** | `Super+Ctrl+Right` | - | **Next workspace** |
| **F5** | `Super+Up` | - | **Maximize current window** |
| **F6** | `Super+D` | - | **Show desktop** |
| **F7** | - | `Shift+Alt+F` | **Format Document** |
| **F8** | - | `Ctrl+J` | **Toggle Terminal/Panel** |
| **F9** | - | `Ctrl+Shift+P` | **Command Palette** |
| **F10** | - | `Ctrl+P` | **Quick Open File** |
| **F11** | - | `Ctrl+/` | **Toggle Line Comment** |
| **F12** | - | `Ctrl+K S` | **Save All Files** |

### 🕹️ Navigation Keys (Linux Optimized)
| 🔢 Key | Standard Mode | Linux Mode | 📝 Description |
|---------|---------------|-------------|----------------|
| **Page Up** | `Page Up` | `Page Up` | Standard navigation |
| **Page Down** | `Page Down + F13` | `Ctrl+Alt+Down` | **Workspace switch** |
| **Home** | `Home` | `Ctrl+A` | **Terminal start of line** |
| **End** | `End + F13` | `Ctrl+E` | **Terminal end of line** |
| **Insert** | `Insert` | `Insert` | Standard insert |
| **Delete** | `Delete` | `Delete` | Standard delete |

### 🔢 Numpad (Num Lock Dependent)
| 🔢 Numpad Key | Num Lock ON | Num Lock OFF | Action |
|----------------|-------------|--------------|---------|
| **0-9** | `0-9` | ❌ | Numbers when Num Lock ON |
| **/** | `/` | ❌ | Division when Num Lock ON |
| ***** | `*` | ❌ | Multiplication when Num Lock ON |
| **-** | `-` | ❌ | Subtraction when Num Lock ON |
| **+** | `+` | ❌ | Addition when Num Lock ON |
| **Enter** | `Enter` | `Enter` | Always works |
| **.** | `.` | ❌ | Decimal point when Num Lock ON |

---

## 🎯 Macro Summary

### 🖥️ Ubuntu/GNOME Desktop Features  
✅ **Workspace Management** – F1-F2 switch between workspaces  
✅ **Window Navigation** – F3-F4 move between workspaces  
✅ **Window Control** – F5 maximize, F6 show desktop  
✅ **Super Key Integration** – Full GNOME shortcut support  

### 🚀 Performance Features
✅ **Non-blocking Timing** – All macros use `millis()` for responsive operation  
✅ **Linux Optimized** – Enhanced desktop and terminal shortcuts  
✅ **Developer Focused** – Essential VSCode shortcuts at hardware level  
✅ **Memory Efficient** – Minimal RAM overhead with maximum functionality  

### 💡 Special Notes
🔸 **Ubuntu/GNOME Control** – F1-F6 provide complete desktop and window management  
🔸 **VSCode Integration** – F7-F12 add developer productivity shortcuts  
🔸 **Space Bar** – Mapped to `F24` for special output functions  
🔸 **Num Lock** – Controls both LED and numpad number/arrow functionality  
🔸 **Two-stage Macros** – F12 uses Ctrl+K then S sequence  
🔸 **Mode Dependent** – Some keys behave differently in Linux vs Standard mode  

---

## 🔧 Troubleshooting  

### ⚠️ Common Issues
- **USB Host Shield** doesn't initialize → Check **Serial Monitor** for errors
- **LED not working** → Verify pin 12 connection and polarity  
- **Macros not working** → Ensure **LINUX_MODE** is set to `true`  
- **Keys stuck** → Restart Arduino and check USB connections  

### 🛠️ Customization
- **Modify key mappings** inside the **`KeycodeToAscii()`** function  
- **Add new macros** → Update `MacroType` enum and `startMacro()` function  
- **Change delay timing** → Adjust `MACRO_DELAY` constant (default: 10ms)  
- **Disable Linux mode** → Set `LINUX_MODE` to `false` for standard behavior  

### ⌨️ Technical Details
- **Library**: Uses **`Keyboard.h`** to send key presses to PC
- **USB Host**: **`USBHost`** library for external keyboard support
- **Platform**: **Arduino Leonardo** (ATmega32U4) with native USB HID
- **Compatibility**: Works with Windows, macOS, and Linux systems  

---

## 🔢 Hex Code Reference  
The **hex codes for each keyboard key** and how to check whether a key is usable were obtained from the following GitHub repository:  

🔗 **Hexcode Reference Repository:** [Hexcode_for_keyboard](https://github.com/Nitesh0069/Hexcode_for_keyboard)  

This repository contains all the **hex codes** required for proper key detection and macro execution.  

---

## 🎬 Inspiration  
📺 This project was inspired by the video: **[USB Host Keyboard on Arduino](https://youtu.be/GZEoss4XIgc?si=5EGJQVeLKa6sJi1d)**.  

---

## 📝 License  
📜 This project is **open-source**. Feel free to modify and share!  

🚀 **Enjoy your custom macro keyboard!** 🎛️  
