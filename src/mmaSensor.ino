/*
TODO:
1. ability to change FREQUENCY via mobile app
2. ability to change CONST_GRAVITY via mobile app
3. handling request from mobile app to immediately check a "leak status"
    - checking what type of data Arduino will get
*/

#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define SENSOR_ID 1
#define FREQUENCY 100 // miliseconds
#define CONST_GRAVITY 9.81 // m/s

#define NO_LEAK_CODE 100 // TODO - point 3
#define SMALL_LEAK_CODE 200
#define LARGE_LEAK_CODE 300
#define BIG_LEAK_CODE 400

#define SMALL_LEAK_RANGE 0.3
#define LARGE_LEAK_RANGE 0.5
#define BIG_LEAK_RANGE 0.7


Adafruit_MMA8451 mmaSensor = Adafruit_MMA8451();

void setup(void) {
  Serial.begin(115200);
  if (! mmaSensor.begin()) {
    Serial.println("Sensor couldnt start! Check cables!");
    while (1);
  }
  mmaSensor.setRange(MMA8451_RANGE_2_G);
}

void loop() {
  int statusCode;
  int orientationCode;

  mmaSensor.read();
  sensors_event_t event;
  mmaSensor.getEvent(&event);

  //add orientation reader !

  float x_diff = abs(event.acceleration.x - CONST_GRAVITY);
  float y_diff = abs(event.acceleration.y - CONST_GRAVITY);
  float z_diff = abs(event.acceleration.z - CONST_GRAVITY);

  //orientationCode = orientationChecking(mmaSensor.getOrientation());
  statusCode = constLeakChecking(x_diff, y_diff, z_diff);
  if (statusCode != NO_LEAK_CODE);
  {
    //String message = String(code) + SENSOR_ID;
    //Serial.println(message);
    Serial.println(event.acceleration.x);
    Serial.println(event.acceleration.y);
    Serial.println(event.acceleration.z);
    orientationChecking(mmaSensor.getOrientation());
  }
  delay(FREQUENCY);
}

int constLeakChecking(float x_diff, float y_diff, float z_diff)
{
  int lstatus;

  if (x_diff > SMALL_LEAK_RANGE || y_diff > SMALL_LEAK_RANGE || z_diff > SMALL_LEAK_RANGE)
    lstatus = SMALL_LEAK_CODE;
  else if (x_diff > LARGE_LEAK_RANGE || y_diff > LARGE_LEAK_RANGE || z_diff > LARGE_LEAK_RANGE)
    lstatus = LARGE_LEAK_CODE;
  else if (x_diff > BIG_LEAK_RANGE || y_diff > BIG_LEAK_RANGE || z_diff > BIG_LEAK_RANGE)
    lstatus = BIG_LEAK_CODE;
  else
    lstatus = NO_LEAK_CODE;

  return lstatus;
}

void orientationChecking(int orientation)
{
  //int ostatus;
  switch (orientation) {
    case MMA8451_PL_PUF:
      Serial.println("Portrait Up Front"); //z
      break;
    case MMA8451_PL_PUB:
      Serial.println("Portrait Up Back"); //z
      break;
    case MMA8451_PL_PDF:
      Serial.println("Portrait Down Front"); //y
      break;
    case MMA8451_PL_PDB:
      Serial.println("Portrait Down Back");
      break;
    case MMA8451_PL_LRF:
      Serial.println("Landscape Right Front");
      break;
    case MMA8451_PL_LRB:
      Serial.println("Landscape Right Back");
      break;
    case MMA8451_PL_LLF:
      Serial.println("Landscape Left Front");
      break;
    case MMA8451_PL_LLB:
      Serial.println("Landscape Left Back");
      break;
    }
}
