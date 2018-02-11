#include <Servo.h>

#define delta 50

const int centro = 94;
const int maxAng = -35;
const int minAng = 35;

unsigned long last = 0;

Servo servo;

void setup() {
  Serial.begin(57600);
  servo.attach(6);
  servo.write(centro);
  delay(100);
  DDRB = B00100000;
}
int angulo, ent_analoga, dir;

void loop() {
  if (millis() - last >= delta) {
    ent_analoga = analogRead(0);
    angulo = map(ent_analoga, 0, 1023, centro + minAng, centro + maxAng);
    dir = angulo - centro;
    servo.write(angulo);
    Serial.println(dir);
    Serial.flush();
    if (dir == 0 || dir == maxAng || dir == minAng) {
      digitalWrite(13, HIGH);
    }
    else {
      digitalWrite(13, LOW);
    }
  }
}
