/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <BranchController.h>
#include <Heartbeat.h>


void setup()
{
  Heartbeat::setup();
}

void loop()
{
  Heartbeat::loop();
}
