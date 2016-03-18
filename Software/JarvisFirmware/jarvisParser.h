
#ifndef JPARSER
#define JPARSER

#include "jarvisProtocol.h"

#include <vector>

static std::vector<String> splitStr(String& str,String sep){
	std::vector<String> result;
  if(str.length() < 1) return result;
  int sep_index = str.indexOf(sep);
  if(sep_index == -1)
  {//no hay separador, se devuelve un unico elemento
    result.push_back(str);
    return result;
  }

  while (sep_index >= 0)
  {
    result.push_back(str.substring(0,sep_index));
    str = str.substring(sep_index+sep.length());
    sep_index = str.indexOf(sep);
  }
  if(str.length()) result.push_back(str);
	return result;
}

class jarvisParser {

public:
  jarvisParser() {}

  static String encodeJarvisMsg(std::vector<String>& args)
  {
	  String result;
	  result += P_PACKETSTART;
	  result += M_JARVISMSG;
	  result += P_PACKETSEPARATOR;
	  result += encodeArgs(args);
	  result += P_PACKETTERMINATOR;
	  return result;
  }
  
  static String encodeEspMsg(std::vector<String>& args)
  {
	  String result;
	  result += P_PACKETSTART;
	  result += M_ESPMSG;
	  result += P_PACKETSEPARATOR;
	  result += encodeArgs(args);
	  result += P_PACKETTERMINATOR;
	  return result;
  }
  
  static String encodeNodeMsg(std::vector<String>& args)
  {
	  String result;
	  result += P_PACKETSTART;
	  result += M_NODEMSG;
	  result += P_PACKETSEPARATOR;
	  result += encodeArgs(args);
	  result += P_PACKETTERMINATOR;
	  return result;
  }


protected:

    static String encodeArgs(std::vector<String>& args)
    {
        String result;
        result += args[0];
        for (int i = 1 ; i < args.size() ; i++)
        {
                result += P_PACKETSEPARATOR;
                result += args[i];
        }
        return result;
    }


    void parseBuffer(String& buf)
    {
        if(buf.length() == 0) return;
        int s_index = buf.indexOf(P_PACKETSTART);
        int e_index = buf.indexOf(P_PACKETTERMINATOR);
        //saneado del buffer
        if(s_index < 0)
        {// si no hay inicio de paquete lo que hay en el buffer tiene que ser basura.
            buf = "";
            return;
        }
        int s_lenght = String(P_PACKETSTART).length();
        int e_lenght = String(P_PACKETTERMINATOR).length();
        //extraccion de comandos
        while ((s_index >= 0) && (e_index >= 0)) //Si hay inicio y fin de paquete se extrae el comando.
        {// lo que haya en el buffer hasta el inicio de paquete se descarta(basura)
        String packet = buf.substring(s_index+s_lenght,e_index);
        buf = buf.substring(e_index+e_lenght);
        parsePacket(packet);
        s_index = buf.indexOf(P_PACKETSTART);
        e_index = buf.indexOf(P_PACKETTERMINATOR);
        }
    }

    void parsePacket(String& str)
    {
    std::vector<String> args;
    args = splitStr(str, P_PACKETSEPARATOR);
    //str = "";
    if(args.size() < 2) return;
            String arg = args[0];
            args.erase(args.begin());

            if      (arg == M_NODEMSG)
                    processNodeMsg(args);
            else if (arg == M_JARVISMSG)
                    processJarvisMsg(args);
            else if (arg == M_ESPMSG)
                    processEspMsg(args);
    }

    virtual void processNodeMsg(std::vector<String>& args)
    { // que cada cual implemente su parte en la herencia

    }
    virtual void processJarvisMsg(std::vector<String>& args)
    {

    }
    virtual void processEspMsg(std::vector<String>& args)
    {

    }
};

#endif
