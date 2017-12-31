# ESP8266-GHSW8181
Control an IOGEAR GHSW8148/GHSW8181 over wifi with an ESP8266

## Goals

Expose basic RS-232 control commands for the above HDMI switches over HTTP, with the intent to finagle this into a Homekit device.
The latter will likely be done begrudingly via Homebridge and 8 switch-type accessories (probably lumped into a Platform).

## Configuration

* Copy `wifi.example.h` to `wifi.h`
* Fill in the values to connect to your wifi network

## Usage

### `GET /`

Parameters: none

Response: `text/plain`

Example:

`curl ghsw8181.local`

```
read Command OK
Input: port4
Output: ON
Mode: Next
Goto: OFF
F/W: V1.0.067
```

Emits the raw HDMI switch output from a `read` command.

### `POST /select`

Parameters:

* `port`: int (1-8)

Response: `text/plain`

Example:

`curl -d port=2 ghsw8181.local/select`

```
sw i02 Command OK
```

Emits the raw switch output from a `sw i0X` command.

## Wiring

This was built and tested using an ESP-12e dev board and a [MAX3232 UART-to-RS323 adapter](https://www.amazon.com/gp/product/B00OPU2QJ4).
This adapter (or an equivalent) is NOT OPTIONAL, much to my dismay.
Connect as follows:

| Adapter | ESP8266 |
|---------|---------|
| GND     | G       |
| OUT     | RX      |
| IN      | TX      |
| VCC     | 3v      |

Note that OUT refers to the arrow pointing *away* from the chip, and IN refers to the arrow pointing *towards* it.
The four bottom-most pins on the right side (top view) on the dev board should be the ones you want.

When using a USB-to-serial adapter to debug this, I needed to use a null modem cable (technically, some very shaky jumper wires).
This was not required when actually plugged into the HDMI switch.

## Notes
In the Arduino IDE, I have selected a NodeMCU ESP-12e board, 115200 baud connection.
It would always error out during uploading unless the wiring was disconnected.
No idea if that's normal or something specific to serial output, but disconnecting the wires fixed it.
Pressing the flash button before/during programming was not necessary with the wiring disconnected, and only worked about 2% of the time with it connected.
Follow [this guide](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide) to set up Arduino IDE to speak to the ESP8266 boards (but select NodeMCU instead of the Adafruit board).


Upon setup, it will attempt to broadcast itself at `ghsw8181.local`.
Depending on your network and DNS configuration, this may not resolve.
To find the device's IP, the easiest approach is to probably just check your router's DHCP leases.
It should print to the serial console as well, but that will be inaccessible when connected to the switch (so make note of it when programming via the Arduino IDE).

There is a lot of room for improvement, which is welcome.

There is ABSOLUTELY NO SECURITY here - any plaintext HTTP client that can talk to this can change your inputs or read the current settings.
Only basic validation to ensure you can't run arbitrary serial commands exists.
So generally speaking, make sure this isn't exposed to the internet directly unless you want strangers changing your HDMI switch's input.

## License
MIT
