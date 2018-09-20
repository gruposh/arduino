/**
 * ManualPWM
 * ManualPID
 * ManualPIDPWM
 * ManualPIDPWMREST
 */

#define FAN_PIN 13
int speed = 100;
String inString = ""; 

void setup()
{
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void loop()
{
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }        

    if (inChar == '\n') {
      speed = inString.toInt();
      inString = "";
    }
  }

  pwm(speed);

}

void pwm(int speed) {
  Serial.print("Speed: ");
  Serial.println(speed);
  if (speed > 0) {
    digitalWrite(FAN_PIN, HIGH);
    delayMicroseconds(speed * 10); // 100 Approximately 10% duty cycle @ 1KHz
  }
  if (speed < 100) {
    digitalWrite(FAN_PIN, LOW);
    delayMicroseconds(1000 - (speed * 10));  
  }
}

