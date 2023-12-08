#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico/binary_info.h"

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
		
	while(1){
		if(gpio_get(13)){
			gpio_put(16, 0);
		} else {
			gpio_put(16, 1);
		}
		if(gpio_get(14)){
			gpio_put(17, 0);
		} else {
			gpio_put(17, 1);
		}
		if(gpio_get(15)){
			gpio_put(18, 0);
		} else {
			gpio_put(18, 1);
		}
		
	}
}
