//pins
#define thermoDO  D6
#define thermoCS  D7
#define thermoCLK D8
#define MOTOR_PIN  D1

#define vccPin 3
#define gndPin 2


//pid settings and gains
#define TEMP_READ_DELAY 800 //can only read digital temp sensor every ~750ms
#define OUTPUT_MIN 0
#define OUTPUT_MAX 255
#define KP .12
#define KI .0003
#define KD 0



