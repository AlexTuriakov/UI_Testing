/*
 * spi_eeprom.h
 *
 *  Created on: May 20, 2024
 *
 */

#ifndef INC_SPI_EEPROM_H_
#define INC_SPI_EEPROM_H_

/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
*/

extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

#define DEBUG_UART               &huart1
#define AT25SF081_SPI_PTR		 &hspi2
//#define W25QXX_SPI_PTR           &hspi2
//#define W25QXX_SPI               SPI2

#define  AT25SF081_CS_SELECT      HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
#define  AT25SF081_CS_UNSELECT    HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)


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

//@deprecated
typedef enum
{
	AT25SF081 = 1,
	W25Q20,
	W25Q40,
	W25Q80,
	W25Q16,
	W25Q32,
	W25Q64,
	W25Q128,
	W25Q256,
	W25Q512,

} AT25SF081_ID_t;

typedef struct
{
	AT25SF081_ID_t	ID;
	uint8_t		UniqID[8];
	uint16_t	PageSize;
	uint32_t	PageCount;
	uint32_t	SectorSize;
	uint32_t	SectorCount;
	uint32_t	BlockSize;
	uint32_t	BlockCount;
	uint32_t	CapacityInKiloByte;
	uint8_t		StatusRegister1;
	uint8_t		StatusRegister2;
	uint8_t		StatusRegister3;
	uint8_t		Lock;

} sAT25SF081_t;

typedef enum {
	SELECT,
	UNSELECT
} eCSAdapter_t;

typedef unsigned char (*BatteryTester_AT25SF081_HardwareTransmitReceiveCallback_t)(unsigned char data);
typedef void(*BatteryTester_AT25SF081_HardwareControlCallback_t)(eCSAdapter_t select);
//extern w25qxx_t	w25qxx;
extern sAT25SF081_t eeprom;

//############################################################################
// in Page,Sector and block read/write functions, can put 0 to read maximum bytes
//############################################################################

uint8_t	 BatteryTester_AT25SF081_Init(void);

void  BatteryTester_AT25SF081_EraseChip(void);
void  BatteryTester_AT25SF081_EraseSector(uint32_t SectorAddr);
void  BatteryTester_AT25SF081_EraseBlock(uint32_t BlockAddr);

uint32_t  BatteryTester_AT25SF081_PageToSector(uint32_t PageAddress);
uint32_t  BatteryTester_AT25SF081_PageToBlock(uint32_t PageAddress);
uint32_t  BatteryTester_AT25SF081_SectorToBlock(uint32_t SectorAddress);
uint32_t  BatteryTester_AT25SF081_SectorToPage(uint32_t SectorAddress);
uint32_t  BatteryTester_AT25SF081_BlockToPage(uint32_t BlockAddress);

uint8_t  BatteryTester_AT25SF081_IsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte);
uint8_t  BatteryTester_AT25SF081_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte);
uint8_t  BatteryTester_AT25SF081_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte);

void  BatteryTester_AT25SF081_WriteByte(uint8_t byte, uint32_t addr);
void  BatteryTester_AT25SF081_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize);
void  BatteryTester_AT25SF081_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize);
void  BatteryTester_AT25SF081_WriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize);

void  BatteryTester_AT25SF081_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address);
void  BatteryTester_AT25SF081_ReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void  BatteryTester_AT25SF081_ReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize);
void  BatteryTester_AT25SF081_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize);
void  BatteryTester_AT25SF081_ReadBlock(uint8_t *pBuffer, uint32_t Block_Address, uint32_t OffsetInByte,uint32_t NumByteToRead_up_to_BlockSize);

uint8_t	 BatteryTester_AT25SF081_Spi(uint8_t Data);


#endif /* INC_SPI_EEPROM_H_ */
