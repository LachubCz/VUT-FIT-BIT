/*
 * CardStackInterface: Rozhrani reprezentujici balicek karet
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import java.io.Serializable;

public interface CardStackInterface extends Serializable, CanAddInterface
{	
	/**
	 * [vlozi na vrchol balicek karet s kontrolou podminek]
	 * @param  stack [vkladany balicek karet]
	 * @return	   [pokud probehlo vlozeni uspesne vraci true]
	 */
	public abstract boolean put(CardStack stack);
	
	/**
	 * [metoda odebere ze zasobniku sekvenci karet od zadane karty az po vrchol zasobniku, pokud je hledana karta na vrcholu, bude v sekvenci pouze jedna karta]
	 * @param  card [hledana karta]
	 * @return	  [zasobnik karet obsahujici odebranou sekvenci, pokud hledana karta v zasobniku neni, vraci null]
	 */
	public abstract CardStack pop(Card card);

	/**
	 * [add description]
	 * @param deck          [description]
	 * @param cardsToInsert [description]
	 */
	public abstract void add(CardDeck deck, int cardsToInsert);
}
