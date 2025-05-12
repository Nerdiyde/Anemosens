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

void init_rtc()
{
  //rtc.attach(Wire);
  rtc_initialized = rtc.begin();

  if (!rtc_initialized)
  {
    Serial.println("Could not find a valid DS3232M RTC!");
  } else
  {
    Serial.println("DS3232M initialized.");
    check_rtc_for_lost_power();
    print_timestamp_ds3232m();
    print_human_readable_time_ds3232m();
    disable_rtc_oscillator();
  }
}

void disable_rtc_oscillator()
{
  //this disables the 32kHz oscillator. This makes sense in case you want to save power and do not use the oscillator.
  rtc.disable32K();
}

void enable_rtc_oscillator()
{
  //this enables the 32kHz oscillator.
  rtc.enable32K();
}

float return_rtc_temperature()
{
  return rtc.getTemperature();
}

uint32_t return_timestamp_ds3232m()
{
  DateTime now = rtc.now();
  return now.unixtime();
}

String return_human_readable_time_ds3232m()
{
  DateTime now = rtc.now();
  return now.timestamp();
}

void print_timestamp_ds3232m()
{
  Serial.print("RTC timestamp: ");
  Serial.println(return_timestamp_ds3232m());
}

void print_human_readable_time_ds3232m()
{
  DateTime now = rtc.now();
  Serial.print("RTC Time: ");
  Serial.println(return_human_readable_time_ds3232m());
}

void set_time_ds3232m(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, uint8_t year)
{
  //rtc.now(DateTime(year, month, day, hour, minute, second));
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
  Serial.print("Date set to: year: ");
  Serial.print(year);
  Serial.print(", month: ");
  Serial.print(month);
  Serial.print(", day: ");
  Serial.print(day);
  Serial.print(", hour: ");
  Serial.print(hour);
  Serial.print(", minute: ");
  Serial.print(minute);
  Serial.print(", second: ");
  Serial.print(second);
}

void check_rtc_for_lost_power()
{
  if (rtc.lostPower())
  {
    // Set clock to compile time value and print a warning
    DateTime compile_time = DateTime(F(__DATE__), F(__TIME__));
    Serial.print("RTC lost power, let's set the time to compile time: ");
    Serial.println(compile_time.timestamp());
    rtc.adjust(DateTime(compile_time));
  }
}
