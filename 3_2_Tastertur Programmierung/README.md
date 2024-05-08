Um die Tastatursteuerung für einen Roboter einzurichten, sodass die W-, A-, S- und D-Tasten zur Bewegung in unterschiedlichen Richtungen verwendet werden können, und um eine Glättung (Smoothing) der Steuerbefehle einzuführen, können wir den folgenden Ansatz wählen:

### Code für die Tastatursteuerung mit Smoothing

Wir werden die Steuerbefehle basierend auf der Dauer des Tastendrucks anpassen. Dies ermöglicht eine feinfühlige Steuerung des Roboters, wobei die Befehle über einen Zeitraum von bis zu einer Sekunde zunehmen.

```cpp
#include <Arduino.h>
#include <Bluepad32.h>
#include <uni.h>
#include <math.h>


// Definiere hier die MAC-Adressen der erlaubten Controller
const char* allowedAddresses[] = {"98:B6:E9:01:6C:47", "AA:BB:CC:DD:EE:FF"};
const int numAllowed = sizeof(allowedAddresses) / sizeof(allowedAddresses[0]);
ControllerPtr myControllers[BP32_MAX_GAMEPADS];


int oldLeftSpeed = 0, oldRightSpeed = 0; // Alte Geschwindigkeiten für Glättung



#define NUM_KEYS 256
bool keyState[NUM_KEYS] = {false}; // Speichert den Zustand aller Tasten
unsigned long keyPressTime[NUM_KEYS] = {0}; // Speichert die Zeit, zu der jede Taste gedrückt wurde


void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Console.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Console.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Console.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}


void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Console.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Console.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}


void setSpeed(int leftPWM, int rightPWM) {
    // Hier würde normalerweise der Code stehen, um die Motoren des Roboters anzusteuern
    // Beispiel:
    //analogWrite(LEFT_MOTOR_PIN, leftPWM);
    //analogWrite(RIGHT_MOTOR_PIN, rightPWM);
}

void updateKeyPressDuration(int key, bool isPressed) {
    if (isPressed) {
        if (keyPressTime[key] == 0) { // Taste wurde gerade gedrückt
            keyPressTime[key] = millis();
        }
    } else {
        keyPressTime[key] = 0; // Taste wurde losgelassen
    }
    keyState[key] = isPressed;
}

unsigned long getKeyDuration(int key) {
    if (keyState[key] && keyPressTime[key] > 0) {
        return millis() - keyPressTime[key];
    }
    return 0;
}

void processKeyboard() {
    int pwmLeft = 0, pwmRight = 0;

    // Verarbeitung jeder Taste
    if (keyState[Keyboard_W]) {
        int speed = map(min(getKeyDuration(Keyboard_W), 500UL), 0, 500, 0, 255);
        pwmLeft += speed;
        pwmRight += speed;
    }
    if (keyState[Keyboard_S]) {
        int speed = map(min(getKeyDuration(Keyboard_S), 500UL), 0, 500, 0, 255);
        pwmLeft -= speed;
        pwmRight -= speed;
    }
    if (keyState[Keyboard_A]) {
        int turn = map(min(getKeyDuration(Keyboard_A), 500UL), 0, 500, 0, 255);
        pwmLeft -= turn;
        pwmRight += turn;
    }
    if (keyState[Keyboard_D]) {
        int turn = map(min(getKeyDuration(Keyboard_D), 500UL), 0, 500, 0, 255);
        pwmLeft += turn;
        pwmRight -= turn;
    }
    float deltaL = pwmLeft * 0.1;
    // Prüfe und passe an, falls die Änderung kleiner als 1 oder -1 ist
    if (deltaL > 0 && deltaL < 1) {
        deltaL = 1;  // Minimale positive Anpassung
    } else if (deltaL < 0 && deltaL > -1) {
        deltaL = -1;  // Minimale negative Anpassung
    }
    float deltaR = pwmRight * 0.1;
    // Prüfe und passe an, falls die Änderung kleiner als 1 oder -1 ist
    if (deltaR > 0 && deltaR < 1) {
        deltaR = 1;  // Minimale positive Anpassung
    } else if (deltaR < 0 && deltaR > -1) {
        deltaR = -1;  // Minimale negative Anpassung
    }
    // Glättung der Geschwindigkeitsänderungen
    pwmLeft = oldLeftSpeed * 0.9 + deltaL;
    pwmRight = oldRightSpeed * 0.9 + deltaR;

    // Speichere die aktuellen Geschwindigkeiten für den nächsten Durchlauf
    oldLeftSpeed = pwmLeft;
    oldRightSpeed = pwmRight;

    // Geschwindigkeiten begrenzen, um Überlauf zu vermeiden
    pwmLeft = constrain(pwmLeft, -255, 255);
    pwmRight = constrain(pwmRight, -255, 255);

    setSpeed(pwmLeft, pwmRight);

    Serial.print("Setze Geschwindigkeit: Links = ");
    Serial.print(pwmLeft);
    Serial.print(", Rechts = ");
    Serial.println(pwmRight);
}

void onKeyPressEvent(int key, bool isPressed) {
    updateKeyPressDuration(key, isPressed);
}

void setup() {
    Serial.begin(115200);
        // Füge die erlaubten MAC-Adressen zur Allowlist hinzu
    for (int i = 0; i < numAllowed; ++i) {
        bd_addr_t addr;
        sscanf_bd_addr(allowedAddresses[i], addr);
        uni_bt_allowlist_add_addr(addr);
    }

    // Aktiviere die Whitelist-Überprüfung
    uni_bt_allowlist_set_enabled(false);
	
    BP32.setup(onConnectedController, onDisconnectedController);
}

void loop() {
    if (BP32.update()) {
        // Eventuell eingehende Tastaturereignisse verarbeiten
        for (auto controller : myControllers) {
            if (controller && controller->isConnected() && controller->isKeyboard()) {
                // Beispielsweise könnten Tastaturereignisse so verarbeitet werden:
                if (controller->isKeyPressed(Keyboard_W)) {
                    onKeyPressEvent(Keyboard_W, true);
                } else {
                    onKeyPressEvent(Keyboard_W, false);
                }

                // S-Taste für Rückwärts
                if (controller->isKeyPressed(Keyboard_S)) {
                    onKeyPressEvent(Keyboard_S, true);
                } else {
                    onKeyPressEvent(Keyboard_S, false);
                }

                // A-Taste für Links
                if (controller->isKeyPressed(Keyboard_A)) {
                    onKeyPressEvent(Keyboard_A, true);
                } else {
                    onKeyPressEvent(Keyboard_A, false);
                }

                // D-Taste für Rechts
                if (controller->isKeyPressed(Keyboard_D)) {
                    onKeyPressEvent(Keyboard_D, true);
                } else {
                    onKeyPressEvent(Keyboard_D, false);
                }
            }
        }
    }

    // Verarbeite die aktuellen Tastenzustände, um die Geschwindigkeit zu aktualisieren
    processKeyboard();
    delay(10);  // Kurze Pause zur Schonung der CPU
}
```

### Funktionsweise des Codes

1. **Key Smoothing**: Für jede Taste wird die Zeit gemessen, wie lange sie gedrückt wird. Die Geschwindigkeit und die Richtung der Motoren werden entsprechend der Druckdauer angepasst. Dies sorgt für eine glattere und präzisere Steuerung.

2. **Verarbeitung der Tastendrücke**: Es wird überprüft, ob eine der W-, A-, S- oder D-Tasten gedrückt wird, und die PWM-Werte für die Motoren werden entsprechend angepasst.

3. **PWM-Werte**: Die berechneten PWM-Werte werden begrenzt, um sie innerhalb des gültigen Bereichs für die Motorsteuerung zu halten.

Durch dieses Setup können die Tastatureingaben den Roboter auf eine kontrollierte und sanfte Weise steuern, wobei die maximale Reaktionszeit auf eine Sekunde beschränkt ist. Dies kann besonders nützlich sein, um eine präzise Navigation in komplexen Umgebungen zu ermöglichen.