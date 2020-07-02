# ESP MQTT Button

## Description
An Arduino sketch to connect an ESP8266 (specifically tested with a NodeMCU board) to an LED and a button for publishing MQTT messages on a button press, with visual feedback. Ultra low poer usage (deep sleep when button pressed)

## License
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

## Requirements

 - Arduino IDE set up for ESP8266 (https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
 - PubSubClient installed into arduino IDE (https://www.baldengineer.com/mqtt-tutorial.html/installing-pubsubclient-for-arduino)
 - An ESP8266 on some development board with a button and LED wired as below:
![circuit diagram](circuit.png Circuit Diagram)
## Setup 
Just wire up the circuit, ideally with a nice button such as (https://www.amazon.co.uk/EG-Starts-Illuminated-Buttons-Microswitch/dp/B01LZMANZ7/) (though note the LED in this is 12V< so should be swapped with a 3V3 one or use an external supply). Upload the project, pop in a box with a power cable or battery pack and you are good to go.

