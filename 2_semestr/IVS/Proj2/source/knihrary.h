/**
* @file { knihrary.h }
* @brief { Soubor obsahuje matematickou knihovnu pro ETEPC-5000 }
*/


#include <cmath>
/**
 *
 * @brief      { scitani }
 *
 * @param      i1     { prvni scitane cislo }
 * @param      i2     { druhe scitane cislo }
 *
 * @return     { navraci vysledek scitani }
 */
double add(double i1, double i2)
{
    if (i1 == NAN || i2 == NAN)
    {
        return NAN;
    }

    i1 = i1 * 10000;
    i2 = i2 * 10000;
    double vys = i1 + i2;
    vys= vys / 10000;
    return vys;
}

/**
 *
 * @brief      { odcitani }
 *
 * @param      i1     { cislo od ktereho se odcita }
 * @param      i2     { odcitane cislo }
 *
 * @return     { navraci vysledek odcitani }
 */
double sub(double i1, double i2)
{
    if (i1 == NAN || i2 == NAN)
    {
        return NAN;
    }

    i1 = i1 * 10000;
    i2 = i2 * 10000;
    double vys = i1 - i2;
    vys= vys / 10000;
    return vys;
}

/**
 *
 * @brief      { nasobeni }
 *
 * @param      i1     { prvni cislo nasobeni }
 * @param      i2     { druhe cislo nasobeni }
 *
 * @return     { navraci vysledek nasobeni }
 */
double mul(double i1, double i2)
{
    if ((i1 == NAN) || (i2 == NAN))
    {
        return NAN;
    }

    if (i2 == 0)
        return 0;

    if (i1 == 0)
        return 0;

    if (i2 == 1)
        return i1;

    if (i1 == 1)
        return i2;

    return i1*i2;
}

/**
 *
 * @brief      { deleni }
 *
 * @param      i1     { delene cislo }
 * @param      i2     { delitel }
 *
 * @return     { navraci vysledek deleni }
 */
double division(double i1, double i2)
{
    if ((i1 == NAN) || (i2 == NAN))
    {
        return NAN;
    }

    if (i2 == 0)
    {
        return NAN;
    }

    if (i1 == i2)
        return 1;

    if (i1 == 0)
        return 0;

    if (i2 == 1)
        return i1;

    return i1/i2;
}

/**
 *
 * @brief      { faktorial }
 *
 * @param      i1     { cislo z ktereho se pocita faktorial }
 *
 * @return     { navraci faktorial }
 */
double fact(double i1)
{
  double c, fact = 1;

  if (i1 == NAN)
  {
    return NAN;
  }

  if (i1 < 0)
  {
    return NAN;
  }

  if (i1-(int)i1 != 0)
  {
    return NAN;
  }

  if (i1 > 150)
  {
      return NAN;
  }

  if (i1 == NAN)
  {
      return NAN;
  }

  for (c = 1; c <= i1; c++)
    fact = fact * c;

  return fact;
}

/**
 *
 * @brief      { mocnina z cisla s prirozenym exponentem }
 *
 * @param      i1     { umocnovane cislo }
 * @param      i2     { exponent }
 *
 * @return     { navraci umocnene cislo }
 */
double power (double i1, double i2)
{

    if (i1 == NAN || i2 == NAN)
    {
        return NAN;
    }

    if (i2 == 0)
    {
        if (i1 == 0)
        {
            return NAN;
        }
        else
            return 1;
    }

    if(i2 < 0)
    {
        return NAN;
    }

    if (i2-(int)i2 != 0)
    {
      return NAN;
    }

    if ((i2 > 450) && (i1 != 1))
    {
        return NAN;
    }

    if (i1 == 1)
    {
        return 1;
    }

    double temp = i1;

    while (i2 != 1)
    {
        temp = temp * i1;
        i2--;
    }

    return temp;
}

/**
 *
 * @brief      { prirozeny logaritmus }
 *
 * @param      x     { logaritmovane cislo }
 *
 * @return     { navraci prirozeny logaritmus z cisla x }
 */
double ln (double x)  //funkce pro vypocet prirozeneho logaritmu pomoci zretezeneho zlomku
{
    double cf = 1.0;
    double a, b;
    unsigned int n = 1000000;

    if (x < 0)  //osetreni zaporneho cisla
    {
        return NAN;
    }

    if (x == 0)  //osetreni nuly
    {
        return -INFINITY;
    }

    if (x == INFINITY)  //osetreni nekonecna
    {
        return INFINITY;
    }

    if (x == NAN)  //osetreni NAN
    {
        return NAN;
    }

    x = (x - 1)/(1 + x);
    for (; n >= 1; n--)  //cyklus zretezeneho zlomku
    {
        a = x * x * n *n;
        b = 2*n -1;
        cf = b - (a / cf);
    }
    x = (2*x) / cf;
    return x;
}
