#include"button.h"
#include <stdint.h>
void BUTTON_Init(BUTTON_HandleTypeDef *ButtonX, GPIO_TypeDef *GPIO_Button, uint16_t GPIO_Pin_Button)
{
	ButtonX->GPIO_Button = GPIO_Button;
	ButtonX->GPIO_Pin_Button = GPIO_Pin_Button;
	ButtonX->current_state = 1;
	ButtonX->last_state = 1;
	ButtonX->deboucing_state = 1;
	ButtonX->is_deboucing = 0;
	ButtonX->deboucing_timer = 0;
	ButtonX->counter = 0;
}

void BUTTON_Handle(BUTTON_HandleTypeDef *ButtonX)
{
	// detecting
	uint8_t temp_state = HAL_GPIO_ReadPin(ButtonX->GPIO_Button, ButtonX->GPIO_Pin_Button);
	if (temp_state != ButtonX->deboucing_state)
	{
		ButtonX->deboucing_state = temp_state;
		ButtonX->deboucing_timer = HAL_GetTick();
		ButtonX->is_deboucing = 1;
	}

	// deboucing
	if (ButtonX->is_deboucing == 1 && (HAL_GetTick() - ButtonX->deboucing_timer) > 15)
	{
		ButtonX->current_state = ButtonX->deboucing_state;
		ButtonX->is_deboucing = 0;
	}

	if (ButtonX->current_state != ButtonX->last_state)
	{
		if (ButtonX->current_state == 0) // Press the button
		{
			ButtonX->counter ++ ;

		}
		else // Release button
				{
			ButtonX->counter ++ ;

				}
		}
		ButtonX->last_state = ButtonX->current_state;
	}

