#include "stm32f4xx.h"
#include <stdint.h>
// LAB8
// 7-Segment control macros - Turn OFF (Set bit high)
#define seg_a_off   GPIOE->BSRRL = ((1 << 2))
#define seg_b_off   GPIOE->BSRRL = ((1 << 5))
#define seg_c_off   GPIOC->BSRRL = ((1 << 8))
#define seg_d_off   GPIOE->BSRRL = ((1 << 6))
#define seg_e_off   GPIOD->BSRRL = ((1 << 0))
#define seg_f_off   GPIOC->BSRRL = ((1 << 9))
#define seg_g_off   GPIOE->BSRRL = ((1 << 4))
#define seg_dot_off GPIOC->BSRRL = ((1 << 13))

// 7-Segment control macros - Turn ON (Reset bit low)
#define seg_a_on    GPIOE->BSRRH = ((1 << 2))
#define seg_b_on    GPIOE->BSRRH = ((1 << 5))
#define seg_c_on    GPIOC->BSRRH = ((1 << 8))
#define seg_d_on    GPIOE->BSRRH = ((1 << 6))
#define seg_e_on    GPIOD->BSRRH = ((1 << 0))
#define seg_f_on    GPIOC->BSRRH = ((1 << 9))
#define seg_g_on    GPIOE->BSRRH = ((1 << 4))
#define seg_dot_on  GPIOC->BSRRH = ((1 << 13))

// Digit control macros
#define dig_1_on    GPIOB->BSRRH = ((1 << 5))
#define dig_2_on    GPIOB->BSRRH = ((1 << 4))
#define dig_3_on    GPIOB->BSRRH = ((1 << 7))
#define dig_1_off   GPIOB->BSRRL = ((1 << 5))
#define dig_2_off   GPIOB->BSRRL = ((1 << 4))
#define dig_3_off   GPIOB->BSRRL = ((1 << 7))

// Global variables
unsigned int counter = 0, bt = 0;
unsigned int button_pressed = 0;
unsigned int prev_button_state = 0;

// Function prototypes
void Delay(uint32_t dlyTicks);
void SysTick_Handler(void);
void display_value(unsigned int value);
void display_digit(unsigned int sel_digit, unsigned int display_digit);
void check_button(void);

// Display digit function with complete switch cases
void display_digit(unsigned int sel_digit, unsigned int display_digit) {
    // Select which digit to activate
    switch(sel_digit) {
        case 1: dig_1_on; dig_2_off; dig_3_off; break;
        case 2: dig_1_off; dig_2_on; dig_3_off; break;
        case 3: dig_1_off; dig_2_off; dig_3_on; break;
        default: dig_1_off; dig_2_off; dig_3_off; break;
    }
    
    // Display the digit pattern
    switch(display_digit) {
        case 0: // Display 0
            seg_a_on; seg_b_on; seg_c_on; seg_d_on; 
            seg_e_on; seg_f_on; seg_g_off; seg_dot_off;
            break;
        case 1: // Display 1
            seg_a_off; seg_b_on; seg_c_on; seg_d_off;
            seg_e_off; seg_f_off; seg_g_off; seg_dot_off;
            break;
        case 2: // Display 2
            seg_a_on; seg_b_on; seg_c_off; seg_d_on;
            seg_e_on; seg_f_off; seg_g_on; seg_dot_off;
            break;
        case 3: // Display 3
            seg_a_on; seg_b_on; seg_c_on; seg_d_on;
            seg_e_off; seg_f_off; seg_g_on; seg_dot_off;
            break;
        case 4: // Display 4
            seg_a_off; seg_b_on; seg_c_on; seg_d_off;
            seg_e_off; seg_f_on; seg_g_on; seg_dot_off;
            break;
        case 5: // Display 5
            seg_a_on; seg_b_off; seg_c_on; seg_d_on;
            seg_e_off; seg_f_on; seg_g_on; seg_dot_off;
            break;
        case 6: // Display 6
            seg_a_on; seg_b_off; seg_c_on; seg_d_on;
            seg_e_on; seg_f_on; seg_g_on; seg_dot_off;
            break;
        case 7: // Display 7
            seg_a_on; seg_b_on; seg_c_on; seg_d_off;
            seg_e_off; seg_f_off; seg_g_off; seg_dot_off;
            break;
        case 8: // Display 8
            seg_a_on; seg_b_on; seg_c_on; seg_d_on;
            seg_e_on; seg_f_on; seg_g_on; seg_dot_off;
            break;
        case 9: // Display 9
            seg_a_on; seg_b_on; seg_c_on; seg_d_on;
            seg_e_off; seg_f_on; seg_g_on; seg_dot_off;
            break;
        default: // Turn off all segments
            seg_a_off; seg_b_off; seg_c_off; seg_d_off;
            seg_e_off; seg_f_off; seg_g_off; seg_dot_off;
            break;
    }
}// LAB8

// Display 3-digit value
void display_value(unsigned int value) {
    unsigned int seg1 = 0, seg2 = 0, seg3 = 0;
    
    // Extract individual digits
    seg3 = value % 10;           // Units
    seg2 = (value % 100) / 10;   // Tens
    seg1 = value / 100;          // Hundreds
    
    // Display each digit with multiplexing
    display_digit(1, seg1);
    Delay(3);
    display_digit(2, seg2);
    Delay(3);
    display_digit(3, seg3);
    Delay(3);
}

// Button checking function with debouncing
void check_button(void) {
    unsigned int current_button_state = 0;
    
    // Read button state (PC6 or PC7 - using PC6 here)
    if (!(GPIOC->IDR & (1 << 6))) {  // Button pressed (active low)
        current_button_state = 1;
    }
    
    // Detect rising edge (button press)
    if (current_button_state && !prev_button_state) {
        counter++;
        if (counter > 999) {  // Keep counter within 3-digit range
            counter = 0;
        }
        Delay(50);  // Simple debounce delay
    }
    
    prev_button_state = current_button_state;
}

// SysTick variables and handler
volatile uint32_t msTicks = 0;

void SysTick_Handler(void) {
    msTicks++;
}

// Delay function
void Delay(uint32_t dlyTicks) {
    uint32_t loop = 0, dly = 0;
    dly = dlyTicks;
    for(loop = 0; loop < dly; loop++) {
        for(uint32_t i = 0; i < 1000; i++) {
            __NOP();  // No operation
        }
    }
}

// Main function
int main(void) {
    // Enable GPIO clocks
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB clock
    RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC clock
    RCC->AHB1ENR |= (1 << 3);  // Enable GPIOD clock
    RCC->AHB1ENR |= (1 << 4);  // Enable GPIOE clock
    // LAB 8
    // Configure GPIO Port B (Digit selection: PB4, PB5, PB7)
    GPIOB->MODER   = 0x00005400;  // Output mode for PB4, PB5, PB7
    GPIOB->OTYPER  = 0x00000000;  // Push-pull output type
    GPIOB->OSPEEDR = 0x0000FC00;  // High speed for PB4, PB5, PB7
    GPIOB->PUPDR   = 0x00000000;  // No pull-up/pull-down
    
    // Configure GPIO Port C (Segments and button: PC6, PC8, PC9, PC13)
    GPIOB->MODER   |= (0 << (6*2));  // PC6 as input for button
    GPIOC->MODER   = 0x14051000;  // Output mode for PC8, PC9, PC13, Input for PC6
    GPIOC->OTYPER  = 0x00000000;  // Push-pull output type
    GPIOC->OSPEEDR = 0x3C0F3000;  // High speed for segment pins
    GPIOC->PUPDR   = 0x00001000;  // Pull-up for PC6 (button)
    
    // Configure GPIO Port D (Segment: PD0)
    GPIOD->MODER   = 0x00000001;  // Output mode for PD0
    GPIOD->OTYPER  = 0x00000000;  // Push-pull output type
    GPIOD->OSPEEDR = 0x00000003;  // High speed for PD0
    GPIOD->PUPDR   = 0x00000000;  // No pull-up/pull-down
    
    // Configure GPIO Port E (Segments: PE2, PE4, PE5, PE6)
    GPIOE->MODER   = 0x00001540;  // Output mode for PE2, PE4, PE5, PE6
    GPIOE->OTYPER  = 0x00000000;  // Push-pull output type
    GPIOE->OSPEEDR = 0x00003FC0;  // High speed for segment pins
    GPIOE->PUPDR   = 0x00000000;  // No pull-up/pull-down
    
    // Initialize display - turn off all segments and digits
    seg_a_off; seg_b_off; seg_c_off; seg_d_off;
    seg_e_off; seg_f_off; seg_g_off; seg_dot_off;
    dig_1_off; dig_2_off; dig_3_off;
    
    // Main loop
    while(1) {
        check_button();        // Check for button press
        display_value(counter); // Display current counter value
    }
    
    return 0;
}