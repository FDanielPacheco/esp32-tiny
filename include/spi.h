 /***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Introduction
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @file      spi.h
 * 
 * @version   1.1
 *
 * @date      30-10-2024, 27-12-2024
 *
 * @brief     Prototypes and definitions of the functions to use the SPI peripherals in the ESP32-WROOM32D
 * 
 * @author    Fábio D. Pacheco, 
 * @email     pacheco.castro.fabio@gmail.com
 *
 * @copyright Copyright (c) [2024] [Fábio D. Pacheco]
 * 
 * @note V1.1 - Added multibyte per transfer
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/*
 * SPI is a synchronous data bus, it uses a clock and data lines.
 *
 * Interface names
 * CLK/SCK = Clock signal / Serial clock
 *
 * Replacement names table due to obsolete terms:
 * Old     -  New
 * Master  - Controller
 * Slave   - Peripheral
 * MISO    - CIPO        = Data sent from the peripheral to the controller 
 * MOSI    - COPI        = Data sent from the controller to the peripheral
 * SS      - CS          = Chip select (Active-Low)
 *
 * Extra pin ( BUSY )  
 *
 * The side that generates the clock is called controller. There can only be one microcontroller but can be many peripherals
 *
 */

 /*
  * ESP32 integrates four SPI controllers which can be used to communicate with external
  * devices that use the SPI protocol. Controller SPI0 is used as a buffer for accessing external memory. Controller
  * SPI1 can be used as a master. Controllers SPI2 and SPI3 can be configured as either a master or a slave. When
  * used as a master, each SPI controller can drive multiple CS signals (CS0~CS2) to activate multiple slaves.
  * Controllers SPI1~SPI3 share two DMA channels. 
  */

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Definition file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#ifndef SPI_H
#define SPI_H

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Imported libraries
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#include <core.h>                                                              // Used for basic macros, such as READ_REGISTER or UPDATE_REGISTER 
#include <dport.h>                                                             // Used to enable the clock for the peripheral
#include <gpio.h>                                                              // Used for the IO_MUX and GPIO assignment

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Memory space definitions - SPI
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#define SPI0                          1                                        //!< Peripheral selection for SPI0.
#define SPI1                          2                                        //!< Peripheral selection for SPI1.
#define SPI2                          3                                        //!< Peripheral selection for SPI2.
#define SPI3                          4                                        //!< Peripheral selection for SPI3.

#define MEM_BASE_SPI0                 0x3FF43000                               //!< Memory address of the first register for SPI0.
#define MEM_BASE_SPI1                 0x3FF42000                               //!< Memory address of the first register for SPI1.
#define MEM_BASE_SPI2                 0x3FF64000                               //!< Memory address of the first register for SPI2.
#define MEM_BASE_SPI3                 0x3FF65000                               //!< Memory address of the first register for SPI3.

#define MEM_BASE_SPI(i)              ((i) == SPI0 ? MEM_BASE_SPI0 : \
                                      (i) == SPI1 ? MEM_BASE_SPI1 : \
                                      (i) == SPI2 ? MEM_BASE_SPI2 : \
                                      (i) == SPI3 ? MEM_BASE_SPI3 : 0) 

#define SPI_CMD_REG(i)               (MEM_BASE_SPI(i)+ 0x0000)                 //!< An SPI operation will be triggered when this bit is set. The bit will be cleared once the operation is done. (R/W)
#define SPI_ADDR_REG(i)              (MEM_BASE_SPI(i)+ 0x0004)                 //!< It stores the transmitting address when master is in half-duplex mode or QSPI mode. If the address length is bigger than 32 bits, this register stores the higher 32 bits of address value, SPI_SLV_WR_STATUS_REG stores the rest lower part of address value. If the address length is smaller than 33 bits, this register stores all the address value. The register is in valid only when SPI_USR_ADDR bit is set to 1. (R/W)
#define SPI_CTRL_REG(i)              (MEM_BASE_SPI(i)+ 0x0008)                 //!< Refer to the datasheet
#define SPI_CTRL1_REG(i)             (MEM_BASE_SPI(i)+ 0x000C)                 //!< Reserved
#define SPI_RD_STATUS_REG(i)         (MEM_BASE_SPI(i)+ 0x0010)                 //!< Reserved
#define SPI_CTRL2_REG(i)             (MEM_BASE_SPI(i)+ 0x0014)                 //!< Refer to the datasheet
#define SPI_CLOCK_REG(i)             (MEM_BASE_SPI(i)+ 0x0018)                 //!< Refer to the datasheet
#define SPI_USER_REG(i)              (MEM_BASE_SPI(i)+ 0x001C)                 //!< Refer to the datasheet
#define SPI_USER1_REG(i)             (MEM_BASE_SPI(i)+ 0x0020)                 //!< Refer to the datasheet
#define SPI_USER2_REG(i)             (MEM_BASE_SPI(i)+ 0x0024)                 //!< Refer to the datasheet
#define SPI_MOSI_DLEN_REG(i)         (MEM_BASE_SPI(i)+ 0x0028)                 //!< It indicates the length of MOSI data minus one, in multiples of one bit. It is only valid when SPI_USR_MOSI is set to 1 in master mode. (R/W)
#define SPI_MISO_DLEN_REG(i)         (MEM_BASE_SPI(i)+ 0x002C)                 //!< It indicates the length of MISO data minus one, in multiples of one bit. It is only valid when SPI_USR_MISO is set to 1 in master mode. (R/W)
#define SPI_SLV_WR_STATUS_REG(i)     (MEM_BASE_SPI(i)+ 0x0030)                 //!< In the slave mode this register is the status register for the master to write the slave. In the master mode, if the address length is bigger than 32 bits, SPI_ADDR_REG stores the higher 32 bits of address value, and this register stores the rest lower part of address value. (R/W)
#define SPI_PIN_REG(i)               (MEM_BASE_SPI(i)+ 0x0034)                 //!< Refer to the datasheet
#define SPI_SLAVE_REG(i)             (MEM_BASE_SPI(i)+ 0x0038)                 //!< Refer to the datasheet
#define SPI_SLAVE1_REG(i)            (MEM_BASE_SPI(i)+ 0x003C)                 //!< Refer to the datasheet
#define SPI_SLAVE2_REG(i)            (MEM_BASE_SPI(i)+ 0x0040)                 //!< Refer to the datasheet
#define SPI_SLAVE3_REG(i)            (MEM_BASE_SPI(i)+ 0x0044)                 //!< Refer to the datasheet
#define SPI_SLV_WRBUF_DLEN_REG(i)    (MEM_BASE_SPI(i)+ 0x0048)                 //!< It indicates the length of written data minus one, in multiples of one bit. It is only valid in slave half-duplex mode. (R/W)
#define SPI_SLV_RDBUF_DLEN_REG(i)    (MEM_BASE_SPI(i)+ 0x004C)                 //!< It indicates the length of read data minus one, in multiples of one bit. It is only valid in slave half-duplex mode. (R/W)
#define SPI_SLV_RD_BIT_REG(i)        (MEM_BASE_SPI(i)+ 0x0064)                 //!< It indicates the bit length of data the master reads from the slave, minus one. It is only valid in slave half-duplex mode. (R/W)
#define SPI_W_REG(i,j)               (MEM_BASE_SPI(i)+ 0x0080 + 0x4*(j))       //!< Data buffer. (R/W)
#define SPI_TX_CRC_REG(i)            (MEM_BASE_SPI(i)+ 0x00C0)                 //!< Reserved
#define SPI_EXT2_REG(i)              (MEM_BASE_SPI(i)+ 0x00F8)                 //!< The current state of the SPI state machine: (RO)
#define SPI_DMA_CONF_REG(i)          (MEM_BASE_SPI(i)+ 0x0100)                 //!< Refer to the datasheet 
#define SPI_DMA_OUT_LINK_REG(i)      (MEM_BASE_SPI(i)+ 0x0104)                 //!< Refer to the datasheet

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * SPI State machine
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#define SPI_STATE_IDLE               0
#define SPI_STATE_PREPARATION        1
#define SPI_STATE_SEND_CMD           2
#define SPI_STATE_SEND_DATA          3
#define SPI_STATE_READ_DATA          4
#define SPI_STATE_WRITE_DATA         5
#define SPI_SPATE_WAIT               6
#define SPI_STATE_DONE               7
#define SPI_STATE_UNKOWN             8

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Map pins, based on the datasheet and pins picture
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

// SPI0, SPI1
#define SPI_CS_PIN                   11
#define SPI_WP_PIN                   10
#define SPI_HD_PIN                   9
#define SPI_MOSI_PIN                 8
#define SPI_MISO_PIN                 7
#define SPI_CLK_PIN                  6

// SPI2
#define HSPI_CS_PIN                  15
#define HSPI_CLK_PIN                 14
#define HSPI_MISO_PIN                12
#define HSPI_MOSI_PIN                13

// SPI3
#define VSPI_MOSI_PIN                23
#define VSPI_MISO_PIN                19
#define VSPI_CLK_PIN                 18
#define VSPI_CS_PIN                  5

#define SPI_MAX_REG                  16.0
#define SPI_REG_SIZE                 4.0

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with SPI selected.
 * 
 * @param[in] i: The selected SPI.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define PIN_SPI_COPI(i)              ((i) == SPI0 ? SPI_MOSI_PIN : \
                                      (i) == SPI1 ? SPI_MOSI_PIN : \
                                      (i) == SPI2 ? HSPI_MOSI_PIN : \
                                      (i) == SPI3 ? VSPI_MOSI_PIN : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with SPI selected.
 * 
 * @param[in] i: The selected SPI.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define PIN_SPI_CIPO(i)              ((i) == SPI0 ? SPI_MISO_PIN : \
                                      (i) == SPI1 ? SPI_MISO_PIN : \
                                      (i) == SPI2 ? HSPI_MISO_PIN : \
                                      (i) == SPI3 ? VSPI_MISO_PIN : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with SPI selected.
 * 
 * @param[in] i: The selected SPI.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define PIN_SPI_CLK(i)               ((i) == SPI0 ? SPI_CLK_PIN : \
                                      (i) == SPI1 ? SPI_CLK_PIN : \
                                      (i) == SPI2 ? HSPI_CLK_PIN : \
                                      (i) == SPI3 ? VSPI_CLK_PIN : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with SPI selected.
 * 
 * @param[in] i: The selected SPI.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define PIN_SPI_CS(i)                ((i) == SPI0 ? SPI_CS_PIN : \
                                      (i) == SPI1 ? SPI_CS_PIN : \
                                      (i) == SPI2 ? HSPI_CS_PIN : \
                                      (i) == SPI3 ? VSPI_CS_PIN : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with SPI selected.
 * 
 * @param[in] i: The selected SPI.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define PIN_SPI_WP(i)               ((i) == SPI0 ? SPI_WP_PIN : \
                                     (i) == SPI1 ? SPI_WP_PIN : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with SPI selected.
 * 
 * @param[in] i: The selected SPI.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define PIN_SPI_HD(i)               ((i) == SPI0 ? SPI_HD_PIN : \
                                     (i) == SPI1 ? SPI_HD_PIN : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the bit from the DPORT register appropriate with SPI selected.
 * 
 * @param[in] i: The selected SPI.
 *
 * @return Uppon success it will return the number of the bit, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define DPORT_SPI_BIT(i)            ((i) == SPI0 ? DPORT_SPI01_CLK_EN : \
                                     (i) == SPI1 ? DPORT_SPI01_CLK_EN : \
                                     (i) == SPI2 ? DPORT_SPI2_CLK_EN  : \
                                     (i) == SPI3 ? DPORT_SPI3_CLK_EN  : 0)

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Equation domains, according to their register size
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#define SPI_CLKCNT_N_MAX               63
#define SPI_CLKCNT_H_MAX               63
#define SPI_CLKDIV_PRE_MAX             8191                

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Object SPI
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

class
SPI{
  private:
    uint8_t select;                                                            //!< This variable indicates whitch Timer is selected 

    typedef struct{
      uint8_t  SPI_WR_BIT_ORDER:1      = 0;                                    //!< This bit determines the bit order for command, address and data in transmitted signal. 1: sends LSB first; 0: sends MSB first. (R/W)
      uint8_t  SPI_RD_BIT_ORDER:1      = 0;                                    //!< This bit determines the bit order for received data in received signal. 1: receives LSB first; 0: receives MSB first. (R/W)         
      uint8_t  SPI_FREAD_QIO:1         = 0;                                    //!< This bit is used to enable four-line address writes and data reads in QSPI mode. (R/W)
      uint8_t  SPI_FREAD_DIO:1         = 0;                                    //!< This bit is used to enable two-line address writes and data reads in QSPI mode. (R/W)      
      uint8_t  SPI_WP:1                = 1;                                    //!< This bit determines the write-protection signal output when SPI is idle in QSPI mode. 1: output high; 0: output low. (R/W)
      uint8_t  SPI_FREAD_QUAD:1        = 0;                                    //!< This bit is used to enable four-line data reads in QSPI mode. (R/W)
      uint8_t  SPI_FREAD_DUAL:1        = 0;                                    //!< This bit is used to enable two-line data reads in QSPI mode. (R/W)
      uint8_t  SPI_RESERVED_CTRL_1:5   = 1;                                    //!< Reserved but with a default value, not sure what it does, no manual found on the topic  
      uint8_t  SPI_FASTRD_MODE:1       = 1;                                    //!< This bit is used to enable spi_fread_qio, spi_fread_dio, spi_fread_qout, and spi_fread_dout. 1: enable 0: disable. (R/W)
      uint16_t SPI_RESERVED_CTRL_2:13  = 0x400;                                //!< Reserved but with a default value, not sure what it does, no manual found on the topic  
      uint8_t  SPI_MOSI_DELAY_NUM:3    = 0;                                    //!< It is used to configure the number of system clock cycles by which the MOSI signals are delayed. (R/W)
      uint8_t  SPI_MOSI_DELAY_MODE:2   = 0;                                    //!< This register field determines the way the MOSI signals are delayed by SPI clock. (R/W)
      uint8_t  SPI_MISO_DELAY_NUM:3    = 0;                                    //!< It is used to configure the number of system clock cycles by which the MISO signals are delayed. (R/W)
      uint8_t  SPI_MISO_DELAY_MODE:2   = 0;                                    //!< This register field determines the way MISO signals are delayed by SPI clock. (R/W)
      uint8_t  SPI_HOLD_TIME:4         = 1;                                    //!< The number of SPI clock cycles by which CS pin signals are delayed. It is only valid when SPI_CS_HOLD is set to 1. (R/W)
      uint8_t  SPI_SETUP_TIME:4        = 1;                                    //!< It is to configure the time between the CS signal active edge and the first SPI clock edge. It is only valid in half-duplex mode or QSPI mode and when SPI_CS_SETUP is set to 1. (R/W)
      uint8_t  SPI_CLK_EQU_SYSCLK:1    = 1;                                    //!< In master mode, when this bit is set to 1, SPI output clock is equal to system clock; when set to 0, SPI output clock is divided from system clock. In slave mode, it should be set to 0. (R/W)
      uint16_t SPI_CLKDIV_PRE:13       = 0;                                    //!< In master mode, it is used to configure the pre-divider value for SPI output clock. It is only valid when SPI_CLK_EQU_SYSCLK is 0. In slave mode, it should be set to 0. (R/W)     
      uint8_t  SPI_CLKCNT_N:6          = 3;                                    //!< In master mode, it is used to configure the divider for SPI output clock. It is only valid when SPI_CLK_EQU_SYSCLK is 0. In slave mode, it should be set to 0. (R/W)
      uint8_t  SPI_CLKCNT_H:6          = 1;                                    //!< In master mode, SPI_CLKCNT_H = ⌊ (SPI_CLKCNT_N+1)/2 –1⌋. It is only valid when SPI_CLK_EQU_SYSCLK is 0. In slave mode, it should be set to 0. (R/W)
      uint8_t  SPI_CLKCNT_L:6          = 3;                                    //!< In master mode, it is equal to SPI_CLKCNT_N. It is only valid when SPI_CLK_EQU_SYSCLK is 0. In slave mode, it should be set to 0. (R/W)
      uint8_t  SPI_CS_KEEP_ACTIVE:1    = 0;                                    //!< This bit is only used in master mode where when it is set, the CS signal will keep active. (R/W)
      uint8_t  SPI_CK_IDLE_EDGE:1      = 0;                                    //!< This bit is only used in master mode to configure the logicl level of SPI output clock in idle state. (R/W) 1: the spi_clk line keeps high when idle; 0: the spi_clk line keeps low when idle.
      uint8_t  SPI_MASTER_CK_SEL:3     = 0;                                    //!< This register field contains one bit per spi_cs line. When a bit is set in master mode, the corresponding spi_cs line is made active and the spi_cs pin outputs spi_clk. (R/W)
      uint8_t  SPI_MASTER_CS_POL:5     = 0;                                    //!< This register filed selects the polarity of the spi_cs line. It contains one bit per spi_cs line. Possible values of the bits: (R/W) 0: spi_cs is active-low; 1: spi_cs is active-high.
      uint8_t  SPI_CK_DIS:1            = 0;                                    //!< When set, output of the spi_clk signal is disabled. (R/W) 
      uint8_t  SPI_RESERVED_PIN_1:1    = 0;                                    //!< Reserved but with a default value, not sure what it does, no manual found on the topic
      uint16_t SPI_RESERVED_PIN_2:15   = 0;                                    //!< Reserved but with a default value, not sure what it does, no manual found on the topic
      uint8_t  SPI_RESERVED_PIN_3:2    = 0;                                    //!< Reserved but with a default value, not sure what it does, no manual found on the topic
      uint8_t  SPI_RESERVED_PIN_4:2    = 3;                                    //!< Reserved but with a default value, not sure what it does, no manual found on the topic
      uint8_t  SPI_CS2_DIS:1           = 1;                                    //!< This bit enables the SPI CS2 signal. 1: disables CS2; 0: enables CS2. (R/W)
      uint8_t  SPI_CS1_DIS:1           = 1;                                    //!< This bit enables the SPI CS1 signal. 1: disables CS1; 0: enables CS1. (R/W)
      uint8_t  SPI_CS0_DIS:1           = 0;                                    //!< This bit enables the SPI CS0 signal. 1: disables CS0; 0: enables CS0. (R/W)
    } config_t;    

    typedef struct{
      uint8_t  SPI_USR_COMMAND:1       = 1;                                    //!< This bit enables the command phase of an SPI operation in SPI half-duplex mode and QSPI mode. (R/W)
      uint8_t  SPI_USR_ADDR:1          = 0;                                    //!< This bit enables the address phase of an SPI operation in SPI half-duplex mode and QSPI mode. (R/W)
      uint8_t  SPI_USR_DUMMY:1         = 0;                                    //!< This bit enables the dummy phase of an SPI operation in SPI half-duplex mode and QSPI mode. (R/W)
      uint8_t  SPI_USR_MISO:1          = 0;                                    //!< This bit enables the read-data phase of an SPI operation in SPI half-duplex mode and QSPI mode. (R/W)
      uint8_t  SPI_USR_MOSI:1          = 0;                                    //!< This bit enables the write-data phase of an SPI operation in SPI half-duplex mode and QSPI mode. (R/W)
      uint8_t  SPI_USR_DUMMY_IDLE:1    = 0;                                    //!< The SPI clock signal is disabled in the dummy phase when the bit is set in SPI half-duplex mode and QSPI mode. (R/W)
      uint8_t  SPI_USR_MOSI_HIGHPART:1 = 0;                                    //!< If set, MOSI data is stored in SPI_W8 ~ SPI_W15 of the SPI buffer. (R/W)
      uint8_t  SPI_USR_MISO_HIGHPART:1 = 0;                                    //!< If set, MISO data is stored in SPI_W8 ~ SPI_W15 of the SPI buffer. (R/W)
      uint8_t  SPI_SIO:1               = 0;                                    //!< Set this bit to enable three-line half-duplex communication. (R/W)
      uint8_t  SPI_WR_BYTE_ORDER:1     = 0;                                    //!< This bit determines the byte order of the command, address and data in transmitted signal. 1: big-endian; 0: little-endian. (R/W)
      uint8_t  SPI_RD_BYTE_ORDER:1     = 0;                                    //!< This bit determines the byte order of received data in transmitted signal. 1: big-endian; 0: little_endian. (R/W)
      uint8_t  SPI_CK_OUT_EDGE:1       = 0;                                    //!< This bit, combined with SPI_MOSI_DELAY_MODE, sets the MOSI signal delay mode. It is only valid in master mode. (R/W)
      uint8_t  SPI_CK_I_EDGE:1         = 1;                                    //!< In slave mode, the bit is the same as SPI_CK_OUT_EDGE in master mode. It is combined with SPI_MISO_DELAY_MODE. It is only valid in slave mode. (R/W)
      uint8_t  SPI_CS_SETUP:1          = 0;                                    //!< Setting this bit enables a delay between CS active edge and the first clock edge, in multiples of one SPI clock cycle. In full-duplex mode and QSPI mode, setting this bit results in
      uint8_t  SPI_CS_HOLD:1           = 0;                                    //!< Setting this bit enables a delay between the end of a transmission and CS being inactive, as specified in SPI_HOLD_TIME. (R/W)
      uint8_t  SPI_DOUTDIN:1           = 0;                                    //!< Set the bit to enable full-duplex communication. (R/W)
      uint8_t  SPI_USR_ADDR_BITLEN:6   = 23;                                   //!< It indicates the bit length of the transmitted address minus one in half-duplex mode and QSPI mode, in multiples of one bit. It is only valid when SPI_USR_ADDR is set to 1. (RO)
      uint8_t  SPI_USR_DUMMY_CYCLELEN:8= 7;                                    //!< It indicates the number of SPI clock cycles for the dummy phase minus one in SPI half-duplex mode and QSPI mode. It is only valid when SPI_USR_DUMMY is set to 1. (R/W)
      uint32_t SPI_USR_MOSI_DBITLEN:24 = 0;                                    //!< It indicates the length of MOSI data minus one, in multiples of one bit. It is only valid when SPI_USR_MOSI is set to 1 in master mode. (R/W)
      uint32_t SPI_USR_MISO_DBITLEN:24 = 0;                                    //!< It indicates the length of MISO data minus one, in multiples of one bit. It is only valid when SPI_USR_MISO is set to 1 in master mode. (R/W)
      uint8_t  SPI_USR_COMMAND_BITLEN:4= 7;                                    //!< It indicates the bit length of the command phase minus one in SPI half-duplex mode and QSPI mode. It is only valid when SPI_USR_COMMAND is set to 1. (R/W)
      uint16_t SPI_USR_COMMAND_VALUE   = 0;                                    //!< It indicates the value of the command to be transmitted in SPI half-duplex mode and QSPI mode. It is only valid when SPI_USR_COMMAND is set to 1. (R/W)
    } commands_t;

    uint8_t calculateFreq( uint64_t freq );                                    //!< Calculate all the SPI parameters, in order to setup the frequency of transmission

  public:
    SPI( uint8_t option );
    ~SPI( );
  
    config_t   cfg;                                                            //!< Struct to perform the configuration of the peripheral SPI                                                            
    commands_t cmd;                                                            //!< Struct to perform the configuration of the peripheral SPI

    void init( uint64_t freq );                                                //!< Initialize the peripheral

    uint8_t on = LOW;                                                          //!< State of the peripheral
    void enable( const uint8_t state );

    uint8_t state( void );                                                     //!< Returns the current state of the SPI state machine

    uint8_t * transferChunk( const uint8_t * data, const uint16_t len );       //!< Perform an exchange of information between the controller and the peripheral with the maximum chuck of 60 bytes
    uint8_t * transfer( const uint8_t * data, const uint16_t size );           //!< Perform an exchange of information between the controller and the peripheral with the limit imposed by the function, not by SPI hardware
};

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Global objects and variables
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

extern SPI spi0;                                                               //!< Object associated with SPI0, in order to use this peripheral, initialize it by calling spi0.init( )
extern SPI spi1;                                                               //!< Object associated with SPI1, in order to use this peripheral, initialize it by calling spi1.init( )
extern SPI spi2;                                                               //!< Object associated with SPI2, in order to use this peripheral, initialize it by calling spi2.init( )
extern SPI spi3;                                                               //!< Object associated with SPI3, in order to use this peripheral, initialize it by calling spi3.init( )

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Definition file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#endif

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * End of file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/