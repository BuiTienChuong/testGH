void button_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
// set pa0 input floating
	uint32_t* GPIOA_MODER = (uint32_t*) (GPIOA_ADDR_BASE + 0x00);
	*GPIOA_MODER &= ~( 0b11 << 0 );  // SET GPIOA IN INPUT MODE

	uint32_t* GPIOA_PUPDR = (uint32_t*) (GPIOA_ADDR_BASE + 0x0C);
		*GPIOA_PUPDR &= ~( 0b11 << 0 );  // SET FLOATING
}

char read_button_state()
{
	uint32_t* GPIOA_IDR = (uint32_t*) (GPIOA_ADDR_BASE + 0x10);
		return (*GPIOA_IDR >> 0 & 1 );
}

void leds_init()
{
	// set pd 12-15 in push-pull output
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t* GPIOD_MODER = (uint32_t*) (GPIOD_ADDR_BASE + 0x00);
	*GPIOD_MODER |= (0b01 << 24); // SET PD12 OUTPUT
	*GPIOD_MODER |= (0b01 << 26); // SET PD13 OUTPUT
	*GPIOD_MODER |= (0b01 << 28); // SET PD14 OUTPUT
	*GPIOD_MODER |= (0b01 << 30); // SET PD15 OUTPUT

	uint32_t* GPIOD_OTYPER = (uint32_t*) (GPIOD_ADDR_BASE + 0x04);
	*GPIOD_OTYPER &= ~(0b1111 << 12); // SET PD12-15 PUSH-PULL
}

void led_control( char led_state)
{
	uint32_t* GPIOD_ODR = (uint32_t*) (GPIOD_ADDR_BASE + 0x14);

	if (led_state == 1)
	{
		*GPIOD_ODR |= 1<<12;
	}
	else *GPIOD_ODR &= ~(1<<12);

}