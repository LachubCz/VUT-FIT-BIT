/*
 * CardInterface: Rozhrani reprezentujici kartu
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import java.io.Serializable;

public interface CardInterface extends Serializable
{
	/**
	 * [metoda vraci hodnotu karty]
	 * @return [hodnota karty]
	 */
	public abstract int value();
	  
	/**
	 * [testuje, zda je karta otocena licem nahoru]
	 * @return [vysledek testu, true = karta je otocena licem nahoru]
	 */
	public abstract boolean isTurnedFaceUp();
	  
	/**
	 * Otoci kartu licem nahoru. Pokud tak uz je, nedela nic.
	 * @return Informace, zda doslo k otoceni karty (=true) nebo ne
	 */
	public abstract boolean turnFaceUp();
	  
	/**
	 * [metoda testuje barvu karty]
	 * @return [barva karty]
	 */
	public abstract Card.Color color();
	  
	/**
	 * [testuje, zda ma karta podobnou barvu jako karta zadana, podobnou barvou se mysli cerna (piky, kule) a cervena (kary a srdce)]
	 * @param  c [karta pro srovnani]
	 * @return   [informace o shode barev karet]
	 */
	public abstract boolean similarColorTo(Card c);
	  
	/**
	 * [porovna hodnotu karty se zadanou kartou c, pokud jsou stejne, vraci 0, pokud je karta vetsi nez zadana c, vraci kladny rozdil hodnot]
	 * @param  c [karta s kterou se porovnava]
	 * @return   [vraci rozdil hodnot karet]
	 */
	public abstract int compareValue(Card c);
}
