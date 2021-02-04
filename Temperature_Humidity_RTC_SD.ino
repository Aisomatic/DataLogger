#include <SPI.h>
#include <SD.h>
#include <virtuabotixRTC.h>
#include "DHT.h"

#define DHT1 2
#define DHT2 4 
#define DHTTYPE DHT22

File myFile;
unsigned int sleepTime = 10000;
unsigned long tic, toc, dt;
String fileName = "Data.txt";

DHT dht1(DHT1, DHTTYPE);
DHT dht2(DHT2, DHTTYPE);
virtuabotixRTC myRTC(6, 7, 8); //CLK DAT RST

void initSerial(){
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }
}

void initSD(){
  Serial.println("Initializing SD card ...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

void displayHeaderSerial(){
  Serial.println("Date\t Time\t Humidity[%]\t Temperature[°C]\t Humidity[%]\t Temperature[°C]\t dt");
}

void displayHeaderSD(){
  myFile = SD.open(fileName, FILE_WRITE);
  
  if (myFile) {
    // if the file opened okay, write to it:
    myFile.println("Date\t Time\t Humidity[%]\t Temperature[°C]\t Humidity[%]\t Temperature[°C]\t dt");
    myFile.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(fileName); 
  }
}

void writeDataSerial(float h1, float t1, float h2, float t2, unsigned long dt){
  Serial.print(h1);
  Serial.print("\t ");
  Serial.print(t1);
  Serial.print("\t ");
  Serial.print(h2);
  Serial.print("\t ");
  Serial.print(t2);
  Serial.print("\t ");
  Serial.println(dt);    
}

void writeDataSD(float h1, float t1, float h2, float t2, unsigned long dt){
  myFile = SD.open(fileName, FILE_WRITE);
  
  if (myFile) {
    // if the file opened okay, write to it:
    myFile.print(h1);
    myFile.print("\t ");
    myFile.print(t1);
    myFile.print("\t ");
    myFile.print(h2);
    myFile.print("\t ");
    myFile.print(t2);
    myFile.print("\t ");
    myFile.println(dt);
    myFile.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(fileName); 
  }  
}

void displayDateTimeSerial(){
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/"); 
  Serial.print(myRTC.year); 
  Serial.print("\t ");
  Serial.print(myRTC.hours); 
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.print(myRTC.seconds);
  Serial.print("\t ");     
}

void displayDateTimeSD(){
  myFile = SD.open(fileName, FILE_WRITE);
  if(myFile){
    myFile.print(myRTC.dayofmonth);
    myFile.print("/");
    myFile.print(myRTC.month);
    myFile.print("/"); 
    myFile.print(myRTC.year); 
    myFile.print("\t ");
    myFile.print(myRTC.hours); 
    myFile.print(":");
    myFile.print(myRTC.minutes);
    myFile.print(":");
    myFile.print(myRTC.seconds);
    myFile.print("\t ");
    myFile.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(fileName); 
  }     
}

void setup() {
  initSerial();
  initSD();
  dht1.begin();
  dht2.begin();
  displayHeaderSerial();
  displayHeaderSD();
} 

void loop() {
  tic = millis();
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  myRTC.updateTime();
  
  displayDateTimeSerial();
  writeDataSerial(h1, t1, h2, t2, dt);
  displayDateTimeSD();
  writeDataSD(h1, t1, h2, t2, dt);
  dt = millis() - tic + sleepTime;
  delay(sleepTime);
}
