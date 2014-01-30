/*****************************************************************************
 *
 * Atmel Corporation
 *
 * File              : TWI_Master.h
 * Compiler          : avr-gcc (GCC) 4.8.1
 * Revision          : $Revision: 1.14 $
 * Date              : $Date: 30. Jan 2014 03:18:00 $
 * Updated by        : $Author: Jeremy Butler $
 *
 * Support mail      : avr@atmel.com
 *
 * Supported devices : All devices with a TWI module can be used.
 *                     The example is written for the ATmega16
 *
 * AppNote           : AVR315 - TWI Master Implementation
 *
 * Description       : Header file for TWI_Master.c
 *                     Include this file in the application.
 *
 ****************************************************************************/

#ifndef TWI_MASTER_H_
#define TWI_MASTER_H_

/****************************************************************************
 TWI Status/Control register definitions
 ****************************************************************************/
#define TWI_BUFFER_SIZE 4   // Set this to the largest message size that will be sent including address byte.
#define TWI_TWBR            0x0C        // TWI Bit rate Register setting.
// See Application note for detailed
// information on setting this value.
// Not used defines!
//#define TWI_TWPS          0x00        // This driver presumes prescaler = 00

/****************************************************************************
 Global definitions
 ****************************************************************************/

union TWI_statusReg                       // Status byte holding flags.
{
	unsigned char all;
	struct
	{
		unsigned char lastTransOK :1;
		unsigned char unusedBits :7;
	};
};

extern union TWI_statusReg TWI_statusReg;

typedef enum
{
	STOP,
	REPEAT_START
}TWI_Action_t;

typedef struct TWI_slave
{
	void *device;
	unsigned char address;

	// This function serves as a signal for when the transmission between master
	// and slave has finished.
	//
	// The return value determines what the transceiver should do next.
	TWI_Action_t (*TWI_Next_Action)(const void *dev);

	unsigned char messageBuffer[TWI_BUFFER_SIZE];
	unsigned char messageSize;
} TWI_slave_t;

/****************************************************************************
 Function definitions
 ****************************************************************************/
void TWI_Master_Initialise(void);
unsigned char TWI_Transceiver_Busy(void);
unsigned char TWI_Get_State_Info(void);
void TWI_Start_Transceiver_With_Data(TWI_slave_t *);
void TWI_Start_Transceiver(void);
unsigned char TWI_Get_Data_From_Transceiver(TWI_slave_t *);

/****************************************************************************
 Bit and byte definitions
 ****************************************************************************/
#define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       // Bit position for LSB of the slave address bits in the init byte.
#define TRUE          1
#define FALSE         0
/****************************************************************************
 TWI State codes
 ****************************************************************************/
// General TWI Master status codes
#define TWI_START                  0x08  // START has been transmitted
#define TWI_REP_START              0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  // Arbitration lost// TWI Master Transmitter status codes
#define TWI_MTX_ADR_ACK            0x18  // SLA+W has been transmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  // SLA+W has been transmitted and NACK received
#define TWI_MTX_DATA_ACK           0x28  // Data byte has been transmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  // Data byte has been transmitted and NACK received
// TWI Master Receiver status codes
#define TWI_MRX_ADR_ACK            0x40  // SLA+R has been transmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  // SLA+R has been transmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  // Data byte has been received and ACK transmitted
#define TWI_MRX_DATA_NACK          0x58  // Data byte has been received and NACK transmitted
// TWI Slave Transmitter status codes
#define TWI_STX_ADR_ACK            0xA8  // Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK           0xB8  // Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK          0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8  // Last data byte in TWDR has been transmitted (TWEA = �0�); ACK has been received
// TWI Slave Receiver status codes
#define TWI_SRX_ADR_ACK            0x60  // Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK            0x70  // General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK       0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK      0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK       0x90  // Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK      0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART       0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave
// TWI Miscellaneous status codes
#define TWI_NO_STATE               0xF8  // No relevant state information available; TWINT = �0�
#define TWI_BUS_ERROR              0x00  // Bus error due to an illegal START or STOP condition

#endif /* TWI_MASTER_H_ */
