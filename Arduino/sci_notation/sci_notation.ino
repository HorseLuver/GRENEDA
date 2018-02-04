/*
   Name:  Arthur Bondar
   Date:  January 25
   This function converts number to scientific notation
   with 6 digits of percision
*/

// Example of use
void setup()
{
  Serial.begin(9600);
  Serial.println("--- Booted ---");
}

void loop()
{
  String number;
  char buff[30];
  while (!Serial.available());
  number = Serial.readString();
  Serial.println(number);
  number = sciNotation(number.toFloat());
  Serial.println(buff);
}

/*
   This function takes a float and converts in into scientific notation
   with 5 significant digits -> (+1.12345+EE)
   Example: 100.123 -> (+1.00123+02)
   Return of type String
*/
String sciNotation (float number)
{
  char buff[20];
  uint8_t digit[6], exponent = 0;
  char mntsa_sign, exp_sign;
  /*  getting mantissa sign */
  mntsa_sign = ((number >= 0) ? '+' : '-');
  /*  Absolute value */
  if (number < 0) number *= -1;
  /*  getting exponent sign */
  exp_sign = ((number > -1 && number < 1) ? '-' : '+');
  /*  loop to calculate the exponent  */
  if (exp_sign == '-')
  { /* when number < -10 --- 10 < number */
    for ( ; number < 1; number *= 10) exponent++;
  }
  else
  { /* when -1 > number > 1 */
    for ( ; number >= 10; number /= 10) exponent++;
  }
  /*  breaking down and storing each digit  */
  uint64_t temp = number * 100000;
  for (uint8_t i = 0; i < 6; i++, temp /= 10)
    digit[5 - i] = temp % 10;
  if (exponent < 10)
    sprintf(buff, "(%c%i.%i%i%i%i%i%c0%i)", mntsa_sign, digit[0], digit[1], digit[2], digit[3], digit[4], digit[5], exp_sign, exponent);
  else
    sprintf(buff, "(%c%i.%i%i%i%i%i%c%i)", mntsa_sign, digit[0], digit[1], digit[2], digit[3], digit[4], digit[5], exp_sign, exponent);

  return String(buff);
}

