using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FinanceController : MonoBehaviour
{

    //kazda metoda bude ovlivnovat aktualniho hrace, ostatni hraci - bude se volat z GameControleru
    public void Finance1(List<PlayerController> Players, int index)  //seznam hracskych kontroleru a index hrace ktery stoupl na napovedu
    {
        String Text = "Jdi o 3 pole zpět.";

        //provedeni metody
    }

    public void Finance2(List<PlayerController> Players, int index)
    {
        String Text = "Zrušen distanc (kartu lze zachovat pro pozdější použití, nebo prodat).";
    }

    public void Finance3(List<PlayerController> Players, int index)
    {
        String Text = "Jedeš se zúčastnit trenérského kurzu. Postoupíš na nejbližší pole Trenér. Dostaneš 4.000, pokud jedeš dopředu přes Start.";
    }

    public void Finance4(List<PlayerController> Players, int index)
    {
        String Text = "Zdržíš se na 2 kola.";
    }

    public void Finance5(List<PlayerController> Players, int index)
    {
        String Text = "Distanc (bez 4.000).";
    }
    public void Finance6(List<PlayerController> Players, int index)
    {
        String Text = "Zpět na nejbližší pole Finance.";
    }

    public void Finance7(List<PlayerController> Players, int index)
    {
        String Text = "Zpět na poslední pole ve hře (kůň Napoli), hráč obdrží 4.000.";
    }

    public void Finance8(List<PlayerController> Players, int index)
    {
        String Text = "Zpět na pole Distanc. Obdržíš 4.000, pokud jsi cestou zpět prošel Start.";
    }

    public void Finance9(List<PlayerController> Players, int index)
    {
        String Text = "Zpět na nejbližší pole Finance.";
    }

    public void Finance10(List<PlayerController> Players, int index)
    {
        String Text = "Zpět na start (hráč obdrží 4.000).";
    }
    public void Finance11(List<PlayerController> Players, int index)
    {
        String Text = "Zpět na start (bez 4.000).";
    }

    public void Finance12(List<PlayerController> Players, int index)
    {
        String Text = "Zdržíš se na 2 kola.";
    }

    public void Finance13(List<PlayerController> Players, int index)
    {
        String Text = "Zdržíš se na 1 kolo.";
    }

    public void Finance14(List<PlayerController> Players, int index)
    {
        String Text = "Zpět na pole Parkoviště. Dostaneš 4.000, pokud jsi cestou zpět prošel start.";
    }
}