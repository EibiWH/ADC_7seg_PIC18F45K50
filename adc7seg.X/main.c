/*
 * File:   main.c
 * Author: big_m
 *
 * Created on 25 de mayo de 2019, 06:32 PM
 */


#include "config_main.h"

void InitDisplay7Seg(void);     //Funci�n Inicializa Puertos D y E
void Init8LEDs(void);           //Funci�n Inicializa puerto A
void Display7Seg(float);        //Dunci�n Despliega valores en Displays 

void main(void) {
    volt = 0.0;         //Se inicializa variables
    adcin = 0;
    
    Init8LEDs();        //Inicializa puerto A
    InitDisplay7Seg();  //Inicializa puerto D y E
    
    OSCCON = 0x53;      //Oscilador interno 4 MHz
    ADCON2 = 0x94;      //ACQT = 4TAD, TAD de 1microS, Justificado derecha 
    ADCON1 = 0x00;      //Vref- = GND, Vref+ = +5V
    TRISC = 0x04;       //bit RC2 como entrada
    ANSELC = 0x04;      //RC2 como anal�gico
    ADCON0 = 0x39;      //Enciendo ADC, canal AN14
    
    GIE = 1;            //Habilita interrupciones globales
    PEIE = 1;           //Habilita interrupciones de perif�ricos
    ADIE = 1;           //Habilita interrupci�n ADC
    
    while(1){
        __delay_ms(2);              //Retardo para conversi�n
        ADCON0bits.GO=1;            //Inicia conversi�n ADC
        
        PORTA = adcin/4;            //Asigna valor adcin a puerto A
        volt = (adcin*_vin)/_base;  //Conversi�n a flotante
        Display7Seg(volt);          //Llama a funci�n desplegar en displays
    }
    return;
}

void interrupt adc() {                //Interrupci�n conversi�n                       
    if(ADIF==1) {                     //verifica bandera conversi�n ADC
        ADIE=0;                       //Deshabilita interrupci�n ADC
        adcin = (ADRESH<<8)+ADRESL;   //Asigna valor ADRESH y ADRESL a adcin
    }
    ADIE=1;                           //Habilita interrupci�n ADC
    ADIF=0;                           //Limpia bandera conversi�n ADC
}

void InitDisplay7Seg(void)
{
    LATD = 0x00;
    TRISD = 0x00;       //Todos como salida digital
    ANSELD = 0x00;
    LATE = 0x00;
    TRISE = 0x00;       //Todos como salida digital
    ANSELE = 0x00;
    
    return;
}

void Init8LEDs(void)
{
    LATA = 0x00;
    TRISA = 0x00;       //Todos como salida digital
    ANSELA = 0x00;
    
    return;
}

void Display7Seg(float volt)
{
    unsigned char v_seg[10]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x73};
    int uni, dec, cen;              // Variables
    
    cen = ((int)volt)/1;            //Se obtiene centena
    dec = ((int)((volt-cen)*10));   //Se obtiene decena
    uni = ((int)(volt*100))%10;     //Se obtiene unidad
    
    LATD = v_seg[uni];              //asigna valor Display a puerto D
    LATE = 0x01;                    //Enciende display 
    __delay_ms(_mSec);              //Retardo
    
    LATE = 0x00;                    //Apaga display
    LATD = v_seg[dec];              //asigna valor Display a puerto D
    LATE = 0x02;                    //Enciende display 
    __delay_ms(_mSec);              //Retardo
    
    LATE = 0x00;                    //Apaga display
    LATD = v_seg[cen] + 0x80;       //asigna valor con punto Display a puerto D
    LATE = 0x04;                    //Enciende display 
    __delay_ms(_mSec);              //Retardo
    
    LATE = 0x00;                    //Apaga display
    
    return;
}