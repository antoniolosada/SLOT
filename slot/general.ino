// ms = 0 inicializa la cuenta
// ms = valor despues de inicializada devuelve 1 cuando se excede el valor
int Espera(int ms)
{
  static long tiempo = 0;
  
  if (ms == 0)
    tiempo = millis();
  else
  {
    if (millis() - tiempo >= ms)
    {
      return 1;
    }
  }
  return 0;
}

int ctoi(char c)
{
  return c-'0';
}
