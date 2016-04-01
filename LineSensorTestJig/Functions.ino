#include <SoftwareSerial.h>
#define TxMode LOW    // Master transmit to G15
#define RxMode HIGH   // Master receive from G15
#define CTRL 2
#define HEADER 0xFF

//Instruction
#define READ 0x02
#define WRITE 0x03
#define RESET 0x06

//Address
#define Baud (byte) 0x04
#define DgVal (byte) 0x12
#define S1an (byte) 0x0E
#define S2an (byte) 0x0F
#define S3an (byte) 0x10
#define S4an (byte) 0x11
#define S1th (byte) 0x08
#define S2th (byte) 0x09
#define S3th (byte) 0x0A
#define S4th (byte) 0x0B


//0. Factory Reset
//1. Write Baud Rate
//2. Read Baud Rate
//3. Read Digital Value
//4. Read S1 Analog
//5. Read S2 Analog
//6. Read S3 Analog
//7. Read S4 Analog
//8. Write S1 Threshold
//9. Write S2 Threshold
//10. Write S3 Threshold
//11. Write S4 Threshold
//12. Ping
//===============================================================================================================
//0.Factory Reset
//Input: ID
//Return:
//============================================================================================================
byte factoryRstLs (byte ID)
{
  byte buf[6]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x02); //Length
  mySerial.write(RESET); //Instruction
  byte checksum=0xFF-ID-0x02-RESET;   
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}

//===============================================================================================================
//1.Write Baud Rate
//Input: ID
//Return: Baud Rate
//============================================================================================================
byte writeLsBd (byte ID)
{
  byte buf[8]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x04); //Length
  mySerial.write(WRITE); //Instruction
  mySerial.write(Baud); //Address
  mySerial.write(0x67); //Number of bytes
  byte checksum=0xFF-ID-0x04-WRITE-Baud-0x67;   
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}

//===============================================================================================================
//2.Read Baud Rate
//Input: ID
//Return: Baud Rate
//============================================================================================================
byte readLsBd (byte ID)
{
  byte buf[8]; 
//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID); //ID
  mySerial.write(0x04); //Length
  mySerial.write(READ); //Instruction
  mySerial.write(Baud); //Address
  mySerial.write(0x01); //Number of bytes
  byte checksum=0xFF-ID-0x04-READ-Baud-0x01;
  mySerial.write(checksum); //checksum

 //Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes (buf,7);

  if(nbyte==0)
  {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<7) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    if(buf[4]!=0) // if return status not zero (error occured)
    {
      return(0xFF); 
    }
    else
    {     
      return(buf[5]); //return BaudRate
    }
  }
}

//===============================================================================================================
//3.Read Digital Value
//Input: ID
//Return: Digital Value
//============================================================================================================
byte readLsDgVal (byte ID)
{
  byte buf[8]; 
//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID); //ID
  mySerial.write(0x04); //Length
  mySerial.write(READ); //Instruction
  mySerial.write(DgVal); //Address
  mySerial.write(0x01); //Number of bytes
  byte checksum=0xFF-ID-0x04-READ-DgVal-0x01;
  mySerial.write(checksum); //checksum

 //Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes (buf,7);

  if(nbyte==0)
  {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<7) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    if(buf[4]!=0) // if return status not zero (error occured)
    {
      return(0xFF); 
    }
    else
    {     
      return(buf[5]); //return Digital Value
    }
  }
}

//===============================================================================================================
//4.Read S1 Analog
//Input: ID
//Return: S1 Analog Value
//============================================================================================================
byte readLsS1Anlg (byte ID)
{
  byte buf[8]; 
//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID); //ID
  mySerial.write(0x04); //Length
  mySerial.write(READ); //Instruction
  mySerial.write(S1an); //Address
  mySerial.write(0x01); //Number of bytes
  byte checksum=0xFF-ID-0x04-READ-S1an-0x01;
  mySerial.write(checksum); //checksum

 //Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes (buf,7);

  if(nbyte==0)
  {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<7) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    if(buf[4]!=0) // if return status not zero (error occured)
    {
      return(0xFF); 
    }
    else
    {     
      return(buf[5]); //return Digital Value
    }
  }
}

//===============================================================================================================
//5.Read S2 Analog
//Input: ID
//Return: S2 Analog Value
//============================================================================================================
byte readLsS2Anlg (byte ID)
{
  byte buf[8]; 
//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID); //ID
  mySerial.write(0x04); //Length
  mySerial.write(READ); //Instruction
  mySerial.write(S2an); //Address
  mySerial.write(0x01); //Number of bytes
  byte checksum=0xFF-ID-0x04-READ-S2an-0x01;
  mySerial.write(checksum); //checksum

 //Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes (buf,7);

  if(nbyte==0)
  {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<7) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    if(buf[4]!=0) // if return status not zero (error occured)
    {
      return(0xFF); 
    }
    else
    {     
      return(buf[5]); //return Digital Value
    }
  }
}

//===============================================================================================================
//6.Read S3 Analog
//Input: ID
//Return: S3 Analog Value
//============================================================================================================
byte readLsS3Anlg (byte ID)
{
  byte buf[8]; 
//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID); //ID
  mySerial.write(0x04); //Length
  mySerial.write(READ); //Instruction
  mySerial.write(S3an); //Address
  mySerial.write(0x01); //Number of bytes
  byte checksum=0xFF-ID-0x04-READ-S3an-0x01;
  mySerial.write(checksum); //checksum

 //Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes (buf,7);

  if(nbyte==0)
  {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<7) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    if(buf[4]!=0) // if return status not zero (error occured)
    {
      return(0xFF); 
    }
    else
    {     
      return(buf[5]); //return Digital Value
    }
  }
}

//===============================================================================================================
//7.Read S4 Analog
//Input: ID
//Return: S4 Analog Value
//============================================================================================================
byte readLsS4Anlg (byte ID)
{
  byte buf[8]; 
//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID); //ID
  mySerial.write(0x04); //Length
  mySerial.write(READ); //Instruction
  mySerial.write(S4an); //Address
  mySerial.write(0x01); //Number of bytes
  byte checksum=0xFF-ID-0x04-READ-S4an-0x01;
  mySerial.write(checksum); //checksum

 //Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes (buf,7);

  if(nbyte==0)
  {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<7) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    if(buf[4]!=0) // if return status not zero (error occured)
    {
      return(0xFF); 
    }
    else
    {     
      return(buf[5]); //return Digital Value
    }
  }
}

//===============================================================================================================
//8.Write S1 Threshold
//Input: ID, New Threshold
//Return: -
//============================================================================================================
byte writeLsS1Thres (byte ID, byte NewThres)
{
  byte buf[8]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x04); //Length
  mySerial.write(WRITE); //Instruction
  mySerial.write(S1th); //Address
  mySerial.write(NewThres); //Number of bytes
  byte checksum=0xFF-ID-0x04-WRITE-S1th-NewThres;   
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}
//===============================================================================================================
//9.Write S2 Threshold
//Input: ID, New Threshold
//Return: -
//============================================================================================================
byte writeLsS2Thres (byte ID, byte NewThres)
{
  byte buf[8]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x04); //Length
  mySerial.write(WRITE); //Instruction
  mySerial.write(S2th); //Address
  mySerial.write(NewThres); //Number of bytes
  byte checksum=0xFF-ID-0x04-WRITE-S2th-NewThres;   
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}
//===============================================================================================================
//10.Write S3 Threshold
//Input: ID, New Threshold
//Return: -
//============================================================================================================
byte writeLsS3Thres (byte ID, byte NewThres)
{
  byte buf[8]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x04); //Length
  mySerial.write(WRITE); //Instruction
  mySerial.write(S3th); //Address
  mySerial.write(NewThres); //Number of bytes
  byte checksum=0xFF-ID-0x04-WRITE-S3th-NewThres;   
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}
//===============================================================================================================
//11.Write S4 Threshold
//Input: ID, New Threshold
//Return: -
//============================================================================================================
byte writeLsS4Thres (byte ID, byte NewThres)
{
  byte buf[8]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x04); //Length
  mySerial.write(WRITE); //Instruction
  mySerial.write(S4th); //Address
  mySerial.write(NewThres); //Number of bytes
  byte checksum=0xFF-ID-0x04-WRITE-S4th-NewThres;   
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}

//===============================================================================================================
//4.Ping
//Input: ID
//Return:
//============================================================================================================
byte pingLs (byte ID)
{
  byte buf[8]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x02); //Length
  mySerial.write(0x01); //Instruction
  byte checksum=0xFF-ID-0x02-0x01;  
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}



// Convert ADC value to key number
int get_key(unsigned int input)
{
  int k;
    
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
           
    return k;
        }
  }
    
    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed
    
    return k;
}
