# Sam_n_Dads_GreenHouse

GreenHouse Project with Arduino MKR GSM 1400, 4*soil water level monitors, humiture sensor, photoresistor

Pin allocations for the Arduino:

	- Soil 1 input (analogue pin) A0
	- Soil 2 input (analogue pin) A1
	- Soil 3 input (analogue pin) A2
	- Soil 4 input (analogue pin) A3
	- PhotoResistor (analogue pin) A4

	- Motor 1 (small) output (digital pin) D2 
	- Motor 2 (large) output (digital pin) D3
	- Humiture detector (digital pin) D4

Pin Constant Names:

	Digital Pins - 
		DM1_PIN - Motor 1
		DM2_PIN - Motor 2
		DHT11_PIN - Humiture Sensor

	Analog Pins - 
		AS1_PIN - Soil 1 input 
		AS2_PIN - Soil 2 input 
		AS3_PIN - Soil 3 input 
		AS4_PIN - Soil 4 input 
		APR_PIN - PhotoResistor
	

Resources:
	SMS on Arduino with GSM - https://www.arduino.cc/en/Tutorial/GSMExamplesSendSMS