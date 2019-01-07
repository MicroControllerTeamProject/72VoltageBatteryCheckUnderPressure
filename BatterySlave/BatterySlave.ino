#include <VirtualWire.h>
#include <RFWirelessReceiver.h>
#include <RFWirelessTransmitter.h>
RFWirelessReceiver rfWirelessReceiver(11, 13, 500);
RFWirelessTransmitter rFWirelessTransmitter(12, 50, 500);

char* deviceid = "B";

void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	rfWirelessReceiver.begin();
	rFWirelessTransmitter.begin();
	Serial.println("Begin receveing");

	uint8_t digitalToNumber = getPinsConfiguration();
	char numberToString[1];
	dtostrf(digitalToNumber, 1, 0, numberToString);
	strcat(deviceid, numberToString);
	//Serial.println(deviceid);

}

void loop()
{
	checkArrivedMessageFromMaster();
}

byte getPinsConfiguration()
{
	byte dPinsConfiguration = 0b000;
	for (int i = 0; i < 3; i++) {
		Serial.println(digitalRead(5 + i));
		dPinsConfiguration = dPinsConfiguration | (digitalRead(5 + i) << i);
	}
	return dPinsConfiguration;
}

void checkArrivedMessageFromMaster()
{
	bool result = false;
	String data = "";
	//data = rfWirelessReceiver.GetMessage("BI","A0");
	data = rfWirelessReceiver.GetMessage();
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
				Serial.println("Store data");
				if (data == "OK")
				{
					digitalWrite(13, HIGH);
					delay(500);
					digitalWrite(13, LOW);
					result = true;
				}
			}

		} while (data != "OK" && data != "KO");

		if (rfWirelessReceiver.GetDeviceId() == deviceid)
		{
			sendDataToMaster();
		}

	}

}

void sendDataToMaster()
{
	rFWirelessTransmitter.startTrasmission(deviceid, "XX", 1);
	float data = 15.00;
	/*while (data < 0.50)
	{
		data = data + 0.05;*/

	rFWirelessTransmitter.SendBufferData(deviceid, "X", "XX", data, "0", "0");
	//}
	rFWirelessTransmitter.endTrasmission(deviceid, "XX");
}


