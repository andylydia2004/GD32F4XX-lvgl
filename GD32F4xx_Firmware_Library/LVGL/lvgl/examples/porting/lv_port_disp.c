/**
 * @file lv_port_disp_templ.c
 *
 */

 /*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
//#if 1

///*********************
// *      INCLUDES
// *********************/
//#include "lv_port_disp.h"
//#include "../../lvgl.h"

///*********************
// *      DEFINES
// *********************/
//#define MY_DISP_HOR_RES 800
//#define MY_DISP_VER_RES 480
///**********************
// *      TYPEDEFS
// **********************/

///**********************
// *  STATIC PROTOTYPES
// **********************/
//static void disp_init(void);

//static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

///**********************
// *  STATIC VARIABLES
// **********************/

///**********************
// *      MACROS
// **********************/

///**********************
// *   GLOBAL FUNCTIONS
// **********************/

//void lv_port_disp_init(void)
//{
//    /*-------------------------
//     * Initialize your display
//     * -----------------------*/
//    disp_init();

//    /*-----------------------------
//     * Create a buffer for drawing
//     *----------------------------*/

//    /**
//     * LVGL requires a buffer where it internally draws the widgets.
//     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
//     * The buffer has to be greater than 1 display row
//     *
//     * There are 3 buffering configurations:
//     * 1. Create ONE buffer:
//     *      LVGL will draw the display's content here and writes it to your display
//     *
//     * 2. Create TWO buffer:
//     *      LVGL will draw the display's content to a buffer and writes it your display.
//     *      You should use DMA to write the buffer's content to the display.
//     *      It will enable LVGL to draw the next part of the screen to the other buffer while
//     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
//     *
//     * 3. Double buffering
//     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
//     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
//     *      and you only need to change the frame buffer's address.
//     */

//    /* Example for 1) */
//    static lv_disp_draw_buf_t draw_buf_dsc_1;
//    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

////    /* Example for 2) */
////    static lv_disp_draw_buf_t draw_buf_dsc_2;
////    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /*A buffer for 10 rows*/
////    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                        /*An other buffer for 10 rows*/
////    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

////    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
////    static lv_disp_draw_buf_t draw_buf_dsc_3;
////    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
////    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*An other screen sized buffer*/
////    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

//    /*-----------------------------------
//     * Register the display in LVGL
//     *----------------------------------*/

//    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
//    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

//    /*Set up the functions to access to your display*/

//    /*Set the resolution of the display*/
//    disp_drv.hor_res = MY_DISP_HOR_RES;//480;
//    disp_drv.ver_res = MY_DISP_VER_RES;//320;

//    /*Used to copy the buffer's content to the display*/
//    disp_drv.flush_cb = disp_flush;

//    /*Set a display buffer*/
//    disp_drv.draw_buf = &draw_buf_dsc_1;

//    /*Required for Example 3)*/
//    //disp_drv.full_refresh = 1

//    /* Fill a memory array with a color if you have GPU.
//     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
//     * But if you have a different GPU you can use with this callback.*/
//    //disp_drv.gpu_fill_cb = gpu_fill;

//    /*Finally register the driver*/
//    lv_disp_drv_register(&disp_drv);
//}

///**********************
// *   STATIC FUNCTIONS
// **********************/

///*Initialize your display and the required peripherals.*/
//static void disp_init(void)
//{
//    /*You code here*/
//}

///*Flush the content of the internal buffer the specific area on the display
// *You can use DMA or any hardware acceleration to do this operation in the background but
// *'lv_disp_flush_ready()' has to be called when finished.*/
//static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
//{
//    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

//    int32_t x;
//    int32_t y;
//    for(y = area->y1; y <= area->y2; y++) {
//        for(x = area->x1; x <= area->x2; x++) {
//            /*Put a pixel to the display. For example:*/
//            /*put_px(x, y, *color_p)*/
//            color_p++;
//        }
//    }

//    /*IMPORTANT!!!
//     *Inform the graphics library that you are ready with the flushing*/
//    lv_disp_flush_ready(disp_drv);
//}

///*OPTIONAL: GPU INTERFACE*/

///*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/

//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}


//#else /*Enable this file at the top*/

///*This dummy typedef exists purely to silence -Wpedantic.*/
//typedef int keep_pedantic_happy;
//#endif
#if 1
#include "lv_port_disp.h"
#include "../../lvgl.h"
#include "gd32f4xx.h"
#include "lcd.h"

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
static int32_t x1_flush;
static int32_t y1_flush;
static int32_t x2_flush;
static int32_t y2_fill;
static int32_t y_fill_act;
static const lv_color_t *buf_to_flush;
static lv_disp_t *our_disp = NULL;
static lv_disp_drv_t disp_drv;
extern uint16_t *my_fb;
static uint32_t test1,test2;
#define LCD_FRAME_BUF_ADDR 			0XC0000000
#define TLI_LCD_FRAMEBUF_SIZE 	(800*480*2)
#define COLOR_BUF_SIZE					(LV_HOR_RES_MAX*LV_VER_RES_MAX)

static lv_color_t color_buf2[COLOR_BUF_SIZE]__attribute__((section(".ARM.__at_0xC00BB800")));


/*!
		brief dma transfer data
		param[in] src_addr: source address
		param[in] dst_addr: destinati on address
		param[in] datalength: data length
		param[out] none
		retval none
*/
void dma_transfer(uint32_t src_addr, uint32_t dst_addr, uint32_t datalength)
{
	dma_periph_address_config(DMA1, DMA_CH0, src_addr);
	dma_memory_address_config(DMA1, DMA_CH0, DMA_MEMORY_0, dst_addr);
	dma_transfer_number_config(DMA1, DMA_CH0, datalength);
	dma_interrupt_disable(DMA1, DMA_CH0, DMA_CHXCTL_HTFIE);
	dma_interrupt_enable(DMA1, DMA_CH0, DMA_CHXCTL_FTFIE);
	dma_channel_enable(DMA1, DMA_CH0);
}
void lv_port_disp_init(void)
{
	static lv_disp_draw_buf_t buf;
	lv_disp_draw_buf_init(&buf, color_buf2, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);
	lv_disp_drv_init(&disp_drv);
	disp_drv.draw_buf = &buf;
	disp_drv.flush_cb = disp_flush;
	disp_drv.hor_res = LV_HOR_RES_MAX;
	disp_drv.ver_res = LV_VER_RES_MAX;
	lv_disp_drv_register(&disp_drv);
}
/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this op eration in the background but
 * 'lv_disp_flush_ready()' has to be called when finished*/

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
	/*Return if the area is out the screen*/
	if(area->x2 < 0){
		return;
	}
	if(area->y2 < 0){
		return;
	}
	if(area->x1 > LV_HOR_RES_MAX - 1) {
		return;
	}
	if(area->y1 > LV_VER_RES_MAX - 1) {
		return;
	}
	/*Truncate the area to the screen */
	int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
	int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
	int32_t act_x2 = area->x2 > LV_HOR_RES_MAX - 1 ? LV_HOR_RES_MAX - 1 : area->x2;
	int32_t act_y2 = area->y2 > LV_VER_RES_MAX - 1 ? LV_VER_RES_MAX - 1 : area->y2;
	
	x1_flush = act_x1;
	y1_flush = act_y1;
	x2_flush = act_x2;
	y2_fill = act_y2;
	y_fill_act = act_y1;
	buf_to_flush = color_p;
	dma_transfer((uint32_t)buf_to_flush, (uint32_t)&my_fb[y_fill_act * LV_HOR_RES_MAX + x1_flush],(x2_flush - x1_flush + 1));
}
/*this function handles DMA1 Handler exception*/
void DMA1_Channel0_IRQHandler(void)
{
	if(dma_interrupt_flag_get(DMA1, DMA_CH0, DMA_INT_FLAG_FTF)) 
	{
		dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);
		y_fill_act ++;
		if(y_fill_act > y2_fill)
		{
			lv_disp_flush_ready(&disp_drv);
		} 
		else
		{
			buf_to_flush += x2_flush - x1_flush + 1;
			dma_transfer((uint32_t)buf_to_flush, (uint32_t)&my_fb[y_fill_act * LV_HOR_RES_MAX + x1_flush],(x2_flush - x1_flush + 1));
		}
	}
}




#endif

