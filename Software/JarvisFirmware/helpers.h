#ifndef HELPERS_H
#define HELPERS_H

static bool pinCanPWM(char pin)
{
#ifdef ESP8266
  if(pin == 16)
    return false;
  else
    return true; //Creo que todos los pines del esp pueden hacer pwm el (0 no)
#else
  if((pin == 3)  || //Arduino Nano puede hacer pwm en los pines 3,5,6,9,10,11.
     (pin == 5)  ||
     (pin == 6)  ||
     (pin == 9)  ||
     (pin == 10) ||
     (pin == 11))
    return true;
  else
    return false;
#endif
}

#endif // HELPERS_H
