/*
 * File:   main.c
 * Author: Labmems
 *
 * Created on 27 de abril de 2021, 12:16 AM
 */

#include "device_config.h"
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


#define _XTAL_FREQ 8000000
#define SWEEP_STEP 1
#define LCD_DATA_R          PORTD
#define LCD_DATA_W          LATD
#define LCD_DATA_DIR        TRISD
#define LCD_RS              LATCbits.LATC2
#define LCD_RS_DIR          TRISCbits.TRISC2
#define LCD_RW              LATCbits.LATC1
#define LCD_RW_DIR          TRISCbits.TRISC1
#define LCD_E               LATCbits.LATC0
#define LCD_E_DIR           TRISCbits.TRISC0

int i=0,j=0;                       
int c1,d1,u1,c2,d2,u2,result,n1,n2;                  
double div_result=0,rc,rd,ru;                    
char key,operator,flag=0,rcc,rdc,ruc;      
char op1[3],op2[3],output[3],outdec[3];        


void send2LCD(char);
void LCD_rdy(void);
void LCD_init(void);
void LCD_cmd(char);
void portsInit(void);
char key_scanner(void);
void display_float( double);
void IntegertoString(char*,int);
    
void main(void) {
    portsInit();
    
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_E  = 0;

    LCD_init();
    
    send2LCD('L');
    send2LCD('a');
    send2LCD('b');
    send2LCD('o');
    send2LCD('r');
    send2LCD('a');
    send2LCD('t');
    send2LCD('o');
    send2LCD('r');
    send2LCD('i');
    send2LCD('o');
     __delay_ms(1000);
    
    LCD_cmd(0x01);          // Clear display and move cursor home
    
    while(1){
  
        key = key_scanner();
                
            
        if(key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9'){
            if(flag==0){
                op1[i] = key;       //Assign 1st no string to op1 stored that in array
                send2LCD(op1[i]);
                i++;
                }
            else{
                op2[j] = key;       //Assign 2nd no string to op2 stored that in array
                send2LCD(op2[j]);
                j++;
                }    
        }

        if(key=='+' || key=='-' || key=='*' || key=='/' ){
            operator=key;
            send2LCD(operator);     //Store operator
            flag=1;                        
        }

        if(key=='='){
            if(i==3){
                c1 = op1[0] - '0';
                d1 = op1[1] - '0';
                u1 = op1[2] - '0';
                n1 = (c1*100)+(d1*10)+u1;
            }
            else if(i==2){
                d1 = op1[0] - '0';
                u1 = op1[1] - '0';
                n1 = (d1*10)+u1;    
            }
            else{
                u1 = op1[0] - '0';
                n1 = u1;   
            }
            
            if(j==3){
                c2 = op2[0] - '0';
                d2 = op2[1] - '0';
                u2 = op2[2] - '0';
                n2 = (c2*100)+(d2*10)+u2;
            }
            else if(j==2){
                d2 = op2[0] - '0';
                u2 = op2[1] - '0';
                n2 = (d2*10)+u2;
            }
            else{
                u2 = op2[0] - '0';
                n2 = u2;
            }
            send2LCD('=');                  

            LCD_cmd(0xc0);          //Start displaying result on 2nd Row and 1st location
                                    
            switch(operator){   //Case for addition
                case '+':   
                    result = n1+n2;
                    
                    IntegertoString(output, result);
                    send2LCD(output[0]);
                    if((result > 9) && (result < 100)){
                        send2LCD(output[1]);
                    }
                    if(result > 99){
                        send2LCD(output[1]);
                        send2LCD(output[2]);
                    }
                    break;

                //Case for subtraction
                case '-':   
                    result = n1-n2;
                    
                    IntegertoString(output, result);
                    send2LCD(output[0]);
                    if((result > 9) && (result < 100)){
                        send2LCD(output[1]);
                    }
                    if(result > 99){
                        send2LCD(output[1]);
                        send2LCD(output[2]);
                    }
                    break;

                //Case for multiplication
                case '*':   
                    result = n1*n2;
                    
                    IntegertoString(output, result);
                    send2LCD(output[0]);
                    if((result > 9) && (result < 100)){
                        send2LCD(output[1]);
                    }
                    if(result > 99){
                        send2LCD(output[1]);
                        send2LCD(output[2]);
                    }
                    break;

                //Case for division
                case '/':
                    if(n2==0){  //Check for math error
                        send2LCD('E');
                        send2LCD('E');
                        send2LCD('r');
                        send2LCD('r');
                        send2LCD('o');
                        send2LCD('r');

                    }
                    else{   
                        div_result = (float)n1/(float)n2;

                        //Display float value
                        int integer,decimal;
                        integer = div_result; 
                        
                        IntegertoString(output, integer);
                        send2LCD(output[0]);
                        if((integer > 9) && (integer < 100)){
                            send2LCD(output[1]);
                        }
                        if(integer > 99){
                            send2LCD(output[1]);
                            send2LCD(output[2]);
                        }
                      
                        send2LCD('.');      //Decimal point
                        
                        decimal = (div_result - integer)*1000; 
                        IntegertoString(outdec, decimal);
                        send2LCD(outdec[0]);
                        if((decimal > 9) && (decimal < 100)){
                            send2LCD(outdec[1]);
                        }
                        if(decimal > 99){
                            send2LCD(outdec[1]);
                            send2LCD(outdec[2]);
                        }
                        
                    }   
                    break;

                default :   break;
            }
        } 
        
        if(key == '#'){
            LCD_cmd(0x01);          // Clear display and move cursor home    
            __delay_ms(10);
            
            i=0;
            j=0;
            memset(op1,0,sizeof(op1));          //Clear array op1 by copying 0 to op1's locations
            memset(op2,0,sizeof(op2));          //Clear array op2 by copying 0 to op2's locations    
            memset(output,0,sizeof(output));    //Clear array output by copying 0 to output's locations
            memset(outdec,0,sizeof(outdec));    //Clear array outdec by copying 0 to outdec's locations
            break;
        }
    __delay_ms(20);
    }   
}    

void send2LCD(char xy){
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_E  = 1;
    LCD_DATA_W = xy;
    Nop();
    Nop();
    LCD_E  = 0;
    __delay_ms(250);
}

void LCD_init(void){
    LATC = 0;               // Make sure LCD control port is low
    LCD_E_DIR = 0;          // Set Enable as output
    LCD_RS_DIR = 0;         // Set RS as output 
    LCD_RW_DIR = 0;         // Set R/W as output
    LCD_cmd(0x38);          // Display to 2x16
    __delay_ms(250);  
    LCD_cmd(0x0F);          // Display on, cursor on and blinking
    __delay_ms(250);  
    LCD_cmd(0x01);          // Clear display and move cursor home
    __delay_ms(250);  
}

void LCD_rdy(void){
    char test;
    // configure LCD data bus for input
    LCD_DATA_DIR = 0xFF;
    test = 0x80;
    while(test){
        LCD_RS = 0;         // select IR register
        LCD_RW = 1;         // set READ mode
        LCD_E  = 1;         // setup to clock data
        test = LCD_DATA_R;
        Nop();
        LCD_E = 0;          // complete the READ cycle
        test &= 0x80;       // check BUSY FLAG 
    }
}

void LCD_cmd(char cx) {
   //LCD_rdy();              // wait until LCD is ready
    LCD_RS = 0;             // select IR register
    LCD_RW = 0;             // set WRITE mode
    LCD_E  = 1;             // set to clock data
    Nop();
    LCD_DATA_W = cx;        // send out command
    Nop();                  // No operation (small delay to lengthen E pulse)
    LCD_E = 0;              // complete external write cycle
}

void portsInit(void){
    OSCCON = 0x74;// Set the internal oscillator to 8MHz and stable
    ANSELC = 0;   // Set port D as Digital for 7 segment anodes
    TRISC  = 0x00;      // for Port D, set all pins as outputs for 7 segment anodes
    ANSELD = 0;   // Set port D as Digital for 7 segment anodes
    TRISD  = 0x00;      // for Port D, set all pins as outputs for 7 segment anodes
    ANSELB = 0;   // Set port A as Digital for keypad driving
    TRISB  = 0x0F;      // For Port A, set pins 4 to 7 as outputs (rows), and pins 0 to 3 as inputs (cols)
    WPUB = 0x0F;        // pull-up enable for PORTB pins
    INTCON2bits.RBPU=0;
    LCD_DATA_DIR = 0x00;
}


void IntegertoString(char * string, int number) {

   if(number == 0) { string[0] = '0'; return; };
   int divide = 0;
   int modResult;
   int  length = 0;
   int isNegative = 0;
   int  copyOfNumber;
   int offset = 0;
   copyOfNumber = number;
   if( number < 0 ) {
     isNegative = 1;
     number = 0 - number;
     length++;
   }
   while(copyOfNumber != 0) 
   { 
     length++;
     copyOfNumber /= 10;
   }

   for(divide = 0; divide < length; divide++) {
     modResult = number % 10;
     number    = number / 10;
     string[length - (divide + 1)] = modResult + '0';
   }
   if(isNegative) { 
   string[0] = '-';
   }
   string[length] = '\0';
}