#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>
#include <hal_adc_async.h>
#include <hal_usart_sync.h>
#include <hal_i2c_m_sync.h>

/**
 * @brief Controlador ADC ass�ncrono para o sensor de luz.
 */
extern struct adc_async_descriptor IO_SENSOR_ADC;

/**
 * @brief Descritor de comunica��o USART s�ncrona.
 */
extern struct usart_sync_descriptor TARGETIO;

/**
 * @brief Descritor de comunica��o I2C mestre s�ncrona.
 */
extern struct i2c_m_sync_desc I2C_INSTANCE;

/**
 * @brief Inicializa o controlador ADC para o sensor de luz.
 */
void IO_SENSOR_ADC_INIT(void);

/**
 * @brief Inicializa os pinos necess�rios para a comunica��o USART.
 */
void TARGETIO_PORT_INIT(void);

/**
 * @brief Inicializa o clock para o m�dulo SERCOM0 usado para USART.
 */
void TARGETIO_CLOCK_INIT(void);

/**
 * @brief Inicializa o controlador de comunica��o USART.
 */
void TARGETIO_init(void);

/**
 * @brief Inicializa o clock para o m�dulo SERCOM1 usado para I2C.
 */
void I2C_INSTANCE_CLOCK_INSTANCE(void);

/**
 * @brief Inicializa o controlador de comunica��o I2C.
 */
void I2C_INSTANCE_INIT(void);

/**
 * @brief Inicializa os pinos necess�rios para a comunica��o I2C.
 */
void I2C_INSTANCE_PORT_INIT(void);

/**
 * @brief Inicializa o driver de atraso usando SysTick.
 */
void delay_driver_init(void);

/**
 * @brief Perform system initialization, initialize pins and clocks for peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif

#endif // DRIVER_INIT_INCLUDED
