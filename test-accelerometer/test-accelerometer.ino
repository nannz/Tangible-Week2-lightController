
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
int yRawMax = 420;
int zRawMin = 260;
int zRawMax = 420;
int xRaw = 0;
int yRaw = 0;
int zRaw = 0;

// Take multiple samples to reduce noise
const int sampleSize = 10;

void setup() {
  Serial.begin(9600);

  pinMode(groundpin, OUTPUT);
  pinMode(powerpin, OUTPUT);
  digitalWrite(groundpin, LOW);
  digitalWrite(powerpin, HIGH);

}

void loop() {
  xRaw = ReadAxis(xpin);
  yRaw = ReadAxis(ypin);
  zRaw = ReadAxis(zpin);

  //show the current x,y,z raw data
  //showRawData();

  //check the max and min of xyzAxises
  AutoCalibrate(xRaw, yRaw, zRaw);

  toFractionalG(xRaw, yRaw, zRaw);


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


void toFractionalG(int xRaw, int yRaw, int zRaw){
  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
  long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
  long zScaled = map(zRaw, zRawMin, zRawMax, -1000, 1000);

  // re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;

  Serial.print(" :: ");
  Serial.print(xAccel);
  Serial.print("G, ");
  Serial.print(yAccel);
  Serial.print("G, ");
  Serial.print(zAccel);
  Serial.println("G");
}

