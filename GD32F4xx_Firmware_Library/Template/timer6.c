

#include "timer6.h"

void timer6_config(void)
{
	/*
	TIMER6 Configuration:
	TIMER6CLK = SystemCoreClock/20000 = 10kHz, the period is 1s(0.1ms).*/
	timer_parameter_struct timer_initpara;
	rcu_periph_clock_enable(RCU_TIMER6);
	timer_deinit(TIMER6);
	/* initialize TIMER init parameter struct*/
	timer_struct_para_init(&timer_initpara);
	/* TIMER6 configuration*/
	timer_initpara.prescaler = 19999;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection = TIMER_COUNTER_UP;
	timer_initpara.period = 9;
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER6, &timer_initpara);
	timer_enable(TIMER6);
	timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP);
	timer_interrupt_enable(TIMER6, TIMER_INT_UP);

	nvic_irq_enable(TIMER6_IRQn, 1, 1);
}
void TIMER6_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP))
	{
		/* clear channel 0 interrupt bit*/
		timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP);
		lv_tick_inc(1);
	}
}