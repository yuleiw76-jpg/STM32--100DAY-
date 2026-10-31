#ifndef __DMA_H
#define __DMA_H	 
#include "sys.h"

void DMA_init(DMA_Channel_TypeDef* DMA_CHx,u32 padd,u32 madd,u16 d_num);	
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
		 					    
#endif

