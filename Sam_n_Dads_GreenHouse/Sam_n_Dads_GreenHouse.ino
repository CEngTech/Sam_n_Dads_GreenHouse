
#include <dht.h>
#include <Wire.h> 
#include <GSM.h> //library for SMS

#define PINNUMBER "" //Define pin for SMS (if any)

//Initialize instances of the classes used (GSM and GSM_SMS)
GSM gsmAccess;
GSM_SMS sms;

//set the LCD address to 0x27 for a 16 chars and 2 line display
dht DHT;//create a variable type of dht

const int DHT11_PIN= 7;//Humiture sensor attach to pin7

void setup() {
  Serial.begin(9600);//initialize the serials

  boolean notConnected = true;

  //Check for SMS network connection
  while(notConnected) {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      //connected
      notConnected = false;
    else {
      //not connected
      Serial.println("Not connected");
      delay(1000);
    }
  }
  
  Serial.println("GSM initialized."); 
}

void loop() {
  //READ DATA
  //Serial.println("DHT11:");
  char txtMsg; //Text Message to be sent
  char remoteNumber1 = "07773571078"; //First number to send SMS too
  char remoteNumber2 = "07460780161"; //Second number to send SMS too
  
  int chk = DHT.read11(DHT11_PIN);//read the value returned from sensor
  switch (chk)
  {
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
