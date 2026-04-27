
void UART1_init(){

	__HAL_RCC_GPIOB_CLK_ENABLE();
	// set pb6 as uart1_tx( AF07 ) ; set pb7 as uart1_rx ( AF07 )
	uint32_t* GPIOB_MODER = (uint32_t*) (GPIOB_ADDR_BASE + 0x00);
		*GPIOB_MODER &= ~( 0b1111 << 12 );
        *GPIOB_MODER |= ( 0b10 << 12 ) | ( 0b10 << 14 ) ;      // SET GPIOB IN ALTERNATE MODE

    uint32_t* GPIOB_AFRL = (uint32_t*) (GPIOB_ADDR_BASE + 0x20);
		*GPIOB_AFRL &= ~( 0xFF << 24 );
        *GPIOB_AFRL |= ( 7 << 24 ) | ( 7 << 28 ) ;  //SET AF07 FOR PB6-7

       // UART:
       //  + baudrate : 9600
       //  + frame
       //     * data len : 8 bit
       //     * parity check: none

       __HAL_RCC_USART1_CLK_ENABLE();   // RCC 16Mhz

       uint32_t* BRR = (uint32_t*) (USART1_ADDR_BASE + 0x08);
       		*BRR = ( 104 << 4 ) | ( 3 << 0 ); // BAUDRATE = 9600

       uint32_t* CR1 = (uint32_t*) (USART1_ADDR_BASE + 0x0C);
       		*CR1 &= ~( 1 << 10 ) ; // DISABLE PARITY
       		*CR1 &= ~( 1 << 12 ) ; // DATA LEN = 8 BIT
       		*CR1 |= ( 1 << 13 ) | ( 1 << 2 ) | ( 1 << 3 ) ; // enable usart, trans, receive

       		// enable RXNE interrupt
       		// WHEN RXNE is set, uart1 send interrupt event to NVIC
       	     	*CR1 |= ( 1 << 5 ) ;
       		// NVIC accept interrupt event, from UART1
       		uint32_t* ISER1 = (uint32_t*) (0xE000E104);
       		*ISER1 |= (1 << 5 );
       }

void UART1_SEND_1BYTE( char data )
{
	// before transmitting data, check the " status register "

	uint32_t* SR = (uint32_t*) (USART1_ADDR_BASE + 0x00);
	uint32_t* DR = (uint32_t*) (USART1_ADDR_BASE + 0x04);

	       	while ( ((*SR >> 7) & 1 ) == 0 );  // wait DR empty
	       	*DR = data;                    // write data to DR
	    	while ( ((*SR >> 6) & 1 ) == 0 );  // wait trans complete
            *SR &= ~( 1 << 6 );              // clear TC flag
}

void UART1_SEND_STRING( char * msg)
{
	int msg_len =  strlen(msg);
	for(int i=0;i<msg_len;i++)
	{
		UART1_SEND_1BYTE(msg[i]);
	}
}

char UART1_RECV_1BYTE()
{
	uint32_t* SR = (uint32_t*) (USART1_ADDR_BASE + 0x00);
	uint32_t* DR = (uint32_t*) (USART1_ADDR_BASE + 0x04);

	while ( ((*SR >> 5) & 1 ) == 0 ); // wait receive data ( RXNE flag )
	char recv_data = *DR;
	 return recv_data;
}

// CORE -> STARTUP ->g_pfnVector to check the function
int rx_index = 0;
void USART1_IRQHandler()
{
	uint32_t* DR = (uint32_t*) (USART1_ADDR_BASE + 0x04);
	recv_data[rx_index++] = *DR;
	//Clear RXNE by reading the DR register or Write bit 0
}