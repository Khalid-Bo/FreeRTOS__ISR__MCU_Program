#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <task.h>


// I2C slave adress
#define I2C_SLAVE_ADDRESS 13

//Variables
int Max_Value = 12;
int Min_Value = 6;
float Sensor_data;
float V;
float Vn;


// LED Pinout
int LED_Yellow = 5;
int LED_Green = 4;
int LED_Red = 6;

//Interrupt Pin
int ISRinterrupt_Pin = 2;


void setup() {
  
  Serial.begin(9600);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  pinMode(ISRinterrupt_Pin,INPUT);

  Wire.begin(I2C_SLAVE_ADDRESS);    
  Wire.onRequest(I2C_Transfer_Function);
  
  xTaskCreate(Read_Sensor_Data, "Read Sensor data", 400, NULL ,1,NULL);
  xTaskCreate(Process_Sensor_Data, "Process Sensor Data", 128, (void*)&Sensor_data,1,NULL);
  xTaskCreate(Notify_Output, "Notify With LEDs", 128, (void*)&Sensor_data,1,NULL);

  attachInterrupt(digitalPinToInterrupt(ISRinterrupt_Pin),ISRinterrupt,LOW);
  vTaskStartScheduler();
}

void Read_Sensor_Data( void *pvParameters )
{
  while(1)
  {
    Sensor_data = random(5,20);
    if ( isnan(Sensor_data))
      {
        Serial.println("Failed to read from sensor!");
      }
    vTaskDelay(100);
  }
}

void Process_Sensor_Data(void *pvParameters)
{
  while(1)
  {
    V =*((float*)pvParameters);
    Serial.print("Sensor Data:");
    Serial.println(V);
  }
}

void Notify_Output(void *pvParameters){
  while(1){
    Vn =*((float*)pvParameters);
    if (Vn <= Min_Value) {
    digitalWrite(LED_Yellow,HIGH);
    delay(200);
    digitalWrite(LED_Yellow,LOW);
    delay(200);
    }
    
    else if (Vn>Min_Value and Vn <Max_Value){
    digitalWrite(LED_Green,HIGH);
    delay(200);
    digitalWrite(LED_Green,LOW);
    delay(200);
    }
        
    else {
    digitalWrite(LED_Red,HIGH);
    delay(200);
    digitalWrite(LED_Red,LOW);
    delay(200);
    }
  }
}

void I2C_Transfer_Function(void *pvParameters)
 {  
    Wire.write((int)Sensor_data); 
  } 

void ISRinterrupt()
{
      sleep_cpu();
      digitalWrite(LED_Green,LOW);
      digitalWrite(LED_Red,LOW);
      digitalWrite(LED_Yellow,LOW);
}

void loop() {}
