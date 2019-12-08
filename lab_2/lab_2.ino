#include "button.h"
#include "buzzer.h"

#define PIN_BUZZER 6
#define PIN_BUTTON 5

Button button(PIN_BUTTON);
Buzzer buzzer(PIN_BUZZER);

const int sensorPin = A0;
const float voltsPerMeasurement = 5.0/1024.0;
const float distanceForSignal = 100.0;

int notes[] = {NOTE_G3, NOTE_SILENCE, NOTE_G3, NOTE_SILENCE, NOTE_G3, NOTE_SILENCE, NOTE_G3, NOTE_SILENCE};
double durations[] = {1, 1, 1, 1, 1, 1, 1, 1};
int melodyLength = 8;

void setup() {
//  Serial.begin(115200);
  buzzer.setMelody(notes, durations, melodyLength);
  buzzer.turnSoundOn();

}

void loop() {
  float dist = readDist();
  if (isSightedObject(dist))
  {
    buzzer.turnSoundOn();
  }
  if (button.wasPressed())
  {
    buzzer.turnSoundOff();
  }
  buzzer.playSound();
//  Serial.println(result);

}

bool isSightedObject(float dist){
  return dist <= distanceForSignal;
}

float readDist(){
  float volts = readAnalog() * voltsPerMeasurement;
  // return 65 * pow(volts, -1.10); // for big IR sensor (SHARP 2Y0A02)
  return pow(14.7737/volts, 1.2134); // for small IR sensor (SHARP 2Y0A21)
}

float readAnalog()
{
  int sum = 0;
  int maxV = -5000;
  int minV = 5000;
  int n = 15;
  for (int i = 0; i < n; i++)
  {
    int cur = analogRead(sensorPin);
    if (cur > maxV)
    {
      maxV = cur;
    }
    if (cur < minV)
    {
      minV = cur;
    }
    sum += cur;
    delay(6);
  }
  return (sum - maxV - minV) / (float)(n - 2);
}
