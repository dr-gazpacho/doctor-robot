#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico/binary_info.h"

//16 bit counter - 65535 (2**16 - 1)
//125 MHz clock (125,000,000 cycles/second)
//8 nano secod period (1/125MHz)
//Divider initialized to 1 (counter increments 1 every cycle or 8ns)

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d){
	uint32_t clock = 125000000;
	uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
	if (divider16 / 16 == 0)
		divider16 = 16;
	uint32_t wrap = clock * 16 / divider16 / f - 1;
	pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
	pwm_set_wrap(slice_num, wrap);
	pwm_set_chan_level(slice_num, chan, wrap * d / 100);
	return wrap;
}

int main() {
	
	stdio_init_all();
	//initialize the pins for the buttons
	gpio_set_function(13, GPIO_FUNC_SIO);
	gpio_set_function(14, GPIO_FUNC_SIO);
	gpio_set_function(15, GPIO_FUNC_SIO);
	//initialize all pins for the LEDs
	gpio_init(16);
	gpio_init(17);
	gpio_init(18);
	
	//set the direction of the pins attached to the buttons as input
	gpio_set_dir(13, false);
	gpio_set_dir(14, false);
	gpio_set_dir(15, false);
	//set the pins controlling the LEDs to output
	gpio_set_dir(16, true);
	gpio_set_dir(17, true);
	gpio_set_dir(18, true);
	
	//set up the button's pins to use internal pull-up
	gpio_pull_up(13);
	gpio_pull_up(14);
	gpio_pull_up(15);
	
	gpio_set_function(22, GPIO_FUNC_PWM);
	uint slice_num = pwm_gpio_to_slice_num(22);
	uint chan22 = pwm_gpio_to_channel(22);

	
	while(1){
		if(gpio_get(13)){
			gpio_put(16, 0);
			pwm_set_enabled(slice_num, false);
		} else {
			gpio_put(16, 1);
			pwm_set_freq_duty(slice_num, chan22, 700, 50);
			pwm_set_enabled(slice_num, true);
		}
		if(gpio_get(14)){
			gpio_put(17, 0);
			pwm_set_enabled(slice_num, false);
		} else {
			gpio_put(17, 1);
			pwm_set_freq_duty(slice_num, chan22, 1500, 50);
			pwm_set_enabled(slice_num, true);
		}
		if(gpio_get(15)){
			gpio_put(18, 0);
			pwm_set_enabled(slice_num, false);
		} else {
			gpio_put(18, 1);
			pwm_set_freq_duty(slice_num, chan22, 2000, 50);
			pwm_set_enabled(slice_num, true);
		}
		
	}
}
