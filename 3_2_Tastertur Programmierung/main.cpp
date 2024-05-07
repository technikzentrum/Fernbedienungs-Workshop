#include <Arduino.h>
#include <Bluepad32.h>
#include "uni_bt_allowlist.h"


// Definiere hier die MAC-Adressen der erlaubten Controller
const char* allowedAddresses[] = {"00:11:22:33:44:55", "AA:BB:CC:DD:EE:FF"};
const int numAllowed = sizeof(allowedAddresses) / sizeof(allowedAddresses[0]);

// Timing für Smoothing
unsigned long keyPressTime[256];  // Speichert die Zeit, zu der jede Taste gedrückt wurde

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
	
    BP32.setup(onConnectedController, onDisconnectedController);
}

void loop() {
    if (BP32.update()) {
        for (auto controller : BP32.controllers()) {
            if (controller && controller->isConnected() && controller->isKeyboard()) {
                processKeyboard(controller);
            }
        }
    }
    delay(10);  // Kurze Pause zur Schonung der CPU
}

void onConnectedController(ControllerPtr ctl) {
    Serial.println("Gerät verbunden");
}

void onDisconnectedController(ControllerPtr ctl) {
    Serial.println("Gerät getrennt");
}

void processKeyboard(ControllerPtr ctl) {
    int pwmLeft = 0;
    int pwmRight = 0;

    // W-Taste für Vorwärts
    if (ctl->isKeyPressed(Keyboard_W)) {
        unsigned long duration = updateKeyPressDuration(Keyboard_W);
        int speed = map(min(duration, 1000), 0, 1000, 0, 255);
        pwmLeft += speed;
        pwmRight += speed;
    }

    // S-Taste für Rückwärts
    if (ctl->isKeyPressed(Keyboard_S)) {
        unsigned long duration = updateKeyPressDuration(Keyboard_S);
        int speed = map(min(duration, 1000), 0, 1000, 0, 255);
        pwmLeft -= speed;
        pwmRight -= speed;
    }

    // A-Taste für Links
    if (ctl->isKeyPressed(Keyboard_A)) {
        unsigned long duration = updateKeyPressDuration(Keyboard_A);
        int turn = map(min(duration, 1000), 0, 1000, 0, 255);
        pwmLeft -= turn;
        pwmRight += turn;
    }

    // D-Taste für Rechts
    if (ctl->isKeyPressed(Keyboard_D)) {
        unsigned long duration = updateKeyPressDuration(Keyboard_D);
        int turn = map(min(duration, 1000), 0, 1000, 0, 255);
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

void setSpeed(int leftPWM, int rightPWM) {
    // Hier würde normalerweise der Code stehen, um die Motoren des Roboters anzusteuern
    // Beispiel:
    analogWrite(LEFT_MOTOR_PIN, leftPWM);
    analogWrite(RIGHT_MOTOR_PIN, rightPWM);
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