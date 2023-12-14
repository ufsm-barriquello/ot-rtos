#include <IO1_drivers.h> 

/** @file
 * @brief Funções relacionadas ao sensor de luz e suas operações.
 */

/** 
 * @brief Flag que indica se a conversão analógico-digital do sensor de luz foi concluída.
 */
volatile bool conversion_done = false;

/**
 * @brief Callback para a conclusão da conversão ADC do sensor de luz.
 *
 * @param descr Descritor ADC assíncrono.
 * @param channel Canal ADC.
 */
static void lightSensor_ADC_conversion_callback(const struct adc_async_descriptor *const descr, const uint8_t channel) {
	conversion_done = true;
}

/**
 * @brief Inicializa os parâmetros do sensor de luz, da USART e do sensor de temperatura.
 */
void IO_SENSOR_INIT(void){
   /* Inicialização parâmetros sensor de luz */
   adc_async_register_callback(&IO_SENSOR_ADC, 0, ADC_ASYNC_CONVERT_CB, lightSensor_ADC_conversion_callback);
   adc_async_enable_channel(&IO_SENSOR_ADC, 0);
   adc_async_start_conversion(&IO_SENSOR_ADC);
   
   /* Inicialização parâmetros USART */
   usart_sync_get_io_descriptor(&TARGETIO, &TARGETIO_DEBUG);
   
   /* Inicialização parâmetros do sensor de temperatura */
   i2c_m_sync_enable(&I2C_INSTANCE);
   AT30TSE75X = at30tse75x_construct(&AT30TSE75X_descr.parent, &I2C_INSTANCE, CONF_AT30TSE75X_RESOLUTION);
}

/**
 * @brief Envia um byte para a UART de debug.
 *
 * @param byte_to_send Byte a ser enviado.
 */
void sendByteToUART(uint8_t byte_to_send){
	io_write(TARGETIO_DEBUG, &byte_to_send, 1);
}

/**
 * @brief Lê o valor digital do sensor de luz após passar pelo ADC e calcula a tensão medida pelo sensor.
 *
 * @return Tensão medida pelo sensor de luz.
 */
float readVoltageSensor(void){
	uint8_t IO_SENSOR_VALUE;  //Armazena o valor lido do sensor de luz
	float IO_SENSOR_VOLTAGE; //Armazena a tensão medida pelo sensor
	/* Faz a conversão AD do sensor de luz*/
	adc_async_start_conversion(&IO_SENSOR_ADC);
	while(!conversion_done){}
	adc_async_read_channel(&IO_SENSOR_ADC, 0, &IO_SENSOR_VALUE, 1);
	
	/* Faz a definição dos valores de tensão lidos do sensor a partir dos dados quantizados do ADC*/
	IO_SENSOR_VOLTAGE = IO_SENSOR_VALUE * VCC_TARGET / 255;
	return IO_SENSOR_VOLTAGE;
}

/**
 * @brief Calcula a corrente com base na diferença entre a tensão de referência e a tensão medida,
 * considerando a relação entre a corrente do fototransistor e a iluminância.
 *
 * @param voltage Tensão medida pelo sensor de luz.
 * @return Corrente calculada.
 */
float readCurrentSensor(float voltage){
	float IO_SENSOR_CURRENT;
	/* Calcula a corrente com base na diferença entre a tensão de referência e a tensão medida,
	 considerando a relação entre a corrente do fototransistor e a iluminância*/
	IO_SENSOR_CURRENT = (VCC_TARGET - voltage)/100000; 
	return IO_SENSOR_CURRENT ;
}

/**
 * @brief Calcula a iluminância com base nos valores medidos de corrente.
 *
 * @param current Corrente medida.
 * @return Iluminância calculada.
 */
float readLightSensor(float current){
	float IO_ILUMINANCE;    //Representa a iluminância calculada com base nos valores medidos
	
	IO_ILUMINANCE = (current * 2 *10)/0.000001;
	return IO_ILUMINANCE;
}

/**
 * @brief Lê a temperatura do sensor de temperatura.
 *
 * @return Temperatura lida.
 */
uint16_t readTemperatureSensor(void){
	return (uint16_t)temperature_sensor_read(AT30TSE75X);
}

/**
 * @brief Liga o LED da placa de expansão IO1X.
 */
void IO1X_LED_ON(void){
	gpio_set_pin_level(LED, false);
}

/**
 * @brief Converte um número float em string com a precisão informada.
 * 
 * @param num Número a ser convertido.
 * @param str String resultante da conversão.
 * @param precision Precisão da conversão.
 */
void floatToString(float num, char* str, int precision) {
	int i = 0;
	int integralPart = (int)num;
	
	/* Converte a parte inteira para string */
	do {
		str[i++] = integralPart % 10 + '0';
		integralPart /= 10;
	} while (integralPart > 0);
	
	/* Inverte a string da parte inteira */
	int j;
	int len = i;
	for (j = 0; j < len / 2; j++) {
		char temp = str[j];
		str[j] = str[len - j - 1];
		str[len - j - 1] = temp;
	}
	
	/* Adiciona ponto decimal */
	str[i++] = '.';
	
	/* Converte a parte fracionária para string */
	float fractionalPart = num - (int)num;
	int k;
	for (k = 0; k < precision; k++) {
		fractionalPart *= 10;
		int digit = (int)fractionalPart;
		str[i++] = digit + '0';
		fractionalPart -= digit;
	}
	
	/* Adiciona caractere de término */
	str[i] = '\0';
}
