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
  
  xTaskCreate(Read_Sensor_Data, "Read Data from Sensor", 400, NULL ,1,NULL);
  xTaskCreate(ConsolDisplay_Sensor_Data, "Display Reading Sensor data on Consol", 128, (void*)&Sensor_data,1,NULL);
  xTaskCreate(Notify_Output, "Notify With LEDs the State of the Parameter Measured", 128, (void*)&Sensor_data,1,NULL);

  attachInterrupt(digitalPinToInterrupt(ISRinterrupt_Pin),ISRinterrupt,LOW); // Interrupt function to ensure that our MCU will stay in idle mode until the master send Signal High that wake Up our MCU
  vTaskStartScheduler();}

// Read_Sensor_Data function will Read data from sensor 
void Read_Sensor_Data( void *pvParameters ){
  while(1)
  {
    Sensor_data = random(5,20);
    if ( isnan(Sensor_data))
      {
        Serial.println("Failed to read from sensor!");
      }
    vTaskDelay(100);
  }}


// Display Sensor Information on the console
void ConsolDisplay_Sensor_Data(void *pvParameters){
  while(1)
  {
    V =*((float*)pvParameters);
    Serial.print("Sensor Data:");
    Serial.println(V);
  }}

void Notify_Output(void *pvParameters){
  while(1){
    Vn =*((float*)pvParameters);

    if (Vn <= Min_Value)  // if the Sensor value is lower than our min value posed the The Yellow LED will blink  
    {
      toggleLED(LED_Yellow)
    }
    
    else if (Vn>Min_Value and Vn <Max_Value) // if the Sensor value is between the max and min value posed the The Green LED will blink  
    {
      toggleLED(LED_Green)
    }
        
    else  // if the Sensor value is higher than the Max value posed the The Red LED will blink  
    {
      toggleLED(LED_Red)
    }
  }}

void toggleLED(int pin) {
    digitalWrite(pin, HIGH);
    vTaskDelay(pdMS_TO_TICKS(200));
    digitalWrite(pin, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));}

void I2C_Transfer_Function(void *pvParameters){  
    Wire.write((int)Sensor_data); // Sending Integer Data
  //Wire.write((uint8_t*)&Sensor_data, sizeof(Sensor_data)); // Sending Precise Sensor Informations type Float 
  } 

void ISRinterrupt(){
      sleep_cpu();
      digitalWrite(LED_Green,LOW);
      digitalWrite(LED_Red,LOW);
      digitalWrite(LED_Yellow,LOW);}

void loop() {}
