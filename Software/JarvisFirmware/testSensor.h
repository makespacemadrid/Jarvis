#ifndef TESTSENSOR_H
#define TESTSENSOR_H

#include "nodeComponent.h"
class testSensor : public nodeComponent{
public:
    testSensor(int pin = -1) : nodeComponent(pin){
        m_id ="test";
        m_actions.push_back(A_READ_DATA); //READ_DATA DEVUELVE EL VALOR PREPARADO PARA PROCESAR-RAW VALOR CRUDO

    }
    bool canRead(){return true;}//Habilitamos la lectura
    void setup(){;}
    void update(){;}
    float read(){
        if(!isValid()){
            return 666;//si el sensor esta inhabilitado devuelve 666
        }else{
            //Devuelve la lectura del sensor
        }
    }
    bool isValid(){return m_pin != -1;} //El sensor es utilizable

};
#endif // TESTSENSOR_H
