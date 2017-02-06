
//The circuit:
// analog 0: accelerometer self test
// analog 1: x-axis
// analog 2: y-axis
// analog 3: z-axis
// analog 4: ground
// analog 5: vcc
const int groundpin = 17;//18;  //analog input pin 4
const int powerpin = 19;   //analog input pin 5
const int xpin = A0;
const int ypin = A1;
const int zpin = A2;
// Raw Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xRawMin = 260;
int xRawMax = 420;
int yRawMin = 260;
int yRawMax = 398;
int zRawMin = 260;
int zRawMax = 425;
int xRaw = 0;
int yRaw = 0;
int zRaw = 0;

float accels[3] = {0.0, 0.0, 0.0};

// Take multiple samples to reduce noise
const int sampleSize = 10;
//set up initial value of hue, saturation, and value.
int hue = 0;
int saturation = 255;
int value = 255;
int rgbColor[] = {0, 0, 0};

//--------------------------
//LED
int switchInPin = 2;         // the number of the input pin
int switchOutPin = 13;       // the number of the output pin

int switchState = LOW;      // the current state of the output pin
int readingSwitch;           // the current reading from the input pin
int previousSwitch = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

//the RGB LED
const int greenLEDPin = 9;    // LED connected to digital pin 9
const int redLEDPin = 10;     // LED connected to digital pin 10
const int blueLEDPin = 11;    // LED connected to digital pin 11
int redValue = 0; // value to write to the red LED
int greenValue = 0; // value to write to the green LED
int blueValue = 0; // value to write to the blue LED

int currentColor[] = {0, 0, 0};
bool turnOff = false;
bool turnOn = false;
int beginColor[]  = {255, 255, 255}; //the color when you turn on the led

bool isWorking = false;
void setup() {
  Serial.begin(9600);

  //accelerometer
  pinMode(groundpin, OUTPUT);
  pinMode(powerpin, OUTPUT);
  digitalWrite(groundpin, LOW);
  digitalWrite(powerpin, HIGH);

  //led
  pinMode(switchInPin, INPUT);
  pinMode(switchOutPin, OUTPUT); //for the red Led
  // set the digital pins as outputs - RGB LED
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

}

void loop() {
  xRaw = ReadAxis(xpin);
  yRaw = ReadAxis(ypin);
  zRaw = ReadAxis(zpin);

  //show the current x,y,z raw data
  //showRawData();

  //check the max and min of xyzAxises
  AutoCalibrate(xRaw, yRaw, zRaw);

  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
  long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
  long zScaled = map(zRaw, zRawMin, zRawMax, -1000, 1000);
  // re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;
  /*
    Serial.print(xAccel);
    Serial.print(", ");
    Serial.print(yAccel);
    Serial.print(", ");
    Serial.print(zAccel);
    Serial.println(" ");
  */
  //get the value of saturation
  long saturationFloat = map(zScaled, -1000, 1000, 0, 255);
  saturation = (int)saturationFloat;

  //get the hue value
  float angle = atan2(yAccel, xAccel) * 180 / M_PI;
  float hueValue = map(angle, -180.0, 180.0, 0, 259);
  hue = (int)hueValue;

  //get RGB value from the hue value and saturation value.
  getRGB(hue, saturation, value, rgbColor);
  //    Serial.print("rgb: ");
  //    Serial.print(rgbColor[0]);
  //    Serial.print(", ");
  //    Serial.print(rgbColor[1]);
  //    Serial.print(", ");
  //    Serial.print(rgbColor[2]);
  //    Serial.println(" ");
  Serial.print("rgb: ");
  Serial.print(rgbColor[0]);
  Serial.print(", ");
  Serial.print(rgbColor[1]);
  Serial.print(", ");
  Serial.print(rgbColor[2]);
  Serial.println(" ");

  //---------------------LED------------------
  readingSwitch = digitalRead(switchInPin);
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (readingSwitch == HIGH && previousSwitch == LOW && millis() - time > debounce) {
    if (switchState == HIGH) {//手刚按下去
      Serial.println("switch case 1");
      switchState = LOW;
      turnOn = false;
      turnOff = true;
    } else {//手抬起来
      Serial.println("switch case 2");
      switchState = HIGH;
      turnOn = true;
      turnOff = false;
    }
    time = millis();
  }
  if (turnOff == true) {
    Serial.println("turnning off");

    //    Serial.print("currentRGB: ");
    //    Serial.print(currentColor[0]);
    //    Serial.print(", ");
    //    Serial.print(currentColor[1]);
    //    Serial.print(", ");
    //    Serial.print(currentColor[2]);
    //    Serial.println(". ");

    turnOffLED(currentColor);
    Serial.println("turn off finished");
    turnOff = false;
  }
  if (turnOn == true) {
    turnOnLED(beginColor);
    turnOn = false;
  }
  if (switchState == HIGH && turnOn == false) {
    //Serial.println("set led color");
    setLEDColor(rgbColor[0], rgbColor[1], rgbColor[2]);

  }
  digitalWrite(switchOutPin, switchState);
  previousSwitch = readingSwitch;

  // delay before next reading:
  delay(100);
}

int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading / sampleSize;
}



