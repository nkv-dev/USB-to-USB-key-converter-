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

## 💻 VSCode Integration  
Enhanced with **developer-focused macros** for VSCode productivity:

| 🔢 Function Key | 🎯 VSCode Action | 📝 Description |
|------------------|-------------------|----------------|
| **F7** | `Shift+Alt+F` | Format Document |
| **F8** | `Ctrl+J` | Toggle Terminal/Panel |
| **F9** | `Ctrl+Shift+P` | Command Palette |
| **F10** | `Ctrl+P` | Quick Open File |
| **F11** | `Ctrl+/` | Toggle Line Comment |
| **F12** | `Ctrl+K S` | Save All Files |

## 🎯 Enhanced Macro System  
✅ **Two-stage Macros** – Support for complex sequences (like Ctrl+K then S)  
✅ **Developer Tools** – Essential VSCode shortcuts at your fingertips  
✅ **Non-blocking Execution** – All VSCode macros use millis() timing  
✅ **Memory Efficient** – Only 1 byte additional RAM for VSCode features |  

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

## ⌨️ Example Key Mappings  

| 🔢 Key        | 🎯 Action (Linux Mode)                          |
|--------------|--------------------------------|
| 🅲 `C`       | `Ctrl + C` (Copy)              |
| 🆅 `V`       | `Ctrl + V` (Paste)             |
| 🔢 `Num Lock` | Toggles LED and Num Lock state |
| ⏬ `Page Down` | `Ctrl + Alt + Down` (Workspace switch) |
| ⏭️ `End`      | `Ctrl + E` (Terminal end of line) |
| 🏠 `Home`     | `Ctrl + A` (Terminal start of line) |
| ❌ `Esc`      | Sends Escape                   |
| `~`         | Sends `~` character            |
| 🅱 `B`       | `Alt + F4` (Close window)      |

### 💻 Developer VSCode Shortcuts
| 🔢 Function Key | 🎯 VSCode Action | 📝 Description |
|----------------|-------------------|----------------|
| **F7** | `Shift+Alt+F` | Format Document |
| **F8** | `Ctrl+J` | Toggle Terminal/Panel |
| **F9** | `Ctrl+Shift+P` | Command Palette |
| **F10** | `Ctrl+P` | Quick Open File |
| **F11** | `Ctrl+/` | Toggle Line Comment |
| **F12** | `Ctrl+K S` | Save All Files |

---

## 🔧 Troubleshooting  
⚠️ If the **USB Host Shield** does not initialize, check the **Serial Monitor** for errors.  
🛠️ You can modify key mappings inside the **`KeycodeToAscii()`** function.  
⌨️ The project uses **`Keyboard.h`** to send key presses to the PC.  

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
