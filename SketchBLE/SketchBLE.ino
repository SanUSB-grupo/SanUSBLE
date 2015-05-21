/********Sketch Arduino*********************************************
 Grupo: www.tinyurl.com/SanUSB 
 video-aulas: www.youtube.com/SanUSB2

 Funciona para todos os APKs android disponiveis em :https://play.google.com/store pesquisar por SanUSB.
 Utiliza modem bluetooth slave (HM-10, Linvor v.104 ou Linvor v.106, HC-04, HC-06, JY-MCU, etc.)

/***************************************************************************
//Aplicativo: https://play.google.com/store/apps/details?id=p.b.le
/*Vídeos BLE (Bluetooth Low Energy):https://www.youtube.com/watch?v=a-e88G_YEsI https://www.youtube.com/watch?v=Yi4e-Pt79EA , http://youtu.be/k4-Onma7Xgs e https://www.youtube.com/watch?v=ZRChNz9JO2M
https://dl.dropboxusercontent.com/u/101922388/ProjSanUSB1_MPLABX/Projeto1C18.X.zip
https://dl.dropboxusercontent.com/u/101922388/ApostilaSanUSB/Apostila_SanUSB_MPLABX.pdf
Este firmware envia os dados com sendnum que podem ser visualizados em:
https://docs.google.com/spreadsheets/d/1tDX1NalMiKmKVawsHfS7hpAf7lB5ZeHpYu0nLmau-TE/edit#gid=1687639411
**********************************************************************************
 Mercado Livre: http://lista.mercadolivre.com.br/sanusb

Ligacoes dos pinos do modem bluetooth (desconectar o modem BLE do pinos 0 (Rx) da serial para gravar o microcontrolador (upload) 
e depois de gravar, conectar novamente o pinos Rx do modem no pino 0 do arduino:
Bluetooth--------Arduino 
RX--------------- PINO 1 (Tx)
TX----------------PINO 0 (Rx) (com Resistor de pull-up para Vcc)
GND---------------GND
VCC---------------5V
Quando o Led no pino 13 estiver piscando constantemente, significa que o circuito e o programa foram executados com sucesso.
Resetar o microcontrolador antes de conectar ao Android*/
#include <EEPROM.h>

#define pinored 9     //3   //pinos para serem conectados ao led RGB
#define pinoblue 10   //5
#define pinogreen 11  //6

   unsigned char icg[] = {130,168,86,132,130,170,136,106,0};
   unsigned char icp[] = {130,168,0};
   unsigned char icr[] = {130,168,86,164,138,166,138,168,0};
   unsigned char icb[] = {130,168,86,132,130,170,136,106,0}; 
   unsigned char icm[] = {130,168,86,156,130,154,138,166,194,220,170,166,132,98,106,0};
   unsigned char icn[] = {130,168,86,156,130,154,138,144,154,190,166,194,220,170,166,132,0};
   unsigned char icgs[] = {130,168,86,170,130,164,168,122,98,114,100,96,96,88,96,88,96,26,20,0};  
   unsigned char icms[] = {130,168,86,156,130,154,138,122,166,194,220,170,166,132,98,106,26,20,0};
   unsigned char icrs[] = {130,168,86,164,138,166,138,168,26,20,0};
   unsigned char atc[] =  {130,168,26,20,0};

short int pisca=0, chegou=0, flag=0, c=0, flagRGB=0, led = 0, flagstart=0, flagBD=0,envia=0, AT1=0;
unsigned char comando[30], comand[30], n=0, m=0, k=0, c1=0, i=0;
int redvalue=0, greenvalue=0, bluevalue=0, incrementa=0, ADvalue;
unsigned char S=130>>1, a=168>>1, an=86>>1, u=132>>1,s=130>>1, b=170>>1,x=136>>1,z=106>>1, BD=4;

void setup()  
{
  pinMode(13, OUTPUT); 
  taxa_serial(9600);  
  pinMode(pinored, OUTPUT);
  pinMode(pinogreen, OUTPUT);
  pinMode(pinoblue, OUTPUT);
   for(m=0;m<=25;m++){comando[m]=32;}
  for(m=0;m<=25;m++){comand[m]=32;}  
 
    m=EEPROM.read(250); 
   if(m<=12 || m==255){EEPROM.write(251, 0); } 
   delay(5000); //Tempo para colocar o fio Rx após a gravação do Arduino
   bt_ini();
}

void sends (unsigned char st[]) 
        {for(k=0;st[k]!='\0';k++){sputc(st[k]);}
        }
        
void sendsw (char st[]) 
        {for(k=0;st[k]!='\0';k++){swputc(st[k]);}
        }
 void swputc(char c) 
{
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0=UBRR0H+c;
}

void sputc(unsigned char c)
{
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0=c>>(UCSZ00+UBRR0H); 
}

ISR(USART_RX_vect)
{
  char ReceivedChar;
  ReceivedChar  = UDR0; 
  comando[n]=ReceivedChar; comand[n]= comando[n]&0x55; 

  if ((comando[0]!='L' || comando[0]!=79) && flagstart==0) {flagstart=1; flagBD=1; n=0;}

  if (comando[0]!=79) {chegou=0; pisca=0;}

  if (n>=13 && comando[1]==84) {flag=1;m=n;} 
++n; 

switch (comando[0])
	{
   case  79 : { chegou=1; AT1=1; flag=0; m=0;} 
   break;
   case 'R' : { if (n>=4)  {analogWrite(pinored, 255-comando[1]); //Led RGB anodo comum//
                            analogWrite(pinogreen, 255-comando[2]);
                            analogWrite(pinoblue, 255-comando[3]); n=0; comando[0]=32;//Apaga R 
                            led = !led; digitalWrite(13, led);}}
                            
   /*   case 'R' : { if (n>=4)  {analogWrite(pinored, comando[1]); //Led RGB catodo comum//
                            analogWrite(pinogreen, comando[2]);
                            analogWrite(pinoblue, comando[3]); n=0; comando[0]=32;//Apaga R 
                            led = !led; digitalWrite(13, led);}}
   */
   break;
   case 'L' : {flag=0; n=0; c=1; flagBD=0; digitalWrite(13, HIGH);envia=1;} //habilita envio online via APK SanUSBLE
   break;
   case 'D' : {flag=0; n=0; digitalWrite(13, LOW);envia=0;}//Desabilita envio
   break;
   case 'P': { flag=0; n=0;digitalWrite(13, HIGH);envia=0; } //Desabilita envio
   break; 
   case 'T': { flag=0; n=0;led = !led; digitalWrite(13, led); envia=0;} //Desabilita envio
   break;

          }
				
}

void sendnum(unsigned int sannum)
{
   if(sannum > 9999) {
        swputc(((sannum / 10000) % 10)+UBRR0H+'0'); 
    }
   if(sannum > 999) {
        swputc(((sannum / 1000) % 10)+UBRR0H+'0');
    }
    if(sannum > 99) {
        swputc(((sannum / 100) % 10)+UBRR0H+'0');
    }
    if(sannum > 9) {
        swputc(((sannum / 10) % 10)+UBRR0H+'0');
    }
    swputc((sannum % 10)+UBRR0H+'0') ;
}
 
void taxa_serial(unsigned long baud)
{
  unsigned int baud_sanusb = (F_CPU / 16 / baud) - 1;
  UBRR0H = (unsigned char)(baud_sanusb >> 8);
  UBRR0L = (unsigned char)baud_sanusb;
  UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
  UCSR0B |= _BV(RXCIE0);
  UCSR0B &= ~_BV(TXCIE0);
  UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}

void loop() {
    
       if (envia) { ADvalue=analogRead(A0);
                   led= !led; digitalWrite(13, led); delay(500);
                   sendsw("https://docs.google.com/forms/d/1PZOqjnitER0m03Ix4r9gDBqhp7Xs1YrPmjLymE2VWAU/formResponse?ifq&entry.962023089=");
                   sendnum(ADvalue);
                   sendsw("&entry.1468266733=");
                   sendnum(87);
                   sendsw("&entry.1609904957=");
                   sendnum(41);
                   sendsw("&entry.1589284333=");
                   sendnum(incrementa);
                   sendsw("&submit=Submit*");
                   for (i=0;i<5;i++) 
                      {if (envia){
                       n=0; delay(1000); led= !led; digitalWrite(13, led);}}
        // Este firmware envia os dados com sendnum que podem ser visualizados em:
        //https://docs.google.com/spreadsheets/d/1tDX1NalMiKmKVawsHfS7hpAf7lB5ZeHpYu0nLmau-TE/edit#gid=1687639411
                  }
      ++incrementa; 
      n=0; delay (1000);
              
            }
     
     void bt_ini(void){
     delay(10);
    if(EEPROM.read(255)==0 || EEPROM.read(255)==255){
    UBRR0L =0x67; 
    for (i=0;i<3;i++) {delay(200);sends(icp);}
    delay(500); led= !led; digitalWrite(13, led);
    if(AT1==1){EEPROM.write(255, 1); led= !led; digitalWrite(13, led);
        sends(icb);delay(100);led= !led; digitalWrite(13, led);
        delay(100);led= !led; digitalWrite(13, led);
        sends(icn);delay(100);led= !led; digitalWrite(13, led);
        delay(100);led= !led; digitalWrite(13, led);
        sends(icr);delay(100);led= !led; digitalWrite(13, led);
        delay(1000);led= !led; digitalWrite(13, led);
         n=0;AT1=0;}
      
    UBRR0L =0xCF; 
    for (i=0;i<3;i++) {delay(200);sends(icp);}
    delay(500); led= !led; digitalWrite(13, led);
    if(AT1==1){EEPROM.write(255, 1); led= !led; digitalWrite(13, led);
        sends(icb);delay(100);led= !led; digitalWrite(13, led);
        delay(100);led= !led; digitalWrite(13, led);
        sends(icn);delay(100);led= !led; digitalWrite(13, led);
        delay(100);led= !led; digitalWrite(13, led);
        sends(icr);delay(100);led= !led; digitalWrite(13, led);
        delay(100);led= !led; digitalWrite(13, led);
        //for (i=0;i<20;i++) {n=0; delay(100); led= !led; digitalWrite(13, led);}
        n=0;AT1=0;}
        
    UBRR0L =0x33; 
    for (i=0;i<3;i++) {delay(200);sends(icp);}
    delay(200); led= !led; digitalWrite(13, led);
    if(AT1==1){EEPROM.write(255, 1); led= !led; digitalWrite(13, led);
        sends(icb);delay(100);led= !led; digitalWrite(13, led);
        delay(100);led= !led; digitalWrite(13, led);
        sends(icn);delay(100);led= !led; digitalWrite(13, led);
        delay(100);led= !led; digitalWrite(13, led);
        sends(icr);delay(100);led= !led; digitalWrite(13, led);
        delay(100);led= !led; digitalWrite(13, led);
        n=0;AT1=0;}
               }
    if(icb[0b00000111]==0b01100000){UBRR0L =0x67;}
    if(icb[0b00000111]==0b01100010){UBRR0L =0x33;}
    if(icb[0b00000111]==0b01101010){UBRR0L =0xCF;}
    delay(100);
            
          }
