#include "stm32f4xx.h"

// Pin definitions
#define BUTTON1_PIN 0  // PA0
#define BUTTON2_PIN 1  // PA1
#define MOTOR_PIN1  0  // PB0
#define MOTOR_PIN2  1  // PB1
#define DEBOUNCE_TIME 20  // 20ms debounce
#define PULSE_TIME 2000   // 2 seconds

// Global variable for timing
volatile uint32_t msTicks = 0;

// SysTick interrupt handler
void SysTick_Handler(void) {
    msTicks++;
}

// Configure system clock and SysTick for 1ms interrupts
void SystemClock_Config(void) {
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1); // Error trap
    }
}

// Configure GPIO pins
void GPIO_Config(void) {
    // Enable GPIOA and GPIOB clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

    // Configure PA0, PA1 as inputs with pull-down
    GPIOA->MODER &= ~((3U << (BUTTON1_PIN * 2)) | (3U << (BUTTON2_PIN * 2))); // Input mode
    GPIOA->PUPDR &= ~((3U << (BUTTON1_PIN * 2)) | (3U << (BUTTON2_PIN * 2)));
    GPIOA->PUPDR |= ((2U << (BUTTON1_PIN * 2)) | (2U << (BUTTON2_PIN * 2))); // Pull-down

    // Configure PB0, PB1 as outputs
    GPIOB->MODER &= ~((3U << (MOTOR_PIN1 * 2)) | (3U << (MOTOR_PIN2 * 2)));
    GPIOB->MODER |= ((1U << (MOTOR_PIN1 * 2)) | (1U << (MOTOR_PIN2 * 2))); // Output mode
    GPIOB->OTYPER &= ~((1U << MOTOR_PIN1) | (1U << MOTOR_PIN2)); // Push-pull
    GPIOB->BSRRH = (1U << MOTOR_PIN1) | (1U << MOTOR_PIN2); // Initial stop (reset both pins)
}

// Debounce function
uint8_t Debounce(uint8_t pin) {
    uint8_t state;
    uint32_t start;

    state = (GPIOA->IDR & (1U << pin)) ? 1 : 0;
    start = msTicks;
    while (msTicks - start < DEBOUNCE_TIME) {
        uint8_t new_state = (GPIOA->IDR & (1U << pin)) ? 1 : 0;
        if (new_state != state) {
            state = new_state;
            start = msTicks;
        }
    }
    return state;
}

int main(void) {
    uint8_t button1, button2;
    uint32_t last_pulse_time = 0;
    uint8_t pulse_state = 0; // 0 = off, 1 = on

    // Initialize system
    SystemClock_Config();
    GPIO_Config();

    while (1) {
        // Read button states
        button1 = Debounce(BUTTON1_PIN);
        button2 = Debounce(BUTTON2_PIN);

        // Control motor based on button states
        if (button1 == 0 && button2 == 0) {
            // Stand still
            GPIOB->BSRRH = (1U << MOTOR_PIN1) | (1U << MOTOR_PIN2); // Reset both pins
        }
        else if (button1 == 0 && button2 == 1) {
            // Clockwise
            GPIOB->BSRRL = (1U << MOTOR_PIN1); // Set PB0
            GPIOB->BSRRH = (1U << MOTOR_PIN2); // Reset PB1
        }
        else if (button1 == 1 && button2 == 0) {
            // Anti-clockwise
            GPIOB->BSRRH = (1U << MOTOR_PIN1); // Reset PB0
            GPIOB->BSRRL = (1U << MOTOR_PIN2); // Set PB1
        }
        else { // Both high
            if (msTicks - last_pulse_time >= PULSE_TIME) {
                last_pulse_time = msTicks;
                if (pulse_state == 0) {
                    // Pulse on (clockwise)
                    GPIOB->BSRRL = (1U << MOTOR_PIN1); // Set PB0
                    GPIOB->BSRRH = (1U << MOTOR_PIN2); // Reset PB1
                    pulse_state = 1;
                }
                else {
                    // Pulse off
                    GPIOB->BSRRH = (1U << MOTOR_PIN1) | (1U << MOTOR_PIN2); // Reset both pins
                    pulse_state = 0;
                }
            }
        }
    }
}
