#ifndef _IO1X_Plained_drivers
#define _IO1X_Plained_drivers

#include "driver_init.h"
#include <stdio.h>
#include <at30tse75x.h>
#include <temperature_sensor.h>
#include <at30tse75x_config.h>

#define VCC_TARGET 3.3 /**< Tens�o VCC da placa de R21 usada como referencia */
#define CONF_AT30TSE75X_RESOLUTION 2 /**< Resolu��o configurada para o sensor de temperatura */

struct temperature_sensor *AT30TSE75X; /**< Descritor para o sensor de temperatura AT30TSE75X */
struct io_descriptor* TARGETIO_DEBUG; /**< Descritor para a interface de comunica��o USART de debug */

static struct at30tse75x AT30TSE75X_descr; /**< Descritor do sensor de temperatura AT30TSE75X */

/**
 * @brief Inicializa os par�metros do sensor de luz, USART e sensor de temperatura.
 */
void IO_SENSOR_INIT(void);

/**
 * @brief Envia um byte para a UART de debug.
 *
 * @param byte_to_send Byte a ser enviado.
 */
void sendByteToUART(uint8_t byte_to_send);

/**
 * @brief L� o valor digital do sensor de luz ap�s passar pelo ADC e calcula a tens�o medida pelo sensor.
 *
 * @return Tens�o medida pelo sensor de luz.
 */
float readVoltageSensor(void);

/**
 * @brief Calcula a corrente com base na diferen�a entre a tens�o de refer�ncia e a tens�o medida,
 * considerando a rela��o entre a corrente do fototransistor e a ilumin�ncia.
 *
 * @param voltage Tens�o medida pelo sensor de luz.
 * @return Corrente calculada.
 */
float readCurrentSensor(float voltage);

/**
 * @brief Calcula a ilumin�ncia com base nos valores medidos de corrente.
 *
 * @param current Corrente medida.
 * @return Ilumin�ncia calculada.
 */
float readLightSensor(float current);

/**
 * @brief Liga o LED da placa de expans�o IO1X.
 */
void IO1X_LED_ON(void);

/**
 * @brief Desliga o LED da placa de expans�o IO1X.
 */
void IO1X_LED_OFF(void);

/**
 * @brief Converte um n�mero float em string com a precis�o informada.
 * 
 * @param num N�mero a ser convertido.
 * @param str String resultante da convers�o.
 * @param precision Precis�o da convers�o.
 */
void floatToString(float num, char* str, int precision);


#endif