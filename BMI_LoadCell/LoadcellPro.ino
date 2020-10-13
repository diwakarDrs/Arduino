#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // LiquidCrystal_I2C library
#include <LiquidCrystal.h>
HX711_ADC LoadCell(A0,A1); 
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13; //Pins to which LCD is connected
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float k,a,count,count1;
int BMI;
int  height=0;
int  sample=0;
float T=194.0;
long Number;
int trigPin = 3;    // Trigger
int echoPin = 2;    // Echo
long duration,cm,d;

void setup() {

Serial.begin(9600);
  
//---------------- Start OF TITLE----------------------------------//
  lcd.begin(16,2); // begins connection to the LCD module
   lcd.clear();
  lcd.display();
  lcd.setCursor(0,0); //We are using a 16*2 LCD display
  lcd.write("BMI MEASURE"); //Display a intro message
  Serial.println("BMI MEASURE");
  lcd.setCursor(0,1);   // set the cursor to column 0, line 1
  lcd.write("BY JAYA ENGG"); //Display a intro message 
   Serial.println("BY JAYA ENGG");
  delay(2000); //Wait for display to show info
//--------------------End of TITLE--------------------------//


//-------------WEIGHT------------------------------------//
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(200); // load cells gets 200ms of time to stabilize
  LoadCell.setCalFactor(999.0); // calibration factor for load cell => strongly dependent on your individual setup
  
//-------------End WEIGHT------------------------------------//
  


//--------------HEIGHT-----------------------------------//


pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

//---------------HEIGHT---------------------------------------
 
}


void loop() {
  
//----Start of Weight_____//

  LoadCell.update(); // retrieves data from the load cell
  float a = LoadCell.getData(); // get output value 
  k=(a/23);
  for(int i=0;i<10;i++)
  {
   count=k;
   sample+=count;
  }
  sample/=10;

  lcd.clear();
  lcd.display();
  lcd.setCursor(0, 0); // set cursor to first row
  lcd.display();
  lcd.print("W[KG]:"); // print out to LCD
  Serial.print("W[KG]:");
  lcd.print(sample);
  Serial.println(sample);
 
//----End of Weight_____//



//-----------Start of Height---------------//
 digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  d = (((duration/2) / 31)+19); 
  Number=(T-d);
  for(int i=0;i<10;i++)
  {
   count1=Number;
   height+=count1;
  }
   height/=10; 
  
  
  lcd.setCursor(0,1); // set cursor to first row
  lcd.print("H=");
   Serial.print( "Height=");
  Serial.print( height);
  lcd.print( height);
   
  Serial.println("cm");
  lcd.print("cm");
 
//-----------End of Height---------------// 



//----------Start BMI-----------------//

BMI=((sample)/((height*height)/(10000.00)));

  lcd.setCursor(9,0);
  lcd.print("BMI=");
   Serial.print("BMI=");
  lcd.print(BMI);
  Serial.println(BMI);

//------------BMI CONDITION--------------// 

 if (BMI>18.5 && BMI<24.9)
    {
      lcd.setCursor(9,1);
      lcd.print("HEALTHY");
      Serial.println("YOUR ARE HEALTHY");
      delay(200);     
      }
    else if (BMI<18.5)
    {
        
      lcd.setCursor(9, 1);
      lcd.print("THIN");
      Serial.println("YOUR ARE THIN");
       delay(200); 
      }
    else if (BMI>25 && BMI<29.9)
    {
      
      lcd.setCursor(9, 1);
      lcd.print("OVERWGT");
      Serial.println("YOUR ARE OVERWEIGHT");
       delay(200); 
      }
    else if (BMI>30)
    {
      
      lcd.setCursor(9, 1);
      lcd.print("OBESE");
          Serial.println("YOUR ARE OBESITY");
       delay(200); 
    }  
    Serial.println("");
//----------End of BMI-----------------//    
    
}
