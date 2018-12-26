#include "config.h"

/************************ Example Starts Here *******************************/

// https://www.hackster.io/amal-ns/hall-effect-sensor-with-arduino-390916

// this int will hold the current count for our sketch
int count = 0;

// set up the 'house' feed
AdafruitIO_Feed *house = io.feed("homeless");

/************************ DOOR ALARM ***************************************/
int doorReading = 0;     // the analog reading from the sensor divider
int lightMinValue = 15;
String alarmMessage;

volatile bool notificationSent = true;

void setup() {
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), AlarmStatus, CHANGE);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);



  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  house->onMessage(handleMessage);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  Serial.println(String(ALARM_NAME) + " DOOR Alarm is READY");
  house->save(String(ALARM_NAME) + " DOOR Alarm is READY");
  digitalWrite(BUILTIN_LED, HIGH);

}

void loop() {

  io.run();
  // wait one second (1000 milliseconds == 1 second)
  delay(CYCLE_INTERVAL * 1000);

}

// this function is called whenever a 'house' message
// is received from Adafruit IO. it was attached to
// the house feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  if (!strcmp((char*) data->value(), "HEALTHCHECK")) {
    Serial.println("HEALTHCHECK: Door Alarm is ACTIVE");
    house->save(String("HEALTHCHECK: Door Alarm is ACTIVE"));
    house->save(String("HEALTHCHECK: Door is") + getStatus());
  }

}

String getStatus() {
  return (digitalRead(INTERRUPT_PIN) == 1)?String(": OPEN"):String(": CLOSE");
}

void AlarmStatus()
{
//  doorReading = digitalRead(INTERRUPT_PIN);
//
//  Serial.print("Analog reading = ");
//  Serial.println(doorReading);

  house->save(String(ALARM_NAME) + getStatus());
  Serial.println(String(ALARM_NAME) + getStatus());
}

