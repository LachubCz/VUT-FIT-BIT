using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move : MonoBehaviour {
    public GameObject Panel;
    public bool activate =  false;
    public float speed;
    public GameObject Zobar;

    public void change()  //fukce aktivace pohybu
    {
        activate = true;
    }

    void Start () {
		
	}
	
	//pokud je pohyb zapnut, provede se zmena souradnic do konce dosahnuti bodu
	void Update () {
        Vector3 pos = Panel.transform.position;
        if (activate && pos.x > 803f) 
	    {
            //Debug.Log(pos.x);
            pos.x -= speed;
            Panel.transform.position = pos;
        }
	    if (pos.x < 803f)
	    {
            Zobar.gameObject.SetActive(true);
        }
	}
}
