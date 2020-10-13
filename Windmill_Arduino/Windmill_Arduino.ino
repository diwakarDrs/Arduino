#include <ThingSpeak.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
// Code to use SoftwareSerial
#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(6,7);      // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin

const char* server = "api.thingspeak.com";
  //ThingSpeak.begin(client);


//------- current---------------//

#define analogchannel A0
float value=0;
float rev=0;
int rpm;
int oldtime=0;
int time;
int sensitivity = 185; 
int adcvalue= 0;
int offsetvoltage = 2500; 
float Voltage = 0; 
double ecurrent = 0;

//------------- voltage------------//

int analogInput = A1;
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0; //  
float R2 = 7500.0; // 
int value1 = 0;

//--------------IR damge-----------//

int LED = 15; 
int isObstaclePin = 16;  // This is our input pin
int isObstacle; 
 
//---------------gas sensor-----------//
int smokeA0 = A2;
int sensorThres = 390;
int buzzer =14 ;



//---------------GPS-----------------//
TinyGPSPlus gps;
SoftwareSerial ss(18, 19); 
static const int RXPin = 19, TXPin = 18;
static const uint32_t GPSBaud = 9600;

       

//------------Thingspeak--------------//

unsigned long myChannelNumber = XXXXX;
const char myWriteAPIKey = "---------API KEY--------";

//String apiKey = "--------API KEY-------";     // replace with your channel's thingspeak WRITE API key

String ssid="----------";    // Wifi network SSID
String password ="----------";  // Wifi network password

boolean DEBUG=true;

//======================================================================== showResponce
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial.available()){
        c=espSerial.read();
        if (DEBUG) Serial.print(c);
      }
    }                  
}

//========================================================================
boolean thingSpeakWrite(float value1, float value2, float value3){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  espSerial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(espSerial.find("Error")){
  if (DEBUG) Serial.println("AT+CIPSTART error");
  return false;
  }
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += myWriteAPIKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  getStr +="&field3=";
  getStr += String(value3);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  espSerial.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(espSerial.find(">")){
  espSerial.print(getStr);
  if (DEBUG)  Serial.print(getStr);
  Serial.println("");
  }
  else{
    espSerial.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    Serial.println("");
    return false;
     
  }
  return true;

  Serial.println("");
}


void isr() //interrupt service routine
{
rev++;
}


//================================================================================ setup
void setup() {

 pinMode(analogInput, INPUT);
 Serial.begin(9600);
 lcd.begin(16, 2); 
 lcd.setCursor(0,0);
 lcd.print("BATCH-8");
 Serial.println("BATCH-8");
 lcd.setCursor(0,1);
 lcd.print("---- ENGG CLG");
 Serial.println("---- ENGG CLG");
 Serial.println("");
 delay(300);
 lcd.clear();
 lcd.display();
 lcd.setCursor(0,0);
 lcd.print("MADHAN.S");
 Serial.println("MADHAN.S");
 lcd.setCursor(0,1);
 lcd.print("110XXXX107014");
 Serial.println("110XXXX07014");
 delay(500);
 lcd.clear();
 lcd.display();
 lcd.setCursor(0,0);
 lcd.print("SANOJ.T.SATHYAN");
 Serial.println("SANOJ.T.SATHYAN");
 lcd.setCursor(0,1);
 lcd.print("1108XXXX07030");
 Serial.println("11081XXX030");
 delay(500);
 lcd.clear();
 lcd.display();
 lcd.setCursor(0,0);
 lcd.print("SHIVA RAMANATH.P");
 Serial.println("SHIVA RAMANATH.P");
 lcd.setCursor(0,1);
 lcd.print("11XXXX7043");
 Serial.println("11XXXX43");
 Serial.println("");
 delay(500);
 lcd.clear();
 lcd.display();
 lcd.setCursor(0,0);
 lcd.print("Please wait........");
 delay(500);
// Serial.println("SHIVA RAMANATH.P");
 lcd.setCursor(0,1);
 lcd.print("SETUP COMPLETED");
// Serial.println("1XXXXXX043");
 Serial.println("");
 delay(500);
 
pinMode(isObstaclePin, INPUT);
pinMode (buzzer, OUTPUT);
pinMode(analogInput, INPUT);
                  
  DEBUG=true;           
  Serial.begin(9600); 
  espSerial.begin(9600);  
                                     
  //espSerial.println("AT+RST");         // Enable this line to reset the module;
  //showResponse(1000);
  //espSerial.println("AT+UART_CUR=9600,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
  //showResponse(1000);
  
  attachInterrupt(0,isr,RISING);

  espSerial.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(100);

  espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
  showResponse(500);

   if (DEBUG)  Serial.println("SETUP COMPLETED");

   
}


// ====================================================================== loop
void loop() {
//thingSpeakWrite(Voltage,rpm,ecurrent);
  // Read sensor values
adcvalue = analogRead(analogchannel);
Voltage = (adcvalue / 1024.0) * 5000; 
ecurrent = ((Voltage - offsetvoltage) / sensitivity);
ecurrent=(ecurrent-7);
Serial.print("Current= ");
Serial.print(ecurrent,2);
Serial.println("A");
Serial.print("");

 lcd.clear();
 delay(100);
 lcd.clear();
 lcd.display();
 lcd.setCursor(0,0);
 lcd.print("Current= "); 
 lcd.print(ecurrent,2);
 lcd.print("A"); 
 delay(300);


//          if (DEBUG)  Serial.println("voltage="+String(Voltage)+" mv");
//          if (DEBUG) Serial.println("ecurrent="+String( ecurrent)+" A");
//          if (DEBUG) Serial.println("RPM="+String( rpm)+" rev");
//          if (DEBUG) Serial.println("WindDamage="+String( rpm)+" rev");

//           // if (client.connect(server, 80)) {
//          ThingSpeak.setField(1,Voltage);
//          ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);  
//          delay(10000);
//            }
//thingSpeakWrite(Voltage,rpm,ecurrent);

//thingSpeakWrite(741312,1.0,'DMUVT3B4VG4RMJH8');
  
// Write values to thingspeak
//      
//    ThingSpeak.setField(1, Voltage);
//    ThingSpeak.setField(2, ecurrent);
//    ThingSpeak.setField(3,rpm);
//    //ThingSpeak.setField(4, dht_h);
//    //ThingSpeak.setField(5, bmp.pressureToAltitude(seaLevelPressure, event.pressure));
//   // ThingSpeak.setField(6, dew_point);
//    ThingSpeak.writeField(myChannelNumber, 1,Voltage, myWriteAPIKey);
//    ThingSpeak.writeField(myChannelNumber, 2, ecurrent, myWriteAPIKey);
//    delay(60000);
    
  // thingspeak needs 15 sec delay between updates,     
 // delay(200);  



//-------------------------Voltage------------------

 // read the value at analog input
   value1 = analogRead(analogInput);
   vout = (value1 * 5.0) / 1024.0; // see text
   vin = vout / (R2/(R1+R2)); 
   vin=(vin/2)-(0.5);
  
  lcd.setCursor(0,1);
  lcd.print("voltage =");
  lcd.print(vin,2);
  lcd.print("v");
  delay(500);   
Serial.print("Voltage= ");
Serial.print(vin,2);
Serial.println("v");
Serial.println("");
delay(500);

//------------------------Voltage-----------------------------



//------------------------IR RPM-------------------
delay(100);
detachInterrupt(0); //detaches the interrupt
time=millis()-oldtime; //finds the time
rpm=(rev/time)*60000*3; //calculates rpm for blades
oldtime=millis(); //saves the current time
rev=0;
attachInterrupt(0,isr,RISING);
Serial.print("Turbine RPM= ");
Serial.print(rpm);
Serial.println(" rev");
Serial.println("");
lcd.clear();
lcd.display();
lcd.setCursor(0,0);
lcd.print("TURBINE RPM: ");
lcd.setCursor(0,1);
lcd.print(rpm);
lcd.print(" rev");
delay(300);


//------------------------IR RPM-------------------



//----------------------IR DAMAGE--------------------

isObstacle = digitalRead(isObstaclePin);
 lcd.clear();
 lcd.display();
 lcd.setCursor(1,0);
 lcd.print("Wind damage = ");
 Serial.print("Wind damage = ");
 lcd.print(isObstacle);
 Serial.println(isObstacle);
 
  if (isObstacle ==HIGH)
  {
    Serial.println("OBSTACLE!!, OBSTACLE!!");
    digitalWrite(LED, HIGH);
    lcd.setCursor(1,1);
    lcd.print("WindTurbine Damage");
    Serial.println("WindTurbine Damage");
  }
  else
  {
    //Serial.println("clear");
    digitalWrite(LED, LOW);
    lcd.setCursor(1,1);
    lcd.print("NoDamage");
    Serial.println("NoDamage"); 
  }
  
 Serial.println("");
 delay(200);
//------------------------IR DAMAGE----------------//


//-----------------------GAS SENSOR----------------//
int analogSensor = analogRead(smokeA0);

 //Serial.print("Pin A2: ");
 //Serial.println(analogSensor);
 lcd.clear();
 lcd.display();
 lcd.setCursor(1,0);
 lcd.print("GasValue=");
 Serial.print("GasValue=");
 lcd.print(analogSensor);
 Serial.println(analogSensor);
 delay(500);
 // Checks if it has reached the threshold value
 if (analogSensor > sensorThres)
  {
    
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(500);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(500); 
    lcd.setCursor(1,1);
    lcd.print("SmokeDetected");
    Serial.println("SmokeDetected");
  }
  else
  {
    digitalWrite(buzzer, LOW);
    lcd.setCursor(1,1);
    lcd.print("NoSmoke");
    Serial.println("NoSmoke");
  }
  Serial.println("");
  delay(200);
//-----------------------GAS SENSOR----------------//



//---------------------*GPS*-----------------------------//
while (ss.available() > 0)
    if (gps.encode(ss.read()))
    displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
   lcd.clear();
   lcd.display();
   lcd.setCursor(1,0);
   lcd.print("No GPS detected: check wiring") ;
   while(true);
  }
}
void displayInfo()
{
 //Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());
    String latbuf;
    latbuf += (String(latitude, 6));
    Serial.println(latbuf);
    String lonbuf;
    lonbuf += (String(longitude, 6));
    Serial.println(lonbuf);
    lcd.clear();
    lcd.display();
    lcd.setCursor(1,0);
    lcd.print("LAT=");
    lcd.print(latbuf);
    lcd.print("LONG=");
    lcd.print(lonbuf);
    ThingSpeak.setField(1, latbuf);
    ThingSpeak.setField(2, lonbuf);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  
    delay(20000);
  }
  else
  {
    Serial.print(F("INVALID"));
    lcd.clear();
    lcd.display();
    lcd.setCursor(1,0);
    lcd.print("INVALID");
  }
  
  Serial.print(F("  Date/Time: "));
  lcd.clear();
  lcd.display();
  lcd.setCursor(1,0);
  lcd.print("  Date/Time: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    lcd.setCursor(1,1);
    lcd.print(gps.date.month());
    lcd.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    lcd.print(gps.date.day());
    lcd.print(F("/"));
    Serial.print(gps.date.year());
    lcd.print(gps.date.year());
    lcd.print(F("/"));
  }
  else
  {
    Serial.print(F("INVALID"));
    lcd.setCursor(1,1);
    lcd.print(F("INVALID"));
  }
  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println(); 

//----------------------GPS-----------------------------

}
