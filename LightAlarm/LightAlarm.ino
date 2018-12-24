#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;

// set up the 'house' feed
AdafruitIO_Feed *house = io.feed("homeless");

/************************ LIGHT ALARM ***************************************/
int photocellReading = 0;     // the analog reading from the sensor divider
int lightMinValue = 15;
String alarmMessage;

volatile bool notificationSent = true;

void setup() {

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

  Serial.println("LIGHT_ALARM: LIGHT Alarm is ACTIVE");
  house->save("LIGHT_ALARM: LIGHT Alarm is ACTIVE");
  digitalWrite(BUILTIN_LED, HIGH);

}

void loop() {

  io.run();

  AlarmStatus();
  // wait one second (1000 milliseconds == 1 second)
  delay(CYCLE_INTERVAL * 1000);

}

// this function is called whenever a 'house' message
// is received from Adafruit IO. it was attached to
// the house feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  if (!strcmp((char*) data->value(), "HEALTHCHECK")) {
    Serial.println("HEALTHCHECK: Light Alarm is ACTIVE");
    house->save(String("HEALTHCHECK: Light Alarm is ACTIVE, photocellReading=") + photocellReading);
  }

}

void setAlarmStatusFalse()
{
  Serial.println("High");
  notificationSent = false;
}

void SendNotification(int value)
{
  Serial.print("NotificationSent= ");
  Serial.println(value);
  alarmMessage = String("LIGHT_ALARM: ") + value;
  
  house->save(alarmMessage);
  delay(1000);
}

void AlarmStatus()
{
  photocellReading = analogRead(PHOTOCELL_PIN);

  Serial.print("Analog reading = ");
  Serial.println(photocellReading);
  
  if (photocellReading >= lightMinValue) {  
    SendNotification(photocellReading);
  }
}

