#include "Arduino.h" 
#include "MiniMB.h"

#define RS485_BAUDRATE   9600



#define MMB_FCE_READ_HOLDING_REGISTER 3			//
#define MMB_FCE_READ_INPUT_REGISTER 4			// to read address 30000
#define MMB_FCE_WRITE_SINGLE_REGISTER 6			//

#define MMB_HW_PIN_RECIEVE  7
#define MMB_HW_PIN_TRANS  6
#define MMB_HW_PIN_RD   5

#define MMB_T_RESPONSE_MAX 500		// in mili seconds



MiniMB::MiniMB(SoftwareSerial * COMM485){
	
	pinMode(MMB_HW_PIN_RECIEVE, INPUT);
	digitalWrite(MMB_HW_PIN_RECIEVE,HIGH);
	
	pinMode(MMB_HW_PIN_TRANS, OUTPUT);
	digitalWrite(MMB_HW_PIN_TRANS,HIGH);
	
	pinMode(MMB_HW_PIN_RD, OUTPUT);
	digitalWrite(MMB_HW_PIN_RD,LOW);		//SET TO RECIEVE
	
	this->RS485Port = COMM485;
	this->RS485Port->begin(RS485_BAUDRATE);
}

unsigned int MiniMB::CalculateCRC( unsigned char Message[], int len)
{

unsigned int crc = 0xFFFF;
  
  for (int pos = 0; pos < len; pos++) {
    crc ^= (unsigned int)Message[pos];          // XOR byte into least sig. byte of crc
  
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;  
}

// FCE 03
bool MiniMB::ReadHoldingRegisters(unsigned char PhysicalAddress, unsigned int StartingAddress, unsigned int qty )
{
	unsigned int DataAddress;
	
	if(StartingAddress > 40000 ) {DataAddress = StartingAddress- 40001;}
	if(StartingAddress < 40000 ) {DataAddress = StartingAddress- 30001;}	
	
	byte Message[8];

	Message[0] = PhysicalAddress;					// Address
	Message[1] = MMB_FCE_READ_HOLDING_REGISTER;		// fce code
	Message[2] = highByte(DataAddress);				// Data address MSB
	Message[3] = lowByte(DataAddress);				// Data address LSB
	Message[4] = highByte(qty);						// qty MSB
	Message[5] = lowByte(qty);						// qty LSB

	unsigned int CRC=CalculateCRC(Message, 6);

	Message[6] = lowByte(CRC);			// CRC LSB
	Message[7] = highByte(CRC);			// CRC MSB
	
	
	this->SendMessage(Message,8);

	
return 1;
}

// FCE 06
bool MiniMB::WriteSingleHoldingRegister(unsigned char PhysicalAddress, unsigned int StartingAddress, unsigned int value  )
{

	unsigned int DataAddress;
	
	if(StartingAddress > 40000 ) {DataAddress = StartingAddress- 40001;}
	if(StartingAddress < 40000 ) {DataAddress = StartingAddress- 30001;}	
	
	byte Message[8];

	Message[0] = PhysicalAddress;					// Address
	Message[1] = MMB_FCE_WRITE_SINGLE_REGISTER;		// fce code
	Message[2] = highByte(DataAddress);				// Data address MSB
	Message[3] = lowByte(DataAddress);				// Data address LSB
	Message[4] = highByte(value);					// qty MSB
	Message[5] = lowByte(value);					// qty LSB
	
	unsigned int CRC=CalculateCRC(Message, 6);
	
	Message[6] = lowByte(CRC);			// CRC LSB
	Message[7] = highByte(CRC);			// CRC MSB
	
	this->SendMessage(Message,8);
	
	
}


bool MiniMB::SendMessage(unsigned char Message[], int size)
{
	digitalWrite(MMB_HW_PIN_RD, HIGH);	

	for (int i=0;i<size;i++) {
		this->RS485Port->write(Message[i]);		
	}
		
	digitalWrite(MMB_HW_PIN_RD, LOW);
return 1;	
}




