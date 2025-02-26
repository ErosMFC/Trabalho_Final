#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define LED_R 11           // LED Vermelho
#define JOYSTICK_ZERO 2100 // Valor do ponto zero do joystick
#define JOYSTICK_MIN 2000  // Limite inferior para acender o LED

void configurar_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice, 4095);  // Resolução do PWM
    pwm_set_enabled(slice, true);
}

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    configurar_pwm(LED_R);
    
    while (true) {
        adc_select_input(0); // Seleciona o canal do eixo X
        uint16_t adc_x = adc_read();
        
        // Ajusta o PWM do LED de acordo com o valor do joystick
        uint16_t pwm_value = (adc_x > JOYSTICK_ZERO || adc_x < JOYSTICK_MIN) ? abs(adc_x - JOYSTICK_ZERO) : 0;
        pwm_set_gpio_level(LED_R, pwm_value);
        
        sleep_ms(50); // Pequeno delay para estabilidade da leitura
    }
}
