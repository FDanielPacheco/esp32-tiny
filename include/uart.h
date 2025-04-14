/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Introduction
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @file      uart.h
 * 
 * @version   1.0
 *
 * @date      22-10-2024
 *
 * @brief     Prototypes and definitions of the functions to use the UART in the ESP32
 * 
 * @author    Fábio D. Pacheco, 
 * @email     pacheco.castro.fabio@gmail.com
 *
 * @copyright Copyright (c) [2024] [Fábio D. Pacheco]
 * 
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Definition file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
 
#ifndef UART_H
#define UART_H

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Imported libraries
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#include <core.h>                                                              // Used for basic macros, such as READ_REGISTER or UPDATE_REGISTER
#include <gpio.h>                                                              // Used for the IO_MUX and GPIO assignment

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Memory space definitions - UART 
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#define UART0                                1                                 //!< Peripheral selection for UART0.
#define UART1                                2                                 //!< Peripheral selection for UART1.
#define UART2                                3                                 //!< Peripheral selection for UART2.
#define UDMA0                                1                                       
#define UDMA1                                2      

#define MEM_BASE_UART0                       0x3FF40000                        //!< Memory address of the first register for UART0. 
#define MEM_BASE_UART1                       0x3FF50000                        //!< Memory address of the first register for UART1.
#define MEM_BASE_UART2                       0x3FF6E000                        //!< Memory address of the first register for UART2.
#define MEM_BASE_UHCI_UDMA0                  0x3FF54000                        //!< Memory address of the first register for UHCI DMA0 (General-purpose)
#define MEM_BASE_UHCI_UDMA1                  0x3FF4C000                        //!< Memory address of the first register for UHCI DMA1 (Critical-purpose)
                                   
#define MEM_BASE_UART(i)                     ((i) == UART0 ? MEM_BASE_UART0 : \
                                              (i) == UART1 ? MEM_BASE_UART1 : \
                                              (i) == UART2 ? MEM_BASE_UART2 : 0)
      
#define MEM_BASE_UHCI_UDMA(i)                ((i) == UDMA0 ? MEM_BASE_UHCI_UDMA0 : \
                                              (i) == UDMA1 ? MEM_BASE_UHCI_UDMA1 : 0)
      
#define UART_FIFO_REG(i)                     (MEM_BASE_UART(i) + 0x0000)         //!< UART0 accesses FIFO via this register. (R/W)
#define UART_INT_RAW_REG(i)                  (MEM_BASE_UART(i) + 0x0004)         //!< Raw interrupt status
#define UART_INT_ST_REG(i)                   (MEM_BASE_UART(i) + 0x0008)         //!< Masked interrupt status
#define UART_INT_ENA_REG(i)                  (MEM_BASE_UART(i) + 0x000C)         //!< Interrupt enable bits
#define UART_INT_CLR_REG(i)                  (MEM_BASE_UART(i) + 0x0010)         //!< Interrupt clear bits
#define UART_CLKDIV_REG(i)                   (MEM_BASE_UART(i) + 0x0014)         //!< Clock divider configuration
#define UART_AUTOBAUD_REG(i)                 (MEM_BASE_UART(i) + 0x0018)         //!< Autobaud configuration register 
#define UART_STATUS_REG(i)                   (MEM_BASE_UART(i) + 0x001C)         //!< UART status register
#define UART_CONF0_REG(i)                    (MEM_BASE_UART(i) + 0x0020)         //!< Configuration register 0
#define UART_CONF1_REG(i)                    (MEM_BASE_UART(i) + 0x0024)         //!< Configuration register 1
#define UART_LOWPULSE_REG(i)                 (MEM_BASE_UART(i) + 0X0028)         //!< This register stores the value of the minimum duration of the low-level pulse. It is used in the baud rate detection process. (RO)
#define UART_HIGHPULSE_REG(i)                (MEM_BASE_UART(i) + 0x002C)         //!< This register stores the value of the minimum duration of the high level pulse. It is used in baud rate detection process. (RO)
#define UART_RXD_CNT_REG(i)                  (MEM_BASE_UART(i) + 0x0030)         //!< This register stores the count of the RxD edge change. It is used in the baud rate detection process. (RO)
#define UART_FLOW_CONF_REG(i)                (MEM_BASE_UART(i) + 0x0034)         //!< Software flow-control configuration
#define UART_SLEEP_CONF_REG(i)               (MEM_BASE_UART(i) + 0x0038)         //!< When the number of positive edges of RxD signal is larger than or equal to (UART_ACTIVE_THRESHOLD+2), the system emerges from Light-sleep mode and becomes active. (R/W)
#define UART_SWFC_CONF_REG(i)                (MEM_BASE_UART(i) + 0x003C)         //!< Software flow-control character configuration  
#define UART_IDLE_CONF_REG(i)                (MEM_BASE_UART(i) + 0x0040)         //!< Frame-end idle configuration
#define UART_RS485_CONF_REG(i)               (MEM_BASE_UART(i) + 0x0044)         //!< RS485 mode configuration 
#define UART_AT_CMD_PRECNT_REG(i)            (MEM_BASE_UART(i) + 0x0048)         //!< This register is used to configure the idle-time duration before the first at_cmd is received by the receiver. When the duration is less than what this register indicates, it will not take the next data received as an at_cmd char. (R/W)
#define UART_AT_CMD_POSTCNT_REG(i)           (MEM_BASE_UART(i) + 0x004C)         //!< This register is used to configure the duration between the last at_cmd and the next data. When the duration is less than what this register indicates, it will not take the previous data as an at_cmd char. (R/W)
#define UART_AT_CMD_GAPTOUT_REG(i)           (MEM_BASE_UART(i) + 0x0050)         //!< This register is used to configure the interval between the at_cmd chars. When the interval is greater than the value of this register, it will not take the data as continuous at_cmd chars. The register should be configured to more than half of the baud rate. (R/W)
#define UART_AT_CMD_CHAR_REG(i)              (MEM_BASE_UART(i) + 0x0054)         //!< AT escape sequence detection configuration
#define UART_MEM_CONF_REG(i)                 (MEM_BASE_UART(i) + 0x0058)         //!< UART threshold and allocation configuration 
#define UART_MEM_TX_STATUS_REG(i)            (MEM_BASE_UART(i) + 0x005C)         //!< TX FIFO write and read offset address
#define UART_MEM_RX_STATUS_REG(i)            (MEM_BASE_UART(i) + 0x0060)         //!< RX FIFO write and read offset address
#define UART_MEM_CNT_STATUS_REG(i)           (MEM_BASE_UART(i) + 0x0064)         //!< Receive and transmit memory configuration
#define UART_POSPULSE_REG(i)                 (MEM_BASE_UART(i) + 0x0068)         //!< Autobaud high pulse register 
#define UART_NEGPULSE_REG(i)                 (MEM_BASE_UART(i) + 0x006C)         //!< This register stores the count of RxD negative edges. It is used in the autobaud detection process. (RO)

#define UHCI_CONF0_REG(i)                    (MEM_BASE_UHCI_UDMA(i) + 0x0000)    //!< UART and frame separation config
#define UHCI_CONF1_REG(i)                    (MEM_BASE_UHCI_UDMA(i) + 0x002C)    //!< UHCI config register
#define UHCI_ESCAPE_CONF_REG(i)              (MEM_BASE_UHCI_UDMA(i) + 0x0064)    //!< Escape characters configuration
#define UHCI_HUNG_CONF_REG(i)                (MEM_BASE_UHCI_UDMA(i) + 0x0068)    //!< Timeout configuration

#define UHCI_DMA_OUT_LINK_REG(i)             (MEM_BASE_UHCI_UDMA(i) + 0x0024)    //!< OUT Link descriptor address and control
#define UHCI_DMA_IN_LINK_REG(i)              (MEM_BASE_UHCI_UDMA(i) + 0x0028)    //!< IN Link descriptor address and control
#define UHCI_DMA_OUT_PUSH_REG(i)             (MEM_BASE_UHCI_UDMA(i) + 0x0018)    //!< FIFO data push register
#define UHCI_DMA_IN_POP_REG(i)               (MEM_BASE_UHCI_UDMA(i) + 0x0020)    //!< FIFO data pop register
#define UHCI_DMA_OUT_STATUS_REG(i)           (MEM_BASE_UHCI_UDMA(i) + 0x0014)    //!< DMA FIFO status, UHCI_OUT_EMPTY 1: DMA inlink descriptor’s FIFO is empty. (RO), UHCI_OUT_FULL 1: DMA outlink descriptor’s FIFO is full. (RO)
#define UHCI_DMA_OUT_EOF_DES_ADDR_REG(i)     (MEM_BASE_UHCI_UDMA(i) + 0x0038)    //!< This register stores the address of the outlink descriptor when the EOF bit in this descriptor is 1. (RO)
#define UHCI_DMA_OUT_EOF_BFR_DES_ADDR_REG(i) (MEM_BASE_UHCI_UDMA(i) + 0x0044)    //!< This register stores the address of the outlink descriptor when there are some errors in this descriptor. (RO)
#define UHCI_DMA_IN_SUC_EOF_DES_ADDR_REG(i)  (MEM_BASE_UHCI_UDMA(i) + 0x003C)    //!< This register stores the address of the inlink descriptor when the EOF bit in this descriptor is 1. (RO)
#define UHCI_DMA_IN_ERR_EOF_DES_ADDR_REG(i)  (MEM_BASE_UHCI_UDMA(i) + 0x0040)    //!< This register stores the address of the inlink descriptor when there are some errors in this descriptor. (RO)
#define UHCI_DMA_IN_DSCR_REG(i)              (MEM_BASE_UHCI_UDMA(i) + 0x004C)    //!< The address of the current inlink descriptor x
#define UHCI_DMA_IN_DSCR_BF0_REG(i)          (MEM_BASE_UHCI_UDMA(i) + 0x0050)    //!< The address of the last inlink descriptor x-1
#define UHCI_DMA_IN_DSCR_BF1_REG(i)          (MEM_BASE_UHCI_UDMA(i) + 0x0054)    //!< The address of the second-to-last inlink descriptor x-2
#define UHCI_DMA_OUT_DSCR_REG(i)             (MEM_BASE_UHCI_UDMA(i) + 0x0058)    //!< The address of the current outlink descriptor y
#define UHCI_DMA_OUT_DSCR_BF0_REG(i)         (MEM_BASE_UHCI_UDMA(i) + 0x005C)    //!< The address of the last outlink descriptor y-1  
#define UHCI_DMA_OUT_DSCR_BF1_REG(i)         (MEM_BASE_UHCI_UDMA(i) + 0x0060)    //!< The address of the second-to-last outlink descriptor y-2

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * UART options
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#define UART0_TXD                      1
#define UART0_RXD                      3
#define UART1_TXD                      10
#define UART1_RXD                      9
#define UART1_RTS                      11
#define UART1_CTS                      6
#define UART2_TXD                      17
#define UART2_RXD                      16
#define UART2_RTS                      7
#define UART2_CTS                      8

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with UART selected.
 * 
 * @param[in] i: The selected UART.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define UART_TXD(i)                  ((i) == UART0 ? UART0_TXD : \
                                      (i) == UART1 ? UART1_TXD : \
                                      (i) == UART2 ? UART2_TXD : 0)
                         
/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with UART selected.
 * 
 * @param[in] i: The selected UART.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define UART_RXD(i)                  ((i) == UART0 ? UART0_RXD : \
                                      (i) == UART1 ? UART1_RXD : \
                                      (i) == UART2 ? UART2_RXD : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with UART selected.
 * 
 * @param[in] i: The selected UART.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define UART_RTS(i)                  ((i) == UART1 ? UART1_RTS : \
                                      (i) == UART2 ? UART2_RTS: 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the pin appropriate with UART selected.
 * 
 * @param[in] i: The selected UART.
 *
 * @return Uppon success it will return the number of the pin, otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define UART_CTS(i)                  ((i) == UART1 ? UART1_CTS : \
                                      (i) == UART2 ? UART2_CTS : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro selects the appropriate IO_MUX UART function for the given pin.
 * 
 * @param[in] PIN: The pin whose corresponding UART function is to be determined.
 *
 * @return Uppon success it will return the UART function for the given pin, either `FUNCTION_1` or `FUNCTION_5`, depending on the match. Otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define UART_IO_MUX_PIN_FUNC(PIN)    (UART_TXD(UART0) == (PIN) ? FUNCTION_1 : \
                                      UART_RXD(UART0) == (PIN) ? FUNCTION_1 : \
                                      UART_TXD(UART1) == (PIN) ? FUNCTION_5 : \
                                      UART_RXD(UART1) == (PIN) ? FUNCTION_5 : \
                                      UART_CTS(UART1) == (PIN) ? FUNCTION_5 : \
                                      UART_RTS(UART1) == (PIN) ? FUNCTION_5 : \
                                      UART_TXD(UART2) == (PIN) ? FUNCTION_5 : \
                                      UART_RXD(UART2) == (PIN) ? FUNCTION_5 : \
                                      UART_CTS(UART2) == (PIN) ? FUNCTION_5 : \
                                      UART_RTS(UART2) == (PIN) ? FUNCTION_5 : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro calculates the clock divider value based on the desired baud rate.
 * 
 * @param[in] BR: The desired baud rate for UART communication.
 *
 * @return Returns the calculated clock divider value. This result is a floating-point number.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define CLKDIV( BR )                 ( (F_CPU) * (2) / (BR) )                           

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Object UART
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

class
UART{
  private: 
    uint8_t       select;                                                      //!< This variable indicates whitch Timer is selected 
    uint8_t       on;
  
    typedef struct{
      uint8_t  UART_TICK_REF_ALWAYS_ON:1 = 1;                                  //!< This register is used to select the clock; 1: APB clock; 0: REF_TICK. (R/W)
      uint8_t  UART_DTR_INV:1            = 0;                                  //!< Set this bit to invert the level of the UART DTR signal. (R/W)
      uint8_t  UART_RTS_INV:1            = 0;                                  //!< Set this bit to invert the level of the UART RTS signal. (R/W)
      uint8_t  UART_TXD_INV:1            = 0;                                  //!< Set this bit to invert the level of the UART TxD signal. (R/W)
      uint8_t  UART_DSR_INV:1            = 0;                                  //!< Set this bit to invert the level of the UART DSR signal. (R/W) 
      uint8_t  UART_CTS_INV:1            = 0;                                  //!< Set this bit to invert the level of the UART CTS signal. (R/W)
      uint8_t  UART_RXD_INV:1            = 0;                                  //!< Set this bit to invert the level of the UART Rxd signal. (R/W)
      uint8_t  UART_TXFIFO_RST:1         = 0;                                  //!< Set this bit to reset the UART transmit-FIFO. NOTICE: UART2 doesn’t have any register to reset Tx_FIFO or Rx_FIFO, and the UART1_TXFIFO_RST and UART1_RXFIFO_RST in UART1 may impact the functioning of UART2. Therefore, these two registers in UART1 should only be used when the Tx_FIFO and Rx_FIFO in UART2 do not have any data. (R/W)
      uint8_t  UART_RXFIFO_RST:1         = 0;                                  //!< Set this bit to reset the UART receive-FIFO. NOTICE: UART2 doesn’t have any register to reset Tx_FIFO or Rx_FIFO, and the UART1_TXFIFO_RST and UART1_RXFIFO_RST in UART1 may impact the functioning of UART2. Therefore, these two registers in UART1 should only be used when the Tx_FIFO and Rx_FIFO in UART2 do not have any data. (R/W)
      uint8_t  UART_IRDA_EN:1            = 0;                                  //!< Set this bit to enable the IrDA protocol. (R/W)
      uint8_t  UART_TX_FLOW_EN:1         = 0;                                  //!< Set this bit to enable the flow control function for the transmitter. (R/W)
      uint8_t  UART_LOOPBACK:1           = 0;                                  //!< Set this bit to enable the UART loopback test mode. (R/W)
      uint8_t  UART_IRDA_RX_INV:1        = 0;                                  //!< Set this bit to invert the level of the IrDA receiver. (R/W)
      uint8_t  UART_IRDA_TX_INV:1        = 0;                                  //!< Set this bit to invert the level of the IrDA transmitter. (R/W)
      uint8_t  UART_IRDA_WCTL:1          = 0;                                  //!< 1: The IrDA transmitter’s 11th bit is the same as its 10th bit; 0: set IrDA transmitter’s 11th bit to 0. (R/W)
      uint8_t  UART_IRDA_TX_EN:1         = 0;                                  //!< This is the start enable bit of the IrDA transmitter. (R/W)
      uint8_t  UART_IRDA_DPLX:1          = 0;                                  //!< Set this bit to enable the IrDA loopback mode. (R/W)
      uint8_t  UART_TXD_BRK:1            = 0;                                  //!< Set this bit to enable the transmitter to send NULL, when the process of sending data is completed. (R/W)
      uint8_t  UART_SW_DTR:1             = 0;                                  //!< This register is used to configure the software DTR signal used in software flow control. (R/W)
      uint8_t  UART_SW_RTS:1             = 0;                                  //!< his bit is used in hardware flow control when UART_RX_FLOW_EN is 0. Set this bit to drive the RTS (or rtsn_out) signal low, and reset to drive the signal high. (R/W)
      uint8_t  UART_STOP_BIT_NUM:2       = 1;                                  //!< This register is used to set the length of the stop bit. 0: Invalid. No effect 1: 1 bit 2: 1.5 bits 3: 2 bits (R/W)
      uint8_t  UART_BIT_NUM:2            = 3;                                  //!< UART_BIT_NUM This register is used to set the length of data; 0: 5 bits, 1: 6 bits, 2: 7 bits, 3: 8 bits. (R/W)
      uint8_t  UART_PARITY_EN:1          = 0;                                  //!< Set this bit to enable the UART parity check. (R/W)
      uint8_t  UART_PARITY:1             = 0;                                  //!< This register is used to configure the parity check mode; 0: even, 1: odd. (R/W)
      uint8_t  UART_RX_TOUT_EN:1         = 0;                                  //!< This is the enable bit for the UART receive-timeout function. (R/W)
      uint8_t  UART_RX_TOUT_THRHD:7      = 0;                                  //!< This register is used to configure the UART receiver’s timeout value when receiving a byte. When using APB_CLK as the clock source, the register counts by UART baud cycle multiplied by 8. When using REF_TICK as the clock source, the register counts by UART baud cycle * 8 * (REF_TICK frequency)/(APB_CLK frequency). (R/W)
      uint8_t  UART_RX_FLOW_EN:1         = 0;                                  //!< This is the flow enable bit of the UART receiver; 1: choose software flow control by configuring the sw_rts signal; 0: disable software flow control. (R/W)
      uint8_t  UART_RX_FLOW_THRHD:7      = 0;                                  //!< When UART_RX_FLOW_EN is 1 and the receiver gets more data than its threshold value, the receiver produces an rtsn_out signal that tells the transmitter to stop transferring data. The threshold value is (rx_flow_thrhd_h3, rx_flow_thrhd). (R/W)
      uint8_t  UART_TXFIFO_EMPTY_THRHD:7 = 0x60;                               //!< When the data amount in transmit-FIFO is less than its threshold value, it will produce a TXFIFO_EMPTY_INT_RAW interrupt. The threshold value is (tx_mem_empty_thrhd, txfifo_empty_thrhd). (R/W)
      uint8_t  UART_RXFIFO_FULL_THRHD:7  = 0x60;                               //!< When the receiver gets more data than its threshold value, the receiver will produce an RXFIFO_FULL_INT_RAW interrupt. The threshold value is (rx_flow_thrhd_h3, rxfifo_full_thrhd). (R/W)
      uint8_t  UART_CLKDIV_FRAG:4        = 0;                                  //!< The decimal part of the frequency divider factor. (R/W)
      uint32_t UART_CLKDIV:20            = 0x2B6;                              //!< The integral part of the frequency divider factor. (R/W)
      uint8_t  UART_SEND_XOFF:1          = 0;                                  //!< Hardware auto-clear; set to 1 to send Xoff char. (R/W)
      uint8_t  UART_SEND_XON:1           = 0;                                  //!< Hardware auto-clear; set to 1 to send Xon char. (R/W)
      uint8_t  UART_FORCE_XOFF:1         = 0;                                  //!< Set this bit to set the internal CTSn and stop the transmitter from sending data. (R/W)
      uint8_t  UART_FORCE_XON:1          = 0;                                  //!< Set this bit to clear the internal CTSn and enable the transmitter to continue sending data. (R/W)
      uint8_t  UART_XONOFF_DEL:1         = 0;                                  //!< Set this bit to remove the flow-control char from the received data. (R/W)
      uint8_t  UART_SW_FLOW_CON_EN:1     = 0;                                  //!< Set this bit to enable software flow control. It is used with register sw_xon or sw_xoff. (R/W)
      uint8_t  UART_XOFF_CHAR:8          = 0x13;                               //!< This register stores the Xoff flow control char. (R/W)
      uint8_t  UART_XON_CHAR:8           = 0x11;                               //!< This register stores the Xon flow control char. (R/W)
      uint8_t  UART_XOFF_THRESHOLD:8     = 0xE0;                               //!< When the data amount in receive-FIFO is more than what this register
      uint8_t  UART_XON_THRESHOLD:8      = 0;                                  //!< When the data amount in receive-FIFO is less than what this register indicates, it will send an Xon char, with uart_sw_flow_con_en set to 1. (R/W)
      uint16_t UART_ACTIVE_THRESHOLD:9   = 0xF0;                               //!< hen the number of positive edges of RxD signal is larger than or equal to (UART_ACTIVE_THRESHOLD+2), the system emerges from Light-sleep mode and becomes active. (R/W)
      uint8_t  UART_TX_BRK_NUM:8         = 0xA;                                //!< This register is used to configure the number of zeros (0) sent, after the process of sending data is completed. It is active when txd_brk is set to 1. (R/W)
      uint16_t UART_TX_IDLE_NUM:10       = 0x100;                              //!< This register is used to configure the duration between transfers. (R/W)
      uint16_t UART_RX_IDLE_THRHD:10     = 0x100;                              //!< When the receiver takes more time to receive Byte data than what this register indicates, it will produce a frame-end signal. (R/W)
    } config_t;

  public:
    UART( uint8_t option );
    ~UART( );
    
    void init( uint32_t baudrate );

    uint32_t write( const char * str, uint32_t len );
    uint32_t read( char * str, uint32_t len, uint32_t offset );
    uint32_t available( );

    config_t cfg;                                                              //!< UART configuration structure
};

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Global objects and variables
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

extern UART uart0;                                                             //!< Object associated with UART0, in order to use this peripheral, initialize it by calling uart0.init( )
extern UART uart1;                                                             //!< Object associated with UART1, in order to use this peripheral, initialize it by calling uart1.init( )
extern UART uart2;                                                             //!< Object associated with UART2, in order to use this peripheral, initialize it by calling uart2.init( )   

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Definition file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#endif

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * End of file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/