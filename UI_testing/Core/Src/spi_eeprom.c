/*
 * spi_eeprom.c
 *
 *  Created on: May 20, 2024
 *      Author: Nima Askari
 */
#include "spi_eeprom.h"
#include "stm32f0xx_hal.h"

#define AT25SF081_DUMMY_BYTE        0xA5
#define BatteryTester_AT25SF081_postAddress BatteryTester_AT25SF081_postCommand

sAT25SF081_t eeprom;
static BatteryTester_AT25SF081_HardwareTransmitReceiveCallback_t g_transmitReceiveCallback = 0;
static BatteryTester_AT25SF081_HardwareControlCallback_t g_selectChipCallback = 0;
static BatteryTester_AT25SF081_HardwareTransferCallback_t g_receiveCallback = 0;
static BatteryTester_AT25SF081_HardwareTransferCallback_t g_transmitCallback = 0;

#if (_AT25SF081_USE_FREERTOS == 1)
#define	 BatteryTester_AT25SF081_Delay(delay)		osDelay(delay)
#include "cmsis_os.h"
#else
#define	 BatteryTester_AT25SF081_Delay(delay)		HAL_Delay(delay)
#endif



//###################################################################################################################
/*
 * @new
 */
uint8_t BatteryTester_AT25SF081_transmitReceiveSpi(uint8_t data){
	uint8_t ret = 0xFF;
	if(g_transmitReceiveCallback){
		BatteryTester_AT25SF081_selectChip(SELECT);
		ret = g_transmitReceiveCallback(data);
		BatteryTester_AT25SF081_selectChip(UNSELECT);
	}
	return ret;
}

uint8_t BatteryTester_AT25SF081_postCommand(uint8_t data){
	if(g_transmitReceiveCallback){
		return g_transmitReceiveCallback(data);
	}
	return 0xFF;
}
//###################################################################################################################
/************************************************************/
void BatteryTester_AT25SF081_selectChip(eCSAdapter_t select){
	if(g_selectChipCallback){
		g_selectChipCallback(select);
	}
}
//###################################################################################################################
//###################################################################################################################
/*
 * @denger: blocking wait
 */
void  BatteryTester_AT25SF081_WriteEnable(void)
{
	BatteryTester_AT25SF081_transmitReceiveSpi(AT25SF081_WRITE_ENABLE);
}

//###################################################################################################################
/*
 * @denger: blocking wait
 */
void  BatteryTester_AT25SF081_WriteDisable(void)
{
	BatteryTester_AT25SF081_transmitReceiveSpi(AT25SF081_WRITE_DISABLE);
	//BatteryTester_AT25SF081_Delay(1);
}

//###################################################################################################################

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
	do{
		eeprom.StatusRegister1 =  BatteryTester_AT25SF081_postCommand(AT25SF081_DUMMY_BYTE);
		BatteryTester_AT25SF081_Delay(1);
	}
	while((eeprom.StatusRegister1 & 0x01) == 0x01);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
}

//###################################################################################################################

/********************************************************************/
void BatteryTester_AT25SF081_initDecorator(
		BatteryTester_AT25SF081_HardwareTransmitReceiveCallback_t transmitReceiveCallback,
		BatteryTester_AT25SF081_HardwareControlCallback_t selectChipCallback,
		BatteryTester_AT25SF081_HardwareTransferCallback_t receiveCallback,
		BatteryTester_AT25SF081_HardwareTransferCallback_t transmitCallback){
	g_transmitReceiveCallback = transmitReceiveCallback;
	g_selectChipCallback = selectChipCallback;
	g_receiveCallback = receiveCallback;
	g_transmitCallback = transmitCallback;
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

/*
 * @denger: blocking wait
 */
void BatteryTester_AT25SF081_receiveData(
		uint8_t* pBuffer,
		uint16_t size,
		uint32_t timeout){
	if(g_receiveCallback){
		g_receiveCallback(pBuffer, size, timeout);
	}
}

void BatteryTester_AT25SF081_transmitData(
		uint8_t* pBuffer,
		uint16_t size,
		uint32_t timeout){
	if(g_transmitCallback){
		g_transmitCallback(pBuffer, size, timeout);
	}
}
//###################################################################################################################
/*
 * @denger: blocking wait
 */
void  BatteryTester_AT25SF081_EraseChip(void)
{
	while(eeprom.Lock == 1){
		BatteryTester_AT25SF081_Delay(1);
	}
	eeprom.Lock = 1;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_WriteEnable();
	BatteryTester_AT25SF081_transmitReceiveSpi(AT25SF081_CHIP_ERASE);
	eeprom.Lock = 0;
}

//###################################################################################################################
/*
 * @denger: blocking wait
 */
void  BatteryTester_AT25SF081_EraseSector(uint32_t SectorAddr)
{
	while(eeprom.Lock == 1)
		 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 1;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_WriteEnable();
	SectorAddr *= eeprom.SectorSize;
	BatteryTester_AT25SF081_selectChip(SELECT);
	BatteryTester_AT25SF081_postCommand(AT25SF081_SECTOR_ERASE);
	BatteryTester_AT25SF081_postAddress((SectorAddr & 0xFF0000) >> 16);
	BatteryTester_AT25SF081_postAddress((SectorAddr & 0xFF00) >> 8);
	BatteryTester_AT25SF081_postAddress(SectorAddr & 0xFF);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
	eeprom.Lock = 0;
}

//###################################################################################################################
/*
 * @denger: blocking wait
 */
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
	BatteryTester_AT25SF081_postAddress((BlockAddr & 0xFF0000) >> 16);
	BatteryTester_AT25SF081_postAddress((BlockAddr & 0xFF00) >> 8);
	BatteryTester_AT25SF081_postAddress(BlockAddr & 0xFF);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
	eeprom.Lock = 0;
}

//###################################################################################################################
uint32_t  BatteryTester_AT25SF081_PageToSector(uint32_t PageAddress)
{
	return((PageAddress * eeprom.PageSize) / eeprom.SectorSize);
}

//###################################################################################################################
uint32_t  BatteryTester_AT25SF081_PageToBlock(uint32_t PageAddress)
{
	return((PageAddress * eeprom.PageSize) / eeprom.BlockSize);
}

//###################################################################################################################
uint32_t  BatteryTester_AT25SF081_SectorToBlock(uint32_t SectorAddress)
{
	return((SectorAddress * eeprom.SectorSize) / eeprom.BlockSize);
}

//###################################################################################################################
uint32_t  BatteryTester_AT25SF081_SectorToPage(uint32_t SectorAddress)
{
	return(SectorAddress * eeprom.SectorSize) / eeprom.PageSize;
}

//###################################################################################################################
uint32_t  BatteryTester_AT25SF081_BlockToPage(uint32_t BlockAddress)
{
	return (BlockAddress * eeprom.BlockSize) / eeprom.PageSize;
}

//###################################################################################################################
/*
 * @denger: blocking wait
 */
uint8_t  BatteryTester_AT25SF081_IsEmptyPage(
		uint32_t Page_Address,
		uint32_t OffsetInByte)
{
	while(eeprom.Lock == 1)
	 BatteryTester_AT25SF081_Delay(1);

	eeprom.Lock = 1;
	BatteryTester_AT25SF081_WaitForReady();
	uint8_t	pBuffer[256] = {0,};
	uint32_t WorkAddress = OffsetInByte + Page_Address * eeprom.PageSize;
	uint16_t size = eeprom.PageSize - OffsetInByte;
	BatteryTester_AT25SF081_readArrayFastCommand(WorkAddress, pBuffer, size,	100);
	uint8_t ret = BatteryTester_AT25SF081_isEmptyBuffer(pBuffer, size);
	eeprom.Lock = 0;
	return ret;
}

inline void BatteryTester_AT25SF081_readArrayFastCommand(
		uint32_t address,
		uint8_t* pBuffer,
		uint16_t size,
		uint32_t timeout){
	if(!pBuffer || size == 0 || size > 256){
		return;
	}
	BatteryTester_AT25SF081_selectChip(SELECT);
	BatteryTester_AT25SF081_postCommand(AT25SF081_FAST_READ);
	BatteryTester_AT25SF081_postAddress((address & 0xFF0000) >> 16);
	BatteryTester_AT25SF081_postAddress((address & 0xFF00) >> 8);
	BatteryTester_AT25SF081_postAddress(address & 0xFF);
	BatteryTester_AT25SF081_postCommand(AT25SF081_DUMMY_BYTE);
	BatteryTester_AT25SF081_receiveData(pBuffer, size, timeout);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
}

inline uint8_t BatteryTester_AT25SF081_isEmptyBuffer(
		uint8_t* pBuffer,
		uint16_t size){
	if(!pBuffer || size == 0){
		return 0;
	}
	for(uint16_t i = 0; i < size; i++)
	{
		if(pBuffer[i] != 0xFF)
		{
			return 0;
		}
	}
	return 1;
}
//##################################################################################################################
uint8_t  BatteryTester_AT25SF081_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte)
{
	if(OffsetInByte >= eeprom.SectorSize){
		return 0;
	}
	uint32_t StartPage = BatteryTester_AT25SF081_SectorToPage(Sector_Address) +
				(OffsetInByte / eeprom.PageSize);
	int BytesToCheck = eeprom.SectorSize - OffsetInByte;
	uint32_t LocalOffset = OffsetInByte % eeprom.PageSize;
	while(BytesToCheck > 0)
	{
		if(!BatteryTester_AT25SF081_IsEmptyPage(StartPage, LocalOffset)){
			return 0;
		}
		StartPage++;
		BytesToCheck -= eeprom.PageSize - LocalOffset;
		LocalOffset = 0;
	}
	return 1;
}

//###################################################################################################################
uint8_t  BatteryTester_AT25SF081_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte)
{
	if(OffsetInByte > eeprom.BlockSize){
		return 0;
	}
	uint32_t StartPage = BatteryTester_AT25SF081_BlockToPage(Block_Address)+
			(OffsetInByte/eeprom.PageSize);
	int BytesToCheck = eeprom.BlockSize - OffsetInByte;
	uint32_t LocalOffset = OffsetInByte % eeprom.PageSize;
	while(BytesToCheck > 0)
	{
		if(!BatteryTester_AT25SF081_IsEmptyPage(StartPage, LocalOffset)){
			return 0;
		}
		StartPage++;
		BytesToCheck -= eeprom.PageSize - LocalOffset;
		LocalOffset = 0;
	}
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
	BatteryTester_AT25SF081_selectChip(SELECT);
	BatteryTester_AT25SF081_postCommand(AT25SF081_PAGE_PROGRAMM);
	BatteryTester_AT25SF081_postAddress((addr & 0xFF0000) >> 16);
	BatteryTester_AT25SF081_postAddress((addr & 0xFF00) >> 8);
	BatteryTester_AT25SF081_postAddress(addr & 0xFF);
	BatteryTester_AT25SF081_postCommand(byte);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_WritePage(
		uint8_t* pBuffer,
		uint32_t Page_Address,
		uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_PageSize)
{
	while(eeprom.Lock == 1)
		 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 1;
	if(((NumByteToWrite_up_to_PageSize + OffsetInByte) > eeprom.PageSize) ||
			(NumByteToWrite_up_to_PageSize == 0)){
		NumByteToWrite_up_to_PageSize = eeprom.PageSize - OffsetInByte;
	}
	Page_Address = (Page_Address * eeprom.PageSize) + OffsetInByte;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_WriteEnable();
	BatteryTester_AT25SF081_selectChip(SELECT);
	BatteryTester_AT25SF081_postCommand(AT25SF081_PAGE_PROGRAMM);
	BatteryTester_AT25SF081_postAddress((Page_Address & 0xFF0000) >> 16);
	BatteryTester_AT25SF081_postAddress((Page_Address & 0xFF00) >> 8);
	BatteryTester_AT25SF081_postAddress(Page_Address & 0xFF);
	BatteryTester_AT25SF081_transmitData(pBuffer, NumByteToWrite_up_to_PageSize, 100);
	BatteryTester_AT25SF081_selectChip(UNSELECT);
	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_WriteSector(
		uint8_t* pBuffer,
		uint32_t Sector_Address,
		uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_SectorSize)
{
	if(OffsetInByte > eeprom.SectorSize){
		return;
	}
	if((NumByteToWrite_up_to_SectorSize + OffsetInByte > eeprom.SectorSize)
			|| (NumByteToWrite_up_to_SectorSize == 0)){
		NumByteToWrite_up_to_SectorSize = eeprom.SectorSize - OffsetInByte;
	}
	uint32_t StartPage = BatteryTester_AT25SF081_SectorToPage(Sector_Address) +
			(OffsetInByte / eeprom.PageSize);
	int BytesToWrite = NumByteToWrite_up_to_SectorSize;
	uint32_t LocalOffset = OffsetInByte % eeprom.PageSize;
	while(BytesToWrite > 0)
	{
		BatteryTester_AT25SF081_WritePage(pBuffer, StartPage, LocalOffset, BytesToWrite);
		StartPage++;
		BytesToWrite -= eeprom.PageSize - LocalOffset;
		pBuffer += eeprom.PageSize - LocalOffset;
		LocalOffset = 0;
	}
}

//###################################################################################################################
void  BatteryTester_AT25SF081_WriteBlock(
		uint8_t* pBuffer,
		uint32_t Block_Address,
		uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_BlockSize)
{
	if(OffsetInByte > eeprom.BlockSize){
		return;
	}
	if((NumByteToWrite_up_to_BlockSize + OffsetInByte > eeprom.BlockSize)||
			(NumByteToWrite_up_to_BlockSize == 0)){
		NumByteToWrite_up_to_BlockSize = eeprom.BlockSize - OffsetInByte;
	}
	uint32_t StartPage = BatteryTester_AT25SF081_BlockToPage(Block_Address)+
			(OffsetInByte/eeprom.PageSize);
	int BytesToWrite = NumByteToWrite_up_to_BlockSize;
	uint32_t LocalOffset = OffsetInByte % eeprom.PageSize;
	while(BytesToWrite > 0)
	{
		BatteryTester_AT25SF081_WritePage(pBuffer, StartPage, LocalOffset, BytesToWrite);
		StartPage++;
		BytesToWrite -= eeprom.PageSize - LocalOffset;
		pBuffer += eeprom.PageSize - LocalOffset;
		LocalOffset = 0;
	}
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadByte(uint8_t* pBuffer, uint32_t Bytes_Address)
{
	while(eeprom.Lock==1)
		 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock=1;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_readArrayFastCommand(Bytes_Address, pBuffer, 1, 100);
	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadBytes(
		uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	while(eeprom.Lock == 1)
		BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 1;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_readArrayFastCommand(ReadAddr, pBuffer, NumByteToRead, 2000);

	eeprom.Lock = 0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadPage(
		uint8_t* pBuffer,
		uint32_t Page_Address,
		uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_PageSize)
{
	if(OffsetInByte > eeprom.PageSize){
		return;
	}
	while(eeprom.Lock==1)
		 BatteryTester_AT25SF081_Delay(1);
	eeprom.Lock = 1;

	if((NumByteToRead_up_to_PageSize + OffsetInByte > eeprom.PageSize) ||
			(NumByteToRead_up_to_PageSize==0)){
		NumByteToRead_up_to_PageSize = eeprom.PageSize - OffsetInByte;
	}
	Page_Address = Page_Address * eeprom.PageSize + OffsetInByte;
	BatteryTester_AT25SF081_WaitForReady();
	BatteryTester_AT25SF081_readArrayFastCommand(
			Page_Address, pBuffer, NumByteToRead_up_to_PageSize, 100);
	eeprom.Lock=0;
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadSector(
		uint8_t* pBuffer,
		uint32_t Sector_Address,
		uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_SectorSize)
{
	if(OffsetInByte > eeprom.SectorSize){
		return;
	}
	if((NumByteToRead_up_to_SectorSize + OffsetInByte > eeprom.SectorSize) ||
			(NumByteToRead_up_to_SectorSize == 0)){
		NumByteToRead_up_to_SectorSize = eeprom.SectorSize - OffsetInByte;
	}
	uint32_t StartPage = BatteryTester_AT25SF081_SectorToPage(Sector_Address) +
			(OffsetInByte / eeprom.PageSize);
	int BytesToRead = NumByteToRead_up_to_SectorSize;
	uint32_t LocalOffset = OffsetInByte % eeprom.PageSize;
	while(BytesToRead > 0)
	{
		BatteryTester_AT25SF081_ReadPage(pBuffer, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead -= eeprom.PageSize-LocalOffset;
		pBuffer += eeprom.PageSize - LocalOffset;
		LocalOffset = 0;
	}
}

//###################################################################################################################
void  BatteryTester_AT25SF081_ReadBlock(
		uint8_t* pBuffer,
		uint32_t Block_Address,
		uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_BlockSize)
{
	if(OffsetInByte > eeprom.BlockSize){
		return;
	}
	if((NumByteToRead_up_to_BlockSize + OffsetInByte > eeprom.BlockSize) ||
			(NumByteToRead_up_to_BlockSize == 0)){
		NumByteToRead_up_to_BlockSize = eeprom.BlockSize - OffsetInByte;
	}
	uint32_t StartPage = BatteryTester_AT25SF081_BlockToPage(Block_Address) +
			(OffsetInByte / eeprom.PageSize);
	int	BytesToRead = NumByteToRead_up_to_BlockSize;
	uint32_t LocalOffset = OffsetInByte % eeprom.PageSize;

	while(BytesToRead > 0)
	{
		BatteryTester_AT25SF081_ReadPage(pBuffer, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead-=eeprom.PageSize-LocalOffset;
		pBuffer += eeprom.PageSize - LocalOffset;
		LocalOffset=0;
	}
}
//###################################################################################################################

