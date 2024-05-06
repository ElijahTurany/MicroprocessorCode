/*This is RTC.C*/
#include "RTC.h"
#include "stm32l4xx.h"
#include "stm32l476xx.h"

//Power interface clock enable
#define PWREN			(1U<<28)
//Enable backup domain write protection
#define CR_DBP			(1U<<8)
//LSI oscillator enable
#define CSR_LSION   	(1U<<0)
//LSI oscillator ready
#define CSR_LSIRDY   	(1U<<1)
//Backup domain software reset
#define BDCR_BDRST		(1U<<16)
//RTC Enable
#define RTCEN			(1U<<15)
//Hour format in CR register
#define CR_FMT			(1U<<6)
//Initialization mode for DR and TR registers
#define ISR_INIT		(1U<<7)
//Registers synchronization flag
#define ISR_RSF         (1U<<5)
//read 38.3.7 RTC initialization and configuration
/*After Backup domain reset, all the RTC registers
 * are write-protected. Writing to the RTC
registers is enabled by writing a key into the
Write Protection register, RTC_WPR. The keys are ?? and ??*/
#define RTC_WRITE_PROTECTION_ENABLE_1	((uint8_t)0xCAU)   //00 is an place holder for key
#define RTC_WRITE_PROTECTION_ENABLE_2	((uint8_t)0x53U)

#define RTC_INIT_MASK	0xFFFFFFFFU

#define RTC_ASYNCH_PREDIV		((uint32_t)0x7F)
#define RTC_SYNCH_PREDIV		((uint32_t)0x00F9)

#define MONTH_JANUARY           ((uint8_t)0x01U)  /* January */
#define MONTH_FEBRUARY          ((uint8_t)0x02U)  /* January */
#define MONTH_MARCH             ((uint8_t)0x03U)  /* January */
#define MONTH_APRIL             ((uint8_t)0x04U)  /* January */
#define MONTH_MAY               ((uint8_t)0x05U)  /* January */
#define MONTH_JUNE              ((uint8_t)0x06U)  /* January */
#define MONTH_JULY              ((uint8_t)0x07U)  /* January */
#define MONTH_AUGUST            ((uint8_t)0x08U)  /* January */
#define MONTH_SEPTEMBER         ((uint8_t)0x09U)  /* January */
#define MONTH_OCTOBER           ((uint8_t)0x10U)  /* January */
#define MONTH_NOVEMBER          ((uint8_t)0x11U)  /* January */
#define MONTH_DECEMBER          ((uint8_t)0x12U)   /* December */

#define WEEKDAY_MONDAY      	((uint8_t)0x01U) /*  Monday */
#define WEEKDAY_TUESDAY      	((uint8_t)0x02U) /*  Monday */
#define WEEKDAY_WEDNESDAY      	((uint8_t)0x03U) /*  Monday */
#define WEEKDAY_THURSDAY      	((uint8_t)0x04U) /*  Monday */
#define WEEKDAY_FRIDAY      	((uint8_t)0x05U) /*  Monday */
#define WEEKDAY_SATURDAY      	((uint8_t)0x06U) /*  Monday */
#define WEEKDAY_SUNDAY      	((uint8_t)0x07U) /* Sunday   */
/*fill int code to define all other weekdays with an equivalent hex value */
#define TIME_FORMAT_PM			(1U<<22)

static uint32_t rtc_init_seq(void);
static void rtc_set_asyc_prescaler(uint32_t AsyncPrescaler);
static void rtc_set_syc_prescaler(uint32_t AsyncPrescaler);
static uint32_t exit_init_seq(void);

static void rtc_date_config(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year);
static void rtc_time_config(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds);

void rtc_init(void)
{
	/*Enable Clock access to PWR (power module in block diagram)*/
    //RCC->APB1ENR |=PWREN;
	RCC->APB1ENR1 |=PWREN; //Power interface clock enable
	/*Enable Backup access write protection to configure RTC registers (DBP)*/
    PWR->CR1  |= CR_DBP;
	/*Enable low speed speed Internal clock (LSI) see RCC_CSR*/
    /*Set LSION to 1 to turn on LSI RC oscillator ON at bit 0*/
    RCC->CSR |=CSR_LSION;
	/*Wait for LSI to be enabled see LDIRDY*/
    while ((RCC->CSR & CSR_LSIRDY)!= CSR_LSIRDY){}
	/*Force backup domain to reset, see RCC_BDCR->BDRST*/
    RCC->BDCR |=BDCR_BDRST;
	/*Release backup domain reset*/
    RCC->BDCR &=~BDCR_BDRST;
	/*Set RTC clock source to LSI, select RTCSEL to 10 */
    RCC->BDCR &=~(1U<<8);
    RCC->BDCR |=(1U<<9);
	/*Enable RTC*/
    RCC->BDCR |=RTCEN;
	/*see RTC_WPR. RTC registers are locked for write protection register*/
    /*write these two protection keys below and then initialize the RTC */
	/*The two protection keys info. is provided in the STM32 Ref. manual*/
    RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_1;
    RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_2;

    /*Enter initialization mode*/
    if(rtc_init_seq() != 1)
    {
    	/*initialization error*/
    }
	/*Set hour format to 24 hr format using the bit 6, set to 0 */
    /*Set hour format to AM/PM using the bit 6, set to 1 */
    RTC->CR |=CR_FMT;

	/*Set Asynchronous prescaler*/
    rtc_set_asyc_prescaler(RTC_ASYNCH_PREDIV);
	/*Set Synchronous prescaler*/
    rtc_set_syc_prescaler(RTC_SYNCH_PREDIV);

    /*Set date: Friday 24th, Feb. 2023*/
    rtc_date_config(WEEKDAY_MONDAY, 0x08, MONTH_APRIL, 0x24);

    	/*Set time : 11:58:00 FM */
    rtc_time_config(TIME_FORMAT_PM, 0x2, 0x05, 0x00);

	/*Exit initialization mode*/
    exit_init_seq();
	/*Enable RTC write protection*/
    RTC->WPR = 0xFF;
}

static void rtc_date_config(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year)
{
	register uint32_t temp = 0U;

	temp = (WeekDay << RTC_DR_WDU_Pos)   | \
       (((Year & 0xF0U) << (RTC_DR_YT_Pos - 4U)) | ((Year & 0x0FU) << RTC_DR_YU_Pos))   | \
       (((Month & 0xF0U) << (RTC_DR_MT_Pos - 4U)) | ((Month & 0x0FU) << RTC_DR_MU_Pos)) | \
       (((Day & 0xF0U) << (RTC_DR_DT_Pos - 4U)) | ((Day & 0x0FU) << RTC_DR_DU_Pos));
   MODIFY_REG(RTC->DR, (RTC_DR_WDU | RTC_DR_MT | RTC_DR_MU | RTC_DR_DT | RTC_DR_DU |
		   RTC_DR_YT | RTC_DR_YU), temp);
}

static void rtc_time_config(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes,
		uint32_t Seconds)
{
	register uint32_t temp = 0U;

    temp = Format12_24                                                            | \
(((Hours & 0xF0U) << (RTC_TR_HT_Pos - 4U)) | ((Hours & 0x0FU) << RTC_TR_HU_Pos))     | \
(((Minutes & 0xF0U) << (RTC_TR_MNT_Pos - 4U)) | ((Minutes & 0x0FU) << RTC_TR_MNU_Pos)) | \
(((Seconds & 0xF0U) << (RTC_TR_ST_Pos - 4U)) | ((Seconds & 0x0FU) << RTC_TR_SU_Pos));

MODIFY_REG(RTC->TR, (RTC_TR_PM | RTC_TR_HT | RTC_TR_HU | RTC_TR_MNT | RTC_TR_MNU |
		RTC_TR_ST | RTC_TR_SU), temp);
}

static void rtc_set_asyc_prescaler(uint32_t AsynchPrescaler)
{
/*see RTC_PRER */
	MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_A, AsynchPrescaler << RTC_PRER_PREDIV_A_Pos);
/*this function takes 3, 1st is PRER to be modified, 2nd clear it, 3rd set new value*/
/*shift 16U times to the RTC_PRER, then write the passed-in value from the AsyncPrescaler*/
}

static void rtc_set_syc_prescaler(uint32_t SynchPrescaler)
{
/*see RTC_PRER */
	MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_S, SynchPrescaler);
/*this function takes 3, 1st is PRER to be modified, 2nd clear it, 3rd set new value*/
/*No shift required as it starts from bit 0*/
}

static void rtc_enable_init_mode(void)
{
	/*use rtc_isr*/
	RTC->ISR = RTC_INIT_MASK;
}

static uint32_t rtc_isActiveflag_init(void)
{
	/**/
	return (READ_BIT(RTC->ISR, RTC_ISR_INITF) == (RTC_ISR_INITF));
}


static uint32_t rtc_init_seq(void)
{
	/*Set initialization mode (check initialization flag)*/
	rtc_enable_init_mode();
	/*Check initialization mode is set*/
	while(rtc_isActiveflag_init() !=1){}
	return 1;
}

static void rtc_exit_init_mode(void)
{
	/*use rtc_isr*/
	RTC->ISR = 0;
}

void rtc_clearflag_rs(void)
{
	WRITE_REG(RTC->ISR, (~((RTC_ISR_RSF | RTC_ISR_INIT) & 0x0000FFFFU) |
			(RTC->ISR & RTC_ISR_INIT)));
   /*this function takes 2 arguments*/
}

static uint32_t rtc_isActiveflag_rs(void)
{
	return (READ_BIT(RTC->ISR, RTC_ISR_RSF)==(RTC_ISR_RSF));
}


static uint32_t wait_for_synchro(void)
{
	/*Clear RSF flag*/
	rtc_clearflag_rs();
	/*wait for registers to be synchronized*/
	while(rtc_isActiveflag_rs() != 1)
	{

	}
	return 0;
}


static uint32_t exit_init_seq(void)
{
	/*Disable rtc_init_mode*/
	rtc_exit_init_mode();
	/*Wait for registers to be synchronized*/
	return (wait_for_synchro());
}

uint8_t rtc_convert_bin2bcd(uint8_t value)
{
	return (uint8_t)((((value) / 10U) << 4U) | ((value) % 10U));
}

uint8_t rtc_convert_bcd2bin(uint8_t value)
{
	return (uint8_t)(((uint8_t)((value) & (uint8_t)0xF0U) >> (uint8_t)0x4U) *
			10U + ((value) & (uint8_t)0x0FU));
}

uint32_t rtc_date_get_day(void)
{
	return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_DT | RTC_DR_DU ))) >> RTC_DR_DU_Pos);
}

uint32_t rtc_date_get_year(void)
{
	return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_YT | RTC_DR_YU ))) >> RTC_DR_YU_Pos);

}

uint32_t rtc_date_get_month(void)
{
	return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_MT | RTC_DR_MU ))) >> RTC_DR_MU_Pos);

}

uint32_t rtc_date_get_second(void)
{
	return (uint32_t)((READ_BIT(RTC->TR, (RTC_TR_ST | RTC_TR_SU ))) >> RTC_TR_SU_Pos);

}

uint32_t rtc_date_get_minute(void)
{
	return (uint32_t)((READ_BIT(RTC->TR, (RTC_TR_MNT | RTC_TR_MNU ))) >> RTC_TR_MNU_Pos);

}

uint32_t rtc_date_get_hour(void)
{
	return (uint32_t)((READ_BIT(RTC->TR, (RTC_TR_HT | RTC_TR_HU ))) >> RTC_TR_HU_Pos);

}

