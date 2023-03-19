#include "lcd.h"
#include "stdio.h"
#include "string.h" 
//#include "font.h" 

//LCD的画笔颜色和背景色	   
uint16_t  POINT_COLOR=0x0000;	//画笔颜色
uint16_t  BACK_COLOR=0xFFFF;  //背景色 

extern uint16_t *my_fb;
static void ipa_config(void);
typedef struct
{
  uint32_t n;
  uint32_t p;
  uint32_t r;
  uint32_t v;
} TLI_CLK_t;

TLI_CLK_t tli_clk;

// LCD对应外部SRAM地址
//#define LCD_FRAME_BUF_ADDR 0XC0000000
//uint16_t *ltdc_framebuf[2];                                                                    //
//uint16_t ltdc_lcd_framebuf0[800][480] __attribute__((at(LCD_FRAME_BUF_ADDR)));                 //图层0
//uint16_t ltdc_lcd_framebuf1[800][480] __attribute__((at(LCD_FRAME_BUF_ADDR + 800 * 480 * 2))); //图层1
//__IO uint16_t * ltdc_framebuf = (__IO uint16_t*) (0xc0000000);





void mDelayuS(uint16_t us)
{
    while(us--)
    {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
}
void Write_3Spi_Byte(uint8_t CMD)
{
	uint8_t i;
	LCD_SCL_WRITE(0);
	// mDelayuS(10);
	for (i = 0; i < 8; i++)
	{
		LCD_SCL_WRITE(0);

		if ((CMD & 0x80) != 0)
			LCD_SDA_WRITE(1);
		else
			LCD_SDA_WRITE(0);

		LCD_SCL_WRITE(0);
		mDelayuS(1);
		LCD_SCL_WRITE(1);
		mDelayuS(1);
		CMD <<= 1;
	}
}

void Write_3Spi_cmd(uint16_t addr)
{
	LCD_CS_WRITE(0);
	mDelayuS(5);
	Write_3Spi_Byte(0x20);
	Write_3Spi_Byte(addr >> 8);

	Write_3Spi_Byte(0x00);
	Write_3Spi_Byte(addr);
	mDelayuS(5);
	LCD_CS_WRITE(1);
}

void Write_3Spi_data(uint8_t value)

{
	LCD_CS_WRITE(0);
	mDelayuS(5);
	Write_3Spi_Byte(0x40);
	Write_3Spi_Byte(value);
	mDelayuS(5);
	LCD_CS_WRITE(1);
}

void NT35510_Init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(LCD_SDA_RCU); // SDA
    gpio_mode_set(LCD_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LCD_SDA_PIN);
    gpio_output_options_set(LCD_SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_SDA_PIN);
    /* enable the led clock */
    rcu_periph_clock_enable(LCD_SCL_RCU); // SCL
    gpio_mode_set(LCD_SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LCD_SCL_PIN);
    gpio_output_options_set(LCD_SCL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_SCL_PIN);
    /* enable the led clock */
    rcu_periph_clock_enable(LCD_CS_RCU); // CS
    gpio_mode_set(LCD_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LCD_CS_PIN);
    gpio_output_options_set(LCD_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_CS_PIN);
    /* enable the led clock */
    rcu_periph_clock_enable(LCD_RST_RCU); // RST
    gpio_mode_set(LCD_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LCD_RST_PIN);
    gpio_output_options_set(LCD_RST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_RST_PIN);
    /* 背光 PD13 */
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_13);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    
    LCD_BLK_OFF;
    LCD_SCL_WRITE(1);
    LCD_CS_WRITE(1);
    LCD_SDA_WRITE(1);
    delay_1ms(200);
    LCD_RST_WRITE(1);
    delay_1ms(10);
    LCD_RST_WRITE(0);
    delay_1ms(50);
    LCD_RST_WRITE(1);
    delay_1ms(150);
    
	Write_3Spi_cmd(0xF000);
	Write_3Spi_data(0x55);
	Write_3Spi_cmd(0xF001);
	Write_3Spi_data(0xAA);
	Write_3Spi_cmd(0xF002);
	Write_3Spi_data(0x52);
	Write_3Spi_cmd(0xF003);
	Write_3Spi_data(0x08);
	Write_3Spi_cmd(0xF004);
	Write_3Spi_data(0x01); // Page 1

	//*************AVDD Set AVDD 5.2V*************//
	Write_3Spi_cmd(0xB000);
	Write_3Spi_data(0x0D);
	Write_3Spi_cmd(0xB001);
	Write_3Spi_data(0x0D);
	Write_3Spi_cmd(0xB002);
	Write_3Spi_data(0x0D);

	//************AVDD ratio****************//
	Write_3Spi_cmd(0xB600);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xB601);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xB602);
	Write_3Spi_data(0x34);

	//************AVEE  -5.2V****************//
	Write_3Spi_cmd(0xB100);
	Write_3Spi_data(0x0D);
	Write_3Spi_cmd(0xB101);
	Write_3Spi_data(0x0D);
	Write_3Spi_cmd(0xB102);
	Write_3Spi_data(0x0D);

	//***********AVEE ratio*************//
	Write_3Spi_cmd(0xB700);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xB701);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xB702);
	Write_3Spi_data(0x34);

	//***********VCL  -2.5V*************//
	Write_3Spi_cmd(0xB200);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xB201);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xB202);
	Write_3Spi_data(0x00);

	//**************VCL ratio*****************//
	Write_3Spi_cmd(0xB800);
	Write_3Spi_data(0x24);
	Write_3Spi_cmd(0xB801);
	Write_3Spi_data(0x24);
	Write_3Spi_cmd(0xB802);
	Write_3Spi_data(0x24);

	//*************VGH 15V  (Free pump)*********//
	Write_3Spi_cmd(0xBF00);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xB300);
	Write_3Spi_data(0x0F);
	Write_3Spi_cmd(0xB301);
	Write_3Spi_data(0x0F);
	Write_3Spi_cmd(0xB302);
	Write_3Spi_data(0x0F);

	//*************VGH ratio*****************//
	Write_3Spi_cmd(0xB900);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xB901);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xB902);
	Write_3Spi_data(0x34);

	//***************VGL_REG -10V**************//
	Write_3Spi_cmd(0xB500);
	Write_3Spi_data(0x08);
	Write_3Spi_cmd(0xB501);
	Write_3Spi_data(0x08);
	Write_3Spi_cmd(0xB502);
	Write_3Spi_data(0x08);

	Write_3Spi_cmd(0xC200);
	Write_3Spi_data(0x03);

	//*****************VGLX ratio***************//
	Write_3Spi_cmd(0xBA00);
	Write_3Spi_data(0x24);
	Write_3Spi_cmd(0xBA01);
	Write_3Spi_data(0x24);
	Write_3Spi_cmd(0xBA02);
	Write_3Spi_data(0x24);

	//*************VGMP/VGSP 4.5V/0V*************//
	Write_3Spi_cmd(0xBC00);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xBC01);
	Write_3Spi_data(0x78);
	Write_3Spi_cmd(0xBC02);
	Write_3Spi_data(0x00);

	//************VGMN/VGSN -4.5V/0V****************//
	Write_3Spi_cmd(0xBD00);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xBD01);
	Write_3Spi_data(0x78);
	Write_3Spi_cmd(0xBD02);
	Write_3Spi_data(0x00);

	//************VCOM  -1.25V****************//
	Write_3Spi_cmd(0xBE00);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xBE01);
	Write_3Spi_data(0x64); // 67--------------

	//************Gamma Setting******************//
	Write_3Spi_cmd(0xD100);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD101);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD102);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD103);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD104);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD105);
	Write_3Spi_data(0x3a);
	Write_3Spi_cmd(0xD106);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD107);
	Write_3Spi_data(0x4A);
	Write_3Spi_cmd(0xD108);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD109);
	Write_3Spi_data(0x5c);
	Write_3Spi_cmd(0xD10A);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD10B);
	Write_3Spi_data(0x81);
	Write_3Spi_cmd(0xD10C);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD10D);
	Write_3Spi_data(0xA6);
	Write_3Spi_cmd(0xD10E);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD10F);
	Write_3Spi_data(0xE5);
	Write_3Spi_cmd(0xD110);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD111);
	Write_3Spi_data(0x13);
	Write_3Spi_cmd(0xD112);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD113);
	Write_3Spi_data(0x54);
	Write_3Spi_cmd(0xD114);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD115);
	Write_3Spi_data(0x82);
	Write_3Spi_cmd(0xD116);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD117);
	Write_3Spi_data(0xCA);
	Write_3Spi_cmd(0xD118);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD119);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD11A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD11B);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD11C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD11D);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD11E);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD11F);
	Write_3Spi_data(0x67);
	Write_3Spi_cmd(0xD120);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD121);
	Write_3Spi_data(0x84);
	Write_3Spi_cmd(0xD122);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD123);
	Write_3Spi_data(0xA4);
	Write_3Spi_cmd(0xD124);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD125);
	Write_3Spi_data(0xB7);
	Write_3Spi_cmd(0xD126);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD127);
	Write_3Spi_data(0xCF);
	Write_3Spi_cmd(0xD128);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD129);
	Write_3Spi_data(0xDE);
	Write_3Spi_cmd(0xD12A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD12B);
	Write_3Spi_data(0xF2);
	Write_3Spi_cmd(0xD12C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD12D);
	Write_3Spi_data(0xFE);
	Write_3Spi_cmd(0xD12E);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD12F);
	Write_3Spi_data(0x10);
	Write_3Spi_cmd(0xD130);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD131);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD132);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD133);
	Write_3Spi_data(0x6D);

	Write_3Spi_cmd(0xD200);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD201);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD202);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD203);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD204);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD205);
	Write_3Spi_data(0x3A);
	Write_3Spi_cmd(0xD206);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD207);
	Write_3Spi_data(0x4A);
	Write_3Spi_cmd(0xD208);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD209);
	Write_3Spi_data(0x5C);
	Write_3Spi_cmd(0xD20A);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD20B);
	Write_3Spi_data(0x81);
	Write_3Spi_cmd(0xD20C);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD20D);
	Write_3Spi_data(0xA6);
	Write_3Spi_cmd(0xD20E);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD20F);
	Write_3Spi_data(0xE5);
	Write_3Spi_cmd(0xD210);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD211);
	Write_3Spi_data(0x13);
	Write_3Spi_cmd(0xD212);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD213);
	Write_3Spi_data(0x54);
	Write_3Spi_cmd(0xD214);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD215);
	Write_3Spi_data(0x82);
	Write_3Spi_cmd(0xD216);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD217);
	Write_3Spi_data(0xCA);
	Write_3Spi_cmd(0xD218);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD219);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD21A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD21B);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD21C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD21D);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD21E);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD21F);
	Write_3Spi_data(0x67);
	Write_3Spi_cmd(0xD220);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD221);
	Write_3Spi_data(0x84);
	Write_3Spi_cmd(0xD222);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD223);
	Write_3Spi_data(0xA4);
	Write_3Spi_cmd(0xD224);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD225);
	Write_3Spi_data(0xB7);
	Write_3Spi_cmd(0xD226);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD227);
	Write_3Spi_data(0xCF);
	Write_3Spi_cmd(0xD228);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD229);
	Write_3Spi_data(0xDE);
	Write_3Spi_cmd(0xD22A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD22B);
	Write_3Spi_data(0xF2);
	Write_3Spi_cmd(0xD22C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD22D);
	Write_3Spi_data(0xFE);
	Write_3Spi_cmd(0xD22E);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD22F);
	Write_3Spi_data(0x10);
	Write_3Spi_cmd(0xD230);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD231);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD232);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD233);
	Write_3Spi_data(0x6D);

	Write_3Spi_cmd(0xD300);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD301);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD302);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD303);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD304);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD305);
	Write_3Spi_data(0x3A);
	Write_3Spi_cmd(0xD306);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD307);
	Write_3Spi_data(0x4A);
	Write_3Spi_cmd(0xD308);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD309);
	Write_3Spi_data(0x5C);
	Write_3Spi_cmd(0xD30A);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD30B);
	Write_3Spi_data(0x81);
	Write_3Spi_cmd(0xD30C);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD30D);
	Write_3Spi_data(0xA6);
	Write_3Spi_cmd(0xD30E);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD30F);
	Write_3Spi_data(0xE5);
	Write_3Spi_cmd(0xD310);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD311);
	Write_3Spi_data(0x13);
	Write_3Spi_cmd(0xD312);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD313);
	Write_3Spi_data(0x54);
	Write_3Spi_cmd(0xD314);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD315);
	Write_3Spi_data(0x82);
	Write_3Spi_cmd(0xD316);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD317);
	Write_3Spi_data(0xCA);
	Write_3Spi_cmd(0xD318);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD319);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD31A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD31B);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD31C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD31D);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD31E);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD31F);
	Write_3Spi_data(0x67);
	Write_3Spi_cmd(0xD320);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD321);
	Write_3Spi_data(0x84);
	Write_3Spi_cmd(0xD322);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD323);
	Write_3Spi_data(0xA4);
	Write_3Spi_cmd(0xD324);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD325);
	Write_3Spi_data(0xB7);
	Write_3Spi_cmd(0xD326);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD327);
	Write_3Spi_data(0xCF);
	Write_3Spi_cmd(0xD328);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD329);
	Write_3Spi_data(0xDE);
	Write_3Spi_cmd(0xD32A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD32B);
	Write_3Spi_data(0xF2);
	Write_3Spi_cmd(0xD32C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD32D);
	Write_3Spi_data(0xFE);
	Write_3Spi_cmd(0xD32E);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD32F);
	Write_3Spi_data(0x10);
	Write_3Spi_cmd(0xD330);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD331);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD332);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD333);
	Write_3Spi_data(0x6D);

	Write_3Spi_cmd(0xD400);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD401);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD402);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD403);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD404);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD405);
	Write_3Spi_data(0x3A);
	Write_3Spi_cmd(0xD406);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD407);
	Write_3Spi_data(0x4A);
	Write_3Spi_cmd(0xD408);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD409);
	Write_3Spi_data(0x5C);
	Write_3Spi_cmd(0xD40A);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD40B);
	Write_3Spi_data(0x81);
	Write_3Spi_cmd(0xD40C);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD40D);
	Write_3Spi_data(0xA6);
	Write_3Spi_cmd(0xD40E);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD40F);
	Write_3Spi_data(0xE5);
	Write_3Spi_cmd(0xD410);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD411);
	Write_3Spi_data(0x13);
	Write_3Spi_cmd(0xD412);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD413);
	Write_3Spi_data(0x54);
	Write_3Spi_cmd(0xD414);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD415);
	Write_3Spi_data(0x82);
	Write_3Spi_cmd(0xD416);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD417);
	Write_3Spi_data(0xCA);
	Write_3Spi_cmd(0xD418);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD419);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD41A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD41B);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD41C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD41D);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD41E);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD41F);
	Write_3Spi_data(0x67);
	Write_3Spi_cmd(0xD420);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD421);
	Write_3Spi_data(0x84);
	Write_3Spi_cmd(0xD422);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD423);
	Write_3Spi_data(0xA4);
	Write_3Spi_cmd(0xD424);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD425);
	Write_3Spi_data(0xB7);
	Write_3Spi_cmd(0xD426);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD427);
	Write_3Spi_data(0xCF);
	Write_3Spi_cmd(0xD428);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD429);
	Write_3Spi_data(0xDE);
	Write_3Spi_cmd(0xD42A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD42B);
	Write_3Spi_data(0xF2);
	Write_3Spi_cmd(0xD42C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD42D);
	Write_3Spi_data(0xFE);
	Write_3Spi_cmd(0xD42E);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD42F);
	Write_3Spi_data(0x10);
	Write_3Spi_cmd(0xD430);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD431);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD432);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD433);
	Write_3Spi_data(0x6D);

	Write_3Spi_cmd(0xD500);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD501);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD502);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD503);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD504);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD505);
	Write_3Spi_data(0x3A);
	Write_3Spi_cmd(0xD506);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD507);
	Write_3Spi_data(0x4A);
	Write_3Spi_cmd(0xD508);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD509);
	Write_3Spi_data(0x5C);
	Write_3Spi_cmd(0xD50A);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD50B);
	Write_3Spi_data(0x81);
	Write_3Spi_cmd(0xD50C);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD50D);
	Write_3Spi_data(0xA6);
	Write_3Spi_cmd(0xD50E);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD50F);
	Write_3Spi_data(0xE5);
	Write_3Spi_cmd(0xD510);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD511);
	Write_3Spi_data(0x13);
	Write_3Spi_cmd(0xD512);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD513);
	Write_3Spi_data(0x54);
	Write_3Spi_cmd(0xD514);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD515);
	Write_3Spi_data(0x82);
	Write_3Spi_cmd(0xD516);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD517);
	Write_3Spi_data(0xCA);
	Write_3Spi_cmd(0xD518);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD519);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD51A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD51B);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD51C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD51D);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD51E);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD51F);
	Write_3Spi_data(0x67);
	Write_3Spi_cmd(0xD520);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD521);
	Write_3Spi_data(0x84);
	Write_3Spi_cmd(0xD522);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD523);
	Write_3Spi_data(0xA4);
	Write_3Spi_cmd(0xD524);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD525);
	Write_3Spi_data(0xB7);
	Write_3Spi_cmd(0xD526);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD527);
	Write_3Spi_data(0xCF);
	Write_3Spi_cmd(0xD528);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD529);
	Write_3Spi_data(0xDE);
	Write_3Spi_cmd(0xD52A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD52B);
	Write_3Spi_data(0xF2);
	Write_3Spi_cmd(0xD52C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD52D);
	Write_3Spi_data(0xFE);
	Write_3Spi_cmd(0xD52E);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD52F);
	Write_3Spi_data(0x10);
	Write_3Spi_cmd(0xD530);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD531);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD532);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD533);
	Write_3Spi_data(0x6D);

	Write_3Spi_cmd(0xD600);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD601);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD602);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD603);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD604);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD605);
	Write_3Spi_data(0x3A);
	Write_3Spi_cmd(0xD606);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD607);
	Write_3Spi_data(0x4A);
	Write_3Spi_cmd(0xD608);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD609);
	Write_3Spi_data(0x5C);
	Write_3Spi_cmd(0xD60A);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD60B);
	Write_3Spi_data(0x81);
	Write_3Spi_cmd(0xD60C);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD60D);
	Write_3Spi_data(0xA6);
	Write_3Spi_cmd(0xD60E);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD60F);
	Write_3Spi_data(0xE5);
	Write_3Spi_cmd(0xD610);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD611);
	Write_3Spi_data(0x13);
	Write_3Spi_cmd(0xD612);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD613);
	Write_3Spi_data(0x54);
	Write_3Spi_cmd(0xD614);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD615);
	Write_3Spi_data(0x82);
	Write_3Spi_cmd(0xD616);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD617);
	Write_3Spi_data(0xCA);
	Write_3Spi_cmd(0xD618);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD619);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xD61A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD61B);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xD61C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD61D);
	Write_3Spi_data(0x34);
	Write_3Spi_cmd(0xD61E);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD61F);
	Write_3Spi_data(0x67);
	Write_3Spi_cmd(0xD620);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD621);
	Write_3Spi_data(0x84);
	Write_3Spi_cmd(0xD622);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD623);
	Write_3Spi_data(0xA4);
	Write_3Spi_cmd(0xD624);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD625);
	Write_3Spi_data(0xB7);
	Write_3Spi_cmd(0xD626);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD627);
	Write_3Spi_data(0xCF);
	Write_3Spi_cmd(0xD628);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD629);
	Write_3Spi_data(0xDE);
	Write_3Spi_cmd(0xD62A);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD62B);
	Write_3Spi_data(0xF2);
	Write_3Spi_cmd(0xD62C);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xD62D);
	Write_3Spi_data(0xFE);
	Write_3Spi_cmd(0xD62E);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD62F);
	Write_3Spi_data(0x10);
	Write_3Spi_cmd(0xD630);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD631);
	Write_3Spi_data(0x33);
	Write_3Spi_cmd(0xD632);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xD633);
	Write_3Spi_data(0x6D);

	//**************LV2 Page 0 enable*************//
	Write_3Spi_cmd(0xF000);
	Write_3Spi_data(0x55);
	Write_3Spi_cmd(0xF001);
	Write_3Spi_data(0xAA);
	Write_3Spi_cmd(0xF002);
	Write_3Spi_data(0x52);
	Write_3Spi_cmd(0xF003);
	Write_3Spi_data(0x08);
	Write_3Spi_cmd(0xF004);
	Write_3Spi_data(0x00); // Page 0

	// Write_3Spi_cmd(0xB000);	Write_3Spi_data(0x00);//add RGB mode2
	// Write_3Spi_cmd(0xB001);	Write_3Spi_data(0x10);
	// Write_3Spi_cmd(0xB002);	Write_3Spi_data(0x10);
	// Write_3Spi_cmd(0xB003);	Write_3Spi_data(0x10);
	// Write_3Spi_cmd(0xB004);	Write_3Spi_data(0x10);
	Write_3Spi_cmd(0xB300);
	Write_3Spi_data(0x00);

	//*************480x800*********************//
	Write_3Spi_cmd(0xB500);
	Write_3Spi_data(0x50);

	//***************Display control**************//
	Write_3Spi_cmd(0xB100);
	Write_3Spi_data(0xCC);
	Write_3Spi_cmd(0xB101);
	Write_3Spi_data(0x00);

	//***************Source hold time*************//
	Write_3Spi_cmd(0xB600);
	Write_3Spi_data(0x05);

	//**************Gate EQ control***************//
	Write_3Spi_cmd(0xB700);
	Write_3Spi_data(0x70);
	Write_3Spi_cmd(0xB701);
	Write_3Spi_data(0x70);

	//*************Source EQ control (Mode 2)******//
	Write_3Spi_cmd(0xB800);
	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xB801);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xB802);
	Write_3Spi_data(0x03);
	Write_3Spi_cmd(0xB803);
	Write_3Spi_data(0x03);

	//************Inversion mode  (2-dot)***********//
	Write_3Spi_cmd(0xBC00);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xBC01);
	Write_3Spi_data(0x00);
	Write_3Spi_cmd(0xBC02);
	Write_3Spi_data(0x00);

	//***************Frame rate***************//
	/*
	Write_3Spi_cmd(0xBD00);	Write_3Spi_data(0x01);
	Write_3Spi_cmd(0xBD01);	Write_3Spi_data(0x84);
	Write_3Spi_cmd(0xBD02);	Write_3Spi_data(0x1C);  //0X1C
	Write_3Spi_cmd(0xBD03);	Write_3Spi_data(0x1C);
	Write_3Spi_cmd(0xBD04);	Write_3Spi_data(0x00);*/

	//********Timing control 4H w/ 4-Delayms *******//
	Write_3Spi_cmd(0xC900);
	Write_3Spi_data(0xD0);
	Write_3Spi_cmd(0xC901);
	Write_3Spi_data(0x02);
	Write_3Spi_cmd(0xC902);
	Write_3Spi_data(0x50);
	Write_3Spi_cmd(0xC903);
	Write_3Spi_data(0x50);
	Write_3Spi_cmd(0xC904);
	Write_3Spi_data(0x50);

	Write_3Spi_cmd(0x3600);
	Write_3Spi_data(0x00); // 00
	Write_3Spi_cmd(0x3500);
	Write_3Spi_data(0x00);

	Write_3Spi_cmd(0x3A00);
	Write_3Spi_data(0x55);
	Write_3Spi_cmd(0x1100);
	delay_1ms(120);
	Write_3Spi_cmd(0x2900);
	delay_1ms(10);
	Write_3Spi_cmd(0x2C00);
}










void TLI_ConfigParam(
    int b_HORIZONTAL_SYNCHRONOUS_PULSE,
    int b_HORIZONTAL_BACK_PORCH,
    int b_HORIZONTAL_FRONT_PORCH,
    int b_VERTICAL_SYNCHRONOUS_PULSE,
    int b_VERTICAL_BACK_PORCH,
    int b_VERTICAL_FRONT_PORCH)
{

  tli_parameter_struct tli_init_struct;
  tli_layer_parameter_struct tli_layer_init_struct;

  /* initialize the parameters of structure */
    tli_struct_para_init(&tli_init_struct);
    tli_layer_struct_para_init(&tli_layer_init_struct);
    rcu_periph_clock_enable(RCU_TLI);
    /* enable the periphral clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);
    /* TLI pins AF configure */
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_0); // PB0
    gpio_af_set(GPIOB, GPIO_AF_14, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
    gpio_af_set(GPIOC, GPIO_AF_14, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_af_set(GPIOD, GPIO_AF_14, GPIO_PIN_3);
    gpio_af_set(GPIOF, GPIO_AF_14, GPIO_PIN_10);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11);
    gpio_af_set(GPIOG, GPIO_AF_9, GPIO_PIN_10 | GPIO_PIN_12);
    /* configure TLI GPIO */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
    

    b_HORIZONTAL_SYNCHRONOUS_PULSE = HORIZONTAL_SYNCHRONOUS_PULSE;
    b_HORIZONTAL_BACK_PORCH        = HORIZONTAL_BACK_PORCH       ;
    b_HORIZONTAL_FRONT_PORCH       = HORIZONTAL_FRONT_PORCH      ;
    b_VERTICAL_SYNCHRONOUS_PULSE   = VERTICAL_SYNCHRONOUS_PULSE  ;
    b_VERTICAL_BACK_PORCH          = VERTICAL_BACK_PORCH         ;
    b_VERTICAL_FRONT_PORCH         = VERTICAL_FRONT_PORCH        ;
    if (tli_clk.v == 2 || tli_clk.v == 4 || tli_clk.v == 8 || tli_clk.v == 16)
    {
    /* configure the PLLSAI clock to generate lcd clock */
    if (ERROR == rcu_pllsai_config(tli_clk.n, tli_clk.p, tli_clk.r))
    {
      while (1)
        ;
    }
    switch (tli_clk.v)
    {
    case 2:
      rcu_tli_clock_div_config(RCU_PLLSAIR_DIV2);
      break;
    case 4:
      rcu_tli_clock_div_config(RCU_PLLSAIR_DIV4);
      break;
    case 8:
      rcu_tli_clock_div_config(RCU_PLLSAIR_DIV8);
      break;
    case 16:
      rcu_tli_clock_div_config(RCU_PLLSAIR_DIV16);
      break;
    default:
      rcu_tli_clock_div_config(RCU_PLLSAIR_DIV4);
      break;
    }
    }
    else
    {
    /* configure the PLLSAI clock to generate lcd clock */
    if (ERROR == rcu_pllsai_config(192, 2, 3))
    {
      while (1)
        ;
    }
    rcu_tli_clock_div_config(RCU_PLLSAIR_DIV2);
    }
//    printf("lcd_clk %d %d %d %d ",
//        tli_clk.n,
//        tli_clk.p,
//        tli_clk.r,
//        tli_clk.v);

//    printf("TLI_ConfigParam %d %d %d %d %d %d \r\n",
//        b_HORIZONTAL_SYNCHRONOUS_PULSE,
//        b_HORIZONTAL_BACK_PORCH,
//        b_HORIZONTAL_FRONT_PORCH,
//        b_VERTICAL_SYNCHRONOUS_PULSE,
//        b_VERTICAL_BACK_PORCH,
//        b_VERTICAL_FRONT_PORCH);

    rcu_osci_on(RCU_PLLSAI_CK);
    if (ERROR == rcu_osci_stab_wait(RCU_PLLSAI_CK))
    {
    while (1)
      ;
    }

    /* configure TLI parameter struct */
    tli_init_struct.signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_de = TLI_DE_ACTLIVE_LOW;
    #ifdef LCD_800X4800
    tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;
    #else
    tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_INVERTEDTLI;
    #endif

    /* LCD display timing configuration */
    tli_init_struct.synpsz_hpsz = b_HORIZONTAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.synpsz_vpsz = b_VERTICAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.backpsz_hbpsz = b_HORIZONTAL_SYNCHRONOUS_PULSE + b_HORIZONTAL_BACK_PORCH - 1;
    tli_init_struct.backpsz_vbpsz = b_VERTICAL_SYNCHRONOUS_PULSE + b_VERTICAL_BACK_PORCH - 1;
    tli_init_struct.activesz_hasz = b_HORIZONTAL_SYNCHRONOUS_PULSE + b_HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH - 1;
    tli_init_struct.activesz_vasz = b_VERTICAL_SYNCHRONOUS_PULSE + b_VERTICAL_BACK_PORCH + ACTIVE_HEIGHT - 1;
    tli_init_struct.totalsz_htsz = b_HORIZONTAL_SYNCHRONOUS_PULSE + b_HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH + b_HORIZONTAL_FRONT_PORCH - 1;
    tli_init_struct.totalsz_vtsz = b_VERTICAL_SYNCHRONOUS_PULSE + b_VERTICAL_BACK_PORCH + ACTIVE_HEIGHT + b_VERTICAL_FRONT_PORCH - 1;

    tli_init_struct.backcolor_red = 0xff;
    tli_init_struct.backcolor_green = 0xff;
    tli_init_struct.backcolor_blue = 0xff;

    tli_init(&tli_init_struct);
    //memset((void*)my_fb, 0x0000, 800 * 480* 16);
    tli_layer_init_struct.layer_window_leftpos = b_HORIZONTAL_SYNCHRONOUS_PULSE + b_HORIZONTAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_rightpos = (ACTIVE_WIDTH + b_HORIZONTAL_SYNCHRONOUS_PULSE + b_HORIZONTAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_window_toppos = b_VERTICAL_SYNCHRONOUS_PULSE + b_VERTICAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_bottompos = (ACTIVE_HEIGHT + b_VERTICAL_SYNCHRONOUS_PULSE + b_VERTICAL_BACK_PORCH - 1);

    /* TLI window pixel format configuration */
    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
    /* TLI window specified alpha configuration */
    tli_layer_init_struct.layer_sa = 255; //
    /* TLI layer default alpha R,G,B value configuration */
    tli_layer_init_struct.layer_default_blue = 0xFF;
    tli_layer_init_struct.layer_default_green = 0xFF;
    tli_layer_init_struct.layer_default_red = 0xFF;
    tli_layer_init_struct.layer_default_alpha = 0X00; // ???
    /* TLI window blend configuration */
    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_SA;
    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_SA;
    /* TLI layer frame buffer base address configuration */
    //ltdc_framebuf[0] = (uint16_t *)&ltdc_lcd_framebuf0;
    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)my_fb;

    //  tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)&gImage_Image_RGB565;

    tli_layer_init_struct.layer_frame_line_length = ((ACTIVE_WIDTH * 2) + 3);
    tli_layer_init_struct.layer_frame_buf_stride_offset = (ACTIVE_WIDTH * 2);
    tli_layer_init_struct.layer_frame_total_line_number = ACTIVE_HEIGHT;
    tli_layer_init(LAYER0, &tli_layer_init_struct);

//    tli_layer_init_struct.layer_window_leftpos = b_HORIZONTAL_SYNCHRONOUS_PULSE + b_HORIZONTAL_BACK_PORCH;
//    tli_layer_init_struct.layer_window_rightpos = (ACTIVE_WIDTH + b_HORIZONTAL_SYNCHRONOUS_PULSE + b_HORIZONTAL_BACK_PORCH - 1);
//    tli_layer_init_struct.layer_window_toppos = b_VERTICAL_SYNCHRONOUS_PULSE + b_VERTICAL_BACK_PORCH;
//    tli_layer_init_struct.layer_window_bottompos = (ACTIVE_HEIGHT + b_VERTICAL_SYNCHRONOUS_PULSE + b_VERTICAL_BACK_PORCH - 1);

//    /* TLI window pixel format configuration */
//    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
//    /* TLI window specified alpha configuration */
//    tli_layer_init_struct.layer_sa = 255; //
//    /* TLI layer default alpha R,G,B value configuration */
//    tli_layer_init_struct.layer_default_blue = 0xFF;
//    tli_layer_init_struct.layer_default_green = 0xFF;
//    tli_layer_init_struct.layer_default_red = 0xFF;
//    tli_layer_init_struct.layer_default_alpha = 0X00; // ???
//    /* TLI window blend configuration */
//    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_SA;
//    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_SA;
//    /* TLI layer frame buffer base address configuration */
//    ltdc_framebuf[1] = (uint16_t *)&ltdc_lcd_framebuf1;

//    //  memcpy(ltdc_lcd_framebuf1, gImage_Image_RGB565, sizeof(gImage_Image_RGB565));
//    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)ltdc_framebuf[1];

//    //  tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)&gImage_Image_RGB565;
//    tli_layer_init_struct.layer_frame_line_length = ((ACTIVE_WIDTH * 2) + 3);
//    tli_layer_init_struct.layer_frame_buf_stride_offset = (ACTIVE_WIDTH * 2);
//    tli_layer_init_struct.layer_frame_total_line_number = ACTIVE_HEIGHT; // ACTIVE_HEIGHT;
//    tli_layer_init(LAYER1, &tli_layer_init_struct);

    tli_dither_config(TLI_DITHER_ENABLE);
    tli_layer_enable(LAYER0);
    tli_layer_disable(LAYER1);
    
    /* reload layer0 and layer1 configuration */
    //tli_reload_config(TLI_REQUEST_RELOAD_EN);
		tli_reload_config(TLI_FRAME_BLANK_RELOAD_EN);
    tli_enable();
//		ipa_config();
}

static void ipa_config(void)
{
  rcu_periph_clock_enable(RCU_IPA);
	nvic_irq_enable(IPA_IRQn, 0, 2);
}


//屏幕初始化
void LCD_Init(void)
{
//    printf("Start NT35510_Init\r\n");
    NT35510_Init();
//    printf("Start TLI_ConfigParam\r\n");
    TLI_ConfigParam(0,0,0,0,0,0);
//    printf("LCD_Init Over! \r\n");
    
//    LCD_Clear(1,WHITE);
    LCD_BLK_ON;
}


///////画点
//void LTDC_Draw_Point(uint8_t layer, uint16_t x, uint16_t y, uint32_t color)
//{
//    *(uint16_t *)((uint32_t)ltdc_framebuf[layer] + 2 * (ACTIVE_WIDTH * (y) + x)) = color;
//}
////清屏函数   
//void LCD_Clear(uint8_t layer,uint16_t  color)
//{
//    uint32_t index=0;
//    uint32_t totalpoint=ACTIVE_WIDTH;
//    totalpoint*=ACTIVE_HEIGHT; //得到总点数
//    for(index=0;index<totalpoint;index++)
//    {
//        ltdc_framebuf[layer][index] = color;
//    }
//} 

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
//void LCD_ShowChar(uint8_t layer,uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
//{  
//    uint8_t temp,t1,t;
//    uint16_t y0=y;
//    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
//    num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
//    for(t=0;t<csize;t++)
//    {   
//        if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
//        else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
//        else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
//        else return;								//没有的字库
//        for(t1=0;t1<8;t1++)
//        {			    
//            if(temp&0x80)LTDC_Draw_Point(layer,x,y,POINT_COLOR);
//            else if(mode==0)LTDC_Draw_Point(layer,x,y,BACK_COLOR);
//            temp<<=1;
//            y++;
//            if(y>=ACTIVE_HEIGHT)return;		//超区域了
//            if((y-y0)==size)
//            {
//                y=y0;
//                x++;
//                if(x>=ACTIVE_WIDTH)return;	//超区域了
//                break;
//            }
//        }  	 
//    }  
//}   
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
//void LCD_ShowString(uint8_t layer,uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p)
//{         
//	uint8_t x0=x;
//	width+=x;
//	height+=y;
//    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
//    {       
//        if(x>=width){x=x0;y+=size;}
//        if(y>=height)break;//退出
//        LCD_ShowChar(layer,x,y,*p,size,mode);
//        x+=size/2;
//        p++;
//    }  
//}















