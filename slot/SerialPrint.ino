
int SerialPrintActivo = 0;

void SerialPrint(char *cad, int num)
{
    Serial.print(cad);
    Serial.println(num);
}
void SerialPrint(char *cad, int num,char *cad1, int num1)
{
    Serial.print(cad);
    Serial.print(num);
    Serial.print(cad1);
    Serial.println(num1);
}

void SerialPrint(char *cad)
{
    Serial.println(cad);
}
void SerialPrint(char *cad, char *cad1)
{
    Serial.print(cad);
    Serial.println(cad1);
}

void SerialPrintValor(char *mensaje, int v, int v1, int ini, int corte)
{
  if (v>=ini)
  {
    if (SerialPrintActivo) 
    {
      Serial.print(mensaje);
      Serial.print(v);
      if (v1 > 0)
      {
        Serial.print("; ");
        Serial.print(v1);
      }
      Serial.println();
    }
    if (v>=corte)
      SerialPrintActivo = 0;
  }
}

void IniSerialPrintValor()
{
  SerialPrintActivo = 1;
}
