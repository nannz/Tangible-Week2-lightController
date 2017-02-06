/* switch

   Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
   press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
   a minimum delay between toggles to debounce the circuit (i.e. to ignore
   noise).

   RGB Led
*/

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
int beginColor[]  = {255, 255, 0}; //the color when you turn on the led

void setup()
{
  Serial.begin(9600);
  pinMode(switchInPin, INPUT);
  pinMode(switchOutPin, OUTPUT); //for the red Led

  // set the digital pins as outputs - RGB LED
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

}

void loop()
{
  readingSwitch = digitalRead(switchInPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (readingSwitch == HIGH && previousSwitch == LOW && millis() - time > debounce) {
    if (switchState == HIGH) {
      switchState = LOW;
      //setLEDColor(0, 0, 0);
      turnOff = true;
      turnOn = false;
    } else {
      switchState = HIGH;
      //setLEDColor(0, 0, 255);
      turnOn = true;
      turnOff = false;
    }
    time = millis();
  }

  if (turnOff == true) {
    Serial.println("start turnning off");
    turnOffLED(currentColor);
    Serial.println("turn off finished");
    turnOff = false;
  }
  if (turnOn == true){
    turnOnLED(beginColor);
    turnOn = false;
    //Serial.println(turnOn);
  }
  if(switchState == HIGH && turnOn == false){
    Serial.println("set led color");
    setLEDColor(255,0,0);
  }

  digitalWrite(switchOutPin, switchState);

  previousSwitch = readingSwitch;
}
