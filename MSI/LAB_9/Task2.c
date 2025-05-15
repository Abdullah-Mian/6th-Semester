#include <stm32f4xx.h>
#include <string.h>

void lcd_ini(void);
void lcd_data(char j);
void lcd_cmd(char i);
void lcd_string(char *str);
void lcd_clear_line(uint8_t line);
void delay_ms(uint32_t ms);

volatile uint32_t msTicks = 0;                /* counts 1ms timeTicks */

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of milliseconds
 *----------------------------------------------------------------------------*/
void delay_ms(uint32_t ms) {
  uint32_t startTicks = msTicks;
  while ((msTicks - startTicks) < ms);
}

/*----------------------------------------------------------------------------
  Simple delay function for compatibility
 *----------------------------------------------------------------------------*/
void Delay(uint32_t dlyTicks) {
  uint32_t loop=0, dly=0, loope=0;
  dly = dlyTicks;
  for(loop=0; loop<dly; loop++) {
    for(loope=0; loope<29000; loope++) {
      __nop();
    }
  }
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main(void) {
  char nameStr[] = "Abdullah Laeeq";             // Your name - change this
  char regStr[] = "620-ECB-22AF";            // Your reg number - change this
  
  uint8_t nameLen = strlen(nameStr);
  uint8_t regLen = strlen(regStr);
  
  // Variables for scrolling positions
  int8_t namePos = -16;  // Start position for left to right (off-screen left)
  int8_t regPos = 16;    // Start position for right to left (off-screen right)
  
  char displayBuffer[33]; // Buffer for preparing display text
  uint8_t i, j;

  SystemCoreClockUpdate();                 // Get Core Clock Frequency
  
  // Setup SysTick Timer for 1 msec interrupts
  if (SysTick_Config(SystemCoreClock / 1000)) {
    while (1);                             // Capture error
  }
  
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
  
  // Main loop - handle scrolling text
  while(1) {
    // Clear both lines
    lcd_clear_line(0);
    lcd_clear_line(1);
    
    // Prepare and display first string (left to right on upper line)
    lcd_cmd(0x80);  // First line
    memset(displayBuffer, ' ', 32);
    displayBuffer[32] = '\0';
    
    // Calculate display position for name string
    if (namePos < 0) {
      // String is partially or fully off-screen to the left
      j = 0;
      for (i = -namePos; i < nameLen && j < 16; i++) {
        displayBuffer[j++] = nameStr[i];
      }
    } else if (namePos < 16) {
      // String is partially or fully on-screen
      j = namePos;
      for (i = 0; i < nameLen && j < 16; i++) {
        displayBuffer[j++] = nameStr[i];
      }
    }
    
    // Display upper line
    displayBuffer[16] = '\0';
    lcd_string(displayBuffer);
    
    // Prepare and display second string (right to left on lower line)
    lcd_cmd(0xC0);  // Second line
    memset(displayBuffer, ' ', 32);
    displayBuffer[32] = '\0';
    
    // Calculate display position for reg string
    if (regPos > 0) {
      // String is partially or fully off-screen to the right
      j = 15;
      for (i = 0; i < regLen && j >= regPos - 1 && j < 16; i++) {
        displayBuffer[j--] = regStr[i];
      }
    } else if (regPos > -regLen) {
      // String is partially on-screen
      j = 15 + regPos;
      for (i = 0; i < regLen && j >= 0 && i < regLen; i++) {
        if (j >= 0) {
          displayBuffer[j--] = regStr[i];
        }
      }
    }
    
    // Display lower line
    displayBuffer[16] = '\0';
    lcd_string(displayBuffer);
    
    // Update positions
    namePos++;
    regPos--;
    
    // Reset positions when strings have scrolled completely off screen
    if (namePos > 16) {
      namePos = -nameLen;
    }
    
    if (regPos < -regLen) {
      regPos = 16;
    }
    
    // Delay for animation effect - increased for slower scrolling
    delay_ms(100);
  }
}

/* 
  LCD Initialization function
*/
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

/*
  Function to send command to LCD
*/
void lcd_cmd(char i) {
  GPIOB->BSRRH = (1 << 1);    // RS=0 for command
  
  // Set data pins (PE8-PE15)
  GPIOE->ODR &= ~0x0000FF00;  // Clear data bits
  GPIOE->ODR |= ((uint32_t)i << 8);  // Set data bits (shifted to PE8-PE15)
  
  // Generate Enable pulse
  GPIOE->BSRRL = (1 << 7);    // Enable high
  Delay(1);
  GPIOE->BSRRH = (1 << 7);    // Enable low
  
  Delay(5);  // Wait for command to complete
}

/*
  Function to send data to LCD
*/
void lcd_data(char j) {
  GPIOB->BSRRL = (1 << 1);    // RS=1 for data
  
  // Set data pins (PE8-PE15)
  GPIOE->ODR &= ~0x0000FF00;  // Clear data bits
  GPIOE->ODR |= ((uint32_t)j << 8);  // Set data bits (shifted to PE8-PE15)
  
  // Generate Enable pulse
  GPIOE->BSRRL = (1 << 7);    // Enable high
  Delay(1);
  GPIOE->BSRRH = (1 << 7);    // Enable low
  
  Delay(1);  // Short delay between characters
}

/*
  Function to display a string on LCD
*/
void lcd_string(char *str) {
  while(*str) {
    lcd_data(*str++);
  }
}

/*
  Function to clear a specific line on LCD
*/
void lcd_clear_line(uint8_t line) {
  uint8_t i;
  
  // Position cursor at beginning of line
  if (line == 0) {
    lcd_cmd(0x80);  // First line
  } else {
    lcd_cmd(0xC0);  // Second line
  }
  
  // Write 16 spaces to clear the line
  for (i = 0; i < 16; i++) {
    lcd_data(' ');
  }
}
