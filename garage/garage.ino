#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "dlink-0695";
const char *password = "eczwz46963";

ESP8266WebServer server ( 80 );

const int GARAGE_STATUS_PIN = D3;
const int GARAGE_CLICK_PIN = D1;
const int GARAGE_LIGHT_PIN = D2;


void setup ( void ) {
  pinMode ( GARAGE_STATUS_PIN, INPUT );
  pinMode ( GARAGE_LIGHT_PIN, OUTPUT );
  pinMode ( GARAGE_CLICK_PIN, OUTPUT );
  pinMode ( BUILTIN_LED, OUTPUT );
  digitalWrite ( GARAGE_CLICK_PIN, HIGH);
  digitalWrite ( GARAGE_LIGHT_PIN, HIGH);
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on ( "/click", []() {   
    digitalWrite ( GARAGE_CLICK_PIN, LOW);
    server.send ( 200, "text/html", "OK" );
    digitalWrite ( GARAGE_CLICK_PIN, HIGH);
    ledServerUp();
  } );
  server.on ( "/light", []() {   
    digitalWrite ( GARAGE_LIGHT_PIN, LOW);//We always want to send a high value
    server.send ( 200, "text/html", "OK" );
    digitalWrite ( GARAGE_LIGHT_PIN, HIGH);
    ledServerUp();
  } );
  server.on ( "/status", []() {
    int value = digitalRead ( GARAGE_STATUS_PIN );
    int bufsize = 9;
    char temp[bufsize];
  
    snprintf ( temp, bufsize, "status=%d", value );
    server.send ( 200, "text/html", temp );
    ledServerUp();
  } );
  
  server.on ( "/up", []() {
    server.send ( 200, "text/plain", "status=ok" );
  } );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );

  ledServerUp();
}

void ledServerUp() {
  for (int i =0; i<5; i++) {
    digitalWrite ( BUILTIN_LED, HIGH);
    delay(250);
    digitalWrite ( BUILTIN_LED, LOW);    
  }
  
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
  server.handleClient();
}

