/*
 * FactoryKlondike: Trida implementujici metody pro tvorbu konkretnich balicku pro hru
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

public class FactoryKlondike extends AbstractFactorySolitaire 
{
	/**
	 * [vytvori objekt reprezentujici kartu]
	 * @param  color [barva karty]
	 * @param  value [hodnota karty]
	 * @return	   [vraci kartu, pokud je nektery z parametru neplatny (objekt nelze vytvorit), vraci null]
	 */
	public Card createCard(Card.Color color, int value) 
	{
		if(value > 13 || value < 1) 
		{
			return null;
		}
		else 
		{
			Card card = new Card(color, value);
			return card;
		}
	}
	
	/**
	 * [vytvori objekt reprezentujici balicek karet]
	 * @return [balicek karet]
	 */
	public CardDeck createCardDeck() 
	{
		CardDeck deck = CardDeck.createStandardDeck();
		return deck;
	}

	/**
	 * [vytvari objekt reprezentujici cilovy balicek, cilem hrace je vlozit vsechny karty zadane barvy do ciloveho balicku]
	 * @param  color [zadana barva]
	 * @return	   [balicek]
	 */
	public CardDeck createTargetPack(Card.Color color) 
	{
		CardDeck deck = CardDeck.createTargetPack(color);
		return deck;
	}
		
 	/**
	 * [vytvori zdrojovy balicek]
	 * @return [vrati vytvoreny zdrojovy balicek]
	 */
	public CardStack createSourcePack() 
	{
		CardStack stack = new CardStack();
		return stack;
	}
	
	/**
	 * [vytvori zdrojovy balicek a do jeho decku prida karty]
	 * @param  deck [karty ktere se pridaji do decku ve zdrojovem balicku]
	 * @return	  [vrati vytvoreny zdrojovy balicek]
	 */
	public CardStack createSourcePack(CardDeck deck) 
	{
		CardStack stack = new CardStack();
		stack.add(deck, deck.size());
		return stack;
	}

	/**
	 * [vytvari objekt reprezentujici pracovni pole pro karty]
	 * @return [pracovni pole]
	 */
	public CardStack createWorkingPack() 
	{
		CardStack stack = CardStack.createWorkingPack();
		return stack;
	}

	/**
	 * [vytvari objekt reprezentujici pracovni pole pro karty a do jeho decku prida karty]
	 * @param  deck [karty na pridani]
	 * @return	  [pracovni pole]
	 */
	public CardStack createWorkingPack(CardDeck deck) 
	{
		CardStack stack = CardStack.createWorkingPack();
		stack.add(deck, deck.size());
		return stack;
	}
}
