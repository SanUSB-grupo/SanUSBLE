#include "SanUSB48X.h" // Firmware para configuração e leitura por hardware de EEPROM i2c e de relógio DS1307 (BCD)
#include"i2c_usb.h"//Biblioteca de funcoes I2C com a placa SanUSB, onde RB0(SDA) e RB1(SCL)
//https://dl.dropboxusercontent.com/u/101922388/ProjSanUSB1_MPLABX/Projeto1C18.X.zip
//https://dl.dropboxusercontent.com/u/101922388/ApostilaSanUSB/Apostila_SanUSB_MPLABX.pdf
//Vídeo: https://www.youtube.com/watch?v=F5S4thVGOo8 e em https://www.youtube.com/edit?o=U&video_id=_IqGlj17x5A

unsigned char valor,valorbcd, endereco, numquant=0, temp=0;
unsigned char  comando[6], n=0, m=0, ind;
short int flagA4=0, flagA5=0;
unsigned long int resultado, tensao_lida16;

unsigned int i,j,endereco16=0, posicao=0, valorgravado;
unsigned char byte1,byte2,xis; // 2 Partes do valor da tensao_lida16

unsigned int conv_dec_2bytes(unsigned int valor16) 
{//Função auxiliar para alocar um valor de 16 bits (até 65535) em 2 bytes
    byte1= valor16%256; byte2 = valor16/256; //o que for resto (%) é menos significativo
    return(byte2,byte1);
}
//#pragma interrupt interrupcao
void interrupt interrupcao(){

if (serial_interrompeu)  {
     serial_interrompeu=0;
     comando[n] = le_serial(); 
     if (comando[n]==79) {flagb=1;}
     switch (comando[n]){
            case 'L' : { nivel_alto(pin_b7);}
            break;
            case 'D' : { nivel_baixo(pin_b7);} //Desabilita envio
            break;
            case 'P': {inverte_saida(pin_b7);} //Desabilita envio
            break;
            case 'T': {inverte_saida(pin_b7);} //Desabilita envio
            break;
                        }
     //////////FUNCAO 4: CONFIGURA RELÓGIO////////////////////Ex: Digitar A4H09 (Hora = 09) , A4D15 (Dia = 15).
     if (comando[n]=='A'){n=0;comando[0] = 'A';} //UTILIZAR VALORES DECIMAIS EM DOIS DIGITOS. ex:06, 23, 15, etc.

         if ( comando[1]== '4' && comando[2]== 'H' && n==2) { endereco=2;} //Escreve o endereco das horas
         if ( comando[1]== '4' && comando[2]== 'M' && n==2) { endereco=1;} //Escreve o endereco dos minutos
         if ( comando[1]== '4' && comando[2]== 'S' && n==2) { endereco=0;} //Escreve o endereco dos segundos
         if ( comando[1]== '4' && comando[2]== 'D' && n==2) { endereco=4;} //Escreve o endereco do dia
         if ( comando[1]== '4' && comando[2]== 'N' && n==2) { endereco=5;} //Escreve o endereco do mes
         if ( comando[1]== '4' && comando[2]== 'Y' && n==2) { endereco=6;} //Escreve o endereco do ano
         
         if ( comando[1]== '4' && comando[3]>='0'&&comando[3]<='9'&& n==3) {numquant=(comando[3]-0x30);}
         if ( comando[1]== '4' && comando[4]>='0'&&comando[4]<='9'&& n==4) {numquant=numquant*10+(comando[4]-0x30);                                                                         
                                                                            flagA4=1;
                                                                            }

     //////////////*////////FUNCAO 5: LÊ RELÓGIO////////////////////////////Ex: A5- Lê o relógio e o calendário
         if (comando[1]== '5' && n==1){flagA5=1;}

         ++n; if(n>=5){n=0;}
   }
}

void main(){

    clock_int_48MHz();
    habilita_interrupcao(recep_serial);
    habilita_canal_AD(AN0);
    taxa_serial(9600);
    i2c_ini();

    while(1){

        if (flagA4){ flagA4=0; //Comandos A4 para Configurar o RTC
                escreve_rtc(endereco, dec_para_bcd(numquant)); //Escrever em BCD no RTC
                send_hex(le_rtc(hora)); swputc (':');     //Envia resultado via serial por bluetooth ou qualquer outro modem.
                send_hex(le_rtc(min));  swputc (':');     //Exemplo de resposta: 18:49:37 19/04/14
                send_hex(le_rtc(seg));  swputc (' ');
                send_hex(le_rtc(dia));  swputc ('/');
                send_hex(le_rtc(mes));  swputc ('/');
                send_hex(le_rtc(ano));  swputc(' ');
                    }

        if (flagA5){ flagA5=0; //BCD em hexadecimal representa o decimal
                send_hex(le_rtc(hora)); swputc (':');
                send_hex(le_rtc(min));  swputc (':');
                send_hex(le_rtc(seg));  swputc (' ');
                send_hex(le_rtc(dia));  swputc ('/');
                send_hex(le_rtc(mes));  swputc ('/');
                send_hex(le_rtc(ano));  swputc(' ');
                      }

        resultado = le_AD10bits(0);
        tensao_lida16 = (resultado * 5000)/1023; //Valor até 16 bits (2 bytes)
       // sendnum(tensao_lida16); swputc(' ');
      
        //***************************************************************************
        conv_dec_2bytes(tensao_lida16);
        posicao=2*endereco16; //endereço é o ponteiro de 16 bits (byte 1 e byte 2)
        escreve_ieeprom( posicao, byte2); //Byte mais significativo do int16
        escreve_ieeprom( posicao+1, byte1 ); //byte menos significativo do int16
        ++endereco16; if (endereco16>=150){endereco16=0;} //Buffer de 300 bytes posicao<300
        //***************************************************************************/

        if(entrada_pin_e3==0){
       sendnum(le_eeprom(5)); swputc(' ');

         //***************LEITURA DO BUFFER DA EEPROM EXTERNA I2C********************
                for(i=0; i<10; ++i) { //150 Valores de 16 bits ou 300 de 8 bits.
                    for(j=0; j<15; ++j) {
                        valorgravado= 256*le_ieeprom((i*30)+2*j) + le_ieeprom((i*30)+2*j+1);
                        sendnum(valorgravado); swputc(' ');
                                        }
                        sendrw((char *)"\n\r");
                                    }
                        sendrw((char *)"\n\r");
    //***************************************************************************/
                              }

        //++ind; if(ind>255) {ind=0;}
        //escreve_eeprom(5, ind);
        //inverte_saida(pin_b7);
        tempo_ms(1000);
             }
}

