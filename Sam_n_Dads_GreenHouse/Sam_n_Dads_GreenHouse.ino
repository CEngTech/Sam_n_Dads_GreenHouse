#include <dht.h> //Library for Humiture sensor
#include <Wire.h> 
#include <GSM.h> //library for SMS

#define PINNUMBER "" //Define pin for SMS (if any)

//Initialize instances of the classes used (GSM and GSM_SMS)
GSM gsmAccess;
GSM_SMS sms;

//dht DHT;//create a variable type of dht for Humiture sensor

//Function Decleration
void sendSMS(String remoteNumber[], int remoteNumLen, char txtMsg);
int tempSMS(int tempTimer, String remoteNumber[], int remoteNumLen, char txtMsg);

//Constant Declaration
//Digital Pins
  const int DM1_PIN = 2; //Motor 1 attach to pin2
  const int DM2_PIN = 3; //Motor 1 attach to pin3
  const int DHT11_PIN = 4; //Humiture sensor attach to pin4
//Analog Pins
  const int AS1_PIN = A0; //Soil 1 sensor attach to pin0
  const int AS2_PIN = A1; //Soil 2 sensor attach to pin1
  const int AS3_PIN = A2; //Soil 3 sensor attach to pin2
  const int AS4_PIN = A3; //Soil 4 sensor attach to pin3
  const int APR_PIN = A4; //PhotoResitor sensor attach to pin4
//SMS
  const int txtPeriod = 99999999; //How many loops should happen before a duplicate text is sent
  

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
  String remoteNumber[] = { "07773571078", "07460780161" } ; //Numbers to send SMS to
  int remoteNumLen = sizeof(remoteNumber) / sizeof(char); //Get length of remoteNumber array 
  
  //Humiture Sensor
  int greenhouseTemp = DHT.temperature; //Temperature of greenhouse
  int minDayTemp = 18; //Minimum desired temperature during day time (in celcius)
  int maxDayTemp = 28; //Maximum desired temperature during day time (in celcius)
  int minNightTemp = 10; //Minimum desired temperature during night time (in celcius)
  int tempTimer; //Timer for determining whether to send a text for temperature
  int tempTimerNew; //Variable used to change tempTimer
  int tempChangeTimer; //Timer for tempTimer management
  int greenhouseHum = DHT.humidity ; //Humidity of greenhouse
  int minHum = 70; //Minimum desired humidity
  int humTimer; //Timer for determining whether to send a text for Humidity
  
  //Photoresistor
  int lightLevel = analogRead(APR_PIN); //Light reading (Dark is 1023 Light is 0)
  
  //Soil Sensors
  int soil_1 = analogRead(AS1_PIN); //Read soil 1 pin
  int soil_2 = analogRead(AS2_PIN); //Read soil 2 pin
  int soil_3 = analogRead(AS3_PIN); //Read soil 3 pin
  int soil_4 = analogRead(AS4_PIN); //Read soil 4 pin
  int soilMean = (soil_1 + soil_2 + soil_3 + soil_4) / 4; //Mean for soil moisture

  //Initial timer Setup
  if (tempTimer == NULL){
    tempTimer = 0;
  }

  if (tempChangeTimer == NULL){
    tempChangeTimer = 0;
  }
  //End timer setup
  
  //Temperature Check
  if (lightLevel <= 600) { //If it is day
    if (greenhouseTemp > maxDayTemp) {
      char txtMsg = "Greenhouse Too Hot!";
      tempTimerNew = tempSMS(tempTimer, remoteNumber, remoteNumLen, txtMsg);
      tempTimer = tempTimerNew;
    } 
    else if (greenhouseTemp < minDayTemp) {
      char txtMsg = "Greenhouse Too Cold!";
      tempTimerNew = tempSMS(tempTimer, remoteNumber, remoteNumLen, txtMsg);
      tempTimer = tempTimerNew;
    } 
    else if (greenhouseTemp > (minDayTemp + 2)) {
      //Allow SMS to be sent
      tempTimer = 0;
    }
    else if (greenhouseTemp > (maxDayTemp - 2)) {
      //Allow SMS to be sent
      tempTimer = 0;
    }
    else {
      //Ensures that changes in temperature above and below the minimum and maximum do not re-send SMS
      if (tempChangeTimer < txtPeriod) {
        tempChangeTimer = tempChangeTimer + 1;
        tempTimer = 1;
      } else {
        tempChangeTimer = 0;
        tempTimer = 0;
      }
    }
  } 
  else { //If it is night
    if (greenhouseTemp < minNightTemp) { 
      char txtMsg = "Greenhouse Too Cold!";
      tempTimerNew = tempSMS(tempTimer, remoteNumber, remoteNumLen, txtMsg);
      tempTimer = tempTimerNew;
    }
    else if (greenhouseTemp > (minNightTemp + 2)) {
      //Allow SMS to be sent
      tempTimer = 0;
    }
    else {
      //Ensures that changes in temperature above and below the minimum do not re-send SMS
      if (tempChangeTimer < txtPeriod) {
        tempChangeTimer = tempChangeTimer + 1;
        tempTimer = 1;
      } else {
        tempChangeTimer = 0;
        tempTimer = 0;
      }
    }
  }
  //End Temperature Check

  //Humidity Check
  if (greenhouseHum < minHum) { 
    char txtMsg = "Greenhouse Needs More Humidity!";
    sendSMS(remoteNumber, remoteNumLen, txtMsg);
  }
  //End Humidity Check
  
}
//End Loop

//Send SMS to all stored phone numbers 
void sendSMS(String remoteNumber[], int remoteNumLen, char txtMsg) {
  for (int i = 0; i < remoteNumLen; i = i + 1) {
    sms.beginSMS(remoteNumber[i]);
    sms.print(txtMsg);
    sms.endSMS(); 
  }
}

//Determines if an SMS should be send depending on when the previous was sent 
int tempSMS(int tempTimer, String remoteNumber[], int remoteNumLen, char txtMsg) {
  if (tempTimer == 0) {
    sendSMS(remoteNumber, remoteNumLen, txtMsg);
    tempTimer = tempTimer + 1;
  } 
  else if (tempTimer == txtPeriod) { 
    tempTimer = 0;
  } 
  else {
    tempTimer = tempTimer + 1;
  }
  return(tempTimer);
}
