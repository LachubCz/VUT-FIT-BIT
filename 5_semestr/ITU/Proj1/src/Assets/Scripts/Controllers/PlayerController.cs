using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    public string Name;
    public int Money;
    public int NehrajeKol = 0;  //na zacatku kola se kontroluje zdali hrac muze hrat (pokud je mimo dve kola cislo je 2)

    private List<HorseController> Kone = new List<HorseController>(); //kone ktere vlastni hrac, vyhledavat v nich pomoci ID

    
	public List<HorseController> GetHorseList()
    {
        return this.Kone;
    }

    public void RemoveHorse(int horse_id)
    {
        for (int i = 0; i < this.Kone.Count; i++)
        {
            if (this.Kone[i].ID == horse_id)
            {
                this.Kone.RemoveAt(i);
            }
        }
    }

    public void AddHorse(HorseController kun)
    {
        this.Kone.Add(kun);
    }

    public void ChangeMoney(int checkout)  //pokud se peníze budou odecitat bude zde zaporna hodnota
    {
        this.Money += checkout;
    }

    public void NebudeHrat(int pocetKol)//zmena nehraje kol
    {
        this.NehrajeKol = pocetKol;
    }
}