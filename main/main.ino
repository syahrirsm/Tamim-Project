//TAMIM
#include <SoftwareSerial.h>
#define dialPin 2
#define handsetPin 5
#define ledPin 4
int count;
int digit;
String number = "";
unsigned long prev;
bool hold = true;
bool holdBtn = true;
String _buffer;
int _timeout;
String countri = "+62";
bool looping = false;
SoftwareSerial sim(10, 11); // Pin 10 to TX (SIM800), Pin 11 to RX (SIM800)

void setup() {
  sim.begin(9600);
  Serial.begin(9600);
  pinMode(dialPin, INPUT_PULLUP);
  pinMode(handsetPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.println("READY");
}

void loop() {

  if (digitalRead(dialPin) == LOW) {
    looping = true;
    while (digitalRead(dialPin) == LOW) {}
    hold = true;
    prev = millis();
    count++;
    Serial.print(count);
    if (count > 10) {
      count = 1;
    }
  }
  if (millis() - prev > 1000) {
    if (hold == true) {
      if (count == 10) {
        count = 0;
      }
      hold = false;
      if (looping == true) {
        number += String(count);
      }
      digit++;
      count = 0;
      Serial.println();
      Serial.println(number);
    }
  }


  if (digitalRead(handsetPin) == LOW && digit > 10) {
    Serial.println("Calling: " + countri + number);
    callNumber(number);
    number = "";
    digit = 0;
  }

  //  looping = true;
}

void callNumber(String num) {
  sim.print (F("ATD"));
  sim.print (countri + number);
  sim.print (F(";\r\n"));
  digitalWrite(ledPin, HIGH);
  _buffer = _readSerial();
  digitalWrite(ledPin, LOW);
  Serial.println(_buffer);
}
String _readSerial() {

  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
