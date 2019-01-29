#include <VirtualWire.h>
#include <RFWirelessReceiver.h>
#include <RFWirelessTransmitter.h>
RFWirelessReceiver rfWirelessReceiver(11, 13, 500);
RFWirelessTransmitter rFWirelessTransmitter(12, 50, 500);

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
	digitalToNumber = digitalToNumber - 30;

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
	Serial.print("Dispositivo in attesa : "); Serial.println(deviceid);
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
	float data = analogRead(A0);
	/*while (data < 0.50)
	{
		data = data + 0.05;*/

	rFWirelessTransmitter.SendBufferData(deviceid, "XX", "X", data, "0", "0");
	//}
	rFWirelessTransmitter.endTrasmission(deviceid, "XX");
}


