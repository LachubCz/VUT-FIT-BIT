using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
//using Debug = System.Diagnostics.Debug;

public class HorseController : MonoBehaviour
{
    // promenne obsahujici jedinecne hodnoty pro prave zobrazovaneho konec
    public int ID;

    public string Jmeno;
    public int PorizovaciCena;
    public int ProhlidkaStaji;
    public int NakladyNaDostih;
    public int ZiskZ1Dostihu;
    public int ZiskZ2Dostihu;
    public int ZiskZ3Dostihu;
    public int ZiskZ4Dostihu;
    public int ZiskZ5Dostihu;

    public int HandlirOd;
    public int HandlirDo;
    public int Loterie;
    public int CisloObrazkuKone;

    public string Historie;
    public string Popis;
    public string Vyuziti;

    public Color Color1;
    public Color Color2;
    public Color Color3;

    // promenne objektu zobrazovane karty
    public GameObject shownCardName;
    public GameObject shownCardName2;
    public GameObject shownCardPrice;
    public GameObject shownCardProhlidka;
    public GameObject shownCardFirstDostih;
    public GameObject shownCardSecondDostih;
    public GameObject shownCardThirdDostih;
    public GameObject shownCardFourthDostih;
    public GameObject shownCardFifthDostih;
    public GameObject shownCardDostihPrice;
    public GameObject shownCardHandlir;
    public GameObject shownCardLotery;
    public GameObject shownCardHorsePicture;
    public GameObject shownCardHistory;
    public GameObject shownCardDescription;
    public GameObject shownCardUsage;

    // promenne pro manipulaci s obrazky koni a potrebne hodnoty kostky v loterii
    public Sprite jedna;
    public Sprite dva;
    public Sprite tri;
    public Sprite ctyri;
    public Sprite pet;
    public Sprite sest;

    public Sprite kun1;
    public Sprite kun2;
    public Sprite kun3;
    public Sprite kun4;
    public Sprite kun5;
    public Sprite kun6;
    public Sprite kun7;
    public Sprite kun8;
    public Sprite kun9;
    public Sprite kun10;
    public Sprite kun11;
    public Sprite kun12;
    public Sprite kun13;
    public Sprite kun14;
    public Sprite kun15;
    public Sprite kun16;
    public Sprite kun17;
    public Sprite kun18;
    public Sprite kun19;
    public Sprite kun20;
    public Sprite kun21;
    public Sprite kun22;

    public GameObject NamePanel1;
    public GameObject NamePanel2;

    public GameObject History;
    public GameObject Description;
    public GameObject Usage;
    public GameObject InfoPanel;
    public GameObject PricePanel;
    public GameObject Handler;
    public GameObject Lottery;
    public GameObject Image;

    public GameObject Background1;
    public GameObject Background2;

    // metoda prevadejici barvu v hexadecimalnim tvaru do barvy Color32
    public static Color hexToColor(string hex)
    {
        hex = hex.Replace("0x", "");
        hex = hex.Replace("#", "");
        byte a = 255;
        byte r = byte.Parse(hex.Substring(0, 2), System.Globalization.NumberStyles.HexNumber);
        byte g = byte.Parse(hex.Substring(2, 2), System.Globalization.NumberStyles.HexNumber);
        byte b = byte.Parse(hex.Substring(4, 2), System.Globalization.NumberStyles.HexNumber);
        
        if (hex.Length == 8)
        {
            a = byte.Parse(hex.Substring(6, 2), System.Globalization.NumberStyles.HexNumber);
        }
        return new Color32(r, g, b, a);
    }

    // metoda menici informace v dane nahledove karte v leve casti obrazovky
    public void changeViewCard(string horseName)
    {
        //Debug.Log(horseName);

        // nahrani hodnot daneho kone do promennych teto tridy
        if (loadHorse(horseName) == -1)
            return;

        // zmena barev nahledove karty
        NamePanel1.GetComponent<Image>().color = Color2;
        NamePanel2.GetComponent<Image>().color = Color2;

        History.GetComponent<Image>().color = Color3;
        Description.GetComponent<Image>().color = Color3;
        Usage.GetComponent<Image>().color = Color3;
        InfoPanel.GetComponent<Image>().color = Color3;
        PricePanel.GetComponent<Image>().color = Color3;
        Handler.GetComponent<Image>().color = Color3;
        Lottery.GetComponent<Image>().color = Color3;
        Image.GetComponent<Image>().color = Color3;

        Background1.GetComponent<Image>().color = Color1;
        Background2.GetComponent<Image>().color = Color1;

        // zmena jmena v nahledove karte
        Text shownCardTextName = shownCardName.GetComponent<Text>();
        shownCardTextName.text = this.Jmeno;

        Text shownCardTextName2 = shownCardName2.GetComponent<Text>();
        shownCardTextName2.text = this.Jmeno;

        // zmena ceny kone v nahledove karte
        Text shownCardTextPrice = shownCardPrice.GetComponent<Text>();
        shownCardTextPrice.text = this.PorizovaciCena.ToString();

        // zmena ceny prohlidky staje v nahledove karte
        Text shownCardTextProhlidka = shownCardProhlidka.GetComponent<Text>();
        shownCardTextProhlidka.text = this.ProhlidkaStaji.ToString();

        // zmeny hodnot zisku z dostihu v nahledove karte
        Text shownCardTextFirstDostih = shownCardFirstDostih.GetComponent<Text>();
        shownCardTextFirstDostih.text = this.ZiskZ1Dostihu.ToString();

        Text shownCardTextSecondDostih = shownCardSecondDostih.GetComponent<Text>();
        shownCardTextSecondDostih.text = this.ZiskZ2Dostihu.ToString();

        Text shownCardTextThirdDostih = shownCardThirdDostih.GetComponent<Text>();
        shownCardTextThirdDostih.text = this.ZiskZ3Dostihu.ToString();

        Text shownCardTextFourthDostih = shownCardFourthDostih.GetComponent<Text>();
        shownCardTextFourthDostih.text = this.ZiskZ4Dostihu.ToString();

        Text shownCardTextFifthDostih = shownCardFifthDostih.GetComponent<Text>();
        shownCardTextFifthDostih.text = this.ZiskZ5Dostihu.ToString();

        // zmena nakladu na dostih v nahledove karte
        Text shownCardTextDostihPrice = shownCardDostihPrice.GetComponent<Text>();
        shownCardTextDostihPrice.text = this.NakladyNaDostih.ToString();

        // zmena ceny u handlire v nahledove karte
        Text shownCardTextHandlir = shownCardHandlir.GetComponent<Text>();
        shownCardTextHandlir.text = this.HandlirOd.ToString() + " - " + this.HandlirDo.ToString();

        // zmena historie kone, popisu a vyuziti v nahledove karte
        Text shownCardHistoryText = shownCardHistory.GetComponent<Text>();
        shownCardHistoryText.text = this.Historie;

        Text shownCardDescriptionText = shownCardDescription.GetComponent<Text>();
        shownCardDescriptionText.text = this.Popis;

        Text shownCardUsageText = shownCardUsage.GetComponent<Text>();
        shownCardUsageText.text = this.Vyuziti;

        // zmena potrebne hodnoty kostky v loterii v nahledove karte
        Image shownCardImageLotery = shownCardLotery.GetComponent<Image>();
        switch (this.Loterie)
        {
            case 1:
			shownCardImageLotery.sprite = jedna;
                break;
            case 2:
			shownCardImageLotery.sprite = dva;
                break;
            case 3:
			shownCardImageLotery.sprite = tri;
                break;
            case 4:
			shownCardImageLotery.sprite = ctyri;
                break;
            case 5:
			shownCardImageLotery.sprite = pet;
                break;
            case 6:
			shownCardImageLotery.sprite = sest;
                break;
        }

        // zmena ikonky kone v nahledove karte
        Image HorsePicture = shownCardHorsePicture.GetComponent<Image>();
        switch (this.CisloObrazkuKone)
        {
            case 1:
                HorsePicture.sprite = kun1;
                break;
            case 2:
                HorsePicture.sprite = kun2;
                break;
            case 3:
                HorsePicture.sprite = kun3;
                break;
            case 4:
                HorsePicture.sprite = kun4;
                break;
            case 5:
                HorsePicture.sprite = kun5;
                break;
            case 6:
                HorsePicture.sprite = kun6;
                break;
            case 7:
                HorsePicture.sprite = kun7;
                break;
            case 8:
                HorsePicture.sprite = kun8;
                break;
            case 9:
                HorsePicture.sprite = kun9;
                break;
            case 10:
                HorsePicture.sprite = kun10;
                break;
            case 11:
                HorsePicture.sprite = kun11;
                break;
            case 12:
                HorsePicture.sprite = kun12;
                break;
            case 13:
                HorsePicture.sprite = kun13;
                break;
            case 14:
                HorsePicture.sprite = kun14;
                break;
            case 15:
                HorsePicture.sprite = kun15;
                break;
            case 16:
                HorsePicture.sprite = kun16;
                break;
            case 17:
                HorsePicture.sprite = kun17;
                break;
            case 18:
                HorsePicture.sprite = kun18;
                break;
            case 19:
                HorsePicture.sprite = kun19;
                break;
            case 20:
                HorsePicture.sprite = kun20;
                break;
            case 21:
                HorsePicture.sprite = kun21;
                break;
            case 22:
                HorsePicture.sprite = kun22;
                break;
        }
    }

    // metoda volajici spravnou metodu na plneni promennych skriptu na zaklade jmena kone
    int loadHorse(string jmenoKone)
    {
        switch (jmenoKone)
        {
            case "Fantome": Fantome(); break;
            case "Gavora": Gavora(); break;
            case "Lady Anne": LadyAnne(); break;
            case "Pasek": Pasek(); break;
            case "Koran": Koran(); break;
            case "Neklan": Neklan(); break;
            case "Portlancl": Portlancl(); break;
            case "Japan": Japan(); break;
            case "Kostrava": Kostrava(); break;
            case "Lukava": Lukava(); break;
            case "Melak": Melak(); break;
            case "Grifel": Grifel(); break;
            case "Mohyla": Mohyla(); break;
            case "Metal": Metal(); break;
            case "Tara": Tara(); break;
            case "Furioso": Furioso(); break;
            case "Genius": Genius(); break;
            case "Shagga": Shagga(); break;
            case "Dahoman": Dahoman(); break;
            case "Gira": Gira(); break;
            case "Narcius": Narcius(); break;
            case "Napoli": Napoli(); break;
            default: return -1;
            
        }
        return 0;
    }

    /*
     * nasleduji metody plni promenne skriptu spravnymi hodnotami odpovidajici danemu koni
     */

    void Fantome()
    {
        this.Jmeno = "Fantome";
        this.PorizovaciCena = 1200;
        this.ProhlidkaStaji = 40;
        this.NakladyNaDostih = 1000;
        this.ZiskZ1Dostihu = 200;
        this.ZiskZ2Dostihu = 600;
        this.ZiskZ3Dostihu = 1800;
        this.ZiskZ4Dostihu = 3200;
        this.ZiskZ5Dostihu = 5000;
        this.HandlirOd = 2400;
        this.HandlirDo = 3600;
        this.Loterie = 1;
        this.CisloObrazkuKone = 1;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("A25242FF");
        this.Color2 = hexToColor("E83C32FF"); 
        this.Color3 = hexToColor("E8584FFF");
    }

    void Gavora()
    {
        this.Jmeno = "Gavora";
        this.PorizovaciCena = 1200;
        this.ProhlidkaStaji = 40;
        this.NakladyNaDostih = 1000;
        this.ZiskZ1Dostihu = 200;
        this.ZiskZ2Dostihu = 600;
        this.ZiskZ3Dostihu = 1800;
        this.ZiskZ4Dostihu = 3200;
        this.ZiskZ5Dostihu = 5000;
        this.HandlirOd = 2400;
        this.HandlirDo = 3600;
        this.Loterie = 2;
        this.CisloObrazkuKone = 2;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("A25242FF");
        this.Color2 = hexToColor("E83C32FF");
        this.Color3 = hexToColor("E8584FFF");
    }

    void LadyAnne()
    {
        this.Jmeno = "LadyAnne";
        this.PorizovaciCena = 2000;
        this.ProhlidkaStaji = 120;
        this.NakladyNaDostih = 1000;
        this.ZiskZ1Dostihu = 600;
        this.ZiskZ2Dostihu = 1800;
        this.ZiskZ3Dostihu = 5400;
        this.ZiskZ4Dostihu = 8000;
        this.ZiskZ5Dostihu = 11000;
        this.HandlirOd = 4000;
        this.HandlirDo = 6000;
        this.Loterie = 3;
        this.CisloObrazkuKone = 3;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("936333FF");
        this.Color2 = hexToColor("83501FFF");
        this.Color3 = hexToColor("9A724BFF");
    }

    void Pasek()
    {
        this.Jmeno = "Pasek";
        this.PorizovaciCena = 2000;
        this.ProhlidkaStaji = 120;
        this.NakladyNaDostih = 1000;
        this.ZiskZ1Dostihu = 600;
        this.ZiskZ2Dostihu = 1800;
        this.ZiskZ3Dostihu = 5400;
        this.ZiskZ4Dostihu = 8000;
        this.ZiskZ5Dostihu = 11000;
        this.HandlirOd = 4000;
        this.HandlirDo = 6000;
        this.Loterie = 4;
        this.CisloObrazkuKone = 4;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("936333FF");
        this.Color2 = hexToColor("83501FFF");
        this.Color3 = hexToColor("9A724BFF");
    }

    void Koran()
    {
        this.Jmeno = "Koran";
        this.PorizovaciCena = 2400;
        this.ProhlidkaStaji = 160;
        this.NakladyNaDostih = 1000;
        this.ZiskZ1Dostihu = 800;
        this.ZiskZ2Dostihu = 2000;
        this.ZiskZ3Dostihu = 6000;
        this.ZiskZ4Dostihu = 9000;
        this.ZiskZ5Dostihu = 12000;
        this.HandlirOd = 4800;
        this.HandlirDo = 7200;
        this.Loterie = 5;
        this.CisloObrazkuKone = 5;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("936333FF");
        this.Color2 = hexToColor("83501FFF");
        this.Color3 = hexToColor("9A724BFF");
    }

    void Neklan()
    {
        this.Jmeno = "Neklan";
        this.PorizovaciCena = 2800;
        this.ProhlidkaStaji = 200;
        this.NakladyNaDostih = 2000;
        this.ZiskZ1Dostihu = 1000;
        this.ZiskZ2Dostihu = 3000;
        this.ZiskZ3Dostihu = 9000;
        this.ZiskZ4Dostihu = 12500;
        this.ZiskZ5Dostihu = 15000;
        this.HandlirOd = 5600;
        this.HandlirDo = 8400;
        this.Loterie = 6;
        this.CisloObrazkuKone = 6;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("5D97AEFF");
        this.Color2 = hexToColor("12ACEDFF");
        this.Color3 = hexToColor("6CC8EEFF");
    }

    void Portlancl()
    {
        this.Jmeno = "Portlancl";
        this.PorizovaciCena = 2800;
        this.ProhlidkaStaji = 200;
        this.NakladyNaDostih = 2000;
        this.ZiskZ1Dostihu = 1000;
        this.ZiskZ2Dostihu = 3000;
        this.ZiskZ3Dostihu = 9000;
        this.ZiskZ4Dostihu = 12500;
        this.ZiskZ5Dostihu = 15000;
        this.HandlirOd = 5600;
        this.HandlirDo = 8400;
        this.Loterie = 1;
        this.CisloObrazkuKone = 7;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("5D97AEFF");
        this.Color2 = hexToColor("12ACEDFF");
        this.Color3 = hexToColor("6CC8EEFF");
    }

    void Japan()
    {
        this.Jmeno = "Japan";
        this.PorizovaciCena = 2800;
        this.ProhlidkaStaji = 240;
        this.NakladyNaDostih = 2000;
        this.ZiskZ1Dostihu = 1200;
        this.ZiskZ2Dostihu = 3600;
        this.ZiskZ3Dostihu = 10000;
        this.ZiskZ4Dostihu = 14000;
        this.ZiskZ5Dostihu = 18000;
        this.HandlirOd = 5600;
        this.HandlirDo = 8400;
        this.Loterie = 2;
        this.CisloObrazkuKone = 8;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("5D97AEFF");
        this.Color2 = hexToColor("12ACEDFF");
        this.Color3 = hexToColor("6CC8EEFF");
    }

    void Kostrava()
    {
        this.Jmeno = "Kostrava";
        this.PorizovaciCena = 3600;
        this.ProhlidkaStaji = 280;
        this.NakladyNaDostih = 2000;
        this.ZiskZ1Dostihu = 1400;
        this.ZiskZ2Dostihu = 4000;
        this.ZiskZ3Dostihu = 11000;
        this.ZiskZ4Dostihu = 15000;
        this.ZiskZ5Dostihu = 19000;
        this.HandlirOd = 7200;
        this.HandlirDo = 10800;
        this.Loterie = 3;
        this.CisloObrazkuKone = 9;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("419734FF");
        this.Color2 = hexToColor("2FBA1BFF");
        this.Color3 = hexToColor("67D757FF");
    }

    void Lukava()
    {
        this.Jmeno = "Lukava";
        this.PorizovaciCena = 3600;
        this.ProhlidkaStaji = 280;
        this.NakladyNaDostih = 2000;
        this.ZiskZ1Dostihu = 1400;
        this.ZiskZ2Dostihu = 4000;
        this.ZiskZ3Dostihu = 11000;
        this.ZiskZ4Dostihu = 15000;
        this.ZiskZ5Dostihu = 19000;
        this.HandlirOd = 7200;
        this.HandlirDo = 10800;
        this.Loterie = 4;
        this.CisloObrazkuKone = 10;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("419734FF");
        this.Color2 = hexToColor("2FBA1BFF");
        this.Color3 = hexToColor("67D757FF");
    }

    void Melak()
    {
        this.Jmeno = "Melak";
        this.PorizovaciCena = 4000;
        this.ProhlidkaStaji = 320;
        this.NakladyNaDostih = 2000;
        this.ZiskZ1Dostihu = 1600;
        this.ZiskZ2Dostihu = 4400;
        this.ZiskZ3Dostihu = 12000;
        this.ZiskZ4Dostihu = 16000;
        this.ZiskZ5Dostihu = 20000;
        this.HandlirOd = 8000;
        this.HandlirDo = 12000;
        this.Loterie = 5;
        this.CisloObrazkuKone = 11;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("419734FF");
        this.Color2 = hexToColor("2FBA1BFF");
        this.Color3 = hexToColor("67D757FF");
    }

    void Grifel()
    {
        this.Jmeno = "Grifel";
        this.PorizovaciCena = 4400;
        this.ProhlidkaStaji = 360;
        this.NakladyNaDostih = 3000;
        this.ZiskZ1Dostihu = 1800;
        this.ZiskZ2Dostihu = 5000;
        this.ZiskZ3Dostihu = 14000;
        this.ZiskZ4Dostihu = 17000;
        this.ZiskZ5Dostihu = 21000;
        this.HandlirOd = 8800;
        this.HandlirDo = 13200;
        this.Loterie = 6;
        this.CisloObrazkuKone = 12;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("7B3D67FF");
        this.Color2 = hexToColor("CB359AFF");
        this.Color3 = hexToColor("D95CB0FF");
    }

    void Mohyla()
    {
        this.Jmeno = "Mohyla";
        this.PorizovaciCena = 4400;
        this.ProhlidkaStaji = 360;
        this.NakladyNaDostih = 3000;
        this.ZiskZ1Dostihu = 1800;
        this.ZiskZ2Dostihu = 5000;
        this.ZiskZ3Dostihu = 14000;
        this.ZiskZ4Dostihu = 17000;
        this.ZiskZ5Dostihu = 21000;
        this.HandlirOd = 8800;
        this.HandlirDo = 13200;
        this.Loterie = 1;
        this.CisloObrazkuKone = 13;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("7B3D67FF");
        this.Color2 = hexToColor("CB359AFF");
        this.Color3 = hexToColor("D95CB0FF");
    }

    void Metal()
    {
        this.Jmeno = "Metal";
        this.PorizovaciCena = 4800;
        this.ProhlidkaStaji = 400;
        this.NakladyNaDostih = 3000;
        this.ZiskZ1Dostihu = 2000;
        this.ZiskZ2Dostihu = 6000;
        this.ZiskZ3Dostihu = 15000;
        this.ZiskZ4Dostihu = 18000;
        this.ZiskZ5Dostihu = 22000;
        this.HandlirOd = 9600;
        this.HandlirDo = 14400;
        this.Loterie = 2;
        this.CisloObrazkuKone = 14;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("7B3D67FF");
        this.Color2 = hexToColor("CB359AFF");
        this.Color3 = hexToColor("D95CB0FF");
    }

    void Tara()
    {
        this.Jmeno = "Tara";
        this.PorizovaciCena = 5200;
        this.ProhlidkaStaji = 440;
        this.NakladyNaDostih = 3000;
        this.ZiskZ1Dostihu = 2200;
        this.ZiskZ2Dostihu = 6600;
        this.ZiskZ3Dostihu = 16000;
        this.ZiskZ4Dostihu = 19500;
        this.ZiskZ5Dostihu = 23000;
        this.HandlirOd = 10400;
        this.HandlirDo = 15600;
        this.Loterie = 3;
        this.CisloObrazkuKone = 15;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("AA934CFF");
        this.Color2 = hexToColor("F2B700FF");
        this.Color3 = hexToColor("F2CB54FF");
    }

    void Furioso()
    {
        this.Jmeno = "Furioso";
        this.PorizovaciCena = 5200;
        this.ProhlidkaStaji = 440;
        this.NakladyNaDostih = 3000;
        this.ZiskZ1Dostihu = 2200;
        this.ZiskZ2Dostihu = 6600;
        this.ZiskZ3Dostihu = 16000;
        this.ZiskZ4Dostihu = 19500;
        this.ZiskZ5Dostihu = 23000;
        this.HandlirOd = 10400;
        this.HandlirDo = 15600;
        this.Loterie = 4;
        this.CisloObrazkuKone = 16;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("AA934CFF");
        this.Color2 = hexToColor("F2B700FF");
        this.Color3 = hexToColor("F2CB54FF");
    }

    void Genius()
    {
        this.Jmeno = "Genius";
        this.PorizovaciCena = 5600;
        this.ProhlidkaStaji = 480;
        this.NakladyNaDostih = 3000;
        this.ZiskZ1Dostihu = 2400;
        this.ZiskZ2Dostihu = 7200;
        this.ZiskZ3Dostihu = 17000;
        this.ZiskZ4Dostihu = 20500;
        this.ZiskZ5Dostihu = 24000;
        this.HandlirOd = 11200;
        this.HandlirDo = 16800;
        this.Loterie = 5;
        this.CisloObrazkuKone = 17;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("AA934CFF");
        this.Color2 = hexToColor("F2B700FF");
        this.Color3 = hexToColor("F2CB54FF");
    }

    void Shagga()
    {
        this.Jmeno = "Shagga";
        this.PorizovaciCena = 6000;
        this.ProhlidkaStaji = 500;
        this.NakladyNaDostih = 4000;
        this.ZiskZ1Dostihu = 2600;
        this.ZiskZ2Dostihu = 7800;
        this.ZiskZ3Dostihu = 18000;
        this.ZiskZ4Dostihu = 22000;
        this.ZiskZ5Dostihu = 25500;
        this.HandlirOd = 12000;
        this.HandlirDo = 18000;
        this.Loterie = 6;
        this.CisloObrazkuKone = 18;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("36621EFF");
        this.Color2 = hexToColor("4C852CFF");
        this.Color3 = hexToColor("698F55FF");
    }

    void Dahoman()
    {
        this.Jmeno = "Dahoman";
        this.PorizovaciCena = 6000;
        this.ProhlidkaStaji = 500;
        this.NakladyNaDostih = 4000;
        this.ZiskZ1Dostihu = 2600;
        this.ZiskZ2Dostihu = 7800;
        this.ZiskZ3Dostihu = 18000;
        this.ZiskZ4Dostihu = 22000;
        this.ZiskZ5Dostihu = 25500;
        this.HandlirOd = 12000;
        this.HandlirDo = 18000;
        this.Loterie = 1;
        this.CisloObrazkuKone = 19;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("36621EFF");
        this.Color2 = hexToColor("4C852CFF");
        this.Color3 = hexToColor("698F55FF");
    }

    void Gira()
    {
        this.Jmeno = "Gira";
        this.PorizovaciCena = 6400;
        this.ProhlidkaStaji = 560;
        this.NakladyNaDostih = 4000;
        this.ZiskZ1Dostihu = 3000;
        this.ZiskZ2Dostihu = 9000;
        this.ZiskZ3Dostihu = 20000;
        this.ZiskZ4Dostihu = 24000;
        this.ZiskZ5Dostihu = 28000;
        this.HandlirOd = 12800;
        this.HandlirDo = 19200;
        this.Loterie = 2;
        this.CisloObrazkuKone = 20;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("36621EFF");
        this.Color2 = hexToColor("4C852CFF");
        this.Color3 = hexToColor("698F55FF");
    }

    void Narcius()
    {
        this.Jmeno = "Narcius";
        this.PorizovaciCena = 7000;
        this.ProhlidkaStaji = 700;
        this.NakladyNaDostih = 4000;
        this.ZiskZ1Dostihu = 3500;
        this.ZiskZ2Dostihu = 10000;
        this.ZiskZ3Dostihu = 22000;
        this.ZiskZ4Dostihu = 26000;
        this.ZiskZ5Dostihu = 30000;
        this.HandlirOd = 14000;
        this.HandlirDo = 21000;
        this.Loterie = 3;
        this.CisloObrazkuKone = 21;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("604182FF");
        this.Color2 = hexToColor("8F32F1FF");
        this.Color3 = hexToColor("A272D6FF");
    }

    void Napoli()
    {
        this.Jmeno = "Napoli";
        this.PorizovaciCena = 8000;
        this.ProhlidkaStaji = 1000;
        this.NakladyNaDostih = 4000;
        this.ZiskZ1Dostihu = 4000;
        this.ZiskZ2Dostihu = 12000;
        this.ZiskZ3Dostihu = 28000;
        this.ZiskZ4Dostihu = 34000;
        this.ZiskZ5Dostihu = 40000;
        this.HandlirOd = 16000;
        this.HandlirDo = 24000;
        this.Loterie = 4;
        this.CisloObrazkuKone = 22;
        this.Historie = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam in lorem sit amet leo accumsan lacinia.";
        this.Popis = "Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Fusce nibh.";
        this.Vyuziti = "Donec vitae arcu. Sed convallis magna eu sem. Nullam eget nisl.";
        this.Color1 = hexToColor("604182FF");
        this.Color2 = hexToColor("8F32F1FF");
        this.Color3 = hexToColor("A272D6FF");
    }
}
