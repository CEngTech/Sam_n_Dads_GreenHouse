
#include <dht.h>
#include <Wire.h> 
#include <GSM.h> //library for SMS

#define PINNUMBER "" //Define pin for SMS (if any)

//Initialize instances of the classes used (GSM and GSM_SMS)
GSM gsmAccess;
GSM_SMS sms;

//set the LCD address to 0x27 for a 16 chars and 2 line display
dht DHT;//create a variable type of dht

//Function Decleration
void humitureCheck();

//Digital Pins
const int DM1_PIN = D2; //Motor 1 attach to pin2
const int DM2_PIN = D3; //Motor 1 attach to pin3
const int DHT11_PIN = 4; //Humiture sensor attach to pin4
//Analog Pins
const int AS1_PIN = A0; //Soil 1 sensor attach to pin0
const int AS2_PIN = A1; //Soil 2 sensor attach to pin1
const int AS3_PIN = A2; //Soil 3 sensor attach to pin2
const int AS4_PIN = A3; //Soil 4 sensor attach to pin3
const int APR_PIN = A4; //PhotoResitor sensor attach to pin4
  

void setup() {
  Serial.begin(115200);//initialize the serials

  //Check for SMS network connection
  boolean notConnected = true;
  while(notConnected) {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      //connected
      notConnected = false;
    else {
      //not connected
      //Serial.println("Not connected");
      delay(1000);
    }
  }
  //Serial.println("GSM initialized."); 
}

void loop() {
  //SMS
  char txtMsg; //Text Message to be sent
  char remoteNumber [] = { "07773571078", "07460780161" } ; //Numbers to send SMS to
  int remoteNumLen = sizeof(remoteNumber) / sizeof(char); //Get length of remoteNumber array 
  //Humiture Sensor
  int greenhouseTemp = DHT.temperature; //Temperature of greenhouse
  int minDayTemp = 18;
  int maxDayTemp = 28;
  int minNightTemp = 10;
  int greenhouseHum = DHT.humidity ; //Humidity of greenhouse
  int minHum = 70;
  //Photoresistor
  int lightLevel = analogRead(A4); //Light reading (Dark is 1023 Light is 0)

  //Temperature Check
  if (lightLevel >= /*high light value*/) { //If it is day
    if (greenhouseTemp > maxDayTemp) { 
      char txtMsg = "Greenhouse Too Hot!";
      sendSMS(remoteNumber, remoteNumLen, txtMsg);
    } else if (greenhouseTemp < minDayTemp) {
      char txtMsg = "Greenhouse Too Cold!";
      sendSMS(remoteNumber, remoteNumLen, txtMsg);
    }
  } else { //If it is night
    if (greenhouseTemp < minNightTemp) { 
      char txtMsg = "Greenhouse Too Cold!";
      sendSMS(remoteNumber, remoteNumLen, txtMsg);
  }

  //Humidity Check
  if (greenhouseHum < minHum) { 
    char txtMsg = "Greenhouse Needs More Humidity!";
    sendSMS(remoteNumber, remoteNumLen, txtMsg);
  }
  
}

//Send SMS to all phone numbers 
void sendSMS(remoteNumber, remoteNumLen, txtMsg) {
  for (int i = 0; i < remoteNumLen; i = i + 1) {
    sms.beginSMS(remoteNumber[i]);
    sms.print(txtMsg);
    sms.endSMS(); 
  }
}

void humitureCheck() {
  int chk = DHT.read11(DHT11_PIN);//read the value returned from Humiture sensor

  //Humiture sensor check
  switch (chk) {
    case DHTLIB_OK: 
    //Serial.println("OK!");
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    //Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    //Serial.print("Time out error,\t"); 
    break;
    default: 
    //Serial.print("Unknown error,\t"); 
    break;
  }
}
