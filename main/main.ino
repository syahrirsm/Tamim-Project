//TAMIM
#include <SoftwareSerial.h>
#define dialPin 2
#define handsetPin 3
#define ledPin 4
int count;
int digit;
unsigned long number = 0;
unsigned long prev;
bool hold = true;
bool holdBtn = true;
String _buffer;
int _timeout;
String countri = "+49";

SoftwareSerial sim(10, 11); // Pin 10 to TX (SIM800), Pin 11 to RX (SIM800)

void setup() {
  sim.begin(9600);
  Serial.begin(9600);
  pinMode(dialPin, INPUT);
  pinMode(handsetPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {

  if (digitalRead(dialPin) == LOW) {
    while (digitalRead(dialPin) == LOW) {}
    hold = true;
    prev = millis();
    if (count < 10) {
      count++;
      Serial.print(count);
    }
  }

  if (millis() - prev > 1000) {
    if (hold == true) {
      if (count == 10) {
        count = 0;
      }
      hold = false;
      number = number * 10 + (count - 0);
      digit++;
      count = 0;
      Serial.println();
      Serial.println(number);
    }
  }

  if (digitalRead(handsetPin) == LOW && digit > 12) {
    Serial.println("Calling: " + countri + String(number));
    callNumber(String(number));
    number = 0;
    digit = 0;
  }
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
