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

void init_wind_speed_sensor()
{
  // initialize all the readings to 0:
  for (int i = 0; i < WIND_SPEED_STEP_SIZE_MOVING_AVERAGE_READINGS_COUNT; i++) {
    step_size_readings[i] = 0;
  }
}

int16_t read_hall_sens_value()
{
  int32_t sensorValue = 0;
  for (uint8_t i = 0; i < HALL_SENSOR_SAMPLES_COUNT; i++) {
    sensorValue += analogRead(HALL_SENS_PIN);
  }
  return abs(sensorValue / HALL_SENSOR_SAMPLES_COUNT);
}

void update_wind_speed_moving_average()
{
  uint16_t hall_value = read_hall_sens_value();

  uint16_t middle = wind_speed_min_hall_sensor_value + ((wind_speed_max_hall_sensor_value - wind_speed_min_hall_sensor_value) / 2);
  int16_t speed_sensor_current_deviation = middle - hall_value; // this is the current deviation of the measured sensor value compared to its middle (not triggered) state

  if (wind_speed_min_hall_sensor_value >= hall_value)
  {
    wind_speed_min_hall_sensor_value = hall_value;  // adapt zero_value to lowest possible value
  }

  if (wind_speed_max_hall_sensor_value <= hall_value)
  {
    wind_speed_max_hall_sensor_value = hall_value;
  }

  // this is the difference of the current and previous deviation. If this is positive we are about to detect a rising edge of the signal
  int16_t _deviation_cycle_difference = speed_sensor_current_deviation - speed_sensor_current_deviation_previous;

  step_size_total = step_size_total - step_size_readings[step_size_read_index];
  step_size_readings[step_size_read_index] = _deviation_cycle_difference;
  step_size_total = step_size_total + step_size_readings[step_size_read_index];
  step_size_read_index = step_size_read_index + 1;

  if (step_size_read_index >= WIND_SPEED_STEP_SIZE_MOVING_AVERAGE_READINGS_COUNT)
  {
    step_size_read_index = 0;
  }

  step_size_average = step_size_total / WIND_SPEED_STEP_SIZE_MOVING_AVERAGE_READINGS_COUNT;

  if (step_size_average > SPEED_SENSOR_EDGE_DETECTION_LIMIT)
  {
    if (!signal_edge_detected_previous)
    {
      signal_edge_detected = true;
      signal_edge_detected_previous = true;
    } else
    {
      signal_edge_detected = false;
    }
  } else
  {
    signal_edge_detected_previous = false;
  }

  //in case a signal edge was detected we increase the tick counter for this measuring period
  if (signal_edge_detected)
  {
    wind_speed_tick_counter++;
  }
  speed_sensor_current_deviation_previous = speed_sensor_current_deviation;
}

boolean wind_speed_measurement_is_ready()
{
  return (wind_speed_min_hall_sensor_value != WIND_SPEED_MIN_HALL_SENSOR_VALUE_STANDARD) && (wind_speed_max_hall_sensor_value != WIND_SPEED_MAX_HALL_SENSOR_VALUE_STANDARD);
}

// converts wind speed to wind strength
String convert_meter_per_second_to_wind_strength(float wind_speed)
{
  if (wind_speed < 0.3)
    return "Calm";
  else if (wind_speed >= 0.3 && wind_speed < 1.5)
    return "Light Air";
  else if (wind_speed >= 1.5 && wind_speed < 3.3)
    return "Light Breeze";
  else if (wind_speed >= 3.3 && wind_speed < 5.4)
    return "Gentle Breeze";
  else if (wind_speed >= 5.4 && wind_speed < 7.9)
    return "Moderate Breeze";
  else if (wind_speed >= 7.9 && wind_speed < 10.7)
    return "Fresh Breeze";
  else if (wind_speed >= 10.7 && wind_speed < 13.8)
    return "Strong Breeze";
  else if (wind_speed >= 13.8 && wind_speed < 17.1)
    return "Moderate Gale";
  else if (wind_speed >= 17.1 && wind_speed < 20.7)
    return "Fresh Gale";
  else if (wind_speed >= 20.7 && wind_speed < 24.4)
    return "Strong Gale";
  else if (wind_speed >= 24.4 && wind_speed < 28.4)
    return "Whole Gale";
  else if (wind_speed >= 28.4 && wind_speed < 32.6)
    return "Violent Storm";
  else
    return  "Hurricane";
}

uint16_t convert_meter_per_second_to_bft(float wind_speed)
{
  if (!wind_speed_measurement_is_ready())
    return WIND_SPEED_MEASUREMENT_NOT_READY_VALUE;

  if (wind_speed < 0.3)
    return 0;
  else if (wind_speed >= 0.3 && wind_speed < 1.5)
    return 1;
  else if (wind_speed >= 1.5 && wind_speed < 3.3)
    return 2;
  else if (wind_speed >= 3.3 && wind_speed < 5.4)
    return 3;
  else if (wind_speed >= 5.4 && wind_speed < 7.9)
    return 4;
  else if (wind_speed >= 7.9 && wind_speed < 10.7)
    return 5;
  else if (wind_speed >= 10.7 && wind_speed < 13.8)
    return 6;
  else if (wind_speed >= 13.8 && wind_speed < 17.1)
    return 7;
  else if (wind_speed >= 17.1 && wind_speed < 20.7)
    return 8;
  else if (wind_speed >= 20.7 && wind_speed < 24.4)
    return 9;
  else if (wind_speed >= 24.4 && wind_speed < 28.4)
    return 10;
  else if (wind_speed >= 28.4 && wind_speed < 32.6)
    return 11;
  else
    return  12;
}

uint16_t convert_meter_per_second_to_km_per_hour(float wind_speed)
{
  return wind_speed * WIND_SPEED_MS_TO_KM_H_CONVERSION_FACTOR;
}
