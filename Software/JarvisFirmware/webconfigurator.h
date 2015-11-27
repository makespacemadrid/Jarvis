#include "settings.h"
#ifdef ESP8266
#include <ESP8266WebServer.h>


class webConfigurator {
  public:
    webConfigurator(int port = 80) : m_webServer(port) {};
    void setup()
    {
      m_webServer.on         ( "/", std::bind(&webConfigurator::handleRoot, this));
      m_webServer.on ( "/test.svg", std::bind(&webConfigurator::drawGraph,this));
      m_webServer.onNotFound       (std::bind(&webConfigurator::handleNotFound,this));//Si peta la compilacion aqui para el ESP hay que comentar la linea del .ino #include <StandardCplusplus.h>
//      m_webServer.on ( "/inline", []() {
//        server.send ( 200, "text/plain", "this works as well" );
//      } );
      m_webServer.begin();
      Serial.println ( "HTTP server started" );      
    }

    void handleClient()
    {
      m_webServer.handleClient();
    }
  private:
    ESP8266WebServer m_webServer;
  
    void handleRoot() {
      char temp[1000];
      int sec = millis() / 1000;
      int min = sec / 60;
      int hr = min / 60;
    
      snprintf ( temp, 1000,
    
    "<html>\
      <head>\
        <meta http-equiv='refresh' content='5'/>\
        <title>ESP8266 Demo</title>\
        <style>\
          body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
        </style>\
      </head>\
      <body>\
 <form action=\"action_page.php\">\
  First name:<br>\
  <input type=\"text\" name=\"firstname\" value=\"Mickey\">\
  <br>\
  Last name:<br>\
  <input type=\"text\" name=\"lastname\" value=\"Mouse\">\
  <button type=\"submit\">\
 </form>\ 
      </body>\
    </html>",
    
        hr, min % 60, sec % 60
      );
      m_webServer.send ( 200, "text/html", temp );
    }

    void handleNotFound() {
      String message = "File Not Found\n\n";
      message += "URI: ";
      message += m_webServer.uri();
      message += "\nMethod: ";
      message += ( m_webServer.method() == HTTP_GET ) ? "GET" : "POST";
      message += "\nArguments: ";
      message += m_webServer.args();
      message += "\n";
    
      for ( uint8_t i = 0; i < m_webServer.args(); i++ ) {
        message += " " + m_webServer.argName ( i ) + ": " + m_webServer.arg ( i ) + "\n";
      }
    
      m_webServer.send ( 404, "text/plain", message );
    }

    void drawGraph() {
      String out = "";
      char temp[100];
      out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
      out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
      out += "<g stroke=\"black\">\n";
      int y = rand() % 130;
      for (int x = 10; x < 390; x+= 10) {
        int y2 = rand() % 130;
        sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
        out += temp;
        y = y2;
      }
      out += "</g>\n</svg>\n";
    
      m_webServer.send ( 200, "image/svg+xml", out);
    }
};

#endif
