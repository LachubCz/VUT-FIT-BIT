using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;
using UnityEngine.UI;

public class ShowPanel : MonoBehaviour {

    public GameObject Panel;
    public int lastInput;

    void Start()
    {

    }
    
    //funkce na skryti/odkriti panelu
    public void showHidePanel()
    {
        if (Panel.activeSelf)
        {
            Panel.gameObject.SetActive(false);
        }
        else
        {
            Panel.gameObject.SetActive(true);
        }
    }

    //funkce na skryti/odkryti vice stranek jednoho panelu
    public void showHidePanel(int i)
    {
        if (Panel.activeSelf)
        {
            if (lastInput == i)
            {
                Panel.transform.GetChild(i).gameObject.SetActive(false);
                Panel.gameObject.SetActive(false);
            }
            else
            {
                Panel.transform.GetChild(lastInput).gameObject.SetActive(false);
                Panel.transform.GetChild(i).gameObject.SetActive(true);
                lastInput = i;
            }
        }
        else
        {
            Panel.gameObject.SetActive(true);
            Panel.transform.GetChild(i).gameObject.SetActive(true);
            lastInput = i;
        }
    }
}
