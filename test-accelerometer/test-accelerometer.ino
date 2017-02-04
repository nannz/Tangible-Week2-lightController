
//The circuit:
// analog 0: accelerometer self test
// analog 1: z-axis
// analog 2: y-axis
// analog 3: x-axis
// analog 4: ground
// analog 5: vcc

const int groundpin = 18;  //analog input pin 4
const int powerpin = 19;   //analog input pin 5
const int xpin = A3;
const int ypin = A2;
const int zpin = A1;


void setup() {
  Serial.begin(9600);

  pinMode(groundpin, OUTPUT);
  pinMode(powerpin, OUTPUT);
  digitalWrite(groundpin, LOW);
  digitalWrite(powerpin, HIGH);

}

void loop() {
  // print the sensor values:
  Serial.print("x: ");
  Serial.print(analogRead(xpin));
  // print a tab between values:
  Serial.print("\t");
  Serial.print("y: ");
  Serial.print(analogRead(ypin));
  // print a tab between values:
  Serial.print("\t");
  Serial.print("z: ");
  Serial.print(analogRead(zpin));
  Serial.println();
  // delay before next reading:
  delay(100);

}
