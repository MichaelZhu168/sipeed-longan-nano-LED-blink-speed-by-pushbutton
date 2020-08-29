// another way to controll LED blink speed with software de-bouncing the push-button
// main.c
// part of "GD32V-blink" project

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


void pollSensors(void);
uint32_t testSensor(void);
void blinkLED(void);

uint32_t speedStatus; //  0=slow, 1=fast
uint32_t colorStatus; //  0=red, 1=green, 2=blue


uint64_t startLock_mtime;
uint64_t tmp_mtime;
uint32_t lockStatus; //1=locked time, 0=released time, when it is locked time, not to test the sensors, 


void main(void) {

    colorStatus=0; //red led
    speedStatus=0; //  0=slow, 1=fast
    //lockStatus=0; //  1=locked time, 0=released time, when it is locked time, not to test the sensors, 
    

    /* enable the led clock(s) */
    rcu_periph_clock_enable(LED_RED_GPIO_CLK);
    rcu_periph_clock_enable(LED_GRN_GPIO_CLK);
    /* configure led GPIO port(s) */
    gpio_init(LED_RED_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_RED_PIN);
    gpio_init(LED_GRN_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_GRN_PIN);
    gpio_init(LED_BLU_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_BLU_PIN);

    //--enable push button
    //gpio_init(PUSH_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, PUSH_PIN);
    gpio_init(PUSH_GPIO_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, PUSH_PIN); //pull up
    //gpio_init(PUSH_GPIO_PORT, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, PUSH_PIN); //pull down

    GPIO_BC(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED on
    GPIO_BC(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED on
    GPIO_BC(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED on
    GPIO_BOP(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED off
    GPIO_BOP(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED off
    GPIO_BOP(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED off

    startLock_mtime = get_timer_value();

    while(1) {

        tmp_mtime = get_timer_value() - startLock_mtime;

        if (tmp_mtime >(SystemCoreClock/4000.0 * 1000 )) // only lock 1000ms
        //during locked time, no need to poll the sensors, 
        {
          pollSensors();
          //startLock_mtime = get_timer_value(); //not here. reset the startLock_mtime
        }

        blinkLED();
    }
}



void pollSensors(void) { // testSensor() called 3 times at 10ms intervals to implement de-bounce
  uint32_t test1=0, test2=0, test3=0;
  test1 = testSensor();
  delay_1ms(10); // delay 10ms
  test2 = testSensor();
  delay_1ms(10); // delay 10ms
  test3 = testSensor();
  if (test1 && test2 && test3) { // if all the 3 tests are ture, we can comfirm the required sensor change has occurred
    if (speedStatus==1){
      speedStatus=0;
    }else{
      speedStatus=1;
    }
    //only when push button is pressed firmly then reset the startLock_mtime and not poll sensors 
    //during the period of lock time
    startLock_mtime = get_timer_value(); 
  }
 }

 uint32_t testSensor(void)
 {
   if (RESET==gpio_input_bit_get(PUSH_GPIO_PORT, PUSH_PIN)) 
   //in this project (with Sipeed Longan Nano), pin GPIOA3 is set to input/pull_up. 
   //when buttom is pressed, the PIN is low. when released it is high.
   // connection:  pin GPIOA3<--->Resister 220<--->push button<--->GND 
    {
      return 1;
    }else{
      return 0;
    }
 }

 void blinkLED(void)
 {
    if (colorStatus==0) //red led
    {
      colorStatus=colorStatus+1;
      GPIO_BC(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED on
      if (speedStatus==0){
            delay_1ms(200);
      }else{
            delay_1ms(50);
      }
      GPIO_BOP(LED_RED_GPIO_PORT) = LED_RED_PIN; // red LED off
    }else {
       if (colorStatus==1) //green led
       {
         colorStatus=colorStatus+1;
         GPIO_BC(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED on
          if (speedStatus==0){
            delay_1ms(200);
          }else{
            delay_1ms(50);
          }
          GPIO_BOP(LED_GRN_GPIO_PORT) = LED_GRN_PIN; // green LED off
       }else{   //blue led
          colorStatus=0;
          GPIO_BC(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED on
          if (speedStatus==0){
            delay_1ms(200);
          }else{
            delay_1ms(50);
          }
          GPIO_BOP(LED_BLU_GPIO_PORT) = LED_BLU_PIN; // blue LED off
       } 
    }
 }
