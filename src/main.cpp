#include <usbhub.h>
#include <hiduniversal.h>
#include <SPI.h>
#include <Keyboard.h>  // Arduino Leonardo acts as a keyboard


#define LED_PIN 12  // LED connected to pin 12
#define LINUX_MODE true  // Enable Linux-specific optimizations
#define MACRO_DELAY 10  // Non-blocking macro delay in milliseconds

// Macro types for state management
enum MacroType {
    NO_MACRO = 0,
    ALT_F4_MACRO = 1,
    CTRL_ALT_F_MACRO = 2,
    CTRL_C_MACRO = 3,
    CTRL_V_MACRO = 4,
    CTRL_ALT_DOWN_MACRO = 5,
    PAGE_DOWN_F13_MACRO = 6,
    CTRL_A_MACRO = 7,
    CTRL_E_MACRO = 8,
    END_F13_MACRO = 9,
    // VSCode specific macros
    VSCODE_FORMAT_MACRO = 10,     // Shift+Alt+F
    VSCODE_TOGGLE_PANEL_MACRO = 11, // Ctrl+J
    VSCODE_COMMAND_PALETTE_MACRO = 12, // Ctrl+Shift+P
    VSCODE_QUICK_OPEN_MACRO = 13,   // Ctrl+P
    VSCODE_NEW_TERMINAL_MACRO = 14,  // Ctrl+Shift+`
    VSCODE_COMMENT_LINE_MACRO = 15,  // Ctrl+/
    VSCODE_SAVE_ALL_MACRO = 16,      // Ctrl+K S
    VSCODE_ZOOM_IN_MACRO = 17,       // Ctrl+=
    VSCODE_ZOOM_OUT_MACRO = 18       // Ctrl+-
};

// Macro state management structure
struct MacroState {
    bool inProgress = false;
    unsigned long startTime = 0;
    MacroType macroType = NO_MACRO;
    bool keysPressed = false;
    bool secondStage = false; // For two-stage macros like Ctrl+K then S
};

USB Usb;
HIDUniversal Hid(&Usb);
bool numLockActive = false; // Track Num Lock state
MacroState currentMacro; // Current macro execution state

// Helper function implementations (must be defined before use)
// Removed getMacroName to save RAM - use numbers instead

// Removed getKeyDisplayName to save RAM - use hex codes instead

// Macro management functions
void startMacro(MacroType type) {
    currentMacro.inProgress = true;
    currentMacro.startTime = millis();
    currentMacro.macroType = type;
    currentMacro.keysPressed = false;
    
    Serial.print("Macro ");
    Serial.print(type);
    Serial.println(" started");
    
    // Press the appropriate keys immediately
    switch (type) {
        case ALT_F4_MACRO:
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_F4);
            break;
        case CTRL_ALT_F_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press('f');
            break;
        case CTRL_C_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('c');
            break;
        case CTRL_V_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('v');
            break;
        case CTRL_ALT_DOWN_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_DOWN_ARROW);
            break;
        case PAGE_DOWN_F13_MACRO:
            Keyboard.press(KEY_PAGE_DOWN);
            Keyboard.press(KEY_F13);
            break;
        case CTRL_A_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('a');
            break;
        case CTRL_E_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('e');
            break;
        case END_F13_MACRO:
            Keyboard.press(KEY_END);
            Keyboard.press(KEY_F13);
            break;
        case VSCODE_FORMAT_MACRO:
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press('f');
            break;
        case VSCODE_TOGGLE_PANEL_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('j');
            break;
        case VSCODE_COMMAND_PALETTE_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('p');
            break;
        case VSCODE_QUICK_OPEN_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('p');
            break;
        case VSCODE_NEW_TERMINAL_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('`');
            break;
        case VSCODE_COMMENT_LINE_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('/');
            break;
        case VSCODE_SAVE_ALL_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('k');
            break;
        case VSCODE_ZOOM_IN_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('=');
            break;
        case VSCODE_ZOOM_OUT_MACRO:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('-');
            break;
        default:
            currentMacro.inProgress = false;
            break;
    }
    currentMacro.keysPressed = true;
}

void processMacros() {
    if (!currentMacro.inProgress) return;
    
    unsigned long elapsed = millis() - currentMacro.startTime;
    
    // Handle two-stage macros (like Ctrl+K then S for Save All)
    if (currentMacro.macroType == VSCODE_SAVE_ALL_MACRO && !currentMacro.secondStage) {
        if (elapsed >= MACRO_DELAY / 2) {
            Keyboard.release(KEY_LEFT_CTRL);
            Keyboard.release('k');
            delayMicroseconds(5000); // Small delay between stages
            Keyboard.press('s');
            currentMacro.secondStage = true;
            currentMacro.startTime = millis(); // Reset timer for second stage
        }
        return;
    }
    
    // Check if it's time to release the keys
    if (elapsed >= MACRO_DELAY) {
        Keyboard.releaseAll();
        Serial.println("Macro completed");
        currentMacro.inProgress = false;
        currentMacro.macroType = NO_MACRO;
        currentMacro.keysPressed = false;
        currentMacro.secondStage = false;
    }
}

class KeyboardHandler : public HIDReportParser {
protected:
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) override {
        if (len > 0) {
            uint8_t modifiers = buf[0]; // Modifier keys (Shift, Ctrl, Alt)
            bool shiftPressed = (modifiers & 0x22); // Left Shift (0x02) or Right Shift (0x20)
            bool ctrlPressed = (modifiers & 0x11); // Left Ctrl (0x01) or Right Ctrl (0x10)
            bool altPressed = (modifiers & 0x44); // Left Alt (0x04) or Right Alt (0x40)

            for (uint8_t i = 2; i < len; i++) {
                if (buf[i] > 0) {
                    uint8_t keycode = buf[i];
                    Serial.print("Key: 0x");
                    Serial.print(keycode, HEX);

                    if (keycode == 0x53) { // Num Lock key
                        numLockActive = !numLockActive; // Toggle Num Lock state
                        digitalWrite(LED_PIN, numLockActive ? HIGH : LOW); // Control LED
                        Serial.println(" -> Num Lock");
                        continue; // Skip processing as macro
                    }
                    
                    char c = KeycodeToAscii(keycode, shiftPressed);
                    
                    if (c) {
                        Serial.print(" -> Char: '");
                        Serial.print(c);
                        Serial.println("'");
                        Keyboard.press(c);   // Press key
                        delayMicroseconds(10000); // 10ms equivalent for single keys
                        Keyboard.release(c); // Release key
                    } else {
                        Serial.println(" -> Macro/Other");
                    }
                }
            }
        }
    }



    char KeycodeToAscii(uint8_t keycode, bool shift) {
        switch (keycode) {
            // Letter keys (A-Z)
            case 0x04: return shift ? 'A' : 'a';
            case 0x05:   // 'b' key - Linux Alt+F4 (close window)
            #if LINUX_MODE
            startMacro(ALT_F4_MACRO);
            #else
            startMacro(CTRL_ALT_F_MACRO);
            #endif
            return 0;                        // Do not send 'b' or 'B'

            case 0x06:  // 'C' - Copy
            startMacro(CTRL_C_MACRO);
            return 0; // Don't send 'C' again

            case 0x07: return shift ? 'D' : 'd';
            case 0x08: return shift ? 'E' : 'e';
            case 0x09: return shift ? 'F' : 'f';
            case 0x0A: return shift ? 'G' : 'g';
            case 0x0B: return shift ? 'H' : 'h';
            case 0x0C: return shift ? 'I' : 'i';
            case 0x0D: return shift ? 'J' : 'j';
            case 0x0E: return shift ? 'K' : 'k';
            case 0x0F: return shift ? 'L' : 'l';
            case 0x10: return shift ? 'M' : 'm';
            case 0x11: return shift ? 'N' : 'n';
            case 0x12: return shift ? 'O' : 'o';
            case 0x13: return shift ? 'P' : 'p';
            case 0x14: return shift ? 'Q' : 'q';
            case 0x15: return shift ? 'R' : 'r';
            case 0x16: return shift ? 'S' : 's';
            case 0x17: return shift ? 'T' : 't';
            case 0x18: return shift ? 'U' : 'u';
            case 0x19:  // 'V' - Paste (works on Linux too)
            startMacro(CTRL_V_MACRO);
            return 0;
            case 0x1A: return shift ? 'W' : 'w';
            case 0x1B: return shift ? 'X' : 'x';
            case 0x1C: return shift ? 'Y' : 'y';
            case 0x1D: return shift ? 'Z' : 'z';

            // Number keys (0-9, differentiating numpad keys)
            case 0x1E: return shift ? '!' : '1';
            case 0x1F: return shift ? '@' : '2';
            case 0x20: return shift ? '#' : '3';
            case 0x21: return shift ? '$' : '4';
            case 0x22: return shift ? '%' : '5';
            case 0x23: return shift ? '^' : '6';
            case 0x24: return shift ? '&' : '7';
            case 0x25: return shift ? '*' : '8';
            case 0x26: return shift ? '(' : '9';
            case 0x27: return shift ? ')' : '0';

            // Symbols and special characters
            case 0x2D: return shift ? '_' : '-';
            case 0x2E: return shift ? '+' : '=';
            case 0x2F: return shift ? '{' : '[';
            case 0x30: return shift ? '}' : ']';
            case 0x31: return shift ? '|' : '\\';
            case 0x33: return shift ? ':' : ';';
            case 0x34: return shift ? '"' : '\'';
            case 0x35: return shift ? '~' : '`';
            case 0x36: return shift ? '<' : ',';
            case 0x37: return shift ? '>' : '.';
            case 0x38: return shift ? '?' : '/';

            // Special keys
            case 0x2C: return KEY_F24; // Spacebar
            case 0x28: return '\n'; // Enter
            case 0x2A: return '\b'; // Backspace
            case 0x2B: return '\t'; // Tab
            case 0x29: return KEY_ESC; // ASCII for Escape (ESC)
                
            // Numpad keys (now dependent on Num Lock state)
            case 0x62: return numLockActive ? '0' : 0;
            case 0x59: return numLockActive ? '1' : 0;
            case 0x5A: return numLockActive ? '2' : 0;
            case 0x5B: return numLockActive ? '3' : 0;
            case 0x5C: return numLockActive ? '4' : 0;
            case 0x5D: return numLockActive ? '5' : 0;
            case 0x5E: return numLockActive ? '6' : 0;
            case 0x5F: return numLockActive ? '7' : 0;
            case 0x60: return numLockActive ? '8' : 0;
            case 0x61: return numLockActive ? '9' : 0;
         
            // Numpad operators now affected by Num Lock
            case 0x54: return numLockActive ? '/' : 0;
            case 0x55: return numLockActive ? '*' : 0;
            case 0x56: return numLockActive ? '-' : 0;
            case 0x57: return numLockActive ? '+' : 0;
            case 0x58: return numLockActive ? '\n' : 0;
            case 0x63: return numLockActive ? '.' : 0;
            // Function keys (F1-F12) - VSCode macros mapped to F7-F12
            case 0x3A: return KEY_F1;
            case 0x3B: return KEY_F2;
            case 0x3C: return KEY_F3;
            case 0x3D: return KEY_F4;
            case 0x3E: return KEY_F5;
            case 0x3F: return KEY_F6;
            case 0x40:   // F7 - VSCode Format Document (Shift+Alt+F)
            startMacro(VSCODE_FORMAT_MACRO);
            return 0;
            case 0x41:   // F8 - VSCode Toggle Panel (Ctrl+J)
            startMacro(VSCODE_TOGGLE_PANEL_MACRO);
            return 0;
            case 0x42:   // F9 - VSCode Command Palette (Ctrl+Shift+P)
            startMacro(VSCODE_COMMAND_PALETTE_MACRO);
            return 0;
            case 0x43:   // F10 - VSCode Quick Open (Ctrl+P)
            startMacro(VSCODE_QUICK_OPEN_MACRO);
            return 0;
            case 0x44:   // F11 - VSCode Comment Line (Ctrl+/)
            startMacro(VSCODE_COMMENT_LINE_MACRO);
            return 0;
            case 0x45:   // F12 - VSCode Save All (Ctrl+K S)
            startMacro(VSCODE_SAVE_ALL_MACRO);
            return 0;

            // Page up, Page down, Home, End, Insert, Delete
            case 0x4B: return KEY_PAGE_UP;// Page up
            case 0x4E: // Page Down - Linux workspace switch
            #if LINUX_MODE
            startMacro(CTRL_ALT_DOWN_MACRO);
            #else
            startMacro(PAGE_DOWN_F13_MACRO);
            #endif
            return 0;
            case 0x4A: // Home - Linux terminal start of line
            #if LINUX_MODE
            startMacro(CTRL_A_MACRO);
            return 0;
            #else
            return KEY_HOME;// Home
            #endif
            case 0x4D: // End - Linux terminal end of line
            #if LINUX_MODE
            startMacro(CTRL_E_MACRO);
            #else
            startMacro(END_F13_MACRO);
            #endif
            return 0;
            // End
            case 0x49: return KEY_INSERT;// Insert
            case 0x4C: return KEY_DELETE;// Delete

            default: return 0;
        }
    }
};

KeyboardHandler MyKeyboard;

void setup() {
    Serial.begin(115200);
    Keyboard.begin();
    pinMode(LED_PIN, OUTPUT); // Set LED pin as output

    Serial.println("=== Arduino Macro Keyboard ===");
    Serial.println("Initializing...");
    
    if (Usb.Init() == -1) {
        Serial.println("ERROR: USB Host Shield failed!");
        while (1) {
            digitalWrite(LED_PIN, HIGH);
            delay(100);
            digitalWrite(LED_PIN, LOW);
            delay(100);
        }
    }
    
    Hid.SetReportParser(0, &MyKeyboard);
    Serial.println("SUCCESS: USB Host Shield ready");
    Serial.println("Waiting for keyboard input...");
    Serial.println("==============================");
}

void loop() {
    Usb.Task(); // Keep checking for updates
    processMacros(); // Process any ongoing macro executions
    
    // Simple heartbeat every 10 seconds
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat >= 10000) {
        lastHeartbeat = millis();
        Serial.print("Heartbeat - NumLock: ");
        Serial.print(numLockActive ? "ON" : "OFF");
        Serial.print(" Macro: ");
        Serial.println(currentMacro.inProgress ? "YES" : "NO");
    }
    }