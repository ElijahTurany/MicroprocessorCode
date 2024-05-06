#include "bsp.h"
#include "stm32l476xx.h"

#define GPIOAEN		(1U<<0)
#define GPIOCEN		(1U<<2)

#define LED_PIN 	 5
#define PIN13       (1U<<13)
#define BTN_PIN		PIN13

//#define LED_PIN    5
#define LED_PORT   GPIOA
#define SERVO_PIN  0
#define SERVO_PORT GPIOA

//#Use PA0 pin to connect to a servo motor
void SERVO_Pin_Init()
{
	// Enable GPIOA clock
	RCC->AHB2ENR 		|= RCC_AHB2ENR_GPIOAEN; //typo here

	// Set GPIO moder to Alternative Function (AF) for timer mode; 
	// Step 1) clear bits in moder, step 2)Set bits to AF, step 3) set AF register to AF 2
	
	// Step 1: Clear bits
	SERVO_PORT->MODER  	&=~ (0x03 << (2*SERVO_PIN));  //typo here
	
	// Step 2: set AF mode: use Input(00), Output(01), Alternate Function(10), Analog(11)
	SERVO_PORT->MODER  	|=   0x02 << (2*SERVO_PIN); //typo here

	// 	Clear AF register
	SERVO_PORT->AFR[0] 	&=~ (0xF << (4*SERVO_PIN)); //typo here
	// step 3, set	AF to AF 2 for TIM5_CH1
	SERVO_PORT->AFR[0] 	|=   0x2 << (4*SERVO_PIN); //typo here

	//Set I/O output speed value as high speed
	// Speed clear bits
	SERVO_PORT->OSPEEDR  &=~ (0x03<<(2*SERVO_PIN)); //typo here
	// Set Speed
	SERVO_PORT->OSPEEDR  |=   0x03<<(2*SERVO_PIN); //typo here
	
	//Set I/O as no pull-up pull-down
	// No PUPD(00, reset), Pull-up(01), Pull-down(10), Reserved (11)
	SERVO_PORT->PUPDR    &=~ (0x03<<(2*SERVO_PIN));  //typo here
}

//Timer 2 Channel 1 creates 10ms pulse driving LED
void TIM2_CH1_Init()
{
	// Enable the timer clock //default clock is 4MHz
	// Enable TIMER clock
    RCC->APB1ENR1 	|= RCC_APB1ENR1_TIM2EN;  //typo here

	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM2->CR1 &=~ TIM_CR1_DIR;  //typo here
	
	//to create a 10ms pulse driving Green LED
    TIM2->PSC = 39;       // Prescaler = ?? need a value here
    
	// Auto-reload: Upcouting (0..ARR), Downcouting (ARR..0)
    TIM2->ARR = 1000;        //need a APR value here
    
	// Clear ouput compare mode bits for channel 1
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    
	// OC1M = 110 for PWM Mode 1 output on ch1
    // Output 1 preload enable
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

	// Select output polarity: 1 = active high, 0 = active low
	TIM2->CCMR1 &=~ TIM_CCER_CC1NP; // select active high

    // Enable output for ch1
	TIM2->CCER |= TIM_CCER_CC1E;

    // Main output enable (MOE): 0 = Disable, 1 = Enable
	TIM2->BDTR |= TIM_BDTR_MOE;

	// Output Compare Register for channel 1
	TIM2->CCR1  = 500;
	// Enable counter
	TIM2->CR1  |= TIM_CR1_CEN;
}

//Timer5 channel 1 creates 20ms pulse for driving servo motor
void TIM5_CH1_Init()
{
	// Enable the timer clock
	// Enable TIMER clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;

	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM5->CR1 &= ~TIM_CR1_DIR;

    TIM5->PSC = 39;       // Prescaler = ?? need a value here
    // Auto-reload: Upcouting (0..ARR), Downcouting (ARR..0)
    TIM5->ARR = 2000;   //need a value here
    // Clear ouput compare mode bits for channel 1
	TIM5->CCMR1 &= ~TIM_CCMR1_OC1M;
	// OC1M = 110 for PWM Mode 1 output on ch1
    TIM5->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    // Output 1 preload enable
    TIM5->CCMR1 |= TIM_CCMR1_OC1PE;

	// Select output polarity: 0 = active high, 1 = active low
	TIM5->CCMR1 |= TIM_CCER_CC1NP; // select active high

    // Enable output for ch1
	TIM5->CCER |= TIM_CCER_CC1E;

    // Main output enable (MOE): 0 = Disable, 1 = Enable
	TIM5->BDTR |= TIM_BDTR_MOE;

	// Output Compare Register for channel 1
	TIM5->CCR1  = 1000;
	// Enable counter
	TIM5->CR1  |= TIM_CR1_CEN;
}

void Servo_Move_Left_90_Degree()
{ // 1.7ms
	TIM5->CCR1  = 170;  //need a value
	delayMs(180);
	Servo_Move_Center();
}

void Servo_Move_Center()
{ // 1.5ms
	TIM5->CCR1  = 150;  //need a value
}

void Servo_Move_Right_90_Degree()
{ // 1.3ms
	TIM5->CCR1  = 130;  //need a value
	delayMs(180);
	Servo_Move_Center();
}

void led_dimmer()
{
	int brightness = 0;
	int n = 1;
	while(1)
	{
		// if brightness maximum/maximum change direction
		if (((brightness + n) >= 999) || ((brightness + n) <= 0))//need a max value here
		n = -n;
		brightness += n;
		// set brightness for channel 1
		TIM2->CCR1 = brightness;
		delayMs(5);
    }
}

void delayMs(int n)
	{
		int i, j;
			for(i = 0 ; i < n; i++)
				for(j = 0; j < 800; j++) //was 7000
						{} /* do nothing */
	}


void led_init(void)
{
		// Enable the peripheral clock of GPIO Port
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   		//typo here
		
		// Set GPIO Mode to AlterFunc(10)
		GPIOA->MODER &=~ (3U<<(2*LED_PIN));    //typo here
		GPIOA->MODER |=   2U<<(2*LED_PIN);    //typo here
		
		// 	clear bits for AF register
		GPIOA->AFR[0] 	&= ~(0x0 << (4*LED_PIN)); 	 //typo here
		
		//	Set AF to AF1 to select TIM2_CH1
		GPIOA->AFR[0] 	|=   0x1 << (4*LED_PIN);  	//typo here

		// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
		GPIOA->OSPEEDR &= ~(3U<<(2*LED_PIN));
		GPIOA->OSPEEDR |=   3U<<(2*LED_PIN);  // select High speed

		// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
		GPIOA->OTYPER &= ~(1U<<LED_PIN);       // Push-pull

		// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
		GPIOA->PUPDR   &= ~(3U<<(2*LED_PIN));  // No pull-up, no pull-down
}

void led_on(void)
{
	/*set PA5 high*/
    GPIOA->ODR |=LED_PIN;
}

void led_off(void)
{
	/*set PA5 low*/
	 GPIOA->ODR &=~LED_PIN;
}

 void button_init(void)
{
	/*Enable clock access to PORTC*/
	RCC->AHB2ENR |=GPIOCEN;
	/*Set PC13 as an input pin*/
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);
}


/*bool get_btn_state(void)
{
	check if button is pressed and button is active low
	if(GPIOC->IDR & BTN_PIN)  //missing code here
	{
		return false; //because of active low button
	}
	else
	{
			return true;
	}

}
*/
