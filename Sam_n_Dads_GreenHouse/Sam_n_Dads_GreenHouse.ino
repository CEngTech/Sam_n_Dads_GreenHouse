
//include the libraries
#include <dht.h>
#include <Wire.h> 

dht DHT;//create a variable type of dht

const int DHT11_PIN= 7;//Humiture sensor attach to Digital pin7

void setup()
{
  Serial.begin(115200);//initialize the serial
}

void loop()
{
  // READ DATA
  //Serial.println("DHT11:");
  
  int chk = DHT.read11(DHT11_PIN);//read the value returned from sensor
  switch (chk)
  {
  case DHTLIB_OK:  
    Serial.println(" OK!"); 
    break;
  case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
  case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
  default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
  delay(4000); //wait a while 
  Serial.print("Tem: ");
  Serial.print(DHT.temperature,1);
  Serial.print("  ");
  Serial.print("Hum: ");
  Serial.print(DHT.humidity,1);
}
