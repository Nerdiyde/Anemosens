

# Anemosens

Anemosens is an 3D printable anemometer that mostly consists of SLA printed parts.

  **Meanwhile I improved the initial design files. The latest design files are available here:**
     https://nerdiy.de/de_de/produkt/anemosens-v1-1-teile-fuer-3d-druckbares-anemometer-3d-druckbar-stl-dateien/

![](https://github.com/Nerdiyde/Anemosens/blob/main/pictures/Anemosens_overview.png)

  
## Detailed build guide

  

**You can find more information and detailed HowTo's in the following articles:**

- [Anemosens - Build a 3D printed wind sensor / anemometer](https://nerdiy.de/en/howto-anemosens-ein-3d-gedruckten-windsensor-anemometer-aufbauen/)

  

[![Anemosens_Video](https://img.youtube.com/vi/w3RU5c9Zf3Q/0.jpg)](https://www.youtube.com/watch?v=w3RU5c9Zf3Q)

  
  
  

## MCU PCB

  

![](https://github.com/Nerdiyde/Anemosens/blob/main/pictures/1.png)

  

Thanks to the MCU Board it will be possible to use Anemosens as a standalone Anemometer that can be used for long time observations in remote areas (therefore the microSD-card slot) or just as one part of your weatherstation (therefore the RS485 interface).

  

The RS485 interface is used to make the measured data available via Modbus. This way you can connect the Anemometer easily to your RaspberryPi, PC or many other platforms that support RS485/Modbus.

  

Additionally you can activate a serial-stream of the data (packed as handy JSON) via the USB connection.

  

Last but not least the integrated ESP32 also opens a BLE Server. Thanks to this you also have the possibility to pull the data wirelessly using your smartphone or computer.

  

Of course you can also write your own firmware and program it via the integrated USB-C port. The QWIIC-port makes it easy to connect additional sensors via I2C.

  

**You can find more information about the sensor pcb in the following article:**

  

- [PCB - Assembly of the Anemosens_MCU PCB](https://nerdiy.de/en/howto-pcb-anemosens_mcu-pcb-aufbauen/)

  

![](https://github.com/Nerdiyde/Anemosens/blob/main/pictures/2.png)

![](https://github.com/Nerdiyde/Anemosens/blob/main/pictures/RS485.png)

## Sensor PCB
  

![](https://github.com/Nerdiyde/Anemosens/blob/main/pictures/AS5048B.png)

  

The Anemosens sensor PCB is the basis for the sensor assembly of Anemosens, a 3D printed anemometer that I developed for the second version of WinDIY. (article here: **_[https://nerdiy.de/en/howto-pcb-das-anemosens-sensor-pcb-aufbauen//](https://nerdiy.de/en/howto-pcb-das-anemosens-sensor-pcb-aufbauen/)_**).

  

For this purpose, an AH49E Hall sensor and an AS5048B I2C 14 bit magnetic rotary encoder are installed on the PCB. The AS5048B is used to determine the wind direction. With the help of the AH49E Hall sensor you can measure the wind speed by measuring the time of one complete revolution of the wind blades.

  

To finish the assembly of the PCB you need to divide it into two parts and solder it together at a 90� angle. The finished PCB can then mounted into the 3D printed housing from Anemosens and fixed with three M2x6 screws.

  

**You can find more information about the sensor pcb in the following article:**

  

- [PCB - Assemble the Anemosens sensor PCB](https://nerdiy.de/en/howto-pcb-das-anemosens-sensor-pcb-aufbauen/)

  
  

### Trademarks

All third-party trademarks are the property of their respective owners. More infos here: https://nerdiy.de/en/warenzeichen/

  

### License

Unless otherwise stated, all works presented here and on Nerdiy.de that are not based on software/code are subject to the CC BY-NC-SA 4.0 license (attribution - non-commercial - dissemination under the same conditions 4.0 international).

You can find additional infos here: https://nerdiy.de/en/lizenz/
