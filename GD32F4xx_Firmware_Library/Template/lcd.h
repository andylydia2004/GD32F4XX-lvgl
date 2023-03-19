#ifndef __LCD_H
#define __LCD_H
#include "gd32f4xx.h"
#include "systick.h"


#define LCD_SDA_RCU  			RCU_GPIOE   // SDA
#define LCD_SDA_PORT  		    GPIOE
#define LCD_SDA_PIN    		    GPIO_PIN_6//GPIO_PIN_5  //v0.0.1新屏幕6，老屏幕5
#define LCD_SDA_WRITE(x)	    gpio_bit_write(LCD_SDA_PORT,LCD_SDA_PIN,x)
#define LCD_SCL_RCU  			RCU_GPIOE   // SCL
#define LCD_SCL_PORT  		    GPIOE
#define LCD_SCL_PIN    		    GPIO_PIN_2
#define LCD_SCL_WRITE(x)	    gpio_bit_write(LCD_SCL_PORT,LCD_SCL_PIN,x)
#define LCD_CS_RCU  			RCU_GPIOE   // CS
#define LCD_CS_PORT  		    GPIOE
#define LCD_CS_PIN    		    GPIO_PIN_4
#define LCD_CS_WRITE(x)	        gpio_bit_write(LCD_CS_PORT,LCD_CS_PIN,x)
#define LCD_RST_RCU  			RCU_GPIOD   // RST
#define LCD_RST_PORT  		    GPIOD
#define LCD_RST_PIN    		    GPIO_PIN_12
#define LCD_RST_WRITE(x)	    gpio_bit_write(LCD_RST_PORT,LCD_RST_PIN,x)
#define LCD_BLK_ON              gpio_bit_set(GPIOD,GPIO_PIN_13)	// PD13
#define LCD_BLK_OFF             gpio_bit_reset(GPIOD,GPIO_PIN_13)	// PD13
#define LCD_CS_Clr()            gpio_bit_reset(GPIOE,GPIO_PIN_4)	// PD13
#define LCD_CS_Set()            gpio_bit_set(GPIOE,GPIO_PIN_4)	// PD13
#define LCD_SDI_RCU  			RCU_GPIOC   // SDI
#define LCD_SDI_PORT  		    GPIOC
#define LCD_SDI_PIN    		    GPIO_PIN_1
#define LCD_SDI_WRITE(x)	    gpio_bit_write(LCD_SDI_PORT,LCD_SDI_PIN,x)


#define HORIZONTAL_SYNCHRONOUS_PULSE  10
#define HORIZONTAL_BACK_PORCH         150
#define ACTIVE_WIDTH                  480
#define HORIZONTAL_FRONT_PORCH        40

#define VERTICAL_SYNCHRONOUS_PULSE    10
#define VERTICAL_BACK_PORCH           140
#define ACTIVE_HEIGHT                 800
#define VERTICAL_FRONT_PORCH          15


extern uint16_t *ltdc_framebuf[2];              
extern uint16_t ltdc_lcd_framebuf0[800][480];              
extern uint16_t ltdc_lcd_framebuf1[800][480]; 




//画笔颜色
#define WHITE        0xFFFF
#define BLACK        0x0000	  
#define BLUE         0x001F  
#define BRED         0XF81F
#define GRED         0XFFE0
#define GBLUE        0X07FF
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN        0XBC40 //棕色
#define BRRED        0XFC07 //棕红色
#define GRAY         0X8430 //灰色
//GUI颜色
#define DARKBLUE         0X01CF	//深蓝色
#define LIGHTBLUE        0X7D7C	//浅蓝色  
#define GRAYBLUE         0X5458 //灰蓝色
//以上三色为PANEL的颜色 
#define LIGHTGREEN       0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY            0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


////扫描方向定义
//#define L2R_U2D  0 //从左到右,从上到下
//#define L2R_D2U  1 //从左到右,从下到上
//#define R2L_U2D  2 //从右到左,从上到下
//#define R2L_D2U  3 //从右到左,从下到上
//#define U2D_L2R  4 //从上到下,从左到右
//#define U2D_R2L  5 //从上到下,从右到左
//#define D2U_L2R  6 //从下到上,从左到右
//#define D2U_R2L  7 //从下到上,从右到左	 
//#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向


//LCD的画笔颜色和背景色	   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色



//////////
void NT35510_Init(void);
void TLI_ConfigParam( 
int b_HORIZONTAL_SYNCHRONOUS_PULSE,
int b_HORIZONTAL_BACK_PORCH       ,
int b_HORIZONTAL_FRONT_PORCH      ,
int b_VERTICAL_SYNCHRONOUS_PULSE  ,
int b_VERTICAL_BACK_PORCH         ,
int b_VERTICAL_FRONT_PORCH        );
//////////////////
//void LCD_Init(void);//屏幕初始化
//////////////////
void LTDC_Draw_Point(uint8_t layer, uint16_t x, uint16_t y, uint32_t color);/////画点
//void LCD_Clear(uint8_t layer,uint16_t  color);//清屏函数   
//void LCD_ShowChar(uint8_t layer,uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);////在指定位置显示一个字符
//void LCD_ShowString(uint8_t layer,uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p);//显示字符串

#endif  




