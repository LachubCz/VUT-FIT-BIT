package ija.ija2016.homework2.model.cards;

/**
 * Rozhrani CardDeck
 */
public interface CardDeck 
{
	/**
	 * [Vrati aktualni pocet karet v balicku.]
	 * @return [Aktualni pocet karet v balicku.]
	 */
	public int size();
	
	/**
	 * [Vlozi kartu na vrchol balicku.]
	 * @param  card [Vkladana karta.]
	 * @return      [Uspesnost akce.]
	 */
	public boolean put(Card card);
	
	/**
	 * [Odebere kartu z vrcholu balicku. Pokud je balicek prazdny, vraci null.]
	 * @return [Karta z vrcholu balicku.]
	 */
	public Card pop();
	
	/**
	 * [Vrati kartu z vrcholu zasobniku (karta zustava na zasobniku). Pokud je balicek prazdny, vraci null.]
	 * @return [Karta z vrcholu balicku.]
	 */
	public Card get();
	
	/**
	 * [Vrati kartu na uvedenem indexu. Spodni karta je na indexu 0, vrchol je na indexu size()-1. Pokud je balicek prazdny, nebo index mimo rozsah, vraci null.]
	 * @param  index [Pozice karty v balicku.]
	 * @return       [Karta z vrcholu balicku.]
	 */
	public Card get(int index);
	
	/**
	 * [Test, zda je balicek karet prazdny.]
	 * @return [Vraci true, pokud je balicek prazdny.]
	 */
	public boolean isEmpty();
}
