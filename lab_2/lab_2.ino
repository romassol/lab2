#include "pitches.h"
#include "button.h"
#include "buzzer.h"

#define PIN_BUZZER 45
#define PIN_BUTTON 44

Button button(PIN_BUTTON);
Buzzer buzzer(PIN_BUZZER);

const int sensorPin = A0;
const float voltsPerMeasurement = 5.0/1024.0;
const float distanceForSignal = 100.0;

//float distances[] = {150.5, 178.3, 110.0, 150.4, 130.5, 178.3, 110.0, 515.4, 510.5, 178.3, 150.0, 178.4};
//int i = 0;
bool isSignalOn = false;

int notes[] = {NOTE_G3, NOTE_SILENCE, NOTE_G3, NOTE_SILENCE, NOTE_G3, NOTE_SILENCE, NOTE_G3, NOTE_SILENCE};
double durations[] = {1, 1, 1, 1, 1, 1, 1, 1};
int melodyLength = 8;

void setup() {
  Serial.begin(115200);
  buzzer.setMelody(notes, durations, melodyLength);
}

void loop() {
  float dist = readDist();
  Serial.println(dist);
//  float dist = distances[i];
//  i = (i+1) % 12;
  ifSightedObjectSoundOn(dist);
  if (button.wasPressed())
  {
    buzzer.turnSoundOff();
    isSignalOn = false;
  }
  buzzer.playSound();

}

bool ifSightedObjectSoundOn(float dist){
  if (!isSignalOn && dist <= distanceForSignal)
  {
    buzzer.turnSoundOn();
    isSignalOn = true;
  }
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
