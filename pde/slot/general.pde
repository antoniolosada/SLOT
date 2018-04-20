int Espera(int ms, int ini)
{
  static tiempo = 0;
  
  if (ini)
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
