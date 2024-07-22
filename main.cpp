#include "mbed.h"

#ifndef ENCENDIDO
#define ENCENDIDO  1
#endif

#ifndef APAGADO
#define APAGADO  !(ENCENDIDO)
#endif

#typedef enum{
    CALENTANDO,
    NORMAL,
    ENFRIANDO
} processState_t;

#define delay(ms)   thread_sleep_for(ms)
#define REGISTRY_TIME_INCREMENT   10
#define NUMBER_OF_AVG_SAMPLES 100

//--------- Definición de entradas y salidas ----------

AnalogIn tempSensor (A1);
DigitalOut maxTempLED (D2);
DigitalOut minTempLED (D3);
DigitalOut normalTempLED (D4);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//-------------- Declaración de variables ---------------

float maxTempValue = 50.0;
float minTempValue = 25.0;
float avgTempValue = (maxTempValue + minTempValue)/2;
int accum_time_reg = 0;
float lm35ReadingsArray[NUMBER_OF_AVG_SAMPLES];
float lm35ReadingsSum = 0.0;
float lm35ReadingsAverage = 0.0;
float lm35TempC = 0.0;
processState_t processState = NORMAL;

//--------- Declaración de Funciones prototipos ----------
void initOuputs();
void checkOverHeating();
void checkOverCooling();
void checkRegTime();
float tempConverter(float ratio);
void initSamplerAverager();                                 //El ratio es la señal analógica convertida a digital entre 0.0 y 1.0
void samplerAverager();
void stateUpdate();     


// main() runs in its own thread in the OS
int main()
{
    initOuputs();
    initSamplerAverager();
    while (true) {
        samplerAverager();
        checkOverHeating();
        checkOverCooling();
        stateUpdate();
        checkRegTime();
    }
}

void initOuputs(){
    maxTempLED = APAGADO;
    minTempLED = APAGADO;
    normalTempLED = ENCENDIDO;
}

void initSamplerAverager(){
    for (int i = 0; i < NUMBER_OF_AVG_SAMPLES; i++){
        lm35ReadingsArray[i] = tempSensor.read();
    }
}

float tempConverter(float v){                                    //Convierte señal eléctrica a ºC y actualiza el valor de temperatura
    float tempC = v * 3.3/0.01;
    return tempC;
}

void samplerAverager(){
    static int lm35SampleIndex = 0;
    int i = 0;
    lm35ReadingsArray[lm35SampleIndex] = tempSensor.read();
    lm35SampleIndex++;
    if(lm35SampleIndex >= NUMBER_OF_AVG_SAMPLES) {
        lm35SampleIndex = 0;
    };
    lm35ReadingsSum = 0.0;
    for (i = 0; i < NUMBER_OF_AVG_SAMPLES; i++) {
        lm35ReadingsSum = lm35ReadingsSum + lm35ReadingsArray[i];
    }
    lm35ReadingsAverage = lm35ReadingsSum / NUMBER_OF_AVG_SAMPLES;
    lm35TempC = tempConverter( lm35ReadingsAverage );
}


void checkOverHeating(){                                        //Chequea si la temperatura está por arriba del limite
    if(lm35TempC > maxTempValue){
        processState = ENFRIANDO;
        uartUsb.write("Alcanzó la máxima temperatura\r\n", 30); 
            
    }
}


void checkOverCooling(){                                        //Chequea si la temperatura está por abajo del limite
    if(lm35TempC < minTempValue){
        processState = CALENTANDO;
        uartUsb.write("Alcanzó la mínima temperatura\r\n", 30);
    }
}


void uartTask(){                                                    //Envia un registro de temperatura por Serial
    char str[30];
    sprintf(str, "Temperatura actual:%.2f \xB0 C\r\n", lm35TempC);  
    int stringLength = strlen(str);   //Keil Studio no me tomó la función strlen()
    uartUsb.write(str, stringLength);    
}


void checkRegTime(){                                            //Chequea el tiempo entre envíos de registros
    accum_time_reg = accum_time_reg + REGISTRY_TIME_INCREMENT;
    delay(REGISTRY_TIME_INCREMENT);
    if(accum_time_reg == 500){                              //Tiempo de 5 seg entre registros
        uartTask();                                             //Envía registro de temperatura 
        accum_time_reg = 0;
    }
}
