#include <Arduino.h>

// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <SPI.h>

#define SD_CS 5

uint8_t count;

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

void setup()
{
 Serial.begin(9600);
  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return;
  }
  
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      return;
  }

// create a file called data where all the logs will be dumped
   writeFile(SD, "/data.csv", "0");
}

void loop()
{

  appendFile(SD, "/data.csv", String(count).c_str() );
  delay(1000);
  count++;
}

 