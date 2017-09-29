/*
 * AbstractFactorySolitaire: Abstraktni trida obsahujici metody pro tvorbu jednotlivych balicku
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.board;

import ija.ija2016.homework3.model.cards.Card;
import ija.ija2016.homework3.model.cards.CardDeck;
import ija.ija2016.homework3.model.cards.CardStack;

public abstract class AbstractFactorySolitaire 
{
	/**
	 * [vytvori objekt reprezentujici balicek karet]
	 * @return [balicek karet]
	 */
	public abstract CardDeck createCardDeck();
	
	/**
	 * [vytvori objekt reprezentujici kartu]
	 * @param  color [barva karty]
	 * @param  value [hodnota karty]
	 * @return	   [vraci kartu, pokud je nektery z parametru neplatny (objekt nelze vytvorit), vraci null]
	 */
	public abstract Card createCard(Card.Color color, int value);
	
	/**
	 * [vytvari objekt reprezentujici cilovy balicek, cilem hrace je vlozit vsechny karty zadane barvy do ciloveho balicku]
	 * @param  color [zadana barva]
	 * @return	   [balicek]
	 */
	public abstract CardDeck createTargetPack(Card.Color color);
	
	/**
	 * [vytvori zdrojovy balicek]
	 * @return [vrati vytvoreny zdrojovy balicek]
	 */
	public abstract CardStack createSourcePack();
	
	/**
	 * [vytvori zdrojovy balicek a do jeho decku prida karty]
	 * @param  deck [karty ktere se pridaji do decku ve zdrojovem balicku]
	 * @return	  [vrati vytvoreny zdrojovy balicek]
	 */
	public abstract CardStack createSourcePack(CardDeck deck);
	
	/**
	 * [vytvari objekt reprezentujici pracovni pole pro karty]
	 * @return [pracovni pole]
	 */
	public abstract CardStack createWorkingPack();
	
	/**
	 * [vytvari objekt reprezentujici pracovni pole pro karty a do jeho decku prida karty]
	 * @param  deck [karty na pridani]
	 * @return	  [pracovni pole]
	 */
	public abstract CardStack createWorkingPack(CardDeck deck);
}
