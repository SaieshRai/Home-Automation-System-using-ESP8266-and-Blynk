
//#define BLYNK_PRINT Serial  



       
#include <BlynkSimpleEsp8266.h> 
#include <Adafruit_NeoPixel.h>
#define PIN 9 
#define NUMPIXELS  19 //SD2
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// define the GPIO connected with Relays and switches

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
 
 //defining ON/OFF State for Relay
int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2
int sensor_in =0;
int flame_in = HIGH;



int wifiFlag = 0;
int flag = 0;
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Switch"
#define AUTH ""               // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "OnePlus 12R"             //Enter Wifi Name
#define WIFI_PASS "12345679"         //Enter wifi Password

//get wifi MAX & MIN Value
const int MAX_VAL = -57; // define maximum signal strength (in dBm)
const int MIN_VAL = -120; // define minimum signal strength (in dBm)

BlynkTimer timer;

//Regulate the relay
void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); // turn on relay 1
              toggleState_1 = 0;
              Serial.println("Device1 ON");
               lcd.setCursor(3, 0);
               lcd.print("Device1 ON");
                delay(800);
                lcd.clear();
              }
             else{
              digitalWrite(RelayPin1, HIGH); // turn off relay 1
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
              digitalWrite(RelayPin2, LOW); // turn on relay 2
              toggleState_2 = 0;
              Serial.println("Device2 ON");
              lcd.setCursor(3, 0);
               lcd.print("Device2 ON");
                delay(800);
                lcd.clear();
              }
             else{
              digitalWrite(RelayPin2, HIGH); // turn off relay 2
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

void with_internet(){ //Called when internet is connected 
    //App relay Control
    if (digitalRead(SwitchPin1) == LOW){
      Serial.println(digitalRead(SwitchPin1));
      delay(200);
      relayOnOff(1); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); // Update Button Widget 
   
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      relayOnOff(2);      
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   // Update Button Widget
      
    }
}
void without_internet(){ //Called when offline
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW){
      relayOnOff(2);
    }
}

/*BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_GAS);
  //Blynk.syncVirtual(VPIN_ALARM);
  
} */

// When App button is pushed - switch the relay
BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt(); //pushes the value as 1 or 0 to togglestate that turns relay off or on
  if(toggleState_1 == 0) { //relay is on for toggle state = 0
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
  toggleState_2 = param.asInt(); //pushes the value as 1 or 0 to togglestate that turns relay off or on
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

//GAS Sensor Function
void GAS_DETECT_ON() {
 
    Blynk.virtualWrite(V3, 1);
    Blynk.logEvent("leakage");
    digitalWrite(Exhaust, LOW);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(RelayPin1, HIGH);
    Blynk.virtualWrite(V1, 1);
    digitalWrite(RelayPin2, HIGH);
    Blynk.virtualWrite(V2, 1);
    lcd.clear(); //Clears LCD before printing
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


//FLAME DETECTOR FUNCTION
void FLAME_DETECT_ON() {
  
   Blynk.virtualWrite(V4, 1);
   Blynk.logEvent("fire");
   digitalWrite(RelayPin1, HIGH); //Turns off Appliance connected at first relay for security purpose
   Blynk.virtualWrite(V1, 1);
   digitalWrite(RelayPin2, HIGH); //Turns off Appliance connected at second relay for security purpose
   Blynk.virtualWrite(V2, 1);
  // digitalWrite(15, LOW);
   digitalWrite(Exhaust, LOW); //Turns on Exhaust Fan
   lcd.clear(); //Clears LCD before printing
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


void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
  
  }
  if (isconnected == true) {
    wifiFlag = 0;
    
   }
   delay(10);
}

void clearpixels() { //When called clears the initial colors on led strip
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(  0,   0,   0));
  }
  pixels.show();
  delay(10);
}

void setup() //initialize all the parameters
{
  lcd.init();        //initializes the LCD Display
  lcd.backlight(); //initializes the backlight of LCD Display
  
  WiFi.mode(WIFI_STA); //initialize ESP8266 as a station
  WiFi.disconnect(); 
  delay(10);

  WiFi.begin(WIFI_SSID, WIFI_PASS); //connects to network with mentioned SSID and Password

  while (WiFi.status() != WL_CONNECTED) { //checks the connection status
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    delay(10);
  }
  
  pixels.begin(); //initializes the LED Strip
  pixels.show(); //initialize all pixels to 'off'
  pixels.setBrightness(255); //sets the max brightness limit to 255
  
  
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT); //Setting the pin to which RelayPin 1 is connected as output
  pinMode(RelayPin2, OUTPUT); //Setting the pin to which RelayPin 2 is connected as output
  pinMode(Buzzer, OUTPUT); //Setting the pin to which Buzzer is connected as output

  pinMode(PIN, OUTPUT); //Setting the LED to which RelayPin 1 is connected as output
  pinMode(SwitchPin1, INPUT_PULLUP); //Setting the pin to which switch 1 is connected as input
  pinMode(SwitchPin2, INPUT_PULLUP); //Setting the pin to which RelayPin 2 is connected as input
  pinMode(A0, INPUT); //Setting the pin to which GAS SENSOR is connected as input
  pinMode(Exhaust, OUTPUT);
  pinMode(sensor, INPUT); //Setting the pin to which flame sensor is connected as input
  

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1); //Setting the pin to which RelayPin 1 is connected as LOW(OFF) at startup
  digitalWrite(RelayPin2, toggleState_2); //Setting the pin to which RelayPin 2 is connected as LOW(OFF) at startup

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH); //connects to Blynk server after the checking the defined Token
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
  
   timer.run(); // Initiates SimpleTimer
  
  if (wifiFlag == 0) {
    with_internet(); //calls the with_internet function
    delay(10);
  }
  else if (wifiFlag == 1) {
    without_internet(); //calls the without_internet function 
    delay(10);
  }




 
 
//Triggers the alarm for gas leakage
  sensor_in = analogRead(A0);
  delay(10);
  if(sensor_in>700) {
    GAS_DETECT_ON();
  }
  else {
    GAS_DETECT_OFF();
    delay(10);
  }

  //Triggers the alarm if flame is detected
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
  


  

  
//Show the status of connection

   if (wifiFlag == 1) { //sets the led strip to red if not connected to Blynk server
        for(int i=0;i<NUMPIXELS;i++) {
        pixels.setPixelColor(i, 200,0,0); 
        pixels.show();
        delay(10);
    }

   }
    
  else {
    clearpixels(); //clears the previous red pixels 
    long rssi = WiFi.RSSI(); //returns the strength value of wifi strength in decibels
    int strength = map(rssi, MIN_VAL, MAX_VAL, 0, 18); //rssi returns a value between MIN_VAL & MAX_VAL which is then mapped to values between 0 to 18
  
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
    //lcd.setCursor(0,1);
   // lcd.print("IP:");
    //lcd.setCursor(3,1);
    //lcd.print(WiFi.localIP());
   delay(200);
  }
     
   
}   

 
