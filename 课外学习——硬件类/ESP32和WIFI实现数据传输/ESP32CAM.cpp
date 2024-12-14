#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
 
const char* WIFI_SSID = "Your WiFi SSID";
const char* WIFI_PASS = "Your Password";
 
WebServer server_Jpg(80); //Define webserver port
WiFiServer server_return(88);//Define wifisever port
 
static auto hiRes = esp32cam::Resolution::find(800, 600);
void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server_Jpg.send(503, "", "");
    return;
  }
  //Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
       //         static_cast<int>(frame->size()));
 
  server_Jpg.setContentLength(frame->size());
  server_Jpg.send(200, "image/jpeg");
  WiFiClient client = server_Jpg.client();
  frame->writeTo(client);
}
 
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
 
void  setup(){
  Serial.begin(115200);
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
 
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-hi.jpg");
 
  server_Jpg.on("/cam-hi.jpg", handleJpgHi);
 
  server_Jpg.begin();
  server_return.begin();
}
 
void loop()
{
  server_Jpg.handleClient();
  server_return.hasClient();
  
  WiFiClient client_return = server_return.available();
  if (client_return) {
    String data;
    while (client_return.connected()) {
      if (client_return.available()) {
        char c = client_return.read();
        Serial.println(c);
      }
    }
    
    client_return.stop();
  }
}
