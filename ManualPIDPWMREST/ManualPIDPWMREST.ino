#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <PID_v1.h>
#include "max6675.h"

#include "config.h"

/**
 * ManualPWM
 * ManualPID
 * ManualPIDPWM
 * ManualPIDPWMREST
 */

/**
 * 
 * BBQ REST SERVER
 * Alberto Ladron
 * March 8, 2018
 * 
 * REST Operations
 * 
 * 
 */

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Define the aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

ESP8266WebServer server ( 80 );

MAX6675 thermocouple(THERMO_CLK, THERMO_CS, THERMO_DO);

void setup ( void ) {
  pinMode ( BUILTIN_LED, OUTPUT );

  //MAX6675 PINS
  pinMode(VCC_PIN, OUTPUT); digitalWrite(VCC_PIN, HIGH);
  pinMode(GND_PIN, OUTPUT); digitalWrite(GND_PIN, LOW);
  
  digitalWrite(BUILTIN_LED, LOW);
  
  Serial.begin ( 115200 );
  WiFi.begin ( WIFI_SSID, WIFI_PASS );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( WIFI_SSID );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on( "/admin/lm/set", []() {
    String output = "[";

    output += "]";
  } );


  server.on ( "/getFanStatus", []() {
//    int value = digitalRead ( GARAGE_STATUS_PIN );
//    int bufsize = 9;
//    char temp[bufsize];
//  
//    snprintf ( temp, bufsize, "status=%d", value );
//    server.send ( 200, "text/html", temp );
//    
  } );
  


  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );

  initializePID();

  digitalWrite(BUILTIN_LED, HIGH);
}

void initializePID() {
  //initialize the variables we're linked to
  Input = thermocouple.readFahrenheit();
  Setpoint = 100;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 100);  
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

void loop ( void ) {
  
  compute(Input);
  pwm(Output);
  server.handleClient();
}

void compute(double _Input) {
  double gap = abs(Setpoint-Input); //distance away from setpoint
  if (gap < 10)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();  
}

void pwm(int speed) {
  Serial.print("Speed: ");
  Serial.println(speed);
  digitalWrite(FAN_PIN, HIGH);
  delayMicroseconds(speed * 10); // 100 Approximately 10% duty cycle @ 1KHz
  digitalWrite(FAN_PIN, LOW);
  delayMicroseconds(1000 - (speed * 10));  
}

