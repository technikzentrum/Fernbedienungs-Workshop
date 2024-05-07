Um den Roboter mithilfe eines Controllers zu steuern müssen wir den Code so aufbauen, dass er die Bewegungsachsen des Controllers ausliest und diese Daten zur Steuerung des Roboters verwendet. Hier ist ein Beispielcode, der zeigt, wie du diese Anforderungen implementieren kannst:

```cpp
#include <Arduino.h>
#include <Bluepad32.h>
#include "uni_bt_allowlist.h"

// Definiere hier die MAC-Adressen der erlaubten Controller
const char* allowedAddresses[] = {"00:11:22:33:44:55", "AA:BB:CC:DD:EE:FF"};
const int numAllowed = sizeof(allowedAddresses) / sizeof(allowedAddresses[0]);

// Initialisiere Bluepad32
BP32Class BP32;

void setup() {
    Serial.begin(115200);
    BP32.begin();

    // Füge die erlaubten MAC-Adressen zur Allowlist hinzu
    for (int i = 0; i < numAllowed; ++i) {
        bd_addr_t addr;
        sscanf_bd_addr(allowedAddresses[i], addr);
        uni_bt_allowlist_add_addr(addr);
    }

    // Aktiviere die Whitelist-Überprüfung
    uni_bt_allowlist_set_enabled(true);

    // Richte die Callbacks ein
    BP32.setup(onConnectedController, onDisconnectedController);
}

void loop() {
    // Aktualisiere die Controller-Daten
    if (BP32.update()) {
        // Verarbeite die Daten jedes verbundenen Controllers
        for (auto controller : BP32.controllers()) {
            if (controller && controller->isConnected() && controller->isGamepad()) {
                // Verarbeite Controller
                processGamepad(controller);
            }
        }
    }

    delay(10);  // Kurze Pause zur Schonung der CPU
}

void onConnectedController(ControllerPtr ctl) {
    Serial.println("Controller verbunden");
}

void onDisconnectedController(ControllerPtr ctl) {
    Serial.println("Controller getrennt");
}

void processGamepad(ControllerPtr ctl) {
    // Lies die Werte der rechten Achse aus
    int rightX = ctl->axisRX();
    int rightY = ctl->axisRY();

    // Skaliere die Achsenwerte auf PWM-Werte
    int leftPWM = map(rightY + rightX, -1024, 1024, -255, 255);
    int rightPWM = map(rightY - rightX, -1024, 1024, -255, 255);

    // Setze die Geschwindigkeit des Roboters
    setSpeed(leftPWM, rightPWM);

    // Debug-Ausgabe
    Serial.print("Setze Geschwindigkeit: Links = ");
    Serial.print(leftPWM);
    Serial.print(", Rechts = ");
    Serial.println(rightPWM);
}

void setSpeed(int leftPWM, int rightPWM) {
    // Hier würde normalerweise der Code stehen, um die Motoren des Roboters anzusteuern
    // Beispiel:
    analogWrite(LEFT_MOTOR_PIN, leftPWM);
    analogWrite(RIGHT_MOTOR_PIN, rightPWM);
}

```

### Erklärung:
1. **Whitelist-Setup:** Am Anfang werden die erlaubten MAC-Adressen zur Whitelist hinzugefügt. Die Funktion `uni_bt_allowlist_add_addr` wird verwendet, um jede Adresse hinzuzufügen.
2. **Controller-Verbindung:** Wenn ein Controller verbunden wird, überprüft das System, ob die Adresse des Controllers in der Whitelist enthalten ist, bevor es weitere Aktionen erlaubt.
3. **Datenverarbeitung:** Die Hauptsteuerungslogik befindet sich in der Funktion `processGamepad`. Diese Funktion liest die Position der rechten Joystick-Achse und rechnet diese in Motor-PWM-Signale um.
4. **Motorsteuerung:** Die Funktion `setSpeed` ist dafür zuständig, die Geschwindigkeit des Roboters zu setzen. Sie sollte so angepasst werden, dass sie mit der spezifischen Hardware des Roboters kompatibel ist.

Mit diesem Setup kannst du sicherstellen, dass nur erlaubte Controller den Roboter steuern können, was besonders in Umgebungen mit mehreren potenziellen Steuergeräten oder in öffentlichen Demonstrationen wichtig ist.