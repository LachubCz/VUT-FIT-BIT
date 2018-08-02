using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NahodaController : MonoBehaviour
{
    //kazda metoda bude ovlivnovat aktualniho hrace, ostatni hraci - bude se volat z GameControleru
    public void Napoveda1(List<PlayerController> Players, int index)  //seznam hracskych kontroleru a index hrace ktery stoupl na napovedu
    {
        String Text = "Zaplať pokutu 1000.";

        //provedeni metody
    }

    public void Napoveda2(List<PlayerController> Players, int index)
    {
        String Text = "Pokuta za nedodržení přepisů 400.";
    }

    public void Napoveda3(List<PlayerController> Players, int index)
    {
        String Text = "Renovuješ všechny stáje. Za každý svůj obsazený dostih zaplať 500.";
    }

    public void Napoveda4(List<PlayerController> Players, int index)
    {
        String Text = "Mimořádný zisk z dostihů obdržíš 2.000.";
    }

    public void Napoveda5(List<PlayerController> Players, int index)
    {
        String Text = "Jako dárek k narozeninám obdržíš od každého 200.";
    }
    public void Napoveda6(List<PlayerController> Players, int index)
    {
        String Text = "Mimořádná prémie 500.";
    }

    public void Napoveda7(List<PlayerController> Players, int index)
    {
        String Text = "Obdržíš dotaci 4.000.";
    }

    public void Napoveda8(List<PlayerController> Players, int index)
    {
        String Text = "Zaplať dluh 3.000.";
    }

    public void Napoveda9(List<PlayerController> Players, int index)
    {
        String Text = "Za každý svůj obsazený dostih zaplať 800, za každý svůj obsazený hlavní dostih sezóny zaplať 2.300.";
    }

    public void Napoveda10(List<PlayerController> Players, int index)
    {
        String Text = "Zaplať příspěvek 2.000.";
    }
    public void Napoveda11(List<PlayerController> Players, int index)
    {
        String Text = "Nákup materiálu na opravu 100.";
    }

    public void Napoveda12(List<PlayerController> Players, int index)
    {
        String Text = "Výhra v loterii 1.000.";
    }

    public void Napoveda13(List<PlayerController> Players, int index)
    {
        String Text = "Obdržíš dotaci 2.000.";
    }

    public void Napoveda14(List<PlayerController> Players, int index)
    {
        String Text = "Z banky obdržíš přeplatek 3.000.";
    }
}
