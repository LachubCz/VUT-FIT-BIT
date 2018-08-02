using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Buydostih : MonoBehaviour {
    public GameObject Panel1;
    public GameObject Panel2;
    public GameObject Panel3;
    public GameObject Panel4;
    public GameObject Panel5;
    public GameObject One;
    public int counter = 1;

    //funkce provede zmenu mnozstvi penez v demo ukazce
    public void show()
    {
        switch (counter)
        {
            case 1:
                Panel1.gameObject.SetActive(true);
                One.GetComponent<Text>().text = " $ 24 800";
                counter++;
                break;
            case 2:
                Panel2.gameObject.SetActive(true);
                One.GetComponent<Text>().text = " $ 23 800";
                counter++;
                break;
            case 3:
                Panel3.gameObject.SetActive(true); One.GetComponent<Text>().text = " $ 22 800";
                counter++;
                break;
            case 4:
                Panel4.gameObject.SetActive(true); One.GetComponent<Text>().text = " $ 21 800";
                counter++;
                break;
            case 5:
                Panel5.gameObject.SetActive(true); One.GetComponent<Text>().text = " $ 20 800";
                counter++;
                break;
        }
    }
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
