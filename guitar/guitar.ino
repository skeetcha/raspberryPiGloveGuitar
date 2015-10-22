int flex1 = A0;
int flex2 = A1;
int flex3 = A2;
int flex4 = A3;

void setup()
{
  pinMode(flex1, OUTPUT);
  pinMode(flex2, OUTPUT);
  pinMode(flex3, OUTPUT);
  pinMode(flex4, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Flex 1: ");
  Serial.print(analogRead(flex1));
  Serial.print(", Flex 2: ");
  Serial.print(analogRead(flex2));
  Serial.print(", Flex 3: ");
  Serial.print(analogRead(flex3));
  Serial.print(", Flex 4: ");
  Serial.println(analogRead(flex4));
}
