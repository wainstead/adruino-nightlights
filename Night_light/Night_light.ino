/*

This sketch contains code borrowed from other sketches that came with the Vilros
Ultimate Starter Kit for Arduino. Original license follows:

This sketch was written by SparkFun Electronics,
with lots of help from the Arduino community.
This code is completely free for any use.
Visit http://www.arduino.cc to learn about the Arduino.

Version 2.0 6/2012 MDG
*/

int ledPins[] = {2, 3, 4, 5, 6, 7};
const int arrayLength = 6;
const int redSignalPin = 13;

const int sensorPin = 0;

int lightLevel, high = 0, low = 1023;
int idx = 0;

void setup()
{
  // Set up the digital pins as outputs; this is our line
  // of yellow LEDs
  for (idx = 0; idx <= arrayLength; idx++) {
    pinMode(ledPins[idx], OUTPUT);
  }
  Serial.begin(9600);
  Serial.println("Setup complete!");
}


void loop()
{
  // We'll use the analogRead()
  // function to measure the voltage coming from the photoresistor/
  // resistor pair. This number can range between 0 (0 Volts) and
  // 1023 (5 Volts), but this circuit will have a smaller range
  // between dark and light.

  lightLevel = analogRead(sensorPin);
  //Serial.println(lightLevel);

  //manualTune();  // manually change the range from light to dark
  autoTune();  // have the Arduino do the work for us!

  // The above functions will alter lightLevel to be cover the
  // range from full-on to full-off. Now we can adjust the
  // brightness of the LED:
  if (lightLevel > 140) {
    // It's dark and we want the light show
    analogWrite(redSignalPin, 0);
    // Choose one of the following functions for pretty light sequences
    oneAfterAnotherLoop();
    oneAfterAnotherLoop();
    oneOnAtATime();
    oneOnAtATime();
    //pingPong();
    marquee();
    marquee();
  } else {
    // The ambient light is too bright, light up the red signal pin
    analogWrite(redSignalPin, 200);
    // turn off all yellow LEDs
    for (idx = 0; idx <= arrayLength; idx++) {
      analogWrite(ledPins[idx], 0);
    }
  }
  // The above statement will brighten the LED along with the
  // light level. To do the opposite, replace "lightLevel" in the
  // above analogWrite() statement with "255-lightLevel".
  // Now you've created a night-light!
}

void oneAfterAnotherLoop()
{
  int index;
  int delayTime = 100; // milliseconds to pause between LEDs
                       // make this smaller for faster switching

  // Turn all the LEDs on:

  // This for() loop will step index from 0 to 7
  // (putting "++" after a variable means add one to it)
  // and will then use digitalWrite() to turn that LED on.

  for(index = 0; index < arrayLength; index++) {
    digitalWrite(ledPins[index], HIGH);
    delay(delayTime);
  }

  // Turn all the LEDs off:

  // This for() loop will step index from 7 to 0
  // (putting "--" after a variable means subtract one from it)
  // and will then use digitalWrite() to turn that LED off.

  for(index = arrayLength; index >= 0; index--)
  {
    digitalWrite(ledPins[index], LOW);
    delay(delayTime);
  }
}

/*
oneOnAtATime()

This function will step through the LEDs,
lighting only one at at time.
*/

void oneOnAtATime()
{
  int index;
  int delayTime = 100; // milliseconds to pause between LEDs
                       // make this smaller for faster switching

  // step through the LEDs, from 0 to 7

  for(index = 0; index < arrayLength; index++)
  {
    digitalWrite(ledPins[index], HIGH);  // turn LED on
    delay(delayTime);                    // pause to slow down
    digitalWrite(ledPins[index], LOW);   // turn LED off
  }
}

/*
pingPong()

This function will step through the LEDs,
lighting one at at time in both directions.
*/

void pingPong()
{
  int index;
  int delayTime = 100; // milliseconds to pause between LEDs
                       // make this smaller for faster switching
  Serial.println("Entered pingPong...");
  for (index = 0; index < arrayLength; index++)
  {
    digitalWrite(ledPins[index], HIGH);  // turn LED on
    delay(delayTime);                    // pause to slow down
    digitalWrite(ledPins[index], LOW);   // turn LED off
  }

  for(index = arrayLength; index > 0; index--)
  {
    digitalWrite(ledPins[index], HIGH);  // turn LED on
    delay(delayTime);                    // pause to slow down
    digitalWrite(ledPins[index], LOW);   // turn LED off
  }
  Serial.println("Leaving pingPong...");
}

/*
marquee()

This function will mimic "chase lights" like those around signs.
*/

void marquee()
{
  int index;
  int delayTime = 200; // milliseconds to pause between LEDs
                       // Make this smaller for faster switching

  // Step through the first four LEDs
  // (We'll light up one in the lower 4 and one in the upper 4)

  for(index = 0; index <= 3; index++) // Step from 0 to 3
  {
    digitalWrite(ledPins[index], HIGH);    // Turn a LED on
    digitalWrite(ledPins[index+2], HIGH);  // Skip four, and turn that LED on
    delay(delayTime);                      // Pause to slow down the sequence
    digitalWrite(ledPins[index], LOW);     // Turn the LED off
    digitalWrite(ledPins[index+2], LOW);   // Skip four, and turn that LED off
  }
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
