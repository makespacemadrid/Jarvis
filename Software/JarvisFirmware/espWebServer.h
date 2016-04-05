#include "settings.h"
#define ESP8266
#ifdef ESP8266

#include <ESP8266WebServer.h>


class espWebServer {
  public:
    espWebServer(int port = 80) : m_webserver(port) {};

    void setup()
    {
      //m_webserver.on ( "/"          , std::bind(&espWebServer::handleRoot, this));
      m_webserver.on ( "/test.svg"  , std::bind(&espWebServer::drawGraph,this));

      m_webserver.on ("/list", HTTP_GET, std::bind(&espWebServer::handleFileList, this));
      //load editor
      m_webserver.on ("/edit", HTTP_GET,std::bind(&espWebServer::handleEditGet, this) );

      //create file
      m_webserver.on("/edit", HTTP_PUT, std::bind(&espWebServer::handleFileCreate, this) );
      //delete file
      m_webserver.on("/edit", HTTP_DELETE, std::bind(&espWebServer::handleFileDelete, this) );
      //first callback is called after the request has ended with all parsed arguments
      //second callback handles file uploads at that location

      m_webserver.on("/edit", HTTP_POST, std::bind(&espWebServer::handleEditPost, this), std::bind(&espWebServer::handleFileUpload, this));

      //called when the url is not defined here
      //use it to load content from SPIFFS
      m_webserver.onNotFound(std::bind(&espWebServer::handleNotFound,this));

      m_webserver.on("/all", HTTP_GET, std::bind(&espWebServer::handleReadAll,this));

      m_webserver.begin();
      Serial.println ( "HTTP webserver started" );
    }

    void handleClient()
    {
      m_webserver.handleClient();
    }

    ESP8266WebServer& webServer()
    {
        return m_webserver;
    }

private:
    ESP8266WebServer m_webserver;
    File             m_fsUploadFile;
  
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
      m_webserver.send ( 200, "text/html", temp );
    }

    void handleNotFound() {

      if(!handleFileRead(m_webserver.uri()))
      {
        String message = "File Not Found\n\n";
        message += "URI: ";
        message += m_webserver.uri();
        message += "\nMethod: ";
        message += ( m_webserver.method() == HTTP_GET ) ? "GET" : "POST";
        message += "\nArguments: ";
        message += m_webserver.args();
        message += "\n";
        for ( uint8_t i = 0; i < m_webserver.args(); i++ ) {
          message += " " + m_webserver.argName ( i ) + ": " + m_webserver.arg ( i ) + "\n";
        }
        m_webserver.send ( 404, "text/plain", message );
      }
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
    
      m_webserver.send ( 200, "image/svg+xml", out);
    }

    //format bytes
    String formatBytes(size_t bytes){
      if (bytes < 1024){
        return String(bytes)+"B";
      } else if(bytes < (1024 * 1024)){
        return String(bytes/1024.0)+"KB";
      } else if(bytes < (1024 * 1024 * 1024)){
        return String(bytes/1024.0/1024.0)+"MB";
      } else {
        return String(bytes/1024.0/1024.0/1024.0)+"GB";
      }
    }

    String getContentType(String filename){
      if(m_webserver.hasArg("download")) return "application/octet-stream";
      else if(filename.endsWith(".htm")) return "text/html";
      else if(filename.endsWith(".html")) return "text/html";
      else if(filename.endsWith(".css")) return "text/css";
      else if(filename.endsWith(".js")) return "application/javascript";
      else if(filename.endsWith(".png")) return "image/png";
      else if(filename.endsWith(".gif")) return "image/gif";
      else if(filename.endsWith(".jpg")) return "image/jpeg";
      else if(filename.endsWith(".ico")) return "image/x-icon";
      else if(filename.endsWith(".xml")) return "text/xml";
      else if(filename.endsWith(".pdf")) return "application/x-pdf";
      else if(filename.endsWith(".zip")) return "application/x-zip";
      else if(filename.endsWith(".gz")) return "application/x-gzip";
      return "text/plain";
    }

    void handleEditGet()
    {
        if(!handleFileRead("/edit.htm")) m_webserver.send(404, "text/plain", "FileNotFound");
    }

    void handleEditPost()
    {
        m_webserver.send(200, "text/plain", "");
    }

    bool handleFileRead(String path){
      //DBG_OUTPUT_PORT.println("handleFileRead: " + path);
      if(path.endsWith("/")) path += "index.htm";
      String contentType = getContentType(path);
      String pathWithGz = path + ".gz";
      if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
        if(SPIFFS.exists(pathWithGz))
          path += ".gz";
        File file = SPIFFS.open(path, "r");
        size_t sent = m_webserver.streamFile(file, contentType);
        file.close();
        return true;
      }
      return false;
    }

    void handleFileUpload(){
      if(m_webserver.uri() != "/edit") return;
      HTTPUpload& upload = m_webserver.upload();
      if(upload.status == UPLOAD_FILE_START){
        String filename = upload.filename;
        if(!filename.startsWith("/")) filename = "/"+filename;
        //DBG_OUTPUT_PORT.print("handleFileUpload Name: "); //DBG_OUTPUT_PORT.println(filename);
        m_fsUploadFile = SPIFFS.open(filename, "w");
        filename = String();
      } else if(upload.status == UPLOAD_FILE_WRITE){
        ////DBG_OUTPUT_PORT.print("handleFileUpload Data: "); //DBG_OUTPUT_PORT.println(upload.currentSize);
        if(m_fsUploadFile)
          m_fsUploadFile.write(upload.buf, upload.currentSize);
      } else if(upload.status == UPLOAD_FILE_END){
        if(m_fsUploadFile)
          m_fsUploadFile.close();
        //DBG_OUTPUT_PORT.print("handleFileUpload Size: "); //DBG_OUTPUT_PORT.println(upload.totalSize);
      }
    }

    void handleFileDelete(){
      if(m_webserver.args() == 0) return m_webserver.send(500, "text/plain", "BAD ARGS");
      String path = m_webserver.arg(0);
      //DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
      if(path == "/")
        return m_webserver.send(500, "text/plain", "BAD PATH");
      if(!SPIFFS.exists(path))
        return m_webserver.send(404, "text/plain", "FileNotFound");
      SPIFFS.remove(path);
      m_webserver.send(200, "text/plain", "");
      path = String();
    }

    void handleFileCreate(){
      if(m_webserver.args() == 0)
        return m_webserver.send(500, "text/plain", "BAD ARGS");
      String path = m_webserver.arg(0);
      //DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
      if(path == "/")
        return m_webserver.send(500, "text/plain", "BAD PATH");
      if(SPIFFS.exists(path))
        return m_webserver.send(500, "text/plain", "FILE EXISTS");
      File file = SPIFFS.open(path, "w");
      if(file)
        file.close();
      else
        return m_webserver.send(500, "text/plain", "CREATE FAILED");
      m_webserver.send(200, "text/plain", "");
      path = String();
    }

    void handleFileList() {
      if(!m_webserver.hasArg("dir")) {m_webserver.send(500, "text/plain", "BAD ARGS"); return;}

      String path = m_webserver.arg("dir");
      //DBG_OUTPUT_PORT.println("handleFileList: " + path);
      Dir dir = SPIFFS.openDir(path);
      path = String();

      String output = "[";
      while(dir.next()){
        File entry = dir.openFile("r");
        if (output != "[") output += ',';
        bool isDir = false;
        output += "{\"type\":\"";
        output += (isDir)?"dir":"file";
        output += "\",\"name\":\"";
        output += String(entry.name()).substring(1);
        output += "\"}";
        entry.close();
      }

      output += "]";
      m_webserver.send(200, "text/json", output);
    }


    void handleReadAll()
    {
        String json = "{";
        json += "\"heap\":" + String(ESP.getFreeHeap());
        json += ", \"analog\":" + String(analogRead(A0));
        json += ", \"gpio\":" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
        json += "}";
        m_webserver.send(200, "text/json", json);
        json = String();
    }
};

#endif
