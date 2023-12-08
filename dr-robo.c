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

uint32_t pwm_get_wrap(uint slice_num) {
	valid_params_if(PWM, slice_num >= 0 && slice_num < NUM_PWM_SLICES);
	return pwm_hw->slice[slice_num].top;
}

void pwm_set_duty(uint slice_num, uint chan, int d){
	pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num)*d/100);
}

int main() {
	
	stdio_init_all();
	gpio_set_function(22, GPIO_FUNC_PWM);
	uint slice_num = pwm_gpio_to_slice_num(22);
	uint chan22 = pwm_gpio_to_channel(22);
	pwm_set_freq_duty(slice_num, chan22, 50, 0);
	pwm_set_enabled(slice_num, true);
	
	gpio_init(6);
	gpio_set_dir(6, true);
	
	
	//initialize the pins for the buttons
	gpio_set_function(13, GPIO_FUNC_SIO);
	gpio_set_function(14, GPIO_FUNC_SIO);
	
	//set the direction of the pins attached to the buttons as input
	gpio_set_dir(13, false);
	gpio_set_dir(14, false);
	
	//set up the button's pins to use internal pull-up
	gpio_pull_up(13);
	gpio_pull_up(14);
	
	int state = 0;
	int count = 0;
	int increase = 0;
	int decrease = 0;
	uint64_t time;
	
	while(1){
		increase = gpio_get(13);
		time = time_us_64() + 100;
		decrease = gpio_get(14);
		switch (state) {
			case 0:
				if(increase) {
					state = 1;
					count++;
					pwm_set_duty(slice_num, chan22, count);
					printf("hello world\n");
				}
				else if(decrease){
					printf("decrease\n");
				}
				break;
			case 1:
				if(!increase) {
					state = 0;
				}
				break;
			default:
				state = 0;
		}
		sleep_until((absolute_time_t){time});
	}
}
