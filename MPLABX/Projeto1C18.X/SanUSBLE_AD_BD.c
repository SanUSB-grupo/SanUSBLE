#include "SanUSB48.h"
#include "bt_usb.h"//Aplicativo: https://play.google.com/store/apps/details?id=p.b.le
/*Vídeos BLE (Bluetooth Low Energy): https://www.youtube.com/watch?v=a-e88G_YEsI https://www.youtube.com/watch?v=Yi4e-Pt79EA , http://youtu.be/k4-Onma7Xgs e https://www.youtube.com/watch?v=ZRChNz9JO2M
Baixar MPLABX C18 e outros: https://drive.google.com/?tab=wo&authuser=0#folders/0B5332OAhnMe2STE4MFNBQ2dqYWM
https://dl.dropboxusercontent.com/u/101922388/ProjSanUSB1_MPLABX/Projeto1C18.X.zip
https://dl.dropboxusercontent.com/u/101922388/ApostilaSanUSB/Apostila_SanUSB_MPLABX.pdf
Gerenciador.hex de gravação USB em: https://dl.dropboxusercontent.com/u/101922388/121007SanUSBOrig/Gerenciador/Gerenciador.hex
Este firmware envia os dados com sendnum que podem ser visualizados em:
https://docs.google.com/spreadsheets/d/1tDX1NalMiKmKVawsHfS7hpAf7lB5ZeHpYu0nLmau-TE/edit#gid=1687639411
 */
 
#define pinored pin_c2 //pinos para serem conectados ao led RGB
#define pinoblue pin_c1
#define pinogreen pin_c0

unsigned char valor,valorbcd, endereco, numquant=0, temp=0;
unsigned char  n=0, m=0, ind;
short int envia=0;
unsigned int i=0, w=0, time, temperatura;
unsigned long int incrementa=0;

short int  flag=0, flagstart=0, flagNM=0, vorbei=0;
unsigned char comando[30], comand[30], c1=0;

#pragma interrupt interrupcao
void interrupcao(){

    if (INTCONbits.TMR0IF)     {  //espera o estouro do timer0 -> TMR0L=0
    INTCONbits.TMR0IF = 0;        //limpa a flag de interrupção
    i++; if(i>255){i=0;}
    if(i>=comand[1]){nivel_alto(pinored);} else {nivel_baixo(pinored);}
    if(i>=comand[2]){nivel_alto(pinogreen);} else {nivel_baixo(pinogreen);}
    if(i>=comand[3]){nivel_alto(pinoblue);} else {nivel_baixo(pinoblue);}

    TMR0L = TMR0L + 16; // load time before plus 240us/12(48MHz) = 10us x 4 (prescaler 001) = 80us -> T = 80us * 255 =~  20ms
                                }

if (serial_interrompeu)  { INTCONbits.TMR0IE = 0;//Desabilita interupção timer 0
     serial_interrompeu=0;
     comando[n] = le_serial();
     if (comando[n]==79) {flagb=1;n=0;}

         if (m>13 && comando[0]==65) {m=n; flag=1;}

         if (comando[n]=='R'){n=0;comando[0] = 'R';} //UTILIZAR VALORES DECIMAIS EM DOIS DIGITOS. ex:06, 23, 15, etc.

         if (n>=3 && comando[0]=='R')  {
                         comand[1]= comando[1]; //Led RGB anodo comum//
                         comand[2]= comando[2];
                         comand[3]= comando[3];
                         n=0;  comando[0] = 32;
                         inverte_saida(pin_b7); INTCONbits.TMR0IE = 1;
                                        }
            ++n; m=n; if(n>=29){n=0;}

            switch (comando[0]){
            case  79 : {flag=0; chegou=1;m=0;}

            case 'L' : {flag=0; n=0; c=1; nivel_alto(pin_b7); envia=1;} //habilita envio online via APK SanUSBLE
            break;
            case 'D' : {flag=0; n=0; nivel_baixo(pin_b7); envia=0;} //Desabilita envio
            break;
            case 'P': { flag=0; n=0; nivel_alto(pin_b7);envia=0;} //Desabilita envio
            break;
            case 'T': {flag=0; n=0; inverte_saida(pin_b7); envia=0;} //Desabilita envio
            break;
                               }
                            }
}

void main(){

    clock_int_48MHz();
    habilita_interrupcao(recep_serial);
    taxa_serial(9600);
    habilita_canal_AD(AN0_a_AN1);
    bt_ini();

    T0CON =0B11000001; //TMR0ON, 8 bits, Prescaler 1:4 (001 - see datasheet)
    TMR0L = 16; //conta 240us antes de estourar x 4 (prescaler)
    RCONbits.IPEN = 1;    //apenas interrupções de alta prioridade
    INTCONbits.GIEH = 1;  //Habilita interrupções de alta prioridade
    INTCONbits.TMR0IE = 1;//Habilita interupção timer 0

    while(1){
    if (entrada_pin_e3==0){Reset();} //com interrupcao evitar no while, pois pode dar conflito com a gravacao
       if (envia){
       inverte_saida(pin_b7);tempo_ms (500);
       sendrw((rom char *)"https://docs.google.com/forms/d/1PZOqjnitER0m03Ix4r9gDBqhp7Xs1YrPmjLymE2VWAU/formResponse?ifq&entry.962023089=");
       sendnum(le_AD10bits(0));
       sendrw((rom char *)"&entry.1468266733=");
       temperatura=(420*le_AD10bits(1))/1023;
       sendnum(temperatura);
       sendrw((rom char *)"&entry.1609904957=");
       sendnum(41);
       sendrw((rom char *)"&entry.1589284333=");
       sendnum(incrementa);
       sendrw((rom char *)"&submit=Submit*");
        for (i=0;i<5;i++)
              {if (envia){
               n=0; tempo_ms(100); inverte_saida(pin_b7);}}
        // Este firmware envia os dados com sendnum que podem ser visualizados em:
        //https://docs.google.com/spreadsheets/d/1tDX1NalMiKmKVawsHfS7hpAf7lB5ZeHpYu0nLmau-TE/edit#gid=1687639411
        envia=0;
                    }

      ++incrementa;
      n=0; tempo_ms(300);
     
 
    }
}
