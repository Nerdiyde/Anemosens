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

boolean regular_execution(uint32_t *lastExecution, uint32_t REPEATING_INTERVAL)
{
  //does help to regular execute a repeating task like checking for a display update
  if ((millis() - *lastExecution) >= REPEATING_INTERVAL)
  {
    *lastExecution = millis();
    return true;
  }
  return false;
}

const char * return_esp32_chip_model()
{
  return ESP.getChipModel();
}

uint8_t return_esp32_chip_revision()
{
  return ESP.getChipRevision();
}

uint8_t return_esp32_chip_core_number()
{
  return ESP.getChipCores();
}

uint32_t return_esp32_chip_id()
{
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) 
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  
  return chipId;
}

uint32_t return_cpu_frequency()
{
  return ESP.getCpuFreqMHz();
}

void print_esp32_chip_id()
{
   Serial.print("ESP32 Chip-ID: "); 
   Serial.println(return_esp32_chip_id());
}
