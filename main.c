

#include "gd32vf103.h"
#include "systick.h"

/* BUILTIN LED OF LONGAN BOARDS IS PIN PC13 */
// green = PA1, blue = PA2

#define LED_RED_PIN GPIO_PIN_13
#define LED_RED_GPIO_PORT GPIOC
#define LED_RED_GPIO_CLK RCU_GPIOC

#define LED_GRN_PIN GPIO_PIN_1
#define LED_GRN_GPIO_PORT GPIOA
#define LED_GRN_GPIO_CLK RCU_GPIOA

#define LED_BLU_PIN GPIO_PIN_2
#define LED_BLU_GPIO_PORT GPIOA
#define LED_BLU_GPIO_CLK RCU_GPIOA

#define PUSH_PIN GPIO_PIN_3
#define PUSH_GPIO_PORT GPIOA
#define PUSH_GPIO_CLK RCU_GPIOA



void main(void) {
    /* enable the led clock(s) */
    rcu_periph_clock_enable(LED_RED_GPIO_CLK);
    rcu_periph_clock_enable(LED_GRN_GPIO_CLK);
    /* configure led GPIO port(s) */
    gpio_init(LED_RED_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_RED_PIN);
    gpio_init(LED_GRN_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_GRN_PIN);
    gpio_init(LED_BLU_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_BLU_PIN);

    //gpio_init(PUSH_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, PUSH_PIN);
    gpio_init(PUSH_GPIO_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, PUSH_PIN); //set GPIOA 3 to import, pull up 
    //gpio_init(PUSH_GPIO_PORT, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, PUSH_PIN); //set GPIOA 3 to import, pull down

    GPIO_BC(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED on
    GPIO_BC(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED on
    GPIO_BC(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED on
    GPIO_BOP(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED off
    GPIO_BOP(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED off
    GPIO_BOP(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED off

    while(1) {

        if (SET==gpio_input_bit_get(PUSH_GPIO_PORT, PUSH_PIN))
        {
                GPIO_BC(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED on
                delay_1ms(200);
                GPIO_BOP(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED off
                GPIO_BC(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED on
                delay_1ms(200);
                GPIO_BOP(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED off
                GPIO_BC(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED on
                delay_1ms(200);
                GPIO_BOP(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED off
        }
        else
        {
            /* code */
                GPIO_BC(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED on
                delay_1ms(50);
                GPIO_BOP(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED off
                GPIO_BC(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED on
                delay_1ms(50);
                GPIO_BOP(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED off
                GPIO_BC(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED on
                delay_1ms(50);
                GPIO_BOP(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED off
        }
        
    }
}
