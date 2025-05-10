// you will need to connect the adopter in order to use push button

#include <stdio.h>
#include <stdint.h>        // Added for uint8_t
#include <stm32f4xx.h>

volatile uint32_t msTicks;  // Counts 1ms timeTicks

// SysTick Handler
void SysTick_Handler(void) {
    msTicks++;
}

// Simple Delay Function (Note: Consider using SysTick for accuracy)
void Delay(uint32_t dlyTicks) {
    uint32_t loop, dly, loope;
    dly = dlyTicks;
    for (loop = 0; loop < dly; loop++) {
        for (loope = 0; loope < 29000; loope++) {
            __nop();  // No operation (busy wait)
        }
    }
}

// Read Button State (PC6, assuming button pulls low when pressed)
uint8_t ReadButton(void) {
    return (GPIOC->IDR & (1 << 6)) == 0;  // Return 1 if pressed (low), 0 if not
}

// Set LED State (PB11)
void SetLED(uint8_t state) {
    if (state) {
        GPIOB->BSRRL = (1 << 11);  // Set PB11 high (LED ON)
    } else {
        GPIOB->BSRRH = (1 << 11);  // Set PB11 low (LED OFF)
    }
}

int main(void) {
    SystemCoreClockUpdate();  // Update system clock

    // Enable clocks for GPIOB and GPIOC
    RCC->AHB1ENR |= (1 << 1);  // GPIOB clock
    RCC->AHB1ENR |= (1 << 2);  // GPIOC clock

    // Configure PB11 as output
    GPIOB->MODER &= ~(3 << 22);    // Clear bits 22 and 23
    GPIOB->MODER |= (1 << 22);     // Set PB11 as output (01)
    GPIOB->OTYPER &= ~(1 << 11);   // Push-pull output
    GPIOB->OSPEEDR |= (2 << 22);   // High speed
    GPIOB->PUPDR &= ~(3 << 22);    // No pull-up/pull-down

    // Configure PC6 as input with pull-up
    GPIOC->MODER &= ~(3 << 12);    // Input mode (00)
    GPIOC->OTYPER &= ~(1 << 6);    // Push-pull (not applicable for input)
    GPIOC->OSPEEDR |= (2 << 12);   // High speed
    GPIOC->PUPDR &= ~(3 << 12);    // Clear pull-up/pull-down bits
    GPIOC->PUPDR |= (1 << 12);     // Enable pull-up

    while (1) {
        if (ReadButton()) {  // Button pressed
            SetLED(1);       // Turn LED ON
        } else {
            SetLED(0);       // Turn LED OFF
        }
        Delay(50);           // Debounce delay
    }
}
