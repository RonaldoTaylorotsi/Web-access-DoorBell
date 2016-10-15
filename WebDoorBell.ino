/* Based on:
 * ====== ESP8266 Demo ======
 * (Updated Dec 14, 2014)
 * Ray Wang @ Rayshobby LLC
 * http://rayshobby.net/?p=9734
 * ==========================
 *
 * Modified by R. Wozniak
 * Compiled with Arduino 1.60 and Teensyduino 1.21b6
 * ESP8266 Firmware: AT21SDK95-2015-01-24.bin
 *
 * Change SSID and PASS to match your WiFi settings.
 * The IP address is displayed serial upon successful connection.
 */

#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include "SdFat.h"
SdFat sd;
SdFile file;
#include "Adafruit_GFX.h"
#include "TEENSEY_HX8357.h"
#define BUFFER_SIZE 1024
#define tft_CS 10
#define tft_DC 22
#define tft_RST 23 // RST can be set to -1 if you tie it to Arduino's reset
#define CS_PIN  2
#define bufferSize 32168
#define SampleRate 160

#define SSID  "*"      // change this to match your WiFi SSID
#define PASS  "*"  // change this to match your WiFi password
#define PORT  "80"           // using port 8080 by default

const int chipSelect = 9; 
int X = 0;
int Y = 0;
int Z = 0;
int C = 0;
int S = 0;
int A = 0;
int main_menu = 0;
int draw_menu = 0;
byte V[bufferSize];
int AudioCounter;

int record_message_menu = 0; 
int re_announce_menu = 0;
int leave_message_message = 0;
int hear_message_menu = 0;
int ring_bell_menu = 0;
long reset_time = 0;

char buffer[BUFFER_SIZE];
TEENSEY_HX8357 tft = TEENSEY_HX8357(tft_CS, tft_DC, tft_RST);
XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

// By default we are looking for OK\r\n
char OKrn[] = "OK\r\n";
byte wait_for_esp_response(int timeout, char* term=OKrn) {
  unsigned long t=millis();
  bool found=false;
  int i=0;
  int len=strlen(term);
  // wait for at most timeout milliseconds
  // or if OK\r\n is found
  while(millis()<t+timeout) {
    if(Serial1.available()) {
      buffer[i++]=Serial1.read();
      if(i>=len) {
        if(strncmp(buffer+i-len, term, len)==0) {
          found=true;
          break;
        }
      }
    }
  }
  buffer[i]=0;
  Serial.print(buffer);
  return found;
}

void setup() {
analogReadResolution(8);

 if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
   sd.initErrorHalt();
  }

  tft.begin(HX8357D);
  ts.begin();
    tft.setRotation(2);
    tft.writecommand(0x36);
    tft.writedata(0x80);
  tft.fillScreen(HX8357_BLACK);
 
  //-------------------------------------


//-------------------------------------



  // assume esp8266 operates at 115200 baud rate
  // change if necessary to match your modules' baud rate
  Serial1.begin(115200);  // Teensy Hardware Serial port 1   (pins 0 and 1)
  Serial.begin(115200);   // Teensy USB Serial Port
  //------------
  /*
  // Open next file in root.  The volume working directory, vwd, is root.
  // Warning, openNext starts at the current position of sd.vwd() so a
  // rewind may be neccessary in your application.
  sd.vwd()->rewind();
  while (file.openNext(sd.vwd(), O_READ)) {
    file.printFileSize(&Serial);
    Serial.write(' ');
    file.printModifyDateTime(&Serial);
    Serial.write(' ');
    file.printName(&Serial);
    if (file.isDir()) {
      // Indicate a directory.
      Serial.write('/');
    }
    Serial.println();
    file.close();
  }
  Serial.println("Done!");
  */
  //------------
  delay(5000);
  Serial.println("WIFI Setup -Start");  
  setupWiFi();

  // print device IP address
  Serial.print("device ip addr: ");
  Serial1.println("AT+CIFSR");
  wait_for_esp_response(1000);

    //drawMenu();
}

bool read_till_eol() {
  static int i=0;
  if(Serial1.available()) {
    buffer[i++]=Serial1.read();
    if(i==BUFFER_SIZE)  i=0;
    if(i>1 && buffer[i-2]==13 && buffer[i-1]==10) {
      buffer[i]=0;
      i=0;
      Serial.print(buffer);
      return true;
    }
  }
  return false;
}

void loop() {
    X = 0;
    Y = 0;
    if (C==0)
    {
      tft.fillScreen(HX8357_BLACK); 
     drawMenu();
     C=C+1;
    }
      
 TS_Point p = ts.getPoint();
      Z = p.z;
      if (Z < 4000)//(ts.touched()) 
    // if(ts.touched()) 
       {
      
      Y = map(p.x,88,3800,0,320);

      X = map(p.y,155,3920,0,240);
      
      Serial.print("X =  ");
      Serial.println(X);
      Serial.print("Y =  ");
      Serial.println(Y);
      Serial.print("Z =  ");
      Serial.println(Z);
      }
  
//===================================================
// Leave Message Selected
//===================================================
    if(X > 80 && X < 100) // Leave Message
    { 
      leaveMessage();
      X=0;
      Y = 0;
      }
//===================================================
// Hear Message Selected
//===================================================
    if(X > 55 && X < 75)// Hear Message
    {  
      hearMessage(); 
      X = 0;
      Y = 0;
      }
//===================================================
// Login Selected
//===================================================
    if(X > 20 && X < 50)// Login
    { 
    
   // printDirectory(); 
     login();
     managePassword();
     X = 0;
     Y = 0;
    }
//================================================   
// Ring Bell
//================================================ 
if(X > 104 && X < 220) // Ring Bell
{ 
  
 ringDoorBell();
  
} // End Of Ring Bell
  
  int ch_id, packet_len;
  char *pb;  
  if(read_till_eol()) {
    if(strncmp(buffer, "+IPD,", 5)==0) {
      // request: +IPD,ch,len:data
      sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
      if (packet_len > 0) {
        // read serial until packet_len character received
        // start from :
        pb = buffer+5;
        while(*pb!=':') pb++;
        pb++;
        if (strncmp(pb, "GET /", 5) == 0) {
          wait_for_esp_response(1000);
          Serial.println("-> serve homepage");
          serve_homepage(ch_id);
        }
      }
    }
  }
}

void serve_homepage(int ch_id) {
  
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nRefresh: 300\r\n";

  String content="";
    content += "<!DOCTYPE html>";
    content += "<html>";
    content += "<body bgcolor=#E6E6FA>";
    
    content += " <h1> <font color=#0000FF>The Taylors - Internet Message Access </font></h1> <br/>  ";
       
    content += "<p> <strong> Access to this private message site is prohibited to all but Family members and friends. Access is logged and monitored as well as password protected. Please feel free to leave legitimate messages for family members. <br/>";
    content += " <br/>";

    content += " <div style=background-color:yellow;color:black;padding:20px;>";
    content += "<h2>Messages</h2>";
    content += " <p>Messages are available from the FTP link.</p>";
    content += "<a href=ftp://96.51.167.193:8080>Message FTP Server</a></div>";
    content += " <br/>";
 
    content += "Brought To You By IOT</strong> </p>";
    
    content += " <p> <font color=#880088> Web platform: TEENSY 3.1 & ESP8266 </font> </p>";
    content += " <p> Teensy server uptime ";
    Serial.print("***************************************** UPTIME = ");
    Serial.println(millis());
    content += "<font color=#0000FF> ";
    content += String(millis()/60000);
    content += " Minutes </font> </p>";
    
      
    content += "</body>";
    content += "</html>";
    content += "<br />\n";       
    content += "\r\n";       

  header += "Content-Length:";
  header += (int)(content.length());
  header += "\r\n\r\n";
  Serial1.print("AT+CIPSEND=");
  Serial1.print(ch_id);
  Serial1.print(",");
  Serial1.println(header.length()+content.length());
  if(wait_for_esp_response(2000)) {
   //delay(100);
   Serial1.print(header);
   Serial1.print(content);
  } 
  else {
  Serial1.print("AT+CIPCLOSE=");
  Serial1.println(ch_id);
 }
}
void drawMenu(void)
{
  tft.setTextColor(HX8357_BLACK);
    tft.setTextSize(3);
  
  tft.setCursor(45, 77);
 tft.fillRect(2,20,238,160,HX8357_BLUE);
  tft.println("Ring Bell");
  
tft.setTextSize(2);

  tft.setCursor(39,197);
  tft.fillRect(2,190,238,30,HX8357_RED);
  tft.println("Leave Message");

  tft.setCursor(44, 237);
  tft.fillRect(2,230,238,30,HX8357_YELLOW);
  tft.println("Hear Message");

  tft.setCursor(90, 277);
  tft.fillRect(2,270,238,30,HX8357_MAGENTA);
  tft.println("Login");
X = 0;
Y = 0;
}

void setupWiFi() {

  // turn on echo
  Serial1.println("ATE1");
  wait_for_esp_response(1000);
  
  // try empty AT command
  //Serial1.println("AT");
  //wait_for_esp_response(1000);

  // set mode 1 (client)
  Serial1.println("AT+CWMODE=3");
  wait_for_esp_response(1000); 
 
  // reset WiFi module
  Serial1.print("AT+RST\r\n");
  wait_for_esp_response(1500);

   //join AP
  Serial1.print("AT+CWJAP=\"");
  Serial1.print(SSID);
  Serial1.print("\",\"");
  Serial1.print(PASS);
  Serial1.println("\"");
  wait_for_esp_response(5000);

  // start server
  Serial1.println("AT+CIPMUX=1");
   wait_for_esp_response(1000);
  
  //Create TCP Server in 
  Serial1.print("AT+CIPSERVER=1,"); // turn on TCP service
  Serial1.println(PORT);
   wait_for_esp_response(1000);
  
  Serial1.println("AT+CIPSTO=30");  
  wait_for_esp_response(1000);

  Serial1.println("AT+GMR");
  wait_for_esp_response(1000);
  
  Serial1.println("AT+CWJAP?");
  wait_for_esp_response(1000);
  
  Serial1.println("AT+CIPSTA?");
  wait_for_esp_response(1000);
  
  Serial1.println("AT+CWMODE?");
  wait_for_esp_response(1000);
  
  Serial1.println("AT+CIFSR");
  wait_for_esp_response(5000);
  
  Serial1.println("AT+CWLAP");
  wait_for_esp_response(5000);
  
  Serial.println("---------------*****##### READY TO SERVE #####*****---------------");
}
 void leaveMessage(void)
  {
    AudioCounter = 0;
    
  tft.fillScreen(HX8357_BLACK); 
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(20,127);
  tft.setTextSize(2);
  tft.fillRect(2,120,238,30,HX8357_RED);
  delay(300);
  
  tft.println("Start Talking Now");
 
  noInterrupts();
      while (AudioCounter < bufferSize)
      {
        V[AudioCounter] = analogRead(A1);
   
        AudioCounter = AudioCounter +1; 
    
       delayMicroseconds(SampleRate);  
      }

  interrupts();
  tft.setCursor(60,157);
  tft.println("Processing");

/*
// Get Min and Max Values for level control
     AudioCounter = 0;
    byte min = 255;
    byte max = 0;
    float mfactor = 0;
    float diff = 0;
   while (AudioCounter < bufferSize)
      {
        
        if(V[AudioCounter] < min ) { min = V[AudioCounter];}
        if(V[AudioCounter] > min) {max = V[AudioCounter];}
       
        AudioCounter = AudioCounter +1;  
        
      }
      mfactor = ((max - min)/255);
      
      diff = max-min;
      mfactor = (1/(diff/255))/2;
     
      min = min - (min - 1);
      max = ((diff)*mfactor);
 */   
  // Dynamic range limitor for spike limiting and autogain improvement
          AudioCounter = 0;   
  
      while (AudioCounter < bufferSize)
      {     
       
        if (V[AudioCounter] < 55) {
          V[AudioCounter]=123;
          V[AudioCounter-1]=123;
          V[AudioCounter-2]=123;
          V[AudioCounter-4]=123;
          V[AudioCounter-5]=123;
          V[AudioCounter-6]=123;
          }
        if (V[AudioCounter] > 220) {V[AudioCounter]=128;} 
        
        AudioCounter = AudioCounter +1;  
      } 

/*  
    AudioCounter = 0;   
  
      while (AudioCounter < bufferSize)
      {     
        if (V[AudioCounter] > 123) {V[AudioCounter]=((V[AudioCounter]*mfactor));}
        if (V[AudioCounter] < 123) {V[AudioCounter]=((V[AudioCounter]/mfactor));}
        AudioCounter = AudioCounter +1;  
      }
      

 // glitch remover 
  
 
*/

  AudioCounter = 200;   
  
      while (AudioCounter < (bufferSize-400))
      {     
        
        if (abs(V[AudioCounter]-V[AudioCounter + 1])> 30)
                     {
                       int U = AudioCounter -190;
                       int W = U + 380;
                       while (U < W)
                       {
                        V[U] = 128;
                        U=U+1;
                     //   Serial.println("");
                      //  Serial.println(U);
                     //   Serial.println("");
                        AudioCounter = AudioCounter +1;
                       }
                    //  Serial.println("DONE");
                     
                     }
        
        AudioCounter = AudioCounter +1;  
      //  Serial.println(AudioCounter);
      } 



                      
                    
  // Step removal(Glitch)    
  AudioCounter = 0;   

      while (AudioCounter < bufferSize-10)
      {     
        
        if (abs(V[AudioCounter]-V[AudioCounter + 1])> 30)
                     {
                    
                     V[AudioCounter+1] = V[AudioCounter+1]+67;
                    V[AudioCounter+2] = V[AudioCounter+2]+61;
                   V[AudioCounter+3] = V[AudioCounter+3]+55;
                     V[AudioCounter+4] = V[AudioCounter+4]+50;
                       V[AudioCounter+5] = V[AudioCounter+5]+45;
                        V[AudioCounter+6] = V[AudioCounter+6]+39;
                       V[AudioCounter+7] = V[AudioCounter+7]+34;
                       V[AudioCounter+8] = V[AudioCounter+8]+30;
                        V[AudioCounter+9] = V[AudioCounter+9]+26;
                         V[AudioCounter+10] = V[AudioCounter+10]+23;
                          V[AudioCounter+11] = V[AudioCounter+11]+20;
                           V[AudioCounter+12] = V[AudioCounter+12]+17;
                        V[AudioCounter+13] = V[AudioCounter+13]+13;
                         V[AudioCounter+14] = V[AudioCounter+14]+11;
                          V[AudioCounter+15] = V[AudioCounter+15]+8;
                           V[AudioCounter+16] = V[AudioCounter+16]+5;
                            V[AudioCounter+17] = V[AudioCounter+17]+-1;
                             V[AudioCounter+18] = V[AudioCounter+18]-2;
                              V[AudioCounter+19] = V[AudioCounter+19]-3;
                               V[AudioCounter+20] = V[AudioCounter+20]-4;
                                V[AudioCounter+21] = V[AudioCounter+21]-5;
                                 V[AudioCounter+22] = V[AudioCounter+22]-6;
                                   V[AudioCounter+23] = V[AudioCounter+23]-10;
                         V[AudioCounter+24] = V[AudioCounter+24]-11;
                          V[AudioCounter+25] = V[AudioCounter+25]-12;
                           V[AudioCounter+26] = V[AudioCounter+26]-13;
                            V[AudioCounter+27] = V[AudioCounter+27]-14;
                             V[AudioCounter+28] = V[AudioCounter+28]-15;
                              V[AudioCounter+29] = V[AudioCounter+29]-16;
                               V[AudioCounter+30] = V[AudioCounter+30]-17;
                                V[AudioCounter+31] = V[AudioCounter+31]-18;
                                 V[AudioCounter+32] = V[AudioCounter+32]-19;
                         V[AudioCounter+33] = V[AudioCounter+33]-20;
                          V[AudioCounter+34] = V[AudioCounter+34]-21;
                           V[AudioCounter+35] = V[AudioCounter+35]-22;
                            V[AudioCounter+36] = V[AudioCounter+36]-23;
                             V[AudioCounter+37] = V[AudioCounter+37]-24;
                              V[AudioCounter+38] = V[AudioCounter+38]-25;
                               V[AudioCounter+39] = V[AudioCounter+39]-26;
                                V[AudioCounter+40] = V[AudioCounter+40]-27;
                                 V[AudioCounter+41] = V[AudioCounter+41]-28;
                    V[AudioCounter+42] = V[AudioCounter+42]-29;
                          V[AudioCounter+43] = V[AudioCounter+43]-30;
                           V[AudioCounter+44] = V[AudioCounter+44]-30;
                           V[AudioCounter+45] = V[AudioCounter+45]-30;
                            V[AudioCounter+46] = V[AudioCounter+46]-30;
                             V[AudioCounter+47] = V[AudioCounter+47]-30;
                              V[AudioCounter+48] = V[AudioCounter+48]-30;
                               V[AudioCounter+49] = V[AudioCounter+49]-30;
                                V[AudioCounter+50] = V[AudioCounter+50]-30;
                                 V[AudioCounter+51] = V[AudioCounter+51]-30;  

                            V[AudioCounter+52] = V[AudioCounter+52]-30;
                          V[AudioCounter+53] = V[AudioCounter+53]-30;
                           V[AudioCounter+54] = V[AudioCounter+54]-30;
                           V[AudioCounter+55] = V[AudioCounter+55]-30;
                            V[AudioCounter+56] = V[AudioCounter+56]-29;
                             V[AudioCounter+57] = V[AudioCounter+57]-29;
                              V[AudioCounter+58] = V[AudioCounter+58]-29;
                               V[AudioCounter+59] = V[AudioCounter+59]-28;
                                V[AudioCounter+60] = V[AudioCounter+60]-28;
                                 V[AudioCounter+61] = V[AudioCounter+61]-28;  

                         V[AudioCounter+62] = V[AudioCounter+62]-28;
                          V[AudioCounter+63] = V[AudioCounter+63]-27;
                           V[AudioCounter+64] = V[AudioCounter+64]-27;
                            V[AudioCounter+65] = V[AudioCounter+65]-27;
                            V[AudioCounter+66] = V[AudioCounter+66]-27;
                             V[AudioCounter+67] = V[AudioCounter+67]-26;
                              V[AudioCounter+68] = V[AudioCounter+68]-26;
                               V[AudioCounter+69] = V[AudioCounter+69]-26;
                                V[AudioCounter+70] = V[AudioCounter+70]-26;
                                 V[AudioCounter+71] = V[AudioCounter+71]-25;  


                         V[AudioCounter+72] = V[AudioCounter+72]-25;
                          V[AudioCounter+73] = V[AudioCounter+73]-25;
                           V[AudioCounter+74] = V[AudioCounter+74]-25;
                           V[AudioCounter+75] = V[AudioCounter+75]-24;
                            V[AudioCounter+76] = V[AudioCounter+76]-24;
                             V[AudioCounter+77] = V[AudioCounter+77]-24;
                              V[AudioCounter+78] = V[AudioCounter+78]-24;
                               V[AudioCounter+79] = V[AudioCounter+79]-23;
                                V[AudioCounter+80] = V[AudioCounter+80]-23;
                                 V[AudioCounter+81] = V[AudioCounter+81]-23;  
                        
                        V[AudioCounter+82] = V[AudioCounter+82]-22;
                          V[AudioCounter+83] = V[AudioCounter+83]-22;
                           V[AudioCounter+84] = V[AudioCounter+84]-22;
                            V[AudioCounter+85] = V[AudioCounter+85]-21;
                            V[AudioCounter+86] = V[AudioCounter+86]-21;
                             V[AudioCounter+87] = V[AudioCounter+87]-21;
                              V[AudioCounter+88] = V[AudioCounter+88]-20;
                               V[AudioCounter+89] = V[AudioCounter+89]-20;
                                V[AudioCounter+90] = V[AudioCounter+90]-20;
                                 V[AudioCounter+91] = V[AudioCounter+91]-19;  


                         V[AudioCounter+92] = V[AudioCounter+92]-19;
                          V[AudioCounter+93] = V[AudioCounter+93]-19;
                           V[AudioCounter+94] = V[AudioCounter+94]-18;
                           V[AudioCounter+95] = V[AudioCounter+95]-18;
                            V[AudioCounter+96] = V[AudioCounter+96]-17;
                             V[AudioCounter+97] = V[AudioCounter+97]-17;
                              V[AudioCounter+98] = V[AudioCounter+98]-16;
                               V[AudioCounter+99] = V[AudioCounter+99]-16;
                                V[AudioCounter+100] = V[AudioCounter+100]-15;
                                 V[AudioCounter+101] = V[AudioCounter+101]-15; 

                    V[AudioCounter+102] = V[AudioCounter+102]-14;
                          V[AudioCounter+103] = V[AudioCounter+103]-14;
                           V[AudioCounter+104] = V[AudioCounter+104]-13;
                            V[AudioCounter+105] = V[AudioCounter+105]-13;
                            V[AudioCounter+106] = V[AudioCounter+106]-12;
                             V[AudioCounter+107] = V[AudioCounter+107]-12;
                              V[AudioCounter+108] = V[AudioCounter+108]-11;
                               V[AudioCounter+109] = V[AudioCounter+109]-11;
                                V[AudioCounter+110] = V[AudioCounter+110]-12;
                                 V[AudioCounter+111] = V[AudioCounter+111]-12;  


                         V[AudioCounter+112] = V[AudioCounter+112]-11;
                          V[AudioCounter+113] = V[AudioCounter+113]-11;
                           V[AudioCounter+114] = V[AudioCounter+114]-10;
                           V[AudioCounter+115] = V[AudioCounter+115]-10;
                            V[AudioCounter+116] = V[AudioCounter+116]-9;
                             V[AudioCounter+117] = V[AudioCounter+117]-9;
                              V[AudioCounter+118] = V[AudioCounter+118]-8;
                               V[AudioCounter+119] = V[AudioCounter+119]-8;
                                V[AudioCounter+120] = V[AudioCounter+120]-7;
                                 V[AudioCounter+121] = V[AudioCounter+121]-7;  

                   V[AudioCounter+122] = V[AudioCounter+122]-14;
                          V[AudioCounter+123] = V[AudioCounter+123]-6;
                           V[AudioCounter+124] = V[AudioCounter+124]-6;
                            V[AudioCounter+125] = V[AudioCounter+125]-5;
                            V[AudioCounter+126] = V[AudioCounter+126]-5;
                             V[AudioCounter+127] = V[AudioCounter+127]-4;
                              V[AudioCounter+128] = V[AudioCounter+128]-4;
                               V[AudioCounter+129] = V[AudioCounter+129]-3;
                                V[AudioCounter+130] = V[AudioCounter+130]-3;
                                 V[AudioCounter+131] = V[AudioCounter+131]-2;  


                         V[AudioCounter+132] = V[AudioCounter+132]-2;
                          V[AudioCounter+133] = V[AudioCounter+133]-1;
                           V[AudioCounter+134] = V[AudioCounter+134]-1;
                           V[AudioCounter+135] = V[AudioCounter+135]-1;
                            V[AudioCounter+136] = V[AudioCounter+136]-1;
                             V[AudioCounter+137] = V[AudioCounter+137]-1;
                              V[AudioCounter+138] = V[AudioCounter+138]-1;
                               V[AudioCounter+139] = V[AudioCounter+139]-1;
                                V[AudioCounter+140] = V[AudioCounter+140]-1;
                                 V[AudioCounter+141] = V[AudioCounter+141]-1;  
                     }
        
        AudioCounter = AudioCounter +1;  
      } 
   

     AudioCounter = 0;
  byte  min = 255;
  byte  max = 0;
  float  mfactor = 0;
  float  diff = 0;    
  while (AudioCounter < bufferSize)
      {
        
        if(V[AudioCounter] < min ) { min = V[AudioCounter];}
        if(V[AudioCounter] > min) {max = V[AudioCounter];}
       
        AudioCounter = AudioCounter +1;  
        
      }

      mfactor = ((max - min)/255);
      
      diff = max-min;
      mfactor = (1/(diff/255))/2;
     
      min = min - (min - 1);
      max = ((diff)*mfactor);
 // Get Min and Max Values for level control again
 
// Dynamic range expander
/*
   AudioCounter = 0;   
  
      while (AudioCounter < bufferSize)
      {     
        if (V[AudioCounter] > 128) {V[AudioCounter]=((V[AudioCounter]*mfactor));}
        if (V[AudioCounter] < 128) {V[AudioCounter]=((V[AudioCounter]/mfactor));}
        AudioCounter = AudioCounter +1;  
      }
 
*/
  C = 0;
  X=0;
  Y=0;
//--------------------------
// Send Message to File
//--------------------------

 printFileHeader();
 
File  myfile = sd.open("message.wav", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myfile) {
    Serial.print("Writing to Message File...");    
  }

  
  AudioCounter = 0;
   
  while (AudioCounter < bufferSize)
      { 
        
        
          myfile.write(V[AudioCounter]);
        
        AudioCounter = AudioCounter + 1; 
       
      }
      
      // close the file:
    myfile.close();
    Serial.println("done.");
 
  } 
  void hearMessage(void)
{
 
    
  tft.fillScreen(HX8357_BLACK); 
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(13,127);
  tft.setTextSize(2);
  tft.fillRect(2,120,238,30,HX8357_RED);
  tft.println("Play Back Starting");
  delay(500);

  //interrupts();
   AudioCounter = 0;   
  noInterrupts();
      while (AudioCounter < bufferSize)
      {
        
        analogWrite(A14,V[AudioCounter]);
        AudioCounter = AudioCounter +1;  
        delayMicroseconds(SampleRate);  
      }
  interrupts();
  analogWrite(A14,128);
  
  tft.setCursor(100,157);
  tft.println("Done");
  delay(500);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(100,157);
  tft.println("Done");
  C = 0;
  X=0;
  Y=0;
}
void login(void)
{
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(82, 27);
  tft.fillRect(2,20,238,30,HX8357_BLUE);
  tft.setTextSize(2);
  tft.println("Login"); 
           
   drawkeypad();
   X = 0;
   Y = 0;
}
 void ringDoorBell(void)
  {
 
  //const char *msg = "Ring";  
  tft.fillScreen(HX8357_BLACK); 
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(50,127);
  tft.setTextSize(2);
  tft.fillRect(2,120,238,30,HX8357_RED);
  tft.println("Ringing Bell");
  delay(200);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(50,127);
  tft.println("Ringing Bell");
  delay(200);
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(50,127);
  tft.println("Ringing Bell");

  delay(200);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(50,127);
  tft.println("Ringing Bell");
  delay(200);
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(50,127);
  tft.println("Ringing Bell");
  delay(200);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(50,127);
  tft.println("Ringing Bell");
  delay(200);
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(50,127);
  tft.println("Ringing Bell");
  delay(200);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(50,127);
  tft.println("Ringing Bell");
  // =================================
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(100,157);
  tft.println("Done");
  delay(2000);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(100,157);
  tft.println("Done");
  C = 0;
  X=0;
  Y=0;
  
  }
      void managePassword(void)
  {
     while(S==0)
  { 
  TS_Point p = ts.getPoint();
  Y = map(p.x,88,3800,0,320);
  X = map(p.y,155,3920,0,240);
      Serial.print("X =  ");
      Serial.println(X);
      Serial.print("Y =  ");
      Serial.println(Y);
     Serial.print("Z =  ");
     Serial.println(Z);
 if (X > 78 && X < 95 && Y > 126 && Y < 156)
 {
  S = 1; // "E"
  }
   if (X > 78 && X < 95 && Y > 188 && Y < 201)
 {
  S = 1; // "C"
 printDirectory();
  }
   if (X > 78 && X < 95 && Y > 157 && Y < 187)
 {
  S = 1; // "0"
  }
   if (X > 111 && X < 99 && Y > 82 && Y < 125)
 {
  S = 1; // "E"
  }
  }
  C = 0;
  X=0;
  Y=0;
  S = 0;
  }
  void drawkeypad(void)
{
//======================================
// Defaults
//======================================
  X = 60; Y = 50;

  tft.setTextColor(HX8357_BLACK);
  tft.setTextSize(3);
 //===================================== 
  tft.setCursor(20+X, 63+Y);
  tft.fillRect(15+X,61+Y,25,25,HX8357_WHITE);
  tft.println("1");

  tft.setCursor(46+X, 63+Y);
  tft.fillRect(41+X,61+Y,25,25,HX8357_WHITE);
  tft.println("2");

  tft.setCursor(73+X, 63+Y);
  tft.fillRect(67+X,61+Y,25,25,HX8357_WHITE);
  tft.println("3");

  tft.setCursor(19+X, 89+Y);
  tft.fillRect(15+X,87+Y,25,25,HX8357_WHITE);
  tft.println("4");

    tft.setCursor(46+X, 89+Y);
  tft.fillRect(41+X,87+Y,25,25,HX8357_WHITE);
  tft.println("5");

  tft.setCursor(73+X, 89+Y);
  tft.fillRect(67+X,87+Y,25,25,HX8357_WHITE);
  tft.println("6");

  tft.setCursor(19+X, 115+Y);
  tft.fillRect(15+X,112+Y,25,25,HX8357_WHITE);
  tft.println("7");

    tft.setCursor(46+X, 115+Y);
  tft.fillRect(41+X,112+Y,25,25,HX8357_WHITE);
  tft.println("8");

  tft.setCursor(73+X, 115+Y);
  tft.fillRect(67+X,112+Y,25,25,HX8357_WHITE);
  tft.println("9");

  tft.setCursor(19+X, 141+Y);
  tft.fillRect(15+X,138+Y,25,25,HX8357_WHITE);
  tft.println("C");

  tft.setCursor(46+X, 141+Y);
  tft.fillRect(41+X,138+Y,25,25,HX8357_WHITE);
  tft.println("0");

  tft.setCursor(73+X, 141+Y);
  tft.fillRect(67+X,138+Y,25,25,HX8357_WHITE);
  tft.println("E");
X = 0;
Y = 0;
}
void printDirectory() {
   
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(52, 27);
  tft.fillRect(2,20,238,30,HX8357_BLUE);
  tft.setTextSize(2);
  //tft.println("Login"); 
  tft.println("SD File List");
  tft.setCursor(0, 67);
  tft.fillRect(2,60,238,90,HX8357_BLUE);
  tft.setTextSize(1);
 
  SdFat sd;
  SdFile file;
  Y = 67;
  X = 10;
   if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
   }
  sd.vwd()->rewind();
  while (file.openNext(sd.vwd(), O_READ)) {
   // file.printFileSize(&tft);
   // Serial.write(' ');
   // file.printModifyDateTime(&tft);
    //tft.print(" ");
    file.printName(&tft);
    tft.print(" "); 
    file.printFileSize(&tft);

    if (file.isDir()) {
      // Indicate a directory.
      tft.print("/");
    //  Y = Y + 40;
    //  X = 10;
    }
    tft.println();
    file.close();
    tft.println("");
  }
  //tft.println("Done!");
  delay(5000);
 
 
}
void printFileHeader() {
   
 
  SdFat sd;
  SdFile file;

   if (sd.begin(chipSelect, SPI_HALF_SPEED)) {
    

 Serial.print("File Deletion...");
 if (sd.exists("message.wav")) {
    sd.remove("message.wav");
    
  }
  if (sd.exists("message.wav")) {
    Serial.println(" Failed");
  }
   else {Serial.println(" Success");}
   
   File  myfile = sd.open("message.wav", FILE_WRITE);
  myfile.write("RIFF");

   int buf[4] = {212,125,0,0};//{168,125,0,0};
   
       myfile.write(buf[0]);
       myfile.write(buf[1]);   
       myfile.write(buf[2]);                
       myfile.write(buf[3]);
       myfile.write("WAVEfmt");

   int list[] = {32,16,0,0,0,1,0,1,0,00,48,0,0,00,48,0,0,1,0,8,0,100,97,116,97,168,125,0,0};
            

         myfile.write(list[0]);
         myfile.write(list[1]);
         myfile.write(list[2]);
         myfile.write(list[3]);
         myfile.write(list[4]);
         myfile.write(list[5]);
        myfile.write(list[6]);
        myfile.write(list[7]);
        myfile.write(list[8]);
        myfile.write(list[9]);
        myfile.write(list[10]);
        myfile.write(list[11]);
        myfile.write(list[12]);
        myfile.write(list[13]);
        myfile.write(list[14]);
        myfile.write(list[15]);
        myfile.write(list[16]);
        myfile.write(list[17]);
        myfile.write(list[18]); 
        myfile.write(list[19]);
        myfile.write(list[20]);
        myfile.write(list[21]);
        myfile.write(list[22]);
        myfile.write(list[23]);
        myfile.write(list[24]);
        myfile.write(list[25]);
        myfile.write(list[26]);
        myfile.write(list[27]);  
        myfile.write(list[28]);
         
  
  
    myfile.close();
   }
   else Serial.print("Write Failure");
   /*
     File dataFile = sd.open("message.wav");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening message.wav");
  } 
 
  delay(5000);
  */
}

