#include <Wire.h>
#include <SD.h>
#define SD_ChipSelectPin 8 // using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>
#include <SPI.h>

#define flexL1 A0
#define flexL2 A1
#define flexL3 A2
#define flexL4 A3
#define flexR1 A4
#define flexR2 A5
#define flexR3 A6
#define flexR4 A7
// TODO: Define Accelerometers here

#define DEVICE_A (0x1D) // First ADXL345 device address
#define DEVICE_B (0x53) // Second ADXL345 device address
#define TO_READ (6)     // Num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ];     // 6 bytes buffer for saving data read from the device
char str[512];          // String buffer to transform data before sending it to the serial port

int regAddress = 0x32; // First axis-acceleration-data register on the ADXL345
int xa = 0, ya = 0, za = 0;
int xb = 0, yb = 0, zb = 0;

TMRpcm tmrpcm;
static const char wav_1[] PROGMEM = "cmajor/cmajor";

// Calibrations
int flexL1Min = 288;
int flexL1Max = 303;
int flexL2Min = 288;
int flexL2Max = 303;
int flexL3Min = 288;
int flexL3Max = 303;
int flexL4Min = 288;
int flexL4Max = 303;
int flexR1Min = 288;
int flexR1Max = 303;
int flexR2Min = 288;
int flexR2Max = 303;
int flexR3Min = 288;
int flexR3Max = 303;
int flexR4Min = 288;
int flexR4Max = 303;

int fret = 0;

void setup()
{
  // flexSetup(); // Setup the flex sensors
  Serial.begin(9600); // Start Serial for output
  // accelSetup(); // Setup the accels
  soundsSetup(); // Setup the button plays sounds method
}

void loop()
{
  //readSensors();
  
  // Test flex values
  
  // Test accelerometer/fret values
  
  /*if (calibrateFunctionCalled)
  {
    calibrate();
  }*/
  
  // readAccels();
  
  soundsTest();
}

void soundsTest()
{
  tmrpcm.play("cmajor/cmajor1.wav");
  delay(1000);
}

void soundsSetup()
{
  tmrpcm.speakerPin = 46;
  pinMode(10, OUTPUT);
  
  if (!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail, not present");
    return;
  }
}

void flexSetup()
{
  pinMode(flexL1, INPUT);
  pinMode(flexL2, INPUT);
  pinMode(flexL3, INPUT);
  pinMode(flexL4, INPUT);
  pinMode(flexR1, INPUT);
  pinMode(flexR2, INPUT);
  pinMode(flexR3, INPUT);
  pinMode(flexR4, INPUT);
}

void accelSetup()
{
  Wire.begin();
  writeTo(DEVICE_A, 0x2D, 24);
  writeTo(DEVICE_B, 0x2D, 24);
}

void readAccels()
{
  readFrom(DEVICE_A, regAddress, TO_READ, buff); // Read the acceleration data from the first ADXL345
  // Each axis reading comes in 10 bit resolution, ie 2 bytes. Least significant byte first!
  // Thus we are converting both bytes in to one int
  xa = (((int) buff[1]) << 8) || buff[0];
  ya = (((int) buff[3]) << 8) || buff[2];
  za = (((int) buff[5]) << 8) || buff[4];
  
  readFrom(DEVICE_B, regAddress, TO_READ, buff); // Read the accleration data from the second ADXL345
  xb = (((int) buff[1]) << 8) | buff[0];
  yb = (((int) buff[3]) << 8) | buff[2];
  zb = (((int) buff[5]) << 8) | buff[3];
  
  // We send the x, y, and z values as a string to the Serial port
  // sprintf(str, "Accel 1: X - %d, Y - %d, Z - %d;    Accel 2: X - %d, Y - %d, Z - %d", xa, ya, za, xb, yb, zb);
  // Serial.print(str);
  Serial.print("Accel 1: ");
  Serial.print("X - ");
  Serial.print(xa);
  Serial.print(", Y - ");
  Serial.print(ya);
  Serial.print(", Z - ");
  Serial.println(za);
  Serial.print("Accel 2: ");
  Serial.print("X - ");
  Serial.print(xb);
  Serial.print(", Y - ");
  Serial.print(yb);
  Serial.print(", Z - ");
  Serial.println(zb);
  
  // It appears that delay is needed in order not to clog the port
  delay(15);
}

void writeTo(int device, byte address, byte val)
{
  Wire.beginTransmission(device); // Start transmission to device
  Wire.write(address); // Send register address
  Wire.endTransmission(); // End transmission
}

void readFrom(int device, byte address, int num, byte buff[])
{
  Wire.beginTransmission(device); // Start transmission to device
  Wire.write(address); // Sends address to read from
  Wire.endTransmission(); // End transmission
  
  Wire.beginTransmission(device); // Start tranmission to device
  Wire.requestFrom(device, num); // Request 6 bytes from device
  
  int i = 0;
  while (Wire.available()) // Device may send less than requested (abnormal)
  {
    buff[i] = Wire.read(); // Receive a byte
    i++;
  }
  
  Wire.endTransmission(); // End transmission
}

void readSensors()
{
  /*int flexL1Value = map(analogRead(flexL1), 0, 1023, flexL1Min, flexL1Max));
  int flexL2Value = map(analogRead(flexL2), 0, 1023, flexL2Min, flexL2Max));
  int flexL3Value = map(analogRead(flexL3), 0, 1023, flexL3Min, flexL3Max));
  int flexL4Value = map(analogRead(flexL4), 0, 1023, flexL4Min, flexL4Max));
  int flexR1Value = map(analogRead(flexR1), 0, 1023, flexR1Min, flexR1Max));
  int flexR2Value = map(analogRead(flexR2), 0, 1023, flexR2Min, flexR2Max));
  int flexR3Value = map(analogRead(flexR3), 0, 1023, flexR3Min, flexR3Max));
  int flexR4Value = map(analogRead(flexR4), 0, 1023, flexR4Min, flexR4Max));*/
}

void calibrate()
{
  Serial.println("Move your fingers so that they are straight up and down.");
  Serial.println("Measuring in 5 seconds...");
  delay(5000);
  Serial.print("Measuring...");
  flexL1Min = analogRead(flexL1);
  Serial.print(".");
  flexL2Min = analogRead(flexL2);
  Serial.print(".");
  flexL3Min = analogRead(flexL3);
  Serial.print(".");
  flexL4Min = analogRead(flexL4);
  Serial.print(".");
  flexR1Min = analogRead(flexR1);
  Serial.print(".");
  flexR2Min = analogRead(flexR2);
  Serial.print(".");
  flexR3Min = analogRead(flexR3);
  Serial.println(".");
  flexR4Min = analogRead(flexR4);
  Serial.println("Measuring completed.");
  delay(2000);
  Serial.println("Move your fingers so that they are completely bent.");
  Serial.println("Measuring in 5 seconds...");
  delay(5000);
  Serial.print("Measuring...");
  flexL1Max = analogRead(flexL1);
  Serial.print(".");
  flexL2Max = analogRead(flexL2);
  Serial.print(".");
  flexL3Max = analogRead(flexL3);
  Serial.print(".");
  flexL4Max = analogRead(flexL4);
  Serial.print(".");
  flexR1Max = analogRead(flexR1);
  Serial.print(".");
  flexR2Max = analogRead(flexR2);
  Serial.print(".");
  flexR3Max = analogRead(flexR3);
  Serial.println(".");
  flexR4Max = analogRead(flexR4);
  Serial.println("Measuring completed.");
  delay(2000);
}
