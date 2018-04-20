#define SEG1      2
#define SEG2      4
#define SEG3      10
#define SEG4      12
#define SEG5      8
#define SEG6      6
#define SEG7      7
#define SEGP      9

#define OFF      HIGH
#define ON       LOW

void InicializarLed7s()
{
  pinMode(SEG1, OUTPUT);
  pinMode(SEG2, OUTPUT);
  pinMode(SEG3, OUTPUT);
  pinMode(SEG4, OUTPUT);
  pinMode(SEG5, OUTPUT);
  pinMode(SEG6, OUTPUT);
  pinMode(SEG7, OUTPUT);
  pinMode(SEGP, OUTPUT);

  digitalWrite(SEG1, OFF);
  digitalWrite(SEG2, OFF);
  digitalWrite(SEG3, OFF);
  digitalWrite(SEG4, OFF);
  digitalWrite(SEG5, OFF);
  digitalWrite(SEG6, OFF);
  digitalWrite(SEG7, OFF);
  digitalWrite(SEGP, OFF);
}

void EscribirLed7s(int dato, int punto)
{
  switch(dato)
  {
  case '1':
  case 1:
    {
      writeLed7(0,1,1,0,0,0,0,punto);
      break;
    }
  case '2':
  case 2:
    {
      writeLed7(1,1,0,1,1,0,1,punto);
      break;
    }
  case '3':
  case 3:
    {
      writeLed7(1,1,1,1,0,0,1,punto);
      break;
    }
  case '4':
  case 4:
    {
      writeLed7(0,1,1,0,0,1,1,punto);
      break;
    }
  case '5':
  case 5:
    {
      writeLed7(1,0,1,1,0,1,1,punto);
      break;
    }
  case '6':
  case 6:
    {
      writeLed7(1,0,1,1,1,1,1,punto);
      break;
    }
  case '7':
  case 7:
    {
      writeLed7(1,1,1,0,0,0,0,punto);
      break;
    }
  case '8':
  case 8:
    {
      writeLed7(1,1,1,1,1,1,1,punto);
      break;
    }
  case '9':
  case 9:
    {
      writeLed7(1,1,1,1,0,1,1,punto);
      break;
    }
  case 'A':
  case 10:
    {
      writeLed7(1,1,1,0,1,1,1,punto);
      break;
    }
  case 'b':
  case 11:
    {
      writeLed7(0,0,1,1,1,1,1,punto);
      break;
    }
  case 'C':
  case 12:
    {
      writeLed7(1,0,0,1,1,1,0,punto);
      break;
    }
  case 'd':
  case 13:
    {
      writeLed7(0,1,1,1,1,0,1,punto);
      break;
    }
  case 'E':
  case 14:
    {
      writeLed7(1,0,0,1,1,1,1,punto);
      break;
    }
  case 'F':
  case 15:
    {
      writeLed7(1,0,0,0,1,1,1,punto);
      break;
    }
  case 'H':
    {
      writeLed7(0,1,1,0,1,1,1,punto);
      break;
    }
  case 'I':
    {
      writeLed7(0,1,1,0,0,0,0,punto);
      break;
    }
  case 'J':
    {
      writeLed7(0,1,1,1,0,0,0,punto);
      break;
    }
  case 'L':
    {
      writeLed7(0,0,0,1,1,1,0,punto);
      break;
    }
  case 'n':
    {
      writeLed7(0,0,1,0,1,0,1,punto);
      break;
    }
  case 'P':
    {
      writeLed7(1,1,0,0,1,1,1,punto);
      break;
    }
  case 'S':
    {
      writeLed7(1,0,1,1,0,1,1,punto);
      break;
    }
  }
}

void writeLed7(char s1, char s2, char s3, char s4, char s5, char s6, char s7, char p)
{
  digitalWrite(SEG1, !s1);
  digitalWrite(SEG2, !s2);
  digitalWrite(SEG3, !s3);
  digitalWrite(SEG4, !s4);
  digitalWrite(SEG5, !s5);
  digitalWrite(SEG6, !s6);
  digitalWrite(SEG7, !s7);
  digitalWrite(SEGP, !p);
}

void testLed7()
{
    writeLed7(1,1,1,1,1,1,1,1);
    delay(1000);
    writeLed7(0,0,0,0,0,0,0,0);
}

void testCaracteres()
{
  int i;
  //test leds
    writeLed7(1,0,0,0,0,0,0,0);
    delay(200);
    writeLed7(0,1,0,0,0,0,0,0);
    delay(200);
    writeLed7(0,0,1,0,0,0,0,0);
    delay(200);
    writeLed7(0,0,0,1,0,0,0,0);
    delay(200);
    writeLed7(0,0,0,0,1,0,0,0);
    delay(200);
    writeLed7(0,0,0,0,0,1,0,0);
    delay(200);
    writeLed7(0,0,0,0,0,0,1,0);
    delay(200);
    writeLed7(0,0,0,0,0,0,0,1);
    delay(200);
    writeLed7(0,0,0,0,0,0,0,0);
    
    for(i=0;i<16;i++)
    {
      EscribirLed7s(i,1);
      delay(1000);
    }
}
