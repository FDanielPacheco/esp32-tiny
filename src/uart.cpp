/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Introduction
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @file      uart.cpp
 * 
 * @version   1.0
 *
 * @date      17-10-2024
 *
 * @brief     Functions for configuring and using the UART interfaces on the ESP32-WROOM-32D
 * 
 * @author    Fábio D. Pacheco, 
 * @email     pacheco.castro.fabio@gmail.com
 *
 * @copyright Copyright (c) [2024] [Fábio D. Pacheco]
 * 
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Imported libraries
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#include <uart.h>

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Global objects and variables
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

UART uart0( UART0 );                                                           ///< Object associated with UART0, in order to use this peripheral, initialize it by calling uart0.init( )
UART uart1( UART1 );                                                           ///< Object associated with UART1, in order to use this peripheral, initialize it by calling uart1.init( )
UART uart2( UART2 );                                                           ///< Object associated with UART2, in order to use this peripheral, initialize it by calling uart2.init( )

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Object primitives
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Allocates resources associated with the UART object. Does not perform any hardware deconfiguration or reset.
 *
 * @param[in] option The desired UART to select.
 * 
 *    - `UART0` : The peripheral connected to the USB connector.
 * 
 *    - `UART1` : General use UART1, has RTS and CTS. 
 * 
 *    - `UART2` : General use UART2, has RTS and CTS.
 * 
 * @return none
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
UART::UART( uint8_t option ) : select( 0 ) { 
  switch( option ){
    default: break;
    case UART0: select = option; break;
    case UART1: select = option; break;
    case UART2: select = option; break;
  }
};

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Releases any allocated resources associated with the UART object. Does not perform any hardware deconfiguration or reset.
 *
 * @param none
 * 
 * @return none
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
UART::~UART( ) { };

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Configures and initializes the UART interface with the specified baud rate and settings defined in the `cfg` structure. Also initializes the necessary GPIO pins for UART communication.
 *
 * @param[in] baudrate The desired UART transmission speed in bits per second (bps).
 * 
 * @return none
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
void
UART::init( uint32_t baudrate ){
  cfg.UART_CLKDIV = CLKDIV( baudrate );
  cfg.UART_CLKDIV_FRAG = 0;

  // UART Configuration, 1 register
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_TICK_REF_ALWAYS_ON << 27, =  );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_DTR_INV            << 24, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_RTS_INV            << 23, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_TXD_INV            << 22, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_DSR_INV            << 21, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_CTS_INV            << 20, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_RXD_INV            << 19, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_TXFIFO_RST         << 18, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_RXFIFO_RST         << 17, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_IRDA_EN            << 16, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_TX_FLOW_EN         << 15, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_LOOPBACK           << 14, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_IRDA_RX_INV        << 13, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_IRDA_TX_INV        << 12, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_IRDA_WCTL          << 11, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_IRDA_TX_EN         << 10, |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_IRDA_DPLX          << 9,  |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_TXD_BRK            << 8,  |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_SW_DTR             << 7,  |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_SW_RTS             << 6,  |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_STOP_BIT_NUM       << 4,  |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_BIT_NUM            << 2,  |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_PARITY_EN          << 1,  |= );
  UPDATE_REGISTER( UART_CONF0_REG(select), cfg.UART_PARITY             << 0,  |= );

  // UART Configuration, 2 register
  UPDATE_REGISTER( UART_CONF1_REG(select), cfg.UART_RX_TOUT_EN         << 31,  = );
  UPDATE_REGISTER( UART_CONF1_REG(select), cfg.UART_RX_TOUT_THRHD      << 24, |= );
  UPDATE_REGISTER( UART_CONF1_REG(select), cfg.UART_RX_FLOW_EN         << 21, |= );
  UPDATE_REGISTER( UART_CONF1_REG(select), cfg.UART_RX_FLOW_THRHD      << 16, |= );
  UPDATE_REGISTER( UART_CONF1_REG(select), cfg.UART_TXFIFO_EMPTY_THRHD << 8,  |= );
  UPDATE_REGISTER( UART_CONF1_REG(select), cfg.UART_RXFIFO_FULL_THRHD  << 0,  |= );
  
  // UART Baudrate register
  UPDATE_REGISTER( UART_CLKDIV_REG(select), cfg.UART_CLKDIV_FRAG       << 20, = );
  UPDATE_REGISTER( UART_CLKDIV_REG(select), cfg.UART_CLKDIV            << 0 , |= );
  
  // if DMA is used

  // Assign pins mode
  gpio.mode( UART_TXD(select), OUTPUT );
  gpio.mode( UART_RXD(select), INPUT );
  
  // UART0 does not contain hardware flow control
  if( UART0 != select ){
    gpio.mode( UART_CTS(select), INPUT );
    gpio.mode( UART_RTS(select), OUTPUT );
  }

  // Assign pins function
  UPDATE_REGISTER( IO_MUX_PIN_REG( UART_TXD(select) ), UART_IO_MUX_PIN_FUNC( UART_TXD( select ) ) << 12, |= );
  UPDATE_REGISTER( IO_MUX_PIN_REG( UART_RXD(select) ), UART_IO_MUX_PIN_FUNC( UART_RXD( select ) ) << 12, |= );
  UPDATE_REGISTER( IO_MUX_PIN_REG( UART_CTS(select) ), UART_IO_MUX_PIN_FUNC( UART_CTS( select ) ) << 12, |= );
  UPDATE_REGISTER( IO_MUX_PIN_REG( UART_RTS(select) ), UART_IO_MUX_PIN_FUNC( UART_RTS( select ) ) << 12, |= );
}

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Sends the specified data to the selected UART interface.
 *
 * @param[in] str A pointer to the BYTE array containing the data to be transmitted.
 * @param[in] len The number of bytes to write from the BYTE array.
 * 
 * @return The number of bytes successfully written to the UART interface.
 * 
 * @note 
 *   - If no data is available to write or if the buffer points to NULL, the function returns 0.
 *
 *   - Ensure that `len` does not exceed the actual size of the array pointed to by `str`,
 *     providing a length larger than the array's size may result in undefined behavior.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
uint32_t
UART::write( const char * str, uint32_t len ){
  if( NULL == str )                                                            // Check if it's not a null pointer
    return 0;

  uint32_t bytes = 0;
  for( uint32_t i = 0 ; i < len ; ++i ){
    while( 0 != ( READ_REGISTER( UART_STATUS_REG(select) ) & (0xFF << 16) ) ); // Check the UART_STATUS_REG register and the bits UART_TXFIFO_CNT to see if there is information in hold                                                                             
    UPDATE_REGISTER( UART_FIFO_REG(select), str[i], = );                       // Insert the new information in the FIFO memory to be send
    bytes++;                                                                   // Increment the number of bytes sent
  }
  return bytes;
}

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Reads data from the UART FIFO register and writes it to the provided memory buffer. Ensures the read operation respects available data and allocated memory space.
 *
 * @param[out] str Pointer to the character buffer where the read data will be stored.
 * @param[in] len Maximum size of the buffer in bytes.
 * @param[in] offset Starting position in the buffer where data will be written.
 * 
 * @return The number of bytes successfully read and written to the buffer.
 * 
 * @note 
 *   - If no data is available to read or if the buffer lacks space, the function returns 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
uint32_t
UART::read( char * str, uint32_t len, uint32_t offset ){
  if( NULL == str )                                                            // Check if it's not a null pointer
    return 0;

  uint32_t a = available( );                                                   // How many bytes available
  uint32_t n = len - a - offset;                                               // Space available in memory for the variable

  if( !n || !a )                                                               // No memory available to write more information or no information available 
    return 0;

  uint32_t bytes = 0;
  for( uint32_t i = 0 ; i < a ; ++i ){
    str[i + offset] = (READ_REGISTER( UART_FIFO_REG(select) ) & 0xFF );        // Just return the first 8 bits, the rest are reserved  
    bytes++;
  }
  return bytes;
}

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Checks the UART status register to determine how many bytes of data are available in the RX FIFO.
 *
 * @param none
 *  
 * @return The number of bytes currently available in the RX FIFO.
 * 
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
uint32_t
UART::available(  ){
  uint32_t n = 0;
  n = (READ_REGISTER( UART_STATUS_REG(select) ) & (0xFF << 0));                // Store UART_STATUS_REG register and the bits UART_RXFIFO_CNT to see if there is information available
  return n;
}

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * End of file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/