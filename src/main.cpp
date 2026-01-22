#include <usbhub.h>
#include <hiduniversal.h>
#include <SPI.h>
#include <Keyboard.h>  // Arduino Leonardo acts as a keyboard
#include <WString.h>  // For String class

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
String getMacroName(MacroType type) {
    switch (type) {
        case ALT_F4_MACRO: return "Alt+F4 (Close Window)";
        case CTRL_ALT_F_MACRO: return "Ctrl+Alt+F";
        case CTRL_C_MACRO: return "Ctrl+C (Copy)";
        case CTRL_V_MACRO: return "Ctrl+V (Paste)";
        case CTRL_ALT_DOWN_MACRO: return "Ctrl+Alt+Down (Workspace)";
        case PAGE_DOWN_F13_MACRO: return "Page Down+F13";
        case CTRL_A_MACRO: return "Ctrl+A (Terminal Start)";
        case CTRL_E_MACRO: return "Ctrl+E (Terminal End)";
        case END_F13_MACRO: return "End+F13";
        case VSCODE_FORMAT_MACRO: return "VSCode Format Document";
        case VSCODE_TOGGLE_PANEL_MACRO: return "VSCode Toggle Panel";
        case VSCODE_COMMAND_PALETTE_MACRO: return "VSCode Command Palette";
        case VSCODE_QUICK_OPEN_MACRO: return "VSCode Quick Open";
        case VSCODE_NEW_TERMINAL_MACRO: return "VSCode New Terminal";
        case VSCODE_COMMENT_LINE_MACRO: return "VSCode Toggle Comment";
        case VSCODE_SAVE_ALL_MACRO: return "VSCode Save All";
        case VSCODE_ZOOM_IN_MACRO: return "VSCode Zoom In";
        case VSCODE_ZOOM_OUT_MACRO: return "VSCode Zoom Out";
        default: return "Unknown";
    }
}

String getKeyDisplayName(uint8_t keycode) {
    switch (keycode) {
        case 0x04: case 0x05: case 0x06: case 0x07: case 0x08: case 0x09: case 0x0A: 
        case 0x0B: case 0x0C: case 0x0D: case 0x0E: case 0x0F: case 0x10: 
        case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: 
        case 0x17: case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D:
            return String(char('A' + (keycode - 0x04)));
        case 0x1E: case 0x1F: case 0x20: case 0x21: case 0x22: case 0x23: 
        case 0x24: case 0x25: case 0x26: case 0x27:
            return String(char('1' + (keycode - 0x1E)));
        case 0x28: return "Enter";
        case 0x29: return "Esc";
        case 0x2A: return "Backspace";
        case 0x2B: return "Tab";
        case 0x2C: return "Spacebar (F24)";
        case 0x2D: return "-";
        case 0x2E: return "=";
        case 0x2F: return "[";
        case 0x30: return "]";
        case 0x31: return "\\";
        case 0x33: return ";";
        case 0x34: return "'";
        case 0x35: return "`";
        case 0x36: return ",";
        case 0x37: return ".";
        case 0x38: return "/";
        case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F:
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45:
            return "F" + String(1 + (keycode - 0x3A));
        case 0x49: return "Insert";
        case 0x4A: return "Home";
        case 0x4B: return "Page Up";
        case 0x4C: return "Delete";
        case 0x4D: return "End";
        case 0x4E: return "Page Down";
        case 0x53: return "Num Lock";
        case 0x54: return "Numpad /";
        case 0x55: return "Numpad *";
        case 0x56: return "Numpad -";
        case 0x57: return "Numpad +";
        case 0x58: return "Numpad Enter";
        case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E:
        case 0x5F: case 0x60: case 0x61: case 0x62:
            return "Numpad " + String(char('0' + (keycode - 0x59)));
        case 0x63: return "Numpad .";
        default: return "";
    }
}

// Macro management functions
void startMacro(MacroType type) {
    currentMacro.inProgress = true;
    currentMacro.startTime = millis();
    currentMacro.macroType = type;
    currentMacro.keysPressed = false;
    
    Serial.print("🚀 Macro started: ");
    Serial.print(getMacroName(type));
    Serial.print(" (");
    Serial.print(type);
    Serial.println(")");
    
    // Press the appropriate keys immediately
    switch (type) {
        case ALT_F4_MACRO:
            Serial.println("📤 Pressing: Alt + F4");
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_F4);
            break;
        case CTRL_ALT_F_MACRO:
            Serial.println("📤 Pressing: Ctrl + Alt + F");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press('f');
            break;
        case CTRL_C_MACRO:
            Serial.println("📤 Pressing: Ctrl + C (Copy)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('c');
            break;
        case CTRL_V_MACRO:
            Serial.println("📤 Pressing: Ctrl + V (Paste)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('v');
            break;
        case CTRL_ALT_DOWN_MACRO:
            Serial.println("📤 Pressing: Ctrl + Alt + ↓ (Workspace switch)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_DOWN_ARROW);
            break;
        case PAGE_DOWN_F13_MACRO:
            Serial.println("📤 Pressing: Page Down + F13");
            Keyboard.press(KEY_PAGE_DOWN);
            Keyboard.press(KEY_F13);
            break;
        case CTRL_A_MACRO:
            Serial.println("📤 Pressing: Ctrl + A (Terminal start)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('a');
            break;
        case CTRL_E_MACRO:
            Serial.println("📤 Pressing: Ctrl + E (Terminal end)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('e');
            break;
        case END_F13_MACRO:
            Serial.println("📤 Pressing: End + F13");
            Keyboard.press(KEY_END);
            Keyboard.press(KEY_F13);
            break;
        // VSCode specific macros
        case VSCODE_FORMAT_MACRO:
            Serial.println("📤 Pressing: Shift + Alt + F (Format)");
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press('f');
            break;
        case VSCODE_TOGGLE_PANEL_MACRO:
            Serial.println("📤 Pressing: Ctrl + J (Toggle Panel)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('j');
            break;
        case VSCODE_COMMAND_PALETTE_MACRO:
            Serial.println("📤 Pressing: Ctrl + Shift + P (Command Palette)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('p');
            break;
        case VSCODE_QUICK_OPEN_MACRO:
            Serial.println("📤 Pressing: Ctrl + P (Quick Open)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('p');
            break;
        case VSCODE_NEW_TERMINAL_MACRO:
            Serial.println("📤 Pressing: Ctrl + Shift + ` (New Terminal)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('`');
            break;
        case VSCODE_COMMENT_LINE_MACRO:
            Serial.println("📤 Pressing: Ctrl + / (Toggle Comment)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('/');
            break;
        case VSCODE_SAVE_ALL_MACRO:
            Serial.println("📤 Pressing: Ctrl + K (Stage 1 of Save All)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('k');
            // We'll handle 's' key in a sequential manner for Ctrl+K then S
            break;
        case VSCODE_ZOOM_IN_MACRO:
            Serial.println("📤 Pressing: Ctrl + = (Zoom In)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('=');
            break;
        case VSCODE_ZOOM_OUT_MACRO:
            Serial.println("📤 Pressing: Ctrl + - (Zoom Out)");
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('-');
            break;
        default:
            Serial.println("❌ Unknown macro type!");
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
            Serial.println("🔄 Stage 2: Releasing Ctrl+K, pressing 's'");
            Keyboard.release(KEY_LEFT_CTRL);
            Keyboard.release('k');
            delayMicroseconds(5000); // Small delay between stages
            Keyboard.press('s');
            currentMacro.secondStage = true;
            currentMacro.startTime = millis(); // Reset timer for second stage
            Serial.println("✅ Save All macro stage 2 complete");
        }
        return;
    }
    
    // Check if it's time to release the keys
    if (elapsed >= MACRO_DELAY) {
        Serial.print("⏰ Releasing macro keys after ");
        Serial.print(elapsed);
        Serial.println("ms");
        Keyboard.releaseAll();
        Serial.println("🔓 All keys released");
        
        Serial.print("✅ Macro completed: ");
        Serial.println(getMacroName(currentMacro.macroType));
        
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

            // Log modifier state changes
            static uint8_t lastModifiers = 0;
            if (modifiers != lastModifiers) {
                Serial.print("🎹 Modifiers: ");
                Serial.print("Shift:"); Serial.print(shiftPressed ? "ON" : "OFF");
                Serial.print(" Ctrl:"); Serial.print(ctrlPressed ? "ON" : "OFF");
                Serial.print(" Alt:"); Serial.println(altPressed ? "ON" : "OFF");
                lastModifiers = modifiers;
            }

            for (uint8_t i = 2; i < len; i++) { // Start from index 2 (actual keypresses)
                if (buf[i] > 0) {
                    uint8_t keycode = buf[i]; // Get keycode
                    
                    // Log key detection
                    Serial.print("⌨️  Key detected: 0x");
                    Serial.print(keycode, HEX);
                    Serial.print(" (");
                    Serial.print(i - 2);
                    Serial.print(")");
                    
                    // Identify key by name
                    String keyName = getKeyDisplayName(keycode);
                    if (keyName != "") {
                        Serial.print(" - ");
                        Serial.print(keyName);
                    }
                    
                    if (shiftPressed) Serial.print(" [SHIFT]");
                    if (ctrlPressed) Serial.print(" [CTRL]");
                    if (altPressed) Serial.print(" [ALT]");
                    Serial.println();

                    if (keycode == 0x53) { // Num Lock key
                        numLockActive = !numLockActive; // Toggle Num Lock state
                        digitalWrite(LED_PIN, numLockActive ? HIGH : LOW); // Control LED
                        Serial.print("🔢 Num Lock: ");
                        Serial.print(numLockActive ? "ON ✅" : "OFF ❌");
                        Serial.print(" | LED: ");
                        Serial.println(numLockActive ? "HIGH" : "LOW");
                        continue; // Skip processing as macro
                    }
                    
                    char c = KeycodeToAscii(keycode, shiftPressed);
                    
                    if (c) {
                        Serial.print("📤 Sending character: '");
                        Serial.print(c);
                        Serial.println("'");
                        Keyboard.press(c);   // Press key
                        // Simple delay for single key press - using micro delay
                        delayMicroseconds(10000); // 10ms equivalent for single keys
                        Keyboard.release(c); // Release key
                        Serial.println("✅ Key released");
                    } else if (currentMacro.inProgress) {
                        Serial.println("🚀 Macro started - non-blocking execution");
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

    Serial.println("=== Arduino Macro Keyboard Converter ===");
    Serial.println("Initializing USB Host Shield...");
    
    if (Usb.Init() == -1) {
        Serial.println("❌ USB Host Shield initialization failed!");
        Serial.println("Check connections and reset Arduino.");
        while (1) {
            digitalWrite(LED_PIN, HIGH);
            delay(100);
            digitalWrite(LED_PIN, LOW);
            delay(100);
        }
    }
    
    Hid.SetReportParser(0, &MyKeyboard);
    
    #if LINUX_MODE
    Serial.println("✅ Linux optimization mode ENABLED");
    Serial.println("🎯 Special Linux mappings:");
    Serial.println("   B → Alt+F4 (Close window)");
    Serial.println("   Home → Ctrl+A (Terminal start)");
    Serial.println("   End → Ctrl+E (Terminal end)");
    Serial.println("   Page Down → Ctrl+Alt+↓ (Workspace switch)");
    #else
    Serial.println("✅ Standard mode ENABLED");
    #endif
    
    Serial.println("💻 VSCode developer macros:");
    Serial.println("   F7 → Format Document (Shift+Alt+F)");
    Serial.println("   F8 → Toggle Terminal (Ctrl+J)");
    Serial.println("   F9 → Command Palette (Ctrl+Shift+P)");
    Serial.println("   F10 → Quick Open (Ctrl+P)");
    Serial.println("   F11 → Toggle Comment (Ctrl+/)");
    Serial.println("   F12 → Save All (Ctrl+K S)");
    
    Serial.println("📝 Copy/Paste macros:");
    Serial.println("   C → Ctrl+C (Copy)");
    Serial.println("   V → Ctrl+V (Paste)");
    
    Serial.println("🎹 Spacebar → F24 (Special output)");
    Serial.println("🔢 Num Lock → LED toggle + numpad mode");
    Serial.println("✨ Ready for keyboard input!");
    Serial.println("=====================================");
}

void loop() {
    Usb.Task(); // Keep checking for updates
    processMacros(); // Process any ongoing macro executions
    
    // Optional: Add heartbeat to show system is running
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat >= 30000) { // Every 30 seconds
        lastHeartbeat = millis();
        Serial.println("💓 System heartbeat - USB Host Shield active");
        Serial.print("🔢 Num Lock status: ");
        Serial.println(numLockActive ? "ON" : "OFF");
        Serial.print("⚡ Macro in progress: ");
        Serial.println(currentMacro.inProgress ? "YES" : "NO");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    }
    }