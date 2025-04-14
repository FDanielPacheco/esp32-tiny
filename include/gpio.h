/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Introduction
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @file      gpio.h
 * 
 * @version   1.0
 *
 * @date      16-10-2024
 *
 * @brief     Prototypes and definitions of the functions to use the GPIO in the ESP32
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
 
#ifndef GPIO_H
#define GPIO_H

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Imported libraries
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#include <core.h>                                                              // Used for basic macros, such as READ_REGISTER or UPDATE_REGISTER

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Memory space definitions - GPIO Matrix Registers
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#define MEM_BASE_GPIO                  0x3FF44000                              //!< Memory address of the first GPIO register
#define GPIO_OUT_REG                   (MEM_BASE_GPIO + 0x0004)                //!< GPIO0-31 output value. (R/W)     
#define GPIO_OUT_W1TS_REG              (MEM_BASE_GPIO + 0x0008)                //!< GPIO0-31 output set register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_OUT_REG will be set. (WO)
#define GPIO_OUT_W1TC_REG              (MEM_BASE_GPIO + 0x000C)                //!< GPIO0-31 output clear register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_OUT_REG will be cleared. (WO)
#define GPIO_OUT1_REG                  (MEM_BASE_GPIO + 0x0010)                //!< GPIO32-39 output value. (R/W)
#define GPIO_OUT1_W1TS_REG             (MEM_BASE_GPIO + 0x0014)                //!< GPIO32-39 output value set register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_OUT1_DATA will be set. (WO)
#define GPIO_OUT1_W1TC_REG             (MEM_BASE_GPIO + 0x0018)                //!< GPIO32-39 output value clear register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_OUT1_DATA will be cleared. (WO)
#define GPIO_ENABLE_REG                (MEM_BASE_GPIO + 0x0020)                //!< GPIO0-31 output enable. (R/W)
#define GPIO_ENABLE_W1TS_REG           (MEM_BASE_GPIO + 0x0024)                //!< GPIO0-31 output enable set register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_ENABLE will be set. (WO)
#define GPIO_ENABLE_W1TC_REG           (MEM_BASE_GPIO + 0x0028)                //!< GPIO0-31 output enable clear register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_ENABLE will be cleared. (WO)
#define GPIO_ENABLE1_REG               (MEM_BASE_GPIO + 0x002C)                //!< GPIO32-39 output enable. (R/W)
#define GPIO_ENABLE1_W1TS_REG          (MEM_BASE_GPIO + 0x0030)                //!< GPIO32-39 output enable set register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_ENABLE1 will be set. (WO)
#define GPIO_ENABLE1_W1TC_REG          (MEM_BASE_GPIO + 0x0034)                //!< GPIO32-39 output enable clear register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_ENABLE1 will be cleared. (WO)
#define GPIO_STRAP_REG                 (MEM_BASE_GPIO + 0x0038)                //!< GPIO strapping results: Bit5-bit0 of boot_sel_chip[5:0] correspond to MTDI, GPIO0, GPIO2, GPIO4, MTDO, GPIO5, respectively.
#define GPIO_IN_REG                    (MEM_BASE_GPIO + 0x003C)                //!< GPIO0-31 input value. Each bit represents a pad input value, 1 for high level and 0 for low level. (RO)
#define GPIO_IN1_REG                   (MEM_BASE_GPIO + 0x0040)                //!< GPIO32-39 input value. Each bit represents a pad input value. (RO)
#define GPIO_STATUS_REG                (MEM_BASE_GPIO + 0x0044)                //!< GPIO0-31 interrupt status register. Each bit can be either of the two interrupt sources for the two CPUs. The enable bits in GPIO_PINn_INT_ENA, corresponding to the 13-16 bits in GPIO_PINn_REG should be set to 1. (R/W)
#define GPIO_STATUS_W1TS_REG           (MEM_BASE_GPIO + 0x0048)                //!< GPIO0-31 interrupt status set register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_STATUS_INT will be set. (WO)
#define GPIO_STATUS_W1TC_REG           (MEM_BASE_GPIO + 0x004C)                //!< GPIO0-31 interrupt status clear register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_STATUS_INT will be cleared. (WO)
#define GPIO_STATUS1_REG               (MEM_BASE_GPIO + 0x0050)                //!< GPIO32-39 interrupt status register. Each bit can be either of the two interrupt sources for the two CPUs. The enable bits in GPIO_PINn_INT_ENA, corresponding to the 13-16 bits in GPIO_PINn_REG should be set to 1. (R/W)
#define GPIO_STATUS1_W1TS_REG          (MEM_BASE_GPIO + 0x0054)                //!< GPIO32-39 interrupt status set register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_STATUS1_INT will be set. (WO)
#define GPIO_STATUS1_W1TC_REG          (MEM_BASE_GPIO + 0x0058)                //!< GPIO32-39 interrupt status clear register. For every bit that is 1 in the value written here, the corresponding bit in GPIO_STATUS1_INT will be cleared. (WO)
#define GPIO_ACPU_INT_REG              (MEM_BASE_GPIO + 0x0060)                //!< GPIO0-31 APP CPU interrupt status. (RO)
#define GPIO_ACPU_NMI_INT_REG          (MEM_BASE_GPIO + 0x0064)                //!< GPIO0-31 APP CPU non-maskable interrupt status. (RO)
#define GPIO_PCPU_INT_REG              (MEM_BASE_GPIO + 0x0068)                //!< GPIO0-31 PRO CPU interrupt status. (RO)
#define GPIO_PCPU_NMI_INT_REG          (MEM_BASE_GPIO + 0x006C)                //!< GPIO0-31 PRO CPU non-maskable interrupt status. (RO) 
#define GPIO_ACPU_INT1_REG             (MEM_BASE_GPIO + 0x0074)                //!< GPIO32-39 APP CPU interrupt status. (RO)
#define GPIO_ACPU_NMI_INT1_REG         (MEM_BASE_GPIO + 0x0078)                //!< GPIO32-39 APP CPU non-maskable interrupt status. (RO)
#define GPIO_PCPU_INT1_REG             (MEM_BASE_GPIO + 0x007C)                //!< GPIO32-39 PRO CPU interrupt status. (RO)
#define GPIO_PCPU_NMI_INT1_REG         (MEM_BASE_GPIO + 0x0080)                //!< GPIO32-39 PRO CPU non-maskable interrupt status. (RO)

#define MEM_BASE_GPIO_PIN              (MEM_BASE_GPIO + 0x0088)
#define GPIO_PIN_REG(PIN)              (MEM_BASE_GPIO_PIN + (0x04 * (PIN)))

/**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************//**
 * @brief This macro returns the memory register location appropriate with `PIN` selected.
 * 
 * @param[in] PIN The GPIO pin number to configure (0–39).
 *
 * @return Uppon success it will return the memory register location (uint32_t), otherwise 0.
 *
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define IO_MUX_PIN_REG(PIN)            ((0)  == (PIN) ? 0x3FF49044 : \
                                        (1)  == (PIN) ? 0x3FF49088 : \
                                        (2)  == (PIN) ? 0x3FF49040 : \
                                        (3)  == (PIN) ? 0x3FF49084 : \
                                        (4)  == (PIN) ? 0x3FF49048 : \
                                        (5)  == (PIN) ? 0x3FF4906C : \
                                        (6)  == (PIN) ? 0x3FF49060 : \
                                        (7)  == (PIN) ? 0x3FF49064 : \
                                        (8)  == (PIN) ? 0x3FF49068 : \
                                        (9)  == (PIN) ? 0x3FF49054 : \
                                        (10) == (PIN) ? 0x3FF49058 : \
                                        (11) == (PIN) ? 0x3FF4905C : \
                                        (12) == (PIN) ? 0x3FF49034 : \
                                        (13) == (PIN) ? 0x3FF49038 : \
                                        (14) == (PIN) ? 0x3FF49030 : \
                                        (15) == (PIN) ? 0x3FF4903C : \
                                        (16) == (PIN) ? 0x3FF4904C : \
                                        (17) == (PIN) ? 0x3FF49050 : \
                                        (18) == (PIN) ? 0x3FF49070 : \
                                        (19) == (PIN) ? 0x3FF49074 : \
                                        (21) == (PIN) ? 0x3FF4907C : \
                                        (22) == (PIN) ? 0x3FF49080 : \
                                        (23) == (PIN) ? 0x3FF4908C : \
                                        (24) == (PIN) ? 0x3FF49090 : \
                                        (25) == (PIN) ? 0x3FF49024 : \
                                        (26) == (PIN) ? 0x3FF49028 : \
                                        (27) == (PIN) ? 0x3FF4902C : \
                                        (32) == (PIN) ? 0x3FF4901C : \
                                        (33) == (PIN) ? 0x3FF49020 : \
                                        (34) == (PIN) ? 0x3FF49014 : \
                                        (35) == (PIN) ? 0x3FF49018 : \
                                        (36) == (PIN) ? 0x3FF49004 : \
                                        (37) == (PIN) ? 0x3FF49008 : \
                                        (38) == (PIN) ? 0x3FF4900C : \
                                        (39) == (PIN) ? 0x3FF49010 : 0)

#define FUNCTION_1                      0                                      //!< IO MUX Function of the pad 1 
#define FUNCTION_2                      1                                      //!< IO MUX Function of the pad 2
#define FUNCTION_3                      2                                      //!< IO MUX Function of the pad 3
#define FUNCTION_4                      3                                      //!< IO MUX Function of the pad 4
#define FUNCTION_5                      4                                      //!< IO MUX Function of the pad 5 
#define FUNCTION_6                      5                                      //!< IO MUX Function of the pad 6

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Object GPIO
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

class 
GPIO {
  private:
    uint32_t mask;                                                             //!< It's responsible to store pins selected to update the GPIO register from GPIO00-31
    uint8_t mask1;                                                             //!< It's responsible to store pins selected to update the GPIO register from GPIO32-39

  public:
    GPIO( );                                                                   //!< Object constructor, it clears the mask
    ~GPIO( );                                                                  //!< Object deconstructor
    
    uint8_t mode( uint8_t pin, uint8_t state );                                //!< Selects the desired functional mode of the pin, can be either INPUT or OUTPUT, for the number of the pin refer to the datasheet 
    uint8_t write( uint8_t pin, uint8_t state );                               //!< Write to the PORT the desired state of the pin, can be either HIGH (Vcc) or LOW (Gnd) 
    uint8_t read( uint8_t pin );                                               //!< Read the pad input value, returns 1 for high level and 0 for low level
};

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Global objects and variables
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

extern GPIO gpio;                                                              //!< Object associated with GPIO

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * Definition file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/

#endif

/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
 * End of file
 **************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/