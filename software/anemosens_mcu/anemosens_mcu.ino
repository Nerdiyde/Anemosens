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

/*ToDO:
   sd karte: Automatische archivierung der messwerte, ausgabe der messwerte über usb
   rtc: Einstellung der Zeit über USB, Autoamtische konfiguration der Uhrzeit zur Kompilierzeit
   bme280: Auslesen der Daten, Ausgabe über USB auf Anfrage, Optional kann die speicherung der Werte auf der SD Karte aktiviert werden
   RS485: Modbusfunktionalität: Daten werden über RS485 zur Verfügung gestellt.
   ausgabe der gesammelten daten über serial connection
   SPIFFS: Daten können im SPiffs gespeichert werden
   Ringspeicherfunktion (auf SD-Karte speichern, falls keine SD Karte vorhanden im internen/externen Flash speichern)
   esp web tools zum flashen auf nerdiy.de zur verfügung stellen
*/

//Wind direction sensing
#include <ams_as5048b.h>
#include <ArduinoJson.h>

//SD card
#include "FS.h"
#include "SD.h"
#include "SPI.h"

//RTC
#include "RTClib.h"

//BME280 climate sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//Modbus
#include <ModbusRTU.h>

//BLE server
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

//Config stuff
#include "_config.h"
#include "_defs.h"


//Control settings

/*
  Available data:
  - RTC Time
  - RTC Temperature in °C
  - Wind speed in m/s
  - Wind speed in bft
  - Wind speed as string
  - Wind direction
  - BME280 temperature in °C
  - BME280 humidity in rel%
  - BME280 air pressure in hPa
  - Uptime

  Possible settings:
  - SD card logging activated/deactivated
  - SD card ring storage activated/deactivated
  - Periodic JSON output via USB-serial-connection activated/deactivated
  - Serial report period in ms
  - BLE data server activated/deactivated
  - Set current wind direction to zero degrees
  - Set RS485 baudrate
  - Restart device
  - Set RTC time

*/

void setup() {
  init_serial();
  print_esp32_chip_id();

  init_wind_direction_sensor();
  init_wind_speed_sensor();
  init_rs485();
  init_sd_card();
  init_bme280();
  init_rtc();
  init_ble_server();
}

void loop()
{
  handle_rs485();
  //update_ble_server_values();

  /*
    Measure sensor data
  */

  // update wind direction sensor data
  if (regular_execution(&wind_dir_last_moving_average_update_timestamp, WIND_DIR_MOVING_AVERAGE_UPDATE_PERIOD))
  {
    update_wind_dir_moving_average();
  }

  // update wind speed sensor data
  update_wind_speed_moving_average();

  /*
    Evaluate measured sensor data
  */
  if (regular_execution(&wind_dir_last_evaluation_timestamp, WIND_DIR_EVALUATION_PERIOD))
  {
    wind_direction = read_wind_dir_moving_average();
  }

  if (regular_execution(&wind_speed_last_evaluation_timestamp, WIND_SPEED_EVALUATION_PERIOD))
  {
    float rotationsCounter = (float)wind_speed_tick_counter / (float)SPEED_MAGNETS_COUNT; // because there are three magnets the total amount of ticks need to be devided by the amount of magnets
    float rotations_per_second = rotationsCounter * (1000.0 / (float)WIND_SPEED_EVALUATION_PERIOD); // calculate the rotations per second
    float rotations_per_minute = rotations_per_second * SECONDS_PER_MINUTE;
    windspeed_in_meter_per_second = (rotations_per_minute * CONSTANT_PI * WIND_SPEED_SENSOR_SHOVELS_DIAMETER) / SECONDS_PER_MINUTE;
    wind_speed_tick_counter = 0;
  }

  /*
    Update and send measured sensor data
  */
  if (regular_execution(&last_data_report_timestamp, DATA_REPORT_PERIOD))
  {
    if (send_data_via_serial_acivtated)
    {

      StaticJsonDocument<200> output;

      if (rtc_initialized)
      {
        output["time"] = return_human_readable_time_ds3232m();
        output["temp_rtc"] = return_rtc_temperature();
      }

      output["dir_deg"] = wind_direction;
      output["dir_com"] = convert_degree_to_compass(wind_direction);

      if (wind_speed_measurement_is_ready())
      {
        output["m/s"] = windspeed_in_meter_per_second;
        output["bft"] = convert_meter_per_second_to_bft(windspeed_in_meter_per_second);
        output["string"] = convert_meter_per_second_to_wind_strength(windspeed_in_meter_per_second);
      }

      if (bme280_is_running())
      {
        output["temp_bme"] = serialized(String(return_temperature_bme280(), 2));
        output["humi"] = serialized(String(return_humidity_bme280(), 2));
        output["pressure"] = serialized(String(return_pressure_bme280(), 2));
        output["alt"] = serialized(String(return_altitude_bme280(), 2));
      }

      serializeJsonPretty(output, Serial);
    }

    update_rs485_register_values();
  }


}
