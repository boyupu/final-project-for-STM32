

typedef void  (* SYSTICK_for_BUTTON_FUNCTION) (void );
void register_sfbf_callback (SYSTICK_for_BUTTON_FUNCTION p_func);

void	 HoldCheck_per_4ms(void);
//register_sfbf_callback(HoldCheck_per_4ms);
