#include <stdio.h>
#include <stm32f4xx.h>
volatile uint32_t msTicks;                      /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}
/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay(uint32_t dlyTicks) {                                              
  uint32_t loop=0,dly=0,loope=0;
  dly = dlyTicks; 
  for(loop=0;loop<dly;loop++){
    for(loope=0;loope<29000;loope++){
      __nop();
    }
  }
}
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main(void) {
  SystemCoreClockUpdate();             // Get Core Clock Frequency 
  
  RCC->AHB1ENR |= (1 << 1);            // Enable GPIOB clock
  RCC->AHB1ENR |= (1 << 3);            // Enable GPIOD clock
  
  // Configure GPIOB register directly using your values
  GPIOB->MODER = 0x55444444;           // Setting Direction of Port B 
  GPIOB->OTYPER = 0x00000000;          // To configure the output type of Port B 
  GPIOB->OSPEEDR = 0xAAAAAAAA;         // To configure the speed of Port B  
  GPIOB->PUPDR = 0x00000000;           // No pull-up/pull-down
  
  // Configure GPIOD, ensuring PD8, PD10, and PD11 are properly set as outputs
  // For pins 8-11, we need bits 16-23 in MODER to be set as outputs (01 for each pin)
  // 0101 0101 0000 0000 0000 0000 0000 0000 (for pins 0-15)
  GPIOD->MODER &= ~(0xFF << 16);       // Clear bits 16-23 (for pins 8-11)
  GPIOD->MODER |= 0x55 << 16;          // Set pins 8-11 as outputs (01 pattern)
  
  GPIOD->OTYPER = 0x00000000;          // Push-pull for all pins
  GPIOD->OSPEEDR = 0xAAAAAAAA;         // High speed for all pins
  GPIOD->PUPDR = 0x00000000;
 
  while(1) {                           // Infinite Loop 
    // Turn ON all LEDs
    // PB11-PB15
    GPIOB->BSRRL = ((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));
    // PD8-PD11
    GPIOD->BSRRL = ((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11));
    
    Delay(300);
    
    // Turn OFF all LEDs
    // PB11-PB15
    GPIOB->BSRRH = ((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));
    // PD8-PD11
    GPIOD->BSRRH = ((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11));
    
    Delay(300);
  }
}
