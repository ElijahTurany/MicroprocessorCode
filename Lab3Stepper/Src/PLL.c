#include "PLL.h"
#include "stm32l476xx.h"

//******************************************
// Configure PLL as system clock source
//******************************************
void System_Clock_Init(void){

	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
    // must be correctly programmed according to the frequency of the CPU clock
    // (HCLK) and the supply voltage of the device.
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;

	/* Enable the Internal High Speed oscillator (HSI)*/
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);

	/*Reset PLL (PLL off)*/
	RCC->CR    &= ~RCC_CR_PLLON;

	/* Select HSI (16MHz) as a clock source for PLL*/
	/* 00 = No clock, 01 = MSI, 10 = HSI, 11 = HSE*/
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	/*HSI 16MHz selected*/
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;

	// Create PLL clock 80 MHz from HSI 16MHz
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) =
	//16MHz * 20/2 = 160 MHz
	// f(PLL_R) = f(VCO clock) / PLLR = 160MHz/2 = 80MHz
	/* 000: PLLM = 1, 001: PLLM = 2, 010: PLLM = 3, 011: PLLM = 4,
	 * 100: PLLM = 5, 101: PLLM = 6, 110: PLLM = 7, 111: PLLM = 8*/
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 20U << 8; //missing code here
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 1U << 4; //missing code here

	/* Enable PLLR parameter and select its value*/
	/*00: PLLR = 2, 01: PLLR = 4, 10: PLLR = 6, 11: PLLR = 8*/
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

	/*Enable PLL*/
	RCC->CR   |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0);

	// PLL selected as the system clock
	// 00: MSI, 01:HSI, 10: HSE, 11: PLL
	RCC->CFGR &= ~RCC_CFGR_SW; //clear system clock selection bits
	RCC->CFGR |= RCC_CFGR_SW_PLL; //select PLL as system clock

	// Wait until System Clock (PLL) has been selected
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	// The maximum frequency of the AHB is 80MHz,
	//pre scaler is 1 for APB1 and the APB2 to use 80 MHz.
	/* AHB pre scaler = 1; SYSCLK not divided*/
	RCC->CFGR &= ~RCC_CFGR_HPRE;
	/* APB high-speed pre scaler (APB1) = 1, HCLK not divided*/
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	/*APB high-speed pre scaler (APB2) = 1, HCLK not divided*/
	RCC->CFGR &= ~RCC_CFGR_PPRE2;
}
