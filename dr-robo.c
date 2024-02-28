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

void do_linear_brightness(bool enabled, uint slice_num, uint chan){
	int d = 0;

	for(int c = 100; c >= 0; c--){
		d = (c * c) / 100;
		pwm_set_duty(slice_num, chan, d);
		sleep_ms(3);
	}
}

int main() {
	
	stdio_init_all();
	gpio_init(4);
	gpio_set_dir(4, GPIO_OUT);
	gpio_init(5);
	gpio_set_dir(5, GPIO_OUT);
	
	gpio_set_function(16, GPIO_FUNC_PWM);
	gpio_set_function(21, GPIO_FUNC_PWM);
	gpio_set_function(22, GPIO_FUNC_PWM);
	
	//arms
	uint arm_slice = pwm_gpio_to_slice_num(21);
	uint chan21 = pwm_gpio_to_channel(21);
	
	//head
	uint slice_num = pwm_gpio_to_slice_num(22);
	uint chan22 = pwm_gpio_to_channel(22);
	
	//light
	uint led_slice_num = pwm_gpio_to_slice_num(16);
	uint chan16 = pwm_gpio_to_channel(16);
	
	pwm_set_freq_duty(arm_slice, chan21, 50, 0);
	pwm_set_enabled(arm_slice, true);
	
	pwm_set_freq_duty(slice_num, chan22, 50, 0);
	pwm_set_enabled(slice_num, true);
	
	pwm_set_freq_duty(led_slice_num, chan16, 2000, 0);
	pwm_set_enabled(led_slice_num, true);
	
	
	//initialize the pins for the buttons
	//button for LEDs
	gpio_set_function(12, GPIO_FUNC_SIO);
	//buttons for servos
	gpio_set_function(10, GPIO_FUNC_SIO);
	gpio_set_function(11, GPIO_FUNC_SIO);
	gpio_set_function(13, GPIO_FUNC_SIO);
	gpio_set_function(14, GPIO_FUNC_SIO);
	gpio_set_function(15, GPIO_FUNC_SIO);
	
	//set the direction of the pins attached to the buttons as input
	gpio_set_dir(10, false);
	gpio_set_dir(11, false);
	gpio_set_dir(12, false);
	gpio_set_dir(13, false);
	gpio_set_dir(14, false);
	gpio_set_dir(15, false);
	
	//set up the button's pins to use internal pull-up
	gpio_pull_up(10);
	gpio_pull_up(11);
	gpio_pull_up(12);
	gpio_pull_up(13);
	gpio_pull_up(14);
	gpio_pull_up(15);
	
	int state = 0;
	int count = 2;
	int armCount = 2;
	int toggleArmHeadMode = 0;
	int toggleSmoothMove = 0;
	int armHeadToggleState = 0;
	int smoothMoveToggleState = 0;
	int led = 0;
	int increase = 0;
	int decrease = 0;
	int reset = 0;
	uint64_t time;
	
	pwm_set_duty(arm_slice, chan21, armCount);
	pwm_set_duty(slice_num, chan22, count);
	
	gpio_put(4, 1);
	gpio_put(5, 1);
	
	while(1){
		toggleSmoothMove = !gpio_get(10);
		toggleArmHeadMode = !gpio_get(11);
		led = !gpio_get(12);
		increase = !gpio_get(13);
		decrease = !gpio_get(14);
		reset = !gpio_get(15);
		time = time_us_64() + 100;

		switch (state) {
			case 0:
				//if increase pressed, increase duty/angle
				if(toggleSmoothMove) {
					state = 1;
					smoothMoveToggleState = !smoothMoveToggleState;
				}
				if(toggleArmHeadMode) {
					state = 1;
					armHeadToggleState = !armHeadToggleState;
				}
				//handle increase in all modes
				if(increase) {
					state = 1;
					if(armHeadToggleState && !smoothMoveToggleState) {
						if(armCount < 12){
							armCount++;
						} else {
							armCount = 12;
						}
						pwm_set_duty(arm_slice, chan21, armCount);
					}
					if(smoothMoveToggleState) {
						if(armHeadToggleState) {
							armCount = 12;
							pwm_set_duty(arm_slice, chan21, armCount);
						} else {
							count = 12;
							pwm_set_duty(slice_num, chan22, count);
						}
							
					}
					if(!armHeadToggleState && !smoothMoveToggleState) {
						if(count < 12){
							count++;
						} else {
							count = 12;
						}
						pwm_set_duty(slice_num, chan22, count);
					}
				}
				//handle decrease in all modes
				if(decrease) {
					state = 1;
					if(armHeadToggleState && !smoothMoveToggleState) {
						if(armCount > 2){
							armCount--;
						} else {
							armCount = 2;
						}
						pwm_set_duty(arm_slice, chan21, armCount);
					}
					if(smoothMoveToggleState) {
						if(armHeadToggleState) {
							armCount = 2;
							pwm_set_duty(arm_slice, chan21, armCount);
						} else {
							count = 2;
							pwm_set_duty(slice_num, chan22, count);
						}
					}
					if(!armHeadToggleState && !smoothMoveToggleState) {
						if(count > 2){
							count--;
						} else {
							count = 2;
						}
						pwm_set_duty(slice_num, chan22, count);
					}
				}
				//if reset pressed, center
				if(reset) {
					state = 1;
					count = 6;
					armCount = 6;
					armHeadToggleState ? 
						pwm_set_duty(arm_slice, chan21, armCount)
						:
						pwm_set_duty(slice_num, chan22, count);
					armHeadToggleState = 0;
					smoothMoveToggleState = 0;
				}
				if(led && !increase && !decrease) {
					do_linear_brightness(!led, led_slice_num, chan16);
					state = 0;
				}
				break;
			case 1:
				if(!increase && !decrease && !reset && !led
					&& !toggleSmoothMove && !toggleArmHeadMode) {
					state = 0;
				}
				break;
			default:
				state = 0;
		}
		sleep_until((absolute_time_t){time});
	}
}
