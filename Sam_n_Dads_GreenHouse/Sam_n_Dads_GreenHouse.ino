
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
const int DM1_PIN = 2; //Motor 1 attach to pin2
const int DM2_PIN = 3; //Motor 1 attach to pin3
const int DHT11_PIN = 4; //Humiture sensor attach to pin4
//Analog Pins
const int AS1_PIN = 0; //Soil 1 sensor attach to pin0
const int AS2_PIN = 1; //Soil 2 sensor attach to pin1
const int AS3_PIN = 2; //Soil 3 sensor attach to pin2
const int AS4_PIN = 3; //Soil 4 sensor attach to pin3
const int APR_PIN = 4; //PhotoResitor sensor attach to pin4
  

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
  //READ DATA
  //Serial.println("DHT11:");
  //SMS
  char txtMsg; //Text Message to be sent
  char remoteNumber [] = { "07773571078", "07460780161" } ; //Numbers to send SMS to
  int remoteNumLen = sizeof(remoteNumber) / sizeof(char); //Get length of remoteNumber array 
  //Humiture
  int greenhouseTemp = DHT.temperature; //Temperature of greenhouse
  //int minTemp = ;
  //int maxTemp = ;
  int greenhouseHum = DHT.humidity ; //Humidity of greenhouse
  //int minHum = ;
  //int maxHum = ;

  if (greenhouseTemp > maxTemp) {
    char txtMsg = "Greenhouse Too Hot!";
    sendSMS(remoteNumber, remoteNumLen, txtMsg);
  } else if (greenhouseTemp < minTemp) {
    char txtMsg = "Greenhouse Too Cold!";
    sendSMS(remoteNumber, remoteNumLen, txtMsg);
  }

  if (greenhouseHum > maxHum) {
    char txtMsg = "Greenhouse Too Humid!";
    sendSMS(remoteNumber, remoteNumLen, txtMsg);
  } else if (greenhouseHum < minHum) {
    char txtMsg = "Greenhouse Not Humid Enough!";
    sendSMS(remoteNumber, remoteNumLen, txtMsg);
  }
  
}

void sendSMS(remoteNumber, remoteNumLen, txtMsg) {
  for (int i = 0; i < remoteNumLen; i = i + 1 ) {
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
