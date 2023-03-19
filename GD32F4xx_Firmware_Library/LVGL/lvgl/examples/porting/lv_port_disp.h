/**
 * @file lv_port_disp_templ.h
 *
 */

// /*Copy this file as "lv_port_disp.h" and set this value to "1" to enable content*/
//#if 0

//#ifndef LV_PORT_DISP_TEMPL_H
//#define LV_PORT_DISP_TEMPL_H

//#ifdef __cplusplus
//extern "C" {
//#endif

///*********************
// *      INCLUDES
// *********************/
//#include "lvgl/lvgl.h"

///*********************
// *      DEFINES
// *********************/

///**********************
// *      TYPEDEFS
// **********************/

///**********************
// * GLOBAL PROTOTYPES
// **********************/

///**********************
// *      MACROS
// **********************/

//#ifdef __cplusplus
//} /*extern "C"*/
//#endif

//#endif /*LV_PORT_DISP_TEMPL_H*/

//#endif /*Disable/Enable content*/




/*copy this file as "lv_port_disp.h" and set this value to "1" to enable content*/
#if 1
	#ifndef LV_PORT_DISP_H
	#define LV_PORT_DISP_H
	#ifdef __cplusplus
	extern "C" {
	#endif
	#include "lvgl.h"
	void lv_port_disp_init(void);
	extern lv_disp_drv_t *g_disp_drv;
  extern uint8_t g_gpu_state;
	#ifdef __cplusplus
	}
	#endif
	#endif
	/*LV_PORT_DISP_TEMPL_H*/
#endif
/*Disable/Enable*/

