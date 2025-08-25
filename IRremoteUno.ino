#include "IRremote.h" // for remote reciever

int receiver = 11; //ir input pin
IRrecv irrecv(receiver);
decode_results results;

long baud = 115200; //roomba OI default baud
bool power = false; //power state of roomba

void setup() 
{
  Serial.begin(baud);
  irrecv.enableIRIn();
}

void translateIR()
{
  switch(results.value)
  {
    case 0xFFA25D: powerRoomba(); break; //roomba quit/on
    case 0xFF629D: forward(); break;
    case 0xFFA857: backward(); break;
    case 0xFF22DD: left(); break;
    case 0xFFC23D: right(); break;    
  }
}

void initialiseRoomba()
{
  Serial.write(128);    //opcode 128 starts the open interface
  delay(2050);
  Serial.write(132);  //switches to Full mode, 131 is Safe mode
}

void exitRoomba()
{
  Serial.write(173); //opcode 173 causes return to off mode
  delay(50);
}

void powerRoomba()
{
  if(power)
  {
      exitRoomba();
  }
  else
  {
    initialiseRoomba();
  }
}

void right()  //uses direct drive op 145, two bytes for each wheel in two's complement, high first, range +-500mms^-1
{
  Serial.write(145);
  delay(50);
  Serial.write(0);  //Serial.write() sends in bytes to uno tx and rx pins
  delay(50);
  Serial.write(200);  //right wheel +200mms^-1
  delay(50);
  Serial.write(255);
  delay(50);
  Serial.write(56); //left wheel -200mms^-1
  delay(500);

  //wait half a second then stop motion

  Serial.write(145);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
}

void left()
{
  Serial.write(145);
  delay(50);
  Serial.write(255);
  delay(50);
  Serial.write(56);
  delay(50);
  Serial.write(0); 
  delay(50);
  Serial.write(200);
  delay(500);

  Serial.write(145);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
}

void forward()
{
  Serial.write(145);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(200);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(200);
  delay(500);

  Serial.write(145);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
}

void backward()
{
  Serial.write(145);
  delay(50);
  Serial.write(255);
  delay(50);
  Serial.write(56);
  delay(50);
  Serial.write(255);
  delay(50);
  Serial.write(56);
  delay(500);

  Serial.write(145);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
  Serial.write(0);
  delay(50);
}

void loop() 
{
 if(irrecv.decode(&results))
 {
  translateIR();
  irrecv.resume();
 } 
}
