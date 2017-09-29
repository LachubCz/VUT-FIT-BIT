/*
 * CardBoardInterface: Rozhrani reprezentujici hraci desku
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

public interface CardBoardInterface 
{
	/**
	 * [metoda vrati zdrojovy balicek]
	 * @return [zdrojovy balicek]
	 */
	public CardStack getSourcePack();
		
	/**
	 * [metoda vrati cilovy balicek]
	 * @param  index [cislo ciloveho balicku]
	 * @return	   [cilovy balicek]
	 */
	public CardDeck getDeck(int index);
		
	/**
	 * [metoda vrati pracovni balicek]
	 * @param  index [cislo pracovniho balicku]
	 * @return	   [pracovni balicek]
	 */
	public  CardStack getStack(int index);

	/**
	 * [metoda zjistuje zda-li nastal konec hry]
	 * @return [pokud nastal konec hry vraci true]
	 */
	public boolean isGameOver();

	/**
	 * [pomocna metoda pro createHint - najde balicky, kam muze byt karta polozena]
         * @param toCard  [karta pro kterou se ziskava hint]
	 * @return	  [instance tridy CardHint ve ktere jsou ulozeny balicky na ktere se da karta polozit]
	 */
	public CardHint hintForCard(Card toCard);

	/**
	 * [metoda vytvori napovedu pro vybranou kartu]
	 * @param  toCard [karta pro napovedu]
	 * @return	  [cislo ciloveho (0,1,2,3) / pracovniho (10,11,12,13,14,15,16) balicku]
	 */
	public int createHint(Card toCard);
	  
	/**
	 * [metoda aktualizuje zobrazovanou plochu]
	 */
	public void update();
	 
	/**
	 * [metoda nacte ulozenou instanci cardboard]
	 * @param  filename [jmeno ulozene hry]
	 * @return		  [pokud se nacteni povedlo vraci true]
	 */
	public boolean LoadGame (String filename);
	   
	/**
	 * [metoda ulozi aktualni instanci cardboard]
	 * @param  filename [jmeno ukladane hry]
	 * @return		  [pokud se ulozeni povedlo vraci true]
	 */ 
	public boolean SaveGame (String filename);
}
