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



//Serial
#define BAUDRATE 115200
#define SEND_DATA_VIA_SERIAL_ACTIVATED_STANDARD true

// Wind direction config
#define WIND_DIR_MOVING_AVERAGE_UPDATE_PERIOD 100  //in ms
#define WIND_DIR_EVALUATION_PERIOD 1000  //in ms

//#define WIND_SPEED_UPDATE_TIME 1000

#define HALL_SENS_PIN 33
#define WIND_DIR_SENS_ADDRESS 0x43 //I2C address of the wind direction sensor

// Wind speed config
#define WIND_SPEED_SENSOR_SHOVELS_DIAMETER 0.110 // diameter of the wind shovels in m
#define WIND_SPEED_EVALUATION_PERIOD 1000  //time period between the evaulation and calcluation of the windspeed within in the past period
#define HALL_SENSOR_SAMPLES_COUNT 50  //number of measurements that will be collected to calcualte the average hall sensor value at that time
#define WIND_SPEED_MIN_HALL_SENSOR_VALUE_STANDARD 1600  // this is the standard value for the lowest hall sensor value. It will be automatically adapted during the first rotations of the speed sensor shovels 
#define WIND_SPEED_MAX_HALL_SENSOR_VALUE_STANDARD 1800    // this is the standard value for the biggest hall sensor value. It will be automatically adapted during the first rotations of the speed sensor shovels 
#define SPEED_MAGNETS_COUNT 3 // this is the number of magnets that are attached to the speed sensor shovels
#define SPEED_SENSOR_EDGE_DETECTION_LIMIT 5 // This is the value the moving average of the sensor deviation needs to reach to detect the rising edge

#define WIND_SPEED_STEP_SIZE_MOVING_AVERAGE_READINGS_COUNT 10  // size of the moving average for the wind speed step_size

#define WIND_SPEED_MS_TO_KM_H_CONVERSION_FACTOR 3.6
#define WIND_SPEED_MEASUREMENT_NOT_READY_VALUE 999

//RS485

#define RS485_BAUDRATE 19200
#define RS485_SERIAL_ID 2
#define RS485_RTS_PIN 25 //TX/RX selection pin
#define RS485_TX_PIN_VALUE HIGH
#define RS485_RX_PIN_VALUE LOW
#define RS485_RX_PIN        13
#define RS485_TX_PIN        4
#define STANDARD_RS485_DEVICE_ADDRESS 1

//Read only registers: "Input register"
#define INPUT_REGISTER_ADDRESS_WIND_SPEED_M_S 0
#define INPUT_REGISTER_ADDRESS_WIND_SPEED_K_M_H 1
#define INPUT_REGISTER_ADDRESS_WIND_SPEED_BFT 2
#define INPUT_REGISTER_ADDRESS_WIND_DIRECTION 3
#define INPUT_REGISTER_ADDRESS_TEMPERATURE 4
#define INPUT_REGISTER_ADDRESS_HUMIDITY 5
#define INPUT_REGISTER_ADDRESS_PRESSURE 6

//Read only input: "Discrete Input"
#define DISCRETE_INPUT_REGISTER_ADDRESS_WIND_DATA_READY 0

// misc
#define SECONDS_PER_MINUTE 60.0
#define CONSTANT_PI 3.14159265359
#define DATA_REPORT_PERIOD 500  //in ms

//unit consts
#define U_RAW 1
#define U_TRN 2
#define U_DEG 3
#define U_RAD 4
#define U_GRAD 5
#define U_MOA 6
#define U_SOA 7
#define U_MILNATO 8
#define U_MILSE 9
#define U_MILRU 10


//SD card
#define SD_CS 33
#define SCK 18
#define MISO 19
#define MOSI 23

//RTC DS3232M
#define DS3232M_I2C_ADDRESS 0x68
#define READ_DS3232M_INTERVAL 1000

//BME280 climate sensor

#define BME280_ADDR 0x76
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_NOT_RUNNING_VALUE 9999.99
//BLE server
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

//UUIDs generated using https://www.uuidgenerator.net/
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_WIND_DIRECTION_UUID "9a768717-51f6-42ec-a8de-346860763756"
#define CHARACTERISTIC_WIND_SPEED_UUID "241d5b75-94b0-4b9e-b3f0-dcb0288e88f6"
#define CHARACTERISTIC_TEMPERATURE_BME_UUID "c9b7139e-6562-4e2e-9d43-a63d0ff98f29"
#define CHARACTERISTIC_SYSTEM_UUID "4739c08d-1e6e-4ec9-9974-55533118504e"
#define BLE_SERVER_NAME      "Anemosens_MCU"
