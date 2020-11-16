#include "config.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

AdafruitIO_Feed *humidity = io.feed(ROOM ".humidity");
AdafruitIO_Feed *temperature = io.feed(ROOM ".temperature");

#define DHTTYPE DHT22
DHT_Unified dht(DHTPIN, DHTTYPE);

void setup()
{

  // start the serial connection
  Serial.begin(115200);
  dht.begin();

  // wait for serial monitor to open
  while (!Serial)
    ;

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void loop()
{
  sensors_event_t event;

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  Serial.println("Sensing for room " ROOM);

  dht.temperature()
      .getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
    delay(1000);
    return;
  }
  else
  {
    Serial.print("sending temperature -> ");
    Serial.println(event.temperature);
    temperature->save(event.temperature);
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading humidity!"));
    delay(1000);
    return;
  }
  else
  {
    Serial.print("sending humidity -> ");
    Serial.println(event.relative_humidity);
    humidity->save(event.relative_humidity);
  }

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(60000);
}
