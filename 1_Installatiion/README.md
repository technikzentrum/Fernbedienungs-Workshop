### Installationsanleitung für PlatformIO in VS Code

**Benötigte Software:**
- Microsoft Visual Studio Code (VS Code)
- PlatformIO IDE Extension für VS Code
- Git (optional, aber empfohlen)

**1. Visual Studio Code installieren**
   - Gehe zu [Visual Studio Code](https://code.visualstudio.com) und lade die Version für dein Betriebssystem herunter.
   - Führe die heruntergeladene Datei aus und folge den Installationsanweisungen.

   ![VS Code Download](https://code.visualstudio.com/assets/download.png)

**2. PlatformIO IDE Extension installieren**
   - Anleitung auf Englisch: [PlatformIO offizielle Anleitung](https://docs.platformio.org/en/stable/integration/ide/vscode.html#installation)
   - Starte VS Code nach der Installation.
   - Öffne den Extension-Manager, indem du auf das Quadrat-Symbol auf der linken Seite klickst oder `Strg+Shift+X` drückst.
   - Gib in das Suchfeld `PlatformIO IDE` ein und wähle die entsprechende Extension aus.
   - Klicke auf „Installieren“.

   ![PlatformIO IDE Extension](https://docs.platformio.org/en/stable/_images/platformio-ide-vscode-pkg-installer.png)

**3. Überprüfung der Git-Installation**
   - Um Git-basierte Funktionen innerhalb von PlatformIO nutzen zu können, muss Git auf deinem System installiert sein.
   - Öffne das Terminal oder die Eingabeaufforderung und gib `git --version` ein, um zu überprüfen, ob Git installiert ist.
   - Wenn dies nicht der Fall ist, installiere Git von [Git SCM](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).
   - [Download](https://git-scm.com/download/win)

**4. Erstes Projekt erstellen**
   - Klicke in VS Code unten in der PlatformIO-Toolbar auf „PlatformIO Home“. :house:
   - Wähle „New Project“.
   - Gib die notwendigen Informationen ein, wähle das Board "WEMOS D1 MINI ESP32" und bestätige mit „Finish“.

   ![Neues Projekt](https://docs.platformio.org/en/stable/_images/platformio-ide-vscode-new-project.png)

**5. Erstes Beispielprogramm**
   - Öffne die `main.cpp` Datei im `src` Ordner des neu erstellten Projekts.
   - Ersetze den Inhalt durch das Blink-Beispiel oder ein ähnliches einfaches Programm.
   - Kompiliere dein Projekt mit der Tastenkombination `Strg+Alt+B` oder klicke auf den „Build“-Button :heavy_check_mark: in der PlatformIO-Toolbar.

   ![Blink Beispiel](https://docs.platformio.org/en/stable/_images/platformio-ide-vscode-blink-project.png)

**6. Weiterführende Informationen(OPTIONAL)**
   - Für eine detailliertere Einführung in die Nutzung von PlatformIO empfehlen wir die [Tutorials und Beispiele](https://docs.platformio.org/en/stable/tutorials/index.html#tutorials) auf der offiziellen Website.
   - Beachte die „platformio.ini“ Datei für projektspezifische Einstellungen und Anpassungen.