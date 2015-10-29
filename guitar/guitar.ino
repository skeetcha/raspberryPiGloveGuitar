#include <SPI.h>
#include <EEPROM.h>

#define leftAccel 7
#define rightAccel 6

#define GAUGE 0

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

int address = 0;

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
  eepromLoop();
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

void eepromLoop()
{
  unsigned long time = millis();
  unsigned long prevTime = EEPROM.read(address);
  
  EEPROM.write(address, prevTime + time);
  
  Serial.print("On Gauge ");
  Serial.println(GAUGE);
  Serial.print("Time until strings have broken: ");
  
  if (GAUGE == 0)
  {
    Serial.print((72000000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 1)
  {
    Serial.print((79200000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 2)
  {
    Serial.print((86400000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 3)
  {
    Serial.print((93600000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 4)
  {
    Serial.print((100800000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 5)
  {
    Serial.print((108000000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 6)
  {
    Serial.print((115200000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 7)
  {
    Serial.print((122400000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 8)
  {
    Serial.print((129600000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 9)
  {
    Serial.print((136800000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 10)
  {
    Serial.print((144000000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  else if (GAUGE == 11)
  {
    Serial.print((151200000 - (prevTime + time)) / 3600000);
    Serial.println(" hours");
  }
  
  unsigned long int timeNow = EEPROM.read(address);
  
  if ((GAUGE == 0 && timeNow == 72000000) || (GAUGE == 1 && timeNow == 79200000) || (GAUGE == 2 && timeNow == 86400000) || (GAUGE == 3 && timeNow == 93600000) || (GAUGE == 4 && timeNow == 100800000) || (GAUGE == 5 && timeNow == 108000000) || (GAUGE == 6 && timeNow == 115200000) || (GAUGE == 7 && timeNow == 122400000) || (GAUGE == 8 && timeNow == 129600000) || (GAUGE == 9 && timeNow == 136800000) || (GAUGE == 10 && timeNow == 144000000) || (GAUGE == 11 && timeNow == 151200000)
  {
    Serial.println("Strings broke");
  }
  
  address += 1;
  if (address == EEPROM.length())
  {
    address = 0;
  }
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
