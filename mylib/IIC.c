#include "IIC.h"


void MPUI2C_Init(void) {
    // Enable clock for GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Configure SDA and SCL as open-drain output
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = MPUI2C_SDA_PIN | MPUI2C_SCL_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; // Open-drain output
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPUI2C_SDA_PORT, &GPIO_InitStruct);

    // Set SDA and SCL high initially
    GPIO_SetBits(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN);
    GPIO_SetBits(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN);
}

void MPUI2C_Start(void) {
    // Generate start condition: SDA goes low while SCL is high
    MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, 1); // SDA high
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 1); // SCL high
    MPUI2C_DELAY();
    MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, 0); // SDA low
    MPUI2C_DELAY();
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 0); // SCL low
}

void MPUI2C_Stop(void) {
    // Generate stop condition: SDA goes high while SCL is high
    MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, 0); // SDA low
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 1); // SCL high
    MPUI2C_DELAY();
    MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, 1); // SDA high
    MPUI2C_DELAY();
}

void MPUI2C_CekAck(void){
	// Check for ACK
    MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, 1); // Release SDA
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 1); // Clock high
    MPUI2C_DELAY();
    if (MPUI2C_GPIO_Read(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN) == 0) {
        // ACK received
    } else {
        // NACK received
    }
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 0); // Clock low
    MPUI2C_DELAY();
};

void MPUI2C_Write(uint8_t data) {
    // Send 8 bits of data
    for (uint8_t i = 0; i < 8; i++) {
        MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, (data & 0x80) >> 7); // Send MSB first
        data <<= 1;
        MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 1); // Clock high
        MPUI2C_DELAY();
        MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 0); // Clock low
        MPUI2C_DELAY();
    }

   MPUI2C_CekAck();
}




uint8_t MPUI2C_Read(uint8_t ack) {
    uint8_t data = 0;

    // Read 8 bits of data
    for (uint8_t i = 0; i < 8; i++) {
        MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 1); // Clock high
        MPUI2C_DELAY();
        data <<= 1;
        data |= MPUI2C_GPIO_Read(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN); // Read SDA
        MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 0); // Clock low
        MPUI2C_DELAY();
    }

    // Send ACK or NACK
    if (ack) {
        MPUI2C_Ack();
    } else {
        MPUI2C_Nack();
    }

    return data;
}

void MPUI2C_Ack(void) {
    // Send ACK: SDA low
    MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, 0); // SDA low
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 1); // Clock high
    MPUI2C_DELAY();
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 0); // Clock low
    MPUI2C_DELAY();
    MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, 1); // Release SDA
}

void MPUI2C_Nack(void) {
    // Send NACK: SDA high
    MPUI2C_GPIO_Write(MPUI2C_SDA_PORT, MPUI2C_SDA_PIN, 1); // SDA high
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 1); // Clock high
    MPUI2C_DELAY();
    MPUI2C_GPIO_Write(MPUI2C_SCL_PORT, MPUI2C_SCL_PIN, 0); // Clock low
    MPUI2C_DELAY();
}

void MPUI2C_GPIO_Write(GPIO_TypeDef* port, uint16_t pin, uint8_t state) {
    if (state) {
        GPIO_SetBits(port, pin); // Set pin high
    } else {
        GPIO_ResetBits(port, pin); // Set pin low
    }
}

uint8_t MPUI2C_GPIO_Read(GPIO_TypeDef* port, uint16_t pin) {
    return GPIO_ReadInputDataBit(port, pin); // Read pin state
}
