#ifndef JPROTOCOL
#define JPROTOCOL

#define P_PACKETSTART       F("[")
#define P_PACKETSEPARATOR   F("|")
#define P_PACKETTERMINATOR  F("]")

#define M_NODEMSG         F("NODE")
#define M_JARVISMSG       F("JARVIS")
#define M_ESPMSG          F("ESP")

#define C_SETAP       F("setap")
#define C_SETCLIENT   F("setclient")
#define C_WSTATUS     F("wifistatus")
#define C_LOCALIP     F("localip")
#define C_BRIDGEMODE  F("bridgemode")
#define C_RESET       F("reset")
#define C_ENABLE      F("enable")
#define C_DISABLE     F("disable")
#define C_PING        F("ping")
#define C_PONG        F("pong")

#endif
