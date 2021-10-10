boolean regular_execution(uint32_t *lastExecution, uint32_t REPEATING_INTERVAL)
{
  //does help to regular execute a repeating task like checking for a display update
  if ((millis() - *lastExecution) >= REPEATING_INTERVAL)
  {
    *lastExecution = millis();
    return true;
  }
  return false;
}

const char * return_esp32_chip_model()
{
  return ESP.getChipModel();
}

uint8_t return_esp32_chip_revision()
{
  return ESP.getChipRevision();
}

uint8_t return_esp32_chip_core_number()
{
  return ESP.getChipCores();
}

uint32_t return_esp32_chip_id()
{
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) 
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  
  return chipId;
}

uint32_t return_cpu_frequency()
{
  return ESP.getCpuFreqMHz();
}

void print_esp32_chip_id()
{
   Serial.print("ESP32 Chip-ID: "); 
   Serial.println(return_esp32_chip_id());
}
