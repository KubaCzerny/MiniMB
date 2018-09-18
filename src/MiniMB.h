//#ifndef MiniMB_h
//#define MiniMB_h

#define MMB_HW_PIN_RECIEVE	7
#define MMB_HW_PIN_TRANS	6
#define MMB_HW_PIN_RD		5

#include <SoftwareSerial.h>

class MiniMB
{
	
public:	

	MiniMB(SoftwareSerial * COMM485);
	
	bool ReadHoldingRegisters(unsigned char PhysicalAddress, unsigned int StartingAddress, unsigned int qty  );
	bool WriteSingleHoldingRegister(unsigned char PhysicalAddress, unsigned int StartingAddress, unsigned int value  );
	
private:

	int Baudrate;
	int T_delay;
	
	unsigned int CalculateCRC(unsigned char Message[], int len);	
	bool SendMessage(unsigned char Message[], int size);

	SoftwareSerial *RS485Port;	// SoftwareSerial port
	
	
};