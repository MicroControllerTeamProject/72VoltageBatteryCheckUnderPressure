#include <VirtualWire.h>
#include <RFWirelessTransmitter.h>
#include <RFWirelessReceiver.h>

RFWirelessReceiver rfWirelessReceiver(11, 13, 500);
RFWirelessTransmitter rFWirelessTransmitter(12, 50, 500);

char* devicesID[6] = { "B31","B1","B2","B3","B4","B5" };
bool  status[6] = { false,false,false,false,false,false };

void setup()
{
	pinMode(13, OUTPUT);
	Serial.begin(9600);
	rfWirelessReceiver.begin();
	rFWirelessTransmitter.begin();
}
uint8_t ciclo = 0;

void loop()
{
		if (ciclo == 2)
		{
			ciclo = 0;
			status[0] = false;
			status[1] = false;
			status[2] = false;
			status[3] = false;
			status[4] = false;
			status[5] = false;
		}

		if (!status[ciclo])
		{
			Serial.print("Data Master find slave "); Serial.println(devicesID[ciclo]);

			trasmitMessageToSlave(devicesID[ciclo]);

			//wait for receve data from slave
			for (int i = 0; i < 3; i++)
			{
				if (receiveMessageFromSlave(devicesID[ciclo]))
				{
					ciclo++;
					return;
				}
			}
		}
	
}
void trasmitMessageToSlave(char* deviceId)
{
	rFWirelessTransmitter.startTrasmission(deviceId, "XX", 1);
	float data = 0.00;
	/*while (data < 0.10)
	{
		data = data + 0.05;*/
	rFWirelessTransmitter.SendBufferData(deviceId, "XX", "X", data, "0", "0");
	//}
	rFWirelessTransmitter.endTrasmission(deviceId, "XX");
}
bool receiveMessageFromSlave(char* deviceId)
{
	bool response = false;
	String data = "";
	//data = rfWirelessReceiver.GetMessage("BI","A0");
	data = rfWirelessReceiver.GetMessage();
	if (data == "init device transmission")
	{
		do {
			data = rfWirelessReceiver.GetMessage();
			if (data != "" && rfWirelessReceiver.GetDeviceId() == deviceId)
			{
				//Program Business Logic : Insert here your logic
				Serial.println(data);
				if (data == "OK")
				{
					digitalWrite(13, HIGH);
					delay(500);
					digitalWrite(13, LOW);
					response = true;
					status[ciclo] = true;
				}
			}
		} while (data != "OK" && data != "KO");
	}
	return response;
}
