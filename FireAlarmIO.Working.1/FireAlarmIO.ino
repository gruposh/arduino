// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;

// set up the 'house' feed
AdafruitIO_Feed *house = io.feed("house");

/************************ SMOKE ALARM ***************************************/
unsigned int SLEEP_TIME         = 32400; // Sleep time between reads (in seconds) 32400 = 9hrs
byte CYCLE_COUNTER                  = 2;  // This is the number of times we want the Counter to reach before triggering a CO signal to controller (Kidde should be 2).
byte CYCLE_INTERVAL        = 2; // How long do we want to watch for smoke or CO (in seconds). Kidde should be 2

byte oldValue;
byte coValue;
byte smokeValue;
volatile bool notificationSent = true;

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  pinMode(SMOKE_ALARM_PIN, INPUT_PULLUP);
  pinMode ( BUILTIN_LED, OUTPUT );
  digitalWrite(BUILTIN_LED, LOW);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  house->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  attachInterrupt(digitalPinToInterrupt(SMOKE_ALARM_PIN), setAlarmStatusFalse, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  SendNotification();  
  // wait one second (1000 milliseconds == 1 second)
  delay(5000);

}

// this function is called whenever a 'house' message
// is received from Adafruit IO. it was attached to
// the house feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  if (!strcmp((char*) data->value(), "HEALTHCHECK")) {
    Serial.println("HEALTHCHECK: Fire Alarm is ACTIVE");
    house->save("HEALTHCHECK: Fire Alarm is ACTIVE");
  }

}

void setAlarmStatusFalse()
{
      Serial.println("High");
      notificationSent = false;      
}

 void SendNotification()
 {
  Serial.print("NotificationSent=");
  Serial.println(notificationSent);
  if (!notificationSent) {
    Serial.println("SMOKE_ALARM: There is an alarm, checking...");
    house->save("SMOKE_ALARM: There is an alarm, checking...");
    AlarmStatus();
    notificationSent = true;
  }
 }

void AlarmStatus()
{

  // We will check the status now, this could be called by an interrupt or heartbeat
  byte value;
  byte wireValue;
  byte previousWireValue;
  byte siren_low_count   = 0;
  byte siren_high_count   = 0;
  unsigned long startedAt = millis();

  Serial.println("Status Check");
  //Read the Pin
  value = digitalRead(SMOKE_ALARM_PIN);
  // If Pin returns a 0 (LOW), then we have a Alarm Condition
  if (value == 0)
  {
    //We are going to check signal wire for CYCLE_INTERVAL time
    //This will be used to determine if there is smoke or carbon monoxide
    while (millis() - startedAt < CYCLE_INTERVAL * 1000)
    {
      wireValue = digitalRead(SMOKE_ALARM_PIN);
      if (wireValue != previousWireValue)
      {
        if (wireValue == 0)
        {
          siren_low_count++;
        }
        else
        {
          siren_high_count++;
        }
        previousWireValue = wireValue;
      }
    }
    // Eval siren hit count against our limit. If we are => then CYCLE_COUNTER then there is Carbon Monoxide
    if (siren_high_count >= CYCLE_COUNTER)
    {
      Serial.println("CO Detected");
      //Check to make sure we haven't already sent an update to controller
      if (coValue == 0 )
      {
        //update gateway CO is detected.
        house->save("SMOKE_ALARM: WARNING Carbon Monoxide Detected");
        Serial.println("CO Detected sent to gateway");
        coValue = 1;
      }
    }
    else
    {
      Serial.println("Smoke Detected");
      //Check to make sure we haven't already sent an update to controller
      if (smokeValue == 0 )
      {
        //update gateway smoke is detected.
        house->save("SMOKE_ALARM: WARNING Smoke Detected");
        Serial.println("Smoke Detected sent to gateway");
        smokeValue = 1;
      }
    }
    oldValue = value;
    AlarmStatus(); //run AlarmStatus() until there is no longer an alarm
  }
  //Pin returned a 1 (High) so there is no alarm.
  else
  {
    //If value has changed send update to gateway.
    if (oldValue != value)
    {
      //Send all clear msg to controller
      house->save("SMOKE_ALARM: All Clear, No alarm.");
      delay(DWELL_TIME); //allow the radio to regain power before transmitting again
      house->save("SMOKE_ALARM: Confirmation All Clear, No alarm.");
      oldValue = value;
      smokeValue = 0;
      coValue = 0;
      Serial.println("No Alarm");
    }
  }
}
