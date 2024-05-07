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

// Timing für Smoothing
unsigned long keyPressTime[256];  // Speichert die Zeit, zu der jede Taste gedrückt wurde

void onConnectedController(ControllerPtr ctl) {
    Serial.println("Gerät verbunden");
}

void onDisconnectedController(ControllerPtr ctl) {
    Serial.println("Gerät getrennt");
}

void setSpeed(int leftPWM, int rightPWM) {
    // Hier würde normalerweise der Code stehen, um die Motoren des Roboters anzusteuern
    // Beispiel:
    //analogWrite(LEFT_MOTOR_PIN, leftPWM);
    //analogWrite(RIGHT_MOTOR_PIN, rightPWM);
}

unsigned long updateKeyPressDuration(int key) {
    if (keyPressTime[key] == 0) {
        // Taste wurde gerade gedrückt
        keyPressTime[key] = millis();
        return 0;
    } else {
        // Taste ist gedrückt, berechne wie lange
        return millis() - keyPressTime[key];
    }
}

void clearKeyPressDuration(int key) {
    keyPressTime[key] = 0;
}

void processKeyboard(ControllerPtr ctl) {
    int pwmLeft = 0;
    int pwmRight = 0;

    // W-Taste für Vorwärts
    if (ctl->isKeyPressed(Keyboard_W)) {
        unsigned long duration = updateKeyPressDuration(Keyboard_W);
        int speed = map(min(duration, (unsigned long)1000), 0, 1000, 0, 255);
        pwmLeft += speed;
        pwmRight += speed;
    }

    // S-Taste für Rückwärts
    if (ctl->isKeyPressed(Keyboard_S)) {
        unsigned long duration = updateKeyPressDuration(Keyboard_S);
        int speed = map(min(duration, (unsigned long)1000), 0, 1000, 0, 255);
        pwmLeft -= speed;
        pwmRight -= speed;
    }

    // A-Taste für Links
    if (ctl->isKeyPressed(Keyboard_A)) {
        unsigned long duration = updateKeyPressDuration(Keyboard_A);
        int turn = map(min(duration, (unsigned long)1000), 0, 1000, 0, 255);
        pwmLeft -= turn;
        pwmRight += turn;
    }

    // D-Taste für Rechts
    if (ctl->isKeyPressed(Keyboard_D)) {
        unsigned long duration = updateKeyPressDuration(Keyboard_D);
        int turn = map(min(duration, (unsigned long)1000), 0, 1000, 0, 255);
        pwmLeft += turn;
        pwmRight -= turn;
    }

    setSpeed(constrain(pwmLeft, -255, 255), constrain(pwmRight, -255, 255));

    // Debug-Ausgabe
    Serial.print("Setze Geschwindigkeit: Links = ");
    Serial.print(pwmLeft);
    Serial.print(", Rechts = ");
    Serial.println(pwmRight);
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
    uni_bt_allowlist_set_enabled(true);
	
    BP32.setup(onConnectedController, onDisconnectedController);
}

void loop() {
    if (BP32.update()) {
        for (auto controller : myControllers) {
            if (controller && controller->isConnected() && controller->isKeyboard()) {
                processKeyboard(controller);
            }
        }
    }
    delay(10);  // Kurze Pause zur Schonung der CPU
}
```

### Funktionsweise des Codes

1. **Key Smoothing**: Für jede Taste wird die Zeit gemessen, wie lange sie gedrückt wird. Die Geschwindigkeit und die Richtung der Motoren werden entsprechend der Druckdauer angepasst. Dies sorgt für eine glattere und präzisere Steuerung.

2. **Verarbeitung der Tastendrücke**: Es wird überprüft, ob eine der W-, A-, S- oder D-Tasten gedrückt wird, und die PWM-Werte für die Motoren werden entsprechend angepasst.

3. **PWM-Werte**: Die berechneten PWM-Werte werden begrenzt, um sie innerhalb des gültigen Bereichs für die Motorsteuerung zu halten.

Durch dieses Setup können die Tastatureingaben den Roboter auf eine kontrollierte und sanfte Weise steuern, wobei die maximale Reaktionszeit auf eine Sekunde beschränkt ist. Dies kann besonders nützlich sein, um eine präzise Navigation in komplexen Umgebungen zu ermöglichen.