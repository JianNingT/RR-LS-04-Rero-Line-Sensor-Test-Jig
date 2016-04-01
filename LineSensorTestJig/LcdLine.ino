#include <SoftwareSerial.h>
#include <LCD4Bit_mod.h>
#include <Servo.h>
Servo myServo;
#define TxMode LOW    // Master transmit to G15
#define RxMode HIGH   // Master receive from G15
#define CTRL 2

SoftwareSerial mySerial(0, 1);
LCD4Bit_mod lcd = LCD4Bit_mod(2);
byte errStatus =0;
byte result = 0;
byte test1a = 0;
byte test2a = 0;
byte test3a = 0;
byte test4a = 0;
byte test1b = 0;
byte test2b = 0;
byte test3b = 0;
byte test4b = 0;
int calcvar = 0;
byte thresval = 0;
byte s1 = 0;
byte s2 = 0;
byte s3 = 0;
byte s4 = 0;

//LCD
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;



void setup(){
  
  pinMode(2, OUTPUT);
  pinMode(13,OUTPUT);
  
  myServo.attach(11,544,2700);
  myServo.write(544);

   lcd.init();
   lcd.clear();
   lcd.cursorTo(1,0);
   lcd.printIn("CYTRON TECH.");
   lcd.cursorTo(2, 0);
   lcd.printIn("Line Test Jig");  
   digitalWrite(13,HIGH);
   delay(1000);
   digitalWrite(13,LOW);

}

void loop(){

//Waiting for Sensor

  lcd.clear();
  lcd.cursorTo(1,0);
  lcd.printIn("Connect Sensor");
  lcd.cursorTo(2,0);
  lcd.printIn("Press 'Select'");
  result = 0;
  
  while(result == 0){

  adc_key_in = analogRead(0);    // read the value from the sensor  
  key = get_key(adc_key_in);            // convert into key press
  
  if (key != oldkey)            // if keypress is detected
  {
    delay(50);    // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);            // convert into key press
    if (key != oldkey)        
    {     
      oldkey = key;
      if (key >=0){
        result = 1;
        if (key == 4){
            mySerial.begin(19200);
        }
        else{

            //Select not pressed. Reseting Baud Rate from 
            
            lcd.clear();
            lcd.cursorTo(1,0);
            lcd.printIn("Reseting");
            lcd.cursorTo(2,0);
            lcd.printIn("Baud Rate");
            mySerial.begin(115200);
            result = writeLsBd(0x68);
            delay(1000);
            lcd.clear();
              if(result == 0){
                lcd.cursorTo(1,0);
                lcd.printIn("BaudRate Reset");
                lcd.cursorTo(2,0);
                lcd.printIn("Successful");
                digitalWrite(13,HIGH);
                delay(1000);
                digitalWrite(13,LOW);
                }
                else{
                  lcd.cursorTo(1,0);
                  lcd.printIn("Unknown Error");
                  digitalWrite(13,HIGH);
                  delay(1000);
                  digitalWrite(13,LOW);
              }
              while(1){}
  
          }
      }
    }
  
  }
  }

  result = 0;
  
//Check if Baud Rate is default (19200).
  lcd.clear();

  lcd.cursorTo(1,0);
  lcd.printIn("CheckingBaudRate");

  byte baudRateCheck = readLsBd(0x68);

  delay(1000);

  lcd.clear();
 
  if(errStatus == 0){
  lcd.cursorTo(1,0);
  lcd.printIn("Baud Rate");
  lcd.cursorTo(2,0);
  lcd.printIn("Check Complete");
  }
  else{
    lcd.cursorTo(1,0);
    lcd.printIn("Can't Connect");
    lcd.cursorTo(2,0);
    lcd.printIn("To Sensor");
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    while(1){}
  }


//Calibrate Line Sensor
  
  lcd.clear();
  lcd.cursorTo (1,0);
  lcd.printIn("Calibrate:Black");
  myServo.write(1050);
  delay (1500);
  test1a = readLsS1Anlg(0x68);
  test2a = readLsS2Anlg(0x68);
  test3a = readLsS3Anlg(0x68);
  test4a = readLsS4Anlg(0x68);

  
  lcd.clear();
  lcd.cursorTo (1,0);
  lcd.printIn("Calibrate:White");
  myServo.write(1475);
  delay (1500);
  test1b = readLsS1Anlg(0x68);
  test2b = readLsS2Anlg(0x68);
  test3b = readLsS3Anlg(0x68);
  test4b = readLsS4Anlg(0x68);

  calcvar = test1a + test1b;
  thresval = calcvar/2;
  writeLsS1Thres (0x68,thresval);
  
  calcvar = test2a + test2b;
  thresval = calcvar/2;
  writeLsS2Thres (0x68,thresval);
  
  calcvar = test3a + test3b;
  thresval = calcvar/2;
  writeLsS3Thres (0x68,thresval);
  
  calcvar = test4a + test4b;
  thresval = calcvar/2;
  writeLsS4Thres (0x68,thresval);  

  
//Beginning Sensor Testing

  lcd.clear(); 
  myServo.write(1875);
  lcd.cursorTo (1,0);
  lcd.printIn("Beginning");
  lcd.cursorTo (2,0);
  
  lcd.printIn("Sensor Test");
  delay(1000);
      
      result = readLsDgVal(0x68);
      test1a = result << 7;
      test1a = test1a >> 7;
      test2a = result << 6;
      test2a = test2a >> 7;
      test3a = result << 5;
      test3a = test3a >> 7;
      test4a = result << 4;
      test4a = test4a >> 7;

      if (test1a == 1){
        s1 = 0;
      }
      else{
        s1 = 1;
      }
      if (test2a == 0){
        s2 = 0;
      }
      else{
        s2 = 1;
      }
      if (test3a == 1){
        s3 = 0;
      }
      else{
        s3 = 1;
      }
      if (test4a == 0){
        s4 = 0;
      }
      else{
        s4 = 1;
      }
  
  myServo.write(2265);
  delay(1000);
      
      result = readLsDgVal(0x68);
      test1a = result << 7;
      test1a = test1a >> 7;
      test2a = result << 6;
      test2a = test2a >> 7;
      test3a = result << 5;
      test3a = test3a >> 7;
      test4a = result << 4;
      test4a = test4a >> 7;

      if (test1a == 0){
        s1 = 0;
      }
      else{
        s1 = 1;
      }
      if (test2a == 1){
        s2 = 0;
      }
      else{
        s2 = 1;
      }
      if (test3a == 0){
        s3 = 0;
      }
      else{
        s3 = 1;
      }
      if (test4a == 1){
        s4 = 0;
      }
      else{
        s4 = 1;
      }

// Results
lcd.clear();
lcd.cursorTo(1,0);
if ( s1 + s2 + s3 + s4 == 0){
  lcd.printIn("Sensor Test");
  lcd.cursorTo(2,0);
  lcd.printIn("Successful");
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
}
else{
  lcd.printIn("Sensor Error:");
    
    lcd.cursorTo(2,0);
    if(s1 == 1){
      lcd.printIn("S1");
      s1 = 0;
    }
    else if(s2 == 1){
      lcd.printIn("S2");
      s2 = 0;
    }
    else if(s3 == 1){
     lcd.printIn("S3");
     s3 = 0;
    }
    else if(s4 == 1){
     lcd.printIn("S4");
     s4 = 0;
    }
    else{}
    
    lcd.cursorTo(2,3);
    if(s2 == 1){
      lcd.printIn("S2");
      s2 = 0;
    }
    else if(s3 == 1){
     lcd.printIn("S3");
     s3 = 0;
    }
    else if(s4 == 1){
     lcd.printIn("S4");
     s4 = 0;
    }
    else{}

    lcd.cursorTo(2,6);
    if(s3 == 1){
     lcd.printIn("S3");
     s3 = 0;
    }
    else if(s4 == 1){
     lcd.printIn("S4");
     s4 = 0;
    }
    else{}

    lcd.cursorTo(2,6);
    if(s4 == 1){
     lcd.printIn("S4");
     s4 = 0;
    }
    else{}
    
    digitalWrite(13,HIGH);
    delay(250);
    digitalWrite(13,LOW);
    delay(250);
    digitalWrite(13,HIGH);
    delay(250);
    digitalWrite(13,LOW);
    delay(250);
    digitalWrite(13,HIGH);
    delay(250);
    digitalWrite(13,LOW);
    delay(250);
}
  myServo.write(544);
    while (pingLs(0x68) == 0){

  }

}

