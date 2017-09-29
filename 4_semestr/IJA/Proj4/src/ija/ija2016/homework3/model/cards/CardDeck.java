/*
 * CardDeck: Trida reprezentujici balicek karet
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

public class CardDeck implements CardDeckInterface 
{
	public ArrayList<Card> karty;
	private Card.Color color;
  
	/**
	 * [konstruktor]
	 * @param  color [barva balicku]
	 */
	public CardDeck(Card.Color color) 
	{
		this.color = color;
		this.karty = new ArrayList<Card>(13);
	}
		
	/**
	 * [konstruktor]
	 * @param  size [velikost balicku]
	 */
	public CardDeck(int size) 
	{
		if (size % 4 != 0) 
		{
			throw new IllegalArgumentException("Size has to be divisible by 4");
		}
		int numberofeachcard = size / 4;
		Set<Card> cardDeck = new HashSet<Card>();
		for (int i = 1; i <= numberofeachcard; i++) cardDeck.add(new Card(Card.Color.CLUBS, i));
		for (int i = 1; i <= numberofeachcard; i++) cardDeck.add(new Card(Card.Color.DIAMONDS, i));
		for (int i = 1; i <= numberofeachcard; i++) cardDeck.add(new Card(Card.Color.HEARTS, i));
		for (int i = 1; i <= numberofeachcard; i++) cardDeck.add(new Card(Card.Color.SPADES, i));
		this.color = null;
		this.karty = new ArrayList<Card>(size);
		this.karty.addAll(cardDeck);
	}
	
	/**
	 * [konstruktor vytvarejici balicek o 52 kartach]
	 */
	public CardDeck() 
	{
		this.karty = new ArrayList<Card>(52);
	}
	
	/**
	 * [metoda ziska barvu balicku]
	 * @return [barva]
	 */
	public Card.Color getDeckColor() 
	{
		return this.color;
	}
  
	/**
	 * [metoda vytvarejici standartni balicek karet]
	 * @return [balicek o velikosti 52 karet, kazda 13 karet]
	 */
	public static CardDeck createStandardDeck() 
	{
		CardDeck balicek = new CardDeck(52);
		balicek.color = null;
		return balicek;
	}
	
	/**
	 * [metoda vytvari cilovy balicek]
	 * @param  color [cilova barva]
	 * @return	   [cilovy balicek]
	 */
	public static CardDeck createTargetPack(Card.Color color) 
	{
		CardDeck balicek = new CardDeck(color);
		return balicek;
	}
  
	/**
	 * [metoda vraci velikost balicku]
	 * @return [velikost balicku]
	 */
	public int size() 
	{
		return karty.size();
	}
  
	/**
	 * [vlozi kartu na vrchol balicku]
	 * @param  card [vkladana karta]
	 * @return	  [uspesnost akce]
	 */
	public boolean put(Card card) 
	{
		if(this.color != null) 
		{
			if(this.karty.isEmpty()) 
			{
				if((card.color() == this.color) && (card.value() == 1)) 
				{
					this.karty.add(card);
					return true;
				}
				else 
				{
					return false;
				}
			}
			else if(((this.size()+1) == card.value()) && ((this.color == card.color()))) 
			{
				this.karty.add(card);
				return true;
			}
			else 
			{
				return false;
			}
		}
		else 
		{
			return false;
		}
	}
	
	/**
	 * [metoda zjisti zda jde vlozit karta]
	 * @param  card [vkladana karta]
	 * @return	  [vraci true pokud jde vlozit]
	 */
	public boolean canPutCard(Card card) 
	{
		if(this.color != null) 
		{
			if(this.karty.isEmpty()) 
			{
				if((card.color() == this.color) && (card.value() == 1)) 
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else if(((this.size()+1) == card.value()) && ((this.color == card.color()))) 
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		else 
		{
			return false;
		}
	}
	
	/**
	 * [odebere kartu z vrcholu balicku, pokud je balicek prazdny, vraci null]
	 * @return [vraci kartu z vrcholu balicku]
	 */
	public Card pop() 
	{
		if(this.isEmpty()) 
		{
			return null;
		}
		Card Card = (Card)karty.remove(karty.size() - 1);
		return Card;
	}
  
	/**
	 * [vrati kartu z vrcholu zasobniku (karta zustava na zasobnkiku), pokud je balicek prazdny, vraci null]
	 * @return [karta z vrcholu balicku]
	 */
	public Card get() 
	{
		if(this.karty.isEmpty()) 
		{
			return null;
		}
		Card card = this.karty.get(this.karty.size()-1);
		return card;
	}
	
	/**
	 * [vrati kartu na uvedenem indexu, spodni karta je na indexu 0, vrchol je na indexu size() -1, pokud je balicek prazdny, nebo index mimo rozsah, vraci null]
	 * @param  index [pozice karty v balicku]
	 * @return	   [karta na uvedenem indexu]
	 */
	public Card get(int index) 
	{
		if((this.karty.isEmpty()) || (index > (this.karty.size()-1)) || (index < 0)) 
		{
			return null;
		}
		Card card = this.karty.get(index);
		return card;
	}
  
	/**
	 * [metoda testuje zdali je balicek prazdny]
	 * @return [vraci true, pokud je balicek prazdny]
	 */
	public boolean isEmpty() 
	{
		return this.karty.isEmpty();
	}
	
	/**
	 * [vrati kartu na vrcholu balicku]
	 * @return [karta na vrcholu balicku]
	 */
	public Card top() 
	{
		return this.isEmpty() ? null : this.karty.get(this.size() - 1);
	}

	/**
	 * [metoda na kontrolu moznosti vlozeni karty(pouziva se pro cilove balicky)]
	 * @param  card [vkladana karta]
	 * @return	  [pokud lze vlozit vrati true]
	 */
	public boolean canPut(Card card) 
	{
		if(card == null) 
		{
			return false;
		}
		if(this.isEmpty()) 
		{
			return card.value() == 1 && card.color() == this.color;
		}
		return this.top().value() + 1 == card.value() && this.top().color() == card.color();
	}

	/**
	 * [odebrani karty z balicku, bez navratove hodnoty (pouziva se pro cilove balicky)]
	 * @param pop [odebirana karta]
	 */
	public void emplace(Card pop) 
	{
		this.karty.add(pop);
	}

	/**
	 * [kontrola zdali balicek obsahuje danou kartu (pouziva se pro cilove balicky)]
	 * @param  card [kontrolovana karta]
	 * @return	  [pokud ji obsahuje vraci true]
	 */
	boolean contains(Card card) 
	{
		return this.karty.contains(card);
	}
}
