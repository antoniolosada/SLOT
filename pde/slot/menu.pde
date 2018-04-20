#define MARGEN_TIEMPO      500
#define MARGEN_GAS         100
#define MARGEN_MIN_GAS     20
#define PULSACIONES_MENU   3


int ComprobarMenu(int valor, int Posicion)
{
  static tiempo = 0;
  static Contador = 0;
  int Codigo;
  int margen;
  
  Codigo = Pulsacion(valor, Posicion, margen);
  
    if (Codigo == PULSACION_FRENO)
    {
      if (tiempo == 0)
        tiempo = millis();
      else
      {
         if ((millis()-tiempo < MARGEN_TIEMPO) && margen < MARGEN_GAS)
         {
            Contador++;
            if (Contador == PULSACIONES_MENU)
            {
              Contador = 0;
              //Entramos en el menú
              Menu();
            }
         }
         else
           Contador = 0;
      }
    }
}

int Pulsacion(int valor, int Posicion, int &margen)
{
  static int valor_max_freno = 0;
  static int valor_min_gas = 255;
  static PosicionAnt = TOPE_FRENO;
  
  if (valor > valor_max) valor_max = valor;
  if (valor < valor_min) valor_min = valor;
  
  if ((Posicion != TOPE_FRENO) && (PosicionAnt == TOPE_FRENO))
    valor_max = 0;
  
  if ((Posicion == TOPE_FRENO) && (PosicionAnt != TOPE_FRENO) && (valor_max_freno > MARGEN_MIN_GAS))
  {
    margen = valor_max_freno;
    return PULSACION_FRENO;  
  }

  if ((Posicion != TOPE_GAS) && (PosicionAnt == TOPE_GAS))
    valor_min_gas = 255;
  
  if ((Posicion == TOPE_GAS) && (PosicionAnt != TOPE_GAS) && (valor_min_gas < 255-MARGEN_MIN_GAS))
  {
    margen = valor_min_gas;
    return PULSACION_GAS;  
  }
  
  return 0;
  
  PosicionAnt = Posicion;
}

void Menu()
{
  int gatillo;
  char Opciones[4] = {'A','F','r','o'};
  int Opcion = 0;
  int margen;
  
  EscribirLed7s("A",1);

  Espera(0)
  while(!gatillo && !Espera(5000))
  {
    LeerHallNormalizado(Posicion, valor)
    gatillo = Pulsacion(valor, Posicion, margen);
  }

  switch(gatillo)
  {
    case PULSACION_GAS:
    {
      EntrarOpcion(Opcion);
      break;
    }
    case PULSACION_FRENO:
    {
      if (Opcion == 3) return;
      EscribirLed7s(Opciones[++Opcion],1);
      break;
    }
      
  }
}

void EntrarOpcion(int Opcion)
{
  
}
