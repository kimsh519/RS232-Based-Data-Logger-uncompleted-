#include <mcp_can.h>
#include <SPI.h>
#include <SD.h>

#define CAN0_INT 2                              // Set INT to pin 2  (메가로 수정 필요)
MCP_CAN CAN0(10);    
// Set CS to pin 10  (메가로 수정 필요)
const int chipSelect = 4;
char temp[10] = {0,};
char output[10] = {0,};
char output1[128] = {0,};
char noise[20] = {0,};
char serialPacket[7] = {0,}; // 송신 문자열 초기화
//////////////FOR BMS////////////////////BMS CAN 코드 젤 아래에 있음
char msgString[128]; 
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
/////////////////////////////////////////////
int i = 0; // 노이즈 제거에 사용
int z = 0; // z는 수신된 데이터가 1 일때 5회이상 반복시 재전송
int y = 0; // y는 수신된 데이터가 0 일때 5회이상 반복시 재전송


long unsigned MAX_VOLTAGE = 0;
long unsigned VOLTAGE = 0;
long unsigned RPM = 0;
long unsigned CURRENT = 0;
long unsigned CURRENTRAMP = 0;
long unsigned RXID = 0;
float SPEED = 0;
float floatBATTEMP = 0;
long unsigned intspeed = 0;
long unsigned intBATTEMP = 0;
long unsigned CONTROLLERTEMP = 0; //IGBT
long unsigned MOTORTEMP = 0;  
long unsigned BATTEMP = 0;

 int data1  = 0;  //string으로 변환(output[0]
 int data2 = 0;   //string으로 변환(output[1]
const byte pinBtn=2;
bool stateBtn = true;
void(*resetFunc)(void)=0;
void setup() 
{
void getUInt64fromHex(char const *str);
Serial.begin(115200);
Serial1.begin(115200);
Serial2.begin(115200);
pinMode(13,OUTPUT);
pinMode(pinBtn,INPUT_PULLUP);
delay(1000); //

    if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)// -> 8Mhz, 250KBPS로 통신
      Serial.println("MCP2515 Initialized Successfully!");
    else
      Serial.println("Error Initializing MCP2515...");
    
    CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
    pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
                                                               //2번핀을 인풋으로 선언 
    Serial.println("MCP2515 Library Receive Example...");
    while (!Serial) {
    ; // 시리얼 통신이 연결 될때까지 while 문 돌며 기다림
  }
while(1)
{
  
if (!SD.begin(chipSelect)) 
{
 
    Serial.println("Card failed, or not present");
    delay(1000);
    // don't do anything more:
 

}
  else if(SD.begin(chipSelect))
  {
    Serial.println("Connected");
    break;
  }
 
  
}

}


void loop() 
{
  /*stateBtn = (bool) digitalRead(pinBtn);
  Serial.println(stateBtn);*/

  
          request_RPM();
request_MOTORTEMP();
request_CONTROLLERTEMP();
request_VOLTAGE();
//request_BATTEMP();

request_CURRENT();
request_CURRENTRAMP();
//request_MAX_VOLTAGE();
CURRENT = map(CURRENT,0,600,0,100) ;
CURRENTRAMP = map(CURRENTRAMP,0,600,0,100) ;
SPEED = (RPM/(5.461167*4.4545)) * 60 * 2 * 3.14 * 0.001 * 0.256;
intspeed = int(SPEED);
Serial.println(intspeed);
//Speed
     Serial2.print ("n0.val="); 
     Serial2.print (intspeed); //실수임
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff); 

     
     
//Battery 
     Serial2.print ("n1.val=");
     Serial2.print (VOLTAGE);
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff);  
     VOLTAGE = map(VOLTAGE, 0, 294, 0, 100);       
     Serial2.print ("j0.val=");
     Serial2.print (VOLTAGE);
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff); 
      
//BatteryTemp

floatBATTEMP = BATTEMP/100;
intBATTEMP = int(floatBATTEMP);

     Serial2.print ("n2.val=");
     Serial2.print (BATTEMP);
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff);
     BATTEMP = map(BATTEMP, 0, 60, 0, 100);   // 값, 값최소, 값최대, 범위최소, 범위최대
     Serial2.print ("j1.val=");
     Serial2.print (BATTEMP);
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff);  // -> BMS...
     
//ContorllerTemp
  CONTROLLERTEMP = map(CONTROLLERTEMP, 0, 32767, 0, 80);   // 값, 값최소, 값최대, 범위최소, 범위최대
     Serial2.print ("n3.val=");
     Serial2.print (CONTROLLERTEMP);
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff); 

     Serial2.print ("j2.val=");
     Serial2.print (CONTROLLERTEMP);
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff); 
 
 
//MotorTemp    
     MOTORTEMP = map(MOTORTEMP, 0, 32767, 0, 100);    
     Serial2.print ("n4.val=");
     Serial2.print (MOTORTEMP);
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff);    

     Serial2.print ("j3.val=");
     Serial2.print (MOTORTEMP);
     Serial2.write (0xff);
     Serial2.write (0xff);
     Serial2.write (0xff);    
     
 
    String speeds = "";
    speeds ="A"+ String(intspeed);
 
    String mtemp = "";
    mtemp ="B"+ String(MOTORTEMP);

    String ctemp = "";
    ctemp ="C"+ String(CONTROLLERTEMP);

    String cu = "";
    cu ="D"+ String(CURRENT);

    String cur = "";
    cur ="E"+ String(CURRENTRAMP);
 SDload(speeds);
 SDload(mtemp);
 SDload(ctemp);
 SDload(cu);
 SDload(cur);
 


//delay(100);
 
      }
    

  







/////////////////////////////SPEED//////////////////////////////////////////////////
 void request_RPM()
  {
    
  char serialPacket[7] = {0,};

  serialPacket[0] = 0x33;   // 3
  serialPacket[1] = 0x64;   // D
  serialPacket[2] = 0x30;   // 0
  serialPacket[3] = 0x30;   // 0
  serialPacket[4] = 0x33;   // 3
  serialPacket[5] = 0x30;   // 0
  serialPacket[6] = 0x58;   // X
 
  Serial.println("Sending");
  Serial1.write(serialPacket, sizeof(serialPacket));
  Serial.write(serialPacket, sizeof(serialPacket));
  Serial.println();

           while(Serial1.available() == 0 )//수신된 데이터 없음
    {
      Serial.println("수신대기 및 재전송대기");      
  //     delay(500);
        y = y + 1;  
          //Serial.print("y값");
          //Serial.println(y);
         if(y == 10)
          {
          
              Serial.println("데이터 5회 미수신으로 인한 재전송");
                   
                Serial.println("Sending again[0]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
                  y=0; //초기화
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  
  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
                   Serial.print("z값");
          Serial.println(z);
         if(z == 5) //1BYTE데이터 5회이상 시 재전송
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
 
                Serial.println("Sending again[1]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));             
                Serial.println();
                  z=0; //초기화
          }
    }
    
  else if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0; //초기화
      y = 0; //초기화
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1]; //수신데이터 재배열
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }
 
  
 //  delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());
Serial.println("수신된데이터");
Serial.print("output[0] : ");
Serial.println(output[0]);
Serial.print("output[1] : ");
Serial.println(output[1]);

        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
          Serial.print("output[0]의 정수형, data1 : ");
          Serial.println(data1);
          Serial.println(" ");

 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
          Serial.println("output[1]의 정수형, data2 : ");
          Serial.println(data2);
          Serial.println(" ");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);  //0일때도 생각해야함
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.print("RPM 16진수 값 : "); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
RPM = (long unsigned)getUInt64fromHex(output1);
Serial.println("RPM 정수 값");
Serial.println(RPM);

Serial.println("수신종료");

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }

//   SDload(RPM);
}
/////////////////////////////CURRENT-CMD////////////////////////////////////////////////
 void request_CURRENT() //바모카 전압 측정값
  {
    
  char serialPacket[7] = {0,};

  serialPacket[0] = 0x33;   // 3
  serialPacket[1] = 0x64;   // D
  serialPacket[2] = 0x30;   // 0
  serialPacket[3] = 0x30;   // 0
  serialPacket[4] = 0x32;   // 2
  serialPacket[5] = 0x36;   // 6
  serialPacket[6] = 0x58;   // X
  
  Serial.println("Sending");
  Serial1.write(serialPacket, sizeof(serialPacket));
  Serial.write(serialPacket, sizeof(serialPacket));
  Serial.println();

           while(Serial1.available() == 0 )//수신된 데이터 없음
    {
      Serial.println("수신대기 및 재전송대기");      
 //      delay(500);
        y = y + 1;  
          Serial.print("y값");
          Serial.println(y);
         if(y == 10)
          {
          
              Serial.println("데이터 5회 미수신으로 인한 재전송");
                   
                Serial.println("Sending again[0]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
                  y=0; //초기화
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  
  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
                   Serial.print("z값");
          Serial.println(z);
         if(z == 5) //1BYTE데이터 5회이상 시 재전송
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
 
                Serial.println("Sending again[1]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));             
                Serial.println();
                  z=0; //초기화
          }
    }
    
  else if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0; //초기화
      y = 0; //초기화
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1]; //수신데이터 재배열
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }
 // delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());
Serial.println("수신된데이터");
Serial.print("output[0] : ");
Serial.println(output[0]);
Serial.print("output[1] : ");
Serial.println(output[1]);

        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
          Serial.print("output[0]의 정수형, data1 : ");
          Serial.println(data1);
          Serial.println(" ");

 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
          Serial.println("output[1]의 정수형, data2 : ");
          Serial.println(data2);
          Serial.println(" ");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);  //0일때도 생각해야함
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.print("CURRENT 16진수 값 : "); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
CURRENT = (long unsigned)getUInt64fromHex(output1);
Serial.println("CURRENT 정수 값");
Serial.println(CURRENT);

Serial.println("수신종료");

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }
//SDload(CURRENT);
}

/////////////////////////////CURRENT-RAMP////////////////////////////////////////////////
 void request_CURRENTRAMP() //바모카 전압 측정값
  {
    
  char serialPacket[7] = {0,};

  serialPacket[0] = 0x33;   // 3
  serialPacket[1] = 0x64;   // D
  serialPacket[2] = 0x30;   // 0
  serialPacket[3] = 0x30;   // 0
  serialPacket[4] = 0x32;   // 2
  serialPacket[5] = 0x32;   // 2
  serialPacket[6] = 0x58;   // X
  
  Serial.println("Sending");
  Serial1.write(serialPacket, sizeof(serialPacket));
  Serial.write(serialPacket, sizeof(serialPacket));
  Serial.println();

           while(Serial1.available() == 0 )//수신된 데이터 없음
    {
      Serial.println("수신대기 및 재전송대기");      
 //      delay(500);
        y = y + 1;  
          Serial.print("y값");
          Serial.println(y);
         if(y == 10)
          {
          
              Serial.println("데이터 5회 미수신으로 인한 재전송");
                   
                Serial.println("Sending again[0]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
                  y=0; //초기화
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  
  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
                   Serial.print("z값");
          Serial.println(z);
         if(z == 5) //1BYTE데이터 5회이상 시 재전송
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
 
                Serial.println("Sending again[1]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));             
                Serial.println();
                  z=0; //초기화
          }
    }
    
  else if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0; //초기화
      y = 0; //초기화
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1]; //수신데이터 재배열
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }

  
//  delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());
Serial.println("수신된데이터");
Serial.print("output[0] : ");
Serial.println(output[0]);
Serial.print("output[1] : ");
Serial.println(output[1]);

        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
          Serial.print("output[0]의 정수형, data1 : ");
          Serial.println(data1);
          Serial.println(" ");

 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
          Serial.println("output[1]의 정수형, data2 : ");
          Serial.println(data2);
          Serial.println(" ");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);  //0일때도 생각해야함
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.print("CURRENT RAMP 16진수 값 : "); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
CURRENTRAMP = (long unsigned)getUInt64fromHex(output1);
Serial.println("CURRENT RAMP 정수 값");
Serial.println(CURRENTRAMP);

Serial.println("수신종료");

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }
//SDload(CURRENTRAMP);
}
/////////////////////////////VOLTAGE////////////////////////////////////////////////
 void request_VOLTAGE() //바모카 전압 측정값
  {
    
  char serialPacket[7] = {0,};

  serialPacket[0] = 0x33;   // 3
  serialPacket[1] = 0x64;   // D
  serialPacket[2] = 0x30;   // 0
  serialPacket[3] = 0x30;   // 0
  serialPacket[4] = 0x65;   // e
  serialPacket[5] = 0x62;   // b
  serialPacket[6] = 0x58;   // X
  
  Serial.println("Sending");
  Serial1.write(serialPacket, sizeof(serialPacket));
  Serial.write(serialPacket, sizeof(serialPacket));
  Serial.println();

           while(Serial1.available() == 0 )//수신된 데이터 없음
    {
      Serial.println("수신대기 및 재전송대기");      
 //      delay(500);
        y = y + 1;  
          Serial.print("y값");
          Serial.println(y);
         if(y == 10)
          {
          
              Serial.println("데이터 5회 미수신으로 인한 재전송");
                   
                Serial.println("Sending again[0]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
                  y=0; //초기화
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  
  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
                   Serial.print("z값");
          Serial.println(z);
         if(z == 5) //1BYTE데이터 5회이상 시 재전송
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
 
                Serial.println("Sending again[1]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));             
                Serial.println();
                  z=0; //초기화
          }
    }
    
  else if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0; //초기화
      y = 0; //초기화
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1]; //수신데이터 재배열
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }
 
//  delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());
Serial.println("수신된데이터");
Serial.print("output[0] : ");
Serial.println(output[0]);
Serial.print("output[1] : ");
Serial.println(output[1]);

        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
          Serial.print("output[0]의 정수형, data1 : ");
          Serial.println(data1);
          Serial.println(" ");

 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
          Serial.println("output[1]의 정수형, data2 : ");
          Serial.println(data2);
          Serial.println(" ");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);  //0일때도 생각해야함
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.print("VOLTAGE 16진수 값 : "); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
VOLTAGE = (long unsigned)getUInt64fromHex(output1);
Serial.println("VOLTAGE 정수 값");
Serial.println(VOLTAGE);

Serial.println("수신종료");

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }

}
/////////////////////////////MAX_VOLTAGE////////////////////////////////////////////
 void request_MAX_VOLTAGE() //바모카 전압 측정값
  {
    
  char serialPacket[7] = {0,};

  serialPacket[0] = 0x33;   // 3
  serialPacket[1] = 0x64;   // D
  serialPacket[2] = 0x30;   // 0
  serialPacket[3] = 0x30;   // 0
  serialPacket[4] = 0x36;   // 6
  serialPacket[5] = 0x34;   // 4
  serialPacket[6] = 0x58;   // X
  
  Serial.println("Sending");
  Serial1.write(serialPacket, sizeof(serialPacket));
  Serial.write(serialPacket, sizeof(serialPacket));
  Serial.println();

           while(Serial1.available() == 0 )//수신된 데이터 없음
    {
      Serial.println("수신대기 및 재전송대기");      
//       delay(500);
        y = y + 1;  
          Serial.print("y값");
          Serial.println(y);
         if(y == 10)
          {
          
              Serial.println("데이터 5회 미수신으로 인한 재전송");
                   
                Serial.println("Sending again[0]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
                  y=0; //초기화
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  
  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
                   Serial.print("z값");
          Serial.println(z);
         if(z == 5) //1BYTE데이터 5회이상 시 재전송
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
 
                Serial.println("Sending again[1]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));             
                Serial.println();
                  z=0; //초기화
          }
    }
    
  else if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0; //초기화
      y = 0; //초기화
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1]; //수신데이터 재배열
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }
 
//  delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());
Serial.println("수신된데이터");
Serial.print("output[0] : ");
Serial.println(output[0]);
Serial.print("output[1] : ");
Serial.println(output[1]);

        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
          Serial.print("output[0]의 정수형, data1 : ");
          Serial.println(data1);
          Serial.println(" ");

 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
          Serial.println("output[1]의 정수형, data2 : ");
          Serial.println(data2);
          Serial.println(" ");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);  //0일때도 생각해야함
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.print("MAX_VOLTAGE 16진수 값 : "); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
MAX_VOLTAGE = (long unsigned)getUInt64fromHex(output1);
Serial.println("MAX_VOLTAGE 정수 값");
Serial.println(MAX_VOLTAGE);

Serial.println("수신종료");

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }
//SDload(MAX_VOLTAGE);
}
////////////////////////////RXID////////////////////////////////////////////////////
void request_RXID()
  {
    
  char serialPacket[7] = {0,};

  serialPacket[0] = 0x33;   // 3
  serialPacket[1] = 0x64;   // D
  serialPacket[2] = 0x30;   // 0
  serialPacket[3] = 0x30;   // 0
  serialPacket[4] = 0x36;   // 6
  serialPacket[5] = 0x38;   // 8
  serialPacket[6] = 0x58;   // X
  
  Serial.println("Sending");
  Serial1.write(serialPacket, sizeof(serialPacket));
  Serial.write(serialPacket, sizeof(serialPacket));
  Serial.println();

         while(Serial1.available() == 0 )//수신된 데이터 없음
    {
      Serial.println("수신대기 및 재전송대기");      
//       delay(500);
        y = y + 1;  
                  Serial.print("y값");
          Serial.println(y);
         if(y == 10)
          {
              Serial.println("데이터 5회 미수신으로 인한 재전송");
                           
                Serial.println("Sending again[0]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
                  y=0; //초기화
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  
  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
                   Serial.print("z값");
          Serial.println(z);
         if(z == 5) //1BYTE데이터 5회이상 시 재전송
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
                
                Serial.println("Sending again[1]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));             
                Serial.println();
                  z=0; //초기화
          }
    }
    
  else if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0; //초기화
      y = 0; //초기화
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1]; //수신데이터 재배열
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }

//  delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());
Serial.println("수신된데이터");
Serial.print("output[0] : ");
Serial.println(output[0]);
Serial.print("output[1] : ");
Serial.println(output[1]);

        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
          Serial.print("output[0]의 정수형, data1 : ");
          Serial.println(data1);
          Serial.println(" ");

 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
          Serial.println("output[1]의 정수형, data2 : ");
          Serial.println(data2);
          Serial.println(" ");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);  //0일때도 생각해야함
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.print("RXID 16진수 값 : "); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
RXID = (long unsigned)getUInt64fromHex(output1);
Serial.println("RXID 정수 값");
Serial.println(RXID);

Serial.println("수신종료");

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }
//SDload(RXID);
}


/////////////////////////////MOTORTEMP////////////////////////////////////////////
 void request_MOTORTEMP() //바모카 전압 측정값
  {
    
  char serialPacket[7] = {0,};

  serialPacket[0] = 0x33;   // 3
  serialPacket[1] = 0x64;   // D
  serialPacket[2] = 0x30;   // 0
  serialPacket[3] = 0x30;   // 0
  serialPacket[4] = 0x34;   // 4
  serialPacket[5] = 0x39;   // 9
  serialPacket[6] = 0x58;   // X
  
  Serial.println("Sending");
  Serial1.write(serialPacket, sizeof(serialPacket));
  Serial.write(serialPacket, sizeof(serialPacket));
  Serial.println();

           while(Serial1.available() == 0 )//수신된 데이터 없음
    {
      Serial.println("수신대기 및 재전송대기");      
 //      delay(500);
        y = y + 1;  
          Serial.print("y값");
          Serial.println(y);
         if(y == 10)
          {
          
              Serial.println("데이터 5회 미수신으로 인한 재전송");
                   
                Serial.println("Sending again[0]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
                  y=0; //초기화
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  
  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
                   Serial.print("z값");
          Serial.println(z);
         if(z == 5) //1BYTE데이터 5회이상 시 재전송
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
 
                Serial.println("Sending again[1]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));             
                Serial.println();
                  z=0; //초기화
          }
    }
    
  else if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0; //초기화
      y = 0; //초기화
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1]; //수신데이터 재배열
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }
//  delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());
Serial.println("수신된데이터");
Serial.print("output[0] : ");
Serial.println(output[0]);
Serial.print("output[1] : ");
Serial.println(output[1]);

        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
          Serial.print("output[0]의 정수형, data1 : ");
          Serial.println(data1);
          Serial.println(" ");

 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
          Serial.println("output[1]의 정수형, data2 : ");
          Serial.println(data2);
          Serial.println(" ");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);  //0일때도 생각해야함
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.print("MOTORTEMP 16진수 값 : "); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
MOTORTEMP = (long unsigned)getUInt64fromHex(output1);
Serial.println("MOTORTEMP 정수 값");
Serial.println(MOTORTEMP);

Serial.println("수신종료");

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }
//SDload(MOTORTEMP);
}

/////////////////////////////CONTROLLERTEMP////////////////////////////////////////////
 void request_CONTROLLERTEMP() //바모카 전압 측정값
  {
    
  char serialPacket[7] = {0,};

  serialPacket[0] = 0x33;   // 3
  serialPacket[1] = 0x64;   // D
  serialPacket[2] = 0x30;   // 0
  serialPacket[3] = 0x30;   // 0
  serialPacket[4] = 0x34;   // 4
  serialPacket[5] = 0x61;   // a
  serialPacket[6] = 0x58;   // X
  
  Serial.println("Sending");
 Serial1.write(serialPacket, sizeof(serialPacket));
  Serial.write(serialPacket, sizeof(serialPacket));
  Serial.println();

           while(Serial1.available() == 0 )//수신된 데이터 없음
    {
      Serial.println("수신대기 및 재전송대기");      
 //      delay(500);
        y = y + 1;  
          Serial.print("y값");
          Serial.println(y);
         if(y == 10)
          {
          
              Serial.println("데이터 5회 미수신으로 인한 재전송");
                   
                Serial.println("Sending again[0]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
                  y=0; //초기화
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  
  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
                   Serial.print("z값");
          Serial.println(z);
         if(z == 5) //1BYTE데이터 5회이상 시 재전송
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
 
                Serial.println("Sending again[1]");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));             
                Serial.println();
                  z=0; //초기화
          }
    }
    
  else if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0; //초기화
      y = 0; //초기화
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1]; //수신데이터 재배열
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }

//  delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());
Serial.println("수신된데이터");
Serial.print("output[0] : ");
Serial.println(output[0]);
Serial.print("output[1] : ");
Serial.println(output[1]);

        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
          Serial.print("output[0]의 정수형, data1 : ");
          Serial.println(data1);
          Serial.println(" ");

 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
          Serial.println("output[1]의 정수형, data2 : ");
          Serial.println(data2);
          Serial.println(" ");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);  //0일때도 생각해야함
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.print("CONTROLLERTEMP 16진수 값 : "); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
CONTROLLERTEMP = (long unsigned)getUInt64fromHex(output1);
Serial.println("CONTROLLERTEMP 정수 값");
Serial.println(CONTROLLERTEMP);

Serial.println("수신종료");

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }
//SDload(CONTROLLERTEMP);
}

////////////////////////변환/////////////////////////////////////////////////////////
  uint64_t getUInt64fromHex(char const *str)  //16진수 문자형을 10진수 정수형으로 변환
    {
        uint64_t accumulator = 0;
        for (size_t i = 0 ; isxdigit((unsigned char)str[i]) ; ++i)
        {
            char c = str[i];
            accumulator *= 16;
            if (isdigit(c)) /* '0' .. '9'*/
                accumulator += c - '0';
            else if (isupper(c)) /* 'A' .. 'F'*/
                accumulator += c - 'A' + 10;
            else /* 'a' .. 'f'*/
                accumulator += c - 'a' + 10;
    
        }
    
        return accumulator;
    }
///////////////////////////////SD카드 저장//////////////////////////////////////////////
void SDload(String data)
{     
       
      File dataFile = SD.open("datalog.txt", FILE_WRITE); //datalog.txt 라는 파일을 생성하고 datafile file에 저장
   
    
    
      if (dataFile) {
        dataFile.println(data);
        dataFile.close();
        // print to the serial port too:
        Serial.println(data);
        digitalWrite(13,HIGH);
 delay(100);
 digitalWrite(13,LOW);
      }                                           //datafile 값이 있으면 data를 datafile에 저장
     
      else {
        Serial.println("error opening datalog.txt");
      }
        
             
//없으면 에러 출력
 
 

}
/* refference, 구데이터라 이것저것 수정필요
void recieve()
{
         while(Serial1.available() == 0 )
    {
      Serial.println("수신대기 및 재전송대기");      
 //      delay(500);
        y = y + 1;  
         if(y = 5);
          {
              Serial.println("데이터 5회 미수신으로 인한 재전송");
              char serialPacket[7] = {0,};
              
                serialPacket[0] = 0x33;   // 3
                serialPacket[1] = 0x64;   // D
                serialPacket[2] = 0x30;   // 0
                serialPacket[3] = 0x30;   // 0
                serialPacket[4] = 0x36;   // 6
                serialPacket[5] = 0x34;   // 4
                serialPacket[6] = 0x58;   // X
                
                Serial.println("Sending");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
          }
      if(Serial.peek() == 88)
      {
        Serial.println("재전송을위한 탈출");    
        break;
      }
    }
  while(Serial1.available() > 0 )
{  //abcde를 입력하면 read()는 인트값을 반환, available은 문자열 개수(5)를 반환

  Serial.println(Serial1.available());
  
  if(Serial1.available() == 1)
    {
         z = z + 1;  
         if(z = 5);
          {
            Serial.println("초기값 1 byte로 5회반복으로 인한 재전송");
              char serialPacket[7] = {0,};
              
                serialPacket[0] = 0x33;   // 3
                serialPacket[1] = 0x64;   // D
                serialPacket[2] = 0x30;   // 0
                serialPacket[3] = 0x30;   // 0
                serialPacket[4] = 0x36;   // 6
                serialPacket[5] = 0x34;   // 4
                serialPacket[6] = 0x58;   // X
                
                Serial.println("Sending");
                Serial1.write(serialPacket, sizeof(serialPacket));
                Serial.write(serialPacket, sizeof(serialPacket));
                Serial.println();
          }
    }
    
  if(Serial1.available() == 2)
  {
      Serial.println(Serial1.available());
      Serial.println("저장된 바이트 수 : 2");
      z = 0;
      y = 0;
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        
        output[0] = temp[1];
        output[1] = temp[0];
  }
  
  else if(Serial1.available() > 2)  //2 byte 초과시 노이즈 제거
  {
    Serial.println("2 byte 초과로 입력됨");
    
    int S1A = Serial1.available();
    int i = 0;
    for(i=0;i<S1A;i++)
    {
       noise[i] = Serial1.read();
    }
  }
 delay(500);
}

Serial.print("시리얼1에 할당된 바이트크기 : ");
Serial.println(Serial.available());

Serial.println("출력값");
Serial.println(output[0]);
Serial.println(output[1]);
Serial.println("수신종료");

/////////////////////////////////////////////////
        unsigned char wantbyte1 = (output[0]);  // 그중 output[n] 데이터 추출 '0x02'
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();// -> 2
  
  //        Serial.println("data1#####################");
  //        Serial.println(data1);
  //        Serial.println("data1fin##################");
          
 
        unsigned char wantbyte2 = (output[1]);  // 그중 output[n] 데이터 추출 '0x01'
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt(); // -> 1
          
  //        Serial.println("data2#####################");
  //        Serial.println(data2,HEX);
  //        Serial.println("data2fin##################");

             if(data2 < 10)
                {
                  data1 = (data1 * 16);
                }

       sprintf(output1, "%x%x", data1,data2); //%x가 16진수임

  //     Serial.println("output1#####################");
       Serial.println("재배열된 16진수 값"); 
       Serial.println(output1);
  //     Serial.println("output1fin##################");
     
// Serial.println(output1[0]);
// Serial.println(output1[1]);
// Serial.println(output1[2]);
// Serial.println(output1[3]);

 
voltage = (long unsigned)getUInt64fromHex(output1);
Serial.println("수신 정수 값");
 Serial.println(voltage);

/////////////////////////////////////////////////
if(Serial.read() == 88)
  {
    Serial.println("Serial.read() 값 : ");    
    Serial.println(Serial.read());
    Serial.println("수신데이터 없음, 재전송");    
  }
//함수마지막줄
}
*/

////////////////////////////BMS//////////////////////////////////////////////////////
  void request_BATTEMP()  //배터리 온도 
  {
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
        // !는 NOT을 의미한다, 왜 LOW일때 데이터를 받는가? INTPIN이 HIGH면 오류신호인가?
        CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
        
        if((rxId & 0x80000000) == 0x80000000)     // &는 and gate 0,1이 만나면 0출력 Determine if ID is standard (11 bits) or extended (29 bits) 
          sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
        else
          sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
        //확장형인지 기본형인지 구분완료
      
        Serial.print(msgString);
      
        if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
          sprintf(msgString, " REMOTE REQUEST FRAME");
          Serial.print(msgString); //이 구문에서 원하는 바이트를 뽑아내는 법
        } else {
          for(byte i = 0; i<len; i++){ //이 구문에서 원하는 바이트를 뽑아내는 법
            sprintf(msgString, " 0x%.2X", rxBuf[i]); //rxBuf[i]의 데이터를 활용해야한다.
            Serial.print(msgString); 
          }
        }
        
    Serial.println();
  }//if문 닫음

//원하는id(온도의경우0x038) 에서 원하는byte 데이터를 받아야함 -> msgString 구조를 분리하는법(아마)
//배열에서 값을 추출  :  x = mySensVals[4]; -> 원하는 byte 데이터려나..?

  Serial.println("rxId#####################");
  Serial.println(rxId);
  Serial.println("rxIdfin##################");
 
  if(rxId == 64) //rxId가 0x040인 데이터일 때 동작
  {

        unsigned char wantbyte1 = rxBuf[2];  // 그중 rxBuf[n] 데이터 추출
        String stringwantbyte1 = String(wantbyte1); //unsigned char 형태를 string 형태로 변환
        data1 = stringwantbyte1.toInt();
  
          Serial.println("data1#####################");
          Serial.println(data1,HEX);
          Serial.println("data1fin##################");
 
        unsigned char wantbyte2 = rxBuf[3];  // 그중 rxBuf[n] 데이터 추출
        String stringwantbyte2 = String(wantbyte2); //unsigned char 형태를 string 형태로 변환
        data2 = stringwantbyte2.toInt();
          
          Serial.println("data2#####################");
          Serial.println(data2,HEX);
          Serial.println("data2fin##################");

       sprintf(output1, "%x%x", data1,data2);

       Serial.println("output1#####################");
       Serial.println(output1);
       Serial.println("output1fin##################");
     
BATTEMP = (long unsigned)getUInt64fromHex(output1);
 Serial.println(BATTEMP);
 
    } 
  }
  
