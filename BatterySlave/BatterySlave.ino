#include <VirtualWire.h>
#include <RFWirelessReceiver.h>
#include <RFWirelessTransmitter.h>
#include <eRCaGuy_analogReadXXbit.h>

RFWirelessReceiver rfWirelessReceiver(11, 13, 500);
RFWirelessTransmitter rFWirelessTransmitter(12, 50, 500);

eRCaGuy_analogReadXXbit adc;

float _voltageReference = 1.54;

const float MAX_READING_10_bit = 1023.0;
const float MAX_READING_11_bit = 2046.0;
const float MAX_READING_12_bit = 4092.0;
const float MAX_READING_13_bit = 8184.0;
const float MAX_READING_14_bit = 16368.0;
const float MAX_READING_15_bit = 32736.0;
const float MAX_READING_16_bit = 65472.0;
const float MAX_READING_17_bit = 130944.0;
const float MAX_READING_18_bit = 261888.0;
const float MAX_READING_19_bit = 523776.0;
const float MAX_READING_20_bit = 1047552.0;
const float MAX_READING_21_bit = 2095104.0;

char deviceid[3];

void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	rfWirelessReceiver.begin();
	rFWirelessTransmitter.begin();
	Serial.println("Begin receveing");

	pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	pinMode(6, INPUT_PULLUP);
	pinMode(7, INPUT_PULLUP);
	pinMode(8, INPUT_PULLUP);

	uint8_t digitalToNumber = getPinsConfiguration();
	//digitalToNumber = digitalToNumber - 30;

	char* numberToString;

	int index = 1;

	if (digitalToNumber > 9) 
	{
		index = 2;
	}
	numberToString = new char[index];
	dtostrf(digitalToNumber, index, 0, numberToString);
	Serial.println(numberToString);
	strcpy(deviceid, "B");
	strcat(deviceid, numberToString);
}

void loop()
{
	Serial.print("Dispositivo "); Serial.print(deviceid); Serial.println(" in attesa di dati ");
	checkArrivedMessageFromMaster();
}

byte getPinsConfiguration()
{
	byte dPinsConfiguration = 0b00000;
	for (int i = 0; i < 5; i++) {
		Serial.println(digitalRead(4 + i));
		dPinsConfiguration = dPinsConfiguration | (digitalRead(4 + i) << i);
	}
	return dPinsConfiguration;
}

void checkArrivedMessageFromMaster()
{
	bool result = false;
	String data = "";
	//data = rfWirelessReceiver.GetMessage("BI","A0");
	data = rfWirelessReceiver.GetMessage();
	Serial.println(data);
	if (data == "init device transmission")
	{
		do {
			data = rfWirelessReceiver.GetMessage();
			if (data != "" && rfWirelessReceiver.GetDeviceId() == deviceid)
			{
				//Program Business Logic : Insert here your logic
				Serial.println(data);
				if (data == "OK")
				{
					digitalWrite(13, HIGH);
					delay(500);
					digitalWrite(13, LOW);
					result = true;
				}
			}
			if (data != "" && rfWirelessReceiver.GetDeviceId() == "GO")
			{
				
				if (data == "OK")
				{
					Serial.println("Store data");
					digitalWrite(13, HIGH);
					delay(500);
					digitalWrite(13, LOW);
					result = true;
				}
			}

		} while (data != "OK" && data != "KO");

		if (rfWirelessReceiver.GetDeviceId() == deviceid)
		{
			//Serial.println("Invio");
			sendDataToMaster();
		}

	}

}

void sendDataToMaster()
{
	rFWirelessTransmitter.startTrasmission(deviceid, "XX", 1);
	float data = Voltage(A0);
	//float data = analogRead(A0);
	//delay(100);
	/*while (data < 0.50)
	{
		data = data + 0.05;*/

	rFWirelessTransmitter.SendBufferData(deviceid, "XX", "X", data, "0", "0");
	//}
	rFWirelessTransmitter.endTrasmission(deviceid, "XX");
}

float Voltage(uint8_t analogicPort)
{
	/*Serial.println("Lettura Analogica xxValue");*/
	uint8_t num_samples = 1; // 10 o 25
	uint8_t bits_of_precision; //bits of precision for the ADC (Analog to Digital Converter)
	float analog_reading; //the ADC reading
	//16-bit ADC reading
	bits_of_precision = 16; //bits of precision for the ADC (Analog to Digital Converter)
	analog_reading = adc.analogReadXXbit(analogicPort, bits_of_precision, num_samples); //get the avg. of [num_samples] 16-bit readings
	//return analog_reading;																				
	return (analog_reading / MAX_READING_16_bit * _voltageReference);
}


