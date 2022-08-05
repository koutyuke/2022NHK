#include "mbed.h"
#include "PS3.h"
#include "mbed_wait_api.h"
#include "QEI.h"
#define UE 0x60
#define MIGI 0x60
#define HIDARI 0x60
#define SITA 0x60

#define ST 0
#define CC 1
#define CW 2

QEI roriUe     (D2, D3, NC, 600,QEI::X4_ENCODING);
QEI roriSita   (D4, D5, NC, 600,QEI::X4_ENCODING);
QEI roriMigi   (D6, D7, NC, 600,QEI::X4_ENCODING);
QEI roriHidari (D8, D9, NC, 600,QEI::X4_ENCODING);

PS3 ps3 (A0,A1);
I2C i2c (D14,D15);
DigitalOut sig(D13);

int select,start,ue,migi,sita,hidari,L1,R1;
char cw=0xff;
char cc=0x00;
char sb=0x80;

void getdata(void);
void send(char add,char dat);
void justStop(int pulse,char add,int before);

int main(){
    roriUe.reset();
    roriSita.reset();
    roriMigi.reset();
    roriHidari.reset();
    char cw=0xff;
    char cc=0x00;
    char sb=0x80;

    int beforeUe=ST,beforeSita=ST,beforeHidari=ST,beforeMigi=ST;

    while (true) {
        
        if(select){
            sig = 1;
        }else if(R1==1){
            send(UE,cw);
            send(SITA,cw);
            send(HIDARI,cw);
            send(MIGI,cw);
            beforeUe = CW;
            beforeMigi = CW;
            beforeSita = CW;
            beforeHidari = CW;
        }else{
            int numUe,numSita,numMigi,numHidari;
            roriUe.reset();
            roriSita.reset();
            roriMigi.reset();
            roriHidari.reset();

            wait_us(5000);

            numUe = roriUe.getPulses();
            numSita = roriSita.getPulses();
            numHidari = roriHidari.getPulses();
            numMigi = roriMigi.getPulses();

            justStop(numUe, UE, beforeUe);
            justStop(numSita, SITA, beforeSita);
            justStop(numMigi, MIGI, beforeMigi);
            justStop(numHidari, HIDARI, beforeHidari);
        }
    }
}



void getData(void){
    ue = ps3.getButtonState(PS3::ue);
    migi=ps3.getButtonState(PS3::migi);
    sita=ps3.getButtonState(PS3::sita);
    hidari=ps3.getButtonState(PS3::hidari);

    R1=ps3.getButtonState(PS3::R1);
    L1=ps3.getButtonState(PS3::L1);

    select=ps3.getSELECTState();
    start=ps3.getSTARTState();
}

void send(char add,char dat){  
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(15000);
}

void justStop(int pulse,char add,int before){
    switch(before){
        case ST:
            send(add,sb);
        break;

        case CW:
            if(pulse>=0){
                send(add,cc);
            }else{
                send(add,sb);
                before = ST;
            }
        break;

        case CC:
            if(pulse<=0){
                send(add,cw);
            }else{
                send(add,sb);
                before = ST;
            }
        break;
    }
}