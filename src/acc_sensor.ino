#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define DEFAULT_FREQUENCY 100 //millis
#define CONST_SUM 10.50 // m/s

#define NO_LEAK_CODE 100
#define SMALL_LEAK_CODE 200
#define LARGE_LEAK_CODE 300
#define BIG_LEAK_CODE 400

#define DEFAULT_SMALL_LEAK_RANGE 12 // m/s
#define DEFAULT_LARGE_LEAK_RANGE 14 // m/s

int frequency = DEFAULT_FREQUENCY; //milis
int smallLeakRange = DEFAULT_SMALL_LEAK_RANGE; // m/s
int largeLeakRange = DEFAULT_LARGE_LEAK_RANGE; // m/s

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
    String message = String(statusCode);
    Serial.print("LEAK:" + message);
  }

  if (Serial.available())
   {
     String messageType = Serial.readStringUntil(':');
     int messageDeviceType = Serial.readStringUntil(':').toInt();
     int messageFirstValue = Serial.readStringUntil(':').toInt();
     int messageSecondValue = Serial.readStringUntil(':').toInt();
     if (messageType == "GET")
     {
       switch( messageDeviceType )
        {
        case 100: //get all sensor's info
            Serial.println("INFO:" + String(frequency));
            break;

        default:
            Serial.println("Incorrect device type! Try again!");
            break;
        }
     }
     else if (messageType == "PUT")
     {
       switch( messageDeviceType )
        {
        case 600: //change frequency
            frequency = messageFirstValue;
            break;
        case 700: //change leak ranges
            smallLeakRange = messageFirstValue;
            largeLeakRange = messageSecondValue;
        case 900: //reset all values
            frequency = DEFAULT_FREQUENCY;
            smallLeakRange = DEFAULT_SMALL_LEAK_RANGE;
            largeLeakRange = DEFAULT_LARGE_LEAK_RANGE;
        default:
            Serial.println("Incorrect device type! Try again!");
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
