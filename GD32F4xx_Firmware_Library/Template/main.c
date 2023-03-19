/*!
    \file    main.c
    \brief   led spark with systick

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

//#include "gd32f4xx.h"
//#include "systick.h"
//#include <stdio.h>
//#include "main.h"
//#include "gd32f450i_eval.h"

///*!
//    \brief    toggle the led every 500ms
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//void led_spark(void)
//{
//    static __IO uint32_t timingdelaylocal = 0U;

//    if(timingdelaylocal) {

//        if(timingdelaylocal < 500U) {
//            gd_eval_led_on(LED1);
//        } else {
//            gd_eval_led_off(LED1);
//        }

//        timingdelaylocal--;
//    } else {
//        timingdelaylocal = 1000U;
//    }
//}

///*!
//    \brief    main function
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//int main(void)
//{

//    gd_eval_led_init(LED1);
//    systick_config();

//    while(1) {
//    }
//}

#include "gd32f4xx.h"
#include "gd32f450i_eval.h"
#include "systick.h"
#include <stdio.h>
#include "exmc_sdram.h"
#include "../../lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "../lv_examples.h"
//#include "lv_demo.h"
#include "touch.h"
#include "lcd.h"
#include "usart0.h"
#include "timer6.h"



__IO uint16_t * my_fb = (__IO uint16_t*) (0xc0000000);

//static void tli_config(void);
static void tli_blend_config(void);
//static void tli_gpio_config(void);
static void lcd_config(void);
//static void lcd_init(void);
static void dma_config(void);
/*!
	brief main function
	param[in] none
	param[out] none
	retval none
*/
int main(void)
{
	/* configure systick */
	systick_config();
	usart0_init();
	timer6_config();
	nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
	exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);
	lcd_config();
	GT1151_Init();
	delay_1ms(50);
	dma_config();
	delay_1ms(1000);
		
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	lv_example_scroll_6();
	while(1)
	{
		lv_task_handler();
	}
}

static void dma_config(void)
{
	dma_multi_data_parameter_struct dma_init_parameter;
	/* peripheral clock enable */
	rcu_periph_clock_enable(RCU_DMA1);
	/* DMA peripheral configure */
	dma_deinit(DMA1,DMA_CH0);
	dma_init_parameter.periph_width = DMA_PERIPH_WIDTH_16BIT;
	dma_init_parameter.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
	dma_init_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_init_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_parameter.memory_burst_width = DMA_MEMORY_BURST_4_BEAT;
	dma_init_parameter.periph_burst_width = DMA_PERIPH_BURST_4_BEAT;
	dma_init_parameter.critical_value = DMA_FIFO_4_WORD;
	dma_init_parameter.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
	dma_init_parameter.direction = DMA_MEMORY_TO_MEMORY;
	dma_init_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_multi_data_mode_init(DMA1,DMA_CH0,&dma_init_parameter);
	nvic_irq_enable(DMA1_Channel0_IRQn, 0, 0);
}

/*!
	brief LCD Configuration
	param[in] none
	param[out] none
	retval none
*/
static void lcd_config(void)
{
    NT35510_Init();
    TLI_ConfigParam(0,0,0,0,0,0);
	  LCD_BLK_ON ;
}




