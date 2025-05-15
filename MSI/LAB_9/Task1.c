#include <stm32f4xx.h>

void lcd_ini(void);
void lcd_data(char j);
void lcd_cmd(char i);
void lcd_string(char *str); // New function to display a string

volatile uint32_t msTicks;                      /* counts 1ms timeTicks */

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

void Delay(uint32_t dlyTicks) {
  uint32_t loop=0, dly=0, loope=0;
  dly = dlyTicks;
  for(loop=0; loop<dly; loop++) {
    for(loope=0; loope<29000; loope++) {
      __nop();
    }
  }
}

unsigned long LCDDATA=0;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main(void) {
  SystemCoreClockUpdate();                 // Get Core Clock Frequency
  
  /*----------------------------------------------------------------------------
    Initialize GPIO ports for LCD interface
   *----------------------------------------------------------------------------*/
  // Enable clock for GPIOB and GPIOE
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOEEN;
  
  // Configure GPIOB pins 0 (RW) and 1 (RS) as output
  GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1);
  GPIOB->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0); // Output mode
  
  // Configure GPIOE pins 7-15 as output (PE7 = Enable, PE8-PE15 = Data)
  GPIOE->MODER &= ~(GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9 |
                   GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12 |
                   GPIO_MODER_MODER13 | GPIO_MODER_MODER14 | GPIO_MODER_MODER15);
  GPIOE->MODER |= (GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 |
                  GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0 |
                  GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0);
  
  // Set initial states
  GPIOB->BSRRH = (1 << 0);    // LCD RW -> 0 (Write mode)
  
  // Initialize LCD
  lcd_ini();
  
  // Display first line
  lcd_cmd(0x80);  // Set cursor to first line, first position
  lcd_string("MSTI LAB");
  
  // Display second line
  lcd_cmd(0xC0);  // Set cursor to second line, first position
  lcd_string("LCD Interface");
  
  while(1) {
    // Main loop - can add more functionality here
  }
}

void lcd_ini(void) {
  Delay(50);      // Wait for LCD to power up
  
  lcd_cmd(0x38);  // Function set: 8-bit interface, 2 lines, 5x7 dots
  Delay(5);
  lcd_cmd(0x38);  // Repeat command (recommended for reliable initialization)
  Delay(1);
  lcd_cmd(0x38);  // Repeat command (recommended for reliable initialization)
  Delay(1);
  
  lcd_cmd(0x0C);  // Display ON, Cursor OFF, Blink OFF
  Delay(1);
  lcd_cmd(0x06);  // Entry mode set: Increment cursor, No display shift
  Delay(1);
  lcd_cmd(0x01);  // Clear display
  Delay(20);      // Clear display needs longer delay
}

void lcd_cmd(char i) {
  GPIOB->BSRRH = (1 << 1);    // RS=0 for command
  
  // Set data pins (PE8-PE15)
  GPIOE->ODR &= 0x000000FF;   // Clear data bits
  GPIOE->ODR |= ((uint32_t)i << 8);  // Set data bits (shifted to PE8-PE15)
  
  // Generate Enable pulse
  GPIOE->BSRRL = (1 << 7);    // Enable high
  Delay(1);
  GPIOE->BSRRH = (1 << 7);    // Enable low
  
  Delay(5);  // Wait for command to complete
}

void lcd_data(char j) {
  GPIOB->BSRRL = (1 << 1);    // RS=1 for data
  
  // Set data pins (PE8-PE15)
  GPIOE->ODR &= 0x000000FF;   // Clear data bits
  GPIOE->ODR |= ((uint32_t)j << 8);  // Set data bits (shifted to PE8-PE15)
  
  // Generate Enable pulse
  GPIOE->BSRRL = (1 << 7);    // Enable high
  Delay(1);
  GPIOE->BSRRH = (1 << 7);    // Enable low
  
  Delay(5);  // Wait for data to be processed
}

void lcd_string(char *str) {
  while(*str) {
    lcd_data(*str++);
  }
}