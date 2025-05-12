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

void init_bme280()
{
  bme280_initialized = bme_clima.begin(BME280_ADDR);
  if (!bme280_initialized)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme_clima.sensorID(), 16);
    Serial.print("ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("ID of 0x60 represents a BME 280.\n");
    Serial.print("ID of 0x61 represents a BME 680.\n");
  } else
  {
    Serial.println("BME280 initialized.");
    print_temperature_bme280();
    print_humidity_bme280();
    print_pressure_bme280();
    print_altitude_bme280();
  }
}

boolean bme280_is_running()
{
  return bme280_initialized;
}

float return_temperature_bme280()
{
  float temperature = bme280_is_running() ? bme_clima.readTemperature() : BME280_NOT_RUNNING_VALUE;
  return temperature;
}

float return_humidity_bme280()
{
  float humidity = bme280_is_running() ? bme_clima.readHumidity() : BME280_NOT_RUNNING_VALUE;
  return humidity;
}

float return_pressure_bme280()
{
  float pressure = bme280_is_running() ? bme_clima.readPressure() / 100.0F : BME280_NOT_RUNNING_VALUE;
  return pressure;
}

float return_altitude_bme280()
{
  float altitude =bme280_is_running() ?  bme_clima.readAltitude(SEALEVELPRESSURE_HPA) : BME280_NOT_RUNNING_VALUE;
  return altitude;
}


void print_temperature_bme280()
{  
  Serial.print("Temperature: ");
  Serial.print(return_temperature_bme280());
  Serial.println(" *C");
}

void print_humidity_bme280()
{  
  Serial.print("Humidity: ");
  Serial.print(return_humidity_bme280());
  Serial.println("%");
}

void print_pressure_bme280()
{  
  Serial.print("Pressure: ");
  Serial.print(return_pressure_bme280());
  Serial.println(" hPa");
}

void print_altitude_bme280()
{  
  Serial.print("Approx. Altitude = ");
  Serial.print(return_altitude_bme280());
  Serial.println(" m");
}
