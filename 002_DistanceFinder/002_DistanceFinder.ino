#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>

// select the pins used on the LCD panel
LiquidCrystal s_lcd(8, 9, 4, 5, 6, 7);
static const int s_iPinTrigger = 13;
static const int s_iPinEcho = 12;

void setup() {
  Serial.begin(9600);
  s_lcd.begin(16, 2);
  s_lcd.clear();
  s_lcd.setCursor(0, 0);
  s_lcd.print("Distance:");

  // Set the input / output pins.
  pinMode(s_iPinTrigger, OUTPUT);
  pinMode(s_iPinEcho, INPUT);

  // Start in the OFF state.
  digitalWrite(s_iPinTrigger, LOW);
}

/* Converts duration (µs) to distance (cm) */
int durationToDistance(int duration)
{
  // The speed of sound is:
  // 343 m/s.
  // 34300 cm/s
  // 0.0343 cm/µs
  float distance = static_cast<float>(duration) * 0.0343 / 2.0;
  return static_cast<int>(distance);
}

void loop() {
  // Ensure that we start on LOW.
  digitalWrite(s_iPinTrigger, LOW);
  delayMicroseconds(10);

  // Send a HIGH pulse.
  digitalWrite(s_iPinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(s_iPinTrigger, LOW);
  delayMicroseconds(2);

  // Wait for the HIGH pulse to be received.
  int duration = pulseIn(s_iPinEcho, HIGH, 1000000);
  int distance = durationToDistance(duration);
  if ((distance >= 2) && (distance <= 400))
  {
    s_lcd.setCursor(0, 1);
    s_lcd.print(distance);
    if (distance < 10)
      s_lcd.print("   ");
    else if (distance < 100)
      s_lcd.print("  ");
    else
      s_lcd.print(" ");
    s_lcd.print("cm");

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print("cm");
    Serial.println();
  }
}
