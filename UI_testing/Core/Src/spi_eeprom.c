/*
 * spi_eeprom.c
 *
 *  Created on: May 20, 2024
 *      Author: Nima Askari
 */
#include "spi_eeprom.h"
#include "stm32f0xx_hal.h"


#if (INIT_DEBUG == 1)
#include "string.h"
#include "stdio.h"
char buf[64] = {0,};
extern UART_HandleTypeDef huart1;
#endif


#define AT25SF081_DUMMY_BYTE        0xA5

sAT25SF081_t eeprom;
static BatteryTester_AT25SF081_HardwareTransmitReceiveCallback_t g_transmitReceiveCallback = 0;
static BatteryTester_AT25SF081_HardwareControlCallback_t g_selectChipCallback = 0;

#if (_AT25SF081_USE_FREERTOS == 1)
#define	 BatteryTester_AT25SF081_Delay(delay)		osDelay(delay)
#include "cmsis_os.h"
#else
#define	 BatteryTester_AT25SF081_Delay(delay)		HAL_Delay(delay)
#endif



//###################################################################################################################
/*
 * @deprecated
 */
uint8_t	 BatteryTester_AT25SF081_Spi(uint8_t	Data)
{
	uint8_t	ret;

	HAL_SPI_TransmitReceive(AT25SF081_SPI_PTR, &Data, &ret, 1, 100); // spi2


	/*while(!(AT25SF081_SPI->SR & SPI_SR_TXE));
	AT25SF081_SPI->DR = Data;

	while(!(AT25SF081_SPI->SR & SPI_SR_RXNE));
	ret = AT25SF081_SPI->DR;*/

	//while((AT25SF081_SPI->SR & SPI_SR_BSY));
	//__HAL_SPI_CLEAR_OVRFLAG(&hspi2);

	return ret;
}

/*
 * @new
 */
unsigned char BatteryTester_AT25SF081_transmitReceiveSpi(unsigned char data){
	unsigned char ret = 0xFF;
	if(g_transmitReceiveCallback){
		BatteryTester_AT25SF081_selectChip(SELECT);
		ret = g_transmitReceiveCallback(data);
		BatteryTester_AT25SF081_selectChip(UNSELECT);
	}
	return ret;
}

unsigned char BatteryTester_AT25SF081_postCommand(unsigned char data){
	if(g_transmitReceiveCallback){
		return g_transmitReceiveCallback(data);
	}
	return 0xFF;
}
//###################################################################################################################
/*
 * @deprecated
 */
uint32_t  BatteryTester_AT25SF081_ReadID(void)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	AT25SF081_CS_SELECT;

	 BatteryTester_AT25SF081_Spi(AT25SF081_GET_JEDEC_ID);

	Temp0 =  BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);
	Temp1 =  BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);
	Temp2 =  BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);

	AT25SF081_CS_UNSELECT;

	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}
/************************************************************/
void BatteryTester_AT25SF081_selectChip(eCSAdapter_t select){
	if(g_selectChipCallback){
		g_selectChipCallback(select);
	}
}
//###################################################################################################################
/*void  BatteryTester_AT25SF081_ReadUniqID(void)
{
	AT25SF081_CS_SELECT;
	 BatteryTester_AT25SF081_Spi(AT25SF081_READ_UNIQUE_ID);

	for(uint8_t	i = 0; i < 4; i++)
		 BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);

	for(uint8_t	i = 0; i < 8; i++)
		eeprom.UniqID[i] =  BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);

	AT25SF081_CS_UNSELECT;
}*/

//###################################################################################################################
/*
 * @denger: blocking wait
 */
void  BatteryTester_AT25SF081_WriteEnable(void)
{
	//AT25SF081_CS_SELECT;
	//BatteryTester_AT25SF081_Spi(AT25SF081_WRITE_ENABLE);
	//AT25SF081_CS_UNSELECT;
	BatteryTester_AT25SF081_transmitReceiveSpi(AT25SF081_WRITE_ENABLE);
	//BatteryTester_AT25SF081_Delay(1);
}

//###################################################################################################################
/*
 * @denger: blocking wait
 */
void  BatteryTester_AT25SF081_WriteDisable(void)
{
	/*AT25SF081_CS_SELECT;
	 BatteryTester_AT25SF081_Spi(AT25SF081_WRITE_DISABLE);
	AT25SF081_CS_UNSELECT;*/
	BatteryTester_AT25SF081_transmitReceiveSpi(AT25SF081_WRITE_DISABLE);
	//BatteryTester_AT25SF081_Delay(1);
}

//###################################################################################################################
/*uint8_t  BatteryTester_AT25SF081_ReadStatusRegister(uint8_t SelectStatusRegister_1_2_3)
{
	uint8_t	status=0;
	AT25SF081_CS_SELECT;

	if(SelectStatusRegister_1_2_3 == 1)
	{
		 BatteryTester_AT25SF081_Spi(AT25SF081_READ_STATUS_1);
		status =  BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);
		eeprom.StatusRegister1 = status;
	}
	else if(SelectStatusRegister_1_2_3 == 2)
	{
		 BatteryTester_AT25SF081_Spi(AT25SF081_READ_STATUS_2);
		status =  BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);
		eeprom.StatusRegister2 = status;
	}
	else
	{
		 BatteryTester_AT25SF081_Spi(AT25SF081_READ_STATUS_3);
		status =  BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);
		eeprom.StatusRegister3 = status;
	}

	AT25SF081_CS_UNSELECT;

	return status;
}*/

//###################################################################################################################
/*void  BatteryTester_AT25SF081_WriteStatusRegister(uint8_t	SelectStatusRegister_1_2_3, uint8_t Data)
{
	AT25SF081_CS_SELECT;

	if(SelectStatusRegister_1_2_3 == 1)
	{
		 BatteryTester_AT25SF081_Spi(AT25SF081_WRITE_STATUS_1);
		eeprom.StatusRegister1 = Data;
	}
	else if(SelectStatusRegister_1_2_3 == 2)
	{
		 BatteryTester_AT25SF081_Spi(AT25SF081_WRITE_STATUS_2);
		eeprom.StatusRegister2 = Data;
	}
	else
	{
		 BatteryTester_AT25SF081_Spi(AT25SF081_WRITE_STATUS_3);
		eeprom.StatusRegister3 = Data;
	}

	 BatteryTester_AT25SF081_Spi(Data);

	AT25SF081_CS_UNSELECT;
}*/

//###################################################################################################################
/*
 * check status register bit RDY/BSY
 * @denger: blocking wait
 */
void  BatteryTester_AT25SF081_WaitForReady(void)
{
	//BatteryTester_AT25SF081_Delay(1);
	BatteryTester_AT25SF081_selectChip(SELECT);
	BatteryTester_AT25SF081_postCommand(AT25SF081_READ_STATUS_1);
	/*AT25SF081_CS_SELECT;
	BatteryTester_AT25SF081_Spi(AT25SF081_READ_STATUS_1);*/
	do{
		eeprom.StatusRegister1 =  BatteryTester_AT25SF081_postCommand(AT25SF081_DUMMY_BYTE);
		BatteryTester_AT25SF081_Delay(1);
	}
	while((eeprom.StatusRegister1 & 0x01) == 0x01);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
	//AT25SF081_CS_UNSELECT;
}

//###################################################################################################################
/*
 * @deprecated
 */
uint8_t  BatteryTester_AT25SF081_Init(void)
{
	eeprom.Lock = 1;
	while(HAL_GetTick() < 100)
	 BatteryTester_AT25SF081_Delay(1);

	AT25SF081_CS_UNSELECT;
	 BatteryTester_AT25SF081_Delay(100);

	uint32_t id;

	#if (INIT_DEBUG == 1)
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Init Begin...\n", 14, 1000);
	#endif

	id =  BatteryTester_AT25SF081_ReadID();

	#if (INIT_DEBUG == 1)
	snprintf(buf, 64, "ID:0x%lX\n", id);
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen(buf), 1000);
	#endif

	switch(id & 0x0000FFFF)
	{
		case 0x401A:	// 	w25q512
			eeprom.ID = W25Q512;
			eeprom.BlockCount = 1024;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q512\n", 14, 1000);
			#endif
		break;

		case 0x4019:	// 	w25q256
			eeprom.ID = W25Q256;
			eeprom.BlockCount = 512;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q256\n", 14, 1000);
			#endif
		break;

		case 0x4018:	// 	w25q128
			eeprom.ID = W25Q128;
			eeprom.BlockCount = 256;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q128\n", 14, 1000);
			#endif
		break;

		case 0x4017:	//	w25q64
			eeprom.ID = W25Q64;
			eeprom.BlockCount = 128;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q64\n", 13, 1000);
			#endif
		break;

		case 0x4016:	//	w25q32
			eeprom.ID = W25Q32;
			eeprom.BlockCount = 64;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q32\n", 13, 1000);
			#endif
		break;

		case 0x4015:	//	w25q16
			eeprom.ID = W25Q16;
			eeprom.BlockCount = 32;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q16\n", 13, 1000);
			#endif
		break;

		case 0x4014:	//	w25q80
			eeprom.ID = W25Q80;
			eeprom.BlockCount = 16;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q80\n", 13, 1000);
			#endif
		break;

		case 0x4013:	//	w25q40
			eeprom.ID = W25Q40;
			eeprom.BlockCount = 8;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q40\n", 13, 1000);
			#endif
		break;

		case 0x4012:	//	w25q20
			eeprom.ID = W25Q20;
			eeprom.BlockCount = 4;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q20\n", 13, 1000);
			#endif
		break;

		case 0x4011:	//	w25q10
			eeprom.ID = W25Q10;
			eeprom.BlockCount = 2;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25q10\n", 13, 1000);
			#endif
		break;

		////////////////////////////////////////////////////////////////////////////////

		case 0x3017:	//	w25x64
			//eeprom.ID = W25Q64;
			eeprom.BlockCount = 128;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25x64\n", 13, 1000);
			#endif
		break;

		case 0x3016:	//	w25x32
			//eeprom.ID = W25Q32;
			eeprom.BlockCount = 64;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25x32\n", 13, 1000);
			#endif
		break;

		case 0x3015:	//	w25q16
			//eeprom.ID = W25Q16;
			eeprom.BlockCount = 32;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25x16\n", 13, 1000);
			#endif
		break;

		////////////////////////////////////////////////////////////////////////////////
		case 0x3014:	//	w25x80
			//eeprom.ID = W25Q80;
			eeprom.BlockCount = 16;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25x80\n", 13, 1000);
			#endif
		break;

		case 0x3013:	//	w25x40
			//eeprom.ID = W25Q40;
			eeprom.BlockCount = 8;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25x40\n", 13, 1000);
			#endif
		break;

		case 0x3012:	//	w25x20
			//eeprom.ID = W25Q20;
			eeprom.BlockCount = 4;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25x20\n", 13, 1000);
			#endif
		break;

		case 0x3011:	//	w25x10
			//eeprom.ID = W25Q10;
			eeprom.BlockCount = 2;
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Chip: w25x10\n", 13, 1000);
			#endif
		break;



		default:
			#if (INIT_DEBUG == 1)
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Unknown ID\n", 11, 1000);
			#endif

			eeprom.Lock = 0;
			return 0;
	}


	eeprom.PageSize = 256;
	eeprom.SectorSize = 0x1000;
	eeprom.SectorCount = eeprom.BlockCount * 16;
	eeprom.PageCount = (eeprom.SectorCount * eeprom.SectorSize) / eeprom.PageSize;
	eeprom.BlockSize = eeprom.SectorSize * 16;
	eeprom.CapacityInKiloByte = (eeprom.SectorCount * eeprom.SectorSize) / 1024;

	// BatteryTester_AT25SF081_ReadUniqID();
	// BatteryTester_AT25SF081_ReadStatusRegister(1);
	// BatteryTester_AT25SF081_ReadStatusRegister(2);
	// BatteryTester_AT25SF081_ReadStatusRegister(3);

	#if (INIT_DEBUG == 1)
	snprintf(buf, 64, "Page Size: %d Bytes\n", eeprom.PageSize);
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen(buf), 1000);

	snprintf(buf, 64, "Page Count: %lu\n", eeprom.PageCount);
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen(buf), 1000);

	snprintf(buf, 64, "Sector Size: %lu Bytes\n", eeprom.SectorSize);
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen(buf), 1000);

	snprintf(buf, 64, "Sector Count: %lu\n", eeprom.SectorCount);
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen(buf), 1000);

	snprintf(buf, 64, "Block Size: %lu Bytes\n", eeprom.BlockSize);
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen(buf), 1000);

	snprintf(buf, 64, "Block Count: %lu\n", eeprom.BlockCount);
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen(buf), 1000);

	snprintf(buf, 64, "Capacity: %lu KBytes\n", eeprom.CapacityInKiloByte);
	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen(buf), 1000);

	HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Init Done\n", 10, 1000);
	#endif

	eeprom.Lock = 0;
	return 1;
}

/********************************************************************/
void BatteryTester_AT25SF081_initDecorator(
		BatteryTester_AT25SF081_HardwareTransmitReceiveCallback_t transmitReceiveCallback,
		BatteryTester_AT25SF081_HardwareControlCallback_t selectChipCallback){
	g_transmitReceiveCallback = transmitReceiveCallback;
	g_selectChipCallback = selectChipCallback;
	eeprom.Lock = 1;
	BatteryTester_AT25SF081_Delay(100);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
    BatteryTester_AT25SF081_Delay(100);
    /*@default: for AT25SF081*/
    eeprom.BlockCount = 16;
    eeprom.PageSize = 256;
	eeprom.SectorSize = 0x1000;
	eeprom.SectorCount = eeprom.BlockCount * 16;
	eeprom.PageCount = (eeprom.SectorCount * eeprom.SectorSize) / eeprom.PageSize;
	eeprom.BlockSize = eeprom.SectorSize * 16;
	eeprom.CapacityInKiloByte = (eeprom.SectorCount * eeprom.SectorSize) / 1024;
	/*@end*/
	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_EraseChip(void)
{
	while(eeprom.Lock == 1){
		BatteryTester_AT25SF081_Delay(1);
	}
	eeprom.Lock = 1;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_WriteEnable();
	BatteryTester_AT25SF081_transmitReceiveSpi(AT25SF081_CHIP_ERASE);
	BatteryTester_AT25SF081_WaitForReady();
//	BatteryTester_AT25SF081_Delay(10);
	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_EraseSector(unsigned int SectorAddr)
{
	while(eeprom.Lock == 1)
		 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 1;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_WriteEnable();
	SectorAddr *= eeprom.SectorSize;
	BatteryTester_AT25SF081_selectChip(SELECT);
	BatteryTester_AT25SF081_postCommand(AT25SF081_SECTOR_ERASE);
	BatteryTester_AT25SF081_postCommand((SectorAddr & 0xFF0000) >> 16);
	BatteryTester_AT25SF081_postCommand((SectorAddr & 0xFF00) >> 8);
	BatteryTester_AT25SF081_postCommand(SectorAddr & 0xFF);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
	BatteryTester_AT25SF081_WaitForReady();
	//BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_EraseBlock(uint32_t BlockAddr)
{
	while(eeprom.Lock == 1)
		 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 1;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_WriteEnable();
	BlockAddr *= eeprom.BlockSize;
	BatteryTester_AT25SF081_selectChip(SELECT);
	BatteryTester_AT25SF081_postCommand(AT25SF081_BLOCK_ERASE);
	BatteryTester_AT25SF081_postCommand((BlockAddr & 0xFF0000) >> 16);
	BatteryTester_AT25SF081_postCommand((BlockAddr & 0xFF00) >> 8);
	BatteryTester_AT25SF081_postCommand(BlockAddr & 0xFF);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
	BatteryTester_AT25SF081_WaitForReady();
    //BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 0;
}

//###################################################################################################################
unsigned int  BatteryTester_AT25SF081_PageToSector(unsigned int PageAddress)
{
	return((PageAddress * eeprom.PageSize) / eeprom.SectorSize);
}

//###################################################################################################################
unsigned int  BatteryTester_AT25SF081_PageToBlock(unsigned int PageAddress)
{
	return((PageAddress * eeprom.PageSize) / eeprom.BlockSize);
}

//###################################################################################################################
unsigned int  BatteryTester_AT25SF081_SectorToBlock(unsigned int SectorAddress)
{
	return((SectorAddress * eeprom.SectorSize) / eeprom.BlockSize);
}

//###################################################################################################################
unsigned int  BatteryTester_AT25SF081_SectorToPage(unsigned int SectorAddress)
{
	return(SectorAddress * eeprom.SectorSize) / eeprom.PageSize;
}

//###################################################################################################################
unsigned int  BatteryTester_AT25SF081_BlockToPage(unsigned int BlockAddress)
{
	return (BlockAddress * eeprom.BlockSize) / eeprom.PageSize;
}

//###################################################################################################################
uint8_t  BatteryTester_AT25SF081_IsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte)
{
	while(eeprom.Lock == 1)
	 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock = 1;

	uint8_t	pBuffer[256] = {0,};
	uint32_t WorkAddress = 0;
	uint16_t size = 0;

	size = eeprom.PageSize - OffsetInByte;
	WorkAddress = (OffsetInByte + Page_Address * eeprom.PageSize);

	AT25SF081_CS_SELECT;

	 BatteryTester_AT25SF081_Spi(AT25SF081_FAST_READ);

	if(eeprom.ID >= W25Q256)
		 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF000000) >> 24);

	 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF0000) >> 16);
	 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF00) >> 8);
	 BatteryTester_AT25SF081_Spi(WorkAddress & 0xFF);

	 BatteryTester_AT25SF081_Spi(0);

	HAL_SPI_Receive(AT25SF081_SPI_PTR, pBuffer, size, 100);

	AT25SF081_CS_UNSELECT;

	for(uint16_t i = 0; i < size; i++)
	{
		if(pBuffer[i] != 0xFF)
		{
			eeprom.Lock = 0;
			return 0;
		}
	}

	eeprom.Lock = 0;
	return 1;
}

//##################################################################################################################
uint8_t  BatteryTester_AT25SF081_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte)
{
	while(eeprom.Lock == 1)
	 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock = 1;

	uint8_t	pBuffer[256] = {0,};
	uint32_t WorkAddress = 0;
	uint16_t s_buf = 256;
	uint16_t size = 0;

	size = eeprom.SectorSize - OffsetInByte;
	WorkAddress = (OffsetInByte + Sector_Address * eeprom.SectorSize);

//char buf2[64] = {0,};
//snprintf(buf2, 64, "SIZE %d \n", size);
//HAL_UART_Transmit(&huart1, (uint8_t*)buf2, strlen(buf2), 100);

	uint16_t cikl = size / 256;
	uint16_t cikl2 = size % 256;
	uint16_t count_cikle = 0;

	if(size <= 256)
	{
		count_cikle = 1;
		//HAL_UART_Transmit(&huart1, (uint8_t*)"1\n", 2, 100);
	}
	else if(cikl2 == 0)
	{
		count_cikle = cikl;
		//HAL_UART_Transmit(&huart1, (uint8_t*)"2\n", 2, 100);
	}
	else
	{
		count_cikle = cikl + 1;
		//HAL_UART_Transmit(&huart1, (uint8_t*)"3\n", 2, 100);
	}


	for(uint16_t i = 0; i < count_cikle; i++)
	{
		AT25SF081_CS_SELECT;
		 BatteryTester_AT25SF081_Spi(AT25SF081_FAST_READ);

		if(eeprom.ID>=W25Q256)
			 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF000000) >> 24);

		 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF0000) >> 16);
		 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF00) >> 8);
		 BatteryTester_AT25SF081_Spi(WorkAddress & 0xFF);

		 BatteryTester_AT25SF081_Spi(0);

		if(size < 256) s_buf = size;

//snprintf(buf2, 64, "RECIV %d %d %d %lu\n", size, s_buf, i, WorkAddress);
//HAL_UART_Transmit(&huart1, (uint8_t*)buf2, strlen(buf2), 100);

		HAL_SPI_Receive(AT25SF081_SPI_PTR, pBuffer, s_buf, 100);

		AT25SF081_CS_UNSELECT;

		for(uint16_t i = 0; i < s_buf; i++)
		{
			if(pBuffer[i] != 0xFF)
			{
				eeprom.Lock = 0;
				return 0;
			}
		}

		size = size - 256;
		WorkAddress = WorkAddress + 256;
	}

	eeprom.Lock = 0;
	return 1;
}

//###################################################################################################################
uint8_t  BatteryTester_AT25SF081_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte)
{
	while(eeprom.Lock == 1)
	 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock = 1;

	uint8_t	pBuffer[256] = {0,};
	uint32_t WorkAddress = 0;
	uint16_t s_buf = 256;
	uint32_t size = 0;

	size = eeprom.BlockSize - OffsetInByte;
	WorkAddress = (OffsetInByte + Block_Address * eeprom.BlockSize);

//char buf2[64] = {0,};
//snprintf(buf2, 64, "SIZEB %lu \n", size);
//HAL_UART_Transmit(&huart1, (uint8_t*)buf2, strlen(buf2), 100);

	uint16_t cikl = size / 256;
	uint16_t cikl2 = size % 256;
	uint16_t count_cikle = 0;

	if(size <= 256)
	{
		count_cikle = 1;
		//HAL_UART_Transmit(&huart1, (uint8_t*)"1\n", 2, 100);
	}
	else if(cikl2 == 0)
	{
		count_cikle = cikl;
		//HAL_UART_Transmit(&huart1, (uint8_t*)"2\n", 2, 100);
	}
	else
	{
		count_cikle = cikl + 1;
		//HAL_UART_Transmit(&huart1, (uint8_t*)"3\n", 2, 100);
	}


	for(uint16_t i = 0; i < count_cikle; i++)
	{
		AT25SF081_CS_SELECT;
		 BatteryTester_AT25SF081_Spi(AT25SF081_FAST_READ);

		if(eeprom.ID>=W25Q256)
			 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF000000) >> 24);

		 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF0000) >> 16);
		 BatteryTester_AT25SF081_Spi((WorkAddress & 0xFF00) >> 8);
		 BatteryTester_AT25SF081_Spi(WorkAddress & 0xFF);

		 BatteryTester_AT25SF081_Spi(0);

		if(size < 256) s_buf = size;

//snprintf(buf2, 64, "RECIVB %lu %d %d %lu\n", size, s_buf, i, WorkAddress);
//HAL_UART_Transmit(&huart1, (uint8_t*)buf2, strlen(buf2), 100);

		HAL_SPI_Receive(AT25SF081_SPI_PTR, pBuffer, s_buf, 100);

		AT25SF081_CS_UNSELECT;

		for(uint16_t i = 0; i < s_buf; i++)
		{
			if(pBuffer[i] != 0xFF)
			{
				eeprom.Lock = 0;
				return 0;
			}
		}

		size = size - 256;
		WorkAddress = WorkAddress + 256;
	}

	eeprom.Lock = 0;
	return 1;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_WriteByte(uint8_t byte, uint32_t addr)
{
	while(eeprom.Lock == 1)
		 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock = 1;

	 BatteryTester_AT25SF081_WaitForReady();
	 BatteryTester_AT25SF081_WriteEnable();

	AT25SF081_CS_SELECT;

	 BatteryTester_AT25SF081_Spi(AT25SF081_PAGE_PROGRAMM);

	if(eeprom.ID >= W25Q256)
		 BatteryTester_AT25SF081_Spi((addr & 0xFF000000) >> 24);

	 BatteryTester_AT25SF081_Spi((addr & 0xFF0000) >> 16);
	 BatteryTester_AT25SF081_Spi((addr & 0xFF00) >> 8);
	 BatteryTester_AT25SF081_Spi(addr & 0xFF);

	 BatteryTester_AT25SF081_Spi(byte);

	AT25SF081_CS_UNSELECT;

	 BatteryTester_AT25SF081_WaitForReady();

	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize)
{
	while(eeprom.Lock == 1)
		 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock = 1;

	if(((NumByteToWrite_up_to_PageSize + OffsetInByte) > eeprom.PageSize) || (NumByteToWrite_up_to_PageSize == 0))
		NumByteToWrite_up_to_PageSize = eeprom.PageSize - OffsetInByte;

	if((OffsetInByte + NumByteToWrite_up_to_PageSize) > eeprom.PageSize)
		NumByteToWrite_up_to_PageSize = eeprom.PageSize - OffsetInByte;


	 BatteryTester_AT25SF081_WaitForReady();

	 BatteryTester_AT25SF081_WriteEnable();

	AT25SF081_CS_SELECT;

	 BatteryTester_AT25SF081_Spi(AT25SF081_PAGE_PROGRAMM);

	Page_Address = (Page_Address * eeprom.PageSize) + OffsetInByte;

	if(eeprom.ID >= W25Q256)
		 BatteryTester_AT25SF081_Spi((Page_Address & 0xFF000000) >> 24);

	 BatteryTester_AT25SF081_Spi((Page_Address & 0xFF0000) >> 16);
	 BatteryTester_AT25SF081_Spi((Page_Address & 0xFF00) >> 8);
	 BatteryTester_AT25SF081_Spi(Page_Address & 0xFF);

	HAL_SPI_Transmit(AT25SF081_SPI_PTR, pBuffer, NumByteToWrite_up_to_PageSize, 100);

	AT25SF081_CS_UNSELECT;

	 BatteryTester_AT25SF081_WaitForReady();

	 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize)
{
	if((NumByteToWrite_up_to_SectorSize > eeprom.SectorSize) || (NumByteToWrite_up_to_SectorSize == 0))
		NumByteToWrite_up_to_SectorSize = eeprom.SectorSize;

	uint32_t StartPage;
	int32_t	BytesToWrite;
	uint32_t LocalOffset;

	if((OffsetInByte + NumByteToWrite_up_to_SectorSize) > eeprom.SectorSize)
		BytesToWrite = eeprom.SectorSize - OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_SectorSize;

	StartPage =  BatteryTester_AT25SF081_SectorToPage(Sector_Address) + (OffsetInByte / eeprom.PageSize);
	LocalOffset = OffsetInByte % eeprom.PageSize;

	do
	{
		 BatteryTester_AT25SF081_WritePage(pBuffer, StartPage, LocalOffset, BytesToWrite);
		StartPage++;

		BytesToWrite -= eeprom.PageSize - LocalOffset;
		//pBuffer += eeprom.PageSize;
		pBuffer += eeprom.PageSize - LocalOffset;
		LocalOffset = 0;
	}
	while(BytesToWrite > 0);
}

//###################################################################################################################
void  BatteryTester_AT25SF081_WriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize)
{
	if((NumByteToWrite_up_to_BlockSize>eeprom.BlockSize)||(NumByteToWrite_up_to_BlockSize == 0))
		NumByteToWrite_up_to_BlockSize=eeprom.BlockSize;

	uint32_t	StartPage;
	int32_t		BytesToWrite;
	uint32_t	LocalOffset;

	if((OffsetInByte+NumByteToWrite_up_to_BlockSize) > eeprom.BlockSize)
		BytesToWrite = eeprom.BlockSize - OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_BlockSize;

	StartPage =  BatteryTester_AT25SF081_BlockToPage(Block_Address)+(OffsetInByte/eeprom.PageSize);

	LocalOffset = OffsetInByte%eeprom.PageSize;

	do
	{
		 BatteryTester_AT25SF081_WritePage(pBuffer,StartPage,LocalOffset,BytesToWrite);
		StartPage++;
		BytesToWrite -= eeprom.PageSize - LocalOffset;
		//pBuffer += eeprom.PageSize;
		pBuffer += eeprom.PageSize - LocalOffset;
		LocalOffset = 0;
	}
	while(BytesToWrite > 0);
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address)
{
	while(eeprom.Lock==1)
		 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock=1;

	AT25SF081_CS_SELECT;
	 BatteryTester_AT25SF081_Spi(AT25SF081_FAST_READ);

	if(eeprom.ID >= W25Q256)
		 BatteryTester_AT25SF081_Spi((Bytes_Address & 0xFF000000) >> 24);

	 BatteryTester_AT25SF081_Spi((Bytes_Address & 0xFF0000) >> 16);
	 BatteryTester_AT25SF081_Spi((Bytes_Address& 0xFF00) >> 8);
	 BatteryTester_AT25SF081_Spi(Bytes_Address & 0xFF);
	 BatteryTester_AT25SF081_Spi(0);

	*pBuffer =  BatteryTester_AT25SF081_Spi(AT25SF081_DUMMY_BYTE);

	AT25SF081_CS_UNSELECT;

	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadBytes(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	while(eeprom.Lock == 1)
	 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock = 1;

	AT25SF081_CS_SELECT;

	 BatteryTester_AT25SF081_Spi(AT25SF081_FAST_READ);

	if(eeprom.ID >= W25Q256)
		 BatteryTester_AT25SF081_Spi((ReadAddr & 0xFF000000) >> 24);

	 BatteryTester_AT25SF081_Spi((ReadAddr & 0xFF0000) >> 16);
	 BatteryTester_AT25SF081_Spi((ReadAddr& 0xFF00) >> 8);
	 BatteryTester_AT25SF081_Spi(ReadAddr & 0xFF);
	 BatteryTester_AT25SF081_Spi(0);

	HAL_SPI_Receive(AT25SF081_SPI_PTR, pBuffer, NumByteToRead, 2000);

	/*uint16_t i = 0;

	while(NumByteToRead > 0U)
	{
		//while(!(AT25SF081_SPI->SR & SPI_SR_TXE));
		AT25SF081_SPI->DR = 0;

		while(!(AT25SF081_SPI->SR & SPI_SR_RXNE));
		pBuffer[i++] = AT25SF081_SPI->DR;
		NumByteToRead--;
	}*/

	AT25SF081_CS_UNSELECT;

	 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize)
{
	while(eeprom.Lock==1)
		 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock = 1;

	if((NumByteToRead_up_to_PageSize>eeprom.PageSize) || (NumByteToRead_up_to_PageSize==0))
		NumByteToRead_up_to_PageSize=eeprom.PageSize;

	if((OffsetInByte+NumByteToRead_up_to_PageSize) > eeprom.PageSize)
		NumByteToRead_up_to_PageSize = eeprom.PageSize - OffsetInByte;

	Page_Address = Page_Address * eeprom.PageSize + OffsetInByte;
	AT25SF081_CS_SELECT;

	 BatteryTester_AT25SF081_Spi(AT25SF081_FAST_READ);

	if(eeprom.ID >= W25Q256)
		 BatteryTester_AT25SF081_Spi((Page_Address & 0xFF000000) >> 24);

	 BatteryTester_AT25SF081_Spi((Page_Address & 0xFF0000) >> 16);
	 BatteryTester_AT25SF081_Spi((Page_Address& 0xFF00) >> 8);
	 BatteryTester_AT25SF081_Spi(Page_Address & 0xFF);

	 BatteryTester_AT25SF081_Spi(0);

	HAL_SPI_Receive(AT25SF081_SPI_PTR, pBuffer, NumByteToRead_up_to_PageSize, 100);

	/*uint16_t i = 0;

	while(NumByteToRead_up_to_PageSize > 0U)
	{
		while(!(AT25SF081_SPI->SR & SPI_SR_TXE));
		AT25SF081_SPI->DR = 0;

		while(!(AT25SF081_SPI->SR & SPI_SR_RXNE));
		pBuffer[i++] = AT25SF081_SPI->DR;
		NumByteToRead_up_to_PageSize--;
	}*/

	AT25SF081_CS_UNSELECT;

	 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock=0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadSector(uint8_t *pBuffer,uint32_t Sector_Address,uint32_t OffsetInByte,uint32_t NumByteToRead_up_to_SectorSize)
{
	if((NumByteToRead_up_to_SectorSize>eeprom.SectorSize) || (NumByteToRead_up_to_SectorSize==0))
		NumByteToRead_up_to_SectorSize=eeprom.SectorSize;

	uint32_t StartPage;
	int32_t	BytesToRead;
	uint32_t LocalOffset;

	if((OffsetInByte + NumByteToRead_up_to_SectorSize) > eeprom.SectorSize)
		BytesToRead = eeprom.SectorSize - OffsetInByte;
	else
		BytesToRead = NumByteToRead_up_to_SectorSize;

	StartPage =  BatteryTester_AT25SF081_SectorToPage(Sector_Address) + (OffsetInByte / eeprom.PageSize);

	LocalOffset = OffsetInByte % eeprom.PageSize;

	do
	{
		 BatteryTester_AT25SF081_ReadPage(pBuffer, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead -= eeprom.PageSize-LocalOffset;
		//pBuffer+=eeprom.PageSize;
		pBuffer += eeprom.PageSize - LocalOffset;
		LocalOffset = 0;
	}
	while(BytesToRead > 0);
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadBlock(uint8_t *pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t	NumByteToRead_up_to_BlockSize)
{
	if((NumByteToRead_up_to_BlockSize > eeprom.BlockSize) || (NumByteToRead_up_to_BlockSize == 0))
		NumByteToRead_up_to_BlockSize = eeprom.BlockSize;

	uint32_t StartPage;
	int32_t	BytesToRead;
	uint32_t LocalOffset;

	if((OffsetInByte+NumByteToRead_up_to_BlockSize) > eeprom.BlockSize)
		BytesToRead = eeprom.BlockSize-OffsetInByte;
	else
		BytesToRead = NumByteToRead_up_to_BlockSize;

	StartPage =  BatteryTester_AT25SF081_BlockToPage(Block_Address) + (OffsetInByte / eeprom.PageSize);

	LocalOffset = OffsetInByte%eeprom.PageSize;

	do
	{
		 BatteryTester_AT25SF081_ReadPage(pBuffer,StartPage,LocalOffset,BytesToRead);
		StartPage++;
		BytesToRead-=eeprom.PageSize-LocalOffset;
		//pBuffer+=eeprom.PageSize;
		pBuffer += eeprom.PageSize - LocalOffset;
		LocalOffset=0;
	}
	while(BytesToRead > 0);
}
//###################################################################################################################

