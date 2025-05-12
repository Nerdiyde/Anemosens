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

// == Variable & Object Defintions ===========================================================================================================================


//Serial
String serialReceiveBuffer="";
bool send_data_via_serial_acivtated=SEND_DATA_VIA_SERIAL_ACTIVATED_STANDARD;

//RS485
HardwareSerial Serial_RS485(RS485_SERIAL_ID);
ModbusRTU modbus_handler;

uint8_t rs485_device_address=STANDARD_RS485_DEVICE_ADDRESS;

// time variables for periodicall execution of tasks
uint32_t wind_dir_last_moving_average_update_timestamp = 0;
uint32_t wind_dir_last_evaluation_timestamp = 0;
uint32_t wind_speed_last_evaluation_timestamp = 0;
uint32_t last_data_report_timestamp = 0;

// speed sensor variables
uint16_t wind_speed_min_hall_sensor_value = WIND_SPEED_MIN_HALL_SENSOR_VALUE_STANDARD;  // this is the minimum measured hall sensor value which is updated automatically to the lowest measured hall sensor value
uint16_t wind_speed_max_hall_sensor_value = WIND_SPEED_MAX_HALL_SENSOR_VALUE_STANDARD;  // this is the maximum measured hall sensor value which is updated automatically to the biggest measured hall sensor value

uint16_t speed_sensor_current_deviation_previous = 0; // this is the current deviation of the measured sensor value compared to its middle (not triggered) state of the previous measuring cycle

uint16_t wind_speed_tick_counter = 0; // counter variable to save the ticks of the speed sensor durin the measurement period

int32_t step_size_readings[WIND_SPEED_STEP_SIZE_MOVING_AVERAGE_READINGS_COUNT];      // container for the moving average of the past step_size values
uint8_t step_size_read_index = 0;                 // index variable for the moving average of the past step_size values
int32_t step_size_total = 0;                  // total of the moving average of the past step_size values
int32_t step_size_average = 0;                // average calculated from the moving average of the past step_size values

bool signal_edge_detected = false;   // variable to save the state of an detected signal edge
bool signal_edge_detected_previous = false;

float windspeed_in_meter_per_second = 0; // this holds the latest measured wind speed

// direction sensor variables
AMS_AS5048B dir_sensor;
float wind_direction = 0; // this holds the latest measured wind direction

//RTC DS3232M
RTC_DS3231  rtc;

bool rtc_initialized = false;

//BME280 climate sensor
Adafruit_BME280 bme_clima; // I2C

bool bme280_initialized = false;

//BLE server
bool ble_client_connected = false;
bool ble_client_connected_old = false;
