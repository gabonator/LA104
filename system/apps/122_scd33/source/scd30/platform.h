#include <library.h>

typedef uint8_t byte;
typedef bool boolean;

class TwoWire
{
public:
  void begin();
  void end();
  bool beginTransmission(int addr);
  int read();
  void write(int d);
  bool endTransmission(bool stop = true);
  void requestFrom(int addr, int count);
};

void delay(int d);
extern TwoWire Wire;

struct sensors_event_t {
  int version;
  int temperature;
  int type;
  int timestamp;
  int sensor_id;
  int relative_humidity;
};
enum {SENSOR_TYPE_AMBIENT_TEMPERATURE, SENSOR_TYPE_RELATIVE_HUMIDITY};

struct sensor_t {
  int min_value, max_value, resolution;
  int type;
  char name[16];
  int sensor_id;
  int min_delay;
  int version;
};

class Adafruit_Sensor {
};


class Adafruit_I2CDevice : public TwoWire
{
public:
  int address;
  bool begin() {TwoWire::begin(); return true;}
  bool write(uint8_t* buf, int len);
  bool read(uint8_t* buf, int len);
};

extern Adafruit_I2CDevice commoni2c; ///< Pointer to I2C bus interface

long millis();