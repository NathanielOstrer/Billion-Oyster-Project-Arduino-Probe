#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

/*
 * Logger for the Billion Oyster Project, Michael Sloan Warren 2016
 * using Adafruit Datalogger shield based on https://github.com/adafruit/Light-and-Temp-logger/blob/master/lighttemplogger.pde
 * and with i2c components from https://www.whiteboxes.ch/tentacle/docs/
 * 
 * We'll be using the Tentacle Shield by Whitebox on i2c mode so that the pins we need for the datashield are free
 * Use the Whitebox utility program to ensure i2c address match up and to calibrate sensors.
 * 
 * This code is in the public domain and is not intended for commerical use.
 */


 // define the sensor addresses
const int phsensoraddress = 100;
const int dosensoraddress = 101;
const int consensoraddress = 102;
//need to define these using the utility, I use 100-103 (0x64-0x67) for CH0-CH3
//Using a quick i2c scanner isn't a bad idea: http://playground.arduino.cc/Main/I2cScanner


//define how often you want to take readings
const int LOG_INTERVAL = 3600000;  // how many milliseconds between taking data 
const int SYNC_INTERVAL = 3600000; // thow many milliseconds before writing data to disk
//since we won't be taking readings that often, it makes sense to make this the same as LOG_INTERVAL
//for faster readings, we can only sync periodically

//indicate if we want to use the serial monitor to mirror readings
const boolean ECHO_TO_SERIAL = true;

//datalogger variables
const int redLEDpin = 2; // digital pins that connect to the LEDs on the logger shield
const int greenLEDpin = 3; // digital pins that connect to the LEDs on the logger shield
const int chipSelect = 10; // for the data logging shield, we use digital pin 10 for the SD cs line
uint32_t syncTime = 0; // time of last sync()

//i2c variables 
char sensordata[30]; //A 30 byte character array to hold incoming data from the sensors
byte i2c_response_code = 0;              //used to hold the I2C response code.
byte in_char = 0;                    //used as a 1 byte buffer to store in bound bytes from an I2C stamp.
byte sensor_bytes_received = 0;   

File logfile; // the logging file
RTC_DS1307 RTC; // define the Real Time Clock object

void setup(void)
{
  Serial.begin(9600);
  Serial.println();
  
  // use debugging LEDs
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);

  // initialize the SD card
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");
  
  // create a new file on the SD card
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      logfile = SD.open(filename, FILE_WRITE); 
      break; 
    }
  }
  if (! logfile) {
    error("couldnt create file");
  }
  else {
  Serial.print("Logging to: ");
  Serial.println(filename);
  }
  
  // connect to RTC
  Wire.begin();  
  if (!RTC.begin()) {
    logfile.println("RTC failed");
if(ECHO_TO_SERIAL){
    Serial.println("RTC failed");
}
  }
  

  logfile.println("msec,timestamp,datetime,pH,DO");    
if(ECHO_TO_SERIAL){
  Serial.println("msec,timestamp,datetime,pH,DO");
}
 
 // // If you want to set the aref to something other than 5v
 // analogReference(EXTERNAL);
}

void loop(void)
{
  DateTime now;
  digitalWrite(greenLEDpin, HIGH); //green LED indicates that we're between readings
  // delay for the amount of time we want between readings
  delay(LOG_INTERVAL);
  //delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
  digitalWrite(greenLEDpin, LOW); 
  
  // log milliseconds since starting the session
  uint32_t m = millis();
  logfile.print(m);          
  logfile.print(", ");    
if(ECHO_TO_SERIAL){
  Serial.print(m);         
  Serial.print(", ");  
}

  // fetch the time from the shield
  now = RTC.now();
  // log time
  logfile.print(now.unixtime()); // seconds since 1/1/1970 - makes it easier to crunch later
  logfile.print(", ");
  logfile.print('"');
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print('"');
if(ECHO_TO_SERIAL){
  Serial.print(now.unixtime()); // seconds since 1/1/1970
  Serial.print(", ");
  Serial.print('"');
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print('"');
}

//now, take readings from the sensors
//check next tab for more details, just need to change i2c address each time 
 readsensor(phsensoraddress); //ph sensor
  logfile.print(", ");    
  logfile.print(sensordata);  
if(ECHO_TO_SERIAL){
  Serial.print(", ");   
  Serial.print(sensordata);
} //ECHO_TO_SERIAL

  readsensor(dosensoraddress); //do sensor
  logfile.print(", ");    
  logfile.print(sensordata);  
if(ECHO_TO_SERIAL){
  Serial.print(", ");   
  Serial.print(sensordata);
} //ECHO_TO_SERIAL

  readsensor(consensoraddress); //do sensor
  logfile.print(", ");    
  logfile.print(sensordata);  
if(ECHO_TO_SERIAL){
  Serial.print(", ");   
  Serial.print(sensordata);
} //ECHO_TO_SERIAL


//end the line for formatting
  logfile.println();
if(ECHO_TO_SERIAL){
  Serial.println();
}// ECHO_TO_SERIAL

//Time to take the log and store it on the SD card
 // if ((millis() - syncTime) < SYNC_INTERVAL) return;
//  syncTime = millis();
  
  digitalWrite(redLEDpin, HIGH);  // blink red LED to show we are syncing data to the card
  logfile.flush();
  digitalWrite(redLEDpin, LOW);
  
}
