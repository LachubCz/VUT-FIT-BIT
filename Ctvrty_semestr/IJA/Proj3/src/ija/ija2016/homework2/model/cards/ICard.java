package ija.ija2016.homework2.model.cards;

/**
 * Implementace rozhrani Card
 */
public class ICard implements Card 
{
	protected int value;
	
	protected ICard.Color c;
	
	protected boolean TurnedFaceUp;

	/**
	 * [Konstruktor ICard.]
	 * @param  c     [Barva karty.]
	 * @param  value [Hodnota karty.]
	 * @return       [Void.]
	 */
	public ICard(Card.Color c, int value)
	{
		if (value > 13 || value < 1 )
			System.exit(0);
		
		this.c = c;
		this.value = value;
		this.TurnedFaceUp = false;
	}
	
	/**
	 * [Vraci hodnotu karty.]
	 * @return [Hodnota karty.]
	 */
	public int value()
	{
		return value;
	}
	
	/**
	 * [Vrati barvu karty.]
	 * @return [Barva karty.]
	 */
	public ICard.Color color()
	{
		return c;
	}
	
	/**
	 * [Prevede barvu a hodnotu karty na kompaktni tvar.]
	 * @return [Retezec obsahujici kompaktni tvar karty.]
	 */
	public String toString()
	{
		String str = null;
		
		if (this.value > 13)
			System.exit(0);
		
		if (this.value < 11 && this.value != 1)
		{
			str = String.format ("%d", this.value);
			str+="(";
			str+=this.c.toString();
			str+=")";
			return str;
		}
		if(this.value == 1)
		{
			str = "A";
		}	
		if(this.value == 11)
		{
			str = "J";
		}	
		if(this.value == 12)
		{
			str = "Q";
		}	
		if(this.value == 13)
		{
			str = "K";
		}	
			str+="(";
			str+=this.c.toString();
			str+=")";
			return str;
	}
	
	/**
	 * [Metoda pro porovnani dvou objektu typu card.]
	 * @param  o [Porovnavany objekt.]
	 * @return   [Vraci true, pokud jsou objekty shodne.]
	 */
	public boolean equals(Object o)
	{
        if (o == this) return true;
        if (!(o instanceof Card)) {
            return false;
        }

        ICard card = (ICard) o;

        return card.c.equals(c) &&
                card.value == value;
    }
	
	/**
	 * [Metoda hashCode pro objekt typu card.]
	 * @return [Vraci zahashovany int z hashovaci metody.]
	 */
    public int hashCode() 
    {
        int result = 17;
        result = 31 * result + c.hashCode();
        result = 31 * result + value;
        return result;
    }
	
	/**
	 * [Testuje, zda je karta otocena licem nahoru.]
	 * @return [Vysledek testu: true = karta je otocena licem nahoru.]
	 */
	public boolean isTurnedFaceUp()
	{
		if (this.TurnedFaceUp == true)
			return true;
		else
			return false;
	}
	
	/**
	 * [Otoci kartu licem nahoru. Pokud tak uz je, nedela nic.]
	 * @return [Informace, zda doslo k otoceni karty (=true) nebo ne.]
	 */
	public boolean turnFaceUp()
	{
		if (this.isTurnedFaceUp())
		{
			return false;
		}
		else
		{
			this.TurnedFaceUp = true;
			return true;
		}
			
	}
	
	/**
	 * [Testuje, zda ma karta podobnou barvu jako karta zadana. Podobnou barvou se mysli cerna (piky, krize) a cervena (kary a srdce).]
	 * @param  c [Karta pro porovnani.]
	 * @return   [Informace o shode barev karet.]
	 */
	public boolean similarColorTo(Card c)
	{
		if(c.color().equals(Card.Color.HEARTS) || c.color().equals(Card.Color.DIAMONDS))
			if(this.color().equals(Card.Color.HEARTS) || this.color().equals(Card.Color.DIAMONDS))
				return true;
		if(c.color().equals(Card.Color.CLUBS) || c.color().equals(Card.Color.SPADES))
			if(this.color().equals(Card.Color.CLUBS) || this.color().equals(Card.Color.SPADES))
				return true;
		return false;
	}
	
	/**
	 * [Porovna hodnotu karty se zadanou kartou c. Pokud jsou stejne, vraci 0. Pokud je karta vetsi nez zadana c, vraci kladny rozdil hodnot.]
	 * @param  c [Karta, s kterou se porovnava.]
	 * @return   [Vraci rozdil hodnot karet.]
	 */
	public int compareValue(Card c)
	{
		return this.value() - c.value();
	}
	
	/**
	 * [Porovnava barvu dvou karet.]
	 * @param  c [Porovnavana karta.]
	 * @return   [Vraci true pokud jsou karty stejne.]
	 */
	public boolean color_compare(Card c)
	{
		if(this.color().equals(Card.Color.HEARTS) && c.color().equals(Card.Color.HEARTS))
			return true;
		if(this.color().equals(Card.Color.CLUBS) && c.color().equals(Card.Color.CLUBS))
			return true;
		if(this.color().equals(Card.Color.DIAMONDS) && c.color().equals(Card.Color.DIAMONDS))
			return true;
		if(this.color().equals(Card.Color.SPADES) && c.color().equals(Card.Color.SPADES))
			return true;
		return false;
	}
}

