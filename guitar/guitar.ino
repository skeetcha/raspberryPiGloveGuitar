#include <SPI.h>

#define leftAccel 7
#define rightAccel 6

int flex1 = A0;
int flex2 = A1;
int flex3 = A2;
int flex4 = A3;

char POWER_CTL = 0x2D;
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;
char DATAX1 = 0x33;
char DATAY0 = 0x34;
char DATAY1 = 0x35;
char DATAZ0 = 0x36;
char DATAZ1 = 0x37;

int rawX1, rawY1, rawZ1, rawX2, rawY2, rawZ2;
float scaledX1, scaledY1, scaledZ1, scaledX2, scaledY2, scaledZ2;
char values[10];
float scale = 0.0078;
int flex1Read, flex2Read, flex3Read, flex4Read;

void setup()
{
  pinMode(flex1, OUTPUT);
  pinMode(flex2, OUTPUT);
  pinMode(flex3, OUTPUT);
  pinMode(flex4, OUTPUT);
  
  Serial.begin(9600);
  
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  
  pinMode(leftAccel, OUTPUT);
  pinMode(rightAccel, OUTPUT);
  
  digitalWrite(leftAccel, HIGH);
  writeRegister(leftAccel, DATA_FORMAT, 0x01);
  writeRegister(leftAccel, POWER_CTL, 0x08);
  
  digitalWrite(rightAccel, HIGH);
  writeRegister(rightAccel, DATA_FORMAT, 0x01);
  writeRegister(rightAccel, POWER_CTL, 0x08);
}

void loop()
{
  flexTest();
  
  Serial.print("Flex 1: ");
  Serial.print(flex1Read);
  Serial.print(", Flex 2: ");
  Serial.print(flex2Read);
  Serial.print(", Flex 3: ");
  Serial.print(flex3Read);
  Serial.print(", Flex 4: ");
  Serial.println(flex4Read);
  
  accelTest();
  
  Serial.print("Accelerometer 1, Raw: X - ");
  Serial.print(rawX1, DEC);
  Serial.print(", Y - ");
  Serial.print(rawY1, DEC);
  Serial.print(", Z - ");
  Serial.println(rawZ1, DEC);
  Serial.print("Accelerometer 1, Scaled: X - ");
  Serial.print(scaledX1);
  Serial.print("g, Y - ");
  Serial.print(scaledY1);
  Serial.print("g, Z - ");
  Serial.print(scaledZ1);
  Serial.println("g");
  Serial.println();
  delay(500);
  
  Serial.print("Accelerometer 2, Raw: X - ");
  Serial.print(rawX2, DEC);
  Serial.print(", Y - ");
  Serial.print(rawY2, DEC);
  Serial.print(", Z - ");
  Serial.println(rawZ2, DEC);
  Serial.print("Accelerometer 2, Scaled: X - ");
  Serial.print(scaledX2);
  Serial.print("g, Y - ");
  Serial.print(scaledY2);
  Serial.print("g, Z - ");
  Serial.print(scaledZ2);
  Serial.println("g");
  Serial.println();
  delay(500);
}

void writeRegister(int CS, char registerAddress, char value)
{
  digitalWrite(CS, LOW);
  SPI.transfer(registerAddress);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}

void readRegister(int CS, char registerAddress, int numBytes, char * values)
{
  char address = 0x80 | registerAddress;
  if (numBytes > 1)
  {
    address = address | 0x40;
  }
  
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  
  for (int i = 0; i < numBytes; i++)
  {
    values[i] = SPI.transfer(0x00);
  }
  
  digitalWrite(CS, HIGH);
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void flexTest()
{
  flex1Read = analogRead(flex1);
  flex2Read = analogRead(flex2);
  flex3Read = analogRead(flex3);
  flex4Read = analogRead(flex4);
}

void accelTest()
{
  readRegister(leftAccel, DATAX0, 6, values);
  
  rawX1 = ((int) values[1] << 8) | (int) values[0];
  rawY1 = ((int) values[3] << 8) | (int) values[2];
  rawZ1 = ((int) values[5] << 8) | (int) values[4];
  
  readRegister(rightAccel, DATAX0, 6, values);
  
  rawX2 = ((int) values[1] << 8) | (int) values[0];
  rawY2 = ((int) values[3] << 8) | (int) values[2];
  rawZ2 = ((int) values[5] << 8) | (int) values[4];
  
  scaledX1 = (float) rawX1 * scale;
  scaledY1 = (float) rawY1 * scale;
  scaledZ1 = (float) rawZ1 * scale;
  scaledX2 = (float) rawX2 * scale;
  scaledY2 = (float) rawY2 * scale;
  scaledZ2 = (float) rawZ2 * scale;
}
