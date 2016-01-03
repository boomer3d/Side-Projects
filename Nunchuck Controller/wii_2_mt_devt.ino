/*
 * Hayden and Dad's Numchuck Fun!
 * Revision History
 *    01 - Initial Release with some improved noise reduction
 */
 
#include <Servo.h>
Servo servo1; 
Servo servo2;
#include <Wire.h>


#define ZEROX 530  
#define ZEROY 530
#define ZEROZ 530

#define PIN_C 10
#define PIN_Z 9

#define WII_NUNCHUK_I2C_ADDRESS 0x52

// Globals
int     counter;
uint8_t data[6];

bool    prior_read = false;
int     prior_val1  = 0;
int     prior_val2  = 0;

#define   JOY_ZERO    32768
#define   JOY_SCALE   0.7

void setup() 
{ 
  
  servo1.attach(11);
  servo2.attach(12);

 
  Wire.begin();

  Wire.beginTransmission(WII_NUNCHUK_I2C_ADDRESS);
  Wire.write(0xF0);
  Wire.write(0x55);
  Wire.endTransmission();

  Wire.beginTransmission(WII_NUNCHUK_I2C_ADDRESS);
  Wire.write(0xFB);
  Wire.write(0x00);
  Wire.endTransmission();

  // Set up the LED's as well
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_Z, OUTPUT);
} 

void loop() 
{ 
  
    Wire.requestFrom(WII_NUNCHUK_I2C_ADDRESS, 6);

    counter = 0;
   
    while(Wire.available())
    {
   
      data[counter++] = Wire.read();
    }

    
    Wire.beginTransmission(WII_NUNCHUK_I2C_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();

    if(counter >= 5)
    {
      // Read Data
      double accelX = ((data[2] << 2) + ((data[5] >> 2) & 0x03) - ZEROX);
      double accelY = ((data[3] << 2) + ((data[5] >> 4) & 0x03) - ZEROY);
      double accelZ = ((data[4] << 2) + ((data[5] >> 6) & 0x03) - ZEROZ);

      // test code - read joystick
      int   joyX = (data[0])*JOY_SCALE;
      servo1.write(joyX);
      
      int   joyY = (data[1])*JOY_SCALE;
      servo2.write(joyY);

      // New code, read C & Z pins
      bool   c_button = (data[5] & 0x02) == 0;
      bool   z_button = (data[5] & 0x01) == 0;
      digitalWrite(PIN_C, c_button);
      digitalWrite(PIN_Z, z_button);

      // Acceler Reader
/*
      int value1 = constrain(accelY, -180, 180);
      value1 = map(value1, -180, 180, 0, 180);
      if (prior_read) {
        value1 = (value1 + prior_val1)/2;
      }
      servo1.write(value1);
      prior_val1 = value1;
      
      int value2 = constrain(accelX, -180, 180);
      value2 = map(value2, -180, 180, 0, 180); 
      if (prior_read) {
        value2 = (value2 + prior_val2)/2;
      } else
        prior_read = true;
        
      servo2.write(value2);
      prior_val2 = value2;
*/     
      delay(20);
    }
}
