#include "dma.h"

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	    

//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//padd:外设地址
//madd:存储器地址
//d_num:数据传输量 
void DMA_init(DMA_Channel_TypeDef* DMA_CHx,u32 padd,u32 madd,u16 d_num)
{
	DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
    DMA_DeInit(DMA_CHx);//DMA的某个通道
	DMA1_MEM_LEN=d_num;
	DMA_InitStructure.DMA_PeripheralBaseAddr = padd;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = madd;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设，外设是destination，DST
	DMA_InitStructure.DMA_BufferSize = d_num;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变(INCREASE)
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增(INCREASE)
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	  	
} 
//开启一次DMA传输
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA1_Channel4,DMA1_MEM_LEN);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}	  
