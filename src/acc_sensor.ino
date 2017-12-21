#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define SENSOR_ID 1
#define DEFAULT_FREQUENCY 100 //millis
#define CONST_SUM 10.50 // m/s

#define NO_LEAK_CODE 100
#define SMALL_LEAK_CODE 200
#define LARGE_LEAK_CODE 300
#define BIG_LEAK_CODE 400

#define SMALL_LEAK_RANGE 12
#define LARGE_LEAK_RANGE 14

int frequency = DEFAULT_FREQUENCY; //milis

Adafruit_MMA8451 mmaSensor = Adafruit_MMA8451();

void setup(void) {
  Serial.begin(9600);
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

  if (Serial.available())
   {
     String messageType = Serial.readStringUntil(':');
     int messageDeviceType = Serial.readStringUntil(':').toInt();
     int messageValue = Serial.readStringUntil(':').toInt();
     if (messageType == "GET")
     {
       //handling GET messages
     }
     else if (messageType == "PUT")
     {
       switch( messageDeviceType )
        {
        case 600: //change frequency
            frequency = messageValue;
            break;

        default:
            frequency = DEFAULT_FREQUENCY;
            Serial.println("Incorrect device type! Try again!");
            Serial.println("Frequency changed to default");
            break;
        }
     }
     else
     {
       Serial.println("Incorrect message type! Try again!");
     }
   }

  delay(frequency);
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
