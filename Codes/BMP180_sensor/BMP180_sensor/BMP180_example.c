/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

/*
	SAMD21 Xplained Pro and BMP180 sensor.
	I2C Sync driver using SERCOM 3
	Connection:
	SAMD21 X pro ->	BMP180
	3.3V		 -> VCC
	GND			 -> GND
	PA08		 -> SDA
	PA09		 -> SCL
	
	written by:Mohannad Rawashdeh
	1st April 2020
	https://mb-raw.blogspot.com/

*/
#include "atmel_start.h"
#include <hal_gpio.h>
#include <hal_delay.h>
#include "bmp180/bmp180.h"

/*my location in Amman-Jordan and my real altitude 822 meter from sea level*/
#define myCurrentAltitude    822 

static void Start_i2c_bus()
{	
		i2c_m_sync_set_slaveaddr(&I2C_0, BMP180_I2C_ADDR, I2C_M_SEVEN);
		i2c_m_sync_enable(&I2C_0);
}

int main(void)
{
	atmel_start_init();
	
	Start_i2c_bus();
	
	printf("\r\n****************** Boach BMP180/085 Sensor Example **********************\r\n");
	
	uint8_t status= bmp180_sensor_Initi();

	if (status != 0) /*means there is no sensor connected or wrong connection*/
	{
		printf("sensor initi failed ! check your connection \r\n");
		
		return 0;
	}
	while(1)
	{		
		delay_ms(2000);
		/*read the Temp in C*/
		float temp_c=bmp180_read_temperature();
		/*read the pressure in pa*/
		uint32_t pressure=bmp180_read_pressure();
		/*read the sea level pressure based on your altitude , my location in Amman-Jordan and my real altitude 822 meter from sea level*/
		uint32_t realPressure=bmp180_read_SeaLevelPressure(myCurrentAltitude);
		/*you can calculate your altitude in meter based on the sensor's pressure reading*/
		uint32_t AltitudeFpressure=bmp180_get_AltitudeFromPressure(SeaLevelPressure,pressure);
		/*print the results*/
		printf("temperature= %2.1f Celsius\r\n",temp_c);
		printf("pressure= %lu pa\r\n",pressure);
		printf("sea level pressure= %lu pa\r\n",realPressure);
		printf("Altitude= %lu meter\r\n",AltitudeFpressure);
		printf("\r\n**********************************************\r\n");
		gpio_toggle_pin_level(LED0);
	}
}
