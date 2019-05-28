/*
 * File:   main.c
 * Author: big_m
 *
 * Created on 25 de mayo de 2019, 06:32 PM
 */

// CONFIG1L
#pragma config PLLSEL = PLL4X   // PLL Selection (4x clock multiplier)
#pragma config CFGPLLEN = OFF   // PLL Enable Configuration bit (PLL Disabled (firmware controlled))
#pragma config CPUDIV = NOCLKDIV// CPU System Clock Postscaler (CPU uses system clock (no divide))
#pragma config LS48MHZ = SYS24X4// Low Speed USB mode with 48 MHz system clock (System clock at 24 MHz, USB clock divider is set to 4)

// CONFIG1H
#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config PCLKEN = ON     // Primary Oscillator Shutdown (Primary oscillator shutdown firmware controlled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF      // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config nPWRTEN = OFF    // Power-up Timer Enable (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable (BOR enabled in hardware (SBOREN is ignored))
#pragma config BORV = 190       // Brown-out Reset Voltage (BOR set to 1.9V nominal)
#pragma config nLPBOR = OFF     // Low-Power Brown-out Reset (Low-Power Brown-out Reset disabled)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored))
#pragma config WDTPS = 32768    // Watchdog Timer Postscaler (1:32768)

// CONFIG3H
#pragma config CCP2MX = RC1     // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config T3CMX = RC0      // Timer3 Clock Input MUX bit (T3CKI function is on RC0)
#pragma config SDOMX = RB3      // SDO Output MUX bit (SDO function is on RB3)
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)

// CONFIG4L
#pragma config STVREN = OFF      // Stack Full/Underflow Reset (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Block 0 Code Protect (Block 0 is not code-protected)
#pragma config CP1 = OFF        // Block 1 Code Protect (Block 1 is not code-protected)
#pragma config CP2 = OFF        // Block 2 Code Protect (Block 2 is not code-protected)
#pragma config CP3 = OFF        // Block 3 Code Protect (Block 3 is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protect (Boot block is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protect (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Block 0 Write Protect (Block 0 (0800-1FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Block 1 Write Protect (Block 1 (2000-3FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Block 2 Write Protect (Block 2 (04000-5FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Block 3 Write Protect (Block 3 (06000-7FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Registers Write Protect (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protect (Boot block (0000-7FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protect (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Block 0 Table Read Protect (Block 0 is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Block 1 Table Read Protect (Block 1 is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Block 2 Table Read Protect (Block 2 is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Block 3 Table Read Protect (Block 3 is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protect (Boot block is not protected from table reads executed in other blocks)


#include "config_main.h"

void InitDisplay7Seg(void);     //Función Inicializa Puertos D y E
void Init8LEDs(void);           //Función Inicializa puerto A
void Display7Seg(float);        //Dunción Despliega valores en Displays 
int interruptadc(int);

void main(void) {
    volt = 0.0;         //Se inicializa variables
    adcin = 0;
    
    int out;
    
    Init8LEDs();        //Inicializa puerto A
    InitDisplay7Seg();  //Inicializa puerto D y E
    
    OSCCON = 0x53;      //Oscilador interno 4 MHz
    ADCON2 = 0x94;      //ACQT = 4TAD, TAD de 1microS, Justificado derecha 
    ADCON1 = 0x00;      //Vref- = GND, Vref+ = +5V
    LATA = 0x00;        //Limpiamos A
    TRISA = 0x00;       //bit RA1 como salida 
    ANSELA = 0x00;      //RA1 como analógico
    TRISC = 0x04;       //bit RC2 como entrada
    ANSELC = 0x04;      //RC2 como analógico
    ADCON0 = 0x39;      //Enciendo ADC, canal AN14
    
    GIE = 1;            //Habilita interrupciones globales
    PEIE = 1;           //Habilita interrupciones de periféricos
    ADIE = 1;           //Habilita interrupción ADC
    
    while(1){
     //   __delay_ms(1);              //Retardo para conversión
     ADCON0bits.GO=1;            //Inicia conversión ADC
     adcin = interruptadc(adcin);
        
        
        PORTA = adcin/4;            //Asigna valor adcin a puerto A
        //volt = (adcin*_vin)/_base;  //Conversión a flotante
        //Display7Seg(volt);          //Llama a función desplegar en displays
        //LA1=out;
    }
    return;
}

int interruptadc(int adcin) {                //Interrupción conversión                       
    if(ADIF==1) {                     //verifica bandera conversión ADC
        ADIE=0;                       //Deshabilita interrupción ADC
        adcin = (ADRESH<<8)+ADRESL;   //Asigna valor ADRESH y ADRESL a adcin
    }
    ADIE=1;                           //Habilita interrupción ADC
    ADIF=0;                           //Limpia bandera conversión ADC
    return adcin;
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