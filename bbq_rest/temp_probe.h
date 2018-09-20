#include "temp_probe_config.h"

// Probe types used in probeType config
#define PROBETYPE_DISABLED 0  // do not read
#define PROBETYPE_INTERNAL 1  // read via analogRead()
#define PROBETYPE_RF12     2  // RFM12B wireless
#define PROBETYPE_TC_ANALOG  3  // Analog thermocouple, Stein[3] is mV/C

#define STEINHART_COUNT 4

//#define NOISEDUMP_PIN 5

struct __eeprom_probe
{
  char name[PROBE_NAME_SIZE];
  unsigned char probeType;
  char tempOffset;
  int alarmLow;
  int alarmHigh;
  char unused1;
  char unused2;
  float steinhart[STEINHART_COUNT];  // The last one is actually Rknown
};

#define ALARM_IDX_LOW  0
#define ALARM_IDX_HIGH 1
#define ALARM_ID_TO_PROBE(id) (id / 2)
#define ALARM_ID_TO_IDX(id) (id % 2)
#define MAKE_ALARM_ID(probe, idx) (probe * 2 + idx)

class ProbeAlarm
{
public:
  ProbeAlarm(void) {};

  // Check value against Low/High
  void updateStatus(int value);
  void setLow(int value);
  void setHigh(int value);
  int getLow(void) const { return Thresholds[ALARM_IDX_LOW]; }
  int getHigh(void) const { return Thresholds[ALARM_IDX_HIGH]; }
  boolean getLowEnabled(void) const { return Thresholds[ALARM_IDX_LOW] > 0; }
  boolean getHighEnabled(void) const { return Thresholds[ALARM_IDX_HIGH] > 0; }
  boolean getLowRinging(void) const { return Ringing[ALARM_IDX_LOW]; }
  boolean getHighRinging(void) const { return Ringing[ALARM_IDX_HIGH]; }
  void setThreshold(unsigned char idx, int value);
  int getThreshold(unsigned char idx) const { return Thresholds[idx]; }
  void silenceAll(void) { setThreshold(ALARM_IDX_LOW, 0); setThreshold(ALARM_IDX_HIGH, 0); }
  int Thresholds[2];
  boolean Ringing[2];
  boolean Armed[2];
};

#define TSTATUS_NONE  'U'
#define TSTATUS_HIGH  'H'
#define TSTATUS_LOW   'L'
#define TSTATUS_OK    'O'

class TempProbe
{
private:
  const unsigned char _pin;
  unsigned char _probeType;
  char _tempStatus;
  boolean _hasTempAvg;

public:
  TempProbe(const unsigned char pin);

  const unsigned char getPin(void) const { return _pin; }

  /* Configuration */
  // Probe Type
  unsigned char getProbeType(void) const { return _probeType; }
  void setProbeType(unsigned char probeType);
  // Offset (in degrees) applied when calculating temperature
  char Offset;
  // Steinhart coefficients
  float Steinhart[STEINHART_COUNT];
  // Copy struct to members
  void loadConfig(struct __eeprom_probe *config);

  /* Runtime Data/Methods */
  // Last averaged temperature reading
  float Temperature;
  boolean hasTemperature(void) const { return _tempStatus == TSTATUS_OK; }
  char getTempStatus(void) const { return _tempStatus; }
  void setTemperatureC(float T);
  // Temperature moving average
  float TemperatureAvg;
  boolean hasTemperatureAvg(void) const { return _hasTempAvg; }
  // Convert ADC to Temperature
  void calcTemp(unsigned int _accumulator);
  // Perform once-per-period processing
  void processPeriod(void);
  // Output status to serial
  void status(void) const; 

  ProbeAlarm Alarms;
};
