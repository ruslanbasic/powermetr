/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"




/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void GPIO_Inite(void);
void DMA_Inite(void);
void ADC_Inite(void);
void USART1_UART_Inite(void);
void start_conv (void);
uint32_t fsqrt (uint64_t arg);

uint8_t meas;
uint16_t AdcData[804];
uint64_t p_sum, v_sum, i_sum;
uint16_t va_min, ia_min, va_max, ia_max, ia, va, v_offset, i_offset;
uint32_t i_out, v_out, p_out;


int main(void)
{
	/* Configure the system clock */
	SystemClock_Config();


	/* Initialize all configured peripherals */
	//GPIO_Inite();
	ADC_Inite();
	DMA_Inite();
	USART1_UART_Inite();

	uint32_t tmd, tm;
	uint16_t  w, point;
	/* Infinite loop */

	while (1)
	{
		//USART_SendData (USART1, 0x0f);
		tm=0xffffff;
		while (tm)
		{
			tm--;
		}
		start_conv ();
		p_sum=0;
		v_sum=0;
		i_sum=0;

		va_max=0;
		va_min=0x0fff;
		ia_max=0;
		ia_min=0x0fff;


		for (point=1; point<401; point++)
		{
			if (va_max < AdcData[(point*2)+1])
			{
				va_max = AdcData[(point*2)+1];
			}
			if (va_min > AdcData[(point*2)+1])
			{
				va_min = AdcData[(point*2)+1];
			}
			if (ia_max < AdcData[point*2])
			{
				ia_max = AdcData[point*2];
			}
			if (ia_min > AdcData[point*2])
			{
				ia_min = AdcData[point*2];
			}
		}

		v_offset=va_min + ((va_max-va_min)/2);
		i_offset=ia_min + ((ia_max-ia_min)/2);

		for (point=1; point<401; point++)
		{

			if (v_offset > AdcData[(point*2)+1])
			{
				va=v_offset - AdcData[(point*2)+1];
			}
			else
			{
				va= AdcData[(point*2)+1] - v_offset;
			}

			if (i_offset > AdcData[point*2])
			{
				ia=i_offset - AdcData[point*2];
			}
			else
			{
				ia= AdcData[point*2] - i_offset;
			}



			p_sum += (uint64_t)ia*(uint64_t)ia*(uint64_t)va*(uint64_t)va;
			i_sum += (uint64_t)ia*(uint64_t)ia;
			v_sum += (uint64_t)va*(uint64_t)va;


//			tmd=AdcData[point*2];

//			w=0;
//			while (tmd >= 10000)
//			{
//				tmd -= 10000;
//				w++;
//			}
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, w+0x30);
//			w=0;
//			while (tmd >= 1000)
//			{
//				tmd -= 1000;
//				w++;
//			}
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, w+0x30);
//			w=0;
//			while (tmd >= 100)
//			{
//				tmd -= 100;
//				w++;
//			}
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, w+0x30);
//			w=0;
//			while (tmd >= 10)
//			{
//				tmd -= 10;
//				w++;
//			}
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, w+0x30);
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, tmd+0x30);
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, ',');
//
//			tmd=AdcData[(point*2)+1];
//			w=0;
//			while (tmd >= 10000)
//			{
//				tmd -= 10000;
//				w++;
//			}
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, w+0x30);
//			w=0;
//			while (tmd >= 1000)
//			{
//				tmd -= 1000;
//				w++;
//			}
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, w+0x30);
//			w=0;
//			while (tmd >= 100)
//			{
//				tmd -= 100;
//				w++;
//			}
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, w+0x30);
//			w=0;
//			while (tmd >= 10)
//			{
//				tmd -= 10;
//				w++;
//			}
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, w+0x30);
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, tmd+0x30);
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, ',');
//
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, 0x0d);
//			while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//			USART_SendData (USART1, 0x0a);





			///////////
		}
		p_sum=p_sum/400;
		i_sum=i_sum/400;
		v_sum=v_sum/400;

		p_out=(122*fsqrt(p_sum))/100;
		i_out=(100*fsqrt(i_sum))/81;
		v_out=(100*fsqrt(v_sum))/82;

//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'U');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'm');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'i');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'n');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, '=');
//
//		tmd=va_min;
//
//		w=0;
//		while (tmd >= 10000)
//		{
//			tmd -= 10000;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 1000)
//		{
//			tmd -= 1000;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 100)
//		{
//			tmd -= 100;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 10)
//		{
//			tmd -= 10;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, tmd+0x30);
//
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 0x0d);
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 0x0a);
//		////////////////////////////////////////////////////////////////////////////////////
//
//
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'K');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'P');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'o');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'w');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '=');

		tmd=(1000*(p_out/v_out))/i_out;


		w=0;
		while (tmd >= 1000)
		{
			tmd -= 1000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '.');
		w=0;
		while (tmd >= 100)
		{
			tmd -= 100;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		w=0;
		while (tmd >= 10)
		{
			tmd -= 10;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, tmd+0x30);

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0d);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0a);
		////////////////////////////////////////////////////////////////////////////////////

//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'I');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'm');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'i');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'n');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, '=');
//
//		tmd=ia_min;
//
//		w=0;
//		while (tmd >= 10000)
//		{
//			tmd -= 10000;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 1000)
//		{
//			tmd -= 1000;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 100)
//		{
//			tmd -= 100;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 10)
//		{
//			tmd -= 10;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, tmd+0x30);
//
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 0x0d);
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 0x0a);
//		////////////////////////////////////////////////////////////////////////////////////
//
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'I');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'm');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'a');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 'x');
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, '=');
//
//		tmd=ia_max;
//
//		w=0;
//		while (tmd >= 10000)
//		{
//			tmd -= 10000;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 1000)
//		{
//			tmd -= 1000;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 100)
//		{
//			tmd -= 100;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		w=0;
//		while (tmd >= 10)
//		{
//			tmd -= 10;
//			w++;
//		}
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, w+0x30);
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, tmd+0x30);
//
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 0x0d);
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
//		USART_SendData (USART1, 0x0a);
		////////////////////////////////////////////////////////////////////////////////////


		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'U');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'r');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'm');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 's');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '=');

		tmd=v_out;

		w=0;
		while (tmd >= 10000)
		{
			tmd -= 10000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		w=0;
		while (tmd >= 1000)
		{
			tmd -= 1000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		w=0;
		while (tmd >= 100)
		{
			tmd -= 100;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '.');
		w=0;
		while (tmd >= 10)
		{
			tmd -= 10;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, tmd+0x30);

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0d);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0a);
		////////////////////////////////////////////////////////////////////////////////////

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'I');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'r');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'm');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 's');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '=');

		tmd=i_out;

		w=0;
		while (tmd >= 10000)
		{
			tmd -= 10000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '.');
		w=0;
		while (tmd >= 1000)
		{
			tmd -= 1000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		w=0;
		while (tmd >= 100)
		{
			tmd -= 100;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);

		w=0;
		while (tmd >= 10)
		{
			tmd -= 10;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, tmd+0x30);

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0d);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0a);
		////////////////////////////////////////////////////////////////////////////////////

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'P');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'r');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'm');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 's');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '=');

		tmd=p_out;



		w=0;
		while (tmd >= 1000000)
		{
			tmd -= 1000000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '.');
		w=0;
		while (tmd >= 100000)
		{
			tmd -= 100000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);w=0;
		while (tmd >= 10000)
		{
			tmd -= 10000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);

		w=0;
		while (tmd >= 1000)
		{
			tmd -= 1000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		w=0;
		while (tmd >= 100)
		{
			tmd -= 100;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);

		w=0;
		while (tmd >= 10)
		{
			tmd -= 10;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, tmd+0x30);

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0d);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0a);
		////////////////////////////////////////////////////////////////////////////////////

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 'P');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, ' ');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, ' ');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, ' ');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '=');

		tmd=i_out*v_out;

		w=0;
		while (tmd >= 1000000)
		{
			tmd -= 1000000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '.');
		w=0;
		while (tmd >= 100000)
		{
			tmd -= 100000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);

			w=0;
		while (tmd >= 10000)
		{
			tmd -= 10000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		w=0;
		while (tmd >= 1000)
		{
			tmd -= 1000;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);


		w=0;
		while (tmd >= 100)
		{
			tmd -= 100;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);

		w=0;
		while (tmd >= 10)
		{
			tmd -= 10;
			w++;
		}
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, w+0x30);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, tmd+0x30);

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0d);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0a);
		////////////////////////////////////////////////////////////////////////////////////






		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '-');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '-');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '-');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '-');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '-');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '-');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '-');
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, '-');

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0d);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0a);

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0d);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0a);

		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0d);
		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE)){}
		USART_SendData (USART1, 0x0a);


	}


}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

	/////////
#ifdef use_HSE
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	/* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/
	/* Enable HSE */
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);

	/* Wait till HSE is ready and if Time out is reached exit */
	do
	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if ((RCC->CR & RCC_CR_HSERDY) != RESET)
	{
		HSEStatus = (uint32_t)0x01;
	}
	else
	{
		HSEStatus = (uint32_t)0x00;
	}

	if (HSEStatus == (uint32_t)0x01)
	{
		/* Enable Prefetch Buffer and set Flash Latency */
		FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

		/* HCLK = SYSCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

		/* PCLK = HCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

		/* PLL configuration = HSE * 6 = 48 MHz */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL6);

		/* Enable PLL */
		RCC->CR |= RCC_CR_PLLON;

		/* Wait till PLL is ready */
		while((RCC->CR & RCC_CR_PLLRDY) == 0)
		{
		}

		/* Select PLL as system clock source */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

		/* Wait till PLL is used as system clock source */
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
		{
		}
	}
	else
	{ /* If HSE fails to start-up, the application will have wrong clock
		         configuration. User can add here some code to deal with this error */
	}

#endif

#ifdef use_HSI

	/* Enable Prefetch Buffer and set Flash Latency */
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

	/* HCLK = SYSCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

	/* PCLK = HCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

	/* PLL configuration = HSI/2 * 12 = 48 MHz */
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
	RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL12);

	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;

	/* Wait till PLL is ready */
	while((RCC->CR & RCC_CR_PLLRDY) == 0)
	{
	}

	/* Select PLL as system clock source */
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

	/* Wait till PLL is used as system clock source */
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
	{
	}




#endif

	/////////

}

/* ADC init function */
void ADC_Inite(void)
{

	ADC_InitTypeDef ADC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//TIM_OCInitTypeDef TIM_OCInitStruct;

	/* Peripheral clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);

	/**ADC GPIO Configuration
	PA0   ------> ADC_IN0
	PA1   ------> ADC_IN1
	PA3   ------> ADC_IN3
	PA4   ------> ADC_IN4
	PA5   ------> ADC_IN5
	PA6   ------> ADC_IN6
	*/


//	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

//	GPIO_Init (GPIOA, &GPIO_InitStruct);
//
//	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;
//	GPIO_Init (GPIOA, &GPIO_InitStruct);
//
//	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;
//	GPIO_Init (GPIOA, &GPIO_InitStruct);
//
//
//	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4;
//	GPIO_Init (GPIOA, &GPIO_InitStruct);


	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5;
	GPIO_Init (GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;
	GPIO_Init (GPIOA, &GPIO_InitStruct);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStruct);
	/* Configure the ADC1 in continous mode with a resolutuion equal to 12 bits */
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1,&ADC_InitStruct);
	// channels selection and it's sampling time config
	ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_7_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_6, ADC_SampleTime_7_5Cycles);

	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);
	/* Enable ADC1 */
	ADC_Cmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_ADEN));/* Wait the ADCEN falg */
	/* ADC1 regular Software Start Conv */

	// 7,5 >> 20tic >>> 700k 1.42 uS
	// 30mS  600 count 50uS / 2uS
	// >> tick = 1uS pulse =2 all = 50
	// >> 2 заміра на клок таймера


	TIM_TimeBaseInitStruct.TIM_Period = 49;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 47;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;

	TIM_TimeBaseInit (TIM3, &TIM_TimeBaseInitStruct);

//	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Disable;
//	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
//	TIM_OCInitStruct.TIM_Pulse = 47;
//	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
//	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
//
//	TIM_OC1Init(TIM3, &TIM_OCInitStruct);


	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);



	//TIM_Cmd(TIM3, ENABLE);
	ADC_StartOfConversion(ADC1);

}

/* USART1 init function */
void USART1_UART_Inite(void)
{

	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Peripheral clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE);
  
	/**USART1 GPIO Configuration
	PA9   ------> USART1_TX
	PA10   ------> USART1_RX
	 */

	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_10;
  	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

  	GPIO_Init ( GPIOA, &GPIO_InitStruct );

  	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_9;
  	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

  	GPIO_Init ( GPIOA, &GPIO_InitStruct );

  	GPIO_PinAFConfig ( GPIOA, GPIO_PinSource9, GPIO_AF_1);
  	GPIO_PinAFConfig ( GPIOA, GPIO_PinSource10, GPIO_AF_1);


  	USART_InitStruct.USART_BaudRate = 9600;
  	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  	USART_InitStruct.USART_StopBits = USART_StopBits_1;
  	USART_InitStruct.USART_Parity = USART_Parity_No ;
  	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  	USART_Init(USART1, &USART_InitStruct);

	/* USART1 interrupt Init */

	//NVIC_SetPriority(USART1_IRQn, 0);
	//NVIC_EnableIRQ(USART1_IRQn);

  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;

	NVIC_Init (&NVIC_InitStruct);



	USART_Cmd(USART1, ENABLE);

}

/** 
  * Enable DMA controller clock
  */
void DMA_Inite(void)
{

	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* DMA controller clock enable */
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
//	NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
//	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	//DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AdcData[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 804;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	/* ADC DMA request in circular mode */
	ADC_DMARequestModeConfig(ADC1,ADC_DMAMode_Circular);
	ADC_DMACmd(ADC1,ENABLE);

	DMA_ITConfig(DMA1_Channel1 ,DMA_IT_TC, ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel=DMA1_Channel1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;

	NVIC_Init (&NVIC_InitStruct);

}

void GPIO_Inite(void)
{

//  LL_EXTI_InitTypeDef EXTI_InitStruct;
//
//  /* GPIO Ports Clock Enable */
//  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
//
//  /**/
//  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE2);
//
//  /**/
//  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_NO);
//
//  /**/
//  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_INPUT);
//
//  /**/
//  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
//  EXTI_InitStruct.LineCommand = ENABLE;
//  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
//  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
//  LL_EXTI_Init(&EXTI_InitStruct);

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  while(1) 
  {
  }
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		USART_SendData (USART1, USART_ReceiveData (USART1));
	}
}
void DMA1_Channel1_IRQHandler(void)
{
	if (DMA_GetITStatus (DMA1_IT_TC1))
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);
		TIM_Cmd(TIM3, DISABLE);
		meas=1;
	}
}
void start_conv (void)
{
	TIM_SetCounter (TIM3, 0);
	TIM_Cmd(TIM3, ENABLE);
	meas=0;
	while (meas) {}
}
uint32_t fsqrt (uint64_t arg)
{
	uint32_t atmp=1;
	uint8_t lim=100;
	while ( ( ((atmp*atmp-arg) >= 2) || ((arg-atmp*atmp) >= 2) ) && (lim-- > 1))
	{
		atmp=((arg/atmp)+atmp)/2;
	}
	return atmp;
}


#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
