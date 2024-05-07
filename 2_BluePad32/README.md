### Was ist Bluepad32?
Bluepad32 ist eine Bibliothek, die es ermöglicht, verschiedene Bluetooth-Steuergeräte wie Gamepads, Mäuse und Tastaturen einfach in deine Projekte zu integrieren. Sie funktioniert mit Plattformen wie ESP32, ESP32-S3, ESP32-C3, Raspberry Pi Pico W und POSIX-Systemen (Linux, macOS). Mit Bluepad32 kannst du beispielsweise einen Roboter mit einem DualSense-Controller steuern.

#### Unterstützte Controller:
Hier ist eine Grafik, die eine Vielzahl von unterstützten Controllern zeigt, darunter Sony DualSense, Nintendo Switch Pro Controller und viele mehr:

![Unterstützte Gamepads](https://lh3.googleusercontent.com/pw/AMWts8BB7wT51jpn3HxWHuZLiEM2lX05gmTDsnldHszkXuYqxbowNvtxPtpbHh3CNjv1OBzeyadZjNLNBgE4w2tl2WmP8M9gGBCfWhzmZGQnHBlERSoy5W2dj6-EYmT84yteKTFjp4Jz2H3DgByFiKXaxfFC2g=-no)

### Anleitung zur Installation von Bluepad32 mit ESP-IDF und PlatformIO

1. **Projekt klonen**
   - Öffne ein Terminal oder die Kommandozeile auf deinem Computer.
   - Gib den folgenden Befehl ein, um das Template-Projekt zu klonen:
     ```
     git clone --recursive https://github.com/ricardoquesada/esp-idf-arduino-bluepad32-template.git mein_projekt
     ```
   - Dieser Befehl lädt das Projekt und alle erforderlichen Unterprojekte herunter.

2. **Projekt in Visual Studio Code öffnen**
   - Starte Visual Studio Code.
   - Öffne das PlatformIO-Plugin.
   - Wähle „Pick a folder“ und navigiere zum eben geklonten „mein_projekt“-Ordner.

     ![Projekt öffnen](https://lh3.googleusercontent.com/pw/ABLVV85JEEjjsQqcCcfZUclYF1ItYSHPmpzP0SC4VH9Ypqp05r2ixlv9C2xv4p-r6fW_CyCNa8ylmeSjyUg_K2Sp-XUXQRTYO_6HvhQXcXxTZXgQvvNBqA8JaerwCB1UODkXgYa_6ONT19KTO52OMs0eOOeeMg=-no-gm?authuser=0)

3. **Abhängigkeiten installieren und Projekt aufbauen**
   - PlatformIO wird automatisch die ESP-IDF-Toolchain und alle Abhängigkeiten herunterladen. Dies kann einige Minuten dauern.
   - Klicke auf eine der vorkonfigurierten Boards, wie z.B. *esp32-s3-devkit-1*, oder bearbeite die `platformio.ini` Datei, um dein eigenes Board hinzuzufügen.
   - Klicke auf „Build“.

     ![Projekt bauen](https://lh3.googleusercontent.com/pw/ABLVV86DiV9H-wDEv1X8ra_fJAw0OG2sBoM5d0gJElPfptzVpb6n8gzOEHDfKXLMKrivzNSt03XpMWSw-hSVJUi0aavQiwgL0t1rmQeKqfYpXkGCKKwcerrNx8BBkFR3VoKQEPMF-e-xVvKVque2pi1sTa8tWA=-no-gm?authuser=0)

4. **Hochladen und Überwachen**
   - Klicke nach dem Erstellen des Projekts auf „Upload and Monitor“.
   - Das Programm wird auf dein Gerät hochgeladen und der Monitor-Modus gestartet. Hier kannst du die Konsolenausgabe sehen und beispielsweise `help` eingeben, um verfügbare Befehle zu sehen.

     ![Überwachen des Projekts](https://lh3.googleusercontent.com/pw/ABLVV845uPqRtJkUrv4JlODuTr7Shnw0HR7BdojRbxv3xWyiUO-V_Kv42YAKAV-XyoNRPY5vsyj0yRDsRxH0mxz8Q1NYzvhCKw5Ni9MH6UYR8IiaT8XS9hysR81APn8X2tnVgnmJ6ZkSPCgUURnE2MVYIWYrNQ=-no-gm?authuser=0)

Diese Schritte führen dich durch den Prozess der Einrichtung eines Projekts mit Bluepad32 unter Verwendung von PlatformIO und ESP-IDF. Weitere Informationen und spezifische Anleitungen findest du in der [offiziellen Dokumentation](https://github.com/ricardoquesada/esp-idf-arduino-bluepad32-template).

Jetzt können wir den ersten Controller verbinden.