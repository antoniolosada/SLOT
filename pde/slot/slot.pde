
//--------------------------------------------------
//Declara puertos de entradas y salidas y variables
//--------------------------------------------------

#include <EEPROM.h>

int motor=5;     //Declara Pin del motor
long cont = 0;
#define NOFIELD 0L    // Analog output with no applied field, calibrate this
#define TOMILLIGAUSS 1953L  // For A1301: 2.5mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 1953mG
#define MAX_REC  25 

#define EE_CFG     0
#define EE_MINMAX  2
#define EE_MAXMAX  4
#define EE_MINMIN  6
#define EE_MAXMIN  8
#define EE_REC    10
#define CFG_OK     91

#define MOTOR      5
#define FRENO      11
#define FRENO_RELE 3

#define TOPE_GAS    0
#define TOPE_FRENO  1
#define GAS         2

struct Tope
{
  int minimo;
  int maximo;
};

//Datos para normalización
Tope TopeFreno, TopeGas;
int Recorrido[MAX_REC];

//------------------------------------
//Funcion principal
//------------------------------------
void setup() // Se ejecuta cada vez que el Arduino se inicia
{
  int i;
  
  Serial.begin(9600); //Inicia la comunicacion serial Arduino-PC
  
  
  pinMode(MOTOR, OUTPUT);
  pinMode(FRENO, OUTPUT);
  pinMode(FRENO_RELE, OUTPUT);
  
  InicializarLed7s();
  testLed7();
  
  if (eeprom_read_int(EE_CFG) != CFG_OK)
    ConfiguracionHall();
  else
  {
    RecuperarConfiguracion(Recorrido, TopeGas, TopeFreno);
    SerialPrint("Configuracion OK ->", CFG_OK);   
  }
  
}
 
//------------------------------------
//Funcion ciclicla
//------------------------------------
void loop() // Esta funcion se mantiene ejecutando
{           //  cuando este energizado el Arduino
 
  // Si hay algun valor en la Consola Serial
  if (Serial.available()){
 
      //Variable donde se guarda el caracter enviado desde teclado
      char a = Serial.read();
 
      if (a=='c')
      {
         ConfiguracionHall();
      }

     if (a=='a'){ 
          analogWrite(motor,0);
          delay(400);
          digitalWrite(3, HIGH);
     }
     else if (a=='b'){ 
          analogWrite(motor,0);
          delay(400);
          digitalWrite(3, LOW);
     }
 
 
 
      // Si el caracter ingresado esta entre 0 y 5
      if (a>='0' && a<='5'){ 
 
        //Variable para escalar el valor ingresado a rango de PWM
        
        int velocidad = map(a,'0','5',0,255);
        //Escritura de PWM al motor
        analogWrite(motor,velocidad);
        if (a == '0') 
        {
          
          delay(2);
          digitalWrite(3, HIGH);
          delay(200);
          digitalWrite(3, LOW);
          digitalWrite(11, HIGH);
          delay(3000);
          digitalWrite(11, LOW);
        }
        //Mensaje para el usuario
        Serial.print("El motor esta girando a la velocidad ");
        Serial.println(a);
 
      }else{ // Si el caracter ingresado NO esta entre 0 y 5
 
        //Mensaje para el usuario
        Serial.print("Velocidad invalida");
        Serial.println(a);
 
      }
 
  }
  else
  {
    int velocidad, posicion;
    
    LeerHallNormalizado(posicion, velocidad);
    
    SerialPrint("Velocidad normalizada = ", velocidad);
    AsignarVelocidad(velocidad, 0);
    
    ComprobarMenu();
    
    //delay(500);   
    
/*    Serial.print("Valor = ");
    Serial.println(hall);
    Serial.println(h);
    delay(200);*/
  }
 
}

void AsignarVelocidad(int v, int freno)
{
  static int v1 = 0;
  
    if (v > 0)
    {
      digitalWrite(FRENO, LOW);      
      digitalWrite(FRENO_RELE, LOW);      
    }

    analogWrite(motor,v);

    if (v == 0)
     if (freno == 1)
      {
        delay(2);
        digitalWrite(11, HIGH);
        delay(200);
        digitalWrite(11, LOW);
      }
      else if (freno == 2)
      {
        delay(2);
        digitalWrite(3, HIGH);
        delay(200);
        digitalWrite(3, LOW);
      }
}

int Hall()
{
// measure magnetic field
  int raw = analogRead(0);   // Range : 0..1024

//  Uncomment this to get a raw reading for calibration of no-field point
//  Serial.print("Raw reading: ");
//  Serial.println(raw);

  long compensated = raw - NOFIELD;                 // adjust relative to no applied field 
  long gauss = compensated * TOMILLIGAUSS / 1000;   // adjust scale to Gauss

  return(gauss);
}

int LeerHall()
{
  return(analogRead(0));
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
