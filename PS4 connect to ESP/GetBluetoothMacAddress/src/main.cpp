#include <PS4Controller.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>

static char g_macLine[100];

void setup()
{
  Serial.begin(115200);
  delay(1500);
  Serial.println();
  Serial.println("Booting...");

  PS4.begin();
  const uint8_t *address = esp_bt_dev_get_address();

  if (address)
  {
    sprintf(g_macLine,
            "ESP32's Bluetooth MAC address is - %02x:%02x:%02x:%02x:%02x:%02x",
            address[0], address[1], address[2], address[3], address[4], address[5]);
  }
  else
  {
    strcpy(g_macLine, "Failed to read Bluetooth MAC address");
  }

  Serial.println(g_macLine);
}

void loop()
{
  // Print periodically so you still see it if you open Serial Monitor late.
  static unsigned long lastPrintMs = 0;
  if (millis() - lastPrintMs >= 5000)
  {
    lastPrintMs = millis();
    Serial.println(g_macLine);
  }
}