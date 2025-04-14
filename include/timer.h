/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Introduction
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @file      timer.h
 * 
 * @version   1.0
 *
 * @date      17-10-2024
 *
 * @brief     Prototypes and definitions of the functions to use the Timers in the ESP32
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

#ifndef TIMER_H
#define TIMER_H

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Imported libraries
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#include <core.h>                                                              // Used for basic macros, such as READ_REGISTER or UPDATE_REGISTER

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Memory space definitions - TIMER
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#define TIMG0_0                        1                                       //!<  Peripheral selection for TIMER 0 from GROUP 0
#define TIMG0_1                        2                                       //!<  Peripheral selection for TIMER 1 from GROUP 0
#define TIMG1_0                        3                                       //!<  Peripheral selection for TIMER 0 from GROUP 1  
#define TIMG1_1                        4                                       //!<  Peripheral selection for TIMER 1 from GROUP 1

#define MEM_BASE_TIMG0                 0x3FF5F000                              //!<  Memory address of the first register of TIMER GROUP 0
#define MEM_BASE_TIMG1                 0x3FF60000                              //!<  Memory address of the first register of TIMER GROUP 1

#define MEM_BASE_TIMG(i)               ((i) == TIMG0_0 ?  MEM_BASE_TIMG0            : \
                                        (i) == TIMG0_1 ? (MEM_BASE_TIMG0 + 0x0024)  : \
                                        (i) == TIMG1_0 ?  MEM_BASE_TIMG1            : \
                                        (i) == TIMG1_1 ? (MEM_BASE_TIMG1 + 0x0024)  : 0)


#define TIMG_TCONFIG_REG(i)            (MEM_BASE_TIMG(i) + 0x0000)             //!< Refer to the datasheet
#define TIMG_TLO_REG(i)                (MEM_BASE_TIMG(i) + 0x0004)             //!< After writing to TIMGn_TxUPDATE_REG, the low 32 bits of the time-base counter of timer x can be read here. (RO)
#define TIMG_THI_REG(i)                (MEM_BASE_TIMG(i) + 0x0008)             //!< After writing to TIMGn_TxUPDATE_REG, the high 32 bits of the time-base counter of timer x can be read here. (RO)
#define TIMG_TUPDATE_REG(i)            (MEM_BASE_TIMG(i) + 0x000C)             //!< Write any value to trigger a timer 0 time-base counter value update (timer 0 current value will be stored in registers above). (WO)
#define TIMG_TALARMLO_REG(i)           (MEM_BASE_TIMG(i) + 0x0010)             //!< Timer 0 alarm trigger time-base counter value, low 32 bits. (R/W)
#define TIMG_TALARMHI_REG(i)           (MEM_BASE_TIMG(i) + 0x0014)             //!< Timer 0 alarm trigger time-base counter value, high 32 bits. (R/W)
#define TIMG_TLOADLO_REG(i)            (MEM_BASE_TIMG(i) + 0x0018)             //!< Low 32 bits of the value that a reload will load onto timer 0 time-base counter. (R/W)
#define TIMG_TLOADHI_REG(i)            (MEM_BASE_TIMG(i) + 0x001C)             //!< High 32 bits of the value that a reload will load onto timer 0 time-base counter. (R/W) 
#define TIMG_TLOAD_REG(i)              (MEM_BASE_TIMG(i) + 0x0020)             //!< Write any value to trigger a timer 0 time-base counter reload. (WO)

// Watchdog timer configuration register
#define TIMG_WDTCONFIG_REG(i) ((i) == TIMG0_0 ? (MEM_BASE_TIMG(TIMG0_0) + 0x0048 + (0) * 0x4) : \
                               (i) == TIMG0_1 ? (MEM_BASE_TIMG(TIMG0_0) + 0x0048 + (1) * 0x4) : \
                               (i) == TIMG1_0 ? (MEM_BASE_TIMG(TIMG1_0) + 0x0048 + (0) * 0x4) : \
                               (i) == TIMG1_1 ? (MEM_BASE_TIMG(TIMG1_0) + 0x0048 + (1) * 0x4) : 0)

// Write any value to feed the MWDT. (WO)
#define TIMG_WDTFEED_REG(i) (((i) == TIMG0_0 || (i) == TIMG0_1) ? (MEM_BASE_TIMG(TIMG0_0) + 0x0060) : \
                             ((i) == TIMG1_0 || (i) == TIMG1_1) ? (MEM_BASE_TIMG(TIMG1_0) + 0x0060) : 0)

// Write any value to feed the MWDT. (WO)
#define TIMG_WDTWPROTECT_REG(i) (((i) == TIMG0_0 || (i) == TIMG0_1) ? (MEM_BASE_TIMG(TIMG0_0) + 0x0064) : \
                                 ((i) == TIMG1_0 || (i) == TIMG1_1) ? (MEM_BASE_TIMG(TIMG1_0) + 0x0064) : 0)

#define TIMG_RTCCALICFG_REG(i) ((i) == TIMG0_0 ? (MEM_BASE_TIMG(TIMG0_0) + 0x0068 + (0) * 0x4) : \
                                (i) == TIMG0_1 ? (MEM_BASE_TIMG(TIMG0_0) + 0x0068 + (1) * 0x4) : \
                                (i) == TIMG1_0 ? (MEM_BASE_TIMG(TIMG1_0) + 0x0068 + (0) * 0x4) : \
                                (i) == TIMG1_1 ? (MEM_BASE_TIMG(TIMG1_0) + 0x0068 + (1) * 0x4) : 0)

#define TIMG_INT_ENA_REG(i) (((i) == TIMG0_0 || (i) == TIMG0_1) ? (MEM_BASE_TIMG(TIMG0_0) + 0x0098) : \
                             ((i) == TIMG1_0 || (i) == TIMG1_1) ? (MEM_BASE_TIMG(TIMG1_0) + 0x0098) : 0)

#define TIMG_INT_RAW_REG(i) (((i) == TIMG0_0 || (i) == TIMG0_1) ? (MEM_BASE_TIMG(TIMG0_0) + 0x009C) : \
                             ((i) == TIMG1_0 || (i) == TIMG1_1) ? (MEM_BASE_TIMG(TIMG1_0) + 0x009C) : 0)

#define TIMG_INT_ST_REG(i) (((i) == TIMG0_0 || (i) == TIMG0_1) ? (MEM_BASE_TIMG(TIMG0_0) + 0x00A0) : \
                            ((i) == TIMG1_0 || (i) == TIMG1_1) ? (MEM_BASE_TIMG(TIMG1_0) + 0x00A0) : 0)

#define TIMG_INT_CLR_REG(i) (((i) == TIMG0_0 || (i) == TIMG0_1) ? (MEM_BASE_TIMG(TIMG0_0) + 0x00A4) : \
                             ((i) == TIMG1_0 || (i) == TIMG1_1) ? (MEM_BASE_TIMG(TIMG1_0) + 0x00A4) : 0)

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro calculates the the number of ticks to get the desired timebase.
 * 
 * @param[in] T: The desired period.
 * @param[in] K: The timer prescalar.
 *
 * @return Returns the calculated ticks value. This result is a uint64_t number.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define TIMG_TICKS( T, K )             ( (T) * (F_CPU) * 0.000001 / (K) )              

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Object TIMER
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

class
TIMER{
  private:
    volatile bool flag;                                                        //!< This flag is set once the interruption occurs 
    uint8_t       select;                                                      //!< This variable indicates whitch Timer is selected 
    uint64_t      base;                                                        //!< The base time

    typedef struct{
      uint8_t  TIMG_T_INCREASE:1             = 1;                              //!< When set, the timer x time-base counter will increment every clock tick. When cleared, the timer x time-base counter will decrement. (R/W)
      uint8_t  TIMG_T_AUTORELOAD:1           = 1;                              //!< When set, timer x auto-reload at alarm is enabled. (R/W)
      uint16_t TIMG_T_DIVIDER                = 1;                              //!< Timer x clock (Tx_clk) prescale value. (R/W)  The prescaler can divide the APB clock by a factor from 2 to 65536. Specifically, when TIMGn_Tx_DIVIDER is either 1 or 2, the clock divisor is 2; when TIMGn_Tx_DIVIDER is 0, the clock divisor is 65536. Any other value will cause the clock to be divided by exactly that value.
      uint8_t  TIMG_T_EDGE_INT_EN:1          = 0;                              //!< When set, an alarm will generate an edge type interrupt. (R/W)
      uint8_t  TIMG_T_LEVEL_INT_EN:1         = 0;                              //!< When set, an alarm will generate a level type interrupt. (R/W)
      uint8_t  TIMG_T_ALARM_EN:1             = 0;                              //!< When set, the alarm is enabled. This bit is automatically cleared once an alarm occurs. (R/W)
      uint8_t  TIMG_WDT_EN:1                 = 0;                              //!< When set, MWDT is enabled. (R/W)
      uint8_t  TIMG_WDT_STG0:2               = 0;                              //!< Stage 0 configuration. 0: off, 1: interrupt, 2: reset CPU, 3: reset system. (R/W)
      uint8_t  TIMG_WDT_STG1:2               = 0;                              //!< Stage 1 configuration. 0: off, 1: interrupt, 2: reset CPU, 3: reset system. (R/W)
      uint8_t  TIMG_WDT_STG2:2               = 0;                              //!< Stage 2 configuration. 0: off, 1: interrupt, 2: reset CPU, 3: reset system. (R/W) 
      uint8_t  TIMG_WDT_STG3:2               = 0;                              //!< Stage 3 configuration. 0: off, 1: interrupt, 2: reset CPU, 3: reset system. (R/W)
      uint8_t  TIMG_WDT_EDGE_INT_EN:1        = 0;                              //!< When set, an edge type interrupt will occur at the timeout of a stage configured to generate an interrupt. (R/W)
      uint8_t  TIMG_WDT_LEVEL_INT_EN:1       = 0;                              //!< When set, a level type interrupt will occur at the timeout of a stage configured to generate an interrupt. (R/W)
      uint8_t  TIMG_WDT_CPU_RESET_LENGTH:3   = 1;                              //!< CPU reset signal length selection. 0: 100 ns, 1: 200 ns, 2: 300 ns, 3: 400 ns, 4: 500 ns, 5: 800 ns, 6: 1.6 μs, 7: 3.2 μs. (R/W)
      uint8_t  TIMG_WDT_SYS_RESET_LENGTH:3   = 1;                              //!< System reset signal length selection. 0: 100 ns, 1: 200 ns, 2: 300 ns, 3: 400 ns, 4: 500 ns, 5: 800 ns, 6: 1.6 μs, 7: 3.2 μs. (R/W)
      uint8_t  TIMG_WDT_FLASHBOOT_MOD_EN:1   = 1;                              //!< When set, Flash boot protection is enabled. (R/W)
      uint8_t  TIMG_RTC_CALI_CLK_SEL:2       = 1;                              //!< Used to select the clock to be calibrated. 0: RC_SLOW_CLK. 1: RC_FAST_DIV_CLK. 2: XTAL32K_CLK. (R/W)
      uint8_t  TIMG_RTC_CALI_RDY:1           = 0;                              //!< Set this bit to mark the completion of calibration. (RO)
      uint16_t TIMG_RTC_CALI_MAX:1           = 1;                              //!< Calibration time, in cycles of the clock to be calibrated. (R/W)
      uint8_t  TIMGn_RTC_CALI_START:1        = 1;                              //!< Set this bit to starts calibration. (R/W)
      uint8_t  TIMG_INT_WDT_INT_ENA:1        = 0;                              //!< The interrupt enable bit for the TIMGn_INT_WDT_INT interrupt. (R/W)
      uint8_t  TIMG_INT_T1_INT_ENA:1         = 0;                              //!< The interrupt enable bit for the TIMGn_INT_T1_INT interrupt. (R/W)
      uint8_t  TIMG_INT_T0_INT_ENA:1         = 0;                              //!< The interrupt enable bit for the TIMGn_INT_T0_INT interrupt. (R/W)
      uint8_t  TIMG_INT_WDT_INT_CLR:1        = 0;                              //!< Set this bit to clear the TIMGn_INT_WDT_INT interrupt. (WO)
      uint8_t  TIMG_INT_T1_INT_CLR:1         = 0;                              //!< Set this bit to clear the TIMGn_INT_T1_INT interrupt. (WO)
      uint8_t  TIMG_INT_T0_INT_CLR:1         = 0;                              //!< Set this bit to clear the TIMGn_INT_T0_INT interrupt. (WO)
      uint8_t  TIMG_T_WDT_EN:1               = 0;                              //!< When set, MWDT is enabled. (R/W)
      uint8_t  TIMG_T_WDT_STG0:2             = 0;                              //!< Stage 0 configuration. 0: off, 1: interrupt, 2: reset CPU, 3: reset system. (R/W)
      uint8_t  TIMG_T_WDT_STG1:2             = 0;                              //!< Stage 0 configuration. 0: off, 1: interrupt, 2: reset CPU, 3: reset system. (R/W)
      uint8_t  TIMG_T_WDT_STG2:2             = 0;                              //!< Stage 0 configuration. 0: off, 1: interrupt, 2: reset CPU, 3: reset system. (R/W)
      uint8_t  TIMG_T_WDT_STG3:2             = 0;                              //!< Stage 0 configuration. 0: off, 1: interrupt, 2: reset CPU, 3: reset system. (R/W)
      uint8_t  TIMG_T_WDT_EDGE_INT_EN:1      = 0;                              //!< When set, an edge type interrupt will occur at the timeout of a stage configured to generate an interrupt. (R/W) 
      uint8_t  TIMG_T_WDT_LEVEL_INT_EN:1     = 0;                              //!< When set, a level type interrupt will occur at the timeout of a stage configured to generate an interrupt. (R/W)
      uint8_t  TIMG_T_WDT_CPU_RESET_LENGTH:3 = 1;                              //!< CPU reset signal length selection. 0: 100 ns, 1: 200 ns, 2: 300 ns, 3: 400 ns, 4: 500 ns, 5: 800 ns, 6: 1.6 μs, 7: 3.2 μs. (R/W)
      uint8_t  TIMG_T_WDT_SYS_RESET_LENGTH:3 = 1;                              //!< System reset signal length selection. 0: 100 ns, 1: 200 ns, 2: 300 ns, 3: 400 ns, 4: 500 ns, 5: 800 ns, 6: 1.6 μs, 7: 3.2 μs. (R/W)
      uint8_t  TIMG_T_WDT_FLASHBOOT_MOD_EN:1 = 1;                              //!< When set, Flash boot protection is enabled. (R/W)
    } config_t;

  public:
    TIMER( uint8_t option );                                                   //!< Object constructor, receives as an argument the timer selection
    ~TIMER( );                                                                 //!< Object deconstructor

    config_t cfg;                                                              //!< Timer register 18.1 configuration 
    uint16_t counter;                                                          //!< An counter used to breake infinite loops
    uint8_t  on = LOW;                                                         //!< State of the peripheral

    void init( uint64_t timebase );                                            //!< Initialize the Timer with the desired options
    
    void enable( uint8_t state );                                              //!< Turn on or off the peripheral counter 

    void delay( uint64_t time );                                               //!< This delay function halts the progress of the program, it receives as argument the time to wait

    uint8_t check( );                                                          //!< Checks if the alarm got triggered
};

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * General functions
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

uint8_t infiniteLoopBreaker( TIMER &timer );

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Global objects and variables
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

extern TIMER tim0;                                                             // Object associated with timer 0 from group 0 (TIMG0_0), in order to use this peripheral, initialize it by calling tim0.init( )
extern TIMER tim1;                                                             // Object associated with timer 1 from group 0 (TIMG0_1), in order to use this peripheral, initialize it by calling tim1.init( )
extern TIMER tim2;                                                             // Object associated with timer 0 from group 1 (TIMG1_0), in order to use this peripheral, initialize it by calling tim2.init( )
extern TIMER tim3;                                                             // Object associated with timer 1 from group 1 (TIMG1_1), in order to use this peripheral, initialize it by calling tim3.init( )

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Definition file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#endif

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * End of file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/