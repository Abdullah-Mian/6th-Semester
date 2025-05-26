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

// Button definitions (using PC6 and PC7)
#define BUTTON_UP_PIN       6   // PC6 - Increment button (PB1)
#define BUTTON_DOWN_PIN     7   // PC7 - Decrement button (PB2)

// Global variables
unsigned int counter = 0;
unsigned int prev_button_up_state = 0;
unsigned int prev_button_down_state = 0;
unsigned int debounce_delay = 0;

// Function prototypes
void Delay(uint32_t dlyTicks);
void display_value(unsigned int value);
void display_digit(unsigned int sel_digit, unsigned int display_digit);
void check_buttons(void);
void GPIO_Init(void);

// Display digit function with complete digit patterns
void display_digit(unsigned int sel_digit, unsigned int display_digit) {
    // First turn off all digits to prevent ghosting
    dig_1_off; dig_2_off; dig_3_off;
    
    // Select which digit to activate
    switch(sel_digit) {
        case 1: dig_1_on; break;
        case 2: dig_2_on; break;
        case 3: dig_3_on; break;
        default: break;
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
            break;// LAB8
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
}

// Display 3-digit value using multiplexing
void display_value(unsigned int value) {
    unsigned int seg1 = 0, seg2 = 0, seg3 = 0;
    
    // Extract individual digits
    seg3 = value % 10;           // Units
    seg2 = (value % 100) / 10;   // Tens
    seg1 = value / 100;          // Hundreds
    
    // Display each digit with multiplexing
    display_digit(1, seg1);
    Delay(2);
    display_digit(2, seg2);
    Delay(2);
    display_digit(3, seg3);
    Delay(2);
}

// Button checking function with debouncing and simultaneous press detection
void check_buttons(void) {
    unsigned int current_button_up_state = 0;
    unsigned int current_button_down_state = 0;
    
    // Read button states (active low - buttons pull pin to ground when pressed)
    if (!(GPIOC->IDR & (1 << BUTTON_UP_PIN))) {
        current_button_up_state = 1;
    }
    
    if (!(GPIOC->IDR & (1 << BUTTON_DOWN_PIN))) {
        current_button_down_state = 1;
    }
    
    // Check for simultaneous button press (both buttons pressed)
    if (current_button_up_state && current_button_down_state && 
        !prev_button_up_state && !prev_button_down_state) {
        counter = 0;  // Reset counter to zero
        Delay(100);   // Debounce delay
    }
    // Check for UP button press (increment)
    else if (current_button_up_state && !prev_button_up_state && !current_button_down_state) {
        counter++;
        if (counter > 999) {  // Keep counter within 3-digit range
            counter = 999;    // Stop at maximum value
        }
        Delay(50);  // Debounce delay
    }
    // Check for DOWN button press (decrement)
    else if (current_button_down_state && !prev_button_down_state && !current_button_up_state) {
        if (counter > 0) {
            counter--;
        } else {
            counter = 0;  // Stop at minimum value
        }
        Delay(50);  // Debounce delay
    }
    
    // Update previous button states
    prev_button_up_state = current_button_up_state;
    prev_button_down_state = current_button_down_state;
}

// Simple delay function
void Delay(uint32_t dlyTicks) {
    uint32_t loop = 0;
    for(loop = 0; loop < dlyTicks; loop++) {
        for(uint32_t i = 0; i < 1000; i++) {
            __NOP();  // No operation
        }
    }
}

// GPIO initialization function
void GPIO_Init(void) { // LAB8
    // Enable GPIO clocks
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB clock
    RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC clock
    RCC->AHB1ENR |= (1 << 3);  // Enable GPIOD clock
    RCC->AHB1ENR |= (1 << 4);  // Enable GPIOE clock
    
    // Configure GPIO Port B (Digit selection: PB4, PB5, PB7)
    GPIOB->MODER   = 0x00005400;  // Output mode for PB4, PB5, PB7
    GPIOB->OTYPER  = 0x00000000;  // Push-pull output type
    GPIOB->OSPEEDR = 0x0000FC00;  // High speed for PB4, PB5, PB7
    GPIOB->PUPDR   = 0x00000000;  // No pull-up/pull-down
    GPIOC->MODER   = 0x14050000;  // Output mode for PC8, PC9, PC13; Input for PC6, PC7
    GPIOC->OTYPER  = 0x00000000;  // Push-pull output type
    GPIOC->OSPEEDR = 0x3C0F0000;  // High speed for segment pins
    GPIOC->PUPDR   = 0x00005000;  // Pull-up for PC6 and PC7 (buttons)
    GPIOD->MODER   = 0x00000001;  // Output mode for PD0
    GPIOD->OTYPER  = 0x00000000;  // Push-pull output type
    GPIOD->OSPEEDR = 0x00000003;  // High speed for PD0
    GPIOD->PUPDR   = 0x00000000;  // No pull-up/pull-down
    GPIOE->MODER   = 0x00001540;  // Output mode for PE2, PE4, PE5, PE6
    GPIOE->OTYPER  = 0x00000000;  // Push-pull output type
    GPIOE->OSPEEDR = 0x00003FC0;  // High speed for segment pins
    GPIOE->PUPDR   = 0x00000000;  // No pull-up/pull-down
}
int main(void) {
    // Initialize GPIO ports
    GPIO_Init();
    seg_a_off; seg_b_off; seg_c_off; seg_d_off;
    seg_e_off; seg_f_off; seg_g_off; seg_dot_off;
    dig_1_off; dig_2_off; dig_3_off;
    counter = 0;
    while(1) {
        check_buttons();         // Check for button presses
        display_value(counter);  // Display current counter value
    }
    
    return 0;
}