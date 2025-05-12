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

void init_rs485()
{
  Serial_RS485.begin(RS485_BAUDRATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
  pinMode(RS485_RTS_PIN, OUTPUT);
  modbus_handler.begin(&Serial_RS485, RS485_RTS_PIN, true);  //or use RX/TX direction control pin (if required)
  modbus_handler.setBaudrate(RS485_BAUDRATE);
  modbus_handler.server(rs485_device_address);

  //create registers
  modbus_handler.addIreg(INPUT_REGISTER_ADDRESS_WIND_SPEED_M_S);
  modbus_handler.addIreg(INPUT_REGISTER_ADDRESS_WIND_SPEED_K_M_H);
  modbus_handler.addIreg(INPUT_REGISTER_ADDRESS_WIND_SPEED_BFT);
  modbus_handler.addIreg(INPUT_REGISTER_ADDRESS_WIND_DIRECTION);
  modbus_handler.addIreg(INPUT_REGISTER_ADDRESS_TEMPERATURE);
  modbus_handler.addIreg(INPUT_REGISTER_ADDRESS_HUMIDITY);
  modbus_handler.addIreg(INPUT_REGISTER_ADDRESS_PRESSURE);
  
  modbus_handler.addIsts(DISCRETE_INPUT_REGISTER_ADDRESS_WIND_DATA_READY);
  
  //modbus_handler.addHreg(REGISTER_ADDRESS_PRESSURE);
  //modbus_handler.addCoil(DISCRETE_INPUT_REGISTER_ADDRESS_WIND_DATA_READY);
  

}

void handle_rs485()
{  
  modbus_handler.task();
}

void update_rs485_register_values()
{    
  modbus_handler.Ireg(INPUT_REGISTER_ADDRESS_WIND_SPEED_M_S, !wind_speed_measurement_is_ready() ? WIND_SPEED_MEASUREMENT_NOT_READY_VALUE :windspeed_in_meter_per_second);
  modbus_handler.Ireg(INPUT_REGISTER_ADDRESS_WIND_SPEED_K_M_H, !wind_speed_measurement_is_ready() ? WIND_SPEED_MEASUREMENT_NOT_READY_VALUE :convert_meter_per_second_to_km_per_hour(windspeed_in_meter_per_second));
  modbus_handler.Ireg(INPUT_REGISTER_ADDRESS_WIND_SPEED_BFT, !wind_speed_measurement_is_ready() ? WIND_SPEED_MEASUREMENT_NOT_READY_VALUE :convert_meter_per_second_to_bft(windspeed_in_meter_per_second));
  modbus_handler.Ireg(INPUT_REGISTER_ADDRESS_WIND_DIRECTION, !wind_speed_measurement_is_ready() ? WIND_SPEED_MEASUREMENT_NOT_READY_VALUE :wind_direction);
  modbus_handler.Ireg(INPUT_REGISTER_ADDRESS_TEMPERATURE, (return_temperature_bme280()*100));
  modbus_handler.Ireg(INPUT_REGISTER_ADDRESS_HUMIDITY, (return_humidity_bme280()*100));
  modbus_handler.Ireg(INPUT_REGISTER_ADDRESS_PRESSURE, return_pressure_bme280());
  modbus_handler.Ists(DISCRETE_INPUT_REGISTER_ADDRESS_WIND_DATA_READY, wind_speed_measurement_is_ready());  
  
  //modbus_handler.Hreg(REGISTER_ADDRESS_WIND_SPEED_M_S, !wind_speed_measurement_is_ready() ? WIND_SPEED_MEASUREMENT_NOT_READY_VALUE :windspeed_in_meter_per_second);
  //modbus_handler.Coil(DISCRETE_INPUT_REGISTER_ADDRESS_WIND_DATA_READY, wind_speed_measurement_is_ready());
}
