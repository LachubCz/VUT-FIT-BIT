package ija.ija2016.homework2.model.cards;

/**
 * Rozhrani Card
 */
public interface Card 
{	

	/**
	 * Spides (piky). Textova reprezentace barvy je "S".
	 * Diamonds (kary). Textova reprezentace barvy je "D".
	 * Hearts (srdce). Textova reprezentace barvy je "H".
	 * Clubs (krize). Textova reprezentace barvy je "C".
	 */
	public enum Color 
	{
		CLUBS("C"), DIAMONDS("D"), HEARTS("H"), SPADES("S");
	    
		private final String name;       
	
	    private Color(String s) {
	        name = s;
	    }
	    public String toString() {
	        return this.name;
	     }
	}

	/**
	 * [Vraci hodnotu karty.]
	 * @return [Hodnota karty.]
	 */
	public int value();
	
	/**
	 * [Testuje, zda je karta otocena licem nahoru.]
	 * @return [Vysledek testu: true = karta je otocena licem nahoru.]
	 */
	public boolean isTurnedFaceUp();
	
	/**
	 * [Otoci kartu licem nahoru. Pokud tak uz je, nedela nic.]
	 * @return [Informace, zda doslo k otoceni karty (=true) nebo ne.]
	 */
	public boolean turnFaceUp();
	
	/**
	 * [Vrati barvu karty.]
	 * @return [Barva karty.]
	 */
	public Card.Color color();
	
	/**
	 * [Testuje, zda ma karta podobnou barvu jako karta zadana. Podobnou barvou se mysli cerna (piky, krize) a cervena (kary a srdce).]
	 * @param  c [Karta pro porovnani.]
	 * @return   [Informace o shode barev karet.]
	 */
	public boolean similarColorTo(Card c);
	
	/**
	 * [Porovna hodnotu karty se zadanou kartou c. Pokud jsou stejne, vraci 0. Pokud je karta vetsi nez zadana c, vraci kladny rozdil hodnot.]
	 * @param  c [Karta, s kterou se porovnava.]
	 * @return   [Vraci rozdil hodnot karet.]
	 */
	public int compareValue(Card c);
}
