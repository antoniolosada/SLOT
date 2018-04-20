
//--------------------------------------------------
//Declara puertos de entradas y salidas y variables
//--------------------------------------------------

/*
//define DEBUG(a,b) SerialPrint(a,b);
//#define DEBUG(a,b) ;
*/

#include <EEPROM.h>
#include <MemoryFree.h>

#define VER_CFG_ARRANQUE

#define ALGO_MEDIA       1
#define ALGO_PROGRESIVO  2

#define NOFIELD 0L    // Analog output with no applied field, calibrate this
#define TOMILLIGAUSS 1953L  // For A1301: 2.5mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 1953mG
#define MAX_REC  25 

//#define ARDUINO_NANO
//#define PROTOBOARD
//#define ATMEGA328
#define HERMESv1

#ifdef ARDUINO_NANO
  //#define POTE_RESET      //Si esta definido permite resetear cfg con el pote en el arranque
  #define COM_SERIE
  #define HALL_MAX  1000 // El sistema está configurador para obtener el valor mas alto del Hall en el Freno, esta macro invierte el valor si se coloca el hall con la parte plana hacia el imán
  #define PIN_MOTOR      11
  #define PIN_FRENO      10
  #define PIN_FRENO_RELE 12
  #define PIN_HALL       7
  #define PIN_CONSUMO    5
  #define PIN_BOTON      13
  #define PIN_VOLTIMETRO 7
  #define NUM_POTES      1
  #define PIN_POTE       6 
  #define PIN_POTE_1     255 
  #define PIN_POTE_2     255 
  #define PIN_POTE_3     255 

  #define MARGEN_INI_GAS  21
  #define MARGEN_ERROR_MIN_TOPE_GAS   30
  #define MARGEN_ERROR_MIN_FRENO      30
  #define VALOR_MIN_TOPE_GAS   240
  #define VALOR_TOPE_GAS       250
#endif

#ifdef ATMEGA328
  //#define COM_SERIE
  #define HALL_MAX  1000 // El sistema está configurador para obtener el valor mas alto del Hall en el Freno, esta macro invierte el valor si se coloca el hall con la parte plana hacia el imán
  #define POTE_RESET      //Si esta definido permite resetear cfg con el pote en el arranque
  #define PIN_MOTOR      10
  #define PIN_FRENO      9
  #define PIN_FRENO_RELE 13
  #define PIN_HALL       2
  #define PIN_CONSUMO    0
  #define PIN_BOTON      19
  #define PIN_VOLTIMETRO 1
  #define MARGEN_INI_GAS  21
  #define NUM_POTES      1
  #define PIN_POTE       3 
  #define PIN_POTE_1     255 
  #define PIN_POTE_2     255 
  #define PIN_POTE_3     255 

  #define MARGEN_ERROR_MIN_TOPE_GAS   30
  #define MARGEN_ERROR_MIN_FRENO      30
  #define VALOR_MIN_TOPE_GAS   240
  #define VALOR_TOPE_GAS       250
#endif

#ifdef HERMESv1
  #define COM_SERIE
  #define POTE_RESET      //Si esta definido permite resetear cfg con el pote en el arranque
  #define HALL_MAX  1000 // El sistema está configurador para obtener el valor mas alto del Hall en el Freno, esta macro invierte el valor si se coloca el hall con la parte plana hacia el imán
  #define PIN_MOTOR      10
  #define PIN_FRENO      9
  #define PIN_FRENO_RELE 13
  #define PIN_HALL       5
  #define PIN_CONSUMO    0
  #define PIN_BOTON      12
  #define PIN_VOLTIMETRO 5
  #define NUM_POTES      3
  #define PIN_POTE       3 
  #define PIN_POTE_1     255 
  #define PIN_POTE_2     255 
  #define PIN_POTE_3     255 

  #define MARGEN_INI_GAS  3
  #define MARGEN_ERROR_MIN_TOPE_GAS   5
  #define MARGEN_ERROR_MIN_FRENO      5
  #define VALOR_MIN_TOPE_GAS   240
  #define VALOR_TOPE_GAS       250
#endif

#ifdef PROTOBOARD
  #define COM_SERIE
  #define PIN_MOTOR      11
  #define PIN_FRENO      10
  #define PIN_FRENO_RELE 2
  #define PIN_HALL       0
  #define PIN_CONSUMO    2
  #define PIN_BOTON      3 //digital
  #define PIN_RPM        2
  #define PIN_INTR       2
  #define NUM_POTES      1
  #define PIN_POTE       1
  #define PIN_POTE_1     255 
  #define PIN_POTE_2     255 
  #define PIN_POTE_3     255 

  #define MARGEN_INI_GAS  21
  #define MARGEN_ERROR_MIN_TOPE_GAS   30
  #define MARGEN_ERROR_MIN_FRENO      30
  #define VALOR_MIN_TOPE_GAS   240
  #define VALOR_TOPE_GAS       250
#endif

#define EE_CFG     0
#define EE_MINMAX  2
#define EE_MAXMAX  4
#define EE_MINMIN  6
#define EE_MAXMIN  8
#define EE_REC    10

#define EE_CFG_SELEC_ACEL   62
#define EE_CFG_SELEC_FRENO  63
#define EE_CFG_SELEC_ALGO   64

#define EE_ALGOMEDIA_TIME       80
#define EE_ALGOPROG_TIME        85
#define EE_ALGOPROG_PROP_FRENO  86
#define EE_TIEMPO_MAX_FRENO_DIN 87

#define EE_CFG_FRENO       100
#define EE_CFG_ACELERACION 200

#define EE_CFG_POTES       400

#define NO_SELEC    255
#define SELEC_AUTO  254
#define CFG_OK     92

#define TOPE_GAS    0
#define TOPE_FRENO  1
#define GAS         2

#define MAX_CFG_FRENO        10
#define MAX_CFG_ACELERACION  10
#define MAX_TRAMOS_ACEL      5

#define ORDEN_PRIMERO_RELE        'r'
#define ORDEN_PRIMERO_PROGRESIVO  'P'

#define FRENO_RELE              1
#define FRENO_PROGRESIVO        2
#define ETAPA_FINAL_FRENO       3 
#define ETAPA_FINAL_FRENO_RELE  4
#define ETAPA_GAS               5
#define ETAPA_FINAL_SIN_FRENO   6 

#define MAX_TIEMPO_RELE         5000

#define BORRAR_LED  -1
#define ULTIMO_DATO  ' ' 
#define S1_LED      20
#define S2_LED      21
#define S3_LED      22
#define S4_LED      23
#define S5_LED      24
#define S6_LED      25
#define S7_LED      26

#define BAR1_LED    27
#define BAR2_LED    28
#define BAR3_LED    29

#define MENU_RAPIDO  1
#define VER_GAS      2
#define MENU_RAPIDO_TIEMPO_MAX 5000

int CfgOK = 0;
int g_punto = 0;
long g_debugCont = 0;
long g_MenuRapidoActivo = 0;
long g_TiempoMenuRapido = 0;

struct Tope
{
  int minimo;
  int maximo;
};

struct ConfFreno
{
  byte TiempoFrenoRele; // (x3 centésimas de segundo)
  byte PotenciaFrenoProgresivo; // (/2, se puede llegar solo al 50% de reduccion de potencia del freno)
  byte TiempoRecta;             // (x5 centésimas de segundo)
  byte orden;                   // r o P
} CfgFreno[MAX_CFG_FRENO];

struct TramoGas
{
  byte Gatillo;
  byte Potencia;
};

struct ConfAceleracion
{
  TramoGas Tramo[5];
  byte Sensibilidad;
  byte PotenciaMax;
} CfgAceleracion[MAX_CFG_ACELERACION];

byte SelecFreno = NO_SELEC;
byte SelecAceleracion = NO_SELEC;
byte SelecAlgoritmo = ALGO_PROGRESIVO;

struct stAlgoMedia
{
  byte Tiempo;
} AlgoMedia;

struct AlgoProgresivo
{
  byte Tiempo;
  byte ProporcionFrenado;
} AlgoProgresivo;

struct stFrenoDinamico
{
  byte TiempoPotenciaMax;
  byte Funcion;
} FrenoDinamico;

struct stCfgPotes
{
  byte Funcion[4];
  byte Pin[4];  
} CfgPote;

//Datos para normalización
Tope TopeFreno, TopeGas;
int Recorrido[MAX_REC];
int debug;

long cont = 0;


//------------------------------------
//Funcion principal
//------------------------------------
void setup() // Se ejecuta cada vez que el Arduino se inicia
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b)  ;
{
  int i;
  int boton;
  int Pote;
  
  debug = 1;
  CfgPote.Pin[0] = PIN_POTE;
  CfgPote.Pin[1] = PIN_POTE_1;
  CfgPote.Pin[2] = PIN_POTE_2;
  CfgPote.Pin[3] = PIN_POTE_3;
  
#ifdef COM_SERIE
  Serial.begin(115200); //Inicia la comunicacion serial Arduino-PC
#endif

  Serial.print("freeMemory()=");        
  Serial.println(freeMemory()); 

  InicializarLed7s();

#ifdef PIN_INTR
  pinMode(PIN_INTR, INPUT);
#endif

  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_FRENO, OUTPUT);
  pinMode(PIN_FRENO_RELE, OUTPUT);
  pinMode(PIN_BOTON, INPUT);
  digitalWrite(PIN_BOTON, HIGH); // Activa la resistencia pullup
  
  //testMotor();
  //testPulsador();
  //testPotenciometro();
  //testCaracteres();
  //testHall();
  
  EscribirLed7s('8',1);
  //Comienza en modo salida
  //digitalWrite(motor, HIGH);

  boton = digitalRead(PIN_BOTON);

#ifdef POTE_RESET
  Pote = analogRead(PIN_POTE);
  boton = 1;
  if (Pote < 20)
  { //Si arrancamos con el pote a 0 damos 4 seg para pasar el pote al máximo. Si se pasa se entra en calibración
    long ms;
    ms = millis();
    Espera(0);
    while (!Espera(4000))
    {
      EscribirLed7s((millis()-ms)/1000, 1);
      Pote = analogRead(PIN_POTE);
      if (Pote > 1000)
      {
        boton = 0;
        break;
      }
    } //while
  }
#endif
  //Si arrancamos con el botón pulsado calibramos y restauramos valores de fábrica
  CfgOK = (eeprom_read_int(EE_CFG) == CFG_OK);
  if (!CfgOK || !boton)
  {
    //testCaracteres();
    EscribirLed7s(8,0);
    
    InicializarCfgPtr((byte *)CfgFreno, sizeof(CfgFreno));
    InicializarCfgPtr((byte *)CfgAceleracion, sizeof(CfgAceleracion));
    PrecargarCfgFreno();
    PrecargarCfgAceleracion();
    GrabarCfgPtr((byte *)CfgAceleracion, sizeof(CfgAceleracion), EE_CFG_ACELERACION);
    GrabarCfgPtr((byte *)CfgFreno, sizeof(CfgFreno), EE_CFG_FRENO);
    PrecargarCfgGeneral();
    GrabarSelecCfg();
    GrabarAlgoCfg();
    GrabarFrenoDinamicoCfg();
    ConfiguracionHall();
    CfgOK = (eeprom_read_int(EE_CFG) == CFG_OK);
    PrecargarCfgPotes();
    GrabarCfgPotes();
  }
  
  if (!CfgOK)
  {
    EscribirLed7s('E',1);
    return;
  }
  
  LeerCfgPtr((byte *)CfgAceleracion, sizeof(CfgAceleracion), EE_CFG_ACELERACION);
  LeerCfgPtr((byte *)CfgFreno, sizeof(CfgFreno), EE_CFG_FRENO);
  LeerSelecCfg();
  LeerAlgoCfg();
  LeerFrenoDinamicoCfg();
  LeerCfgPotes();
  
  if (AlgoMedia.Tiempo == 0)
    AlgoMedia.Tiempo = 2;
    
  if (AlgoProgresivo.Tiempo == 0)
  {
    AlgoProgresivo.Tiempo = 0;
    AlgoProgresivo.ProporcionFrenado = 1;
  }

  RecuperarConfiguracion(Recorrido, TopeGas, TopeFreno);
  SerialPrint("CFG OK ->", CFG_OK);   

  Serial.print("freeMemory()=");        
  Serial.println(freeMemory()); 

  IniSerialPrintValor(); //Activar visualización de velocidad
 
#ifdef VER_CFG_ARRANQUE  
  VisualizarCfgAceleracion(CfgAceleracion);
  VisualizarCfgFreno(CfgFreno);
  VisualizarSelecCfg();
  VerAlgoCfg();
  VerFrenoDinamicoCfg();
  VerTopesGatillo();
#endif
}
#undef debug
 
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Funcion ciclicla
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b)  ;
{           
  int v;
  int f;
    int vc, velocidad, posicion;

    if (!CfgOK) return;

    v = LeerHallNormalizado(posicion, velocidad);
    DEBUG("V= ", v);
    
    CorregirValorHallNormalizado(posicion, velocidad, SelecAlgoritmo);
    
    ComprobarMenu(velocidad, posicion);
    
    ComprobarCfgRapida();
    
    //testLecturaRPM();
    //SerialPrintValor("", velocidad, v, 17, 254);
    //RecuperarConsumo();
    AsignarVelocidad(velocidad);
    
    SalidaLed7s(ULTIMO_DATO, g_punto, 0);
    
#undef DEBUG 
}

/******************************************************************************
int Hall()
******************************************************************************/

int Hall()
{
// measure magnetic field
  int raw = analogRead(PIN_HALL);   // Range : 0..1024

//  Uncomment this to get a raw reading for calibration of no-field point
//  Serial.print("Raw reading: ");
//  Serial.println(raw);

  long compensated = raw - NOFIELD;                 // adjust relative to no applied field 
  long gauss = compensated * TOMILLIGAUSS / 1000;   // adjust scale to Gauss

  return(gauss);
}

int LeerHall()
{
#ifdef HALL_MAX
  return(HALL_MAX-analogRead(PIN_HALL));
#else
  return(analogRead(PIN_HALL));
#endif

}

int LeerHallNormalizado(int &Posicion, int &valor_normalizado)
{
  int valor;
  //Comprobamos si está dentro del máximo o mínimo
  //Hay que tener en cuenta que en la lectura del Hall el valor mas alto coincide con la posición de freno
  
  valor = LeerHall();
  
  Posicion = GAS;

  if (valor <= TopeGas.maximo)
  {
    valor_normalizado = 255;
    Posicion = TOPE_GAS;
  }
  else if (valor >= TopeFreno.minimo)
  {
    Posicion = TOPE_FRENO;
    valor_normalizado = 0;
  }
  else
  {
      //Normalizamos el valor
      //Localizamos la recta de normalización
      int i, imin=0, imax=MAX_REC;
    
      //SerialPrint("valor=", valor);
      i = (imax+imin)/2;
      while((imax != imin) && (i != imin))
      {
        if (valor <= Recorrido[i])
           imin = i;
        else
           imax = i;
        i = (imax+imin)/2;
          
        //SerialPrint("imax=",imax, " imin=", imin);
        //SerialPrint("Rec i=", Recorrido[i], " i=", i);
      }
      if (i==0) //El valor es mayor que el mas alto
      {
        Posicion = TOPE_FRENO;
        valor_normalizado = 0;
      }
      else if ((i==MAX_REC) && (valor < Recorrido[MAX_REC]))
      {
        Posicion = TOPE_GAS;
        valor_normalizado = 255;
      }
      else
      {
        /*SerialPrint("Rec i=", Recorrido[i], " i=", i);
        SerialPrint("Rec i=", Recorrido[i+1], " i=", i+1);
        SerialPrint("valor=", valor);*/
        valor_normalizado = i*10+map(valor, Recorrido[i], Recorrido[i+1], 0, 10);
        if (valor_normalizado>255) 
          valor_normalizado= 255;
        Posicion = GAS;
      }
  }
  return valor;
}

void testHall()
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
while(1)
  {
    int h, p;
    
    h = LeerHall();
    Serial.print("H="); 
    Serial.println(h); 
    delay(500);
  }
  
}
#undef DEBUG

void testMotor()
{
  Serial.println("ON");
  digitalWrite(PIN_FRENO_RELE, LOW);
  digitalWrite(PIN_FRENO, LOW);
  digitalWrite(PIN_MOTOR, HIGH);
  delay(2000);
  Serial.println("OFF");
  digitalWrite(PIN_MOTOR, LOW);
  delay(1000);
  Serial.println("ON");
  digitalWrite(PIN_MOTOR, HIGH);
  delay(2000);
  Serial.println("OFF");
  digitalWrite(PIN_MOTOR, LOW);
  digitalWrite(PIN_FRENO, HIGH);
  delay(1000);
}

void testPulsador()
{
  int v;
   while(1)
  {
    v = digitalRead(PIN_BOTON);
    Serial.print("p=");
    Serial.println(v);
    delay(1000);
  } 
}


void testPotenciometro()
{
  int v;
   while(1)
  {
    v = analogRead(PIN_POTE);
    Serial.print("p=");
    Serial.println(v);
    delay(1000);
  } 
}

int LeerPote()
{
  return analogRead(PIN_POTE);
}

int LeerBoton()
{
  return digitalRead(PIN_BOTON);
}

int BotonPulsado()
{
  int boton;
  
  boton = digitalRead(PIN_BOTON);
  if (boton)
    return 0;
  else
    return 1;
}

void RecuperarConsumo()
{
  static long acumulador;
  static long valores;
  int valor;
  
  valor = analogRead(PIN_CONSUMO);
  
  acumulador += valor;
  valores++;
  
  if (valores == 2)
  {
    Serial.println(acumulador/valores);
    valores = 0;
    acumulador = 0;
  }
  
}

void testLecturaRPM()
{
  static byte a[30];
  static int m[30];
  static int i = 0;
  static long ms = 0;
  
  Serial.println(analogRead(5));
  return;
  

  i = (i+1) % 30;
  
  if (i)
  {
    a[i] = (byte)digitalRead(18);
    m[i] = micros()-ms;
    ms = micros();
  }
  else
  {
    for (i=0; i<30;i++)
    {
      Serial.print(a[i]);
      Serial.print("  , ");
      Serial.println(m[i]);
    }
    i = 0;
  }
}


void MostrarValor(int pin_analog, int ms)
{
  int valor;
    EscribirLed7s(8,0);

    Espera(0);
    while (!Espera(ms))
    {
      valor = analogRead(pin_analog);
      VerValorLed7s(valor);
    }
}

void VerValorLed7s(int valor)
{
    int digito;
    int punto = 0;

    digito = valor/1000;
    valor = valor%1000;
    EscribirLed7s(BORRAR_LED,punto);
    delay(200);
    EscribirLed7s(digito,punto);
    delay(1000);
    digito = valor/100;
    valor = valor%100;
    EscribirLed7s(BORRAR_LED,punto);
    delay(200);
    EscribirLed7s(digito,punto);
    delay(1000);
    digito = valor/10;
    valor = valor%10;
    EscribirLed7s(BORRAR_LED,punto);
    delay(200);
    punto = 1;
    EscribirLed7s(digito,punto);
    delay(1000);
    EscribirLed7s(BORRAR_LED,punto);
    delay(200);
    EscribirLed7s(valor,punto);
    delay(1000);
    if (punto) 
      punto = 0;
    else
      punto = 1;
}

void TestMotorFreno()
{
  EscribirLed7s('g',1);
  digitalWrite(PIN_FRENO, LOW);
  digitalWrite(PIN_MOTOR, HIGH);
  delay(5000);
  EscribirLed7s('F',1);
  digitalWrite(PIN_MOTOR, LOW);
  digitalWrite(PIN_FRENO, HIGH);
  delay(5000);
  EscribirLed7s('0',1);
  digitalWrite(PIN_MOTOR, LOW);
  digitalWrite(PIN_FRENO, LOW);
  return;

}
