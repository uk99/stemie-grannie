int digitalPinrelais = 6; // pour le relais
int analogPin = A0; // pour le capteur de l'humidité du    sol
int digitalVal; // digital readings
int analogVal; //analog readings
int val ;
#include "RTClib.h"

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void setup()
{
  Serial.begin(9600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  pinMode(digitalPinrelais, OUTPUT);
  pinMode (4, INPUT); // module signal output connected to Arduino pin 8
  digitalWrite(digitalPinrelais, LOW);
}

void loop()
{
  DateTime now = rtc.now();

  if (now.day() % 7 != 0) {
    delay(10000);
    return;
  }

  Serial.println("c'est le moment d'arroser");
  
  val = digitalRead (4);
  analogVal = analogRead(analogPin);//lire la valeur retournée par le capteur de l'humidité
  Serial.println(val ? "La bassine est pleine" : "La bassine est vide");
  Serial.println(analogVal > 750 ? "Il fait sec" : "Il fait Humide");

  if (analogVal > 750 && val) { //si la capteur de l'humidité retourne une valeur<500
    digitalWrite(digitalPinrelais, HIGH);//La pompe à eau arrose la plante
  }
  else { // sinon
    digitalWrite(digitalPinrelais, LOW); //La pompe à eau arrête l'arrosage
  }
  delay(1000);
}
