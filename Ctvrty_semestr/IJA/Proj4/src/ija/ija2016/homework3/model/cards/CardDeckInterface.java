/*
 * CardDeckInterface: Rozhrani reprezentujici balicek karet
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import java.io.Serializable;

public interface CardDeckInterface extends Serializable{
	
	/**
	 * [vrati kartu z vrcholu zasobniku (karta zustava na zasobnkiku), pokud je balicek prazdny, vraci null]
	 * @return [karta z vrcholu balicku]
	 */
	public abstract Card get();
	  
	/**
	 * [vrati kartu na uvedenem indexu, spodni karta je na indexu 0, vrchol je na indexu size() -1, pokud je balicek prazdny, nebo index mimo rozsah, vraci null]
	 * @param  i [pozice karty v balicku]
	 * @return	   [karta na uvedenem indexu]
	 */
	public abstract Card get(int i);
	  
	/**
	 * [metoda testuje zdali je balicek prazdny]
	 * @return [vraci true, pokud je balicek prazdny]
	 */
	public abstract boolean isEmpty();
	  
	/**
	 * [odebere kartu z vrcholu balicku, pokud je balicek prazdny, vraci null]
	 * @return [vraci kartu z vrcholu balicku]
	 */
	public abstract Card pop();
	  
	/**
	 * [vlozi kartu na vrchol balicku]
	 * @param  c [vkladana karta]
	 * @return	  [uspesnost akce]
	 */
	public abstract boolean put(Card c);
	  
	/**
	 * [metoda vraci velikost balicku]
	 * @return [velikost balicku]
	 */
	public abstract int size();
}
