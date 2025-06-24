/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "user_defined.h"

//>>=====================
// Step 1. Type Declaration
typedef void  (* SYSTICK_for_BUTTON_FUNCTION) (void );
// Step 2. Variable Declaration
static SYSTICK_for_BUTTON_FUNCTION  p_callback_sfbf = 0;
// Step 3. register function
void register_sfbf_callback (SYSTICK_for_BUTTON_FUNCTION p_func);
void register_sfbf_callback (SYSTICK_for_BUTTON_FUNCTION p_func) {
	p_callback_sfbf =  p_func;
}
// Step 4. register user's function  (a calling sample)
// void user_sfsb_handler(void);
// register_sfbf_callback(user_sfsb_handler);
//<<==================

__inline static void exti_key_debounce(void);

//====================
//  Core Timer (SysTem Ticker) Interrupt Handler
//=========================================================
extern volatile uint32_t msTick;
void SysTick_Handler(void);
void SysTick_Handler(void)
{
	msTick++;
//-------------------
//	#ifdef USE_EXTI_DEBOUNCE
	if ((msTick & 0x03) == 0){  // bit 0 = bit 1 = 0: 4ms
	 exti_key_debounce();

		//Step 5. ++++ Calling Back ++++++++++++++++++
		if (p_callback_sfbf) {	   // if not NULL
            p_callback_sfbf();  // SYSTICK_for_BUTTON_FUNCTION(void);
       }
    //<<<<<<<+++++++++++++++++++++++++++
	} // ENd OF if ((ms_timer & 0x03) == 0)
//  #endif
//<------------------
}


//#################### cf. user_defined.c  ###########################
void	Debounce(uint32_t N, GPIO_TypeDef* GPIOx);
extern  uint8_t	debState[16];

/**
  * @brief  This function is called by function SysTick_Handler.
  * @param  None
  * @retval : None
  */
 __inline static void exti_key_debounce(void)
{  
  //>>>>>>>> Keys Debounce
//	if ((msTick & 0x03) == 0){  // bit 0 = bit 1 = 0: 4ms
  //==>  for PA0 as EXTI 0
		if (debState[0] != 0 )
		   Debounce(0, GPIOA); 	// pin PA0
/*  ############################################
		In comparison with the one in "user_definied.c", this function lacks
			 function "HoldCheck_per_4ms();"
		############################################
*/		
//	} // ENd OF if ((ms_timer & 0x03) == 0)
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<
}


//========= Preparing Trial ==================================
/* ---> 0. Let this file (callback_example.c) NOT be included in Target Build
     0.1 set a debug breakpoint in Function "void EXTI0_IRQHandler(void)" 
                              in "user_defined.c"
     0.2 run debug and press Key1 to see if the program stops at the breakpoint 
            in function "EXTI0_IRQHandler(void)". Then delete this breakpoint,
            and then let it run and 
                HOLD pressing Key1 until two LEDs flash alternatively.

     0.3 And NOW it go through to the while loop in File "main_callback.c".

////  NOTE ---> to make sure the SETTINGS of  "External interrupt/event Configuration" 
//               in GPIO_Ini_raw.c
#define __EXTI_SETUP              1                       //  0
#define __EXTI_USED               0x00001                 //  1
#define __EXTI_INTERRUPTS         0x00000001              //  2
#define __EXTI_IMR                0x00000001              //  3
#define __EXTI_EMR                0x00000000              //  4
#define __EXTI_RTSR               0x00000001              //  5
#define __EXTI_FTSR               0x00000001              //  6
#define __SYSCFG_EXTICR1          0x00000000              //  7
#define __SYSCFG_EXTICR2          0x00000000              //  8
#define __SYSCFG_EXTICR3          0x00000000              //  9
#define __SYSCFG_EXTICR4          0x00000000              // 10
*/

//========= FIRST Trial ==================================
/* ---> 1. Let this file (callback_example.c) be included in Target Build
				1.1 to add "__WEAK" to  "SysTick_Handler" in user_define.c:
		
__WEAK void SysTick_Handler(void)
{
	msTick++;
//-------------------
	#ifdef USE_EXTI_DEBOUNCE
	 exti_key_debounce();
  #endif
//<------------------
}

     1.2 run debug and press Key1 to see if the program stops at the breakproint.

     1.3 But it NEVER go through to the while loop in File "main_callback.c".

*/

//========= SECOND Trial: Use Callback Function ==================================
/* ---> 2. to be used in "main_callback" file
		  register_sfbf_callback(HoldCheck_per_4ms);  //<<<< additional CODE
		hold_EXTI_ms(2000);

  while(get_ButtonHoldState() ==0){}	
			register_sfbf_callback(0);									//<<<< additional CODE
*/





