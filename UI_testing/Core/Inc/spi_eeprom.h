/*
 * spi_eeprom.h
 *
 *  Created on: May 20, 2024
 *
 */
#include <stdint.h>
#ifndef INC_SPI_EEPROM_H_
#define INC_SPI_EEPROM_H_

/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
*/

#define _ AT25SF081_USE_FREERTOS     0

#define INIT_DEBUG               1

#define  AT25SF081_WRITE_DISABLE     0x04
#define  AT25SF081_WRITE_ENABLE      0x06
#define  AT25SF081_CHIP_ERASE        0xC7 //0x60
#define  AT25SF081_SECTOR_ERASE      0x20
#define  AT25SF081_BLOCK_ERASE       0xD8
#define  AT25SF081_FAST_READ         0x0B
#define  AT25SF081_PAGE_PROGRAMM     0x02
#define  AT25SF081_GET_JEDEC_ID      0x9F
#define  AT25SF081_READ_STATUS_1     0x05
#define  AT25SF081_READ_STATUS_2     0x35
#define  AT25SF081_READ_STATUS_3     0x15
#define  AT25SF081_WRITE_STATUS_1    0x01
#define  AT25SF081_WRITE_STATUS_2    0x31
#define  AT25SF081_WRITE_STATUS_3    0x11
#define  AT25SF081_READ_UNIQUE_ID    0x4B

typedef struct
{
	uint8_t UniqID[8];
	uint16_t PageSize;
	uint32_t PageCount;
	uint32_t SectorSize;
	uint32_t SectorCount;
	uint32_t BlockSize;
	uint32_t BlockCount;
	uint32_t CapacityInKiloByte;
	uint8_t StatusRegister1;
	uint8_t StatusRegister2;
	uint8_t StatusRegister3;
	uint8_t Lock;

} sAT25SF081_t;

typedef enum {
	SELECT,
	UNSELECT
} eCSAdapter_t;

typedef uint8_t (*BatteryTester_AT25SF081_HardwareTransmitReceiveCallback_t)(uint8_t data);
typedef void(*BatteryTester_AT25SF081_HardwareControlCallback_t)(eCSAdapter_t select);
typedef void (*BatteryTester_AT25SF081_HardwareTransferCallback_t)(
		uint8_t* pBuffer,
		uint16_t size,
		uint32_t timeout);
extern sAT25SF081_t eeprom;

//############################################################################
// in Page,Sector and block read/write functions, can put 0 to read maximum bytes
//############################################################################
uint8_t BatteryTester_AT25SF081_transmitReceiveSpi(uint8_t);
uint8_t BatteryTester_AT25SF081_postCommand(uint8_t);
void BatteryTester_AT25SF081_selectChip(eCSAdapter_t);
void  BatteryTester_AT25SF081_WriteEnable(void);
void  BatteryTester_AT25SF081_WriteDisable(void);
void  BatteryTester_AT25SF081_WaitForReady(void);
void BatteryTester_AT25SF081_initDecorator(
		BatteryTester_AT25SF081_HardwareTransmitReceiveCallback_t,
		BatteryTester_AT25SF081_HardwareControlCallback_t,
		BatteryTester_AT25SF081_HardwareTransferCallback_t,
		BatteryTester_AT25SF081_HardwareTransferCallback_t);
void BatteryTester_AT25SF081_receiveData(uint8_t*, uint16_t, uint32_t);
void BatteryTester_AT25SF081_transmitData(uint8_t*, uint16_t, uint32_t);
void  BatteryTester_AT25SF081_EraseChip(void);
void  BatteryTester_AT25SF081_EraseSector(uint32_t);
void  BatteryTester_AT25SF081_EraseBlock(uint32_t);
uint32_t  BatteryTester_AT25SF081_PageToSector(uint32_t);
uint32_t  BatteryTester_AT25SF081_PageToBlock(uint32_t);
uint32_t  BatteryTester_AT25SF081_SectorToBlock(uint32_t);
uint32_t  BatteryTester_AT25SF081_SectorToPage(uint32_t);
uint32_t  BatteryTester_AT25SF081_BlockToPage(uint32_t);
uint8_t  BatteryTester_AT25SF081_IsEmptyPage(uint32_t, uint32_t);
void BatteryTester_AT25SF081_readArrayFastCommand(uint32_t, uint8_t*, uint16_t, uint32_t);
uint8_t BatteryTester_AT25SF081_isEmptyBuffer(uint8_t*, uint16_t);
uint8_t  BatteryTester_AT25SF081_IsEmptySector(uint32_t, uint32_t);
uint8_t  BatteryTester_AT25SF081_IsEmptyBlock(uint32_t, uint32_t);
void  BatteryTester_AT25SF081_WriteByte(uint8_t, uint32_t);
void  BatteryTester_AT25SF081_WritePage(uint8_t*, uint32_t, uint32_t, uint32_t);
void  BatteryTester_AT25SF081_WriteSector(uint8_t*, uint32_t, uint32_t, uint32_t);
void  BatteryTester_AT25SF081_WriteBlock(uint8_t*, uint32_t, uint32_t, uint32_t);
void  BatteryTester_AT25SF081_ReadByte(uint8_t*, uint32_t);
void  BatteryTester_AT25SF081_ReadBytes(uint8_t*, uint32_t, uint32_t);
void  BatteryTester_AT25SF081_ReadPage(uint8_t*, uint32_t, uint32_t, uint32_t);
void  BatteryTester_AT25SF081_ReadSector(uint8_t*, uint32_t, uint32_t, uint32_t);
void  BatteryTester_AT25SF081_ReadBlock(uint8_t*, uint32_t, uint32_t, uint32_t);

#endif /* INC_SPI_EEPROM_H_ */
