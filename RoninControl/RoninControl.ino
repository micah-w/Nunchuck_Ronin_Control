/*
    This code takes in values from a Nintendo Nunchuck and makes a DJI Ronin
    respond accordingly.

    Code modified by Micah Wagoner.

    Code based off of user "Sminkly" on the DJI Forum: https://forum.dji.com/thread-167232-1-1.html
    Code changed to overcome errors and suit needs. WiiChuck Library has been modified to remove all
    functionality for other controllers to reduce memory load. Using the original WiiChuck library
    WILL work, but may be unstable because it uses 93% of the memory. Modified version uses 52% of memory.


    PINOUT:
     - Nunchuck
       - RED: 3.3V
       - WHITE: GND
       - GREEN: A4
       - YELLOW: A5
     - Ronin
       - TOP (BLACK): GROUND
       - MIDDLE (RED): VCC
       - BOTTOM (WHITE): TX
*/

// Include the BMC_SBUS library
#include "BMC_SBUS.h"

// Declare BMC_SBUS Object
BMC_SBUS mySBUS;

// Include Wiichuck Library
#include <WiiChuck.h>

// Declare Nunchuck Object
Accessory nunchuck1;

// Sbus delay value
const int sbusWAIT = 7;      //frame timing delay in msecs

// Declare sbus control channels (For Ronin)
int panChannel = 1;
int tiltChannel = 2;
int rollChannel = 4;

void setup() {

  Serial.begin(115200);
  nunchuck1.begin(); // Start nunchuck
  nunchuck1.type = NUNCHUCK; // Set type
  mySBUS.begin(); // Start SBUS object

}
void loop() {
  nunchuck1.readData();    // Read inputs and update maps
  
  // Assign wiichuck data to integers
  int joystickValueX = nunchuck1.values[0];
  int joystickValueY = nunchuck1.values[1];

  // Map wiichuck values which go from 0-255 to Sbus values 0-2047
  // For no move send 1023
  int sendValueY = map(joystickValueY, 0, 255, 512, 1635);
  int sendValueX = map(joystickValueX, 0, 255, 412, 1635);

  // Set sbus pan and tilt
  mySBUS.Servo(tiltChannel, sendValueY);
  mySBUS.Servo(panChannel, sendValueX);

  // Update SBUS object and send data
  mySBUS.Update();
  mySBUS.Send();

  // Delay for SBUS
  delay(sbusWAIT);

}
