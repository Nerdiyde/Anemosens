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

class BLE_server_callbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      ble_client_connected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      ble_client_connected = false;
    }
};

class ble_char_wind_direction_callbacks: public BLECharacteristicCallbacks
{
    void onRead(BLECharacteristic *pCharacteristic)
    {
      std::string float_as_string((char*)&wind_direction, 2); //convert float to string
      pCharacteristic->setValue(float_as_string);
    }
    /* void onWrite(BLECharacteristic *pCharacteristic)
      {
       std::string rxValue = pCharacteristic->getValue();

       if (rxValue.length() > 0) {
         Serial.println("*********");
         Serial.print("Received Value: ");
         for (int i = 0; i < rxValue.length(); i++)
           Serial.print(rxValue[i]);

         Serial.println();
         Serial.println("*********");
       }
      }*/
};

class ble_char_wind_speed_callbacks: public BLECharacteristicCallbacks
{
    void onRead(BLECharacteristic *pCharacteristic)
    {
      //String value = String(windspeed_in_meter_per_second, 2);
      std::string float_as_string((char*)&windspeed_in_meter_per_second, 2); //convert float to string
      pCharacteristic->setValue(float_as_string);
    }
};
void init_ble_server()
{
  Serial.println("Starting BLE server...");

  // Create the BLE Device
  BLEDevice::init(BLE_SERVER_NAME);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BLE_server_callbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic for wind direction
  BLECharacteristic *pCharacteristic_wind_direction = pService->createCharacteristic(
        CHARACTERISTIC_WIND_DIRECTION_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY
      );

  // Create a BLE Characteristic for wind speed
  BLECharacteristic *pCharacteristic_wind_speed = pService->createCharacteristic(
        CHARACTERISTIC_WIND_SPEED_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY
      );
  //pCharacteristic_wind_direction->setValue("Test");

  pCharacteristic_wind_direction->setCallbacks(new ble_char_wind_direction_callbacks());
  pCharacteristic_wind_speed->setCallbacks(new ble_char_wind_speed_callbacks());

  pService->start();



  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);

  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();
  Serial.println("...done.");

}


void update_ble_server_values()
{
  if (ble_client_connected)
  {
    //pTxCharacteristic->setValue(&txValue, 1);
    //pTxCharacteristic->notify();
    //txValue++;
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

  // disconnecting
  if (!ble_client_connected && ble_client_connected_old)
  {
    // do stuff here on disconnect
    //delay(500); // give the bluetooth stack the chance to get things ready
    //pServer->startAdvertising(); // restart advertising
    //Serial.println("start advertising");
    ble_client_connected_old = ble_client_connected;
  }
  // connecting
  if (ble_client_connected && !ble_client_connected_old) {
    // do stuff here on connecting

    Serial.println("BLE Client connected");
    ble_client_connected_old = ble_client_connected;
  }
}
