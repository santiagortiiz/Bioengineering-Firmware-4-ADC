/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
char dataReady=0;
unsigned int ADCresult;
unsigned int voltaje;
unsigned char control=0;

void control_intensidad_led(void);

CY_ISR(contador);
CY_ISR(ADC_INT);
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    isr_ADC_StartEx(ADC_INT);
    ADC_IRQ_Enable();
    ADC_Start();
    
    LCD_Start();
    LCD_ClearDisplay();
    LCD_PrintString("Voltaje medido [mV]:");
    
    PWM_Init();
    
    Contador_Start();
    isr_500ms_StartEx(contador);

    for(;;)
    {
        if (dataReady==1){      // counts to mV hace lo mismo que 5000/4095
            dataReady=0;
            voltaje=(ADCresult);//*(5000/4095)); // agrego tantos 0s al 5 para mostrar mili, micro... etc
            voltaje=ADC_CountsTo_mVolts(ADCresult);     // sirve para cuando use todo el rango de voltaje  
        }
        if (voltaje>2000){
            led_Write(1);  
        }
        if (voltaje<=2000){
            led_Write(0);
        }
        
        if (voltaje<=500)PWM_Stop();
        
        if (500 <= voltaje && voltaje <= 1000) {
        PWM_Stop();PWM_WritePeriod(10000);PWM_WriteCompare(100);PWM_Start();
        }
        if (1001 <= voltaje && voltaje <= 2000){
            PWM_Stop();PWM_WritePeriod(10000);PWM_WriteCompare(500);PWM_Start();
        }
        if (2001 <= voltaje && voltaje <= 3000){
            PWM_Stop();PWM_WritePeriod(10000);PWM_WriteCompare(2500);PWM_Start();
        }
        if (3001 <= voltaje && voltaje <= 4000) {
            PWM_Stop();PWM_WritePeriod(10000);PWM_WriteCompare(5000);PWM_Start();
        }
        if (4001 <= voltaje && voltaje <= 5000) {
            PWM_Stop();PWM_WritePeriod(10000);PWM_WriteCompare(10000);PWM_Start();
        }
        
        LCD_Position(2,0);
        LCD_PrintNumber(voltaje);
    }
        
}

CY_ISR(ADC_INT){
    dataReady=1;
    ADCresult=ADC_GetResult16();
}

CY_ISR(contador){
    ADC_StartConvert();
}

/* [] END OF FILE */


//void control_intensidad_led()
//{
//    if (500 <= voltaje && voltaje <= 1000) {
//        PWM_Start();PWM_WritePeriod(50000);PWM_WriteCompare(45000);
//    }
//    if (2000 <= voltaje && voltaje <= 3000){
//        PWM_Start();PWM_WritePeriod(50000);PWM_WriteCompare(40000);
//    }
//    if (4000 <= voltaje && voltaje <= 5000) {
//        PWM_Start();PWM_WritePeriod(1999);PWM_WriteCompare(49999);
//    }
//    else PWM_Stop();
//}