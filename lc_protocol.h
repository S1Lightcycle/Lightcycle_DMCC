#ifndef LC_PROTOCOL
#define LC_PROTOCOL

/*********************************************************************
 * INCLUDES
 */

/* Standard includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


/*********************************************************************
 * MACROS
 */



/*********************************************************************
 * CONSTANTS
 */

/* Address definitions */
#define ADDRESS_SERVER         0x00    // locical address of lightcycle base
#define ADDRESS_ROBOT_1        0x01    // locical address of lightcycle bike 1
#define ADDRESS_ROBOT_2        0x02    // locical address of lightcycle bike 2
#define ADDRESS_SPARE_1        0x03    //
#define ADDRESS_SPARE_2        0x04    //
#define ADDRESS_SPARE_3        0x05    //
#define ADDRESS_SPARE_4        0x06    //
#define ADDRESS_BROADCAST      0x07    // address all devices (broadcast)


/* Command definitions */
#define CMD_HEARTBEAT          0x00    // cyclic heartbeat from server
#define CMD_FORWARD            0x01    // 
#define CMD_REVERSE            0x02    //
#define CMD_STOP               0x03    //
#define CMD_TURN_RIGHT_STATIC  0x04    // turn in place; parm = 0...360 degrees
#define CMD_TURN_LEFT_STATIC   0x05    // turn in place; parm = 0...360 degrees
#define CMD_TURN_RIGHT_DYNAMIC 0x06    // turn into; parm = 0...90 degrees
#define CMD_TURN_LEFT_DYNAMIC  0x07    // turn into; parm = 0...90 degrees
#define CMD_SET_SPEED          0x08    // set speed which can be activated with forward or reverse
#define CMD_SPARE_1            0x09    // 
#define CMD_SPARE_2            0x0A    // 
#define CMD_SPARE_3            0x0B    // 
#define CMD_SPARE_4            0x0C    // 
#define CMD_SPARE_5            0x0D    // 
#define CMD_SPARE_6            0x0E    // 
#define CMD_SPARE_7            0x0F    // 

#define HI 0
#define LO 1

/*********************************************************************
 * TYPEDEFS
 */

typedef struct {
  unsigned int address : 3;
  unsigned int command : 4;
  unsigned long parameter : 9;
} lc_protocol_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */

 lc_protocol_t getProtocolStruct(char hi, char lo);
 void buildProtocolData(unsigned int address, unsigned int command, unsigned long parameter, unsigned char data []);
 
/*********************************************************************
*********************************************************************/


#endif
