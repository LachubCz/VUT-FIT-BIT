/*
 * CardHint: Trida, ktera obsahuje pole balicku, ktere byli vyhozene napovedou jako potencialni pro vkladani
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import java.util.ArrayList;

public class CardHint 
{
	private ArrayList<CardDeck> decks  = new ArrayList<>();
	private ArrayList<CardStack> stacks = new ArrayList<>();
	
	/**
	 * [metoda vraci deck s indexem i]
	 * @param  i [index balicku]
	 * @return   [balicek]
	 */
	public CardDeck getCardDeck(int i) 
	{
		return this.decks.get(i);
	}

	/**
	 * [metoda vraci stack s indexem i]
	 * @param  i [index balicku]
	 * @return   [balicek]
	 */
	public CardStack getCardStack(int i) 
	{
		return this.stacks.get(i);
	}
	
	/**
	 * [metoda vrati vsechny decky]
	 * @return [pole balicku]
	 */
	public ArrayList<CardDeck> getCardDecks() 
	{
		return this.decks;
	}

	/**
	 * [metoda vrati vsechny stacky]
	 * @return [pole balicku]
	 */
	public ArrayList<CardStack> getCardStacks() 
	{
		return this.stacks;
	}
	
	/**
	 * [metoda prida stack do pole]
	 * @param  stack [vkladany balicek]
	 * @return	   [pokud se operace zdarila vraci true]
	 */
	public boolean add(CardStack stack) 
	{
		if (this.stacks.add(stack))
			return true;
		return false;
	}
	
	/**
	 * [metoda prida deck do pole]
	 * @param  deck [vkladany balicek]
	 * @return	   [pokud se operace zdarila vraci true]
	 */
	public boolean add(CardDeck deck) 
	{
		if (this.decks.add(deck))
			return true;
		return false;
	}
}
