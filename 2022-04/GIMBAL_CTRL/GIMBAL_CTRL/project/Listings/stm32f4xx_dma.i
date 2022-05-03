# 1 "..\\stm32LIB\\src\\stm32f4xx_dma.c"
























































































































  

 
# 1 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"


























  

 







 
# 1 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"










































  



 



 
    






  


 
  


 

# 85 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




 






 





# 112 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"







            



  





 










 
# 149 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
                                             


 



 



 









 
typedef enum IRQn
{
 
  NonMaskableInt_IRQn         = -14,     
  MemoryManagement_IRQn       = -12,     
  BusFault_IRQn               = -11,     
  UsageFault_IRQn             = -10,     
  SVCall_IRQn                 = -5,      
  DebugMonitor_IRQn           = -4,      
  PendSV_IRQn                 = -2,      
  SysTick_IRQn                = -1,      
 
  WWDG_IRQn                   = 0,       
  PVD_IRQn                    = 1,       
  TAMP_STAMP_IRQn             = 2,       
  RTC_WKUP_IRQn               = 3,       
  FLASH_IRQn                  = 4,       
  RCC_IRQn                    = 5,       
  EXTI0_IRQn                  = 6,       
  EXTI1_IRQn                  = 7,       
  EXTI2_IRQn                  = 8,       
  EXTI3_IRQn                  = 9,       
  EXTI4_IRQn                  = 10,      
  DMA1_Stream0_IRQn           = 11,      
  DMA1_Stream1_IRQn           = 12,      
  DMA1_Stream2_IRQn           = 13,      
  DMA1_Stream3_IRQn           = 14,      
  DMA1_Stream4_IRQn           = 15,      
  DMA1_Stream5_IRQn           = 16,      
  DMA1_Stream6_IRQn           = 17,      
  ADC_IRQn                    = 18,      


  CAN1_TX_IRQn                = 19,      
  CAN1_RX0_IRQn               = 20,      
  CAN1_RX1_IRQn               = 21,      
  CAN1_SCE_IRQn               = 22,      
  EXTI9_5_IRQn                = 23,      
  TIM1_BRK_TIM9_IRQn          = 24,      
  TIM1_UP_TIM10_IRQn          = 25,      
  TIM1_TRG_COM_TIM11_IRQn     = 26,      
  TIM1_CC_IRQn                = 27,      
  TIM2_IRQn                   = 28,      
  TIM3_IRQn                   = 29,      
  TIM4_IRQn                   = 30,      
  I2C1_EV_IRQn                = 31,      
  I2C1_ER_IRQn                = 32,      
  I2C2_EV_IRQn                = 33,      
  I2C2_ER_IRQn                = 34,        
  SPI1_IRQn                   = 35,      
  SPI2_IRQn                   = 36,      
  USART1_IRQn                 = 37,      
  USART2_IRQn                 = 38,      
  USART3_IRQn                 = 39,      
  EXTI15_10_IRQn              = 40,      
  RTC_Alarm_IRQn              = 41,      
  OTG_FS_WKUP_IRQn            = 42,          
  TIM8_BRK_TIM12_IRQn         = 43,      
  TIM8_UP_TIM13_IRQn          = 44,      
  TIM8_TRG_COM_TIM14_IRQn     = 45,      
  TIM8_CC_IRQn                = 46,      
  DMA1_Stream7_IRQn           = 47,      
  FSMC_IRQn                   = 48,      
  SDIO_IRQn                   = 49,      
  TIM5_IRQn                   = 50,      
  SPI3_IRQn                   = 51,      
  UART4_IRQn                  = 52,      
  UART5_IRQn                  = 53,      
  TIM6_DAC_IRQn               = 54,      
  TIM7_IRQn                   = 55,      
  DMA2_Stream0_IRQn           = 56,      
  DMA2_Stream1_IRQn           = 57,      
  DMA2_Stream2_IRQn           = 58,      
  DMA2_Stream3_IRQn           = 59,      
  DMA2_Stream4_IRQn           = 60,      
  ETH_IRQn                    = 61,      
  ETH_WKUP_IRQn               = 62,      
  CAN2_TX_IRQn                = 63,      
  CAN2_RX0_IRQn               = 64,      
  CAN2_RX1_IRQn               = 65,      
  CAN2_SCE_IRQn               = 66,      
  OTG_FS_IRQn                 = 67,      
  DMA2_Stream5_IRQn           = 68,      
  DMA2_Stream6_IRQn           = 69,      
  DMA2_Stream7_IRQn           = 70,      
  USART6_IRQn                 = 71,      
  I2C3_EV_IRQn                = 72,      
  I2C3_ER_IRQn                = 73,      
  OTG_HS_EP1_OUT_IRQn         = 74,      
  OTG_HS_EP1_IN_IRQn          = 75,      
  OTG_HS_WKUP_IRQn            = 76,      
  OTG_HS_IRQn                 = 77,      
  DCMI_IRQn                   = 78,      
  CRYP_IRQn                   = 79,      
  HASH_RNG_IRQn               = 80,      
  FPU_IRQn                    = 81       


# 341 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
    
# 416 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
   
# 456 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

} IRQn_Type;



 

# 1 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"
 




 

























 











# 1 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"
 
 





 









     
# 27 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"
     











# 46 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"





 

     

     
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

     
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

     

     
     
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
typedef   signed       __int64 int_least64_t;

     
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned       __int64 uint_least64_t;

     

     
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef   signed       __int64 int_fast64_t;

     
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned       __int64 uint_fast64_t;

     




typedef   signed           int intptr_t;
typedef unsigned           int uintptr_t;


     
typedef   signed     long long intmax_t;
typedef unsigned     long long uintmax_t;




     

     





     





     





     

     





     





     





     

     





     





     





     

     






     






     






     

     


     


     


     

     
# 216 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"

     



     






     
    
 



# 241 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"

     







     










     











# 305 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"






 
# 45 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"

















 




 



 

 













# 120 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"



 
# 135 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"

# 209 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"

# 1 "..\\stm32LIB\\CMSIS\\Include\\core_cmInstr.h"
 




 

























 












 



 

 
# 1 "..\\stm32LIB\\CMSIS\\Include\\cmsis_armcc.h"
 




 

























 










 



 

 
 





 
static __inline uint32_t __get_CONTROL(void)
{
  register uint32_t __regControl         __asm("control");
  return(__regControl);
}






 
static __inline void __set_CONTROL(uint32_t control)
{
  register uint32_t __regControl         __asm("control");
  __regControl = control;
}






 
static __inline uint32_t __get_IPSR(void)
{
  register uint32_t __regIPSR          __asm("ipsr");
  return(__regIPSR);
}






 
static __inline uint32_t __get_APSR(void)
{
  register uint32_t __regAPSR          __asm("apsr");
  return(__regAPSR);
}






 
static __inline uint32_t __get_xPSR(void)
{
  register uint32_t __regXPSR          __asm("xpsr");
  return(__regXPSR);
}






 
static __inline uint32_t __get_PSP(void)
{
  register uint32_t __regProcessStackPointer  __asm("psp");
  return(__regProcessStackPointer);
}






 
static __inline void __set_PSP(uint32_t topOfProcStack)
{
  register uint32_t __regProcessStackPointer  __asm("psp");
  __regProcessStackPointer = topOfProcStack;
}






 
static __inline uint32_t __get_MSP(void)
{
  register uint32_t __regMainStackPointer     __asm("msp");
  return(__regMainStackPointer);
}






 
static __inline void __set_MSP(uint32_t topOfMainStack)
{
  register uint32_t __regMainStackPointer     __asm("msp");
  __regMainStackPointer = topOfMainStack;
}






 
static __inline uint32_t __get_PRIMASK(void)
{
  register uint32_t __regPriMask         __asm("primask");
  return(__regPriMask);
}






 
static __inline void __set_PRIMASK(uint32_t priMask)
{
  register uint32_t __regPriMask         __asm("primask");
  __regPriMask = (priMask);
}








 







 







 
static __inline uint32_t  __get_BASEPRI(void)
{
  register uint32_t __regBasePri         __asm("basepri");
  return(__regBasePri);
}






 
static __inline void __set_BASEPRI(uint32_t basePri)
{
  register uint32_t __regBasePri         __asm("basepri");
  __regBasePri = (basePri & 0xFFU);
}







 
static __inline void __set_BASEPRI_MAX(uint32_t basePri)
{
  register uint32_t __regBasePriMax      __asm("basepri_max");
  __regBasePriMax = (basePri & 0xFFU);
}






 
static __inline uint32_t __get_FAULTMASK(void)
{
  register uint32_t __regFaultMask       __asm("faultmask");
  return(__regFaultMask);
}






 
static __inline void __set_FAULTMASK(uint32_t faultMask)
{
  register uint32_t __regFaultMask       __asm("faultmask");
  __regFaultMask = (faultMask & (uint32_t)1);
}










 
static __inline uint32_t __get_FPSCR(void)
{

  register uint32_t __regfpscr         __asm("fpscr");
  return(__regfpscr);



}






 
static __inline void __set_FPSCR(uint32_t fpscr)
{

  register uint32_t __regfpscr         __asm("fpscr");
  __regfpscr = (fpscr);

}





 


 



 




 






 







 






 








 










 










 











 








 

__attribute__((section(".rev16_text"))) static __inline __asm uint32_t __REV16(uint32_t value)
{
  rev16 r0, r0
  bx lr
}







 

__attribute__((section(".revsh_text"))) static __inline __asm int32_t __REVSH(int32_t value)
{
  revsh r0, r0
  bx lr
}









 









 








 
# 455 "..\\stm32LIB\\CMSIS\\Include\\cmsis_armcc.h"







 










 












 












 














 














 














 










 









 









 









 

__attribute__((section(".rrx_text"))) static __inline __asm uint32_t __RRX(uint32_t value)
{
  rrx r0, r0
  bx lr
}








 








 








 








 








 








 




   


 



 



# 720 "..\\stm32LIB\\CMSIS\\Include\\cmsis_armcc.h"











 


# 54 "..\\stm32LIB\\CMSIS\\Include\\core_cmInstr.h"

 
# 84 "..\\stm32LIB\\CMSIS\\Include\\core_cmInstr.h"

   

# 211 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"
# 1 "..\\stm32LIB\\CMSIS\\Include\\core_cmFunc.h"
 




 

























 












 



 

 
# 54 "..\\stm32LIB\\CMSIS\\Include\\core_cmFunc.h"

 
# 84 "..\\stm32LIB\\CMSIS\\Include\\core_cmFunc.h"

 

# 212 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"
# 1 "..\\stm32LIB\\CMSIS\\Include\\core_cmSimd.h"
 




 

























 
















 



 

 
# 58 "..\\stm32LIB\\CMSIS\\Include\\core_cmSimd.h"

 
# 88 "..\\stm32LIB\\CMSIS\\Include\\core_cmSimd.h"

 






# 213 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"
















 
# 256 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"

 






 
# 272 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"

 




 













 



 






 



 
typedef union
{
  struct
  {
    uint32_t _reserved0:16;               
    uint32_t GE:4;                        
    uint32_t _reserved1:7;                
    uint32_t Q:1;                         
    uint32_t V:1;                         
    uint32_t C:1;                         
    uint32_t Z:1;                         
    uint32_t N:1;                         
  } b;                                    
  uint32_t w;                             
} APSR_Type;

 





















 
typedef union
{
  struct
  {
    uint32_t ISR:9;                       
    uint32_t _reserved0:23;               
  } b;                                    
  uint32_t w;                             
} IPSR_Type;

 






 
typedef union
{
  struct
  {
    uint32_t ISR:9;                       
    uint32_t _reserved0:7;                
    uint32_t GE:4;                        
    uint32_t _reserved1:4;                
    uint32_t T:1;                         
    uint32_t IT:2;                        
    uint32_t Q:1;                         
    uint32_t V:1;                         
    uint32_t C:1;                         
    uint32_t Z:1;                         
    uint32_t N:1;                         
  } b;                                    
  uint32_t w;                             
} xPSR_Type;

 






























 
typedef union
{
  struct
  {
    uint32_t nPRIV:1;                     
    uint32_t SPSEL:1;                     
    uint32_t FPCA:1;                      
    uint32_t _reserved0:29;               
  } b;                                    
  uint32_t w;                             
} CONTROL_Type;

 









 







 



 
typedef struct
{
  volatile uint32_t ISER[8U];                
        uint32_t RESERVED0[24U];
  volatile uint32_t ICER[8U];                
        uint32_t RSERVED1[24U];
  volatile uint32_t ISPR[8U];                
        uint32_t RESERVED2[24U];
  volatile uint32_t ICPR[8U];                
        uint32_t RESERVED3[24U];
  volatile uint32_t IABR[8U];                
        uint32_t RESERVED4[56U];
  volatile uint8_t  IP[240U];                
        uint32_t RESERVED5[644U];
  volatile  uint32_t STIR;                    
}  NVIC_Type;

 



 







 



 
typedef struct
{
  volatile const  uint32_t CPUID;                   
  volatile uint32_t ICSR;                    
  volatile uint32_t VTOR;                    
  volatile uint32_t AIRCR;                   
  volatile uint32_t SCR;                     
  volatile uint32_t CCR;                     
  volatile uint8_t  SHP[12U];                
  volatile uint32_t SHCSR;                   
  volatile uint32_t CFSR;                    
  volatile uint32_t HFSR;                    
  volatile uint32_t DFSR;                    
  volatile uint32_t MMFAR;                   
  volatile uint32_t BFAR;                    
  volatile uint32_t AFSR;                    
  volatile const  uint32_t PFR[2U];                 
  volatile const  uint32_t DFR;                     
  volatile const  uint32_t ADR;                     
  volatile const  uint32_t MMFR[4U];                
  volatile const  uint32_t ISAR[5U];                
        uint32_t RESERVED0[5U];
  volatile uint32_t CPACR;                   
} SCB_Type;

 















 






























 



 





















 









 


















 










































 









 









 















 







 



 
typedef struct
{
        uint32_t RESERVED0[1U];
  volatile const  uint32_t ICTR;                    
  volatile uint32_t ACTLR;                   
} SCnSCB_Type;

 



 















 







 



 
typedef struct
{
  volatile uint32_t CTRL;                    
  volatile uint32_t LOAD;                    
  volatile uint32_t VAL;                     
  volatile const  uint32_t CALIB;                   
} SysTick_Type;

 












 



 



 









 







 



 
typedef struct
{
  volatile  union
  {
    volatile  uint8_t    u8;                  
    volatile  uint16_t   u16;                 
    volatile  uint32_t   u32;                 
  }  PORT [32U];                          
        uint32_t RESERVED0[864U];
  volatile uint32_t TER;                     
        uint32_t RESERVED1[15U];
  volatile uint32_t TPR;                     
        uint32_t RESERVED2[15U];
  volatile uint32_t TCR;                     
        uint32_t RESERVED3[29U];
  volatile  uint32_t IWR;                     
  volatile const  uint32_t IRR;                     
  volatile uint32_t IMCR;                    
        uint32_t RESERVED4[43U];
  volatile  uint32_t LAR;                     
  volatile const  uint32_t LSR;                     
        uint32_t RESERVED5[6U];
  volatile const  uint32_t PID4;                    
  volatile const  uint32_t PID5;                    
  volatile const  uint32_t PID6;                    
  volatile const  uint32_t PID7;                    
  volatile const  uint32_t PID0;                    
  volatile const  uint32_t PID1;                    
  volatile const  uint32_t PID2;                    
  volatile const  uint32_t PID3;                    
  volatile const  uint32_t CID0;                    
  volatile const  uint32_t CID1;                    
  volatile const  uint32_t CID2;                    
  volatile const  uint32_t CID3;                    
} ITM_Type;

 



 



























 



 



 



 









   







 



 
typedef struct
{
  volatile uint32_t CTRL;                    
  volatile uint32_t CYCCNT;                  
  volatile uint32_t CPICNT;                  
  volatile uint32_t EXCCNT;                  
  volatile uint32_t SLEEPCNT;                
  volatile uint32_t LSUCNT;                  
  volatile uint32_t FOLDCNT;                 
  volatile const  uint32_t PCSR;                    
  volatile uint32_t COMP0;                   
  volatile uint32_t MASK0;                   
  volatile uint32_t FUNCTION0;               
        uint32_t RESERVED0[1U];
  volatile uint32_t COMP1;                   
  volatile uint32_t MASK1;                   
  volatile uint32_t FUNCTION1;               
        uint32_t RESERVED1[1U];
  volatile uint32_t COMP2;                   
  volatile uint32_t MASK2;                   
  volatile uint32_t FUNCTION2;               
        uint32_t RESERVED2[1U];
  volatile uint32_t COMP3;                   
  volatile uint32_t MASK3;                   
  volatile uint32_t FUNCTION3;               
} DWT_Type;

 






















































 



 



 



 



 



 



 



























   







 



 
typedef struct
{
  volatile uint32_t SSPSR;                   
  volatile uint32_t CSPSR;                   
        uint32_t RESERVED0[2U];
  volatile uint32_t ACPR;                    
        uint32_t RESERVED1[55U];
  volatile uint32_t SPPR;                    
        uint32_t RESERVED2[131U];
  volatile const  uint32_t FFSR;                    
  volatile uint32_t FFCR;                    
  volatile const  uint32_t FSCR;                    
        uint32_t RESERVED3[759U];
  volatile const  uint32_t TRIGGER;                 
  volatile const  uint32_t FIFO0;                   
  volatile const  uint32_t ITATBCTR2;               
        uint32_t RESERVED4[1U];
  volatile const  uint32_t ITATBCTR0;               
  volatile const  uint32_t FIFO1;                   
  volatile uint32_t ITCTRL;                  
        uint32_t RESERVED5[39U];
  volatile uint32_t CLAIMSET;                
  volatile uint32_t CLAIMCLR;                
        uint32_t RESERVED7[8U];
  volatile const  uint32_t DEVID;                   
  volatile const  uint32_t DEVTYPE;                 
} TPI_Type;

 



 



 












 






 



 





















 



 





















 



 



 


















 






   








 



 
typedef struct
{
  volatile const  uint32_t TYPE;                    
  volatile uint32_t CTRL;                    
  volatile uint32_t RNR;                     
  volatile uint32_t RBAR;                    
  volatile uint32_t RASR;                    
  volatile uint32_t RBAR_A1;                 
  volatile uint32_t RASR_A1;                 
  volatile uint32_t RBAR_A2;                 
  volatile uint32_t RASR_A2;                 
  volatile uint32_t RBAR_A3;                 
  volatile uint32_t RASR_A3;                 
} MPU_Type;

 









 









 



 









 






























 









 



 
typedef struct
{
        uint32_t RESERVED0[1U];
  volatile uint32_t FPCCR;                   
  volatile uint32_t FPCAR;                   
  volatile uint32_t FPDSCR;                  
  volatile const  uint32_t MVFR0;                   
  volatile const  uint32_t MVFR1;                   
} FPU_Type;

 



























 



 












 
























 












 








 



 
typedef struct
{
  volatile uint32_t DHCSR;                   
  volatile  uint32_t DCRSR;                   
  volatile uint32_t DCRDR;                   
  volatile uint32_t DEMCR;                   
} CoreDebug_Type;

 




































 






 







































 







 






 







 


 







 

 
# 1541 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"

# 1550 "..\\stm32LIB\\CMSIS\\Include\\core_cm4.h"











 










 


 



 





 









 
static __inline void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);              

  reg_value  =  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR;                                                    
  reg_value &= ~((uint32_t)((0xFFFFUL << 16U) | (7UL << 8U)));  
  reg_value  =  (reg_value                                   |
                ((uint32_t)0x5FAUL << 16U) |
                (PriorityGroupTmp << 8U)                      );               
  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR =  reg_value;
}






 
static __inline uint32_t NVIC_GetPriorityGrouping(void)
{
  return ((uint32_t)((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR & (7UL << 8U)) >> 8U));
}






 
static __inline void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISER[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}






 
static __inline void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ICER[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}








 
static __inline uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISPR[(((uint32_t)(int32_t)IRQn) >> 5UL)] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}






 
static __inline void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISPR[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}






 
static __inline void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ICPR[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}








 
static __inline uint32_t NVIC_GetActive(IRQn_Type IRQn)
{
  return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IABR[(((uint32_t)(int32_t)IRQn) >> 5UL)] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}








 
static __inline void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if ((int32_t)(IRQn) < 0)
  {
    ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[(((uint32_t)(int32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - 4)) & (uint32_t)0xFFUL);
  }
  else
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[((uint32_t)(int32_t)IRQn)]               = (uint8_t)((priority << (8U - 4)) & (uint32_t)0xFFUL);
  }
}










 
static __inline uint32_t NVIC_GetPriority(IRQn_Type IRQn)
{

  if ((int32_t)(IRQn) < 0)
  {
    return(((uint32_t)((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[(((uint32_t)(int32_t)IRQn) & 0xFUL)-4UL] >> (8U - 4)));
  }
  else
  {
    return(((uint32_t)((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[((uint32_t)(int32_t)IRQn)]               >> (8U - 4)));
  }
}












 
static __inline uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);    
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(4)) ? (uint32_t)(4) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits     = ((PriorityGroupTmp + (uint32_t)(4)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(4));

  return (
           ((PreemptPriority & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
           ((SubPriority     & (uint32_t)((1UL << (SubPriorityBits    )) - 1UL)))
         );
}












 
static __inline void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* const pPreemptPriority, uint32_t* const pSubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);    
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(4)) ? (uint32_t)(4) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits     = ((PriorityGroupTmp + (uint32_t)(4)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(4));

  *pPreemptPriority = (Priority >> SubPriorityBits) & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL);
  *pSubPriority     = (Priority                   ) & (uint32_t)((1UL << (SubPriorityBits    )) - 1UL);
}





 
static __inline void NVIC_SystemReset(void)
{
  do { __schedule_barrier(); __dsb(0xF); __schedule_barrier(); } while (0U);                                                          
 
  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR  = (uint32_t)((0x5FAUL << 16U)    |
                           (((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR & (7UL << 8U)) |
                            (1UL << 2U)    );          
  do { __schedule_barrier(); __dsb(0xF); __schedule_barrier(); } while (0U);                                                           

  for(;;)                                                            
  {
    __nop();
  }
}

 



 





 













 
static __inline uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > (0xFFFFFFUL ))
  {
    return (1UL);                                                    
  }

  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->LOAD  = (uint32_t)(ticks - 1UL);                          
  NVIC_SetPriority (SysTick_IRQn, (1UL << 4) - 1UL);  
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->VAL   = 0UL;                                              
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL  = (1UL << 2U) |
                   (1UL << 1U)   |
                   (1UL );                          
  return (0UL);                                                      
}



 



 





 

extern volatile int32_t ITM_RxBuffer;                     










 
static __inline uint32_t ITM_SendChar (uint32_t ch)
{
  if (((((ITM_Type *) (0xE0000000UL) )->TCR & (1UL )) != 0UL) &&       
      ((((ITM_Type *) (0xE0000000UL) )->TER & 1UL               ) != 0UL)   )      
  {
    while (((ITM_Type *) (0xE0000000UL) )->PORT[0U].u32 == 0UL)
    {
      __nop();
    }
    ((ITM_Type *) (0xE0000000UL) )->PORT[0U].u8 = (uint8_t)ch;
  }
  return (ch);
}







 
static __inline int32_t ITM_ReceiveChar (void)
{
  int32_t ch = -1;                            

  if (ITM_RxBuffer != 0x5AA55AA5U)
  {
    ch = ITM_RxBuffer;
    ITM_RxBuffer = 0x5AA55AA5U;        
  }

  return (ch);
}







 
static __inline int32_t ITM_CheckChar (void)
{

  if (ITM_RxBuffer == 0x5AA55AA5U)
  {
    return (0);                               
  }
  else
  {
    return (1);                               
  }
}

 










# 464 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
# 1 "..\\stm32LIB\\CMSIS\\system_stm32f4xx.h"

























  



 



   
  


 









 



 




 

extern uint32_t SystemCoreClock;           




 



 



 



 



 



 
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);


 









 
  


   
 
# 465 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
# 466 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



   
 
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;   
typedef const int16_t sc16;   
typedef const int8_t sc8;    

typedef volatile int32_t  vs32;
typedef volatile int16_t  vs16;
typedef volatile int8_t   vs8;

typedef volatile const int32_t vsc32;   
typedef volatile const int16_t vsc16;   
typedef volatile const int8_t vsc8;    

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;   
typedef const uint16_t uc16;   
typedef const uint8_t uc8;    

typedef volatile uint32_t  vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t  vu8;

typedef volatile const uint32_t vuc32;   
typedef volatile const uint16_t vuc16;   
typedef volatile const uint8_t vuc8;    

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;


typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;



 



    



 

typedef struct
{
  volatile uint32_t SR;      
  volatile uint32_t CR1;           
  volatile uint32_t CR2;     
  volatile uint32_t SMPR1;   
  volatile uint32_t SMPR2;   
  volatile uint32_t JOFR1;   
  volatile uint32_t JOFR2;   
  volatile uint32_t JOFR3;   
  volatile uint32_t JOFR4;   
  volatile uint32_t HTR;     
  volatile uint32_t LTR;     
  volatile uint32_t SQR1;    
  volatile uint32_t SQR2;    
  volatile uint32_t SQR3;    
  volatile uint32_t JSQR;    
  volatile uint32_t JDR1;    
  volatile uint32_t JDR2;    
  volatile uint32_t JDR3;    
  volatile uint32_t JDR4;    
  volatile uint32_t DR;      
} ADC_TypeDef;

typedef struct
{
  volatile uint32_t CSR;     
  volatile uint32_t CCR;     
  volatile uint32_t CDR;    
 
} ADC_Common_TypeDef;




 

typedef struct
{
  volatile uint32_t TIR;   
  volatile uint32_t TDTR;  
  volatile uint32_t TDLR;  
  volatile uint32_t TDHR;  
} CAN_TxMailBox_TypeDef;



 
  
typedef struct
{
  volatile uint32_t RIR;   
  volatile uint32_t RDTR;  
  volatile uint32_t RDLR;  
  volatile uint32_t RDHR;  
} CAN_FIFOMailBox_TypeDef;



 
  
typedef struct
{
  volatile uint32_t FR1;  
  volatile uint32_t FR2;  
} CAN_FilterRegister_TypeDef;



 
  
typedef struct
{
  volatile uint32_t              MCR;                  
  volatile uint32_t              MSR;                  
  volatile uint32_t              TSR;                  
  volatile uint32_t              RF0R;                 
  volatile uint32_t              RF1R;                 
  volatile uint32_t              IER;                  
  volatile uint32_t              ESR;                  
  volatile uint32_t              BTR;                  
  uint32_t                   RESERVED0[88];        
  CAN_TxMailBox_TypeDef      sTxMailBox[3];        
  CAN_FIFOMailBox_TypeDef    sFIFOMailBox[2];      
  uint32_t                   RESERVED1[12];        
  volatile uint32_t              FMR;                  
  volatile uint32_t              FM1R;                 
  uint32_t                   RESERVED2;            
  volatile uint32_t              FS1R;                 
  uint32_t                   RESERVED3;            
  volatile uint32_t              FFA1R;                
  uint32_t                   RESERVED4;            
  volatile uint32_t              FA1R;                 
  uint32_t                   RESERVED5[8];          
  CAN_FilterRegister_TypeDef sFilterRegister[28];  
} CAN_TypeDef;



 

typedef struct
{
  volatile uint32_t DR;          
  volatile uint8_t  IDR;         
  uint8_t       RESERVED0;   
  uint16_t      RESERVED1;   
  volatile uint32_t CR;          
} CRC_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;        
  volatile uint32_t SWTRIGR;   
  volatile uint32_t DHR12R1;   
  volatile uint32_t DHR12L1;   
  volatile uint32_t DHR8R1;    
  volatile uint32_t DHR12R2;   
  volatile uint32_t DHR12L2;   
  volatile uint32_t DHR8R2;    
  volatile uint32_t DHR12RD;   
  volatile uint32_t DHR12LD;   
  volatile uint32_t DHR8RD;    
  volatile uint32_t DOR1;      
  volatile uint32_t DOR2;      
  volatile uint32_t SR;        
} DAC_TypeDef;



 

typedef struct
{
  volatile uint32_t IDCODE;   
  volatile uint32_t CR;       
  volatile uint32_t APB1FZ;   
  volatile uint32_t APB2FZ;   
}DBGMCU_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;        
  volatile uint32_t SR;        
  volatile uint32_t RISR;      
  volatile uint32_t IER;       
  volatile uint32_t MISR;      
  volatile uint32_t ICR;       
  volatile uint32_t ESCR;      
  volatile uint32_t ESUR;      
  volatile uint32_t CWSTRTR;   
  volatile uint32_t CWSIZER;   
  volatile uint32_t DR;        
} DCMI_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;      
  volatile uint32_t NDTR;    
  volatile uint32_t PAR;     
  volatile uint32_t M0AR;    
  volatile uint32_t M1AR;    
  volatile uint32_t FCR;     
} DMA_Stream_TypeDef;

typedef struct
{
  volatile uint32_t LISR;    
  volatile uint32_t HISR;    
  volatile uint32_t LIFCR;   
  volatile uint32_t HIFCR;   
} DMA_TypeDef;
 


 

typedef struct
{
  volatile uint32_t CR;             
  volatile uint32_t ISR;            
  volatile uint32_t IFCR;           
  volatile uint32_t FGMAR;          
  volatile uint32_t FGOR;           
  volatile uint32_t BGMAR;          
  volatile uint32_t BGOR;           
  volatile uint32_t FGPFCCR;        
  volatile uint32_t FGCOLR;         
  volatile uint32_t BGPFCCR;        
  volatile uint32_t BGCOLR;         
  volatile uint32_t FGCMAR;         
  volatile uint32_t BGCMAR;         
  volatile uint32_t OPFCCR;         
  volatile uint32_t OCOLR;          
  volatile uint32_t OMAR;           
  volatile uint32_t OOR;            
  volatile uint32_t NLR;            
  volatile uint32_t LWR;            
  volatile uint32_t AMTCR;          
  uint32_t      RESERVED[236];  
  volatile uint32_t FGCLUT[256];    
  volatile uint32_t BGCLUT[256];    
} DMA2D_TypeDef;



 

typedef struct
{
  volatile uint32_t MACCR;
  volatile uint32_t MACFFR;
  volatile uint32_t MACHTHR;
  volatile uint32_t MACHTLR;
  volatile uint32_t MACMIIAR;
  volatile uint32_t MACMIIDR;
  volatile uint32_t MACFCR;
  volatile uint32_t MACVLANTR;              
  uint32_t      RESERVED0[2];
  volatile uint32_t MACRWUFFR;              
  volatile uint32_t MACPMTCSR;
  uint32_t      RESERVED1[2];
  volatile uint32_t MACSR;                  
  volatile uint32_t MACIMR;
  volatile uint32_t MACA0HR;
  volatile uint32_t MACA0LR;
  volatile uint32_t MACA1HR;
  volatile uint32_t MACA1LR;
  volatile uint32_t MACA2HR;
  volatile uint32_t MACA2LR;
  volatile uint32_t MACA3HR;
  volatile uint32_t MACA3LR;                
  uint32_t      RESERVED2[40];
  volatile uint32_t MMCCR;                  
  volatile uint32_t MMCRIR;
  volatile uint32_t MMCTIR;
  volatile uint32_t MMCRIMR;
  volatile uint32_t MMCTIMR;                
  uint32_t      RESERVED3[14];
  volatile uint32_t MMCTGFSCCR;             
  volatile uint32_t MMCTGFMSCCR;
  uint32_t      RESERVED4[5];
  volatile uint32_t MMCTGFCR;
  uint32_t      RESERVED5[10];
  volatile uint32_t MMCRFCECR;
  volatile uint32_t MMCRFAECR;
  uint32_t      RESERVED6[10];
  volatile uint32_t MMCRGUFCR;
  uint32_t      RESERVED7[334];
  volatile uint32_t PTPTSCR;
  volatile uint32_t PTPSSIR;
  volatile uint32_t PTPTSHR;
  volatile uint32_t PTPTSLR;
  volatile uint32_t PTPTSHUR;
  volatile uint32_t PTPTSLUR;
  volatile uint32_t PTPTSAR;
  volatile uint32_t PTPTTHR;
  volatile uint32_t PTPTTLR;
  volatile uint32_t RESERVED8;
  volatile uint32_t PTPTSSR;
  uint32_t      RESERVED9[565];
  volatile uint32_t DMABMR;
  volatile uint32_t DMATPDR;
  volatile uint32_t DMARPDR;
  volatile uint32_t DMARDLAR;
  volatile uint32_t DMATDLAR;
  volatile uint32_t DMASR;
  volatile uint32_t DMAOMR;
  volatile uint32_t DMAIER;
  volatile uint32_t DMAMFBOCR;
  volatile uint32_t DMARSWTR;
  uint32_t      RESERVED10[8];
  volatile uint32_t DMACHTDR;
  volatile uint32_t DMACHRDR;
  volatile uint32_t DMACHTBAR;
  volatile uint32_t DMACHRBAR;
} ETH_TypeDef;



 

typedef struct
{
  volatile uint32_t IMR;     
  volatile uint32_t EMR;     
  volatile uint32_t RTSR;    
  volatile uint32_t FTSR;    
  volatile uint32_t SWIER;   
  volatile uint32_t PR;      
} EXTI_TypeDef;



 

typedef struct
{
  volatile uint32_t ACR;       
  volatile uint32_t KEYR;      
  volatile uint32_t OPTKEYR;   
  volatile uint32_t SR;        
  volatile uint32_t CR;        
  volatile uint32_t OPTCR;     
  volatile uint32_t OPTCR1;    
} FLASH_TypeDef;




 

typedef struct
{
  volatile uint32_t BTCR[8];        
} FSMC_Bank1_TypeDef; 



 
  
typedef struct
{
  volatile uint32_t BWTR[7];     
} FSMC_Bank1E_TypeDef;



 
  
typedef struct
{
  volatile uint32_t PCR2;        
  volatile uint32_t SR2;         
  volatile uint32_t PMEM2;       
  volatile uint32_t PATT2;       
  uint32_t      RESERVED0;   
  volatile uint32_t ECCR2;       
} FSMC_Bank2_TypeDef;



 
  
typedef struct
{
  volatile uint32_t PCR3;        
  volatile uint32_t SR3;         
  volatile uint32_t PMEM3;       
  volatile uint32_t PATT3;       
  uint32_t      RESERVED0;   
  volatile uint32_t ECCR3;       
} FSMC_Bank3_TypeDef;



 
  
typedef struct
{
  volatile uint32_t PCR4;        
  volatile uint32_t SR4;         
  volatile uint32_t PMEM4;       
  volatile uint32_t PATT4;       
  volatile uint32_t PIO4;        
} FSMC_Bank4_TypeDef; 


# 975 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



 

typedef struct
{
  volatile uint32_t MODER;     
  volatile uint32_t OTYPER;    
  volatile uint32_t OSPEEDR;   
  volatile uint32_t PUPDR;     
  volatile uint32_t IDR;       
  volatile uint32_t ODR;       
  volatile uint16_t BSRRL;     
  volatile uint16_t BSRRH;     
  volatile uint32_t LCKR;      
  volatile uint32_t AFR[2];    
} GPIO_TypeDef;



 
  
typedef struct
{
  volatile uint32_t MEMRMP;        
  volatile uint32_t PMC;           
  volatile uint32_t EXTICR[4];     
  uint32_t      RESERVED[2];    
  volatile uint32_t CMPCR;         
} SYSCFG_TypeDef;



 

typedef struct
{
  volatile uint16_t CR1;         
  uint16_t      RESERVED0;   
  volatile uint16_t CR2;         
  uint16_t      RESERVED1;   
  volatile uint16_t OAR1;        
  uint16_t      RESERVED2;   
  volatile uint16_t OAR2;        
  uint16_t      RESERVED3;   
  volatile uint16_t DR;          
  uint16_t      RESERVED4;   
  volatile uint16_t SR1;         
  uint16_t      RESERVED5;   
  volatile uint16_t SR2;         
  uint16_t      RESERVED6;   
  volatile uint16_t CCR;         
  uint16_t      RESERVED7;   
  volatile uint16_t TRISE;       
  uint16_t      RESERVED8;   
  volatile uint16_t FLTR;        
  uint16_t      RESERVED9;   
} I2C_TypeDef;



 

typedef struct
{
  volatile uint32_t KR;    
  volatile uint32_t PR;    
  volatile uint32_t RLR;   
  volatile uint32_t SR;    
} IWDG_TypeDef;



 
  
typedef struct
{
  uint32_t      RESERVED0[2];   
  volatile uint32_t SSCR;           
  volatile uint32_t BPCR;           
  volatile uint32_t AWCR;           
  volatile uint32_t TWCR;           
  volatile uint32_t GCR;            
  uint32_t      RESERVED1[2];   
  volatile uint32_t SRCR;           
  uint32_t      RESERVED2[1];   
  volatile uint32_t BCCR;           
  uint32_t      RESERVED3[1];   
  volatile uint32_t IER;            
  volatile uint32_t ISR;            
  volatile uint32_t ICR;            
  volatile uint32_t LIPCR;          
  volatile uint32_t CPSR;           
  volatile uint32_t CDSR;          
} LTDC_TypeDef;  



 
  
typedef struct
{  
  volatile uint32_t CR;             
  volatile uint32_t WHPCR;          
  volatile uint32_t WVPCR;          
  volatile uint32_t CKCR;           
  volatile uint32_t PFCR;           
  volatile uint32_t CACR;           
  volatile uint32_t DCCR;           
  volatile uint32_t BFCR;           
  uint32_t      RESERVED0[2];   
  volatile uint32_t CFBAR;          
  volatile uint32_t CFBLR;          
  volatile uint32_t CFBLNR;         
  uint32_t      RESERVED1[3];   
  volatile uint32_t CLUTWR;          

} LTDC_Layer_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;    
  volatile uint32_t CSR;   
} PWR_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;             
  volatile uint32_t PLLCFGR;        
  volatile uint32_t CFGR;           
  volatile uint32_t CIR;            
  volatile uint32_t AHB1RSTR;       
  volatile uint32_t AHB2RSTR;       
  volatile uint32_t AHB3RSTR;       
  uint32_t      RESERVED0;      
  volatile uint32_t APB1RSTR;       
  volatile uint32_t APB2RSTR;       
  uint32_t      RESERVED1[2];   
  volatile uint32_t AHB1ENR;        
  volatile uint32_t AHB2ENR;        
  volatile uint32_t AHB3ENR;        
  uint32_t      RESERVED2;      
  volatile uint32_t APB1ENR;        
  volatile uint32_t APB2ENR;        
  uint32_t      RESERVED3[2];   
  volatile uint32_t AHB1LPENR;      
  volatile uint32_t AHB2LPENR;      
  volatile uint32_t AHB3LPENR;      
  uint32_t      RESERVED4;      
  volatile uint32_t APB1LPENR;      
  volatile uint32_t APB2LPENR;      
  uint32_t      RESERVED5[2];   
  volatile uint32_t BDCR;           
  volatile uint32_t CSR;            
  uint32_t      RESERVED6[2];   
  volatile uint32_t SSCGR;          
  volatile uint32_t PLLI2SCFGR;     
  volatile uint32_t PLLSAICFGR;     
  volatile uint32_t DCKCFGR;        

} RCC_TypeDef;



 

typedef struct
{
  volatile uint32_t TR;       
  volatile uint32_t DR;       
  volatile uint32_t CR;       
  volatile uint32_t ISR;      
  volatile uint32_t PRER;     
  volatile uint32_t WUTR;     
  volatile uint32_t CALIBR;   
  volatile uint32_t ALRMAR;   
  volatile uint32_t ALRMBR;   
  volatile uint32_t WPR;      
  volatile uint32_t SSR;      
  volatile uint32_t SHIFTR;   
  volatile uint32_t TSTR;     
  volatile uint32_t TSDR;     
  volatile uint32_t TSSSR;    
  volatile uint32_t CALR;     
  volatile uint32_t TAFCR;    
  volatile uint32_t ALRMASSR; 
  volatile uint32_t ALRMBSSR; 
  uint32_t RESERVED7;     
  volatile uint32_t BKP0R;    
  volatile uint32_t BKP1R;    
  volatile uint32_t BKP2R;    
  volatile uint32_t BKP3R;    
  volatile uint32_t BKP4R;    
  volatile uint32_t BKP5R;    
  volatile uint32_t BKP6R;    
  volatile uint32_t BKP7R;    
  volatile uint32_t BKP8R;    
  volatile uint32_t BKP9R;    
  volatile uint32_t BKP10R;   
  volatile uint32_t BKP11R;   
  volatile uint32_t BKP12R;   
  volatile uint32_t BKP13R;   
  volatile uint32_t BKP14R;   
  volatile uint32_t BKP15R;   
  volatile uint32_t BKP16R;   
  volatile uint32_t BKP17R;   
  volatile uint32_t BKP18R;   
  volatile uint32_t BKP19R;   
} RTC_TypeDef;




 
  
typedef struct
{
  volatile uint32_t GCR;       
} SAI_TypeDef;

typedef struct
{
  volatile uint32_t CR1;       
  volatile uint32_t CR2;       
  volatile uint32_t FRCR;      
  volatile uint32_t SLOTR;     
  volatile uint32_t IMR;       
  volatile uint32_t SR;        
  volatile uint32_t CLRFR;     
  volatile uint32_t DR;        
} SAI_Block_TypeDef;



 

typedef struct
{
  volatile uint32_t POWER;           
  volatile uint32_t CLKCR;           
  volatile uint32_t ARG;             
  volatile uint32_t CMD;             
  volatile const uint32_t  RESPCMD;         
  volatile const uint32_t  RESP1;           
  volatile const uint32_t  RESP2;           
  volatile const uint32_t  RESP3;           
  volatile const uint32_t  RESP4;           
  volatile uint32_t DTIMER;          
  volatile uint32_t DLEN;            
  volatile uint32_t DCTRL;           
  volatile const uint32_t  DCOUNT;          
  volatile const uint32_t  STA;             
  volatile uint32_t ICR;             
  volatile uint32_t MASK;            
  uint32_t      RESERVED0[2];    
  volatile const uint32_t  FIFOCNT;         
  uint32_t      RESERVED1[13];   
  volatile uint32_t FIFO;            
} SDIO_TypeDef;



 

typedef struct
{
  volatile uint16_t CR1;         
  uint16_t      RESERVED0;   
  volatile uint16_t CR2;         
  uint16_t      RESERVED1;   
  volatile uint16_t SR;          
  uint16_t      RESERVED2;   
  volatile uint16_t DR;          
  uint16_t      RESERVED3;   
  volatile uint16_t CRCPR;       
  uint16_t      RESERVED4;   
  volatile uint16_t RXCRCR;      
  uint16_t      RESERVED5;   
  volatile uint16_t TXCRCR;      
  uint16_t      RESERVED6;   
  volatile uint16_t I2SCFGR;     
  uint16_t      RESERVED7;   
  volatile uint16_t I2SPR;       
  uint16_t      RESERVED8;   
} SPI_TypeDef;



 

typedef struct
{
  volatile uint16_t CR1;          
  uint16_t      RESERVED0;    
  volatile uint16_t CR2;          
  uint16_t      RESERVED1;    
  volatile uint16_t SMCR;         
  uint16_t      RESERVED2;    
  volatile uint16_t DIER;         
  uint16_t      RESERVED3;    
  volatile uint16_t SR;           
  uint16_t      RESERVED4;    
  volatile uint16_t EGR;          
  uint16_t      RESERVED5;    
  volatile uint16_t CCMR1;        
  uint16_t      RESERVED6;    
  volatile uint16_t CCMR2;        
  uint16_t      RESERVED7;    
  volatile uint16_t CCER;         
  uint16_t      RESERVED8;    
  volatile uint32_t CNT;          
  volatile uint16_t PSC;          
  uint16_t      RESERVED9;    
  volatile uint32_t ARR;          
  volatile uint16_t RCR;          
  uint16_t      RESERVED10;   
  volatile uint32_t CCR1;         
  volatile uint32_t CCR2;         
  volatile uint32_t CCR3;         
  volatile uint32_t CCR4;         
  volatile uint16_t BDTR;         
  uint16_t      RESERVED11;   
  volatile uint16_t DCR;          
  uint16_t      RESERVED12;   
  volatile uint16_t DMAR;         
  uint16_t      RESERVED13;   
  volatile uint16_t OR;           
  uint16_t      RESERVED14;   
} TIM_TypeDef;



 
 
typedef struct
{
  volatile uint16_t SR;          
  uint16_t      RESERVED0;   
  volatile uint16_t DR;          
  uint16_t      RESERVED1;   
  volatile uint16_t BRR;         
  uint16_t      RESERVED2;   
  volatile uint16_t CR1;         
  uint16_t      RESERVED3;   
  volatile uint16_t CR2;         
  uint16_t      RESERVED4;   
  volatile uint16_t CR3;         
  uint16_t      RESERVED5;   
  volatile uint16_t GTPR;        
  uint16_t      RESERVED6;   
} USART_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;    
  volatile uint32_t CFR;   
  volatile uint32_t SR;    
} WWDG_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;          
  volatile uint32_t SR;          
  volatile uint32_t DR;          
  volatile uint32_t DOUT;        
  volatile uint32_t DMACR;       
  volatile uint32_t IMSCR;       
  volatile uint32_t RISR;        
  volatile uint32_t MISR;        
  volatile uint32_t K0LR;        
  volatile uint32_t K0RR;        
  volatile uint32_t K1LR;        
  volatile uint32_t K1RR;        
  volatile uint32_t K2LR;        
  volatile uint32_t K2RR;        
  volatile uint32_t K3LR;        
  volatile uint32_t K3RR;        
  volatile uint32_t IV0LR;       
  volatile uint32_t IV0RR;       
  volatile uint32_t IV1LR;       
  volatile uint32_t IV1RR;       
  volatile uint32_t CSGCMCCM0R;  
  volatile uint32_t CSGCMCCM1R;  
  volatile uint32_t CSGCMCCM2R;  
  volatile uint32_t CSGCMCCM3R;  
  volatile uint32_t CSGCMCCM4R;  
  volatile uint32_t CSGCMCCM5R;  
  volatile uint32_t CSGCMCCM6R;  
  volatile uint32_t CSGCMCCM7R;  
  volatile uint32_t CSGCM0R;     
  volatile uint32_t CSGCM1R;     
  volatile uint32_t CSGCM2R;     
  volatile uint32_t CSGCM3R;     
  volatile uint32_t CSGCM4R;     
  volatile uint32_t CSGCM5R;     
  volatile uint32_t CSGCM6R;     
  volatile uint32_t CSGCM7R;     
} CRYP_TypeDef;



 
  
typedef struct 
{
  volatile uint32_t CR;                
  volatile uint32_t DIN;               
  volatile uint32_t STR;               
  volatile uint32_t HR[5];             
  volatile uint32_t IMR;               
  volatile uint32_t SR;                
       uint32_t RESERVED[52];      
  volatile uint32_t CSR[54];           
} HASH_TypeDef;



 
  
typedef struct 
{
  volatile uint32_t HR[8];       
} HASH_DIGEST_TypeDef;



 
  
typedef struct 
{
  volatile uint32_t CR;   
  volatile uint32_t SR;   
  volatile uint32_t DR;   
} RNG_TypeDef;



 
  


 
# 1441 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"









# 1456 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




 





 
# 1498 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 1524 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 1564 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 







 
# 1580 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 1590 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




 
  


   
# 1693 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 1701 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 1710 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"





 



 
  
  

 
    
 
 
 

 
 
 
 
 
 
# 1741 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 1767 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
  
 
# 1793 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 1831 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 1873 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 


 


 


 


 
# 1922 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 1960 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 1998 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2027 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 


 


 



 
# 2063 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2085 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 



 
 
 
 
 
 
 
# 2106 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2117 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2135 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"











 





 





 
# 2173 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 












 
# 2194 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
 






 




 





 





 






 




 





 





 






   




 





 





 





 




 





 





 





 




 





 





 
 


 
# 2334 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2351 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2368 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2385 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2419 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2453 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2487 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2521 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2555 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2589 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2623 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2657 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2691 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2725 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2759 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2793 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2827 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2861 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2895 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2929 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2963 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 2997 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3031 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3065 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3099 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3133 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3167 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3201 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3235 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3269 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3303 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3337 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
 
 
 
 
 



 



 


 
 
 
 
 
 


# 3374 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 3383 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"






 





 


 


 


 



 
 
 
 
 
 









































 



 


 


 


 


 


 


 



 



 



 


 


 



 
 
 
 
 

 
 
 
 
 
 
# 3525 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




 






 






 






 






 
 
 
 
 
  
# 3600 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3619 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

  
# 3630 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

  
# 3652 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

  
# 3674 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

  
# 3696 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

  
# 3718 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
 
 
 
 

 

# 3737 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 

# 3746 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 

# 3755 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 



 



 



 



 

# 3780 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 





 

# 3795 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 





 



 



 



 

 






 




 





 





 



 



 




 



 






 
                                                                     
 


 
 
 
 
 
 
# 3895 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3917 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3939 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3961 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 3983 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4005 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
 
 
 
 
 
# 4029 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4037 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4046 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4065 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4073 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4099 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



                                             
 
# 4117 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"


 
 
 
 
 
 











# 4146 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 











# 4169 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 











# 4192 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 











# 4215 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 








































 








































 








































 








































 


































 


































 


































 


































 



























 



























 



























 
# 4612 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4621 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4630 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4641 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4651 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4661 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4671 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4682 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4692 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4702 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4712 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4723 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4733 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4743 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4753 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4764 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4774 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4784 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4794 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4805 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4815 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4825 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4835 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4846 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4856 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4866 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4876 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 4887 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4897 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4907 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 4917 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 



# 5943 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
 
 
 
 
 
































































 
# 6031 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
































































 
































































 
# 6179 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
 
# 6196 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6214 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
 
# 6231 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6265 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
 
 
 
 
 
# 6289 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6298 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 



 





 
 
 
 
 
 
# 6329 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6338 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"







 



# 6359 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



 



 


 
# 6384 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6394 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




 


 



 
 
 
 
 
 


 





 


 



 
 
 
 
 

 




 




 




 




 

# 6465 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




 





 






 






 






 



 




 






 





 




 




 





 



 



 





                                
 




 



 




 



 






 
 
 
 
 
 











 
# 6602 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"


















 


 
# 6634 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 
 
 
 
 
 



# 6653 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 6663 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 6674 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6683 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 6694 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"















 
 








 








 






# 6744 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 











 











 
# 6776 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




















 
# 6822 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6841 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 



  




 







 
# 6885 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6903 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 
# 6931 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 






 









 
# 6975 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 6995 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7023 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 






 








 
# 7066 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7086 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 











 
# 7110 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 





 




 




 
# 7134 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"


 
 
 
 
 
 



 






 
 
 
 
 
 
# 7185 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7215 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7243 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7260 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 



 


 



 
# 7313 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7355 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 



 
# 7394 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7414 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 
# 7432 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7452 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7460 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7468 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 


 


 


 


 


 


 


 


 


 


 


 


 


 


 


 


 


 


 
 
 
 
 
 








 
































 









# 7593 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"







 
# 7610 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 7619 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"





 
# 7631 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
                                     












 
# 7652 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7661 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"






 
# 7675 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 
 
 
 
 
 




 












 


 






# 7719 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 


 


 


 


 


 


 
















 


 
# 7789 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7804 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7830 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 
 
 
 
 
 









# 7862 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7870 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 7880 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 


 


 





















 




 
 
 
 
 
   












 






 


 






  
# 7967 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 7982 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 7997 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8012 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 






  
# 8032 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8047 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8062 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8077 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




           


  
# 8097 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8111 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8125 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8139 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 






  
# 8158 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8172 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8186 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



  
# 8200 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

   



 
 
 
 
 
 
















 









# 8245 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 

























 
# 8288 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 8302 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 8312 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




























 





















 




























 





















 
# 8431 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 


 


 


 


 


 


 
# 8466 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"





# 8477 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 8485 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 8492 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 
# 8503 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"


 
 
 
 
 
 
# 8521 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 



 
# 8545 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 8554 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"







 
# 8574 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 8585 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



 
 
 
 
 
 
# 8602 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



 
# 8614 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"







 



 
 
 
 
 
 



 









 
# 8662 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
 


 






 
 
 
 
 
 
# 8706 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 8722 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 
# 8740 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"
  
 


 
# 8756 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 



  


 








 

  
# 8783 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 






 



 


 


 
# 8812 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 
# 8827 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 
# 8842 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 
 
 

 
# 8857 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 




 




 




 




 


 


 


 


 


 


 
 
 

 
# 8910 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

# 8917 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 



 


 



 


 


 


 



 
 
 

 
# 8992 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 


 


 


 


 




   
# 9043 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 9069 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 
# 9086 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"

 





 


 


 


 




 

 

  

# 1 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"

























 

 



 
 
# 1 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"


























 

 







 
# 1 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"










































  



 



 
    
# 9144 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"



 

  

 

 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"



 



  

 

# 61 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"



    
typedef enum
{ 
  GPIO_Mode_IN   = 0x00,  
  GPIO_Mode_OUT  = 0x01,  
  GPIO_Mode_AF   = 0x02,  
  GPIO_Mode_AN   = 0x03   
}GPIOMode_TypeDef;





   
typedef enum
{ 
  GPIO_OType_PP = 0x00,
  GPIO_OType_OD = 0x01
}GPIOOType_TypeDef;





   
typedef enum
{ 
  GPIO_Low_Speed     = 0x00,  
  GPIO_Medium_Speed  = 0x01,  
  GPIO_Fast_Speed    = 0x02,  
  GPIO_High_Speed    = 0x03   
}GPIOSpeed_TypeDef;

 




  





  
typedef enum
{ 
  GPIO_PuPd_NOPULL = 0x00,
  GPIO_PuPd_UP     = 0x01,
  GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;





  
typedef enum
{ 
  Bit_RESET = 0,
  Bit_SET
}BitAction;





  
typedef struct
{
  uint32_t GPIO_Pin;              
 

  GPIOMode_TypeDef GPIO_Mode;     
 

  GPIOSpeed_TypeDef GPIO_Speed;   
 

  GPIOOType_TypeDef GPIO_OType;   
 

  GPIOPuPd_TypeDef GPIO_PuPd;     
 
}GPIO_InitTypeDef;

 



  



  
# 176 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"

# 195 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"


  




  
# 219 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"

# 236 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"


  



  


  








  





  






  







  






  
# 289 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"



  








  







  




  








  











  





  




  













  




 





  


# 394 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"

# 411 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"





# 440 "..\\stm32LIB\\inc\\stm32f4xx_gpio.h"
                          


  



 
    








 



 

 
 

 
void GPIO_DeInit(GPIO_TypeDef* GPIOx);

 
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

 
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

 
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF);









  



  

 
# 35 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


























 

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"



 



  

 




 

typedef struct
{
  uint16_t TIM_Prescaler;         
 

  uint16_t TIM_CounterMode;       
 

  uint32_t TIM_Period;            

  

  uint16_t TIM_ClockDivision;     
 

  uint8_t TIM_RepetitionCounter;  






 
} TIM_TimeBaseInitTypeDef; 



 

typedef struct
{
  uint16_t TIM_OCMode;        
 

  uint16_t TIM_OutputState;   
 

  uint16_t TIM_OutputNState;  

 

  uint32_t TIM_Pulse;         
 

  uint16_t TIM_OCPolarity;    
 

  uint16_t TIM_OCNPolarity;   

 

  uint16_t TIM_OCIdleState;   

 

  uint16_t TIM_OCNIdleState;  

 
} TIM_OCInitTypeDef;



 

typedef struct
{

  uint16_t TIM_Channel;      
 

  uint16_t TIM_ICPolarity;   
 

  uint16_t TIM_ICSelection;  
 

  uint16_t TIM_ICPrescaler;  
 

  uint16_t TIM_ICFilter;     
 
} TIM_ICInitTypeDef;




 

typedef struct
{

  uint16_t TIM_OSSRState;        
 

  uint16_t TIM_OSSIState;        
 

  uint16_t TIM_LOCKLevel;        
  

  uint16_t TIM_DeadTime;         

 

  uint16_t TIM_Break;            
 

  uint16_t TIM_BreakPolarity;    
 

  uint16_t TIM_AutomaticOutput;  
 
} TIM_BDTRInitTypeDef;

 



 

# 189 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"
                                          
# 202 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"
                                     
 
# 212 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"
 
# 219 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"
 


 
# 231 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"
                                






 

# 260 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


 



 







  



 





                                 




                                 







  



 

# 309 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


 



 

# 327 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 







 



 
  






 



 







  



 







  



 







  



 







  



 







  



 







  



 







  



 

# 451 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 







 



 







  



 







  



 







  



 

# 513 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 

# 529 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 

# 545 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 

# 562 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"

# 571 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 

# 619 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 

# 663 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 

# 679 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"



  



 

# 696 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 

# 724 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 







  



  






 



 







  



 







  



 

# 785 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  




 

# 803 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"
  


  



 

# 818 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 







  



 





                                     


  



 







  



 

# 879 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 

# 895 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


  



 







  


 














# 937 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"



  


 

# 969 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"



  



 




  



 




  



 

# 1014 "..\\stm32LIB\\inc\\stm32f4xx_tim.h"


 



 

 
  

 
void TIM_DeInit(TIM_TypeDef* TIMx);
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode);
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode);
void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter);
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload);
uint32_t TIM_GetCounter(TIM_TypeDef* TIMx);
uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx);
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, uint16_t TIM_UpdateSource);
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint16_t TIM_OPMode);
void TIM_SetClockDivision(TIM_TypeDef* TIMx, uint16_t TIM_CKD);
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);

 
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode);
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2);
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint32_t Compare3);
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint32_t Compare4);
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast);
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast);
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast);
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast);
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear);
void TIM_ClearOC2Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear);
void TIM_ClearOC3Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear);
void TIM_ClearOC4Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear);
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity);
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity);
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity);
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx);
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN);

 
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct);
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);
uint32_t TIM_GetCapture1(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture2(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture3(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture4(TIM_TypeDef* TIMx);
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);

 
void TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct);
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef* TIM_BDTRInitStruct);
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_CCPreloadControl(TIM_TypeDef* TIMx, FunctionalState NewState);

 
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState);
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource);
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT);
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT);
void TIM_DMAConfig(TIM_TypeDef* TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength);
void TIM_DMACmd(TIM_TypeDef* TIMx, uint16_t TIM_DMASource, FunctionalState NewState);
void TIM_SelectCCDMA(TIM_TypeDef* TIMx, FunctionalState NewState);

 
void TIM_InternalClockConfig(TIM_TypeDef* TIMx);
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource);
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,
                                uint16_t TIM_ICPolarity, uint16_t ICFilter);
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter);
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, 
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter);

 
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource);
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource);
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode);
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_MasterSlaveMode);
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter);

    
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity);
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState);

 
void TIM_RemapConfig(TIM_TypeDef* TIMx, uint16_t TIM_Remap);









  



 

 
# 36 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"

























 

 







 
# 38 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"



 



  

 
typedef struct
{
  uint32_t SYSCLK_Frequency;  
  uint32_t HCLK_Frequency;    
  uint32_t PCLK1_Frequency;   
  uint32_t PCLK2_Frequency;   
}RCC_ClocksTypeDef;

 



 
  


 







  
  


 
# 85 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"
 











# 105 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"
 


  
  


 
# 119 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"


  
  


 
# 140 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"


  
  


 
# 155 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"


  
  


 
# 170 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"

# 177 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"



  
  


 







  
  


 
# 261 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"


  
  


 






  



 









  



 









  



 






 
  


  
# 345 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"







  
  


   
# 363 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"


  
  


  











  
  


  
# 412 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"


  
  


  
# 438 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"






  
  


 
# 460 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"
                                   





  
  


 
# 482 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"
                                   





  
  


 
# 507 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"

# 515 "..\\stm32LIB\\inc\\stm32f4xx_rcc.h"




  



  

 
  

 
void RCC_DeInit(void);

 
void        RCC_HSEConfig(uint8_t RCC_HSE);
ErrorStatus RCC_WaitForHSEStartUp(void);
void        RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
void        RCC_HSICmd(FunctionalState NewState);
void        RCC_LSEConfig(uint8_t RCC_LSE);
void        RCC_LSICmd(FunctionalState NewState);
void        RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ);
void        RCC_PLLCmd(FunctionalState NewState);


void        RCC_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SR);






void        RCC_PLLI2SCmd(FunctionalState NewState);
void        RCC_PLLSAIConfig(uint32_t PLLSAIN, uint32_t PLLSAIQ, uint32_t PLLSAIR);
void        RCC_PLLSAICmd(FunctionalState NewState);
void        RCC_ClockSecuritySystemCmd(FunctionalState NewState);
void        RCC_MCO1Config(uint32_t RCC_MCO1Source, uint32_t RCC_MCO1Div);
void        RCC_MCO2Config(uint32_t RCC_MCO2Source, uint32_t RCC_MCO2Div);

 
void        RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);
uint8_t     RCC_GetSYSCLKSource(void);
void        RCC_HCLKConfig(uint32_t RCC_SYSCLK);
void        RCC_PCLK1Config(uint32_t RCC_HCLK);
void        RCC_PCLK2Config(uint32_t RCC_HCLK);
void        RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

 
void        RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource);
void        RCC_RTCCLKCmd(FunctionalState NewState);
void        RCC_BackupResetCmd(FunctionalState NewState);
void        RCC_I2SCLKConfig(uint32_t RCC_I2SCLKSource); 
void        RCC_SAIPLLI2SClkDivConfig(uint32_t RCC_PLLI2SDivQ);
void        RCC_SAIPLLSAIClkDivConfig(uint32_t RCC_PLLSAIDivQ);
void        RCC_SAIBlockACLKConfig(uint32_t RCC_SAIBlockACLKSource);
void        RCC_SAIBlockBCLKConfig(uint32_t RCC_SAIBlockBCLKSource);
void        RCC_LTDCCLKDivConfig(uint32_t RCC_PLLSAIDivR);
void        RCC_TIMCLKPresConfig(uint32_t RCC_TIMCLKPrescaler);

void        RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void        RCC_AHB2PeriphClockCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void        RCC_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void        RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void        RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

void        RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void        RCC_AHB2PeriphResetCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void        RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void        RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void        RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

void        RCC_AHB1PeriphClockLPModeCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void        RCC_AHB2PeriphClockLPModeCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void        RCC_AHB3PeriphClockLPModeCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void        RCC_APB1PeriphClockLPModeCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void        RCC_APB2PeriphClockLPModeCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

 
void        RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState);
FlagStatus  RCC_GetFlagStatus(uint8_t RCC_FLAG);
void        RCC_ClearFlag(void);
ITStatus    RCC_GetITStatus(uint8_t RCC_IT);
void        RCC_ClearITPendingBit(uint8_t RCC_IT);









  



  

 
# 37 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


























 

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"



 



  

 



  
typedef struct
{
  uint32_t ADC_Resolution;                
                                    
  FunctionalState ADC_ScanConvMode;       


  
  FunctionalState ADC_ContinuousConvMode; 

 
  uint32_t ADC_ExternalTrigConvEdge;      


 
  uint32_t ADC_ExternalTrigConv;          


 
  uint32_t ADC_DataAlign;                 

 
  uint8_t  ADC_NbrOfConversion;           


 
}ADC_InitTypeDef;
  


  
typedef struct 
{
  uint32_t ADC_Mode;                      

                                               
  uint32_t ADC_Prescaler;                 

 
  uint32_t ADC_DMAAccessMode;             


 
  uint32_t ADC_TwoSamplingDelay;          

 
  
}ADC_CommonInitTypeDef;


 



  






  
# 141 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 157 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 173 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"
                                     


  




  
# 214 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"
                                     


  




  
# 231 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"
                                      


  




  
# 248 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 288 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  






  




  
# 327 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"












# 358 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 382 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 398 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"
                                            


  




  
# 439 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 455 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 477 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 491 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  
# 505 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"
  
# 513 "..\\stm32LIB\\inc\\stm32f4xx_adc.h"


  




  



  




  



  




  



  




  



  




  



  




  



  




  



  




  

 
   

   
void ADC_DeInit(void);

 
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_CommonInit(ADC_CommonInitTypeDef* ADC_CommonInitStruct);
void ADC_CommonStructInit(ADC_CommonInitTypeDef* ADC_CommonInitStruct);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);

 
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog);
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold,uint16_t LowThreshold);
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel);

 
void ADC_TempSensorVrefintCmd(FunctionalState NewState);
void ADC_VBATCmd(FunctionalState NewState);

 
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_SoftwareStartConv(ADC_TypeDef* ADCx);
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx);
void ADC_EOCOnEachRegularChannelCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ContinuousModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number);
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
uint32_t ADC_GetMultiModeConversionValue(void);

 
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_DMARequestAfterLastTransferCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_MultiModeDMARequestAfterLastTransferCmd(FunctionalState NewState);

 
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length);
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv);
void ADC_ExternalTrigInjectedConvEdgeConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConvEdge);
void ADC_SoftwareStartInjectedConv(ADC_TypeDef* ADCx);
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx);
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel);

 
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT);
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT);









  



  

 
# 38 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_can.h"


























 

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_can.h"



 



 

 






 
typedef struct
{
  uint16_t CAN_Prescaler;   
 
  
  uint8_t CAN_Mode;         
 

  uint8_t CAN_SJW;          


 

  uint8_t CAN_BS1;          

 

  uint8_t CAN_BS2;          
 
  
  FunctionalState CAN_TTCM; 
 
  
  FunctionalState CAN_ABOM;  
 

  FunctionalState CAN_AWUM;  
 

  FunctionalState CAN_NART;  
 

  FunctionalState CAN_RFLM;  
 

  FunctionalState CAN_TXFP;  
 
} CAN_InitTypeDef;



 
typedef struct
{
  uint16_t CAN_FilterIdHigh;         

 

  uint16_t CAN_FilterIdLow;          

 

  uint16_t CAN_FilterMaskIdHigh;     


 

  uint16_t CAN_FilterMaskIdLow;      


 

  uint16_t CAN_FilterFIFOAssignment; 
 
  
  uint8_t CAN_FilterNumber;           

  uint8_t CAN_FilterMode;            
 

  uint8_t CAN_FilterScale;           
 

  FunctionalState CAN_FilterActivation; 
 
} CAN_FilterInitTypeDef;



 
typedef struct
{
  uint32_t StdId;  
 

  uint32_t ExtId;  
 

  uint8_t IDE;     

 

  uint8_t RTR;     

 

  uint8_t DLC;     

 

  uint8_t Data[8]; 
 
} CanTxMsg;



 
typedef struct
{
  uint32_t StdId;  
 

  uint32_t ExtId;  
 

  uint8_t IDE;     

 

  uint8_t RTR;     

 

  uint8_t DLC;     
 

  uint8_t Data[8]; 
 

  uint8_t FMI;     

 
} CanRxMsg;

 



 



 





 




 



 












 


 


   










 
  



   





 



 









 



 
# 289 "..\\stm32LIB\\inc\\stm32f4xx_can.h"




 



 
# 306 "..\\stm32LIB\\inc\\stm32f4xx_can.h"




 



 



 



 



 



 







 



 







 



 





 




 



 



 



 






 



 





 




 



 




 




 



 





 	






 



 






 



 



 	




 



 



 




 




                                                          
# 481 "..\\stm32LIB\\inc\\stm32f4xx_can.h"


 



 

 

 

 




 
# 505 "..\\stm32LIB\\inc\\stm32f4xx_can.h"

 



 

 





# 526 "..\\stm32LIB\\inc\\stm32f4xx_can.h"








 

  


  


 
# 549 "..\\stm32LIB\\inc\\stm32f4xx_can.h"

 



 






 





# 574 "..\\stm32LIB\\inc\\stm32f4xx_can.h"

# 581 "..\\stm32LIB\\inc\\stm32f4xx_can.h"


 



 

 
   

  
void CAN_DeInit(CAN_TypeDef* CANx);

  
uint8_t CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);
void CAN_FilterInit(CAN_FilterInitTypeDef* CAN_FilterInitStruct);
void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct);
void CAN_SlaveStartBank(uint8_t CAN_BankNumber); 
void CAN_DBGFreeze(CAN_TypeDef* CANx, FunctionalState NewState);
void CAN_TTComModeCmd(CAN_TypeDef* CANx, FunctionalState NewState);

 
uint8_t CAN_Transmit(CAN_TypeDef* CANx, CanTxMsg* TxMessage);
uint8_t CAN_TransmitStatus(CAN_TypeDef* CANx, uint8_t TransmitMailbox);
void CAN_CancelTransmit(CAN_TypeDef* CANx, uint8_t Mailbox);

 
void CAN_Receive(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg* RxMessage);
void CAN_FIFORelease(CAN_TypeDef* CANx, uint8_t FIFONumber);
uint8_t CAN_MessagePending(CAN_TypeDef* CANx, uint8_t FIFONumber);

 
uint8_t CAN_OperatingModeRequest(CAN_TypeDef* CANx, uint8_t CAN_OperatingMode);
uint8_t CAN_Sleep(CAN_TypeDef* CANx);
uint8_t CAN_WakeUp(CAN_TypeDef* CANx);

 
uint8_t CAN_GetLastErrorCode(CAN_TypeDef* CANx);
uint8_t CAN_GetReceiveErrorCounter(CAN_TypeDef* CANx);
uint8_t CAN_GetLSBTransmitErrorCounter(CAN_TypeDef* CANx);

 
void CAN_ITConfig(CAN_TypeDef* CANx, uint32_t CAN_IT, FunctionalState NewState);
FlagStatus CAN_GetFlagStatus(CAN_TypeDef* CANx, uint32_t CAN_FLAG);
void CAN_ClearFlag(CAN_TypeDef* CANx, uint32_t CAN_FLAG);
ITStatus CAN_GetITStatus(CAN_TypeDef* CANx, uint32_t CAN_IT);
void CAN_ClearITPendingBit(CAN_TypeDef* CANx, uint32_t CAN_IT);









 



 

 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"


























 

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"



 



  

 


  
typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_RD,
  FLASH_ERROR_PGS,
  FLASH_ERROR_PGP,
  FLASH_ERROR_PGA,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_ERROR_OPERATION,
  FLASH_COMPLETE
}FLASH_Status;

 



   



  
# 90 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"


# 108 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"


  



  











  



 
# 155 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"

# 168 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"

















  



  
# 215 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"




 



 





 



  
# 259 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"




 



 


  
 





  



  





  



  





  




  





 
  


   
# 319 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"


 
  


 





 



  





  



  
# 359 "..\\stm32LIB\\inc\\stm32f4xx_flash.h"


 



 







  



  







  



  



  



  



  



  




  




  

 
  
 
 
void FLASH_SetLatency(uint32_t FLASH_Latency);
void FLASH_PrefetchBufferCmd(FunctionalState NewState);
void FLASH_InstructionCacheCmd(FunctionalState NewState);
void FLASH_DataCacheCmd(FunctionalState NewState);
void FLASH_InstructionCacheReset(void);
void FLASH_DataCacheReset(void);

    
void         FLASH_Unlock(void);
void         FLASH_Lock(void);
FLASH_Status FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange);
FLASH_Status FLASH_EraseAllSectors(uint8_t VoltageRange);
FLASH_Status FLASH_EraseAllBank1Sectors(uint8_t VoltageRange);
FLASH_Status FLASH_EraseAllBank2Sectors(uint8_t VoltageRange);
FLASH_Status FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);
FLASH_Status FLASH_ProgramByte(uint32_t Address, uint8_t Data);

  
void         FLASH_OB_Unlock(void);
void         FLASH_OB_Lock(void);
void         FLASH_OB_WRPConfig(uint32_t OB_WRP, FunctionalState NewState);
void         FLASH_OB_WRP1Config(uint32_t OB_WRP, FunctionalState NewState);
void         FLASH_OB_PCROPSelectionConfig(uint8_t OB_PcROP);
void         FLASH_OB_PCROPConfig(uint32_t OB_PCROP, FunctionalState NewState);
void         FLASH_OB_PCROP1Config(uint32_t OB_PCROP, FunctionalState NewState);
void         FLASH_OB_RDPConfig(uint8_t OB_RDP);
void         FLASH_OB_UserConfig(uint8_t OB_IWDG, uint8_t OB_STOP, uint8_t OB_STDBY);
void         FLASH_OB_BORConfig(uint8_t OB_BOR);
void         FLASH_OB_BootConfig(uint8_t OB_BOOT);
FLASH_Status FLASH_OB_Launch(void);
uint8_t      FLASH_OB_GetUser(void);
uint16_t     FLASH_OB_GetWRP(void);
uint16_t     FLASH_OB_GetWRP1(void);
uint16_t     FLASH_OB_GetPCROP(void);
uint16_t     FLASH_OB_GetPCROP1(void);
FlagStatus   FLASH_OB_GetRDP(void);
uint8_t      FLASH_OB_GetBOR(void);

 
void         FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
FlagStatus   FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void         FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetStatus(void);
FLASH_Status FLASH_WaitForLastOperation(void);









  



  

 
# 40 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"


























  

 
# 599 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"



 



 


 
# 41 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"


























  

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"



 



  

 



 

typedef struct
{
  uint16_t SPI_Direction;           
 

  uint16_t SPI_Mode;                
 

  uint16_t SPI_DataSize;            
 

  uint16_t SPI_CPOL;                
 

  uint16_t SPI_CPHA;                
 

  uint16_t SPI_NSS;                 

 
 
  uint16_t SPI_BaudRatePrescaler;   



 

  uint16_t SPI_FirstBit;            
 

  uint16_t SPI_CRCPolynomial;        
}SPI_InitTypeDef;



 

typedef struct
{

  uint16_t I2S_Mode;         
 

  uint16_t I2S_Standard;     
 

  uint16_t I2S_DataFormat;   
 

  uint16_t I2S_MCLKOutput;   
 

  uint32_t I2S_AudioFreq;    
 

  uint16_t I2S_CPOL;         
 
}I2S_InitTypeDef;

 



 

# 125 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"

# 134 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"















 
  
# 159 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"


 



 







 



 







  



 







 



 







 



 







  



 

# 243 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"


  



 







 



 

# 271 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"


 
  



 

# 290 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"


 
  


 

# 306 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"


 



 







 



 

# 336 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"






 
            


 







 



 






 



 







 



 






 



 







 



 























 



 

# 443 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"

# 450 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"


 



 




 



 

# 486 "..\\stm32LIB\\inc\\stm32f4xx_spi.h"


 
  


 

 
  

  
void SPI_I2S_DeInit(SPI_TypeDef* SPIx);

 
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct);
void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct);
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct);
void I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct);
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, uint16_t SPI_DataSize);
void SPI_BiDirectionalLineConfig(SPI_TypeDef* SPIx, uint16_t SPI_Direction);
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft);
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_TIModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState);

void I2S_FullDuplexConfig(SPI_TypeDef* I2Sxext, I2S_InitTypeDef* I2S_InitStruct);

  
void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data);
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx);

 
void SPI_CalculateCRC(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_TransmitCRC(SPI_TypeDef* SPIx);
uint16_t SPI_GetCRC(SPI_TypeDef* SPIx, uint8_t SPI_CRC);
uint16_t SPI_GetCRCPolynomial(SPI_TypeDef* SPIx);

 
void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionalState NewState);

 
void SPI_I2S_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT, FunctionalState NewState);
FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG);
void SPI_I2S_ClearFlag(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG);
ITStatus SPI_I2S_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT);
void SPI_I2S_ClearITPendingBit(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT);









 



 

 
# 42 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_exti.h"


























 

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_exti.h"



 



 

 



 

typedef enum
{
  EXTI_Mode_Interrupt = 0x00,
  EXTI_Mode_Event = 0x04
}EXTIMode_TypeDef;





 

typedef enum
{
  EXTI_Trigger_Rising = 0x08,
  EXTI_Trigger_Falling = 0x0C,  
  EXTI_Trigger_Rising_Falling = 0x10
}EXTITrigger_TypeDef;






 

typedef struct
{
  uint32_t EXTI_Line;               
 
   
  EXTIMode_TypeDef EXTI_Mode;       
 

  EXTITrigger_TypeDef EXTI_Trigger; 
 

  FunctionalState EXTI_LineCmd;     
  
}EXTI_InitTypeDef;

 



 



 

# 128 "..\\stm32LIB\\inc\\stm32f4xx_exti.h"
                                          


# 143 "..\\stm32LIB\\inc\\stm32f4xx_exti.h"
                    


 



 

 
 

 
void EXTI_DeInit(void);

 
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line);

 
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line);
void EXTI_ClearFlag(uint32_t EXTI_Line);
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);









 



 

 
# 43 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"


























  

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"



 



  

  



  
  
typedef struct
{
  uint32_t USART_BaudRate;            



 

  uint16_t USART_WordLength;          
 

  uint16_t USART_StopBits;            
 

  uint16_t USART_Parity;              




 
 
  uint16_t USART_Mode;                
 

  uint16_t USART_HardwareFlowControl; 

 
} USART_InitTypeDef;



  
  
typedef struct
{

  uint16_t USART_Clock;   
 

  uint16_t USART_CPOL;    
 

  uint16_t USART_CPHA;    
 

  uint16_t USART_LastBit; 

 
} USART_ClockInitTypeDef;

 



  
  
# 118 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"








  
  


                                    




  



  
  
# 149 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"


  



  
  
# 163 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"


  



  
  





  



  
# 190 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"


  



  






  



 
  






  



 







 



 







  



 
  
# 257 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"



 



 

# 278 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"


 



 







  



 







 



 
  







 



 







  



 

# 350 "..\\stm32LIB\\inc\\stm32f4xx_usart.h"
                              








  



  

 
   

  
void USART_DeInit(USART_TypeDef* USARTx);

 
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
void USART_StructInit(USART_InitTypeDef* USART_InitStruct);
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SetPrescaler(USART_TypeDef* USARTx, uint8_t USART_Prescaler);
void USART_OverSampling8Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_OneBitMethodCmd(USART_TypeDef* USARTx, FunctionalState NewState);

  
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
uint16_t USART_ReceiveData(USART_TypeDef* USARTx);

 
void USART_SetAddress(USART_TypeDef* USARTx, uint8_t USART_Address);
void USART_WakeUpConfig(USART_TypeDef* USARTx, uint16_t USART_WakeUp);
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState);

 
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength);
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SendBreak(USART_TypeDef* USARTx);

 
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState);

 
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SetGuardTime(USART_TypeDef* USARTx, uint8_t USART_GuardTime);

 
void USART_IrDAConfig(USART_TypeDef* USARTx, uint16_t USART_IrDAMode);
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState);

 
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState);

 
void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG);
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT);
void USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT);









  



  

 
# 44 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_syscfg.h"


























 

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_syscfg.h"



 



  

 
 
  


  



  
# 69 "..\\stm32LIB\\inc\\stm32f4xx_syscfg.h"

# 81 "..\\stm32LIB\\inc\\stm32f4xx_syscfg.h"
                                         


  




  
# 122 "..\\stm32LIB\\inc\\stm32f4xx_syscfg.h"


  




  













# 149 "..\\stm32LIB\\inc\\stm32f4xx_syscfg.h"







# 163 "..\\stm32LIB\\inc\\stm32f4xx_syscfg.h"
                                                                                              


  




  







  



  

 
  
 
void SYSCFG_DeInit(void);
void       SYSCFG_MemoryRemapConfig(uint8_t SYSCFG_MemoryRemap);
void       SYSCFG_MemorySwappingBank(FunctionalState NewState);
void       SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex);
void       SYSCFG_ETH_MediaInterfaceConfig(uint32_t SYSCFG_ETH_MediaInterface); 
void       SYSCFG_CompensationCellCmd(FunctionalState NewState); 
FlagStatus SYSCFG_GetCompensationCellStatus(void);









  



  

 
# 45 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\stm32f4xx_iwdg.h"


























 

 







 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_iwdg.h"



 



 

 
 



 
  


 






 



 
# 83 "..\\stm32LIB\\inc\\stm32f4xx_iwdg.h"


 



 






 



 

 
 

 
void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess);
void IWDG_SetPrescaler(uint8_t IWDG_Prescaler);
void IWDG_SetReload(uint16_t Reload);
void IWDG_ReloadCounter(void);

 
void IWDG_Enable(void);

 
FlagStatus IWDG_GetFlagStatus(uint16_t IWDG_FLAG);









 



 

 
# 46 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"
# 1 "..\\stm32LIB\\inc\\misc.h"


























 

 







 
# 39 "..\\stm32LIB\\inc\\misc.h"



 



 

 



 

typedef struct
{
  uint8_t NVIC_IRQChannel;                    


 

  uint8_t NVIC_IRQChannelPreemptionPriority;  


 

  uint8_t NVIC_IRQChannelSubPriority;         


 

  FunctionalState NVIC_IRQChannelCmd;         

    
} NVIC_InitTypeDef;
 
 



 



 







 



 

# 104 "..\\stm32LIB\\inc\\misc.h"


 



 

# 122 "..\\stm32LIB\\inc\\misc.h"















 



 







 



 

 
 

void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState);
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);









 



 

 
# 47 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"

# 60 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"

# 72 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"





 
 



 
   



 
 

 
# 107 "..\\stm32LIB\\inc\\stm32f4xx_conf.h"



 
# 9115 "..\\stm32LIB\\CMSIS\\stm32f4xx.h"




 

















 









 

  

 

 
# 39 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"



 



 

 



 

typedef struct
{
  uint32_t DMA_Channel;            
 
 
  uint32_t DMA_PeripheralBaseAddr;  

  uint32_t DMA_Memory0BaseAddr;    

 

  uint32_t DMA_DIR;                

 

  uint32_t DMA_BufferSize;         

 

  uint32_t DMA_PeripheralInc;      
 

  uint32_t DMA_MemoryInc;          
 

  uint32_t DMA_PeripheralDataSize; 
 

  uint32_t DMA_MemoryDataSize;     
 

  uint32_t DMA_Mode;               


 

  uint32_t DMA_Priority;           
 

  uint32_t DMA_FIFOMode;          


 

  uint32_t DMA_FIFOThreshold;      
 

  uint32_t DMA_MemoryBurst;        


 

  uint32_t DMA_PeripheralBurst;    


   
}DMA_InitTypeDef;

 



 

# 134 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"






  
# 149 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"

# 158 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"


  




  









  




  



  




  







  




  







  




  









  




  









  




  







  




  











  




  







  




  











  




  











  




  











  




 
# 346 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"

# 353 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"


  



 
# 400 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"




# 424 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"


  




  









  




  
# 487 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"





# 512 "..\\stm32LIB\\inc\\stm32f4xx_dma.h"


  




  







  




  







  




  






  



  

 
  

  
void DMA_DeInit(DMA_Stream_TypeDef* DMAy_Streamx);

 
void DMA_Init(DMA_Stream_TypeDef* DMAy_Streamx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
void DMA_Cmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState);

 
void DMA_PeriphIncOffsetSizeConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_Pincos);
void DMA_FlowControllerConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FlowCtrl);

 
void DMA_SetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx, uint16_t Counter);
uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx);

 
void DMA_DoubleBufferModeConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t Memory1BaseAddr,
                                uint32_t DMA_CurrentMemory);
void DMA_DoubleBufferModeCmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState);
void DMA_MemoryTargetConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t MemoryBaseAddr,
                            uint32_t DMA_MemoryTarget);
uint32_t DMA_GetCurrentMemoryTarget(DMA_Stream_TypeDef* DMAy_Streamx);

 
FunctionalState DMA_GetCmdStatus(DMA_Stream_TypeDef* DMAy_Streamx);
uint32_t DMA_GetFIFOStatus(DMA_Stream_TypeDef* DMAy_Streamx);
FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG);
void DMA_ClearFlag(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG);
void DMA_ITConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT, FunctionalState NewState);
ITStatus DMA_GetITStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT);
void DMA_ClearITPendingBit(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT);









 



 


 
# 125 "..\\stm32LIB\\src\\stm32f4xx_dma.c"
# 126 "..\\stm32LIB\\src\\stm32f4xx_dma.c"



 




  

 
 

 







# 157 "..\\stm32LIB\\src\\stm32f4xx_dma.c"

 
 
 
 




 





















 






 
void DMA_DeInit(DMA_Stream_TypeDef* DMAy_Streamx)
{
   
  ((void)0);

   
  DMAy_Streamx->CR &= ~((uint32_t)((uint32_t)0x00000001));

   
  DMAy_Streamx->CR  = 0;
  
   
  DMAy_Streamx->NDTR = 0;
  
   
  DMAy_Streamx->PAR  = 0;
  
   
  DMAy_Streamx->M0AR = 0;

   
  DMAy_Streamx->M1AR = 0;

   
  DMAy_Streamx->FCR = (uint32_t)0x00000021; 

   
  if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6000) + 0x010)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000))->LIFCR = (uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020));
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6000) + 0x028)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000))->LIFCR = (uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 6);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6000) + 0x040)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000))->LIFCR = (uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 16);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6000) + 0x058)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000))->LIFCR = (uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 22);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6000) + 0x070)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000))->HIFCR = (uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) | (uint32_t)0x20000000);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6000) + 0x088)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000))->HIFCR = (uint32_t)((uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 6) | (uint32_t)0x20000000);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6000) + 0x0A0)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000))->HIFCR = (uint32_t)(uint32_t)((uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 16) | (uint32_t)0x20000000);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6000) + 0x0B8)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000))->HIFCR = (uint32_t)((uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 22) | (uint32_t)0x20000000);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x010)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400))->LIFCR = (uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020));
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x028)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400))->LIFCR = (uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 6);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x040)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400))->LIFCR = (uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 16);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x058)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400))->LIFCR = (uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 22);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x070)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400))->HIFCR = (uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) | (uint32_t)0x20000000);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x088)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400))->HIFCR = (uint32_t)((uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 6) | (uint32_t)0x20000000);
  }
  else if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x0A0)))
  {
     
    ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400))->HIFCR = (uint32_t)((uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 16) | (uint32_t)0x20000000);
  }
  else 
  {
    if (DMAy_Streamx == ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x0B8)))
    {
       
      ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400))->HIFCR = (uint32_t)((uint32_t)((uint32_t)(((uint32_t)0x00000001) | ((uint32_t)0x00000004) | ((uint32_t)0x00000008) | ((uint32_t)0x00000010) | ((uint32_t)0x00000020)) << 22) | (uint32_t)0x20000000);
    }
  }
}











 
void DMA_Init(DMA_Stream_TypeDef* DMAy_Streamx, DMA_InitTypeDef* DMA_InitStruct)
{
  uint32_t tmpreg = 0;

   
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);
  ((void)0);

   
   
  tmpreg = DMAy_Streamx->CR;

   
  tmpreg &= ((uint32_t)~(((uint32_t)0x0E000000) | ((uint32_t)0x01800000) | ((uint32_t)0x00600000) |                          ((uint32_t)0x00030000) | ((uint32_t)0x00006000) | ((uint32_t)0x00001800) |                          ((uint32_t)0x00000400) | ((uint32_t)0x00000200) | ((uint32_t)0x00000100) |                          ((uint32_t)0x000000C0)));




   
   
   
   
   
   
   
   
   
   
   
  tmpreg |= DMA_InitStruct->DMA_Channel | DMA_InitStruct->DMA_DIR |
            DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
            DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
            DMA_InitStruct->DMA_Mode | DMA_InitStruct->DMA_Priority |
            DMA_InitStruct->DMA_MemoryBurst | DMA_InitStruct->DMA_PeripheralBurst;

   
  DMAy_Streamx->CR = tmpreg;

   
   
  tmpreg = DMAy_Streamx->FCR;

   
  tmpreg &= (uint32_t)~(((uint32_t)0x00000004) | ((uint32_t)0x00000003));

  

 
  tmpreg |= DMA_InitStruct->DMA_FIFOMode | DMA_InitStruct->DMA_FIFOThreshold;

   
  DMAy_Streamx->FCR = tmpreg;

   
   
  DMAy_Streamx->NDTR = DMA_InitStruct->DMA_BufferSize;

   
   
  DMAy_Streamx->PAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

   
   
  DMAy_Streamx->M0AR = DMA_InitStruct->DMA_Memory0BaseAddr;
}






 
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
   
   
  DMA_InitStruct->DMA_Channel = 0;

   
  DMA_InitStruct->DMA_PeripheralBaseAddr = 0;

   
  DMA_InitStruct->DMA_Memory0BaseAddr = 0;

   
  DMA_InitStruct->DMA_DIR = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_BufferSize = 0;

   
  DMA_InitStruct->DMA_PeripheralInc = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_MemoryInc = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_PeripheralDataSize = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_MemoryDataSize = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_Mode = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_Priority = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_FIFOMode = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_FIFOThreshold = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_MemoryBurst = ((uint32_t)0x00000000);

   
  DMA_InitStruct->DMA_PeripheralBurst = ((uint32_t)0x00000000);
}


























 
void DMA_Cmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState)
{
   
  ((void)0);
  ((void)0);

  if (NewState != DISABLE)
  {
     
    DMAy_Streamx->CR |= (uint32_t)((uint32_t)0x00000001);
  }
  else
  {
     
    DMAy_Streamx->CR &= ~(uint32_t)((uint32_t)0x00000001);
  }
}


















 
void DMA_PeriphIncOffsetSizeConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_Pincos)
{
   
  ((void)0);
  ((void)0);

   
  if(DMA_Pincos != ((uint32_t)0x00000000))
  {
     
    DMAy_Streamx->CR |= (uint32_t)((uint32_t)0x00008000);     
  }
  else
  {
     
    DMAy_Streamx->CR &= ~(uint32_t)((uint32_t)0x00008000);    
  }
}

















 
void DMA_FlowControllerConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FlowCtrl)
{
   
  ((void)0);
  ((void)0);

   
  if(DMA_FlowCtrl != ((uint32_t)0x00000000))
  {
     
    DMAy_Streamx->CR |= (uint32_t)((uint32_t)0x00000020);   
  }
  else
  {
     
    DMAy_Streamx->CR &= ~(uint32_t)((uint32_t)0x00000020);    
  }
}


 






































 





















 
void DMA_SetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx, uint16_t Counter)
{
   
  ((void)0);

   
  DMAy_Streamx->NDTR = (uint16_t)Counter;
}






 
uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx)
{
   
  ((void)0);

   
  return ((uint16_t)(DMAy_Streamx->NDTR));
}


 






















































 
  















 
void DMA_DoubleBufferModeConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t Memory1BaseAddr,
                                uint32_t DMA_CurrentMemory)
{  
   
  ((void)0);
  ((void)0);

  if (DMA_CurrentMemory != ((uint32_t)0x00000000))
  {
     
    DMAy_Streamx->CR |= (uint32_t)(((uint32_t)0x00080000));    
  }
  else
  {
     
    DMAy_Streamx->CR &= ~(uint32_t)(((uint32_t)0x00080000));    
  }

   
  DMAy_Streamx->M1AR = Memory1BaseAddr;
}









 
void DMA_DoubleBufferModeCmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState)
{  
   
  ((void)0);
  ((void)0);

   
  if (NewState != DISABLE)
  {
     
    DMAy_Streamx->CR |= (uint32_t)((uint32_t)0x00040000);
  }
  else
  {
     
    DMAy_Streamx->CR &= ~(uint32_t)((uint32_t)0x00040000);
  }
}






















 
void DMA_MemoryTargetConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t MemoryBaseAddr,
                           uint32_t DMA_MemoryTarget)
{
   
  ((void)0);
  ((void)0);
    
   
  if (DMA_MemoryTarget != ((uint32_t)0x00000000))
  {
     
    DMAy_Streamx->M1AR = MemoryBaseAddr;    
  }  
  else
  {
     
    DMAy_Streamx->M0AR = MemoryBaseAddr;  
  }
}






 
uint32_t DMA_GetCurrentMemoryTarget(DMA_Stream_TypeDef* DMAy_Streamx)
{
  uint32_t tmp = 0;
  
   
  ((void)0);

   
  if ((DMAy_Streamx->CR & ((uint32_t)0x00080000)) != 0)
  {
     
    tmp = 1;
  }  
  else
  {
     
    tmp = 0;    
  }
  return tmp;
}


 









































































 

















 
FunctionalState DMA_GetCmdStatus(DMA_Stream_TypeDef* DMAy_Streamx)
{
  FunctionalState state = DISABLE;

   
  ((void)0);

  if ((DMAy_Streamx->CR & (uint32_t)((uint32_t)0x00000001)) != 0)
  {
     
    state = ENABLE;
  }
  else
  {
    
 
    state = DISABLE;
  }
  return state;
}













 
uint32_t DMA_GetFIFOStatus(DMA_Stream_TypeDef* DMAy_Streamx)
{
  uint32_t tmpreg = 0;
 
   
  ((void)0);
  
   
  tmpreg = (uint32_t)((DMAy_Streamx->FCR & ((uint32_t)0x00000038)));
  
  return tmpreg;
}














 
FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG)
{
  FlagStatus bitstatus = RESET;
  DMA_TypeDef* DMAy;
  uint32_t tmpreg = 0;

   
  ((void)0);
  ((void)0);

   
  if (DMAy_Streamx < ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x010)))
  {
     
    DMAy = ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000)); 
  } 
  else 
  {
     
    DMAy = ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400)); 
  }

   
  if ((DMA_FLAG & (uint32_t)0x20000000) != (uint32_t)RESET)
  {
     
    tmpreg = DMAy->HISR;
  }
  else
  {
     
    tmpreg = DMAy->LISR;
  }   
 
   
  tmpreg &= (uint32_t)(uint32_t)0x0F7D0F7D;

   
  if ((tmpreg & DMA_FLAG) != (uint32_t)RESET)
  {
     
    bitstatus = SET;
  }
  else
  {
     
    bitstatus = RESET;
  }

   
  return  bitstatus;
}














 
void DMA_ClearFlag(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG)
{
  DMA_TypeDef* DMAy;

   
  ((void)0);
  ((void)0);

   
  if (DMAy_Streamx < ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x010)))
  {
     
    DMAy = ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000)); 
  } 
  else 
  {
     
    DMAy = ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400)); 
  }

   
  if ((DMA_FLAG & (uint32_t)0x20000000) != (uint32_t)RESET)
  {
     
    DMAy->HIFCR = (uint32_t)(DMA_FLAG & (uint32_t)0x0F7D0F7D);
  }
  else 
  {
     
    DMAy->LIFCR = (uint32_t)(DMA_FLAG & (uint32_t)0x0F7D0F7D);
  }    
}














 
void DMA_ITConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT, FunctionalState NewState)
{
   
  ((void)0);
  ((void)0);
  ((void)0);

   
  if ((DMA_IT & ((uint32_t)0x00000080)) != 0)
  {
    if (NewState != DISABLE)
    {
       
      DMAy_Streamx->FCR |= (uint32_t)((uint32_t)0x00000080);
    }    
    else 
    {
       
      DMAy_Streamx->FCR &= ~(uint32_t)((uint32_t)0x00000080);  
    }
  }

   
  if (DMA_IT != ((uint32_t)0x00000080))
  {
    if (NewState != DISABLE)
    {
       
      DMAy_Streamx->CR |= (uint32_t)(DMA_IT  & (uint32_t)(((uint32_t)0x00000010) | ((uint32_t)0x00000008) | ((uint32_t)0x00000004) | ((uint32_t)0x00000002)));
    }
    else
    {
       
      DMAy_Streamx->CR &= ~(uint32_t)(DMA_IT & (uint32_t)(((uint32_t)0x00000010) | ((uint32_t)0x00000008) | ((uint32_t)0x00000004) | ((uint32_t)0x00000002)));
    }    
  }
}














 
ITStatus DMA_GetITStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT)
{
  ITStatus bitstatus = RESET;
  DMA_TypeDef* DMAy;
  uint32_t tmpreg = 0, enablestatus = 0;

   
  ((void)0);
  ((void)0);
 
   
  if (DMAy_Streamx < ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x010)))
  {
     
    DMAy = ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000)); 
  } 
  else 
  {
     
    DMAy = ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400)); 
  }

   
  if ((DMA_IT & (uint32_t)0x0F3C0F3C) != (uint32_t)RESET)
  {
     
    tmpreg = (uint32_t)((DMA_IT >> 11) & (uint32_t)(((uint32_t)0x00000010) | ((uint32_t)0x00000008) | ((uint32_t)0x00000004) | ((uint32_t)0x00000002)));   
    
     
    enablestatus = (uint32_t)(DMAy_Streamx->CR & tmpreg);
  }
  else 
  {
     
    enablestatus = (uint32_t)(DMAy_Streamx->FCR & ((uint32_t)0x00000080)); 
  }
 
   
  if ((DMA_IT & (uint32_t)0x20000000) != (uint32_t)RESET)
  {
     
    tmpreg = DMAy->HISR ;
  }
  else
  {
     
    tmpreg = DMAy->LISR ;
  } 

   
  tmpreg &= (uint32_t)(uint32_t)0x0F7D0F7D;

   
  if (((tmpreg & DMA_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {
     
    bitstatus = SET;
  }
  else
  {
     
    bitstatus = RESET;
  }

   
  return  bitstatus;
}














 
void DMA_ClearITPendingBit(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT)
{
  DMA_TypeDef* DMAy;

   
  ((void)0);
  ((void)0);

   
  if (DMAy_Streamx < ((DMA_Stream_TypeDef *) (((((uint32_t)0x40000000) + 0x00020000) + 0x6400) + 0x010)))
  {
     
    DMAy = ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6000)); 
  } 
  else 
  {
     
    DMAy = ((DMA_TypeDef *) ((((uint32_t)0x40000000) + 0x00020000) + 0x6400)); 
  }

   
  if ((DMA_IT & (uint32_t)0x20000000) != (uint32_t)RESET)
  {
     
    DMAy->HIFCR = (uint32_t)(DMA_IT & (uint32_t)0x0F7D0F7D);
  }
  else 
  {
     
    DMAy->LIFCR = (uint32_t)(DMA_IT & (uint32_t)0x0F7D0F7D);
  }   
}



 



 



 



 

 
