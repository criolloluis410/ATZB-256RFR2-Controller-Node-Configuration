/**
 * \file usart_serial.c
 *
 * Created: 11/10/2022 10:09:24
 * Author: pract
 *
 * \brief  API file of USART as generated by Project Wizard
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \page license License
 * Copyright(c) 2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */


#include "usart_serial.h"

//TODO (Project Wizard) - Call this function to send test data to Usart Serial Port

/**
 * \brief This function transmits Debug Data to the terminal
 */
void send_usart_data(const char* testData)
{
	printf("%s", "TestData...  ");
	printf(testData);
	printf("\n\r");
	//INFO (Project Wizard) - Alternatively sio2host_tx("Hello",5); function can be used to send data as well
}

//TODO (Project Wizard) - Call this function to read test data from Usart Serial Port
/**
 * \brief This function performs a blocking one byte read functionality 
 */
char read_usart_data(void)
{
	return sio2host_getchar();
 //INFO (Project Wizard) -  Function sio2host_rx(&rx_data,MAX_LENGTH) can be used to receive buffered data
 /* It returns the pointer to the received/buffered data in "rx_data" and returns how many bytes of data have been received by the device */
}