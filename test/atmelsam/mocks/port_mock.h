#ifndef PORT_MOCK_H
#define PORT_MOCK_H

/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

#define PINS_PER_PORT 32
#define PORT_GROUP_NUM 1

typedef volatile       uint8_t  RoReg8;  /**< Read only  8-bit register (volatile const unsigned int) */

typedef union {
  struct {
    uint32_t DIR:32;           /*!< bit:  0..31  Port Data Direction                */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_DIR_Type;

typedef union {
  struct {
    uint32_t DIRCLR:32;        /*!< bit:  0..31  Port Data Direction Clear          */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_DIRCLR_Type;

typedef union {
  struct {
    uint32_t DIRSET:32;        /*!< bit:  0..31  Port Data Direction Set            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_DIRSET_Type;

typedef union {
  struct {
    uint32_t DIRTGL:32;        /*!< bit:  0..31  Port Data Direction Toggle         */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_DIRTGL_Type;

typedef union {
  struct {
    uint32_t OUT:32;           /*!< bit:  0..31  Port Data Output Value             */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_OUT_Type;

typedef union {
  struct {
    uint32_t OUTCLR:32;        /*!< bit:  0..31  Port Data Output Value Clear       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_OUTCLR_Type;

typedef union {
  struct {
    uint32_t OUTSET:32;        /*!< bit:  0..31  Port Data Output Value Set         */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_OUTSET_Type;

typedef union {
  struct {
    uint32_t OUTTGL:32;        /*!< bit:  0..31  Port Data Output Value Toggle      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_OUTTGL_Type;

typedef union {
  struct {
    uint32_t IN:32;            /*!< bit:  0..31  Port Data Input Value              */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_IN_Type;

typedef union {
  struct {
    uint32_t SAMPLING:32;      /*!< bit:  0..31  Input Sampling Mode                */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_CTRL_Type;

typedef union {
  struct {
    uint32_t PINMASK:16;       /*!< bit:  0..15  Pin Mask for Multiple Pin Configuration */
    uint32_t PMUXEN:1;         /*!< bit:     16  Peripheral Multiplexer Enable      */
    uint32_t INEN:1;           /*!< bit:     17  Input Enable                       */
    uint32_t PULLEN:1;         /*!< bit:     18  Pull Enable                        */
    uint32_t :3;               /*!< bit: 19..21  Reserved                           */
    uint32_t DRVSTR:1;         /*!< bit:     22  Output Driver Strength Selection   */
    uint32_t :1;               /*!< bit:     23  Reserved                           */
    uint32_t PMUX:4;           /*!< bit: 24..27  Peripheral Multiplexing            */
    uint32_t WRPMUX:1;         /*!< bit:     28  Write PMUX                         */
    uint32_t :1;               /*!< bit:     29  Reserved                           */
    uint32_t WRPINCFG:1;       /*!< bit:     30  Write PINCFG                       */
    uint32_t HWSEL:1;          /*!< bit:     31  Half-Word Select                   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_WRCONFIG_Type;

typedef union {
  struct {
    uint8_t  PMUXE:4;          /*!< bit:  0.. 3  Peripheral Multiplexing Even       */
    uint8_t  PMUXO:4;          /*!< bit:  4.. 7  Peripheral Multiplexing Odd        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PORT_PMUX_Type;

typedef union {
  struct {
    uint8_t  PMUXEN:1;         /*!< bit:      0  Peripheral Multiplexer Enable      */
    uint8_t  INEN:1;           /*!< bit:      1  Input Enable                       */
    uint8_t  PULLEN:1;         /*!< bit:      2  Pull Enable                        */
    uint8_t  :3;               /*!< bit:  3.. 5  Reserved                           */
    uint8_t  DRVSTR:1;         /*!< bit:      6  Output Driver Strength Selection   */
    uint8_t  :1;               /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PORT_PINCFG_Type;


typedef struct {
  volatile PORT_DIR_Type             DIR;         /**< \brief Offset: 0x00 (R/W 32) Data Direction */
  volatile PORT_DIRCLR_Type          DIRCLR;      /**< \brief Offset: 0x04 (R/W 32) Data Direction Clear */
  volatile PORT_DIRSET_Type          DIRSET;      /**< \brief Offset: 0x08 (R/W 32) Data Direction Set */
  volatile PORT_DIRTGL_Type          DIRTGL;      /**< \brief Offset: 0x0C (R/W 32) Data Direction Toggle */
  volatile PORT_OUT_Type             OUT;         /**< \brief Offset: 0x10 (R/W 32) Data Output Value */
  volatile PORT_OUTCLR_Type          OUTCLR;      /**< \brief Offset: 0x14 (R/W 32) Data Output Value Clear */
  volatile PORT_OUTSET_Type          OUTSET;      /**< \brief Offset: 0x18 (R/W 32) Data Output Value Set */
  volatile PORT_OUTTGL_Type          OUTTGL;      /**< \brief Offset: 0x1C (R/W 32) Data Output Value Toggle */
  volatile PORT_IN_Type              IN;          /**< \brief Offset: 0x20 (R/  32) Data Input Value */
  volatile PORT_CTRL_Type            CTRL;        /**< \brief Offset: 0x24 (R/W 32) Control */
  volatile PORT_WRCONFIG_Type        WRCONFIG;    /**< \brief Offset: 0x28 ( /W 32) Write Configuration */
       RoReg8                    Reserved1[0x4];
  volatile PORT_PMUX_Type            PMUX[16];    /**< \brief Offset: 0x30 (R/W  8) Peripheral Multiplexing n */
  volatile PORT_PINCFG_Type          PINCFG[32];  /**< \brief Offset: 0x40 (R/W  8) Pin Configuration n */
       RoReg8                    Reserved2[0x20];
} PortGroup;

typedef struct {
       PortGroup                 Group[PORT_GROUP_NUM];    /**< \brief Offset: 0x00 PortGroup groups [GROUPS] */
} Port;

extern volatile Port* PORT;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif