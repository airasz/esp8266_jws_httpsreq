/*
    HTTP over TLS (HTTPS) example sketch
    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.
    Limitations:
      only RSA certificates
      no support of Perfect Forward Secrecy (PFS)
      TLSv1.2 is supported since version 2.4.0-rc1
    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
config: board: generic esp8266 1st index



*/

// this line add in windows

#include <ESP8266WiFi.h>
// #include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS     5
#define TFT_RST    16  // you can also connect this to the Arduino reset
                       // in which case, set this #define pin to -1!
#define TFT_DC     2
#define TFT_SCLK 0   // set these to be whatever pins you like!
#define TFT_MOSI 4   // set these to be whatever pins you like!
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


#ifndef STASSID
#define STASSID "XZ5C"
#define STAPSK  "air46664"
#endif
#define pinBL 12

// ESP8266WiFiMulti wifimulti;

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "bimasislam.kemenag.go.id";
const int httpsPort = 443;   //443
 WiFiClientSecure client;
 
// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char fingerprint[] PROGMEM = "0D D6 D2 55 34 1C 36 E5 9F 01 3F 7C 76 26 F7 3F 34 4B 67 DE";
String sshubuh,sdzuhur,sashar,smaghrib,sisyak;
int jsb,jdz,jas,jmg,jis,msb,mdz,mas,mmg,mis;
String datajadwal;
String jadwal0,jadwal1,jadwal2,jadwal3,jadwal4;
String tftlog;
String txtft;
void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(pinBL, OUTPUT);
  digitalWrite(pinBL, HIGH);

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST77XX_BLACK);
  tftsetlog("starting jadwal solat");
  tftsetlog("connecting to "+ String(ssid));
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("connecting>");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
    Serial.print(">");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  tftsetlog("connected");
  // Use WiFiClientSecure class to create TLS connection
  tftsetlog("request jadwal to "+ String(host));
  reqjadwal();

}
int logline=0;
void tftsetlog(String txt){
  // logline+=10;
  // tft.setCursor(0,0);
  tft.setTextWrap(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(">");
  tft.setTextColor(ST77XX_WHITE);
  tft.print(txt+"\n");
  // if(logline>150){
  //   logline=0;
  //   tft.fillScreen(ST77XX_BLACK);
  // }
}

void fill_schedule (String text){
  for(int i=0; i<6; i++){  

    int awal=text.indexOf("'")+1;
    Serial.printf("awal = %d ",awal);
    int akhir=text.indexOf(";")-1;

    Serial.printf("akhir = %d ",akhir);
    Serial.println("hasil pemisahan");
    // if(i<6){    txtft=txtft+ text.substring(awal,akhir)+"\n";}else{
    //   txtft= text.substring(awal,(akhir-10))+"\n+"+txtft;
    // }
    if(i==0){
      sshubuh="Shubuh  = "+text.substring(awal,akhir);
      jsb=(text.substring(awal,akhir-3).toInt());
      msb=(text.substring(awal+3,akhir).toInt());
      Serial.print("jsb = ");
      Serial.println(jsb);
      Serial.print("msb = ");
      Serial.println(msb);
      txtft=txtft+sshubuh+"\n";
      Serial.println(sshubuh);
    }else if(i==1){
      sdzuhur="Dzuhur  = "+text.substring(awal,akhir);
      jdz=(text.substring(awal,akhir-3).toInt());
      mdz=(text.substring(awal+3,akhir).toInt());
      txtft=txtft+sdzuhur+"\n";
      Serial.println(sdzuhur);

    }else if(i==2){
      sashar="Ashar   = "+text.substring(awal,akhir);
      jas=(text.substring(awal,akhir-3).toInt());
      mas=(text.substring(awal+3,akhir).toInt());
      txtft=txtft+sashar+"\n";
      Serial.println(sashar);
    }else if(i==3){
      smaghrib="Maghrib = "+text.substring(awal,akhir);
      jmg=(text.substring(awal,akhir-3).toInt());
      mmg=(text.substring(awal+3,akhir).toInt());
      txtft=txtft+smaghrib+"\n";
      Serial.println(smaghrib);
    }else if(i==4){
      sisyak="Isya'   = "+text.substring(awal,akhir);
      jis=(text.substring(awal,akhir-3).toInt());
      mis=(text.substring(awal+3,akhir).toInt());
      txtft=txtft+sisyak+"\n";
      Serial.println(sisyak);
    }else if(i==5){
      String waktu="Jadwal sholat \n"+text.substring(awal,(akhir-9));
      txtft=waktu+"\n\n\n"+txtft+"\n\n\nsumber :\nbimasislam.kemenag.go.id";
      datajadwal=txtft;
      Serial.println(waktu);
    }
    text=text.substring(akhir+2);
    Serial.print("hasil trimming  = ");
    Serial.println(text);  
  }
  tft.fillScreen(ST77XX_BLACK);
  testdrawtext(string2char(txtft), ST77XX_WHITE);

}
char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}


void reqjadwal (){

  Serial.print("connecting to ");
  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);

  // if (!client.connect(host, httpsPort)) {
  //   Serial.println("connection failed");
  //   return;
  // }
  client.connect(host, httpsPort);
  if(!client.connected()){
    Serial.println("connection failed retry");
    // delay(1000);
    client.connect(host, httpsPort);
  }
  if(!client.connected()){
    Serial.println("connection failed retry");
    // delay(1000);
    client.connect(host, httpsPort);
    return; 
  }
  // String url = "/adzan/ajax/ajax.daily1.php?id=67";
  String url = "/widget/jadwalshalat/42d38931fbd98d4764cc39ab8694a0f1f42d2e7d";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  line=client.readString();
  int awal=line.indexOf("wsbh");
  int akhir=line.indexOf("serverdate")-3;


  String ekstrak=line.substring(awal,akhir);
  Serial.print("Ekstrk > ");
  // Serial.println(ekstrak);
  fill_schedule(ekstrak);

  // testdrawtext("wait...", ST77XX_WHITE);
  // Serial.println("reply was:");
  // Serial.println("==========");
  // Serial.println(line);
  // Serial.println("==========");
  // Serial.println("closing connection");
}

bool BLstate=0;
long prevmillis;
int angka=0;
void loop() {
  if((prevmillis+1000)<millis()){
    angka++;
    if(angka==30&&txtft.length()<12){
      // reqjadwal();
      ESP.restart();
    }else if(angka>60){
      angka=0;
      if(!BLstate){
        BLstate=1;
        
      tft.fillScreen(ST77XX_BLACK);
      int rnd=random(0,5);
      if (rnd==0){
        testdrawtext(string2char(txtft), ST77XX_WHITE);
      }else if(rnd==1){
        testdrawtext(string2char(txtft), ST77XX_GREEN);
      }else if(rnd==2){
        testdrawtext(string2char(txtft), ST77XX_RED);
      }else if(rnd>2){
        testdrawtext(string2char(txtft), ST77XX_BLUE);
      }
      } else {BLstate=0;}
      digitalWrite(pinBL, BLstate);
    }
    prevmillis=millis();
  }
}
