#include <stdio.h>
#include <stdint.h>
#include <stm32f4xx.h>

volatile uint32_t msTicks = 0;

// SysTick Handler for 1ms ticks
void SysTick_Handler(void) {
    msTicks++;
}

// Delay function using SysTick
void Delay(uint32_t dlyTicks) {
    uint32_t start = msTicks;
    while ((msTicks - start) < dlyTicks);
}

// Read Button States (PC6 and PC7)
uint8_t ReadButtonPC6(void) {
    return (GPIOC->IDR & (1 << 6)) == 0;  // Return 1 if pressed (low)
}

uint8_t ReadButtonPC7(void) {
    return (GPIOC->IDR & (1 << 7)) == 0;  // Return 1 if pressed (low)
}

// Set LED States (PB11-PB14)
void SetLEDs(uint8_t ledState) {
    GPIOB->ODR = (GPIOB->ODR & ~((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14))) | 
                 (ledState << 11);  // Shift bits to PB11-PB14
}

int main(void) {
    // Declare all variables at the start of the block
    int i;                // Loop variable
    uint8_t ledState = 0; // Bitmask for LEDs (PB11-PB14)
    uint8_t prevButtonPC6 = 0;
    uint8_t prevButtonPC7 = 0;
    uint8_t buttonPC6, buttonPC7;

    // Update system clock and configure SysTick for 1ms ticks
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1);  // Error trap
    }

    // Enable clocks for GPIOB (LEDs) and GPIOC (buttons)
    RCC->AHB1ENR |= (1 << 1);  // GPIOB clock
    RCC->AHB1ENR |= (1 << 2);  // GPIOC clock

    // Configure GPIOB pins PB11-PB14 as outputs for LEDs
    for (i = 11; i <= 14; i++) {
        GPIOB->MODER &= ~(3 << (i * 2));  // Clear mode bits
        GPIOB->MODER |= (1 << (i * 2));   // Set as output
        GPIOB->OTYPER &= ~(1 << i);       // Push-pull
        GPIOB->OSPEEDR |= (2 << (i * 2)); // High speed
        GPIOB->PUPDR &= ~(3 << (i * 2));  // No pull-up/pull-down
    }

    // Configure GPIOC pins PC6-PC7 as inputs for buttons
    for (i = 6; i <= 7; i++) {
        GPIOC->MODER &= ~(3 << (i * 2));  // Input mode
        GPIOC->PUPDR &= ~(3 << (i * 2));  // Clear pull-up/pull-down
        GPIOC->PUPDR |= (1 << (i * 2));   // Enable pull-up
    }

    SetLEDs(ledState);  // Initialize LEDs off

    // Main loop
    while (1) {
        buttonPC6 = ReadButtonPC6();
        buttonPC7 = ReadButtonPC7();

        // Button PC6 press: turn on next LED
        if (!prevButtonPC6 && buttonPC6) {
            if (ledState != 0xF) {  // If not all LEDs are on
                ledState = (ledState << 1) | 1;  // Shift left and set LSB
                ledState &= 0xF;  // Keep only 4 bits
                SetLEDs(ledState);
            }
        }

        // Button PC7 press: turn off last LED
        if (!prevButtonPC7 && buttonPC7) {
            if (ledState != 0) {  // If any LEDs are on
                ledState >>= 1;  // Shift right
                SetLEDs(ledState);
            }
        }

        prevButtonPC6 = buttonPC6;
        prevButtonPC7 = buttonPC7;
        Delay(50);  // Debounce delay
    }
}
