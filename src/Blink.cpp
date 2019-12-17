/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <BranchController.h>
#include <Heartbeat.h>
#include <Util.h>

using namespace Util;

void setup()
{
  Heartbeat::setup();
  Util::setup();
}

void loop()
{
  Heartbeat::loop();
}
