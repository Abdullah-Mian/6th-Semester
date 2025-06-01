#include "stm32f4xx.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks */

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
    msTicks++;
}

/*----------------------------------------------------------------------------
  Delay function using SysTick
 *----------------------------------------------------------------------------*/
void Delay(uint32_t dlyTicks) {
    uint32_t startTicks = msTicks;
    while ((msTicks - startTicks) < dlyTicks) {
        // Wait for the desired number of milliseconds
    }
}

/*----------------------------------------------------------------------------
  GPIO Configuration for 28BYJ-48 Stepper Motor
 *----------------------------------------------------------------------------*/
void GPIO_Config(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER &= ~((3U << (11*2)) | (3U << (12*2)) | (3U << (13*2)) | (3U << (14*2)));
    GPIOB->MODER |= ((1U << (11*2)) | (1U << (12*2)) | (1U << (13*2)) | (1U << (14*2)));
    GPIOB->OTYPER &= ~((1U << 11) | (1U << 12) | (1U << 13) | (1U << 14));
    GPIOB->OSPEEDR &= ~((3U << (11*2)) | (3U << (12*2)) | (3U << (13*2)) | (3U << (14*2)));
    GPIOB->OSPEEDR |= ((1U << (11*2)) | (1U << (12*2)) | (1U << (13*2)) | (1U << (14*2)));
    GPIOB->PUPDR &= ~((3U << (11*2)) | (3U << (12*2)) | (3U << (13*2)) | (3U << (14*2)));
    GPIOB->BSRRH = ((1U << 11) | (1U << 12) | (1U << 13) | (1U << 14));
}

/*----------------------------------------------------------------------------
  System Clock Configuration
 *----------------------------------------------------------------------------*/
void SystemClock_Config(void) {
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1); // Capture error
    }
}

/*----------------------------------------------------------------------------
  Wave Drive Steps
 *----------------------------------------------------------------------------*/
void Step_A(void) {
    GPIOB->BSRRL = (1U << 11);
    GPIOB->BSRRH = ((1U << 12) | (1U << 13) | (1U << 14));
}

void Step_B(void) {
    GPIOB->BSRRL = (1U << 12);
    GPIOB->BSRRH = ((1U << 11) | (1U << 13) | (1U << 14));
}

void Step_C(void) {
    GPIOB->BSRRL = (1U << 13);
    GPIOB->BSRRH = ((1U << 11) | (1U << 12) | (1U << 14));
}

void Step_D(void) {
    GPIOB->BSRRL = (1U << 14);
    GPIOB->BSRRH = ((1U << 11) | (1U << 12) | (1U << 13));
}

/*----------------------------------------------------------------------------
  Wave Drive Control
 *----------------------------------------------------------------------------*/
void WaveDrive_Clockwise(uint32_t steps) {
    uint32_t i;
    for (i = 0; i < steps; i++) {
        Step_A(); Delay(2);
        Step_B(); Delay(2);
        Step_C(); Delay(2);
        Step_D(); Delay(2);
    }
}

void WaveDrive_CounterClockwise(uint32_t steps) {
    uint32_t i;
    for (i = 0; i < steps; i++) {
        Step_D(); Delay(2);
        Step_C(); Delay(2);
        Step_B(); Delay(2);
        Step_A(); Delay(2);
    }
}

/*----------------------------------------------------------------------------
  Full Step Drive
 *----------------------------------------------------------------------------*/
void FullStepDrive_Clockwise(uint32_t steps) {
    uint32_t i;
    for (i = 0; i < steps; i++) {
        GPIOB->BSRRL = ((1U << 11) | (1U << 12));
        GPIOB->BSRRH = ((1U << 13) | (1U << 14));
        Delay(2);
        GPIOB->BSRRL = ((1U << 12) | (1U << 13));
        GPIOB->BSRRH = ((1U << 11) | (1U << 14));
        Delay(2);
        GPIOB->BSRRL = ((1U << 13) | (1U << 14));
        GPIOB->BSRRH = ((1U << 11) | (1U << 12));
        Delay(2);
        GPIOB->BSRRL = ((1U << 11) | (1U << 14));
        GPIOB->BSRRH = ((1U << 12) | (1U << 13));
        Delay(2);
    }
}

/*----------------------------------------------------------------------------
  Half Step Drive
 *----------------------------------------------------------------------------*/
void HalfStepDrive_Clockwise(uint32_t steps) {
    uint32_t i;
    for (i = 0; i < steps; i++) {
        GPIOB->BSRRL = (1U << 11);
        GPIOB->BSRRH = ((1U << 12) | (1U << 13) | (1U << 14));
        Delay(1);
        GPIOB->BSRRL = ((1U << 11) | (1U << 12));
        GPIOB->BSRRH = ((1U << 13) | (1U << 14));
        Delay(1);
        GPIOB->BSRRL = (1U << 12);
        GPIOB->BSRRH = ((1U << 11) | (1U << 13) | (1U << 14));
        Delay(1);
        GPIOB->BSRRL = ((1U << 12) | (1U << 13));
        GPIOB->BSRRH = ((1U << 11) | (1U << 14));
        Delay(1);
        GPIOB->BSRRL = (1U << 13);
        GPIOB->BSRRH = ((1U << 11) | (1U << 12) | (1U << 14));
        Delay(1);
        GPIOB->BSRRL = ((1U << 13) | (1U << 14));
        GPIOB->BSRRH = ((1U << 11) | (1U << 12));
        Delay(1);
        GPIOB->BSRRL = (1U << 14);
        GPIOB->BSRRH = ((1U << 11) | (1U << 12) | (1U << 13));
        Delay(1);
        GPIOB->BSRRL = ((1U << 11) | (1U << 14));
        GPIOB->BSRRH = ((1U << 12) | (1U << 13));
        Delay(1);
    }
}

/*----------------------------------------------------------------------------
  Utility Functions
 *----------------------------------------------------------------------------*/
void Stepper_Stop(void) {
    GPIOB->BSRRH = ((1U << 11) | (1U << 12) | (1U << 13) | (1U << 14));
}

/*----------------------------------------------------------------------------
  Rotate by Angle (Adjusted for Wave Drive)
 *----------------------------------------------------------------------------*/
void Stepper_RotateAngle(float angle, uint8_t direction) {
    // 28BYJ-48: 2048 steps = 360 degrees in wave drive mode
    uint32_t steps = (uint32_t)((angle / 360.0f) * 2048.0f);
    if (direction == 1) {
        WaveDrive_Clockwise(steps / 4); // Each cycle is 4 steps
    } else {
        WaveDrive_CounterClockwise(steps / 4);
    }
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main(void) {
    SystemClock_Config();
    GPIO_Config();
    Stepper_Stop();
    Delay(1000);

    while (1) {
        WaveDrive_Clockwise(50);
        Delay(1000);
        WaveDrive_CounterClockwise(50);
        Delay(1000);
        FullStepDrive_Clockwise(25);
        Delay(1000);
        HalfStepDrive_Clockwise(50);
        Delay(2000);
        Stepper_RotateAngle(90.0f, 1); // Rotate 90 degrees clockwise
        Delay(2000);
    }
}
