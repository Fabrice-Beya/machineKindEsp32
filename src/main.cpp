#include <Arduino.h>

// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include "OBD.h"
#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "photon";
const char* password = "Mehuda@#001";

#define SD_CS 5

// logger states
#define STATE_SD_READY 0x1
#define STATE_OBD_READY 0x2
#define STATE_MEMS_READY 0x10

// maintain subsystem states

byte state = 0;

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

bool checkSDCard(){

    state &= ~STATE_SD_READY;

    if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return false;
    }
  
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      return false;
    }

    state |= STATE_SD_READY;
    return true;
}

void logServer(String Entry){
    StaticJsonBuffer<300> JSONbuffer;
    JsonObject& JSONencoder = JSONbuffer.createObject();

    JSONencoder["data"] = Entry;

    if ((WiFi.status() == WL_CONNECTED)) { 
 
    HTTPClient http;
 
    http.begin("https://mkiot.herokuapp.com/obd"); 
    http.addHeader("Content-Type", "application/json");  
    int httpCode = http.POST(JSONencoder);                                        
 
    if (httpCode > 0) { 
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 
  delay(10000);
}

void logData(const char* pid, String value){

    String Entry = String(millis()) + "," + String(pid) + "," + value + "\r\n";
    appendFile(SD, "/data.csv", Entry.c_str());
    logServer(Entry);
}

class CMyOBD : public COBD
{
  public:
    void dataIdleLoop()
    {
        if (!(state))
        {
            delay(10);
            return;
        }
    }
};

CMyOBD OBD;

void ProccessMEMS(){
    int16_t acc[3];
    int16_t gyro[3];
    int16_t temp;

    if (!OBD.memsRead(acc, gyro, 0, &temp))
        return;
    
    acc[0] /= ACC_DATA_RATIO;
    acc[1] /= ACC_DATA_RATIO;
    acc[2] /= ACC_DATA_RATIO;
    gyro[0] /= GYRO_DATA_RATIO;
    gyro[1] /= GYRO_DATA_RATIO;
    gyro[2] /= GYRO_DATA_RATIO;

    String ACC = String(acc[0]) + " " + String(acc[1]) + " " + String(acc[2]);
    String GYRO = String(gyro[0]) + " " + String(gyro[1]) + " " + String(gyro[2]);

    logData("PID_ACC: ", ACC);
    logData("PID_GYRO: ", GYRO);

}
void setup()
{
    Serial.begin(9600);
  
    if(checkSDCard()){
        // create a new csv file with a 0 starting point.
        writeFile(SD, "/data.csv", "elapsed time,parameter id,value\r\n");
    } else {
        Serial.println("SD card failure.");
    }

    // byte version = OBD.begin();

    // if (version && OBD.memsInit())
    //     state |= STATE_MEMS_READY;
    
    // // initialize the OBD until success
    // while (!OBD.init(OBD_PROTOCOL))
    //     ;
    // state |= STATE_OBD_READY;

    // char buf[64];
    // if (OBD.getVIN(buf, sizeof(buf)))
    // {
    //     //TODO:Log
    //     // get VIN as a string, buffer length should be >= OBD_RECV_BUF_SIZE
    //     logData("PID_VIN", buf);
    // }

     WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
}


void loop()
{
    // if (state & STATE_MEMS_READY)
    // {
    //     ProccessMEMS();
    // } else
    // {
    //     Serial.println("Mems Unavailable");
    // }

    logServer("Test");
    
    delay(100);
}

