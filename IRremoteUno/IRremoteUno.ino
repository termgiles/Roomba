#include "IRremote.h" // for remote reciever

int receiver = 11; //ir input pin
IRrecv irrecv(receiver);
decode_results results;
long countNoInstructions = 0; //needs more than 16 bits

long baud = 115200; //roomba OI default baud
bool power = false; //power state of roomba
int speedSetting = 200; //configurable wheel speed variable
int lastInstruction;

void setup() 
{
  Serial.begin(baud);
  irrecv.enableIRIn();
}

void translateIR()
{
  if (results.value != 0xFFFFFF){   //check for NEC repeat code
    lastInstruction = results.value;
  }
  switch(lastInstruction)
  {
    case 0xFFA25D: powerRoomba(); break; //roomba quit/on, 0x denotes hex
    case 0xFF629D: forward(); break;
    case 0xFFA857: backward(); break;
    case 0xFF22DD: left(); break;
    case 0xFFC23D: right(); break;
    case 0xFF02FD: whereIsLove(); break; //play / stop    
  }
}

void initialiseRoomba()
{
  Serial.write(128);    //opcode 128 starts the open interface
  delay(2050);
  Serial.write(132);  //switches to Full mode, 131 is Safe mode
  power = true;
}

void exitRoomba()
{
  Serial.write(173); //opcode 173 causes return to off mode
  delay(50);
  power = false;
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

void setSpeed(int setting)
{
  switch(setting)
  {
    case 4: speedSetting = 100; break;
    case 5: speedSetting = 200; break;
    case 6: speedSetting = 500; break; //500 is max (will be complicated as two's compliment)
  }
}

void sendTwosComplement(int denary){
  //floor and ceiling to +-500
  int high;
  int low;
  if (denary >= 0)
  {
    denary = denary > 500 ? 500 : denary;
    if (denary >= 256){
      high = 1;
      low = denary -256;
    }
    else{
      high = 0;
      low = denary;
    }
  }
  if (denary < 0 )
  {
    denary = denary < -500 ? -500: denary;
    if (denary <= -256){
      high = 254;
      low = 255 + (denary + 256);
    }
    else{
      high = 255;
      low = 255 + denary + 1;
    }
  }
    Serial.write(high);
    delay(10);
    Serial.write(low);
    delay(10);
}
  

void stopMotion(){
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

void left()  //uses direct drive op 145, two bytes for each wheel in two's complement, high first, range +-500mms^-1
{
  Serial.write(145);
  // delay(50);
  // Serial.write(0);  //Serial.write() sends in bytes to uno tx and rx pins
  // delay(50);
  // Serial.write(200);  //right wheel +200mms^-1
  // delay(50);
  // Serial.write(255);
  // delay(50);
  // Serial.write(56); //left wheel -200mms^-1
  // delay(50);
  delay(10);
  sendTwosComplement(200);
  sendTwosComplement(-200);

  //wait half a second then stop motion

  // stopMotion();
}

void right()
{
  Serial.write(145);
  delay(10);
  sendTwosComplement(-200);
  sendTwosComplement(200);
  // delay(50);
  // Serial.write(255);
  // delay(50);
  // Serial.write(56);
  // delay(50);
  // Serial.write(0); 
  // delay(50);
  // Serial.write(200);
  // delay(50);

  // stopMotion();
}

void forward()
{
  Serial.write(145);
  delay(10);
  sendTwosComplement(200);
  sendTwosComplement(200);
  // Serial.write(145);
  // delay(50);
  // Serial.write(0);
  // delay(50);
  // Serial.write(200);
  // delay(50);
  // Serial.write(0);
  // delay(50);
  // Serial.write(200);
  // delay(50);

  // stopMotion();
}

void backward()
{
  Serial.write(145);
  delay(10);
  sendTwosComplement(-200);
  sendTwosComplement(-200);
  // Serial.write(145);
  // delay(50);
  // Serial.write(255);
  // delay(50);
  // Serial.write(56);
  // delay(50);
  // Serial.write(255);
  // delay(50);
  // Serial.write(56);
  // delay(50);

  // stopMotion();
}

void whereIsLove()  //opcode for songs is 140
{
  delay(10);
  Serial.write(140);
  delay(10);
  Serial.write(1);  //song number
  delay(10);
  Serial.write(16); //song length
  delay(10);

  Serial.write(60);   //C4
  delay(10);
  Serial.write(32); //duration in 64ths of a second
  delay(10);
  Serial.write(62);
  delay(10);
  Serial.write(32);
  delay(10);
  Serial.write(64);
  delay(10);
  Serial.write(32);
  delay(10);
  Serial.write(65);
  delay(10);
  Serial.write(32);
  delay(10);
  Serial.write(62);
  delay(10);
  Serial.write(96);  
  delay(10);
  Serial.write(64);
  delay(10);
  Serial.write(32);
  delay(10);

  Serial.write(64);
  delay(10);
  Serial.write(128); //max byte size is 255
  delay(10);
  Serial.write(129); //add two beat rest
  delay(10);
  Serial.write(128);

  Serial.write(64);
  delay(10);
  Serial.write(32);
  delay(10);
  Serial.write(65);
  delay(10);
  Serial.write(32);
  delay(10);
  Serial.write(67);
  delay(10);
  Serial.write(32);
  delay(10);
  Serial.write(69);
  delay(10);
  Serial.write(32);
  delay(10);
  Serial.write(65);
  delay(10);
  Serial.write(96);
  delay(10);
  Serial.write(67);
  delay(10);
  Serial.write(32);
  delay(10);

  Serial.write(67);
  delay(10);
  Serial.write(256);
  delay(10);

  Serial.write(65);
  delay(10);
  Serial.write(32);
  delay(10);
  // Serial.write(67);
  // delay(10);
  // Serial.write(32);
  // delay(10);

  Serial.write(141);   //opcode to play
  delay(10);
  Serial.write(1);
  delay(50);
}

void loop() 
{
 if(irrecv.decode(&results))  //EITHER It's an overflow or an NEC repeat code, but holding down produces 0xFFFFFF
 {
  // Serial.println(results.value, HEX);
  translateIR();
  irrecv.resume();
  countNoInstructions = 0;
 } 
 else
 {
  countNoInstructions ++;
  if ( countNoInstructions > 35000)
  {
    countNoInstructions = 0;
    // Serial.println("Reset count");
    stopMotion(); //will stop motion if a continued wheel instruction is not received
  }
 }
}
