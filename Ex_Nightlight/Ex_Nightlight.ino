// which analog pin to connect
#define LIGHTSENSOR A0
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// the value of the 'other' resistor
#define SERIESRESISTOR 10000

int samples[NUMSAMPLES];
float thresholdBrightSurrounding = 2; //Task 9: different bright/dark thresholds
float thresholdDarkSurrounding = 2.5; //Task 4: internal LED on/off; defines the threshold for switching on the LED
float thresholdDarkBit = thresholdDarkSurrounding * 1023 / 5;
bool lightOn = false;
int ledPin = 5; //defines pin for LED´
unsigned long previousMillis; //saves the time of the last update (for whole measurement)
unsigned long currentMillis; //saves current milliseconds of the program sequence
unsigned long interval = 100; //interval, in which the sensor should be started again
unsigned long previousMillisSwitch; //saves time of the last switch between light/dark
float lightInterval; //saves the time period between light/dark

//Task 10: function to switch state
void switchState() {
  lightInterval = (currentMillis - previousMillisSwitch) / 1000.00;
  previousMillisSwitch = currentMillis;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); //Task 4: internal LED on/off; defines LED_builtin LED as output
  pinMode(ledPin, OUTPUT); //Task 6: use external LED & Task 7: adaptive brightness LED; defines ledPin (5) as output
}

void loop() {
  uint8_t i;
  float average;
  float vOut;
  int val;

  currentMillis = millis();

  //defines period of time at which measurement should begin again
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    //Task 2: first circuit; take N-number of samples in a row, with a slight delay
    for (int i = 0; i < NUMSAMPLES; i++) {
      samples[i] = analogRead(LIGHTSENSOR);
      delay(10);
    }

    //Task 2: first circuit; average all the samples out
    average = 0;
    for (int i = 0; i < NUMSAMPLES; i++) {
      average += samples[i];
    }
    average /= NUMSAMPLES;
    //Serial.println(average);

    //Task 3: converting to volts
    vOut = (5 * average) / 1023;
    //Serial.println(vOut);

    //Task 4: internal LED on/off
    //if(vOut >= thresholdDarkSurrounding){
    //  digitalWrite(LED_BUILTIN, HIGH);
    //  Serial.println("It is dark, turn on the light.");
    //}else{
    //  digitalWrite(LED_BUILTIN, LOW);
    //  Serial.println("It is bright, turn off the light.");
    //}

    //Task 5: only react when dark/light changes
    //if (vOut >= thresholdDarkSurrounding && lightOn == false) {
    //  digitalWrite(LED_BUILTIN, HIGH);
    //  Serial.println("It is dark, turn on the light.");
    //  lightOn = true;
    //} else if (vOut <= thresholdDarkSurrounding && lightOn == true) {
    //  digitalWrite(LED_BUILTIN, LOW);
    //  Serial.println("It is bright, turn off the light.");
    //  lightOn = false;
    //}

    //Task 6: use external LED in addition to integral
    //if (vOut >= thresholdDarkSurrounding && lightOn == false) {
    //  digitalWrite(ledPin, HIGH);
    //  digitalWrite(LED_BUILTIN, HIGH);
    //  Serial.println("It is dark, turn on the light.");
    //  lightOn = true;
    //} else if (vOut <= thresholdDarkSurrounding && lightOn == true) {
    //  digitalWrite(ledPin, LOW);
    //  digitalWrite(LED_BUILTIN, LOW);
    //  Serial.println("It is bright, turn off the light.");
    //  lightOn = false;
    //}

    //Task 7: adaptive brightness LED; use analogWrite on Pin 5
    //if (vOut >= thresholdDarkSurrounding && lightOn == false) {
    //  digitalWrite(LED_BUILTIN, HIGH);
    //  Serial.println("It is dark, turn on the light.");
    //  lightOn = true;
    //} else if (vOut <= thresholdDarkSurrounding && lightOn == true) {
    //  analogWrite(ledPin, 0);
    //  digitalWrite(LED_BUILTIN, LOW);
    //  Serial.println("It is bright, turn off the light.");
    //  lightOn = false;
    //}

    //Task 7: adaptive brightness LED; adapt light: the darker outside, the brighter the LED
    //if (lightOn == true) {
    //  val = map(average, thresholdDarkBit, 1023, 0, 255);
    //  analogWrite(ledPin, val);
    //  Serial.println(average);
    //}

    if (vOut >= thresholdDarkSurrounding && lightOn == false) { //Task 9: different bright/dark thresholds
      lightOn = true;
      //lightInterval = (currentMillis - previousMillisSwitch) / 1000.00; //Task 8: measure time on/off
      //previousMillisSwitch = currentMillis; //Task 8: measure time on/off
      digitalWrite(LED_BUILTIN, HIGH);
      switchState(); //Task 10: function to switch state
      Serial.print("Dark! Period in which it was bright in the surrounding: ");
      Serial.println(lightInterval);
    } else if (vOut <= thresholdBrightSurrounding && lightOn == true) { //Task 9: different bright/dark thresholds
      lightOn = false;
      //lightInterval = (currentMillis - previousMillisSwitch) / 1000.00; //Task 8: measure time on/off
      //previousMillisSwitch = currentMillis; //Task 8: measure time on/off
      analogWrite(ledPin, 0);
      digitalWrite(LED_BUILTIN, LOW);
      switchState(); //Task 10: function to switch state
      Serial.print("Bright! Period in which it was dark in the surrounding: ");
      Serial.println(lightInterval);
    }

    if (lightOn == true) {
      val = map(average, thresholdDarkBit, 1023, 0, 255);
      analogWrite(ledPin, val);
    }
  }
}
