//       1
//      ---
//  6  |   |  2
//     | 7 |
//      ---
//  5  |   |  3
//     |   |
//      ---
//       4    . P

#ifdef HERMESv1
  #define SEG1      7
  #define SEG2      8
  #define SEG3      2
  #define SEG4      4
  #define SEG5      3
  #define SEG6      6
  #define SEG7      5
  #define SEGP      1
#endif

#ifdef ATMEGA328
  #define SEG1      4
  #define SEG2      3
  #define SEG3      1
  #define SEG4      8
  #define SEG5      7
  #define SEG6      5
  #define SEG7      6
  #define SEGP      2
#endif

#ifdef ARDUINO_NANO_PROTO
  #define SEG1      2
  #define SEG2      4
  #define SEG3      10
  #define SEG4      12
  #define SEG5      8
  #define SEG6      6
  #define SEG7      7
  #define SEGP      9
#endif

#ifdef ARDUINO_NANO
  #define SEG1      4
  #define SEG2      5
  #define SEG3      8
  #define SEG4      7
  #define SEG5      6
  #define SEG6      3
  #define SEG7      2
  #define SEGP      9
#endif

#ifdef PROTOBOARD
  #define SEG1      12 
  #define SEG2      13
  #define SEG3      6
  #define SEG4      5
  #define SEG5      4
  #define SEG6      9
  #define SEG7      8
  #define SEGP      7
#endif

#define OFF      HIGH
#define ON       LOW
#define RETARDO_CAR  3000

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

int EscribirLed7s(int dato, int punto)
{
  static int valor;
  
  switch(dato)
  {
  case '0': //cero
  case 'O': //O mayuscula
  case 0:
    {
      writeLed7(1,1,1,1,1,1,0,punto);
      break;
    }
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
  case 'g':
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
  case 'B':
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
  case 'D':
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
  case 'c':
    {
      writeLed7(0,0,0,1,1,0,1,punto);
      break;
    }
  case 'o':
    {
      writeLed7(0,0,1,1,1,0,1,punto);
      break;
    }
  case 'r':
    {
      writeLed7(0,0,0,0,1,0,1,punto);
      break;
    }
  case BORRAR_LED:
    {
      writeLed7(0,0,0,0,0,0,0,punto);
      break;
    }
  case S1_LED:
    {
      writeLed7(1,0,0,0,0,0,0,punto);
      break;
    }
  case S2_LED:
    {
      writeLed7(0,1,0,0,0,0,0,punto);
      break;
    }
  case S3_LED:
    {
      writeLed7(0,0,1,0,0,0,0,punto);
      break;
    }
  case S4_LED:
    {
      writeLed7(0,0,0,1,0,0,0,punto);
      break;
    }
  case S5_LED:
    {
      writeLed7(0,0,0,0,1,0,0,punto);
      break;
    }
  case S6_LED:
    {
      writeLed7(0,0,0,0,0,1,0,punto);
      break;
    }
  case S7_LED:
    {
      writeLed7(0,0,0,0,0,0,1,punto);
      break;
    }
  case BAR1_LED:
    {
      writeLed7(1,0,0,0,0,0,0,punto);
      break;
    }
  case BAR2_LED:
    {
      writeLed7(1,0,0,0,0,0,1,punto);
      break;
    }
  case BAR3_LED:
    {
      writeLed7(1,0,0,1,0,0,1,punto);
      break;
    }
  }
  
  valor = dato;
  return valor;
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
    delay(RETARDO_CAR);
    writeLed7(0,1,0,0,0,0,0,0);
    delay(RETARDO_CAR);
    writeLed7(0,0,1,0,0,0,0,0);
    delay(RETARDO_CAR);
    writeLed7(0,0,0,1,0,0,0,0);
    delay(RETARDO_CAR);
    writeLed7(0,0,0,0,1,0,0,0);
    delay(RETARDO_CAR);
    writeLed7(0,0,0,0,0,1,0,0);
    delay(RETARDO_CAR);
    writeLed7(0,0,0,0,0,0,1,0);
    delay(RETARDO_CAR);
    writeLed7(0,0,0,0,0,0,0,1);
    delay(RETARDO_CAR);
    writeLed7(0,0,0,0,0,0,0,0);
    
    for(i=0;i<16;i++)
    {
      EscribirLed7s(i,1);
      delay(1000);
    }
}

void SalidaLed7s(int dato, int punto, int opcion)
{
  static int valor;
  
  if (dato == ULTIMO_DATO)
  {
    EscribirLed7s(valor, punto);
    return;
  }
  //Si está activo el menú rápido no se procesan otras peticiones
  if (g_MenuRapidoActivo)
  {
    if (opcion == MENU_RAPIDO) 
    {
      EscribirLed7s(dato, punto);
      valor = dato; 
      return;
    }
    else
        EscribirLed7s(valor, punto);
  }
  else
  {
    EscribirLed7s(dato, punto);
    valor = dato; 
  }
    
}
