#define MARGEN_VALOR_POTE 30

#define MARGEN_TIEMPO      1000
#define MARGEN_MIN_TIEMPO  100
#define MARGEN_GAS         250
#define MARGEN_MIN_GAS     50
#define PULSACIONES_MENU   3
#define ESPERA_PULSACION_MENU 7000
#define PULSACION_FRENO    1
#define PULSACION_GAS      2
#define OPCION_ULTIMA      10
#define OPCION_SALIDA      20
#define MAX_NIVELES_MENU   6 

#define MENU_MIN_VALOR     120
#define MENU_MAX_VALOR     150
#define MENU_TIEMPO        3000
#define MENU_TIEMPO_CONFIRM  500
#define BOTON_MANGEN_PULSACION 200
#define EST_ESPERA_MENU          1
#define EST_ESPERA_CONFIRMACION  2

#define SALTO_OPCION     1
#define SALTO_SIGUIENTE  2

#define POTE_MOV_MENOS_MARGEN 30
#define POTE_MOV_MAS_MARGEN 30
#define POTE_TIEMPO_MAX  1000
#define MARGEN_ERROR_MIN_POTE 2
#define MARGEN_ERROR_MAX_POTE 8

#define BT_POTE_PULSADO_BOTON      1
#define BT_POTE_ESPERA_PULSACION   2 
#define BT_POTE_ESPERA             3
#define BT_POTE_ESPERA_ACTIVACION  4
#define BT_POTE_LIM_MIN            30
#define BT_POTE_ESPERA_MENU        1000
#define MENU_RAPIDO_ESPERA_POTE    3000
#ifdef ARDUINO_NANO
  #define BT_POTE_VALOR_MEDIO        289
#else
  #define BT_POTE_VALOR_MEDIO        512
#endif

#define POTE_MOV_MENOS 11
#define POTE_MOV_MAS   12
#define PULSADO_BOTON  10

struct stDatosMenuRapido
{
  byte SelecFreno;
  byte SelecAceleracion;
  byte Sensibilidad;
  byte FrenoProgresivo;
} DatosMenuRapido;

int g_retBoton = 0;
int gl_TiempoEsperaMenuRapido;

struct Opcion
{
  char *Opcs; //Lista de opciones
  int  Enlace; //Código de enlace, 0 =Fin
  int  Salto; //1=Salto lista de opciones, 2=Salto único de continuacion, 0=Sin salto, Fin
  int  punto; //Punto led 7 segmentos
  int  Codigo; //Código de opción
};
char Eleccion[MAX_NIVELES_MENU+1];

#define MAX_OPCS 40
Opcion Opciones[MAX_OPCS] = {{"AFCS", 11, 1, 1, 10}, //Aceleración. Frenada, Configuración, Salir
                      {"0123456789Ld", 0, 0, 0, 11}, {"0123456789A", 0, 0, 0, 12}, {"crFAPdLCS", 21, 1, 0, 13}, //Opcs Aceleración, Opcs Frenada, Opcs Configuración
                      {"cS", 0, 0, 0, 21}, {"rS", 0, 0, 0, 22}, {"0123456789", 31, 2, 0, 23}, {"0123456789", 61, 2, 0, 24}, {"12S",111,1,0,25}, {"0123456789", 131, 2, 0, 26}, {"LS", 0, 0, 0, 27}, {"1234", 132, 2, 0, 28},// Calibracion, Reset, Freno, Aceleración, Algo Potencia, Tiempo freno dinámico total, Lectura Voltios, Configuración de potes
                      {"rPcoS", 41, 1, 1, 31}, // Curva de freno, Tiempo rele, potencia de resta de freno progresivo, tiempo de curva para activación automática, órden de entrada de frenos
                      {"0123456789", 51, 2, 0, 41}, {"0123456789", 52, 2, 0, 42}, {"0123456789", 53, 2, 0, 43}, {"rP", 0, 0, 0, 44},
                      {"0123456789", 0, 0, 1, 51}, {"0123456789", 0, 0, 1, 52}, {"0123456789", 0, 0, 1, 53},
                      {"gPALS", 71, 1, 1, 61}, //Cfg aceleración, Puntos de gas, correspondencia puntos de potencia, sensibilidad o potencia de arranque, Limitación de potencia
                      {"0123456789", 81, 2, 0, 71}, {"0123456789", 82, 2, 0, 72}, {"0123456789",83, 2, 0, 73}, {"0123456789",84, 2, 0, 74},
                      {"0123456789", 91, 2, 1, 81}, {"0123456789", 92, 2, 1, 82}, {"0123456789", 0, 0, 1, 83}, {"0123456789", 0, 0, 1, 84},
                      {"0123456789", 101, 2, 0, 91}, {"0123456789", 102, 2, 0, 92},
                      {"0123456789", 0, 0, 1, 101}, {"0123456789", 0, 0, 1, 102} ,
                      {"01234567890", 0, 0, 1, 111}, {"01234567890", 121, 2, 0, 112},
                      {"123456789", 0, 0, 1, 121}, 
                      {"0123456789", 0, 0, 1, 131}, 
                      {"FASdPcrb", 0, 0, 1, 132} //Curvas Freno, Curvas Aceleración, Sensibilidad, Potencia de deceleración, Configuración Algoritmo de control de Potencia, Control de curva, menú rápido, borrar asignación
              };
//opciones de selección del menú rápido
char opcion = 0;
char opcionA = 0;
char opcionF = 0;
char opcionS = 0;
char opciond = 0;
char opcionP = 0;
#define NUM_OPCS 6
#define NUM_OPCS_A 12
#define NUM_OPCS_F 13
#define NUM_OPCS_S 17
#define NUM_OPCS_d 17
#define NUM_OPCS_P 29
                      

/**************************************************************************************************************************************
int ComprobarMenu(int valor, int Posicion)
**************************************************************************************************************************************/

int ComprobarMenu(int valor, int Posicion)
{
  static long tiempo = 0;
  static long Contador = 0;
  static int Estado = 0;
  int Codigo;
  int margen;
  
  Codigo = Pulsacion(valor, Posicion, margen);
  
  switch(Estado)
  {
   case 0:
   case EST_ESPERA_MENU:
   {
      if ((valor >= MENU_MIN_VALOR) && (valor <= MENU_MAX_VALOR))
      {
        if (Estado == 0)
        { //Comenzamos a contar el timepo para entrar en el menu
          tiempo = millis();
          g_punto = 1;
          Estado = EST_ESPERA_MENU;
        }
        else if (millis()-tiempo > MENU_TIEMPO)
        { //Llevamos más del tiempo mínimo, ahora tenemos poco tiempo para confirmar frenando
          tiempo = millis();
          g_punto = 0;
          Estado = EST_ESPERA_CONFIRMACION;
        }
      }
      else
      {
          tiempo = 0;
          g_punto = 0;
          Estado = 0;
      } 
      break;
   }
   case EST_ESPERA_CONFIRMACION:
   {
      if (millis()-tiempo < MENU_TIEMPO_CONFIRM)
      {
        if (Codigo == PULSACION_FRENO)
        {
          tiempo = 0;
          g_punto = 0;
          Estado = 0;
          AsignarVelocidad(0);
          Eleccion[0] = 0;
          MenuRecursivo(Opciones, 0, 0, Eleccion, 0);
        }
      }
      else
      {
          tiempo = 0;
          g_punto = 0;
          Estado = 0;
      }
      break;
   }
  } 
}

/**************************************************************************************************************************************
int ComprobarMenu3Toques(int valor, int Posicion)
**************************************************************************************************************************************/
int ComprobarMenu3Toques(int valor, int Posicion)
{
  static long tiempo = 0;
  static long Contador = 0;
  static int valorLed7 = 0;
  int Codigo;
  int margen;
  int opcion;
  
  debug = 1;
  Codigo = Pulsacion(valor, Posicion, margen);
  
  if (Codigo == PULSACION_FRENO)
  {
    #ifdef DEBUG
      SerialPrint("Pulsado freno");
    #endif
    if (tiempo == 0)
    {
      tiempo = millis();
      Contador = 0;
    }
    else
    {
       if (millis()-tiempo >= MARGEN_TIEMPO)
       {
         EscribirLed7s(BORRAR_LED,0);
         tiempo = millis();
         Contador = 0;
#ifdef DEBUG
         SerialPrint("Iniciando contador de entrada en menu");
#endif         
       }
       else if ((millis()-tiempo > MARGEN_MIN_TIEMPO) && (margen < MARGEN_GAS)  && (margen > MARGEN_MIN_GAS))
       {
          tiempo = millis();
          Contador++;
          valorLed7 = EscribirLed7s(Contador, 1);
#ifdef DEBUG
          SerialPrint("Pulsación de freno en tiempo ->", margen);
#endif          
          if (Contador == PULSACIONES_MENU)
          {
            Contador = 0;
            //Entramos en el menú
            AsignarVelocidad(0);
            opcion = MenuRecursivo(Opciones, 0, 0, Eleccion, 0);
          }
       }
       else
         Contador = 0;
    }
  }
}

/**************************************************************************************************************************************
int EsperaPulsacion(int ms)
**************************************************************************************************************************************/

int EsperaPulsacion(int ms)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  int valor;
  int Posicion;
  int margen;
  int evento = 0;
  
  debug = 0;
  
  Espera(0);
  while(!Espera(ms) && !evento)
  {
    LeerHallNormalizado(Posicion, valor);
    evento = Pulsacion(valor, Posicion, margen);
    
    if (!evento)
    {
      evento = ComprobarBoton();
      if (evento == PULSADO_BOTON)
        return PULSACION_GAS;
      evento = ComprobarPote();
    }
    
  }
  if (!evento)  DEBUG("Pulsacion ->", evento);
  return evento;
}
#undef DEBUG

int Pulsacion(int valor, int Posicion, int &margen)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  static int valor_max_freno = 0;
  static int valor_min_gas = 255;
  static int PosicionAnt = TOPE_FRENO;
  
  debug = 0;
  
  if (valor > valor_max_freno) valor_max_freno = valor;
  if (valor < valor_min_gas) valor_min_gas = valor;
  
  //Empezamos a mover el gatillo y quitarlo de la posición de freno, inicializamos la lectura del máximo recorrido que va a hacer
  if ((Posicion != TOPE_FRENO) && (PosicionAnt == TOPE_FRENO))
  {
    valor_max_freno = 0;
    DEBUG("Abandono de posicion de freno", "");
  }
  else if ((Posicion == TOPE_FRENO) && (PosicionAnt != TOPE_FRENO))
  {
    //Hemos vuelto al freno y comprobamos que hemos movido suficiente el gatillo para comprobar que no fue una falsa lectura del hall
    DEBUG("Vuelta a la posicion de freno", valor_max_freno);
    if (valor_max_freno > MARGEN_MIN_GAS)
    {
      margen = valor_max_freno;
      valor_max_freno = 0;
      PosicionAnt = Posicion;
      return PULSACION_FRENO;  
    }
  }
  else if ((Posicion != TOPE_GAS) && (PosicionAnt == TOPE_GAS))
  {
    valor_min_gas = 255;
    DEBUG("Abandono de posicion de gas", "");
  }
  else if ((Posicion == TOPE_GAS) && (PosicionAnt != TOPE_GAS) && (valor_min_gas < 255-MARGEN_MIN_GAS))
  {
    margen = valor_min_gas;
    valor_min_gas = 255;
    PosicionAnt = Posicion;
    return PULSACION_GAS;  
  }

  PosicionAnt = Posicion;
  
  return 0;
  
}
#undef DEBUG

/**************************************************************************************************************************************
int MenuRecursivo(struct Opcion *Opcs, int ini, int opcion, char *Eleccion, int nivel)
**************************************************************************************************************************************/

int MenuRecursivo(struct Opcion *Opcs, int ini, int opcion, char *Eleccion, int nivel)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  int evento;
  int NumOpcs;
  char *CadOpcs;
  int salto;
  int seleccion;
  int dato;
  int inc;

  CadOpcs = Opcs[ini].Opcs;
  NumOpcs = strlen(CadOpcs);

  DEBUG("entrando en menu, ini->", ini);
  while(1)
  {
      //Visualizar Opciones
      EscribirLed7s(CadOpcs[opcion], Opcs[ini].punto);
      evento = EsperaPulsacion(ESPERA_PULSACION_MENU); 
      
      inc = 0;
      if (evento == PULSACION_FRENO)
        inc = 1;
      else if (evento == POTE_MOV_MAS)
        inc = 1;
      else if (evento == POTE_MOV_MENOS)
        inc = -1;
      
      if (inc)
      {
        //Avanzamos a la siguiente opción
        DEBUG("Avanzando opcion ->", opcion)
        DEBUG("->de ", NumOpcs)
        opcion = (opcion+inc)%NumOpcs;
        if (opcion < 0)
          opcion = NumOpcs - 1;
        if (opcion == NumOpcs) //Al recorrer todas las opciones salimos del menú
        {
          EscribirLed7s(BORRAR_LED, 0);
        }
      }
      //Se ha seleccionado la opción ***********************
      else if (evento == PULSACION_GAS)
      {
        Eleccion[nivel] = CadOpcs[opcion]; //Guardamos la opción
        Eleccion[nivel+1] = 0;
        if (CadOpcs[opcion] == 'S')
        { // SALIDA
          EscribirLed7s(BORRAR_LED,0);
          OpcionEscogida(Eleccion);
          return OPCION_SALIDA;
        }
        else if (Opcs[ini].Enlace == 0)
        {
          //Se ha escogido una opcion final
          EscribirLed7s(BORRAR_LED,0);
          if (OpcionEscogida(Eleccion) == OPCION_SALIDA)
            return OPCION_SALIDA;
          else
            return OPCION_ULTIMA;
        }

        //Entramos en la opción
        if (Opcs[ini].Salto == SALTO_OPCION)
          salto = BuscarIndice(Opcs[ini].Enlace+opcion);
        else
          salto = BuscarIndice(Opcs[ini].Enlace);

        dato = LeerOpcionEscogida(Eleccion);          
        DEBUG("opcion = ", dato)
        seleccion = MenuRecursivo(Opcs, salto, dato, Eleccion, nivel+1);   
        switch (seleccion)
        {
          case OPCION_SALIDA:
              return OPCION_SALIDA;
          case OPCION_ULTIMA:
          {
            //Si retornamos desde una opción final, debemos retornar hasta una zona de selección
            if (Opcs[ini].Salto == SALTO_SIGUIENTE) 
               return OPCION_ULTIMA;
          }
        }
      }
      else // Salimos de este nivel por exceso de tiempo
      {
        EscribirLed7s(BORRAR_LED, 0);
        return 0;
      }
  }
}
#undef DEBUG

/**************************************************************************************************************************************
int OpcionEscogida(char *Eleccion)
**************************************************************************************************************************************/
int OpcionEscogida(char *Eleccion)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  int opcion;
  debug = 1;
  
  DEBUG("Opcion escogida ->", Eleccion);
  
  switch(Eleccion[0])
  {
    case 'A':
    {
      if (Eleccion[1]=='L')
        SelecAceleracion = NO_SELEC;
      else
      {
        opcion = ctoi(Eleccion[1]);
        SelecAceleracion = opcion;
      }
      GrabarSelecCfg();
      break;
    }
    case 'F':
    {
      if (Eleccion[1]=='L')
        SelecFreno = SELEC_AUTO;
      else
      {
        opcion = ctoi(Eleccion[1]);
        SelecFreno = opcion;
      }
      GrabarSelecCfg();
      break;
    }
    case 'C':
    {
        switch(Eleccion[1])
        {
          case 'c': //Calibración
          {
            if (Eleccion[2] == 'c')
              ConfiguracionHall();
            return 0;
          }
          case 'r': //Restaurar valores de fábrica
          {
            if (Eleccion[2] == 'r')
            {
              InicializarCfgPtr((byte *)CfgAceleracion, sizeof(CfgAceleracion));
              InicializarCfgPtr((byte *)CfgFreno, sizeof(CfgFreno));
              PrecargarCfgAceleracion();
              PrecargarCfgFreno();
              PrecargarCfgGeneral();
              GrabarSelecCfg();
              GrabarAlgoCfg();
              GrabarFrenoDinamicoCfg();
              GrabarCfgPtr((byte *)CfgAceleracion, sizeof(CfgAceleracion), EE_CFG_ACELERACION);
              GrabarCfgPtr((byte *)CfgFreno, sizeof(CfgFreno), EE_CFG_FRENO);
              DEBUG("Size acel ->", sizeof(CfgAceleracion));
              DEBUG("Size freno ->", sizeof(CfgFreno));
            }
            return 0;
          }
          case 'F': //Cfg Freno
          {
            opcion = Eleccion[2]-'0';
            switch(Eleccion[3])
            {
              case 'r': //Tiempo del relé
              {
                CfgFreno[opcion].TiempoFrenoRele = ctoi(Eleccion[4])*10+ctoi(Eleccion[5]);
                DEBUG("Cfg Freno Relé->", CfgFreno[opcion].TiempoFrenoRele);
                break;
              }
              case 'P': //Potencia del freno progresivo
              {
                CfgFreno[opcion].PotenciaFrenoProgresivo = ctoi(Eleccion[4])*10+ctoi(Eleccion[5]);
                DEBUG("Cfg Freno Progresivo->", CfgFreno[opcion].PotenciaFrenoProgresivo);
                break;
              }
              case 'c': //Tiempo de gas máximo para detección automática de entrada en curva
              {
                //Solo admitimos configuraciones automáticas en las posiciones 0..4
                if (opcion>4)
                {
                  EscribirLed7s('E',1);
                  delay(1000);
                }
                else
                {
                  CfgFreno[opcion].TiempoRecta = ctoi(Eleccion[4])*10+ctoi(Eleccion[5]);
                  DEBUG("Cfg Tiempo Recta->", CfgFreno[opcion].TiempoRecta);
                }
                break;
              }
              case 'o': //Orden de entrada de los frenos
              {
                CfgFreno[opcion].orden = Eleccion[4];
                DEBUG("Cfg orden primer freno->", CfgFreno[opcion].orden);
                break;
              }
              case 'S': //Salvar y salir
              {
                GrabarCfgPtr((byte *)CfgFreno, sizeof(CfgFreno), EE_CFG_FRENO);
                DEBUG("Configuracion de freno grabada","")
                break;
              }
            }
          }
          case 'A': //Cfg curvas aceleración
          {
            opcion = Eleccion[2]-'0'; //Recuperamos la curva de aceleración
            switch(Eleccion[3])
            {
              case 'g': //Porcentaje de movimiento de gatillo
              {
                CfgAceleracion[opcion].Tramo[0].Gatillo = ctoi(Eleccion[4])*10+ctoi(Eleccion[5]);
                DEBUG("Gatillo (%)->", CfgAceleracion[opcion].Tramo[0].Gatillo)
                DEBUG(" Pos->", opcion);
                CfgAceleracion[opcion].Tramo[1].Gatillo = ctoi(Eleccion[6])*10+ctoi(Eleccion[7]);
                DEBUG("Gatillo (%)->", CfgAceleracion[opcion].Tramo[1].Gatillo)
                DEBUG(" Pos->", opcion);
                CfgAceleracion[opcion].Tramo[2].Gatillo = 0; //Indicamos que sola hay dos tramo
                break;
              }
              case 'P': //Procentaje de potencia suministrado
              {
                CfgAceleracion[opcion].Tramo[0].Potencia = ctoi(Eleccion[4])*10+ctoi(Eleccion[5]);
                DEBUG("Potencia (%)->", CfgAceleracion[opcion].Tramo[0].Potencia);
                CfgAceleracion[opcion].Tramo[1].Potencia = ctoi(Eleccion[6])*10+ctoi(Eleccion[7]);
                DEBUG("Potencia (%)->", CfgAceleracion[opcion].Tramo[0].Potencia);
                break;
              }
              case 'A': //Potencia de Arranque o sensibilidad
              {
                CfgAceleracion[opcion].Sensibilidad = ctoi(Eleccion[4])*10+ctoi(Eleccion[5]);
                DEBUG("Sensibilidad (%)->", CfgAceleracion[opcion].Sensibilidad);
                break;
              }
              case 'L': //Limitación de potencia
              {
                CfgAceleracion[opcion].PotenciaMax = ctoi(Eleccion[4])*10+ctoi(Eleccion[5]);
                DEBUG("Potencia Max (%)->", CfgAceleracion[opcion].PotenciaMax);
                break;
              }
              case 'S': //Salvar y salir
              {
                GrabarCfgPtr((byte *)CfgAceleracion, sizeof(CfgAceleracion), EE_CFG_ACELERACION);
                DEBUG("Configuracion de aceleracion grabada","");
                //VisualizarCfgAceleracion(CfgAceleracion);
                break;
              }
            }
          }
          case 'P': //Algoritmos de control de potencia
          {
            switch(Eleccion[2])
            {
              case '1': //Media
              {
                AlgoMedia.Tiempo = ctoi(Eleccion[3]);
                SelecAlgoritmo = 1;
                GrabarAlgoCfg();
                break;  
              }
              case '2': //Progresivo
              {
                SelecAlgoritmo = 2;
                AlgoProgresivo.Tiempo = ctoi(Eleccion[3]);
                AlgoProgresivo.ProporcionFrenado = ctoi(Eleccion[4]);
                GrabarAlgoCfg();
                break;
              }
            }
          }
          case 'd': //Datos de frenado dinámico
          {
            FrenoDinamico.TiempoPotenciaMax = ctoi(Eleccion[2])*10+ctoi(Eleccion[3]);
            DEBUG("o=", FrenoDinamico.TiempoPotenciaMax);
            GrabarFrenoDinamicoCfg();
            break;
          }
          case 'L': //Lectura voltímetro
          {
            if (Eleccion[2] == 'L')
              LecturaVoltimetro(30);
            break;
          }
          case 'C': //Configuración de potes
          {
            int pote = Eleccion[2]-'0';
            CfgPote.Funcion[pote] = Eleccion[3];
            GrabarCfgPotes();
          }
        }
    }    
  }

#undef DEBUG
}

/**************************************************************************************************************************************
int LeerOpcionEscogida(char *Eleccion)
**************************************************************************************************************************************/

int LeerOpcionEscogida(char *Eleccion)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  int opcion = 0;
  int nivel, i, lon;
  
  lon = strlen(Eleccion);
  for (i=lon;i<MAX_NIVELES_MENU;i++)
    Eleccion[i] = 0;
  
  
  DEBUG("Opcion escogida ->", Eleccion);
  nivel = strlen(Eleccion);
  DEBUG("nivel ->", nivel);
  if (nivel >=1)
  switch(Eleccion[0])
  {
    case 'A':
    {
      if (SelecAceleracion == NO_SELEC) return 0;
      return SelecAceleracion;
      break;
    }
    case 'F':
    {
      if (SelecFreno == NO_SELEC) return 0;
      return SelecFreno;
      break;
    }
    case 'C':
    {
        if (nivel >=2)
        switch(Eleccion[1])
        {
          case 'F': //Cfg Freno
          {
            if (nivel == 2)            
            {
              if ((SelecFreno >= 0) && (SelecFreno <= MAX_CFG_FRENO) && (SelecFreno != NO_SELEC) && (SelecFreno != 'A'))
                opcion = SelecFreno;
              else if (SelecFreno == 'A')
                opcion = 10;
              else if (SelecFreno == 'd')
                opcion = 12;
              else
                opcion = 0;
              return opcion;
            }
            else if (nivel >= 4)
            {
              opcion = Eleccion[2]-'0';
              switch(Eleccion[3])
              {
                case 'r': //Tiempo del relé
                {
                  if (nivel == 4)
                    return CfgFreno[opcion].TiempoFrenoRele/10;
                  else
                    return CfgFreno[opcion].TiempoFrenoRele%10;
                  break;
                }
                case 'P': //Potencia del freno progresivo
                {
                  if (nivel == 4)
                    return CfgFreno[opcion].PotenciaFrenoProgresivo/10;
                  else
                    return CfgFreno[opcion].PotenciaFrenoProgresivo%10;
                  break;
                }
                case 'c': //Tiempo de gas máximo para detección automática de entrada en curva
                {
                  if (nivel == 4)
                    return CfgFreno[opcion].TiempoRecta/10;
                  else
                    return CfgFreno[opcion].TiempoRecta%10;
                  break;
                }
                case 'o': //Orden de entrada de los frenos
                {
                  if (CfgFreno[opcion].orden == 'r')
                    return 0;
                  else
                    return 1;
                  break;
                }
                case 'S': //Salvar y salir
                {
                  break;
                }
              }//case
            }//if
          }
          case 'A': //Cfg curvas aceleración
          {
            if (nivel == 2)            
            {
              if ((SelecAceleracion >= 0) && (SelecAceleracion <= MAX_CFG_FRENO) && (SelecAceleracion != NO_SELEC) && (SelecAceleracion != 'L'))
                opcion = SelecAceleracion;
              else if (SelecAceleracion == 'L')
                opcion = 10;
              else
                opcion = 0;
              return opcion;  
            }
            else if (nivel >= 4)
            {
              opcion = Eleccion[2]-'0'; //Recuperamos la curva de aceleración
              switch(Eleccion[3])
              {
                case 'g': //Porcentaje de movimiento de gatillo
                {
                  switch(nivel)
                  {
                    case 4: {
                      return CfgAceleracion[opcion].Tramo[0].Gatillo/10;
                      break;
                    }
                    case 5: {
                      return CfgAceleracion[opcion].Tramo[0].Gatillo%10;
                      break;
                    }
                    case 6: {
                      return CfgAceleracion[opcion].Tramo[1].Gatillo/10;
                      break;
                    }
                    case 7: {
                      return CfgAceleracion[opcion].Tramo[1].Gatillo%10;
                      break;
                    }
                  }
                  break;
                }
                case 'P': //Porcentaje de potencia suministrado
                {
                  switch(nivel)
                  {
                    case 4: {
                      return CfgAceleracion[opcion].Tramo[0].Potencia/10;
                      break;
                    }
                    case 5: {
                      return CfgAceleracion[opcion].Tramo[0].Potencia%10;
                      break;
                    }
                    case 6: {
                      return CfgAceleracion[opcion].Tramo[1].Potencia/10;
                      break;
                    }
                    case 7: {
                      return CfgAceleracion[opcion].Tramo[1].Potencia%10;
                      break;
                    }
                  }
                  break;
                }
                case 'A': //Potencia de Arranque o sensibilidad
                {
                  switch(nivel)
                  {
                    case 4: {
                      return CfgAceleracion[opcion].Sensibilidad/10;
                      break;
                    }
                    case 5: {
                      return CfgAceleracion[opcion].Sensibilidad%10;
                      break;
                    }
                  }
                  break;
                }
                case 'L': //Limitación de potencia
                {
                  switch(nivel)
                  {
                    case 4: {
                      return CfgAceleracion[opcion].PotenciaMax/10;
                      break;
                    }
                    case 5: {
                      return CfgAceleracion[opcion].PotenciaMax%10;
                      break;
                    }
                  }
                  break;
                }
                case 'S': //Salvar y salir
                {
                  break;
                }
              }//case
            } //if
          }
          case 'P': //Algoritmos de control de potencia
          {
            if (nivel == 2)
              return SelecAlgoritmo-1;
            else
            {
              switch(Eleccion[2])
              {
                case '1': //Media
                {
                  return AlgoMedia.Tiempo;
                  break;  
                }
                case '2': //Progresivo
                {
                  if (nivel == 3) 
                    return AlgoProgresivo.Tiempo;
                  else
                    return AlgoProgresivo.ProporcionFrenado-1;
                  break;
                }
              }
            }//else
          }
          case 'd': //Freno dinámico
          {
            if (nivel == 2)
              return FrenoDinamico.TiempoPotenciaMax/10;
            else
              return FrenoDinamico.TiempoPotenciaMax%10;
          }
        } // if nivel >=2
    }    
  } //if nivel >=1
  return 0;
#undef DEBUG
}

/**************************************************************************************************************************************
int ComprobarPlsacionPote()
**************************************************************************************************************************************/
int ComprobarPulsacionPote()
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  static long s_TiempoPote = 0;
  static int Pote = -1;
  static int estado = BT_POTE_ESPERA;
  //Si llevamos el Pote al mínimo desde una posición alejada, interpretamos la piulsación de un botón para las cfg que no tienen botón
  
  Pote = LeerPote();
  
  switch(estado)
  {
    case BT_POTE_ESPERA:
    {
      if (Pote > BT_POTE_LIM_MIN)
        estado = BT_POTE_ESPERA_PULSACION;
      break;
    }
    case BT_POTE_ESPERA_PULSACION:
    case BT_POTE_ESPERA_ACTIVACION:
    {
      if ((Pote > BT_POTE_VALOR_MEDIO) && (estado == BT_POTE_ESPERA_PULSACION))
      {
          estado =  BT_POTE_ESPERA_ACTIVACION;
          return estado;
      }

      if (Pote < 10)
      { //Se produjo una pulsacion desde un lugar lejano a 0
        estado = BT_POTE_ESPERA;
        return BT_POTE_PULSADO_BOTON;
      }
      break;
    }
  } //switch
  return 0;
}
#undef DEBUG
/**************************************************************************************************************************************
int ComprobarPote()
**************************************************************************************************************************************/
int ComprobarPote()
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  static int PoteAnt = -1;
  static int Pote = -1;
  static long TiempoMov = 0;
  int margen;
  
  
  Pote = LeerPote();

  if (BotonPulsado())
  {
    DEBUG("Pote Ant=", PoteAnt);
    DEBUG("Pote    =", Pote);
    DEBUG("Tiempo  =", TiempoMov);
  }
  
  margen = abs(PoteAnt-Pote);
  if (margen && (margen < MARGEN_ERROR_MAX_POTE))
  { //Se ha comenzado a mover el potenciómetro
    //DEBUG("Pote Ant=", PoteAnt);
    //DEBUG("Pote    =", Pote);
    //DEBUG("Inic tiempo", TiempoMov);
    if (TiempoMov == 0) 
    {
      TiempoMov = millis();
      PoteAnt = Pote;
    }
  }
  
  if ((TiempoMov > 0) && ((millis()-TiempoMov) > POTE_TIEMPO_MAX))
  { //Se ha excedido el tiempo del movimiento
    TiempoMov = 0;
    PoteAnt = Pote;
    DEBUG("Fin tiempo","");
  }
  else if (TiempoMov > 0)
  {
    if (PoteAnt - Pote > POTE_MOV_MENOS_MARGEN)
    {
      PoteAnt = Pote;
      TiempoMov = millis();
      return POTE_MOV_MENOS;
    } 
    else if (Pote - PoteAnt > POTE_MOV_MAS_MARGEN)
    {
      PoteAnt = Pote;
      TiempoMov = millis();
      return POTE_MOV_MAS;
    }
  }
  else
    PoteAnt = Pote;
  return 0;  
}
#undef DEBUG

/**************************************************************************************************************************************
int ComprobarPulsaciones()
**************************************************************************************************************************************/
int ComprobarPulsaciones()
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  int Pote;
  
  if (ComprobarBoton() == PULSADO_BOTON)
  {
    DEBUG("Boton Pulsado", "");
  }
  Pote = ComprobarPote();
  if (Pote == POTE_MOV_MAS)
    DEBUG("Pote Mov =", "MAS");
  if (Pote == POTE_MOV_MENOS)
    DEBUG("Pote Mov =", "MENOS");
}
#undef DEBUG

/**************************************************************************************************************************************
int ComprobarBoton()
**************************************************************************************************************************************/
int ComprobarBoton()
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  static int botonAnt = -1;
  int boton;
  static long TiempoMin = 0;
  
  g_retBoton = 0;
  boton = LeerBoton();
  //Comprobamos si se está pulsando el botón
  if ((botonAnt == 1) && (boton == 0) && ((millis()-TiempoMin) > BOTON_MANGEN_PULSACION))
  {
    // Pulsado botón
    botonAnt = boton;
    TiempoMin = millis();
    g_retBoton = PULSADO_BOTON;
    DEBUG("retboton=", g_retBoton);
    return PULSADO_BOTON;
  }
  else
    botonAnt = boton;
    
  return 0;  
}
#undef DEBUG

/**************************************************************************************************************************************
int ComprobarCfgRapida()
**************************************************************************************************************************************/
int ComprobarCfgRapida()
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  static char opciones[NUM_OPCS] = "AFSdP";
  static char A[NUM_OPCS_A] = "0123456789L";
  static char F[NUM_OPCS_F] = "0123456789Ad";
  static char S[NUM_OPCS_S] = "0123456789AbCdEF";
  static char d[NUM_OPCS_d] = "0123456789AbCdEF";
  static char P[NUM_OPCS_P] = {'0',27,28,29,'1',27,28,29,'2',27,28,29,'3',27,28,29,'4',27,28,29,'5',27,28,29,'6',27,28,29};
  int NumOpcs = NUM_OPCS;
  int NumOpcsA = NUM_OPCS_A;
  int NumOpcsF = NUM_OPCS_F;
  int NumOpcsS = NUM_OPCS_S;
  int NumOpcsd = NUM_OPCS_d;
  int NumOpcsP = NUM_OPCS_P;
  int boton = 0;
  int Pote = 0;
  int inc = 0;
  static long TiempoActivacionMenuPote = 0;
  int Espera_ms;

  //Al finalizar el tiempo sin pulsaciones aplicamos las configuraciones
  if ((g_TiempoMenuRapido > 0) && (millis()-g_TiempoMenuRapido > MENU_RAPIDO_TIEMPO_MAX))
  {
    g_TiempoMenuRapido = 0;
    g_MenuRapidoActivo = 0;
    //Aplicamos los cambios
    if ((SelecFreno != DatosMenuRapido.SelecFreno) || (SelecAceleracion != DatosMenuRapido.SelecAceleracion))
    {
      SelecFreno = DatosMenuRapido.SelecFreno;
      SelecAceleracion = DatosMenuRapido.SelecAceleracion;
      GrabarSelecCfg();
    }
    if ((SelecAceleracion != NO_SELEC) && (SelecAceleracion != 'L'))
    {
      if (CfgAceleracion[SelecAceleracion].Sensibilidad != DatosMenuRapido.Sensibilidad)
      {
        DEBUG("Grabando sensibilidad","");
        CfgAceleracion[SelecAceleracion].Sensibilidad = DatosMenuRapido.Sensibilidad;
        //GrabarCfgPtr((byte *)CfgAceleracion, sizeof(CfgAceleracion), EE_CFG_ACELERACION);
        GrabarCfgPtr((byte *)&(DatosMenuRapido.Sensibilidad), sizeof(byte), EE_CFG_ACELERACION+sizeof(ConfAceleracion)*SelecAceleracion+sizeof(TramoGas[5]));
      }
    }
    if ((SelecFreno != NO_SELEC) && (SelecFreno != 'A') && (SelecFreno != 'd'))
    {
      if (CfgFreno[SelecFreno].PotenciaFrenoProgresivo != DatosMenuRapido.FrenoProgresivo)
      {
        CfgFreno[SelecFreno].PotenciaFrenoProgresivo = DatosMenuRapido.FrenoProgresivo;
        DEBUG("Pot Freno=", CfgFreno[SelecFreno].PotenciaFrenoProgresivo);
        
        //GrabarCfgPtr((byte *)CfgFreno, sizeof(CfgFreno), EE_CFG_FRENO);
        GrabarCfgPtr((byte *)&(DatosMenuRapido.FrenoProgresivo), sizeof(byte), EE_CFG_FRENO+sizeof(ConfFreno)*SelecFreno+sizeof(byte));
      }
    }
  }

  boton = ComprobarBoton();
  Espera_ms = 0;

//---------------------------------------------------------------------------------------------------------------
#ifdef POTE_RESET
  Pote = ComprobarPulsacionPote();
  //Solo detectamos la pulsación del pote si no hemos entrado en la zona de asignación de valores del menú rápido
  if ((Pote == BT_POTE_PULSADO_BOTON) && (g_TiempoMenuRapido == 0))
  {
    //Solo avanzamos si estamos dentro del menú rápido
    if (TiempoActivacionMenuPote > 0) 
      opcion = (opcion+1) % (NumOpcs-1);
    SalidaLed7s(opciones[opcion], g_punto, MENU_RAPIDO);
    TiempoActivacionMenuPote = millis();
    g_MenuRapidoActivo = 1;
  }
  if (TiempoActivacionMenuPote > 0)
  {
    if (millis() - TiempoActivacionMenuPote > MENU_RAPIDO_ESPERA_POTE)
    {
      TiempoActivacionMenuPote = 0;
      g_MenuRapidoActivo = 0;
    }
    else if (Pote == BT_POTE_ESPERA_ACTIVACION)
    {
      //Si llevamos el pote a la mitad antes del tiempo indicado se activa el menú rápido
      TiempoActivacionMenuPote = 0;
      //g_TiempoMenuRapido = millis();
      boton = PULSADO_BOTON;
      Espera_ms = BT_POTE_ESPERA_MENU;
    }  
  }
#endif  
//---------------------------------------------------------------------------------------------------------------
  if ((boton == PULSADO_BOTON))
  {
    if (g_TiempoMenuRapido > 0)
      opcion = (opcion+1) % (NumOpcs-1);
    SalidaLed7s(opciones[opcion], g_punto, MENU_RAPIDO);
    InicializaMenuRapido(Espera_ms);
    g_MenuRapidoActivo = 1;
  }
  Pote = ComprobarPote();
  if (Pote == POTE_MOV_MAS)
     inc = 1;
  else if (Pote == POTE_MOV_MENOS)
    inc = -1;
  else
    inc = 0;
  
  if (inc && (g_TiempoMenuRapido > 0))
  {
    //Si el menú está activo, solo permitimos cambiar la opcion si ha transcurrido el tiempo indicado para cambio de opciones
    if (millis() - g_TiempoMenuRapido < gl_TiempoEsperaMenuRapido)
      inc = 0;
    else
      //Inicializamos menú rápido
      InicializaMenuRapido(0);

    
    switch(opciones[opcion])
    {
      case 'A':
      {
          opcionA = (opcionA+inc) % (NumOpcsA-1);
          if (opcionA < 0) opcionA = NumOpcsA-2;
          if (A[opcionA] == 'L')
            DatosMenuRapido.SelecAceleracion = 'L';
          else
            DatosMenuRapido.SelecAceleracion = ctoi(A[opcionA]);
          SalidaLed7s(A[opcionA], g_punto, MENU_RAPIDO);
          break;
      }
      case 'F':
      {
          opcionF = (opcionF+inc) % (NumOpcsF-1);
          if (opcionF < 0) opcionF = NumOpcsF-2;
          if (F[opcionF] == 'A')
            DatosMenuRapido.SelecFreno = 'A';
          else if (F[opcionF] == 'd')
            DatosMenuRapido.SelecFreno = 'd';
          else
            DatosMenuRapido.SelecFreno = ctoi(F[opcionF]);
          SalidaLed7s(F[opcionF], g_punto, MENU_RAPIDO);
          break;
      }
      case 'S':
      {
          opcionS = (opcionS+inc) % (NumOpcsS-1);
          if (opcionS < 0) opcionS = NumOpcsS-2;

          if ((SelecAceleracion != NO_SELEC) && (SelecAceleracion != 'L'))
          {
            SalidaLed7s(S[opcionS], g_punto, MENU_RAPIDO);
            DatosMenuRapido.Sensibilidad = (opcionS * 4);
          }
          break;
      }
      case 'd':
      {
          if ((SelecFreno != 'd') && (SelecFreno != 'A') && (SelecFreno != NO_SELEC))
          {
            opciond = (opciond+inc) % (NumOpcsd-1);
            if (opciond < 0) opciond = NumOpcsd-2;
            
            SalidaLed7s(d[opciond], g_punto, MENU_RAPIDO);
            DatosMenuRapido.FrenoProgresivo = (opciond * 6);
            DEBUG("opcion=",opciond);
          }
          break;
      }
      case 'P':
      {
          if (SelecAlgoritmo == ALGO_MEDIA)
          {
            DEBUG("opcionM=",opcionP);
            opcionP = (opcionP+inc*4);
            if (opcionP > NumOpcsP-1) opcionP = 0;
            if (opcionP < 0) opcionP = NumOpcsP-5;
            
            SalidaLed7s(P[opcionP], g_punto, MENU_RAPIDO);
            AlgoMedia.Tiempo = opcionP/4;
          }
          else if (SelecAlgoritmo == ALGO_PROGRESIVO)
          {
            DEBUG("opcionP=",opcionP);
            opcionP = (opcionP+inc) % (NumOpcsP-1);
            if (opcionP < 0) opcionP = NumOpcsP-1;

            AlgoProgresivo.Tiempo = opcionP/4;
            AlgoProgresivo.ProporcionFrenado = opcionP%4+1;
            SalidaLed7s(P[opcionP], g_punto, MENU_RAPIDO);
          }
          GrabarAlgoCfg();
          break;
      }
    }
  }    
  
}
#undef DEBUG

void AplicarConfiguracionRapida()
{
    CfgFreno[SelecFreno].PotenciaFrenoProgresivo = DatosMenuRapido.FrenoProgresivo;
    SelecFreno =  DatosMenuRapido.SelecFreno; 
    SelecAceleracion = DatosMenuRapido.SelecAceleracion;
}

int BuscarIndice(int salto)
{
  int i;
  
  for (i=0; i<MAX_OPCS; i++)
  {
    if (Opciones[i].Codigo == salto)
      return i;
  }
  return 0;
}

void InicializaMenuRapido(int Espera)
{
  if (g_TiempoMenuRapido == 0)
  {
    DatosMenuRapido.Sensibilidad = CfgAceleracion[SelecAceleracion].Sensibilidad;
    DatosMenuRapido.FrenoProgresivo = CfgFreno[SelecFreno].PotenciaFrenoProgresivo;
    DatosMenuRapido.SelecFreno = SelecFreno;
    DatosMenuRapido.SelecAceleracion = SelecAceleracion;
    
    //Situamos los índices en los valores actuales
    opcionS = DatosMenuRapido.Sensibilidad / 4;
    if (opcionS > NUM_OPCS_S-2) opcionS = NUM_OPCS_S-2;

    opciond = DatosMenuRapido.FrenoProgresivo / 6;
    if (opciond > NUM_OPCS_d-2) opciond = NUM_OPCS_d-2;
    
    if (SelecFreno == 'A')
      opcionF = 10;
    else if (SelecFreno == 'd')
      opcionF = 11;
    else if (SelecFreno == NO_SELEC)
      opcionF = 0;
    else
      opcionF = SelecFreno;

    if ((SelecAceleracion == 'L') || (SelecAceleracion == NO_SELEC))
      opcionA = 10;
    else
      opcionA = SelecAceleracion;
    
    if (SelecAlgoritmo == ALGO_MEDIA)
    {
      opcionP = AlgoMedia.Tiempo*4;
      if (opcionP > NUM_OPCS_P-2) opcionP = NUM_OPCS_P-5;
    }  
    else if (SelecAlgoritmo == ALGO_PROGRESIVO)
    {
      opcionP = AlgoProgresivo.Tiempo*4+(AlgoProgresivo.ProporcionFrenado-1)%3;
      if (opcionP > NUM_OPCS_P-2) opcionP = 0;
    }
  }
  gl_TiempoEsperaMenuRapido = Espera;
  g_TiempoMenuRapido = millis();
}

/**************************************************************************************************************************************
int ConfiguracionPotes
**************************************************************************************************************************************/
void ConfiguracionPotes()
{
  #define DIR_DESCENDENTE 0
  #define DIR_ASCENDENTE  1
  int pote;
  static int ValorAnterior[4];
  int ValorActual[4];
  int PuntoCambio, flancoAscendente, flancoDescendente;
  byte direccion;
  
  LeerValoresPotes(ValorActual, ValorAnterior);
  
  for (pote=0; pote<NUM_POTES; pote++)
  {
    switch(CfgPote.Funcion[pote])
    {
      case 'F': //Cfg Freno
        {
          long freno, frenoS, frenoB, frenoA;
            if ((ValorActual[pote] != ValorAnterior[pote]) && 
                  (DatosMenuRapido.SelecFreno != 'A') && 
                  (DatosMenuRapido.SelecFreno != 'd') &&
                  (DatosMenuRapido.SelecFreno != NO_SELEC))
            {
              /*'Recuperamos el punto de salto medio según el valor actual
              'Calculamos los puntos de salto por flanco ascendente y descendente
              'Comprobamos la dirección de evolución del valor y si atraviesa algún valor de flanco*/
              PuntoCambio = DatosMenuRapido.SelecFreno/10*1024;
              flancoAscendente = PuntoCambio + MARGEN_VALOR_POTE;
              flancoDescendente = PuntoCambio - MARGEN_VALOR_POTE;
              
              if (ValorAnterior[pote] > ValorActual[pote])
                direccion = DIR_DESCENDENTE;
              else
                direccion = DIR_ASCENDENTE;
              
              if (direccion == DIR_ASCENDENTE)
                if ((ValorActual[pote] >= flancoAscendente) && (ValorAnterior[pote] < flancoAscendente))
                  DatosMenuRapido.SelecFreno++;
                else if ((ValorActual[pote] <= flancoAscendente) && (ValorAnterior[pote] > flancoDescendente))
                  DatosMenuRapido.SelecFreno--;

              
              freno = ValorActual[pote]*10/1024;
              frenoA = ValorAnterior[pote]*10/1024;
              frenoS = freno + MARGEN_VALOR_POTE;
              frenoB = freno - MARGEN_VALOR_POTE;
            }
            DatosMenuRapido.SelecFreno = SelecFreno;
            break;
        }
      case 'A': //Cfg Aceleración
        {
//            DatosMenuRapido.SelecAceleracion = ctoi(A[opcionA]);
            break;
        }
      case 'S': //Sensibilidad
        {
//            DatosMenuRapido.Sensibilidad = CfgAceleracion[SelecAceleracion].Sensibilidad;
        }
      case 'd': //Potencia de deceleración
        {
//            DatosMenuRapido.FrenoProgresivo = CfgFreno[SelecFreno].PotenciaFrenoProgresivo;
        }
      case 'P': //Cfg algoritmo control de potencia
        {
        }
      case 'r': //Menú rápido
        {
        }
      case 'c': //Control de potencia en curva (en medios)
        {
          break;
        }
    }
  }
}

void LeerValoresPotes(int *van, int *vac)
{
  static int valor[4];
  int i;
  
  for (i=0; i< NUM_POTES; i++)
  {
    
  }
}
