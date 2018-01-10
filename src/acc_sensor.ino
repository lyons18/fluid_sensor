#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

//Domyślna wartość częstitliwości - wartość w milisekundach
#define DEFAULT_FREQUENCY 100

//Domyślna, suma przyśpieszeń z 3 osi akcelerometru w spoczynku - wartość w m/s
#define CONST_SUM 10.50 // m/s

//Kody wycieków
#define NO_LEAK_CODE 100
#define SMALL_LEAK_CODE 200
#define LARGE_LEAK_CODE 300
#define BIG_LEAK_CODE 400

//Domyślne progi poszczególnych poziomów wycieku - wartości w milisekundach
#define DEFAULT_SMALL_LEAK_RANGE 12 
#define DEFAULT_LARGE_LEAK_RANGE 14 

int frequency = DEFAULT_FREQUENCY; //milis
int smallLeakRange = DEFAULT_SMALL_LEAK_RANGE; // m/s
int largeLeakRange = DEFAULT_LARGE_LEAK_RANGE; // m/s

//Utworzenie obiektu sensora, z którego będziemy pobierać dane
Adafruit_MMA8451 mmaSensor = Adafruit_MMA8451();

//W funkcji setup sprawdzamy, czy Arduino widzi połączenie z sensorem
//Jeśli nie, wyświetla komunikat o ponownym sprawdzeniu okablowania
void setup(void) {
  Serial.begin(9600);
  if (! mmaSensor.begin()) {
    Serial.println("Sensor couldnt start! Check cables!");
    while (1);
  }
  //Ustawienie domyślnego zakresu przyśpieszeń dla sensora 
  mmaSensor.setRange(MMA8451_RANGE_2_G);
}

void loop() {
  int statusCode;

  //Odczytywanie danych z sensora, ze wszystkich trzech osi
  mmaSensor.read();
  sensors_event_t event;
  mmaSensor.getEvent(&event);

  //Sumowanie danych ze wszystkich trzech osi a następnie sprawdzenie wycieku
  //na podstawie tej sumy funkcja constLeakChecking sprawdza czy nastąpił wyciek
  //i ustawia zmienną status code na odpowiedni kod wycieku
  float sum = abs(event.acceleration.x) + abs(event.acceleration.y) + abs(event.acceleration.z);
  statusCode = constLeakChecking(sum);

  //Jeśli wyciek następuje to wysyłamy wiadomość typu LEAK wraz z kodem wycieku
  if (statusCode != NO_LEAK_CODE)
  {
    String message = String(statusCode);
    Serial.print("LEAK:" + message);
  }

  //Nasłuchiwanie na zapytania od aplikacji
  if (Serial.available())
   {
     //Odczytywanie poszczególnych cześci wiadomości
     //Odpowiednie reagowanie na wiadomości w zależności od ich typu
     //oraz kodów 
     String messageType = Serial.readStringUntil(':');
     int messageDeviceType = Serial.readStringUntil(':').toInt();
     int messageFirstValue = Serial.readStringUntil(':').toInt();
     int messageSecondValue = Serial.readStringUntil(':').toInt();
     if (messageType == "GET")
     {
       switch( messageDeviceType )
        {
        case 100: //pobierz dane konfiguracyjne czujnika
            Serial.print("INFO:"+String(frequency)+":"+String(smallLeakRange)+":"+String(largeLeakRange));
            break;

        default:
            Serial.print("Incorrect device type! Try again!");
            break;
        }
     }
     else if (messageType == "PUT")
     {
       switch( messageDeviceType )
        {
        case 600: //zmień częstotliwość pobierania danych z czujnika
            frequency = messageFirstValue;
            break;
        case 700: //zmień progi poszczególnych poziomów wycieku
            smallLeakRange = messageFirstValue;
            largeLeakRange = messageSecondValue;
            break;
        case 900: //zresetuj wszystkie wartości do wartości domyślnych
            frequency = DEFAULT_FREQUENCY;
            smallLeakRange = DEFAULT_SMALL_LEAK_RANGE;
            largeLeakRange = DEFAULT_LARGE_LEAK_RANGE;
            break;
        default:
            Serial.print("Incorrect device type! Try again!");
            break;
        }
     }
     else
     {
       Serial.print("Incorrect message type! Try again!");
     }
   }
  delay(frequency);
}

//Funkcja określająca poziom wycieku na podstawie
//sumy ze wszystkich 3 osi akcelerometru 
int constLeakChecking(float sum)
  {
    int lstatus;

    if (sum < CONST_SUM)
      lstatus = NO_LEAK_CODE;
    else if (sum < smallLeakRange)
      lstatus = SMALL_LEAK_CODE;
    else if (sum < largeLeakRange)
      lstatus = LARGE_LEAK_CODE;
    else
      lstatus = BIG_LEAK_CODE;

    return lstatus;
  }
