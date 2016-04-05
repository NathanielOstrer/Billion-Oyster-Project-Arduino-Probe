void readsensor(int address){
i2c_response_code = 254;
sensor_bytes_received = 0;                          // reset data counter
memset(sensordata, 0, sizeof(sensordata));          // clear sensordata array;

  Wire.beginTransmission(address);                  //open line to i2c address
  Wire.write("r");                                  //transmit the read command
  Wire.endTransmission();                           //end the I2C data transmission.

 
  while (i2c_response_code == 254) {                 // in case the cammand takes longer to process, we keep looping here until we get a success or an error
      delay(1000);                                  // delay to allow the stamp time to read
      Wire.requestFrom(address, 48, 1);     //call the circuit and request 48 bytes (this is more then we need).
      i2c_response_code = Wire.read();              //the first byte is the response code, we read this separately.
      //Serial.println(i2c_response_code);          //debugging
      while (Wire.available()) {                    //as long are there bytes to receive...
        in_char = Wire.read();                      //receive a byte.

        if (in_char == 0) {                         //once we are sent a null command.
          Wire.endTransmission();                   //end the I2C data transmission.
          break;                                    //exit the while loop.
        }
        else {
          sensordata[sensor_bytes_received] = in_char;        //load this byte into our array.
          sensor_bytes_received++;                //keep track of how many bytes it is
      }
    }
  } 
  //Serial.println(sensordata);
}

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while(1);
}

