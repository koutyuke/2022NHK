#include "mbed.h"
#include "vector"
#include "string.h"
#include <cstring>

DigitalIn data0(D8);
DigitalIn data1(D9);
DigitalIn data2(D2);
DigitalIn data3(D3);
DigitalIn data4(D4);
DigitalIn data5(D5);
DigitalIn data6(D6);
DigitalIn data7(D7);

UnbufferedSerial PC(USBTX,USBRX);

int getAngre();

int main(){

    while (true) {
        printf("%d\n",getAngre());
        wait_us(10000);
    }

    return 0;
}

int getAngre(){
    int save,value,result;
    int getData[8] = {
        data0.read()^1,
        data1.read()^1,
        data2.read()^1,
        data3.read()^1,
        data4.read()^1,
        data5.read()^1,
        data6.read()^1,
        data7.read()^1,
    };
    result = getData[7] << 7;
    save = getData[7];
    for(int i=6;i>=0;i--){
        value = save ^ getData[i];
        result = result | (value<<i);
        save = value;
    }
    return result;
}

