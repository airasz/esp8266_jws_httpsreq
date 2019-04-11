const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;

void setup()
{
 pinMode(RED_PIN, OUTPUT);
 pinMode(GREEN_PIN, OUTPUT);
 pinMode(BLUE_PIN, OUTPUT);
 Serial.begin(9600);
 delay(500);
 InitDisplay();
 LandscapeMode();
 EraseScreen();
}



void loop()
{
 ChangeBackgroundColor(0xFF,0xFF);
 delay(2000);
 ChangeBackgroundColorNew(RGB888toRGB565("FFFFFF"));
 delay(2000);
}



uint16_t RGB888toRGB565(const char *rgb32_str_)
{
 long rgb32=strtoul(rgb32_str_, 0, 16);
 return (rgb32>>8&0xf800)|(rgb32>>5&0x07e0)|(rgb32>>3&0x001f);
}


void ChangeBackgroundColorNew(unsigned int color) //Here was the issue, just added 'unsigned' to the parameter
{
 do
 {
   Serial.write(0x42);
   Serial.write(color / 256);
   Serial.write(color % 256);
 } while(uLCD()!=0x06);
}


void ChangeBackgroundColor(int color1, int color2)
{
 do
 {
   Serial.write(0x42);
   Serial.write(color1);
   Serial.write(color2);
 } while(uLCD()!=0x06);
}


void LandscapeMode()
{
 do
 {
   Serial.write(0x59);
   Serial.write(0x04);
   Serial.write(0x02);
 } while(uLCD()!=0x06);
}


void EraseScreen()
{
 do
 {
   Serial.write(0x45);
 } while(uLCD()!=0x06);
}


void InitDisplay()
{
 do
 {
   Serial.write(0x55);
 } while(uLCD()!=0x06);
}


int uLCD()
{
   int reply = 0x00;
   int timeout = 0;
   while(Serial.available() == 0 && timeout < 10000)
   {
     timeout++;
     SetLED(0x00);
     delayMicroseconds(500);
   }
   reply = Serial.read();
   SetLED(reply);
   Serial.flush();
   timeout = 0;
   return(reply);
}


void SetLED (int reply)
{    
 if (reply == 0x15) {
   digitalWrite(RED_PIN, HIGH);
   delay(100);
   digitalWrite(RED_PIN, LOW);
 } else if (reply == 0x06) {
   digitalWrite(GREEN_PIN, HIGH);    
   delayMicroseconds(25);
   digitalWrite(GREEN_PIN, LOW);
 } else {
   digitalWrite(BLUE_PIN, HIGH);
   digitalWrite(BLUE_PIN, LOW);
 }
}