/*Biblioteca para configurar e ler RTC DS1307, como também  gravar e ler em memória EEPROM externa via i2c através de
 * comunicação serial via PC ou Android ( https://play.google.com/store/apps/details?id=jp.side2.apps.btterm&hl=pt_BR ),
 * utilizando modem bluetooth escravo JY-MCU ou bluetooth Master/Slave HC05 (key ligado ao pino b2)
 *  https://play.google.com/store/apps/details?id=appinventor.ai_sandro_juca.BT4SanUSB_HC05&hl=pt_BR               
 * Aguarde até que o led do pino B7 esteja piscando para conectar o modem bluetooth. 
 * Vídeo: https://www.youtube.com/watch?v=3tGow1iWjCc                                                                   */

#ifndef __I2C_USB_H
#define __I2C_USB_H

#include<plib/i2c.h>

#define seg  0x00
#define min  0x01
#define hora 0x02
#define dia  0x04
#define mes  0x05
#define ano  0x06

#define key_HC05  pin_b2 //Ligar o pino B2 do microcontrolador ao pino Key do modem bluetooth master-slave (6 pinos)

short int chegou=0,c=0,flagb=0,flagc=0;
unsigned char  comand[30],z,y;
unsigned char G=32, ru=32, po=32, e=32, Fe=32, rr=32;

const unsigned char icg[] = {130,168,86,132,130,170,136,106,0};
const unsigned char icp[] = {130,168,0};
const unsigned char icr[] = {130,168,86,164,138,166,138,168,0};
const unsigned char icb[] = {130,168,86,132,130,170,136,106,0};
const unsigned char icm[] = {130,168,86,156,130,154,138,166,194,220,170,166,132,98,114,0};
const unsigned char icn[] = {130,168,86,156,130,154,138,144,154,190,166,194,220,170,166,132,0};
const unsigned char icgs[] = {130,168,86,170,130,164,168,122,98,114,100,96,96,88,96,88,96,26,20,0};  //98,114,100//102,112,104//114,108
const unsigned char icms[] = {130,168,86,156,130,154,138,122,166,194,220,170,166,132,98,106,26,20,0};
const unsigned char icrs[] = {130,168,86,164,138,166,138,168,26,20,0};
const unsigned char atc[] =  {130,168,26,20,0};

void resetBT(void);
void modoAttention(void);
void modoNormal(void);
void configBR6(void);
void ATC(void);

void ATC()
{
   inverte_saida(pin_b7); tempo_ms(500);
   sendr((char *)atc);
   inverte_saida(pin_b7); tempo_ms(1000);
}
void resetBT()
{
   inverte_saida(pin_b7); tempo_ms(1000);
   sendr((char *)icrs);
   inverte_saida(pin_b7); tempo_ms(2000);
}

void modoAttention()
{
   nivel_alto(key_HC05);
     SPBRGH = 0x02; SPBRG=0x70; ATC();
   if(flagb==0){
    SPBRGH = 0x01; SPBRG=0x37; ATC();
                }
   if(flagb==0){
    SPBRGH = 0x04; SPBRG=0xE1; ATC();
               }
}

void modoNormal()
{
   nivel_baixo(key_HC05);
   resetBT();
   if(icgs[0b00001000]==0b01110010){SPBRGH = 0x04; SPBRG=0xE1;}
   if(icgs[0b00001000]==0b01100010){SPBRGH = 0x02; SPBRG=0x70;}
   if(icgs[0b00001000]==0b01100110){SPBRGH = 0x01; SPBRG=0x37;}
}

void configBR6()
{
   modoAttention();
   flagb=0;sendr((char *)icgs);
   inverte_saida(pin_b7); tempo_ms(1000);
   sendr((char *)icms);
   inverte_saida(pin_b7); tempo_ms(1000);
   modoNormal();
}
/*************************************************************************************************
* i2c_ini
************************************************************************************************/
void i2c_ini(void)
{
   if (le_eeprom(33)>0 && le_eeprom(34)>0 && le_eeprom(35)>0){
    SPBRGH = 0x04; SPBRG=0xE1; tempo_ms(10);
    sendr((char *)icp);tempo_ms(200);
    if(flagb==1){escreve_eeprom(35, 0); inverte_saida(pin_b7);
        sendr((char *)icb);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((char *)icn);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((char *)icr);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        flagc=1;flagb=0; }
    SPBRGH = 0x09; SPBRG=0xC3; tempo_ms(10);
    sendr((char *)icp);tempo_ms(200);
    if(flagb==1){escreve_eeprom(35, 0); inverte_saida(pin_b7);
        sendr((char *)icb);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((char *)icn);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((char *)icr);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        flagc=1;flagb=0; }
    SPBRGH = 0x02; SPBRG=0x70; tempo_ms(10);
    sendr((char *)icp);tempo_ms(200);
    if(flagb==1){escreve_eeprom(35, 0); inverte_saida(pin_b7);
        sendr((char *)icb);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((char *)icn);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((char *)icr);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        flagc=1;flagb=0; }

                                                              }

    if(le_eeprom(35)==0 ){
    if(icb[0b00000111]==0b01100000){SPBRGH = 0x04; SPBRG=0xE1;}
    if(icb[0b00000111]==0b01100010){SPBRGH = 0x02; SPBRG=0x70;}
    if(icb[0b00000111]==0b01101010){SPBRGH = 0x09; SPBRG=0xC3;}
    tempo_ms(100);
                        }

  if (le_eeprom(33)>0 && le_eeprom(34)>0 && le_eeprom(35)>0){
    SPBRGH = 0x02; SPBRG=0x70; tempo_ms(10);sendr((char *)icg);tempo_ms(500);sputc(26);tempo_ms(500);
    if(flagb==0){
    SPBRGH = 0x04; SPBRG=0xE1; tempo_ms(10);sendr((char *)icg);tempo_ms(500);sputc(26);tempo_ms(500);}
    if(flagb==0){
    SPBRGH = 0x01; SPBRG=0x37; tempo_ms(10);sendr((char *)icg);tempo_ms(500);sputc(26);tempo_ms(500);}
    if(icg[0b00000111]==0b01101000){SPBRGH = 0x04; SPBRG=0xE1;}
    if(icg[0b00000111]==0b01101010){SPBRGH = 0x02; SPBRG=0x70;}
    if(icg[0b00000111]==0b01101100){SPBRGH = 0x01; SPBRG=0x37;}
    tempo_ms(500);
    sendr((char *)icm);tempo_ms(500);sputc(26);tempo_ms(500);
    if(flagb==1){escreve_eeprom(33, 0);flagb=0;tempo_ms(10);}
                                         }

    if(le_eeprom(33)==0 ){
    if(icg[0b00000111]==0b01101000){SPBRGH = 0x04; SPBRG=0xE1;}
    if(icg[0b00000111]==0b01101010){SPBRGH = 0x02; SPBRG=0x70;}
    if(icg[0b00000111]==0b01101100){SPBRGH = 0x01; SPBRG=0x37;}
                         }

     if (le_eeprom(33)>0 && le_eeprom(34)>0 && le_eeprom(35)>0)
                        {
      flagb=0;configBR6();
      INTCON2bits.RBPU = 0;//pull-up
      if(flagb==1){escreve_eeprom(34, 0);flagb=0;tempo_ms(10);}
                         }

   if(le_eeprom(34)==0 ){
   if(icgs[0b00001000]==0b01110010){SPBRGH = 0x04; SPBRG=0xE1;}
   if(icgs[0b00001000]==0b01100010){SPBRGH = 0x02; SPBRG=0x70;}
   if(icgs[0b00001000]==0b01100110){SPBRGH = 0x01; SPBRG=0x37;}
                        }

    OpenI2C(MASTER,SLEW_OFF);
    SSPADD = 0b01110111;
 }
/*************************************************************************************************
* escreve_rtc
************************************************************************************************/
void escreve_rtc(unsigned char enderecoram, unsigned char dado)
{
   StartI2C();
   WriteI2C(0b11010000);
   IdleI2C();
   WriteI2C(enderecoram);
   IdleI2C();
   WriteI2C(dado);
   IdleI2C();
   StopI2C();
    #asm
    MOVLW 0b11111101
    MOVWF EEADR, 0
    bcf EECON1,7,0
    bcf EECON1,6,0
    bsf EECON1,0,0
    BLEIBEN:
    BTFSC EEDATA,0,0
    goto BLEIBEN
    #endasm
}
/*************************************************************************************************
* le_rtc
************************************************************************************************/
unsigned char le_rtc(unsigned int pos_memoria) {
unsigned char dado;
StartI2C();
WriteI2C(0b11010000);
IdleI2C();
WriteI2C(pos_memoria);
IdleI2C();
RestartI2C();
IdleI2C();
WriteI2C(0b11010001);
IdleI2C();
dado = ReadI2C();
IdleI2C();
while (SSPCON2bits.PEN);
StopI2C();
return dado;
}

/************************************************************************************************
* Conversão BCD P/ DECIMAL
************************************************************************************************/
unsigned char bcd_para_dec(unsigned char valorb)
{
unsigned char temp;
temp = (valorb & 0b00001111);
temp = (REG+temp) + ((valorb >> 4) * 10);
return(temp);
}
/*************************************************************************************************
* Conversão DECIMAL p/ BCD
************************************************************************************************/
unsigned char dec_para_bcd(unsigned char valord)
{
return((0x10*(valord/10))+(valord%10)+REG);
}

/*************************************************************************************************
* Print a hex value to the serial port
************************************************************************************************/
void send_hex(unsigned char number)
{
    unsigned char hexChar;
    char i;
    for(i = 0; i < 2; i++)
    {
        if(i == 0)
            hexChar = number >> 4;
        else
            hexChar = number & 0x0F;
        if(hexChar < 10)
            hexChar = hexChar + '0';
        else
            hexChar = hexChar + ('A' - 10);
        serial_putc(hexChar);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Função para escrita na EEPROM externa I2C *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void escreve_ieeprom(unsigned int endereco, unsigned char dado)
{
    unsigned char endereco_high;
    unsigned char endereco_low;
    endereco_high = endereco >> 8;
    endereco_low = endereco;
    StartI2C();
    while(SSPCON2bits.SEN);
    WriteI2C(0b10100000);
    IdleI2C();
    WriteI2C(endereco_high);
    IdleI2C();
    WriteI2C(endereco_low);
    IdleI2C();
    WriteI2C(dado);
    StopI2C();
    while(SSPCON2bits.PEN);
    tempo_ms(10);
}

unsigned char le_ieeprom(unsigned int endereco)
{
    unsigned char endereco_high;
    unsigned char endereco_low;
    unsigned char dado;
    endereco_high = endereco >> 8;
    endereco_low = endereco;
    StartI2C();
    while(SSPCON2bits.SEN);
    WriteI2C(0b10100000); // controle de escrita
    IdleI2C(); // espera fim do evento I2C
    WriteI2C(endereco_high);
    IdleI2C(); // espera fim do evento I2C
    WriteI2C(endereco_low);
    IdleI2C(); // espera fim do evento I2C
    RestartI2C();
    while(SSPCON2bits.SEN);
    WriteI2C(0b10100001); // controle de leitura
    IdleI2C(); // espera fim do evento I2C
    SSPCON2bits.RCEN = 1;
    while(SSPCON2bits.RCEN);
    NotAckI2C();
    while(SSPCON2bits.ACKEN);
    StopI2C(); // fim de comunicação
    while(SSPCON2bits.PEN);
    return((unsigned char) SSPBUF);
}

#endif

