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
