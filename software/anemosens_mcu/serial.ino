void init_serial()
{
  Serial.begin(115200);
}

void check_for_serial_receive_event()
{
  while (Serial.available())
  {
    char character = (char)Serial.read();
    if (character == 13) {
      check_serial_message(serialReceiveBuffer);
      serialReceiveBuffer = "";
    } else
    {
      serialReceiveBuffer += character;
    }
  }
}

void check_serial_message(String message)
{
  Serial.print(F("SERIAL: \""));
  Serial.print(String(message));
  Serial.println(F("\" received."));

  message.toLowerCase();
  if (message == "stopm")
  {
    
  } else if (message == "gupa")
  {

  } else if (message.indexOf('=') != -1)
  {
    String receivedValueString = message.substring(message.indexOf('=') + 1);
    String receivedCommand = message.substring(0, message.indexOf('='));
    float receivedValue = receivedValueString.toFloat();

    if (receivedCommand == "bmr")
    {
      //send_speed_brakeRight_motor(receivedValue);
    }
  } else
  {
    serial_command_unknown_response(message);
  }
  message = "";
}


void serial_command_unknown_response(String message)
{

}
