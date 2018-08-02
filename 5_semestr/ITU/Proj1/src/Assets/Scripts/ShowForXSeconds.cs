using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShowForXSeconds : MonoBehaviour
{
    public float num;  //rychlost mizeni panelu
    public float start=  1f;//delka zobrazeni panelu
    public bool yesno;  //je panel zobrazen nebo ne
    public GameObject Panel;  //panel
    public List<GameObject> DiceNumber = new List<GameObject>(6); //objekty reprezentujici jednotlive hody kostkou

    
	//metoda na zviditelneni objektu
    public void makevisible(int number)
    {
        start = 1f;
        Panel.gameObject.SetActive(true);
        DiceNumber[number].gameObject.SetActive(true);
        yesno = true;
    }

	void Start () {
		
	}
	
	// pri kazdem updatu se kontroluje zdali se ma panel skryt nebo ne
	void Update ()
    {
        if (yesno)
        {
            start -= num;
           
        }
        if (start <= 0)
        {
            for (int i = 0; i < 6; i++)
            {
                DiceNumber[i].gameObject.SetActive(false);
            }
            Panel.gameObject.SetActive(false);

            yesno = false;
        }
	}
}
