using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ChangeMoney : MonoBehaviour {
    public GameObject One;
    public GameObject Two;
    public GameObject Three;

	//zmena mnozstvi penez na prvnim miste
    public void changeOne(string str)
    {
        One.GetComponent<Text>().text = str;
    }
	//zmena mnozstvi penez na druhem miste
    public void changeTwo(string str)
    {
        Two.GetComponent<Text>().text = str;
    }
	//zmena mnozstvi penez na tretim miste
    public void changeThree(string str)
    {
        Three.GetComponent<Text>().text = str;
    }

    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
