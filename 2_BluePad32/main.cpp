#include <Arduino.h>
#include <Bluepad32.h>
#include "uni_bt_allowlist.h"  // Dein Whitelist-Code

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
                // Verarbeite nur erlaubte Controller
                if (uni_bt_allowlist_is_allowed_addr(controller->bdAddr())) {
                    processGamepad(controller);
                }
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