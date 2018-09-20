#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "config.h"

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

ESP8266WebServer server ( 80 );

void setup ( void ) {
  pinMode ( BUILTIN_LED, OUTPUT );
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

  server.on ( "/setLowTemp", []() {   
//    digitalWrite ( GARAGE_CLICK_PIN, LOW);
//    server.send ( 200, "text/html", "OK" );
//    digitalWrite ( GARAGE_CLICK_PIN, HIGH);
    
  } );
  server.on ( "/setHighTemp", []() {   
//    digitalWrite ( GARAGE_LIGHT_PIN, LOW);//We always want to send a high value
//    server.send ( 200, "text/html", "OK" );
//    digitalWrite ( GARAGE_LIGHT_PIN, HIGH);
    
  } );
  server.on ( "/getCurrentTemp", []() {
//    int value = digitalRead ( GARAGE_STATUS_PIN );
//    int bufsize = 9;
//    char temp[bufsize];
//  
//    snprintf ( temp, bufsize, "status=%d", value );
//    server.send ( 200, "text/html", temp );
    
  } );  
  server.on ( "/getLowTemp", []() {
//    int value = digitalRead ( GARAGE_STATUS_PIN );
//    int bufsize = 9;
//    char temp[bufsize];
//  
//    snprintf ( temp, bufsize, "status=%d", value );
//    server.send ( 200, "text/html", temp );
    
  } );
  server.on ( "/getHighTemp", []() {
//    int value = digitalRead ( GARAGE_STATUS_PIN );
//    int bufsize = 9;
//    char temp[bufsize];
//  
//    snprintf ( temp, bufsize, "status=%d", value );
//    server.send ( 200, "text/html", temp );
//    
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

    digitalWrite(BUILTIN_LED, HIGH);
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

