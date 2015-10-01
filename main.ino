
#define flexL1 A0
#define flexL2 A1
#define flexL3 A2
#define flexL4 A3
#define flexR1 A4
#define flexR2 A5
#define flexR3 A6
#define flexR4 A7
// TODO: Define Accelerometers here

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
  pinMode(flexL1, INPUT);
  pinMode(flexL2, INPUT);
  pinMode(flexL3, INPUT);
  pinMode(flexL4, INPUT);
  pinMode(flexR1, INPUT);
  pinMode(flexR2, INPUT);
  pinMode(flexR3, INPUT);
  pinMode(flexR4, INPUT);
  Serial.begin(9600);
}

void loop()
{
  readSensors();
  
  // Test flex values
  
  // Test accelerometer/fret values
  
  /*if (calibrateFunctionCalled)
    {
      calibrate();
    }*/
}

void readSensors()
{
  int flexL1Value = map(analogRead(flexL1), 0, 1023, flexL1Min, flexL1Max));
  int flexL2Value = map(analogRead(flexL2), 0, 1023, flexL2Min, flexL2Max));
  int flexL3Value = map(analogRead(flexL3), 0, 1023, flexL3Min, flexL3Max));
  int flexL4Value = map(analogRead(flexL4), 0, 1023, flexL4Min, flexL4Max));
  int flexR1Value = map(analogRead(flexR1), 0, 1023, flexR1Min, flexR1Max));
  int flexR2Value = map(analogRead(flexR2), 0, 1023, flexR2Min, flexR2Max));
  int flexR3Value = map(analogRead(flexR3), 0, 1023, flexR3Min, flexR3Max));
  int flexR4Value = map(analogRead(flexR4), 0, 1023, flexR4Min, flexR4Max));
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
