#ifndef JPROTOCOL
#define JPROTOCOL

//Separadores de paquete
#define P_PACKETSTART       "["
#define P_PACKETSEPARATOR   "|"
#define P_PACKETTERMINATOR  "]"

//Etiquetas de protocolo
#define M_NODEMSG         F("NODE")
#define M_JARVISMSG       F("JARVIS")
#define M_ESPMSG          F("ESP")

#define C_GET_FREEM     F("getfreemem")
#define C_FREEMEM       F("freemem")

#define C_SETAP         F("setap")
#define C_SETCLIENT     F("setclient")
#define C_WSTATUS       F("wifistatus")
#define C_LOCALIP       F("localip")
#define C_BRIDGEMODE    F("bridgemode")
#define C_RESET         F("reset")
#define C_ENABLE        F("enable")
#define C_DISABLE       F("disable")
#define C_PING          F("ping")
#define C_PONG          F("pong")

#define C_GETID         F("getid")
#define C_ID            F("id")

#define C_GETCOMPONENTS F("getcomponents")
#define C_COMPONENT     F("component")

#define C_READSENSOR    F("readsensor")//pregunta
#define C_SENSOR        F("sensor")//respuesta

#define C_READSENSORS   F("readsensors")//pregunta
#define C_SENSORS       F("sensors")//respuesta

#define C_DOACTION      F("doaction")

#define E_EVENT       F("event")
#define E_ACTION      F("action")

//Acciones y eventos
enum jarvisEvents
{
    E_ENABLED,
    E_DISABLED,    
    E_ACTIVATED,
    E_DEACTIVATED,
    E_GLOBAL_POWERON,
    E_GLOBAL_SHUTDOWN,
    E_COFFEE_MAKING,
    E_COFFEE_MADE
};

enum jarvisActions
{
    A_ENABLE,
    A_DISABLE,
    A_ACTIVATE,
    A_DEACTIVATE,
    A_DIMM,
    A_BLINK,
    A_SET_COLOR,
    A_CYLON,
    A_BEEP,
    A_MAKE_COFFEE
};

#endif
