// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#include "max6675.h"
#include "config.h"
#include <AutoPID.h>

//fahrenheit was int, let's try to change it to a double
double fahrenheit = 0;
double temperature, setPoint, outputVal;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

//input/output variables passed by reference, so they are updated automatically
AutoPID myPID(&temperature, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

unsigned long lastTempUpdate; //tracks clock time of last temp update

//call repeatedly in loop, only updates after a certain time interval
//returns true if update happened
bool updateTemperature() {
  if ((millis() - lastTempUpdate) > TEMP_READ_DELAY) {
    temperature = thermocouple.readFahrenheit(); //get temp reading
    lastTempUpdate = millis();
    return true;
  }
  return false;
}//void updateTemperature
  
void setup() {
  Serial.begin(9600);
  setPoint = 220;
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  pinMode ( BUILTIN_LED, OUTPUT );
  pinMode(MOTOR_PIN, OUTPUT);
  
  Serial.println("BBQ PIT");
  // wait for MAX chip to stabilize
  delay(500);

  while (!updateTemperature()) {} //wait until temp sensor updated

  //if temperature is more than 4 degrees below or above setpoint, OUTPUT will be set to min or max respectively
  myPID.setBangBang(4);
  //set PID update interval to 4000ms
  myPID.setTimeStep(4000);

}

void loop() {
  // basic readout test, just print the x temp
   //fahrenheit = thermocouple.readFahrenheit();
   //celsius = thermocouple.readCelsius());

  updateTemperature();
  //setPoint = analogRead(POT_PIN);
  myPID.run(); //call every loop, updates automatically at certain time interval
  analogWrite(MOTOR_PIN, outputVal);
  digitalWrite(BUILTIN_LED, myPID.atSetPoint(1)); //light up LED when we're at setpoint +-1 degree

}
