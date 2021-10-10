
/*ToDO:
   sd karte: Automatische archivierung der messwerte, ausgabe der messwerte über usb
   rtc: Einstellung der Zeit über USB, Autoamtische konfiguration der Uhrzeit zur Kompilierzeit
   bme280: Auslesen der Daten, Ausgabe über USB auf Anfrage, Optional kann die speicherung der Werte auf der SD Karte aktiviert werden
   RS485: Modbusfunktionalität: Daten werden über RS485 zur Verfügung gestellt.
   ausgabe der gesammelten daten über serial connection
   SPIFFS: Daten können im SPiffs gespeichert werden
   Ringspeicherfunktion
*/

#include <ams_as5048b.h>
#include <ArduinoJson.h>

//Serial
#define BAUDRATE 115200
String serialReceiveBuffer;

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
#include <ModbusRTU.h>

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

HardwareSerial Serial_RS485(RS485_SERIAL_ID);
ModbusRTU modbus_handler;

uint8_t rs485_device_address = STANDARD_RS485_DEVICE_ADDRESS;


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

AMS_AS5048B dir_sensor;

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

boolean signal_edge_detected = false;   // variable to save the state of an detected signal edge
boolean signal_edge_detected_previous = false;

float windspeed_in_meter_per_second = 0; // this holds the latest measured wind speed

// direction sensor variables
float wind_direction = 0; // this holds the latest measured wind direction

//SD card
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SD_CS 33
#define SCK 18
#define MISO 19
#define MOSI 23

//RTC DS3232M
#include "RTClib.h"

RTC_DS3231  rtc;

#define DS3232M_I2C_ADDRESS 0x68
#define READ_DS3232M_INTERVAL 1000

boolean rtc_initialized = false;

//BME280 climate sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME280_ADDR 0x76
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_NOT_RUNNING_VALUE 9999.99

Adafruit_BME280 bme_clima; // I2C

boolean bme280_initialized = false;

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


boolean ble_client_connected = false;
boolean ble_client_connected_old = false;

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

  if (regular_execution(&last_data_report_timestamp, DATA_REPORT_PERIOD))
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

    update_rs485_register_values();
  }


}
