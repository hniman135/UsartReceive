
/*====================================================================================
 * 								INCLUDE FILES
=================================================================================== */
#include "at32f403a_407_board.h"
#include "at32f403a_407_clock.h"

/*====================================================================================
 * 								LOCAL FUNTION PROTOTYPES
====================================================================================*/
void usart_configuration(void);

/*====================================================================================
 * 								GLOBAL FUNTIONS
====================================================================================*/
int main(void)
{
	/*The system clock is configured as follow:
		  *         system clock (sclk)   = hext / 2 * pll_mult
		  *         system clock source   = pll (hext)
		  *         - hext                = HEXT_VALUE
		  *         - sclk                = 240000000
		  *         - ahbdiv              = 1
		  *         - ahbclk              = 240000000
		  *         - apb2div             = 2
		  *         - apb2clk             = 120000000
		  *         - apb1div             = 2
		  *         - apb1clk             = 120000000
		  *         - pll_mult            = 60
		  *         - pll_range           = GT72MHZ (greater than 72 mhz)*/
  system_clock_config();
  at32_board_init();
  at32_led_off(LED2);
  at32_led_off(LED3);
  at32_led_off(LED4);
  usart_configuration();


  while(1)	/*Get accurate data to toggle LED status:
  	  	  	  	  *LED2: 0x02
  	  	  	  	  *LED3: 0x03
  	  	  	  	  *LED4: 0x04							*/
  {
	  while(usart_flag_get(USART1, USART_RDBF_FLAG) == RESET);
	    if(usart_data_receive(USART1) == 0x02)
	    {
	      at32_led_toggle(LED2);
	    }
	    else if(usart_data_receive(USART1) == 0x03)
	    {
	      at32_led_toggle(LED3);
	    }
	    else if(usart_data_receive(USART1) == 0x04)
		{
		  at32_led_toggle(LED4);
		}
  }
}

/*====================================================================================
 * 								LOCAL FUNTIONS
====================================================================================*/
/*Config USART1: Enable Transmitter and Receiver Mode for USART 1*/
void usart_configuration(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the usart1 and gpio clock */
  crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the usart1 tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the usart1 rx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure usart1 param */
  usart_init(USART1, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART1, TRUE);
  usart_receiver_enable(USART1, TRUE);
  usart_enable(USART1, TRUE);
}
