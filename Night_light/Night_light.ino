/*

This sketch was written by SparkFun Electronics,
with lots of help from the Arduino community.
This code is completely free for any use.
Visit http://www.arduino.cc to learn about the Arduino.

Version 2.0 6/2012 MDG
*/

int ledPins[] = {2,3,4}; //,5,6,7,8,9};
const int arrayLength = 3;
const int redSignalPin = 13;

const int sensorPin = 0;

// We'll also set up some global variables for the light level:

int lightLevel, high = 0, low = 1023;
int idx = 0;
  
void setup()
{
  // We'll set up the LED pin to be an output.
  // (We don't need to do anything special to use the analog input.)
  
  for (idx = 0; idx <= arrayLength; idx++) {
    pinMode(ledPins[idx], OUTPUT);
  }
  Serial.begin(9600);
  Serial.println("Setup complete!");
}


void loop()
{
  // Just as we've done in the past, we'll use the analogRead()
  // function to measure the voltage coming from the photoresistor
  // resistor pair. This number can range between 0 (0 Volts) and
  // 1023 (5 Volts), but this circuit will have a smaller range
  // between dark and light.

  lightLevel = analogRead(sensorPin);
  Serial.println(lightLevel);

  manualTune();  // manually change the range from light to dark
  //autoTune();  // have the Arduino do the work for us!

  // The above functions will alter lightLevel to be cover the
  // range from full-on to full-off. Now we can adjust the
  // brightness of the LED:
  if (lightLevel > 140) {
    // It's dark and we want the light show
    analogWrite(redSignalPin, 0);
    for (idx = 0; idx <= 7; idx++) {
      analogWrite(ledPins[idx], lightLevel);
    }
  } else {
    analogWrite(redSignalPin, 200);
    for (idx = 0; idx <= 7; idx++) {
      analogWrite(ledPins[idx], 0);
    }
  }
  // The above statement will brighten the LED along with the
  // light level. To do the opposite, replace "lightLevel" in the
  // above analogWrite() statement with "255-lightLevel".
  // Now you've created a night-light!
}


void manualTune()
{
  lightLevel = map(lightLevel, 0, 1023, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
  //Serial.print("manualTune: lightLevel: ");
  //Serial.println(lightLevel);
} 


void autoTune()
{
  // As we mentioned above, the light-sensing circuit we built
  // won't have a range all the way from 0 to 1023. It will likely
  // be more like 300 (dark) to 800 (light).
  
  // In the manualTune() function above, you need to repeatedly
  // change the values and try the program again until it works.
  // But why should you have to do that work? You've got a
  // computer in your hands that can figure things out for itself!

  // In this function, the Arduino will keep track of the highest
  // and lowest values that we're reading from analogRead().

  // If you look at the top of the sketch, you'll see that we've
  // initialized "low" to be 1023. We'll save anything we read
  // that's lower than that:
  
  if (lightLevel < low)
  {
    low = lightLevel;
  }

  // We also initialized "high" to be 0. We'll save anything
  // we read that's higher than that:
  
  if (lightLevel > high)
  {
    high = lightLevel;
  }
  
  // Once we have the highest and lowest values, we can stick them
  // directly into the map() function. No manual tweaking needed!
  
  // One trick we'll do is to add a small offset to low and high,
  // to ensure that the LED is fully-off and fully-on at the limits
  // (otherwise it might flicker a little bit).
  
  lightLevel = map(lightLevel, low+30, high-30, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
  
  // Now we'll return to the main loop(), and send lightLevel
  // to the LED.
}
