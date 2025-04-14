/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Introduction
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @file      spi.cpp
 * 
 * @version   1.1
 *
 * @date      04-11-2024, 27-12-2024
 *
 * @brief     Functions to use the SPI peripherals in the ESP32-WROOM32D
 * 
 * @author    Fábio D. Pacheco, 
 * @email     pacheco.castro.fabio@gmail.com
 *
 * @copyright Copyright (c) [2024] [Fábio D. Pacheco]
 * 
 * @note V1.1 - Added multibyte per transfer
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Imported libraries
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#include <spi.h>

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Global objects and variables
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

SPI spi0( SPI0 );                                                              //!< Object associated with SPI0, in order to use this peripheral, initialize it by calling spi0.init( )
SPI spi1( SPI1 );                                                              //!< Object associated with SPI1, in order to use this peripheral, initialize it by calling spi1.init( )
SPI spi2( SPI2 );                                                              //!< Object associated with SPI2, in order to use this peripheral, initialize it by calling spi2.init( )
SPI spi3( SPI3 );                                                              //!< Object associated with SPI3, in order to use this peripheral, initialize it by calling spi3.init( )

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Object primitives
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Allocates resources associated with the SPI object. Does not perform any hardware deconfiguration or reset.
 *
 * @param[in] option The desired SPI to select.
 * 
 *    - `SPI0` : Serial peripheral interface 0.
 * 
 *    - `SPI1` : Serial peripheral interface 1 
 * 
 *    - `SPI2` : Serial peripheral interface 2
 *
 *    - `SPI3` : Serial peripheral interface 3
 * 
 * @return none
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
SPI::SPI( uint8_t option ) : select( 0 ) { 
  switch( option ){
    default: break;
    case SPI0: select = option; break;
    case SPI1: select = option; break;
    case SPI2: select = option; break;
    case SPI3: select = option; break;
  }
};

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Releases any allocated resources associated with the SPI object. Does not perform any hardware deconfiguration or reset.
 *
 * @param none
 * 
 * @return none
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
SPI::~SPI( ) { };

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Configures and initializes the SPI interface with the specified frequency transmission (`freq`) and settings defined in the `cfg` and `cmd` structure. Also initializes the necessary GPIO pins for SPI communication.
 *
 * @param[in] freq The desired SPI transmission speed in Hertz (Hz).
 * 
 * @return none
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
void 
SPI::init( uint64_t freq ){
  // Give the clock to the peripheral
  enable( HIGH );

  // Configuration 1 (CTRL)
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_WR_BIT_ORDER    << 26 ,  = );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_RD_BIT_ORDER    << 25 , |= );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_FREAD_QIO       << 24 , |= );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_FREAD_DIO       << 23 , |= );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_WP              << 21 , |= );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_FREAD_QUAD      << 20 , |= );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_RESERVED_CTRL_1 << 15 , |= );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_FREAD_DUAL      << 14 , |= );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_FASTRD_MODE     << 13 , |= );
  UPDATE_REGISTER( SPI_CTRL_REG(select),  cfg.SPI_RESERVED_CTRL_2 << 0  , |= );

  // Configuration 2 (CTRL2)
  UPDATE_REGISTER( SPI_CTRL2_REG(select), cfg.SPI_MOSI_DELAY_NUM  << 23 ,  = );
  UPDATE_REGISTER( SPI_CTRL2_REG(select), cfg.SPI_MOSI_DELAY_MODE << 21 , |= );
  UPDATE_REGISTER( SPI_CTRL2_REG(select), cfg.SPI_MISO_DELAY_NUM  << 18 , |= );
  UPDATE_REGISTER( SPI_CTRL2_REG(select), cfg.SPI_MISO_DELAY_MODE << 16 , |= );
  UPDATE_REGISTER( SPI_CTRL2_REG(select), cfg.SPI_HOLD_TIME       << 4  , |= );
  UPDATE_REGISTER( SPI_CTRL2_REG(select), cfg.SPI_SETUP_TIME      << 0  , |= );

  // Pin configuration (PIN)
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_RESERVED_PIN_1  << 31 ,  = );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_CS_KEEP_ACTIVE  << 30 , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_CK_IDLE_EDGE    << 29 , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_RESERVED_PIN_2  << 14 , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_MASTER_CK_SEL   << 11 , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_RESERVED_PIN_3  << 9  , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_MASTER_CS_POL   << 6  , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_CK_DIS          << 5  , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_RESERVED_PIN_4  << 3  , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_CS2_DIS         << 2  , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_CS1_DIS         << 1  , |= );
  UPDATE_REGISTER( SPI_PIN_REG(select),   cfg.SPI_CS0_DIS         << 0  , |= );
  
  // User configuration register
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_USR_COMMAND       << 31,  = );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_USR_ADDR          << 30, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_USR_DUMMY         << 29, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_USR_MISO          << 28, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_USR_MOSI          << 27, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_USR_DUMMY_IDLE    << 26, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_USR_MOSI_HIGHPART << 25, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_USR_MISO_HIGHPART << 24, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_SIO               << 16, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_WR_BYTE_ORDER     << 11, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_RD_BYTE_ORDER     << 10, |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_CK_OUT_EDGE       << 7,  |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_CK_I_EDGE         << 6,  |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_CS_SETUP          << 5,  |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_CS_HOLD           << 4,  |= );
  UPDATE_REGISTER( SPI_USER_REG(select), cmd.SPI_DOUTDIN           << 0,  |= );  

  UPDATE_REGISTER( SPI_USER1_REG(select), cmd.SPI_USR_ADDR_BITLEN    << 26,  = );
  UPDATE_REGISTER( SPI_USER1_REG(select), cmd.SPI_USR_DUMMY_CYCLELEN << 0 , |= );

  UPDATE_REGISTER( SPI_USER2_REG(select), cmd.SPI_USR_COMMAND_BITLEN << 28,  = );
  UPDATE_REGISTER( SPI_USER2_REG(select), cmd.SPI_USR_COMMAND_VALUE  << 0,  |= );

  // Frequency configuration
  if( !calculateFreq( freq ) ){
    UPDATE_REGISTER( SPI_CLOCK_REG(select), cfg.SPI_CLK_EQU_SYSCLK << 31 ,  = );
    UPDATE_REGISTER( SPI_CLOCK_REG(select), cfg.SPI_CLKDIV_PRE     << 18 , |= );
    UPDATE_REGISTER( SPI_CLOCK_REG(select), cfg.SPI_CLKCNT_N       << 12 , |= );
    UPDATE_REGISTER( SPI_CLOCK_REG(select), cfg.SPI_CLKCNT_H       << 6  , |= );
    UPDATE_REGISTER( SPI_CLOCK_REG(select), cfg.SPI_CLKCNT_L       << 0  , |= );
  } 
  else
    UPDATE_REGISTER( SPI_CLOCK_REG(select), cfg.SPI_CLK_EQU_SYSCLK << 31 ,  = );

  // Assign pins mode
  gpio.mode( PIN_SPI_COPI(select), OUTPUT );
  gpio.mode( PIN_SPI_CIPO(select), INPUT  );
  gpio.mode( PIN_SPI_CLK(select) , OUTPUT );
  gpio.mode( PIN_SPI_CS(select)  , OUTPUT );

  // Assign pins function
  UPDATE_REGISTER( IO_MUX_PIN_REG( PIN_SPI_COPI(select) ), 1 << 12, |= );
  UPDATE_REGISTER( IO_MUX_PIN_REG( PIN_SPI_CIPO(select) ), 1 << 12, |= );
  UPDATE_REGISTER( IO_MUX_PIN_REG( PIN_SPI_CLK(select) ) , 1 << 12, |= );
}

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This functions turns the clock to the peripheral according to the `state`.
 *
 * @param[in] state Specifies the desired SPI peripheral state.
 *
 *    - `HIGH` : to enable the clock.
 * 
 *    - `LOW` : to disable the clock.
 * 
 * @return none
 * 
 * @note Updates the `on` member variable to reflect the current state of the SPI.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
void 
SPI::enable( const uint8_t state ){
  if( HIGH == state ){
    if( !(READ_REGISTER( DPORT_PERIP_CLK_EN_REG ) & (HIGH << DPORT_SPI_BIT(select) )) )
      UPDATE_REGISTER( DPORT_PERIP_CLK_EN_REG, HIGH << DPORT_SPI_BIT(select), |= );
    on = HIGH;
  }
  else{
    if( (READ_REGISTER( DPORT_PERIP_CLK_EN_REG ) & (HIGH << DPORT_SPI_BIT(select) )) >> DPORT_SPI_BIT(select) )
      UPDATE_REGISTER( DPORT_PERIP_CLK_EN_REG, ~(HIGH << DPORT_SPI_BIT(select)), &= );
    on = LOW;
  }
}

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Calculates the clock divider configuration for SPI to achieve the desired frequency. Updates the SPI configuration strucutre `cfg` if successful.
 *
 * @param[in] freq Desired SPI clock frequency in Hertz (Hz).
 * 
 * @return The state of the configuration.
 *   
 *   - `0`: Success (valid clock configuration found).
 *   
 *   - `1`: Failure (invalid frequency or no valid configuration).
 *
 * @note
 *
 *   - If cfg.SPI_CLK_EQU_SYSCLK is true, the SPI clock is set to the system clock.
 * 
 *   - If the desired frequency exceeds F_CPU, the function immediately returns failure.
 * 
 *   - The function iterates over possible prescaler values to find a valid configuration.
 * 
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
uint8_t 
SPI::calculateFreq( uint64_t freq ){
  if( cfg.SPI_CLK_EQU_SYSCLK )                                                 // System clock as output frequency to SPI
    return 1;

  if( F_CPU < freq )                                                           // Impossible desired frequency
    return 1;                                          

  float N = -1, H = -1, K = 0, a;

  while( SPI_CLKDIV_PRE_MAX > K ){
    freq *= 0.5;
    a = F_CPU / ( freq * ( K + 1.0 ));
    N = a - 1.0;
    H = a / 2.0 - 1;
    if( (0 <= N && N <= SPI_CLKCNT_N_MAX) && (0 <= H && H <= SPI_CLKCNT_H_MAX) )
      break;
    K++;
  }

  if( SPI_CLKDIV_PRE_MAX > K ){
    cfg.SPI_CLKDIV_PRE = (uint16_t) K;
    cfg.SPI_CLKCNT_N   = (uint8_t)  N;
    cfg.SPI_CLKCNT_H   = (uint8_t)  H;
    cfg.SPI_CLKCNT_L   = (uint8_t)  N; 
    return 0;                                                                  // Success return
  }
  return 1;                                                                    // Not found values for that frequency
}

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Performs an SPI data transfer, only one chunk of memory per call, is full-duplex communication.
 *
 * @param[in] data The data to be transmitted (uint8_t).
 * @param[in] len The number of bits to transmit (ex.: sizeof(data) * 8 ).
 * 
 * @return Data received during the SPI transfer.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
uint8_t *
SPI::transferChunk( const uint8_t * data, const uint16_t len ){
  // Verify possible nullable pointer
  if( (NULL == data) || (!len) )
    return NULL;

  // Wait until the SPI is idle (any previous operation must complete)
  while( READ_REGISTER( SPI_CMD_REG(select) ) & (1 << 18) );
  while( SPI_STATE_IDLE != state( ) );

  uint8_t n = len / 8.0;                                                       // The number of bytes sent
  if( SPI_MAX_REG * SPI_REG_SIZE < n )
    return NULL;
  
  
  // Load the data to be transmitted into the SPI data register
  for( uint8_t i = 0, reg = 0, shift = 0 ; i < n ; ++i ){
    if( 24 <= shift )
      reg ++;                                                                   // The information won't fit in a 32 bits register, so move one SPI register

    shift = (8 * i - reg * 32);

    if( !shift ) UPDATE_REGISTER( SPI_W_REG(select, reg), data[i] << shift, = );
    else         UPDATE_REGISTER( SPI_W_REG(select, reg), data[i] << shift, |= );
  }
  
  // Set the transmit data length (MOSIdata length) in bits; subtract 1 for zero-based indexing
  cmd.SPI_USR_MOSI_DBITLEN = len - 1;                                          
  UPDATE_REGISTER( SPI_MOSI_DLEN_REG(select), cmd.SPI_USR_MOSI_DBITLEN << 0 , = );

  // Set the receive data length (MISIdata length) in bits; subtract 1 for zero-based indexing
  cmd.SPI_USR_MISO_DBITLEN = len - 1;                                          
  UPDATE_REGISTER( SPI_MISO_DLEN_REG(select), cmd.SPI_USR_MISO_DBITLEN << 0 , = );

  // Start the SPI transfer operation by setting the appropriate bit in the command register
  UPDATE_REGISTER( SPI_CMD_REG(select), 1 << 18 , = );
  
  // Wait for the SPI transfer to complete (poll until bit 18 is cleared)
  while( READ_REGISTER( SPI_CMD_REG(select) ) & (1 << 18) );
  while( SPI_STATE_IDLE != state( ) );

  // Receive buffer  
  uint8_t * buf = (uint8_t *) malloc( n );
  if( NULL == buf )
    return NULL;

  // Read the information inside the SPI register
  for( uint8_t i = 0, reg = 0, shift = 0 ; i < n ; ++i ){
    if( 24 <= shift )
      reg ++;                                                                   // The information won't fit in a 32 bits register, so move one SPI register
   
    shift = (8 * i - reg * 32);
   
    buf[i] = (uint8_t) ((READ_REGISTER( SPI_W_REG(select, reg) ) >> shift) & 0xFF);
  }
  return buf;
}

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Performs SPI transfers, high level interface.
 *
 * @param[in] data The data to be transmitted (uint8_t).
 * @param[in] len The number of bytes to transmit (ex.: sizeof(data) ).
 * 
 * @return Data received during all the SPI transfers.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
uint8_t * 
SPI::transfer( const uint8_t * data, const uint16_t size ){
  // Verify possible nullable pointer
  if( (NULL == data) || (!size) )
    return NULL;

  // Receive buffer  
  uint8_t * buf = (uint8_t *) malloc( size );
  if( NULL == buf )
    return NULL;

  // Calculate the number of chunks
  uint8_t chunkMaxSize = SPI_MAX_REG * SPI_REG_SIZE;
  uint8_t chunks = size / chunkMaxSize;
  
  // In case the amount of information doesn't have a fraction part, the self chunk needs to be included
  if( 0 < (size % chunkMaxSize) )
    chunks++;

  uint16_t totalBytes = 0, nBytes = 0;
  uint8_t  *tmp = buf;

  // Fill the buffer
  for( uint8_t i = 1, * chunkData = NULL  ; i <= chunks ; ++i ){    
    nBytes = size - (chunks - i) * chunkMaxSize - totalBytes;   

    chunkData = transferChunk( data + totalBytes, nBytes * 8 );
    if( NULL == chunkData ){
      free( buf );
      return NULL;
    }
    
    memcpy( tmp, chunkData, nBytes );
    tmp += nBytes;    
    totalBytes += nBytes;
    free( chunkData );
  }
  return buf;
}


/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief Gets the current state of the SPI state machine.
 *
 * @param none
 * 
 * @return The state of the state machine.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
uint8_t
SPI::state( void ){
  return (uint8_t)( READ_REGISTER( SPI_EXT2_REG(select) ) & 0x7 );
}