/*
  Demonstrates how to set and get time from the RTC.

  The type tmElements_t and functions like breakTime() are from
  the Time library.
*/

#include <PCF85063A.h>

PCF85063A rtc;

void setup() {
  tmElements_t tm;
  Serial.begin(57600);

  Serial.println(String("Resetting RTC, status: ") + rtc.reset());
  Serial.println("Setting time to Tue, 08 Nov 2016 23:49:50");

  /* Convert Unix timestamp to tmElements_t */
  breakTime(1478648990, tm);
  rtc.time_set(&tm);
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

  /* Print Unix timestamp */
  Serial.print("Unix time: ");
  Serial.println(makeTime(t));

  delay(1000);
}
