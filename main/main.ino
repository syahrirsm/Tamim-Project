//TAMIM
#include <SoftwareSerial.h>
#define dialPin 2
#define handsetPin 3
#define ledPin 4
#define ring 9
int count;
int digit;
String number = "";
unsigned long prev, prev2;
bool hold = true;
bool holdBtn = true;
String _buffer;
int _timeout;
String countri = "+62"; //countri code
bool looping = false;
int action = 0;
SoftwareSerial sim(10, 11); // Pin 10 to TX (SIM800), Pin 11 to RX (SIM800)

void setup() {
  sim.begin(9600);
  Serial.begin(9600);
  pinMode(dialPin, INPUT_PULLUP);
  pinMode(handsetPin, INPUT_PULLUP);
  pinMode(ring, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("READY");
}

void loop() {

  if (digitalRead(dialPin) == LOW && digitalRead(ring) != LOW) {
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


  if (digitalRead(handsetPin) == LOW && digit > 10 && digitalRead(ring) != LOW) {
    while (digitalRead(handsetPin) == LOW && digit > 10 && digitalRead(ring) != LOW) {}
    Serial.println("Calling: " + countri + number);
    callNumber(number);
    number = "";
    digit = 0;
    action = 1;
  }

  else if (digitalRead(handsetPin) == LOW && digit < 10) {
    digit = 0;
    number = "";
  }

  else if (digitalRead(ring) == LOW && digitalRead(handsetPin) == LOW && digit < 10) {
    while (digitalRead(ring) == LOW && digitalRead(handsetPin) == LOW && digit < 10) {}
    Serial.println("ANGKAT PANGGILAN");
    sim.print ("ATA\r\n");
    action = 1;
  }

  else if (action == 1 && digitalRead(handsetPin) == LOW) {
    //    while (action == 1 && digitalRead(handsetPin) == LOW) {}
    Serial.println("MATIKAN PANGGILAN");
    sim.print (F("ATH\r\n"));
    action = 0;
  }



  if (digitalRead(ring) == LOW) {
    if (millis() - prev2 > 500) {
      prev2 = millis();
      digitalWrite(ledPin, HIGH);
    }
    else if (millis() - prev2 > 250) {
      digitalWrite(ledPin, LOW);
    }
  }
  else {
    digitalWrite(ledPin, LOW);
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
