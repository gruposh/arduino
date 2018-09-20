
#define MOTOR_PIN D1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Setup ready");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(MOTOR_PIN, LOW);
        delay(3000);
        digitalWrite(MOTOR_PIN, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(3000);
}
