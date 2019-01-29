#include <VirtualWire.h>
#include <RFWirelessTransmitter.h>
RFWirelessTransmitter rFWirelessTransmitter(12, 50, 500);

void setup()
{
	Serial.begin(9600);
	rFWirelessTransmitter.begin();
}


void loop()
{
	trasmitMessageToSlave("GO");
	delay(5000);
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
