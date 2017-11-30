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
#define CONST_SUM 10.50 // m/s

#define NO_LEAK_CODE 100
#define SMALL_LEAK_CODE 200
#define LARGE_LEAK_CODE 300
#define BIG_LEAK_CODE 400

#define SMALL_LEAK_RANGE 12
#define LARGE_LEAK_RANGE 14


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

  mmaSensor.read();
  sensors_event_t event;
  mmaSensor.getEvent(&event);

  float sum = abs(event.acceleration.x) + abs(event.acceleration.y) + abs(event.acceleration.z);
  statusCode = constLeakChecking(sum);
  if (statusCode != NO_LEAK_CODE)
  {
    String message = String(statusCode) + SENSOR_ID;
    Serial.println(message);
  }
  delay(FREQUENCY);
}

int constLeakChecking(float sum)
  {
    int lstatus;

    if (sum < CONST_SUM)
      lstatus = NO_LEAK_CODE;
    else if (sum < SMALL_LEAK_RANGE)
      lstatus = SMALL_LEAK_CODE;
    else if (sum < LARGE_LEAK_RANGE)
      lstatus = LARGE_LEAK_CODE;
    else
      lstatus = BIG_LEAK_CODE;

    return lstatus;
  }
