#ifndef _IO1X_Plained_drivers
#define _IO1X_Plained_drivers

#include "driver_init.h"
#include <stdio.h>
#include <at30tse75x.h>
#include <temperature_sensor.h>
#include <at30tse75x_config.h>

#define VCC_TARGET 3.3 /**< Tensão VCC da placa de R21 usada como referencia */
#define CONF_AT30TSE75X_RESOLUTION 2 /**< Resolução configurada para o sensor de temperatura */

struct temperature_sensor *AT30TSE75X; /**< Descritor para o sensor de temperatura AT30TSE75X */
struct io_descriptor* TARGETIO_DEBUG; /**< Descritor para a interface de comunicação USART de debug */

static struct at30tse75x AT30TSE75X_descr; /**< Descritor do sensor de temperatura AT30TSE75X */

/**
 * @brief Inicializa os parâmetros do sensor de luz, USART e sensor de temperatura.
 */
void IO_SENSOR_INIT(void);

/**
 * @brief Envia um byte para a UART de debug.
 *
 * @param byte_to_send Byte a ser enviado.
 */
void sendByteToUART(uint8_t byte_to_send);

/**
 * @brief Lê o valor digital do sensor de luz após passar pelo ADC e calcula a tensão medida pelo sensor.
 *
 * @return Tensão medida pelo sensor de luz.
 */
float readVoltageSensor(void);

/**
 * @brief Calcula a corrente com base na diferença entre a tensão de referência e a tensão medida,
 * considerando a relação entre a corrente do fototransistor e a iluminância.
 *
 * @param voltage Tensão medida pelo sensor de luz.
 * @return Corrente calculada.
 */
float readCurrentSensor(float voltage);

/**
 * @brief Calcula a iluminância com base nos valores medidos de corrente.
 *
 * @param current Corrente medida.
 * @return Iluminância calculada.
 */
float readLightSensor(float current);

/**
 * @brief Liga o LED da placa de expansão IO1X.
 */
void IO1X_LED_ON(void);

/**
 * @brief Desliga o LED da placa de expansão IO1X.
 */
void IO1X_LED_OFF(void);

/**
 * @brief Converte um número float em string com a precisão informada.
 * 
 * @param num Número a ser convertido.
 * @param str String resultante da conversão.
 * @param precision Precisão da conversão.
 */
void floatToString(float num, char* str, int precision);


#endif