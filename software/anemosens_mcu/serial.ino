/*                                 _   _                 _  _               _
                                 | \ | |               | |(_)             | |
  __      ____      ____      __ |  \| |  ___  _ __  __| | _  _   _     __| |  ___
  \ \ /\ / /\ \ /\ / /\ \ /\ / / | . ` | / _ \| '__|/ _` || || | | |   / _` | / _ \
   \ V  V /  \ V  V /  \ V  V /_ | |\  ||  __/| |  | (_| || || |_| | _| (_| ||  __/
    \_/\_/    \_/\_/    \_/\_/(_)|_| \_| \___||_|   \__,_||_| \__, |(_)\__,_| \___|
                                                               __/ |
                                                              |___/
     Anemosens_MCU firmware by Fabian Steppat
     Infos on www.nerdiy.de/anemosens

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

void init_serial()
{
  Serial.begin(115200);
}

void check_for_serial_receive_event()
{
  while (Serial.available())
  {
    char character = (char)Serial.read();
    if (character == 13) {
      check_serial_message(serialReceiveBuffer);
      serialReceiveBuffer = "";
    } else
    {
      serialReceiveBuffer += character;
    }
  }
}

void check_serial_message(String message)
{
  Serial.print(F("SERIAL: \""));
  Serial.print(String(message));
  Serial.println(F("\" received."));

  message.toLowerCase();
  if (message == "stopm")
  {
    
  } else if (message == "gupa")
  {

  } else if (message.indexOf('=') != -1)
  {
    String receivedValueString = message.substring(message.indexOf('=') + 1);
    String receivedCommand = message.substring(0, message.indexOf('='));
    float receivedValue = receivedValueString.toFloat();

    if (receivedCommand == "bmr")
    {
      //send_speed_brakeRight_motor(receivedValue);
    }
  } else
  {
    serial_command_unknown_response(message);
  }
  message = "";
}


void serial_command_unknown_response(String message)
{

}
