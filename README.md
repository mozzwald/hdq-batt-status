# HDQ Battery Status Example for Arduino
This example sketch prints battery status information from Texas Instruments HDQ capable fuel gauge ic's (used in iPhone batteries). It attempts to detect if an iPhone battery is connected by matching the device type, firmware and hardware versions. This is a work in progress. Tested with Arduino Nano v3.0 board and Arduino IDE v1.6.5.

## Usage:
First, install the [HDQLib](https://github.com/mozzwald/HDQLib) into your Arduino IDE Libraries directory.

Connect battery ground to Arduino ground. Connect the gas gauge pin from the battery to pin 7 of the Arduino. Use a 4.7k-10k pull-up resistor from pin 7 to Arduino 5V.

Now compile this sketch for your target board and upload it. It will print battery status information to the serial port every 5 seconds.

## Datasheets
See these datasheets for more information on the fuel gauge ic's:
 - [bq27541](http://www.ti.com/lit/gpn/bq27541)
 - [bq27545](http://www.ti.com/lit/gpn/bq27545-g1)
