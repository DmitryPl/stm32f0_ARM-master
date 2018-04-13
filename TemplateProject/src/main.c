#include "includes/stm32f0xx_ll_bus.h"
#include "includes/stm32f0xx_ll_gpio.h"
#include "includes/stm32f0xx_ll_rcc.h"
#include "includes/stm32f0xx_ll_system.h"
#include "includes/stm32f0xx_ll_exti.h"
#include "includes/stm32f051x8.h"

#define true  1
#define false 0

#define SEG_A           GPIOC, LL_GPIO_PIN_4
#define SEG_B           GPIOC, LL_GPIO_PIN_5
#define SEG_C           GPIOB, LL_GPIO_PIN_0
#define SEG_D           GPIOB, LL_GPIO_PIN_1
#define SEG_E           GPIOB, LL_GPIO_PIN_2
#define SEG_F           GPIOB, LL_GPIO_PIN_10
#define SEG_G           GPIOB, LL_GPIO_PIN_11
#define SEG_DP          GPIOB, LL_GPIO_PIN_12
#define COM1            GPIOF, LL_GPIO_PIN_5
#define COM2            GPIOA, LL_GPIO_PIN_4
#define COM4            GPIOA, LL_GPIO_PIN_6
#define COM3            GPIOA, LL_GPIO_PIN_7
#define RING            GPOIC, LL_GPIO_PIN_3
#define GREEN           GPIOC, LL_GPIO_PIN_9
#define BLUE            GPIOC, LL_GPIO_PIN_8

#define max_ziro  0
#define max_one   9
#define max_two   5
#define max_three 9
#define max_four  2
#define DP        0

void SystemClock_Config(void);
//int NumberOfClicks(viod);
static void SetDigit(int num);
static void SwitchDigit(int num, int* mas);
static int IsItTime(void);
static void EXTI0_1_IRQHandler(void);
static void DigInc(void);
static void AlarmSound(void);
static void SetTime(int* set);;
inline PortsConfig(void);

int buttom_flag = 0;
int alarm = -1;
int ticks = -1;
int sys_ticks = 0;
int num_of_set = 0;

int digits_mas[5] = { 0, 0, 0, 0, 0, 0 };
int alarm_mas[5] = { 0, 0, 0, 0, 0, 1 }; 
int stops_mas[5] = { max_ziro + 1, max_one + 1, max_two + 1, max_three + 1, max_four + 1, DP };

int main(void) {
        SystemClock_Config();
        NVIC_SetPriority(SysTick_IRQn, 1);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
        ButtonInit();
        PortsConfig();

        while (true) {
                if (buttom_flag) {
                        sys_ticks++;
                        num_of_set++;
                        buttom_flag = 0;
                }
                if (sys_ticks) {
                        sys_ticks++;
                }
                if (sys_ticks == 1000) {
                        if (num_of_set == 1) {
                                SetTime(digits_mas);
                        }
                        if (num_of_set > 1) {
                                SetTime(alarm_mas);
                        }
                        sys_ticks = 0;
                        num_of_set = 0;
                }
                SwitchDigit(ticks % 10, digits_mas);
        }
}

static void SetTime(int* set) {
        digits_mas_set[5] = { 0, 0, 0, 0, 0, 1 };
        buttom_flag = 0;
        sys_ticks = 0;
        num_of_set = 0;
        int j = 0;
        for (size_t i = 0; i < 5; ) {
                j++;
                if (buttom_flag) {
                        sys_ticks++;
                        num_of_set++;
                        buttom_flag = 0;
                }
                if (sys_ticks) {
                        sys_ticks++;
                }
                if (sys_ticks == 1000) {
                        if (num_of_set == 1) {
                                if (digits_mas[i] == stops_mas[i]) {
                                        digits_mas[i] = 0;
                                }
                                else {
                                        digits_mas[i]++;
                                }
                        }
                        if (num_of_set > 1) {
                                i++;
                        }
                        sys_ticks = 0;
                        num_of_set = 0;
                }
                SwitchDigit(j % 10, digits_mas_set);
        }
        set[0] = digits_mas_set[0];
        set[1] = digits_mas_set[1];
        set[2] = digits_mas_set[2];
        set[3] = digits_mas_set[3];
        set[4] = digits_mas_set[4];
        set[5] = digits_mas_set[5];
}

inline void ButtonInit(void) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);
        LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
        LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);
        LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
        NVIC_EnableIRQ(EXTI0_1_IRQn);
        NVIC_SetPriority(EXTI0_1_IRQn, 16);
}

void EXTI0_1_IRQHandler(void) {
        if(button_flag == 0) {
		button_flag = 1;
	}
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);	
}

inline void PortsConfig(void) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);

        LL_GPIO_SetPinMode(GREEN, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(BLUE, LL_GPIO_MODE_OUTPUT);

        LL_GPIO_SetPinMode(RING, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(COM1, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(COM2, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(COM3, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(COM4, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(SEG_A, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(SEG_B, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(SEG_C, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(SEG_D, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(SEG_E, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(SEG_F, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(SEG_G, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(SEG_DP, LL_GPIO_MODE_OUTPUT);
}

/**
  * System Clock Configuration
  * The system Clock is configured as follow :
  *    System Clock source            = PLL (HSI/2)
  *    SYSCLK(Hz)                     = 48000000
  *    HCLK(Hz)                       = 48000000
  *    AHB Prescaler                  = 1
  *    APB1 Prescaler                 = 1
  *    HSI Frequency(Hz)              = 8000000
  *    PLLMUL                         = 12
  *    Flash Latency(WS)              = 1
  */

void SystemClock_Config(void) {
        /* Set FLASH latency */
        LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

        /* Enable HSI and wait for activation*/
        LL_RCC_HSI_Enable();
        while (LL_RCC_HSI_IsReady() != 1) ;

        /* Main PLL configuration and activation */
        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2,
                                    LL_RCC_PLL_MUL_12);

        LL_RCC_PLL_Enable();
        while (LL_RCC_PLL_IsReady() != 1) ;

        /* Sysclk activation on the main PLL */
        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
        LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
        while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) ;

        /* Set APB1 prescaler */
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

        /* Set systick to 1ms */
        SysTick_Config(48000000 / 1000);

        /* Update CMSIS variable (which can be updated also
         * through SystemCoreClockUpdate function) */
        SystemCoreClock = 168000000;
}

void NMI_Handler(void) { }

void HardFault_Handler(void) {
                while (1) ;
}

void SVC_Handler(void) { }

void PendSV_Handler(void) { }

void SysTick_Handler(void) {
        if (ticks == -1) {
                LL_GPIO_TogglePin(COM1);
                LL_GPIO_TogglePin(COM2);
                LL_GPIO_TogglePin(COM3);
                LL_GPIO_TogglePin(COM4);
                ticks++;;
        }
        AlarmSound();
        ticks++;
        if (ticks == 1000) {
                DigInc();
                ticks = 0;
        }
}

static void AlarmSound(void) {
        if (IsItTime()) {
                if (alarm == -1) {
                        alarm = 1;
                }
        }
        if (alarm == 1) {
                LL_GPIO_TogglePin(RING);
        }
        alarm++;
        if (alarm == 5000) {
                alarm = -1;
                LL_GPIO_TogglePin(RING);
        }
}

static int IsItTime(void) {
        if (alarm_mas[0] == digits_mas[0] && alarm_mas[1] == digits_mas[1] \
        \ alarm_mas[2] == digits_mas[2] && alarm_mas[3] == digits_mas[3] \
        \ && alarm_mas[4] == digits_mas[4]) {
                return true;
        }
        else {
                return false;
        }
}

static void SetDigit(int digit) {
        if (digit > 9 || digit < 0) {
                LL_GPIO_TogglePin(SEG_G);
        }
        if (digit % 3 == 1) {
                if (digit == 4) {
                        LL_GPIO_TogglePin(SEG_G);
                        LL_GPIO_TogglePin(SEG_F);
                }
                if (digit == 7) {
                        LL_GPIO_TogglePin(SEG_A);
                }
        }
        else {
                LL_GPIO_TogglePin(SEG_A);
                LL_GPIO_TogglePin(SEG_D);
                if (digit != 0) {
                        LL_GPIO_TogglePin(SEG_G);
                }
                if (digit != 2 && digit != 3) {
                        LL_GPIO_TogglePin(SEG_F);
                }
                if (digit != 3 && digit != 9 && digit != 5) {
                        LL_GPIO_TogglePin(SEG_E);
                }
        }
        if (digit != 2) {
                LL_GPIO_TogglePin(SEG_C);
        }
        if (digit != 5 && digit != 6) {
                LL_GPIO_TogglePin(SEG_B);
        }
}

static void SwitchDigit(int num, int* mas) {
        if (num == 0 || num == 1) {
                SetDigit(mas[4]);
                LL_GPIO_TogglePin(COM1);
        }
        if (num == 2 || num == 3) {
                SetDigit(mas[3]);
                LL_GPIO_TogglePin(COM2);
        }
        if (num == 4 || num == 5) {
                SetDigit(mas[2]);
                LL_GPIO_TogglePin(COM3);
        }
        if (num == 6 || num == 7) {
                SetDigit(mas[1]);
                LL_GPIO_TogglePin(COM4);
        }
        if (num == 8 || num == 9) {
                if (mas[5]) {
                        LL_GPIO_TogglePin(SEG_DP);
                }
                LL_GPIO_TogglePin(COM2);
        }
} 

static void DigInc(void) {
        digits_mas[0]++;

        for (int i = 1; i < 4; i++) {
                digits_mas[i] += (digits_mas[i-1] == stops_mas[i-1]);
        }
        for (int i = 0; i < 4; i++) {
                if (digits_mas[i] == stops_mas[i]) {
                        digits_mas[i] = 0;
                }
        }

        if (digits_mas[5] == 1) {
                digits_mas[5] = 0;
        }
        else {
                digits_mas[5] = 1;
        }
}