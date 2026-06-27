#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "MotorDriverLogic.h"

uint pwm_slice_num_forward;
uint pwm_slice_num_backward;
uint chan_in1;
uint chan_in2;

void motor_init(int IN1_PIN, int IN2_PIN, int ENA_PIN) {
    gpio_init(IN1_PIN);
    gpio_init(IN2_PIN);
    gpio_init(ENA_PIN);
    gpio_set_function(IN1_PIN, GPIO_FUNC_PWM);
    gpio_set_function(IN2_PIN, GPIO_FUNC_PWM);
    gpio_set_dir(ENA_PIN, GPIO_OUT);
    pwm_slice_num_forward = pwm_gpio_to_slice_num(IN1_PIN);
    pwm_slice_num_backward = pwm_gpio_to_slice_num(IN2_PIN);
    chan_in1 = pwm_gpio_to_channel(IN1_PIN);
    chan_in2 = pwm_gpio_to_channel(IN2_PIN);
    pwm_set_wrap(pwm_slice_num_forward, 10000);
    pwm_set_wrap(pwm_slice_num_backward, 10000);
}

void motor_driver(int PWM, int ENA_PIN, int motor_enable) {
    
    gpio_put(ENA_PIN, motor_enable);

    if (motor_enable) {
        
        if(PWM > 0){
            pwm_set_chan_level(pwm_slice_num_forward, chan_in1, PWM);
            pwm_set_chan_level(pwm_slice_num_backward, chan_in2, 0);
        } else {
            pwm_set_chan_level(pwm_slice_num_forward, chan_in1, 0);
            pwm_set_chan_level(pwm_slice_num_backward, chan_in2, -PWM);
        }
        pwm_set_enabled(pwm_slice_num_forward, true);
        pwm_set_enabled(pwm_slice_num_backward, true);

    }
}

