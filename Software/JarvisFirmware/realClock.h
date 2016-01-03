#ifndef REALCLOCK_H
#define REALCLOCK_H

#include "nodeComponent.h"
#include "RTClib.h"

RTC_DS1307 RTC;

class realClock : public nodeComponent{
public:
    realClock(int pin = -1) : nodeComponent(pin){
        m_id ="test";
        m_actions.push_back(A_READ_DATA); //READ_DATA DEVUELVE EL VALOR PREPARADO PARA PROCESAR-RAW VALOR CRUDO

    }
    bool canRead(){return true;}//Habilitamos la lectura
    void setup(){
        Wire.begin();//Inicializamos la librería wire
        RTC.begin();//Inicializamos el módulo reloj
    }
    void update(){;}

    //Poner el modulo en hora de forma manual
    void setDatatime(){
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    //Autoponerse en hora
    void autoDataTimeAjust(){
        ;
        /*Conectarse al servidor NTP UDP con el servidor en 192.168.1.22
        se recibe la fecha y la hora y se pone en hora el RTC*/
    }

    float read(){
        if(!isValid()){
            return 666;//si el sensor esta inhabilitado devuelve 666
        }else{
            //Devuelve la lectura del sensor
            return now.day()+"/"+now.month()+"/"+now.year()+"-"+now.hour()+":"+now.minute()+":"+now.second();
        }
    }

    string getHour(){
        return now.hour()+":"+now.minute()+":"+now.second();
    }

    string getDate(){
        return now.day()+"/"+now.mounth()+"/"+now.year();
    }

    bool isValid(){return m_pin != -1;} //El sensor es utilizable

};

#endif // REALCLOCK_H
