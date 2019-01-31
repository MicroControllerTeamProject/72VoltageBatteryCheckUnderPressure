#include <VirtualWire.h>
#include <RFWirelessTransmitter.h>
#include <RFWirelessReceiver.h>

RFWirelessReceiver rfWirelessReceiver(11, 13, 500);
RFWirelessTransmitter rFWirelessTransmitter(12, 50, 500);

char* devicesID[6] = { "B1","B1","B1","B1","B1","B1" };
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
		if (ciclo == 6)
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

			unsigned long startTime = millis();
			while (!receiveMessageFromSlave(devicesID[ciclo]) && ((millis()-startTime)<3000))
			{

			}
			ciclo++;
			//wait before trasmit an otherone.
			delay(500);
	
		}
	
}
void trasmitMessageToSlave(char* deviceId)
{
	rFWirelessTransmitter.startTrasmission(deviceId, "XX", 1);

	float data = 0.00;
	/*while (data < 0.10)
	{
		data = data + 0.05;*/
	rFWirelessTransmitter.sendBufferData(deviceId, "XX", "X", data, "0", "0");
	//}
	rFWirelessTransmitter.endTrasmission(deviceId, "XX");
}
bool receiveMessageFromSlave(char* deviceId)
{
	bool response = false;
	String message = "";
	//data = rfWirelessReceiver.GetMessage("BI","A0");
	message = rfWirelessReceiver.GetMessage();
	if (message == "init device transmission")
	{
		unsigned long startTime = millis();
		float sensorValue = 0.00;
		do {
			message = rfWirelessReceiver.GetMessage();
			if (message != "" && rfWirelessReceiver.GetDeviceId() == deviceId)
			{
				//Program Business Logic : Insert here your logic
				//Serial.println(message);
				
				if (message == "OK")
				{
					/*digitalWrite(13, HIGH);
					delay(500);
					digitalWrite(13, LOW);*/
					response = true;
					status[ciclo] = true;
				}
				else
				{
					sensorValue = rfWirelessReceiver.GetSensorValue().toFloat();
					Serial.print("Sensor value : "); Serial.println(sensorValue);
				}
			}
		} while (message != "OK" && message != "KO" && ((millis() - startTime) < 3000));
		if (message == "OK" &&  sensorValue != 0.00)
		{
			Serial.println("Dato ricevuto correttamente");
		}
	}
	return response;
}
