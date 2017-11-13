/*---------------------------------------------------------------------------/
/  FatFs - FAT file system module configuration file  R0.10b (C)ChaN, 2014
/---------------------------------------------------------------------------*/

#ifndef _FFCONF
#define _FFCONF 32020

/*-----------------------------------------------------------------------------/
/ Additional user header to be used  
/-----------------------------------------------------------------------------*/
/* Replace 'stm32xxx' with the STM32 Serie used, ex: stm32f4xx_hal.h */
#include "stm32f4xx_hal.h" 

/* If uSD is used, then include the uSD BSP header file. 
   Replace 'stm32xxx' with your EVAL board name, ex: stm324x9i_eval_sd.h 
   */
//#include "stm32xxx_eval_sd.h"

/* If USB Host MSC is used, then need to include the USBH and UBSH MSC core header files */
#include "usbh_core.h"
#include "usbh_msc.h"
/* hUSBH to be updated with the USBH handle defined in the application code */
#define  HOST_HANDLE   handleUSBH 

/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations
/---------------------------------------------------------------------------*/

#define	_USE_ERASE              0	/* 0:Disable or 1:Enable */
/* To enable sector erase feature, set _USE_ERASE to 1. Also CTRL_ERASE_SECTOR command
/  should be added to the disk_ioctl() function. */


/*---------------------------------------------------------------------------/
/ System Configurations
/---------------------------------------------------------------------------*/


#define _WORD_ACCESS            0	/* 0 or 1 */
/* The _WORD_ACCESS option is an only platform dependent option. It defines
/  which access method is used to the word data on the FAT volume.
/
/   0: Byte-by-byte access. Always compatible with all platforms.
/   1: Word access. Do not choose this unless under both the following conditions.
/
/  * Address misaligned memory access is always allowed for ALL instructions.
/  * Byte order on the memory is little-endian.
/
/  If it is the case, _WORD_ACCESS can also be set to 1 to improve performance and
/  reduce code size. Following table shows an example of some processor types.
/
/   ARM7TDMI    0           ColdFire    0           V850E       0
/   Cortex-M3   0           Z80         0/1         V850ES      0/1
/   Cortex-M0   0           RX600(LE)   0/1         TLCS-870    0/1
/   AVR         0/1         RX600(BE)   0           TLCS-900    0/1
/   AVR32       0           RL78        0           R32C        0
/   PIC18       0/1         SH-2        0           M16C        0/1
/   PIC24       0           H8S         0           MSP430      0
/   PIC32       0           H8/300H     0           x86         0/1
*/


//#define _USE_FIND 0
//#define _FS_NORTC 1

#endif /* _FFCONF */
