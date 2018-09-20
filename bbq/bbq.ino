// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#include "max6675.h"

int thermoDO = D6;
int thermoCS = D7;
int thermoCLK = D8;
int MOTOR_PIN = D1;

const double MIN_TEMP = 75;
const double MAX_TEMP = 78;
int fahrenheit = 0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
int vccPin = 3;
int gndPin = 2;
  
void setup() {
  Serial.begin(9600);
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  pinMode ( BUILTIN_LED, OUTPUT );
  pinMode(MOTOR_PIN, OUTPUT);

  
  Serial.println("BBQ PIT");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the x temp
   fahrenheit = thermocouple.readFahrenheit();
   Serial.print("C = "); 
   Serial.println(thermocouple.readCelsius());
   Serial.print("F = ");
   Serial.println(fahrenheit);

   if ( fahrenheit < MIN_TEMP ) {
    Serial.println("FAN ON");
    digitalWrite ( BUILTIN_LED, HIGH);
    digitalWrite(MOTOR_PIN, HIGH);    
   } 
   
   if (fahrenheit > MAX_TEMP) {
    Serial.println("FAN OFF");
    digitalWrite ( BUILTIN_LED, LOW);
    digitalWrite(MOTOR_PIN, LOW);
        
   }
   delay(1000);
}
