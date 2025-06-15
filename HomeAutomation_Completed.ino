#include <BlynkSimpleEsp8266.h> 
#include <Adafruit_NeoPixel.h>
#define PIN 9 
#define NUMPIXELS  19 //SD2
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define RelayPin1 14 //D5
#define RelayPin2 12 //D6
#define Buzzer 13   //D7
#define SwitchPin1 10  //SD3
#define SwitchPin2 0   //D3 
#define Exhaust 16 //D0
#define sensor 3//RX //fire_sensor
#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_GAS         V3
#define VPIN_ALARM       V4
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int toggleState_1 = 1; 
int toggleState_2 = 1; 
int sensor_in =0;
int flame_in = HIGH;
int wifiFlag = 0;
int flag = 0;
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Switch"
#define AUTH ""               // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "OnePlus 12R"             //Enter Wifi Name
#define WIFI_PASS "12345679"         //Enter wifi Password
const int MAX_VAL = -57; 
const int MIN_VAL = -120; 

BlynkTimer timer;


void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); 
              toggleState_1 = 0;
              Serial.println("Device1 ON");
               lcd.setCursor(3, 0);
               lcd.print("Device1 ON");
                delay(800);
                lcd.clear();
              }
             else{
              digitalWrite(RelayPin1, HIGH); 
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
              lcd.setCursor(3, 0);
              lcd.print("Device1 OFF");
              delay(800);
              lcd.clear();
              }
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(RelayPin2, LOW); 
              toggleState_2 = 0;
              Serial.println("Device2 ON");
              lcd.setCursor(3, 0);
               lcd.print("Device2 ON");
                delay(800);
                lcd.clear();
              }
             else{
              digitalWrite(RelayPin2, HIGH); 
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
              lcd.setCursor(3, 0);
               lcd.print("Device2 OFF");
                delay(800);
                lcd.clear();
              }
             delay(100);
      break;
      default : break;      
      }
  
}

void with_internet(){ 
    
    if (digitalRead(SwitchPin1) == LOW){
      Serial.println(digitalRead(SwitchPin1));
      delay(200);
      relayOnOff(1); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);  
   
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      relayOnOff(2);      
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   
      
    }
}
void without_internet(){ 
   
    if (digitalRead(SwitchPin1) == LOW){
      relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW){
      relayOnOff(2);
    }
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt(); 
  if(toggleState_1 == 0) { 
    lcd.setCursor(3, 0);
    lcd.print("Device1 ON");
    Serial.println("Device1 ON");
    delay(500);
    lcd.clear();
  }
  else {
    lcd.setCursor(3, 0);
    lcd.print("Device1 OFF");
    Serial.println("Device1 OFF");
    delay(500);
    lcd.clear();
  }
  
  digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt(); 
  if(toggleState_2 == 0) {
    lcd.setCursor(3, 0);
    lcd.print("Device2 ON");
    Serial.println("Device2 ON");
    delay(500);
    lcd.clear();
  }
  else {
    lcd.setCursor(3, 0);
    lcd.print("Device2 OFF");
    Serial.println("Device2 OFF");
    delay(500);
    lcd.clear();
  }
  digitalWrite(RelayPin2, toggleState_2);
}


void GAS_DETECT_ON() {
 
    Blynk.virtualWrite(V3, 1);
    Blynk.logEvent("leakage");
    digitalWrite(Exhaust, LOW);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(RelayPin1, HIGH);
    Blynk.virtualWrite(V1, 1);
    digitalWrite(RelayPin2, HIGH);
    Blynk.virtualWrite(V2, 1);
    lcd.clear(); 
    lcd.setCursor(2, 0);
    lcd.print("LEAKDETECTED");
    Serial.println("LEAKDETECTED");
  
    for (int i =0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(  255,  100,   0)); 
  }
    pixels.show();
    delay(5000);
    }
  void GAS_DETECT_OFF() {  
  
    digitalWrite(Exhaust, HIGH);
    Blynk.virtualWrite(V3,0);
    digitalWrite(Buzzer, LOW);
    delay(10);
    lcd.clear();
  }

void FLAME_DETECT_ON() {
  
   Blynk.virtualWrite(V4, 1);
   Blynk.logEvent("fire");
   digitalWrite(RelayPin1, HIGH); 
   Blynk.virtualWrite(V1, 1);
   digitalWrite(RelayPin2, HIGH); 
   Blynk.virtualWrite(V2, 1);
  
   digitalWrite(Exhaust, LOW); 
   lcd.clear(); 
   lcd.setCursor(2, 0);
   lcd.print("FIREDETECTED");
   Serial.println("FIREDETECTED");
   
  
   for (int i =0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(  255,  200,   0)); 
  }
    pixels.show(); 
   delay(3000);
  }
   
void FLAME_DETECT_OFF() {
  Blynk.virtualWrite(V4,0);
  digitalWrite(Buzzer, HIGH); 
  digitalWrite(Exhaust, HIGH);
  delay(10);
  lcd.clear(); 
  
}
void checkBlynkStatus() { 

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
  
  }
  if (isconnected == true) {
    wifiFlag = 0;
    
   }
   delay(10);
}

void clearpixels() { 
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(  0,   0,   0));
  }
  pixels.show();
  delay(10);
}

void setup() 
{
  lcd.init();        
  lcd.backlight(); 
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect(); 
  delay(10);
  WiFi.begin(WIFI_SSID, WIFI_PASS); 
  while (WiFi.status() != WL_CONNECTED) { 
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    delay(10);
  }
  pixels.begin(); 
  pixels.show(); 
  pixels.setBrightness(255); 
  Serial.begin(9600);
  pinMode(RelayPin1, OUTPUT); 
  pinMode(RelayPin2, OUTPUT); 
  pinMode(Buzzer, OUTPUT); 
  pinMode(PIN, OUTPUT); 
  pinMode(SwitchPin1, INPUT_PULLUP); 
  pinMode(SwitchPin2, INPUT_PULLUP); 
  pinMode(A0, INPUT); 
  pinMode(Exhaust, OUTPUT);
  pinMode(sensor, INPUT); 
  digitalWrite(RelayPin1, toggleState_1); 
  digitalWrite(RelayPin2, toggleState_2); 

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); 
  Blynk.config(AUTH); 
}

void checkconnection() {
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Not Connected");
    
  }
  else if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected");
    
  }
}

void loop()
{  
  
   timer.run(); 
  
  if (wifiFlag == 0) {
    with_internet(); 
    delay(10);
  }
  else if (wifiFlag == 1) {
    without_internet(); 
    delay(10);
  }

  sensor_in = analogRead(A0);
  delay(10);
  if(sensor_in>700) {
    GAS_DETECT_ON();
  }
  else {
    GAS_DETECT_OFF();
    delay(10);
  }

  
  flame_in = digitalRead(sensor);
  delay(10);
  if(flame_in == LOW) {
   FLAME_DETECT_ON(); 
   delay(3000);
  }
  else{
    FLAME_DETECT_OFF();
    delay(10);
  }
  
   if (wifiFlag == 1) { 
        for(int i=0;i<NUMPIXELS;i++) {
        pixels.setPixelColor(i, 200,0,0); 
        pixels.show();
        delay(10);
    }

   }
    
  else {
    clearpixels(); 
    long rssi = WiFi.RSSI(); 
    int strength = map(rssi, MIN_VAL, MAX_VAL, 0, 18); 
  
    for (int i = 0; i < strength; i++) {
    
      if(strength == 0) {
        pixels.setPixelColor(i, pixels.Color(255,   0,   0));
     }
      else if (strength == 1) { 
        pixels.setPixelColor(i, pixels.Color(255,   4,   0));
     }
     else if(strength == 2) {
      pixels.setPixelColor(i, pixels.Color(255,   8,   0));
     }
     else if(strength == 3) {
       pixels.setPixelColor(i, pixels.Color(255,  16,   0));
     }
     else if(strength == 4) {
      pixels.setPixelColor(i, pixels.Color(255,  64,   0));
    }
     else if(strength == 5) {
      pixels.setPixelColor(i, pixels.Color(255, 128,   0));
     }
     else if(strength == 6) {
      pixels.setPixelColor(i, pixels.Color(150, 150,   0));
     }
     else if(strength == 7) { 
      pixels.setPixelColor(i, pixels.Color(128, 150,   0));
     }
     else if(strength == 8) {
      pixels.setPixelColor(i, pixels.Color( 64, 150,   0));
     }
     else if(strength == 9) { 
      pixels.setPixelColor(i, pixels.Color(  0, 150,   0));
     }
     else if(strength == 10){ 
      pixels.setPixelColor(i, pixels.Color( 0, 150, 128));
     }
      else if(strength == 11){ 
      pixels.setPixelColor(i, pixels.Color( 0, 150, 128));
     }
      else if(strength == 12){ 
        pixels.setPixelColor(i, pixels.Color( 0, 120, 140));
     }
      else if(strength == 13){ 
        pixels.setPixelColor(i, pixels.Color( 0, 110, 150));
     }
      else if(strength == 14){ 
        pixels.setPixelColor(i, pixels.Color( 0, 100, 200));
     }
      else if(strength == 15){ 
        pixels.setPixelColor(i, pixels.Color( 0, 90, 200));
     }
      else if(strength == 16){ 
      pixels.setPixelColor(i, pixels.Color( 0, 70, 200));
     }
      else if(strength == 17){ 
      pixels.setPixelColor(i, pixels.Color( 0, 50, 200));
     }
      else if(strength == 18){ 
        pixels.setPixelColor(i, pixels.Color( 0, 20, 200));
     }
     else {
        pixels.setPixelColor(i, pixels.Color( 0, 0, 200));
        
    }
  }
        pixels.show();
        delay(70);
      
    }

      
 if (WiFi.status() != WL_CONNECTED) {
  
  lcd.setCursor(2, 0);
  lcd.print("Not Connected");
  delay(800);
  
 
  }
  else if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();   
    lcd.setCursor(3, 0);
    lcd.print("Connected");
   delay(200);
  }
     
   
}   

 
