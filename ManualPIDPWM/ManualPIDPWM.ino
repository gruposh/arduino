/**
 * ManualPWM
 * ManualPID
 * ManualPIDPWM
 * ManualPIDPWMREST
 */

#include <PID_v1.h>

#define PIN_TERMOMETHER 0
#define PIN_OUTPUT 3
#define FAN_PIN 13

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Define the aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void setup()
{
  //initialize the variables we're linked to
  Input = analogRead(PIN_TERMOMETHER);
  Setpoint = 100;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 100);
}

void loop()
{
  Input = analogRead(PIN_TERMOMETHER);
  compute(Input);

  pwm(Output);
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

void setOutputLimits(double _min, double _max) {
  myPID.SetOutputLimits(_min, _max);  
}

void setAggTunings(double _kp, double _ki, double _kd) {
  aggKp = _kp;
  aggKi = _ki;
  aggKd = _kd;
}

void setConsTunings(double _kp, double _ki, double _kd) {
  consKp = _kp;
  consKi = _ki;
  consKd = _kd;  
}

void setSetpoint(double _Setpoint) {
  Setpoint = _Setpoint;
}

