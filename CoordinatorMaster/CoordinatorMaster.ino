#include <VirtualWire.h>
#include <RFWirelessTransmitter.h>
RFWirelessTransmitter rFWirelessTransmitter(12, 50, 500);

int operationNumber = 0;

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
	operationNumber++;
	String  a = String(operationNumber);
	char k[3];
	a.toCharArray(k, 3);
	/*itoa(operationNumber, k, 2);*/
	Serial.println(k);
	rFWirelessTransmitter.startTrasmission(deviceId, k, 1);
	float data = 0.00;
	/*while (data < 0.10)
	{
		data = data + 0.05;*/
	rFWirelessTransmitter.sendBufferData(deviceId, k, "X", data, "0", "0");
	//}
	rFWirelessTransmitter.endTrasmission(deviceId, k);

	if (operationNumber == 99) { operationNumber = 9; };
}
