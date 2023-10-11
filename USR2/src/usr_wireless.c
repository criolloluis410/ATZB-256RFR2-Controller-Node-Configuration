 
#include "usr_wireless.h"
#include "sio2host.h"
#include "wireless_config.h"
#include "math.h"

uint16_t direccion=0xAB01;
// Datos de Subida
uint8_t NB[4];// Nivel de bateria del dispositivo
strc_NS listaNS[7];// Lista con niveles de senal de los nodos proximos, la cantidad de elementos no deberia exeder del numero de nodos
int longitudListaNS=0;// Contiene la cantidad de niveles de energia almacenados en el nodo
int cont_NS=0;// Ayuda a agregar nuevos valores de niveles de energia recibidos

//// Funciones para comvercion de numeros flotantes /////////////////////////////////////////////////////////////////////////////////////////
void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);

void usr_wireless_app_task(void)
{
	uint8_t mensaje_2[5];
	//mensaje_2[0]=(uint8_t)(direccion >> 8);
	//mensaje_2[1]=(uint8_t)direccion;
	mensaje_2[0]='a';
	mensaje_2[1]='b';
	mensaje_2[2]=0x00;
	mensaje_2[3]=0x0A;
	mensaje_2[4]=0xFF;
	
	int8_t chat_input = sio2host_getchar_nowait();
	static uint8_t payload_length,payload[aMaxPHYPacketSize-FRAME_OVERHEAD];	
	if (chat_input != -1)
	{
		if(payload_length == 0)
		{
		//	printf("\n\r TX :");
		}	
		payload[payload_length++] = chat_input;
		
	}
	if((chat_input == '\r')||(payload_length == (aMaxPHYPacketSize-FRAME_OVERHEAD)))
	{	
		//LED_Toggle(LED0);
		transmit_sample_frame(payload,payload_length-1);	
		sio2host_tx(mensaje_2,5);
		payload_length = 0;
	}
	if((chat_input == 'c')||(payload_length == (aMaxPHYPacketSize-FRAME_OVERHEAD)))
	{	
		LED_Toggle(LED0);
		LED_Toggle(LED1);
		LED_Toggle(LED2);
		if (cont_NS==0)
		{
			longitudListaNS=1;
		}
		else if (cont_NS>0)
		{
			longitudListaNS=cont_NS;
		}
		
		// Obtener nivel de Bateria ////////////////////////////////////////////////////////////
		char charBateria[20]; // Almacena el nivel de la bateria comvertido en una cadena de caracteres
		float floatBateria=get_bat_sensor_data();// Obtiene en nivel de bateria
		ftoa(floatBateria, charBateria, 4);// Combierte el nivel de bateria de float a una cadena de caracteres hexadecimales
		NB[0]=(uint8_t)charBateria[0];// almacena los cuatro primeros valores de la cadena en el vector NB
		NB[1]=(uint8_t)charBateria[1];
		NB[2]=(uint8_t)charBateria[2];
		NB[3]=(uint8_t)charBateria[3];	
			
		int indice=0;
		uint8_t TRAMA[50];//Trama de subida
		//-------IDENTIFICADOR FUENTE---------------------------------------//
		TRAMA[0]=(uint8_t)(SRC_ADDR >> 8);
		TRAMA[1]=(uint8_t) SRC_ADDR;
		//-------NIVEL DE BATERIA-------------------------------------------//	
		TRAMA[2]=NB[0];
		TRAMA[3]=NB[1];
		TRAMA[4]=NB[2];		
		TRAMA[5]=NB[3];	
			
		for (int i=0; i<longitudListaNS; i++)
		{	
		//-------NIVEL DE SEÑAL--------------------------------------------//
			TRAMA[indice+6]=(uint8_t)(listaNS[i].NS >> 8);
			TRAMA[indice+7]=(uint8_t)listaNS[i].NS;
		//-------IDENTIFICADOR DEL NIVEL DE SEÑAL--------------------------//
			TRAMA[indice+8]=(uint8_t)(listaNS[i].ID_DIR_NS >> 8);
			TRAMA[indice+9]=(uint8_t)listaNS[i].ID_DIR_NS;
			
			indice=indice+4;
		}
		TRAMA[(longitudListaNS*4)+6]=0xFE;// CH de parada

		//transmit_sample_frame(TRAMA, (longitudListaNS*4)+7);
		sio2host_tx(TRAMA, (longitudListaNS*4)+7);
		payload_length = 0;
	}
		
}

void usr_frame_received_cb(frame_info_t *frame)
{
	memset(&trama_recibida,0,sizeof(trama_recibida));// Elimina informacion previa de la estructura de recepcion
	memcpy(&trama_recibida,frame->mpdu,sizeof(trama_recibida));// copia la informacion de la memoria fuente(buffer) al detino
	//bmm_buffer_free(frame->buffer_header);//Elimina los datos del buffer, evita superpocicion.
	if (trama_recibida.d_dstn==SRC_ADDR) // Si se cumple esta condicion la trama es de subida
	{
		LED_Toggle(LED2);
		sio2host_tx(&(frame->mpdu[FRAME_OVERHEAD-FCS_LEN+1]),(frame->mpdu[0])-FRAME_OVERHEAD);
	}
	else if(trama_recibida.d_dstn==0xFFFF) // Si se cumple esta condicion la trama es de bajada
	{   LED_Toggle(LED0);
		// Almacenar Niveles de Energia ////////////////////////////////////////////////////////////
		int coincedencia_ID_NS=0;
		//int longitudListaNS_1=sizeof(listaNS);
		int longitudListaNS_1=7;
		// Berificar que el Nivel de energia no ha sido almacenado previamente
		for (int i=0; i<longitudListaNS_1; i++)
		{
			if (listaNS[i].ID_DIR_NS==trama_recibida.d_orgn)
			{
				coincedencia_ID_NS=1;// Si esta condicion se cumple, el NS de la trama rx ya se enuentra en la lista de niveles de energia
			}
			}
			if (coincedencia_ID_NS==0)// Si esta condicion se cumple, el NS de la trama rx se debe extraer del buffer y almacenarlo
			{
				// Extraccion del nivel de energia
				uint8_t *payload_ptr=frame->mpdu;
				uint8_t mpdu_len =payload_ptr[0]+2;
				uint8_t potencia_trama=payload_ptr[mpdu_len];
				//Agregacion de nuevos datos de NS
				listaNS[cont_NS].NS=potencia_trama;
				listaNS[cont_NS].ID_DIR_NS=trama_recibida.d_orgn;
				cont_NS++;
			}
	}
	delay_ms(0);
}

void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame)
{

}

///////////////////////////////////////////////////////////////////////////////////

void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

void ftoa(float n, char* res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.'; // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter
		// is needed to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}
////////////////////////////////////////////////////////////////////////////////////7
float get_bat_sensor_data(void)
{
	float bat_voltage;
	bat_voltage = reverse_float(convert_mv_to_v(tfa_get_batmon_voltage()));
	return bat_voltage;
}

/*Reverses a float variable*/
float reverse_float( const float float_val )
{
	#if UC3
	float retuVal;
	char *floatToConvert = (char *)&float_val;
	char *returnFloat = (char *)&retuVal;
	/* swap the bytes into a temporary buffer */
	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];
	return retuVal;
	#else
	return float_val; //nothing to be done for Little Endian System
	#endif
}

/* Converts milli Volt into Volt*/
float convert_mv_to_v(uint16_t float_val)
{
	return (float_val * (1.0/1000));
}
