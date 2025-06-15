# Home Automation with Fire and Gas Security

This project implements a smart home automation and safety system using an ESP8266 module.
It features:
- Relay control via Blynk app and physical switches
- LCD status display
- Flame and gas leakage detection with alerts
- WiFi signal strength visualization on a WS2812 LED strip

## Features

- **Relays**: Control two devices manually or via Blynk (V1, V2).
- **Gas Sensor (MQ series)**: Triggers buzzer, exhaust fan, and alert on leakage (V3).
- **Flame Sensor**: Triggers alarm and shuts down relays during fire (V4).
- **OLED/ LCD**: Shows current status.
- **WS2812 LED Strip**: Displays WiFi signal strength or alert animations.

## Hardware Used

- ESP8266 (e.g., NodeMCU)
- 2-channel relay module
- Flame sensor
- Gas sensor (MQ-2/MQ-135)
- WS2812B LED strip (19 LEDs)
- LCD I2C 16x2 display
- Buzzer
- Exhaust fan

## Libraries Required

- `Blynk`
- `Adafruit NeoPixel`
- `LiquidCrystal_I2C`
- `ESP8266WiFi`
- `Wire`

## Setup

1. Replace your `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, and `AUTH` in the code.
2. Enter your WiFi SSID and password.
3. Flash the code to ESP8266 using Arduino IDE.
4. Connect the components according to the pin configuration in the code.
5. Open the Blynk app and configure virtual pins V1 to V4 as buttons/events.

## Notes

- Make sure the LED strip pin supports PWM (PIN 9 is not supported on ESP8266; use D2/GPIO4 or similar).
- Double-check power requirements for relays and sensors.

## License

MIT License
