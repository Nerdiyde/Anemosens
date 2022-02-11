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

void init_wind_direction_sensor()
{  
  dir_sensor.begin();
}


float read_wind_dir()
{
  return dir_sensor.angleR(U_DEG, true);

}

float read_wind_dir_moving_average()
{
  return dir_sensor.getMovingAvgExp(U_DEG);
}

void set_current_position_to_zero_direction()
{
  dir_sensor.setZeroReg();  
}

String convert_degree_to_compass(float angle)
{
  if (angle < 0.0) return "ERROR";
  else if (angle < 11.25) return "N";     // N
  else if (angle < 33.75) return "NNE";   // NNE
  else if (angle < 56.25) return "NE";    // NE
  else if (angle < 78.75) return "ENE";   // ENE
  else if (angle < 101.25) return "E";    // E
  else if (angle < 123.75) return "ESE";    // ESE
  else if (angle < 146.25) return "SE";   // SE
  else if (angle < 168.75) return "SSE";    // SSE
  else if (angle < 191.25) return "S";    // S
  else if (angle < 213.75) return "SSW";    // SSO
  else if (angle < 236.25) return "SW";   // SO
  else if (angle < 258.75) return "WSW";    // OSO
  else if (angle < 281.25) return "W";    // O
  else if (angle < 303.75) return "WNW";    // ONO
  else if (angle < 326.25) return "NW";   // NO
  else if (angle < 348.75) return "NNW";    // NNO
  else if (angle <= 360.0) return "N";    // N
  
  return "ERROR";
}

void update_wind_dir_moving_average()
{
  dir_sensor.updateMovingAvgExp();
}
