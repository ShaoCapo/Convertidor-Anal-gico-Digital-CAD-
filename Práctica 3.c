#include <xc.h>
#include <stdio.h>

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000 // necessary for __delay_us

char cont = 0;
char adcL;
int adcH;
int next = 0;

void init_adc(void) {
    /*
     * ADCON0 = 0b10000001;
     * ADCON1 = 0b10000000;
     */
    
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.CHS = 0b0000; // AN0
	
    /* 
	   0 = conversion A/D finalizada
       1 = conversion A/D en curso, se finaliza por HW
     */
    ADCON0bits.GO_DONE = 0;
    ADCON0bits.ADON = 1; // ADC habilitado
    
    ADCON1bits.ADFM = 1;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
}

void init_TMR0()
{
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b111; // 1:256
    TMR0 = 158;
}

void __interrupt() INT_TMR0(void)
{
    if(INTCONbits.T0IF){
        TMR0 = 158;
        cont++;
        if(cont == 100){ // para los 500ms
            cont = 0;
            ADCON0bits.GO = 1; // EMPIEZA CONVERSION
        }
        INTCONbits.T0IF = 0;
    }
    
    // 1 = A/D conversion complete (must be cleared in software)
    if(PIR1bits.ADIF){
        adcH = ADRESH;
        adcL = ADRESL;
        adcH = adcH << 8 | adcL;
        
        PORTB = adcL;
        next = 1;
    }
}

void init_uart(void)
{  // See chapter 12.3 PIC16F886 manual (table 12-5) 

  TXSTAbits.BRGH = 0;
  BAUDCTLbits.BRG16 = 0;

  SPBRGH = 0b00000000;
  SPBRG = 0b00100000;  // 9600 baud rate with 20MHz Clock
  
  TXSTAbits.SYNC = 0; // Asynchronous
  TXSTAbits.TX9 = 0; // TX 8 data bit
  RCSTAbits.RX9 = 0; // RX 8 data bit

  PIE1bits.TXIE = 0; // Disable TX interrupt 
  PIE1bits.RCIE = 0; // Disable RX interrupt 

  RCSTAbits.SPEN = 1; // Serial port enable

  TXSTAbits.TXEN = 0; // Reset transmitter  
  TXSTAbits.TXEN = 1; // Enable transmitter
  
 }

// It is needed for printf
void putch(char c){ 
    while (!TXSTAbits.TRMT);
    TXREG = c;
 } 

void main(void) {
    OSCCON = 0b00001000; // External cristal
    init_uart();
    init_adc();
    
    init_TMR0();
    INTCONbits.GIE = 1; // sino esta a 1 NO hay interrupciones
    INTCONbits.T0IE = 1; // igual
    
    // Digital I/O
    ANSEL = 0;
    ANSELH = 0;
    
    // Output
    TRISB = 0;
    
    while(1){
        while(next){
            next = 0;
            printf("%d\r\n", adcH);
        }
    }
}
