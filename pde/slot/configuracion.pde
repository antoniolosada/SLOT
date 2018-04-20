/* Contiene todas las funciones de configuración:
   1. Configuración inicial y ajuste del Hall
   2. Modo de menús
*/

/*
    -. Entramos en modo configuraicón Hall con el gatillo al mínimo
    -. Visualizamos en el led que comenzamos la lectura de la posición de reposo
    -. Durante 5 segundos estabilizamos el máximo y mínimo valor recuperado del hall de la posición mínima
    -. Informamos del final de la lectura de la posición de reposo
    -. Ahora el usuario debe realizar un gatillazo de la posición de reposo a la de máxima velocidad a velocidad constante con un tiempo 
       de recorrido de al menos 1 segundo
    -. Informamos en el led que hemos realizado la lectura
    -. Ahora debe estar sobre 5 segundos en la posición de máxima velocidad para recuperar sus valores máximo y mínimo
    -. Finalmente informamos con un O(k) si se ha leído correctamente o con un E(rror) en caso de que 
*/

#include <EEPROM.h>

#define I_MAX  0
#define I_MIN  1

#define ERROR_MIN  10
#define ERROR_MAX  10
#define MAX_MS_RECORRIDO 3000

int ConfiguracionHall()
{
  int op, i;
  int MinMin, MaxMin, MinMax, MaxMax;
  int punto;
  int Recorrido[40];
  
  //REcuperamos el mínimo
  Serial.println("Recuperando posicion de reposo");
  op = LeerTope(MinMin, MaxMin, ERROR_MIN);
  if (!op) return 0;
  
  Serial.println("Recuperando posicion de maxima aceleracion");
  op = EsperaSiguienteMedida(MinMin-30, 5);
  if (!op) return 0;
  //Una vez superado esperamos dos segundos para que llegue al máximo
  delay(1000);
  op = LeerTope(MinMax, MaxMax, ERROR_MAX);
  if (!op) return 0;
  //Una vez superado esperamos dos segundos para que llegue de nuevo a la posición de reposo
  delay(1000);
  
  //Ahora comprobamos el recorrido
  //Comienza cuando se supera el 50% del márgen entre en valor mínimo y máximo de la posición de reposo
  Serial.println("Recuperando posiciones de recorrido");
  op = EsperaSiguienteMedida(MinMin-(MaxMin-MinMin)-1, 10);
  if (!op) return 0;
  op = LeerRecorrido(Recorrido, MAX_REC, MaxMax, MAX_MS_RECORRIDO);
  if (!op) return 0;
  
  //Almacenamos la información en la EEPROM
  eeprom_write_int(EE_CFG, CFG_OK);  //Configuración OK
  eeprom_write_int(EE_MAXMAX, MaxMax);  
  eeprom_write_int(EE_MINMAX, MinMax);  
  eeprom_write_int(EE_MAXMIN, MaxMin);  
  eeprom_write_int(EE_MINMIN, MinMin);  
  
  for (i=0; i<MAX_REC; i++)
  {
    eeprom_write_int(EE_REC+i*2, Recorrido[i]);  
  }
  
  SerialPrint("Informacion almacenada");
}

int LeerTope(int &Min, int &Max, int Error)
{
  int valor;
  int humbral;
  int segundo;
  long ms;

  //Durante 5 segundos comprobamos mínimos
  segundo = 0;
  Min = 32000;
  Max = -32000;
  ms = millis();
  while (millis()-ms < 5000)
  {
    segundo = (millis()-ms)/1000;
    EscribirLed7s(segundo, 1);
    valor = LeerHall();
    if (valor > Max) 
      Max = valor;
    else if (valor < Min) 
      Min = valor;
  }
  if (Max - Min > Error)
  {
    //La desviación entre el valor mínimo y máximo excede de los parámetros
    EscribirLed7s('E',0);
    return 0;
  }
  else
  {
    Serial.print("Valor Minimo = ");
    Serial.println(Min);
    Serial.print("Valor Maximo = ");
    Serial.println(Max);
    Serial.println("Lectura OK");

    EscribirLed7s(0,0);
  }
  delay(2000);
  
  return 1;
}

int EsperaSiguienteMedida(int humbral, int MaxEspera)
{
  int valor;
  long ms;
  int punto;
  long parpadeo;
  
  //Ahora comprobamos los máximos
  //Comienza cuando se superan 100 unidades o falla si pasan 10 segundos
  ms = millis();
  parpadeo = millis();
  punto = 1;
  while ((valor > humbral) && (millis()-ms < MaxEspera*1000))
  {
    writeLed7(0,0,0,0,0,0,0,punto);
    if (millis() - parpadeo > 200)
    {
      parpadeo = millis();
      punto = !punto;
    }
    valor = LeerHall();
  }
  if (millis()-ms >= MaxEspera*1000)
  {
    Serial.println("ERROR: Tiempo de reconocimiento de maximo excedido");
    return 0;
  }
  return 1;
}

int LeerRecorrido(int *Recorrido, int MaxPos, int MaxMax, int MaxEspera)
{
  int Lecturas[200];
  long ms;
  long tiempo;
  long medida;
  int valor;
  int Pos;
  int i,j;
  
  //Se estima que cada lectura del recorrido dura menos de 2 segundos
  //Cada 10 ms recuperamos una medida
  valor = LeerHall();
  Pos = 0;
  tiempo = 0;
  medida = tiempo;
  ms = millis();
  while ((tiempo < MaxEspera) && (valor > MaxMax) && (Pos < 200))
  {
    tiempo = millis()-ms;

    if (millis()-medida >= 10)
    {
      medida = millis();
      Lecturas[Pos++] = valor;
    }
    
    valor = LeerHall();
    
    EscribirLed7s((millis()-tiempo)/500, 0);
  }
  if (Pos < MaxPos)
  {
    Serial.print("ERROR: Operacion demasiado rapida. No se han recuperado los valores suficientes. Valores = ");
    Serial.println(Pos);
    return 0;
  }
  if (Pos == 200)
  {
    Serial.println("ERROR: Numero maximo de valores excedido. Tiempo maximo de recorrido excedido");
    return 0;
  }
  if (tiempo >= MaxEspera)  
  {
    Serial.println("ERROR: Tiempo maximo de recorrido excedido");
    return 0;
  }
  
  //La lectura fué correcta. Ahora hay que ajustarla al número de valores del array que nos entregan
  Serial.print("Numero de posiciones: ");
  Serial.println(Pos);
  
  //Recuperamos los valores indicados en el array de entrada
  j = 0;
  for (i=0; i<MaxPos;i++)
  {
    j = i*Pos/MaxPos;
    Recorrido[i] = Lecturas[j];
  }
  
  return 1;
}

void eeprom_write_int(int pos, int dato)
{
  EEPROM.write(pos, dato/256);
  EEPROM.write(pos+1, dato%256);
}

int eeprom_read_int(int pos)
{
  int valor;
  valor = EEPROM.read(pos);
  valor *= 256;
  valor += EEPROM.read(pos+1);
  return valor;
}

int RecuperarConfiguracion(int *Recorrido, struct Tope &TopeGas, struct Tope &TopeFreno)
{
  int Cfg, i;
  
  Cfg = eeprom_read_int(EE_CFG);
  
  TopeGas.maximo = eeprom_read_int(EE_MAXMAX);
  TopeGas.minimo = eeprom_read_int(EE_MINMAX);  
  TopeFreno.maximo = eeprom_read_int(EE_MAXMIN);  
  TopeFreno.minimo = eeprom_read_int(EE_MINMIN);  

  SerialPrint("TopeGas.maximo=", TopeGas.maximo);
  SerialPrint("TopeGas.minimo=", TopeGas.minimo);  
  SerialPrint("TopeFreno.maximo=", TopeFreno.maximo);  
  SerialPrint("TopeFreno.minimo=", TopeFreno.minimo);  
  
  for (i=0; i<MAX_REC; i++)
  {
    Recorrido[i] = eeprom_read_int(EE_REC+i*2);  
    SerialPrint("Recorrido=", Recorrido[i], " i=", i);  
  }
}


