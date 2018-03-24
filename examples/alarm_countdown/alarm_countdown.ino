/*
  Demonstrates the use of the alarm and countdown features.

  The type tmElements_t and functions like breakTime() are from
  the Time library.
*/

#include <PCF85063A.h>

PCF85063A rtc; /* Chip enable on pin 9 */

void setup() {
  tmElements_t tm;
  Serial.begin(57600);

  Serial.println("Resetting RTC");
  rtc.reset();

  Serial.println("Setting time to Tue, 08 Nov 2016 23:49:50");
  breakTime(1478648990, tm);
  rtc.time_set(&tm);

  Serial.println("Setting alarm at 23:50:00");
  breakTime(1478648990 + 10, tm);
  rtc.alarm_set(tm.Second, tm.Minute, tm.Hour, tm.Day, tm.Wday);

  Serial.println("Enabling repeating countdown timer with 5 second interval");
  rtc.countdown_set(true, PCF85063A::CNTDOWN_CLOCK_1HZ, 5, false, false);
}

/* Print human readable time */
void print_time(tmElements_t t) {
  Serial.print(String("Year ") + (1970 + t.Year));
  Serial.print(String(" Month ") + t.Month);
  Serial.print(String(" Day ") + t.Day);
  Serial.print(String(" Wday ") + t.Wday);
  Serial.print(String(" Hour ") + t.Hour);
  Serial.print(String(" Minute ") + t.Minute);
  Serial.print(String(" Second ") + t.Second);
  Serial.print("\n");
}

void loop() {
  tmElements_t t;

  /* Get and print current time */
  rtc.time_get(&t);
  print_time(t);

  /* Read control registers */
  PCF85063A_Regs regs;
  rtc.ctrl_get(&regs);

  /* Set up new registers by defaulting alarm bits high. This avoids clobbering
     alarm bits that triggered during our processing. */
  PCF85063A_Regs new_regs = regs;
  PCF85063A_REG_SET(new_regs, PCF85063A_REG_AF);
  PCF85063A_REG_SET(new_regs, PCF85063A_REG_TF);

  /* Check for alarms */
  if (PCF85063A_REG_GET(regs, PCF85063A_REG_AF))
  {
    Serial.println("Alarm triggered!");
    PCF85063A_REG_CLEAR(new_regs, PCF85063A_REG_AF);
  }

  if (PCF85063A_REG_GET(regs, PCF85063A_REG_TF))
  {
    Serial.println("Countdown timer triggered!");
    PCF85063A_REG_CLEAR(new_regs, PCF85063A_REG_TF);
  }

  rtc.ctrl_set(new_regs, false);
  delay(1000);
}
