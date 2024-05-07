#include <Arduino.h>
#include <Bluepad32.h>
#include <uni.h>


// Definiere hier die MAC-Adressen der erlaubten Controller
const char* allowedAddresses[] = {"98:B6:E9:01:6C:47", "AA:BB:CC:DD:EE:FF"};
const int numAllowed = sizeof(allowedAddresses) / sizeof(allowedAddresses[0]);
ControllerPtr myControllers[BP32_MAX_GAMEPADS];


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

void setup() {
    Serial.begin(115200);
    uni_bt_allowlist_init();

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
        for (auto controller : myControllers) {
            if (controller && controller->isConnected() && controller->isGamepad()) {
                // Verarbeite Controller
                processGamepad(controller);
            }
        }
    }

    delay(10);  // Kurze Pause zur Schonung der CPU
}