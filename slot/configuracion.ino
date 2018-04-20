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

#define ERROR_MIN  25
#define ERROR_MAX  25
#define MAX_MS_RECORRIDO 3000

int ConfiguracionHall()
#define DEBUG(a,b) SerialPrint(a,b);
//define DEBUG(a,b) ;
{
  int op, i;
  int MinMin, MaxMin, MinMax, MaxMax;
  int punto;
  int Recorrido[40];
  
  delay(2000);
  //REcuperamos el mínimo
  DEBUG("Recuperando posicion de reposo","")
  op = LeerTope(MinMin, MaxMin, ERROR_MIN);
  if (!op) return 0;
  DEBUG("Freno, Min =",MinMin)
  DEBUG("Freno, Max =",MaxMin)
  
  DEBUG("Recuperando posicion de maxima aceleracion","");
  op = EsperaSiguienteMedida(MinMin-MARGEN_ERROR_MIN_FRENO, 5);
  if (!op) return 0;
  //Una vez superado esperamos dos segundos para que llegue al máximo
  delay(2000);
  op = LeerTope(MinMax, MaxMax, ERROR_MAX);
  if (!op) return 0;
  DEBUG("TopeGas, Min =",MinMax)
  DEBUG("TopeGas, Max =",MaxMax)
  //Una vez superado esperamos dos segundos para que llegue de nuevo a la posición de reposo
  delay(2000);
  
  //Ahora comprobamos el recorrido
  //Comienza cuando se supera Valor mínimo de reposo menos el margen de error
  DEBUG("Recuperando posiciones de recorrido","");
  op = EsperaSiguienteMedida(MinMin-MARGEN_ERROR_MIN_FRENO, 10);
  if (!op) return 0;
  op = LeerRecorrido(Recorrido, MAX_REC, MaxMax+MARGEN_ERROR_MIN_TOPE_GAS, MAX_MS_RECORRIDO);
  if (!op) return 0;
  
  //Almacenamos la información en la EEPROM
  eeprom_write_int(EE_CFG, CFG_OK);  //Configuración OK
  eeprom_write_int(EE_MAXMAX, MaxMax);  
  eeprom_write_int(EE_MINMAX, MinMax+MARGEN_ERROR_MIN_TOPE_GAS);  
  eeprom_write_int(EE_MAXMIN, MaxMin);  
  eeprom_write_int(EE_MINMIN, MinMin-MARGEN_ERROR_MIN_FRENO);  
  
  for (i=0; i<MAX_REC; i++)
  {
    eeprom_write_int(EE_REC+i*2, Recorrido[i]);  
  }
  DEBUG("Informacion almacenada","");
}
#undef DEBUG

int LeerTope(int &Min, int &Max, int Error)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b)  ;
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
  DEBUG("Valor Minimo = ", Min)
  DEBUG("Valor Maximo = ", Max)
  if (Max - Min > Error)
  {
    //La desviación entre el valor mínimo y máximo excede de los parámetros
    DEBUG("Parametros excedidos.","")
    EscribirLed7s('E',0);
    return 0;
  }
  else
  {
    DEBUG("Lectura OK","")
    EscribirLed7s(0,0);
  }
  delay(2000);
  
  return 1;
}
#undef DEBUG

int EsperaSiguienteMedida(int humbral, int MaxEspera)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b)  ;
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
  valor = LeerHall();
  
  DEBUG("Valor = ", valor);
  DEBUG("Humbral = ", humbral);
  while ((valor > humbral) && (millis()-ms < MaxEspera*1000))
  {
    if (punto)
      writeLed7(0,0,0,0,0,0,1,punto);
    else
      writeLed7(0,0,0,0,0,0,0,punto);
    if (millis() - parpadeo > 200)
    {
      parpadeo = millis();
      punto = !punto;
    }
    valor = LeerHall();

  DEBUG("Valor = ", valor);
  DEBUG("Humbral = ", humbral);
  }
  if (millis()-ms >= MaxEspera*1000)
  {
    DEBUG("ERROR: Tiempo de reconocimiento de maximo excedido", "");
    return 0;
  }
  return 1;
}
#undef DEBUG

int LeerRecorrido(int *Recorrido, int MaxPos, int MaxMax, int MaxEspera)
#define DEBUG(a,b) SerialPrint(a,b);
//define DEBUG(a,b)  ;
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
  DEBUG("Inicio de lectura de recorrido",valor);
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
    
    EscribirLed7s(tiempo/500, 0);
  }
  DEBUG("Tiempo=", tiempo);
  if (Pos < MaxPos)
  {
    DEBUG("ERROR: Operacion demasiado rapida. No se han recuperado los valores suficientes. Valores = ",Pos);
    return 0;
  }
  if (Pos == 200)
  {
    DEBUG("ERROR: Numero maximo de valores excedido. Tiempo maximo de recorrido excedido","");
    return 0;
  }
  if (tiempo >= MaxEspera)  
  {
    DEBUG("ERROR: Tiempo maximo de recorrido excedido","");
    return 0;
  }
  
  //La lectura fué correcta. Ahora hay que ajustarla al número de valores del array que nos entregan
  DEBUG("Numero de posiciones: ", Pos);
  
  //Recuperamos los valores indicados en el array de entrada
  j = 0;
  for (i=0; i<MaxPos;i++)
  {
    j = i*Pos/MaxPos;
    Recorrido[i] = Lecturas[j];
    DEBUG("Lectura =", Lecturas[j]); 
  }
  
  return 1;
}
#undef DEBUG

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
  
  debug = 1;
  Cfg = eeprom_read_int(EE_CFG);
  
  TopeGas.maximo = eeprom_read_int(EE_MAXMAX);
  TopeGas.minimo = eeprom_read_int(EE_MINMAX);  
  TopeFreno.maximo = eeprom_read_int(EE_MAXMIN);  
  TopeFreno.minimo = eeprom_read_int(EE_MINMIN);  

#ifdef DEBUG
  SerialPrint("TopeGas.maximo=", TopeGas.maximo);
  SerialPrint("TopeGas.minimo=", TopeGas.minimo);  
  SerialPrint("TopeFreno.maximo=", TopeFreno.maximo);  
  SerialPrint("TopeFreno.minimo=", TopeFreno.minimo);  
#endif  
  
  for (i=0; i<MAX_REC; i++)
  {
    Recorrido[i] = eeprom_read_int(EE_REC+i*2);  
#ifdef DEBUG
    SerialPrint("Recorrido=", Recorrido[i], " i=", i);  
#endif    
  }
  return 0;
}

int GrabarCfgPtr(byte *Cfg, int longitud, int eeprom_ini)
{
  int i;
  
  for (i=0; i<longitud; i++)
  {
    EEPROM.write(eeprom_ini+i, Cfg[i]);
  }
}

int LeerCfgPtr(byte *Cfg, int longitud, int eeprom_ini)
{
  int i;
  
  for (i=0; i<longitud; i++)
  {
    Cfg[i] = EEPROM.read(eeprom_ini+i);
  }
}

int InicializarCfgPtr(byte *Cfg, int longitud)
{
  int i;

  for (i=0; i<longitud; i++)
  {
    Cfg[i] = 0;
  }
}

void VerTopesGatillo()
{
  SerialPrint("GM=", TopeGas.maximo);
  SerialPrint("Gm=", TopeGas.minimo);  
  SerialPrint("FM=", TopeFreno.maximo);  
  SerialPrint("Fm=", TopeFreno.minimo);  
  
}

void GrabarCfgPotes()
{
  GrabarCfgPtr((byte *)&CfgPote, sizeof(CfgPote), EE_CFG_POTES);
}
void LeerCfgPotes()
{
  LeerCfgPtr((byte *)&CfgPote, sizeof(CfgPote), EE_CFG_POTES);
}
void PrecargarCfgPotes()
{
  CfgPote.Funcion[0] = 'r';
  CfgPote.Funcion[1] = 'b';
  CfgPote.Funcion[2] = 'b';
  CfgPote.Funcion[3] = 'b';
}
int GrabarSelecCfg()
{
  EEPROM.write(EE_CFG_SELEC_ACEL, SelecAceleracion);
  EEPROM.write(EE_CFG_SELEC_FRENO, SelecFreno);
}
int LeerSelecCfg()
{
  SelecAceleracion = EEPROM.read(EE_CFG_SELEC_ACEL);
  SelecFreno = EEPROM.read(EE_CFG_SELEC_FRENO);
}

void GrabarFrenoDinamicoCfg()
{
  EEPROM.write(EE_TIEMPO_MAX_FRENO_DIN, FrenoDinamico.TiempoPotenciaMax);
}

void LeerFrenoDinamicoCfg()
{
  FrenoDinamico.TiempoPotenciaMax = EEPROM.read(EE_TIEMPO_MAX_FRENO_DIN);
}

void VerFrenoDinamicoCfg()
{
  Serial.print("FDT=");
  Serial.println(FrenoDinamico.TiempoPotenciaMax);
}

void GrabarAlgoCfg()
{
  EEPROM.write(EE_CFG_SELEC_ALGO, SelecAlgoritmo);
  EEPROM.write(EE_ALGOMEDIA_TIME, AlgoMedia.Tiempo);
  EEPROM.write(EE_ALGOPROG_TIME, AlgoProgresivo.Tiempo);
  EEPROM.write(EE_ALGOPROG_PROP_FRENO, AlgoProgresivo.ProporcionFrenado);
}
void LeerAlgoCfg()
{
  SelecAlgoritmo = EEPROM.read(EE_CFG_SELEC_ALGO);
  AlgoMedia.Tiempo = EEPROM.read(EE_ALGOMEDIA_TIME);
  AlgoProgresivo.Tiempo = EEPROM.read(EE_ALGOPROG_TIME);
  AlgoProgresivo.ProporcionFrenado = EEPROM.read(EE_ALGOPROG_PROP_FRENO);
}


void VerAlgoCfg()
{
  Serial.print("Algo=");
  Serial.println(SelecAlgoritmo);
  Serial.print("AMT=");
  Serial.println(AlgoMedia.Tiempo);
  Serial.print("APT=");
  Serial.print(AlgoProgresivo.Tiempo);
  Serial.print(", PF=");
  Serial.println(AlgoProgresivo.ProporcionFrenado);
}


int VisualizarCfgAceleracion(struct ConfAceleracion *Cfg)
{
  int i,j;
  
  Serial.print("-A=>");
  Serial.println(SelecAceleracion);
  for(i=0; i<MAX_CFG_ACELERACION; i++)
  {
    if (Cfg[i].Tramo[0].Gatillo>0)
    {
      Serial.print("C->");
      Serial.println(i);
      Serial.print("S->");
      Serial.println(Cfg[i].Sensibilidad);
    }   
    for (j=0;(j<MAX_TRAMOS_ACEL);j++)
    {
      if (Cfg[i].Tramo[j].Gatillo==0)
        break;
      Serial.print("%G: ");
      Serial.print(Cfg[i].Tramo[j].Gatillo);
      Serial.print(" %P: ");
      Serial.println(Cfg[i].Tramo[j].Potencia);
    }
  }
        Serial.println("---");
}

int VisualizarCfgFreno(struct ConfFreno *Cfg)
{
  int i;
  Serial.print("-F=>");
  Serial.println(SelecFreno);
  for(i=0; i<MAX_CFG_FRENO; i++)
  {
    if (Cfg[i].orden!=0) 
    {
      Serial.print("C->");
      Serial.println(i);
      Serial.print("TFreno(ms):");
      Serial.println(Cfg[i].TiempoFrenoRele*30);
      Serial.print("FrPro(%):");
      Serial.println(100-Cfg[i].PotenciaFrenoProgresivo/2);
      Serial.print("T.Recta(ms): ");
      Serial.println(Cfg[i].TiempoRecta*50);
      Serial.print("Orden: ");
      Serial.println((char)Cfg[i].orden);
    }
  }
  Serial.println("---");
  return 0;
}

void VisualizarSelecCfg()
{
#define NDEBUG
  
#ifdef DEBUG
  Serial.print("Selec Aceleracion: ");
  Serial.println(SelecAceleracion);
  Serial.print("Selec Freno: ");
  Serial.println(SelecFreno);
#endif  

#undef DEBUG
}

void PrecargarCfgGeneral()
{
  SelecFreno = NO_SELEC;
  SelecAceleracion = NO_SELEC;
  SelecAlgoritmo = ALGO_MEDIA;
  AlgoMedia.Tiempo = MS_MEDIA_LECTURAS;
  AlgoProgresivo.Tiempo = RETARDO_VALOR_CORRECCION;
  AlgoProgresivo.ProporcionFrenado = PROPORCION_FRENADO;
}

void PrecargarCfgAceleracion()
{
   //---------------------------------------  
   CfgAceleracion[0].Tramo[0].Gatillo =  20;
   CfgAceleracion[0].Tramo[0].Potencia = 30;
   CfgAceleracion[0].Tramo[1].Gatillo =  60;
   CfgAceleracion[0].Tramo[1].Potencia = 80;
   CfgAceleracion[0].Sensibilidad = 10;
   CfgAceleracion[0].PotenciaMax = 99;
   //---------------------------------------  
   CfgAceleracion[1].Tramo[0].Gatillo =  20;
   CfgAceleracion[1].Tramo[0].Potencia = 30;
   CfgAceleracion[1].Tramo[1].Gatillo =  70;
   CfgAceleracion[1].Tramo[1].Potencia = 60;
   CfgAceleracion[1].Sensibilidad = 20;
   CfgAceleracion[1].PotenciaMax = 99;
   //---------------------------------------  
   CfgAceleracion[2].Tramo[0].Gatillo =  20;
   CfgAceleracion[2].Tramo[0].Potencia = 30;
   CfgAceleracion[2].Tramo[1].Gatillo =  80;
   CfgAceleracion[2].Tramo[1].Potencia = 70;
   CfgAceleracion[2].Sensibilidad = 10;
   CfgAceleracion[2].PotenciaMax = 99;
   //---------------------------------------  
   CfgAceleracion[3].Tramo[0].Gatillo =  20;
   CfgAceleracion[3].Tramo[0].Potencia = 30;
   CfgAceleracion[3].Tramo[1].Gatillo =  90;
   CfgAceleracion[3].Tramo[1].Potencia = 70;
   CfgAceleracion[3].Sensibilidad = 10;
   CfgAceleracion[3].PotenciaMax = 99;
   //---------------------------------------  
   CfgAceleracion[4].Tramo[0].Gatillo =  0;
   CfgAceleracion[4].Tramo[0].Potencia = 0;
   CfgAceleracion[4].Tramo[1].Gatillo =  0;
   CfgAceleracion[4].Tramo[1].Potencia = 0;
   CfgAceleracion[4].Sensibilidad = 0;
   CfgAceleracion[4].PotenciaMax = 0;
}

void PrecargarCfgFreno()
{
  //------------------------------------
  CfgFreno[0].TiempoFrenoRele = 0; // Freno progresivo maximo tiempo ilimitado
  CfgFreno[0].PotenciaFrenoProgresivo = 0;
  CfgFreno[0].TiempoRecta = 0;
  CfgFreno[0].orden = 'p';  
  //------------------------------------
  CfgFreno[1].TiempoFrenoRele = 0; // Relé máximo tiempo
  CfgFreno[1].PotenciaFrenoProgresivo = 0;
  CfgFreno[1].TiempoRecta = 0;
  CfgFreno[1].orden = 'r';  
  //------------------------------------
  CfgFreno[2].TiempoFrenoRele = 2; // 6 centésimas
  CfgFreno[2].PotenciaFrenoProgresivo = 20;
  CfgFreno[2].TiempoRecta = 0;
  CfgFreno[2].orden = 'r';  
  //------------------------------------
  CfgFreno[3].TiempoFrenoRele = 5; // 15 centésimas
  CfgFreno[3].PotenciaFrenoProgresivo = 0;
  CfgFreno[3].TiempoRecta = 0;
  CfgFreno[3].orden = 'r';  
  //------------------------------------
  CfgFreno[4].TiempoFrenoRele = 5; // 15 centésimas
  CfgFreno[4].PotenciaFrenoProgresivo = 10;
  CfgFreno[4].TiempoRecta = 0;
  CfgFreno[4].orden = 'r';  
  //------------------------------------
  CfgFreno[5].TiempoFrenoRele = 10; // 30 centésimas
  CfgFreno[5].PotenciaFrenoProgresivo = 0;
  CfgFreno[5].TiempoRecta = 0;
  CfgFreno[5].orden = 'r';  
  //------------------------------------
  CfgFreno[6].TiempoFrenoRele = 33; // un segundo
  CfgFreno[6].PotenciaFrenoProgresivo = 0;
  CfgFreno[6].TiempoRecta = 0;
  CfgFreno[6].orden = 'r';  
  //------------------------------------
  CfgFreno[7].TiempoFrenoRele = 50; // un segundo y medio
  CfgFreno[7].PotenciaFrenoProgresivo = 0;
  CfgFreno[7].TiempoRecta = 0;
  CfgFreno[7].orden = 'r';  
  //------------------------------------
  CfgFreno[8].TiempoFrenoRele = 0; 
  CfgFreno[8].PotenciaFrenoProgresivo = 0;
  CfgFreno[8].TiempoRecta = 0;
  CfgFreno[8].orden = 'r';  
}

