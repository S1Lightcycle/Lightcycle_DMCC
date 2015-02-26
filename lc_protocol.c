/*********************************************************************
 * INCLUDES
 */
#include "lc_protocol.h"

/**************************************************************************************************
 *                                           Constant
 **************************************************************************************************/

#define MASK_ADDRESS      0xE0  // 0b11100000 address fields
#define MASK_COMMAND   	  0x1E  // 0b00011110 command fields
#define MASK_PARAMETER_HI 0x01  // 0b00000001 parameter fields
#define MASK_PARAMETER_LO 0x00FF  // 0b00000001 parameter fields
#define MASK_PARAMETER 0x01FF

/**************************************************************************************************
 *                                        Local Variables
 **************************************************************************************************/


/**************************************************************************************************
 *                                     Local Function Prototypes
 **************************************************************************************************/

/*
parse from the raw package the protocol struct

package
hi -----------> lo
111 1111 1|11111111
adr|c md|parameter
*/
lc_protocol_t getProtocolStruct(char hi, char lo) {	
	lc_protocol_t protocol;
		protocol.address = (hi & MASK_ADDRESS) >> 5;
        protocol.command = (hi & MASK_COMMAND) >> 1;
        protocol.parameter = lo | ((hi & MASK_PARAMETER_HI) << 8);        
	return protocol;
 }

 /*
 build a raw package by address, command and parameter
 data[HI] and data[LO] are the two bytes of the protocol
 */
void buildProtocolData(unsigned int address, unsigned int command, unsigned long parameter, unsigned char data []) {
  unsigned char hi = ((parameter & MASK_PARAMETER) >> 8);
  hi |= (command << 1);
  hi |= (address << 5);
  data[HI] = hi;  
  data[LO] = (parameter & MASK_PARAMETER_LO);
}
