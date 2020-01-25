
#include <cstdio>

#include "main.hpp"
#include "string.h"
#include "system_init.h"
#include "debug.h"

const uint64_t devAddr = 0x2601163D;
const uint8_t nwkSKey[16] = {0xBE,0xB8,0xC0,0x7F,0xA0,0x85,0xAA,0x6B,0x6D,0xD5,0xB1,0x46,0xC6,0xC5,0xC1,0x8D}; //"BEB8C07FA085AA6B6DD5B146C6C5C18D";
const uint8_t appSKey[16] = {0x2C,0xF1,0xAE,0xA8,0x78,0xF9,0xCB,0x80,0x25,0xA4,0xF7,0x85,0x1A,0xA2,0x4A,0xFF};//"2CF1AEA878F9CB8025A4F7851AA24AFF";

const uint8_t* test_packet = (uint8_t*)("Hello world");

static constexpr uint64_t lora_frq = 868100000;

// Buffers
uint8_t sys_uart_rx_buffer[255];
uint8_t sys_uart_tx_buffer[255];
uint8_t cmd_buffer[255];

// STM HAL instances
ADC_HandleTypeDef hadc;
CRC_HandleTypeDef hcrc;
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

// Pins
STM32OutputPin led_gpio(GPIOA, GPIO_PIN_15);
STM32OutputPin gps_on(GPIOB, GPIO_PIN_0);
STM32OutputPin gps_rst(GPIOA, GPIO_PIN_1);
STM32OutputPin lora_select(GPIOA, GPIO_PIN_4);
STM32OutputPin lora_reset(GPIOA, GPIO_PIN_8);
//STM32OutputPin lora_dio0(GPIOA, GPIO_PIN_11);
STM32OutputPin extflash_select(GPIOB, GPIO_PIN_10);

STM32OutputPin* output_pins[] = {&lora_select, &lora_reset, &led_gpio, &gps_on, &extflash_select};

// Peripherals
STM32_I2C i2c2(I2C2);
STM32Spi lora_spi(SPI1, lora_select);
STM32Spi extflash_spi(SPI1, extflash_select);
STM32_FIFO_UART sys_uart(&huart1, { sys_uart_rx_buffer, sizeof(sys_uart_rx_buffer) }, { sys_uart_tx_buffer, sizeof(sys_uart_tx_buffer) });

// Devices
Bar_MS5637 barometer(i2c2);
Temp_PCT2075 temp_sensor(i2c2);
AT25SF041 extflash(extflash_spi);
SemtechSpiDev semtech_dev(lora_spi, lora_reset, lora_reset);
Sx1276_Lora lora_radio(semtech_dev);

// Others
LoRaWANPacketGen lorawan_packet_gen(nwkSKey, appSKey, static_cast<uint32_t>(devAddr));
Commands cmd({ cmd_buffer, sizeof(cmd_buffer) });


extern "C" void USART1_IRQHandler()
{
	sys_uart.irq();
}

State_struct	st;
Ms_tmr_struct 	ms_tmr;
Sec_tmr_struct 	sec_tmr;
Led_struct 		led;
Flag_struct		flags;


// ----------------------------------------------------------------------------
static void handle_led(volatile uint32_t & tmr, STM32OutputPin & pin)
{
	if(tmr){
		pin.set();
		tmr--;
		if(!tmr){
			pin.clear();
		}
	}
	else
		pin.clear();
}

// ----------------------------------------------------------------------------
static void handle_tmr_flags(volatile uint32_t & tmr, volatile bool & flag)
{
	if(tmr){
		tmr--;
		if(!tmr){
			flag = true;
		}
	}
}

// ----------------------------------------------------------------------------
void systick_handler()
{
	handle_led(led.red, led_gpio);
}

// ----------------------------------------------------------------------------
void sec_handler()
{
	st.uptime++;

	led.red = 2;

	handle_tmr_flags(sec_tmr.measurement, flags.measurement);
}

// ----------------------------------------------------------------------------
bool can_stop()
{
	bool led_active = led.red;

	// TODO dalsi podminky pro povoleni usnuti

	return !led_active;
}




/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();

  delay_ms(50); // stabilizace napeti pro flashku

  SystemClock_Config();

  volatile int temp = 0;

  MX_GPIO_Init();
  MX_ADC_Init();
  MX_CRC_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  for(uint8_t i=0; i<(sizeof(output_pins)/sizeof(output_pins[0])); i++)
	  output_pins[i]->Init();

  gps_on.set();
  delay_ms(50);
  //gps_on.clear();
  //delay_ms(50);

  barometer.Init();
  temp_sensor.Init();
  lora_select.set();
  lora_reset.set();
  extflash_select.set();

  lora_spi.Init();
  extflash_spi.Init();

  lora_radio.InitLoraRadio(868100000);

  bool flash_init = extflash.init();

  DataRef pck = {(uint8_t*)test_packet, 11};
  DataRef pck_raw = {nullptr, 0};
  char buff[256];

  /*
  HAL_UART_Transmit(&huart2, (uint8_t*)"$PMTK104*37\r\n", 13, 1000);
  HAL_UART_Receive(&huart2, (uint8_t*)buff, 1024, 50);
  HAL_UART_Abort(&huart2);
  HAL_UART_Transmit(&huart2, (uint8_t*)"$PMTK300,1000,0,0,0,0*1C\r\n", 26, 1000);
  HAL_UART_Transmit(&huart2, (uint8_t*)"$PMTK314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,1,0*2D\r\n", 51, 1000);
  */

  uint8_t* t =(uint8_t*)"$PMTK000*32\r\n";
  volatile uint32_t time_to_fix_sec;

  if(flash_init)
	  led.red = 1000;
  else
	  led.red = 10;

  I2C_MUX(false);
  //SPI_MUX(false);
  //USART1_MUX(false);
  USART2_MUX(false);

  sys_uart.rx_start();

  cmd.implicit_lf(true);

  while (1)
  {
	  if(flags.sec)
	  {
		  flags.sec = false;

		  sec_handler();
	  }

	  cmd.task();

	  if(can_stop())
	  {// Je mozne prejit do stopu a probudit se az na RTC

		  //HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	  }
	  else
	  {// zbyva dokoncit tasky

		  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	  }

	  /*
	  memset(buff,0,256);
	  //HAL_UART_Transmit(&huart2, t, 13, 1000);
	  HAL_UART_Abort(&huart2);
	  HAL_UART_Receive(&huart2, (uint8_t*)buff, 255, 500);

	  for(int i=0;i<256;i++)
	  {

		  if (strncmp(&buff[i],"$GPGGA,",7)==0)
		  {
				if(buff[i+18] != 0x2c && buff[i+18] != 0)
				{
						HAL_RTC_GetTime(&hrtc, &nTime, RTC_FORMAT_BIN);
						HAL_RTC_GetDate(&hrtc, &yy, RTC_FORMAT_BIN);
						if(nTime.Minutes != sTime.Minutes)
							time_to_fix_sec = (((nTime.Minutes - sTime.Minutes)*60) - sTime.Seconds) + nTime.Seconds;
						else
							time_to_fix_sec = nTime.Seconds -  sTime.Seconds;
						led_gpio.set();
						HAL_Delay(10);
						led_gpio.clear();
						HAL_Delay(10);
				}

		  }

		  if (strncmp(&buff[i],"$GPGSV,",7)==0)
		  {
			  if(buff[i+12] != 0x30 && buff[i+12] != 0)
			  {
				  led_gpio.set();
				  HAL_Delay(10);
				  led_gpio.clear();
				  HAL_Delay(10);
			  }
		  }
	  }
	  */

	//  temp = temp_sensor.get_temp();
	 // lorawan_packet_gen.Gen_PacketDataUp(true, pck, pck_raw);
	//  lora_radio.sendPacket(pck_raw);
	 // while(lora_radio.txActive());

  }

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
