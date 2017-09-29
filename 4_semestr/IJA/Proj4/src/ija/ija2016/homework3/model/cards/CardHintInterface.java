/*
 * CardHintInterface: Rozhrani pro tridu, ktera reprezentuje pole balicku, ktere byli vyhozene napovedou jako potencialni pro vkladani
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import java.util.ArrayList;

public interface CardHintInterface 
{
	/**
	 * [metoda vrati vsechny decky]
	 * @return [pole balicku]
	 */
	public ArrayList<CardDeck> getCardDecks();
	
	/**
	 * [metoda vrati vsechny stacky]
	 * @return [pole balicku]
	 */
	public ArrayList<CardStack> getCardStacks();
}
