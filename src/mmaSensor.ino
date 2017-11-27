#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mmaSensor = Adafruit_MMA8451();

void setup(void) {
  Serial.begin(115200);
  if (! mmaSensor.begin()) {
    Serial.println("Sensor couldnt start! Check cables!");
    while (1);
  }
  Serial.println("Sensor MMA8451 found!");
  mmaSensor.setRange(MMA8451_RANGE_2_G);
}

void loop() {
  mmaSensor.read();
  sensors_event_t event;
  mmaSensor.getEvent(&event);
  float dataArray[3] = {event.acceleration.x, event.acceleration.y, event.acceleration.z};

  //Here should be code for BLE - convert array and send it via Bluetooth

  Serial.println("Sending data...");
  delay(1500);
}
