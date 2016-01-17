/**
 ******************************************************************************
 * @file    stm32f4_discovery_lis3dsh.c
 * @author  Federico Iannucci - SE4
 * @version V1.0
 * @date    04-April-2015
 * @brief   LIS3DSH Accelerator library Module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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

#include <stm32f4xx_helper_lis3dsh.h>

static SPI_HandleTypeDef SpiHandle;

void SPIx_LIS3DSH_Init()
{
	/* Lis3dsh Init */
	LIS3DSH_InitTypeDef li;
	li.Axes_Enable = XYZ;
	li.BlockDataUpdate = BDU_CONTINUOS;
	li.FullScale = FS_16G;
	li.OutputDataRate = ODR_400_HZ;
	LIS3DSH_init(li);
}

/**
 * @brief  Configures LIS3DSH
 * @param  InitTypeDef: Specifies the configuration.
 * @retval None
 */
void LIS3DSH_init(LIS3DSH_InitTypeDef li) {

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure the LIS3DSH Control pins --------------------------------*/
	/* Enable CS GPIO clock and configure GPIO pin for LIS3DSH Chip select */
	LIS3DSH_SPI_CS_GPIO_CLK_ENABLE();

	/* Configure GPIO PIN for LIS Chip select */
	GPIO_InitStructure.Pin = LIS3DSH_SPI_CS_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(LIS3DSH_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

	/* Deselect: Chip Select high */
	/* SPI start when the chip select is low */
	LIS3DSH_CS_HIGH();

	/* Configure the SPI1 to communicate with the accelerometer */
	if (HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
	{
		/* SPI configuration -----------------------------------------------------*/
		SpiHandle.Instance = LIS3DSH_SPI;
		SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
		SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
		SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
		SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
		SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
		SpiHandle.Init.CRCPolynomial = 7;
		SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
		SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
		SpiHandle.Init.NSS = SPI_NSS_SOFT;
		SpiHandle.Init.TIMode = SPI_TIMODE_DISABLED;
		SpiHandle.Init.Mode = SPI_MODE_MASTER;

		/* SPI low level configuration */
		/* Enable the SPI peripheral */
		LIS3DSH_SPI_CLK_ENABLE();

		/* Enable SCK, MOSI and MISO GPIO clocks */
		LIS3DSH_SPI_GPIO_CLK_ENABLE();

		/* SPI SCK, MOSI, MISO pin configuration */
		GPIO_InitStructure.Pin = (LIS3DSH_SPI_SCK_PIN | LIS3DSH_SPI_MISO_PIN
				| LIS3DSH_SPI_MOSI_PIN);
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructure.Alternate = LIS3DSH_SPI_AF;
		HAL_GPIO_Init(LIS3DSH_SPI_GPIO_PORT, &GPIO_InitStructure);

		HAL_SPI_Init(&SpiHandle);
	}

	/* Configures the inner register of the accelerometer */
	LIS3DSH_writeRegister(LIS3DSH_CTRL_REG4_ADDR, li.OutputDataRate | li.Axes_Enable | li.BlockDataUpdate);
	LIS3DSH_writeRegister(LIS3DSH_CTRL_REG5_ADDR, li.FullScale);
}

/**
 * @brief  Read the axes value of LIS3DSH
 * @param  axes : return the axes values.
 * @retval Axes values.
 */
void LIS3DSH_readAxes(LIS3DSH_AxesOutputTypeDef* axes) {

	// Dichiaro variabili che conterrano i valori degli assi e degli offset
	uint8_t x_offset = 0x00;
	uint8_t y_offset = 0x00;
	uint8_t z_offset = 0x00;
	uint8_t tempData = 0;

	// Read x-axis
	// Read MSB
	LIS3DSH_readRegister(LIS3DSH_OUT_X_H_ADDR, &tempData);
	axes->x = (uint16_t) tempData;
	axes->x = axes->x << 8;	// Shift to the MSB.
	// Read LSB
	LIS3DSH_readRegister(LIS3DSH_OUT_X_L_ADDR, &tempData);
	axes->x = axes->x | (uint16_t) tempData; // Write LSB.

	// Read y-axis
	// Read MSB
	LIS3DSH_readRegister(LIS3DSH_OUT_Y_H_ADDR, &tempData);
	axes->y = (uint16_t) tempData;
	axes->y = axes->y << 8;	// Shift to the MSB.
	// Read LSB
	LIS3DSH_readRegister(LIS3DSH_OUT_Y_L_ADDR, &tempData);
	axes->y = axes->y | (uint16_t) tempData; // Write LSB.

	// Read z-axis
	// Read MSB
	LIS3DSH_readRegister(LIS3DSH_OUT_Z_H_ADDR, &tempData);
	axes->z = (uint16_t) tempData;
	axes->z = axes->z << 8;	// Shift to the MSB.
	// Read LSB
	LIS3DSH_readRegister(LIS3DSH_OUT_Z_L_ADDR, &tempData);
	axes->z = axes->z | (uint16_t) tempData; // Write LSB.
}

/**
 * @brief  Writes a register of the accelerometer.
 * @param  addr: Accelerometer's internal address to write to.
 * @param  value: Value to write.
 * @retval None
 */
void LIS3DSH_writeRegister(LIS3DSH_Register addr, uint8_t value) {

	/* Set chip select Low at the start of the transmission */
	LIS3DSH_CS_LOW();

	uint8_t dummyByte = 0;

	/* Send the Address of the indexed register */
	/* Send a Byte through the SPI peripheral */
	if (HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &addr,
			(uint8_t*) &dummyByte, 1, LIS3DSH_FLAG_TIMEOUT) != HAL_OK) {
		//Error TODO
	}

	/* Send the value of the indexed register */
	/* Send a Byte through the SPI peripheral */
	if (HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &value,
			(uint8_t*) &dummyByte, 1, LIS3DSH_FLAG_TIMEOUT) != HAL_OK) {
		//Error TODO
	}

	/* Set chip select High at the end of the transmission */
	LIS3DSH_CS_HIGH();
}

/**
 * @brief  Reads a register from the Accelerometer.
 * @param  addr : Address to read.
 * @param  value: The value read.
 * @retval None
 */
void LIS3DSH_readRegister(LIS3DSH_Register addr, uint8_t* value) {

	/* Set chip select Low at the start of the transmission */
	LIS3DSH_CS_LOW();

	addr |= (uint8_t)0x80;

	uint8_t dummyByte = 0;

	/* Send the Address of the indexed register */
	/* Send a Byte through the SPI peripheral */
	if (HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &addr,
			(uint8_t*) &dummyByte, 1, LIS3DSH_FLAG_TIMEOUT) != HAL_OK) {
		//Error TODO
	}

	/* Send the value of the indexed register */
	/* Send a Byte through the SPI peripheral */
	if (HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &dummyByte,
			(uint8_t*) value, 1, LIS3DSH_FLAG_TIMEOUT) != HAL_OK) {
		//Error TODO
	}

	/* Set chip select High at the end of the transmission */
	LIS3DSH_CS_HIGH();
}

void lis3dsh_readXAxes() {

}
void lis3dsh_readYAxes() {

}
void lis3dsh_readZAxes() {

}
