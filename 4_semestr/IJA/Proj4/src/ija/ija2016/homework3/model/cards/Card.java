/*
 * Card: Trida reprezentujici kartu
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import java.util.Objects;

public class Card implements CardInterface 
{
	private final Card.Color barva;
	private final int value;
	private boolean isturned;
	
	/**
	 * [konstruktor pro vytvoreni karty]
	 * @param  c	 [barva karty]
	 * @param  value [hodnota karty]
	 */
	public Card(Card.Color c, int value) 
	{
		this.isturned = false;
		this.barva = c;
		this.value = value;
	}

	/**
	 * [otoci kartu licem dolu, pokud tak uz je, nedela nic]
	 * @return [informace, zda doslo k otoceni karty (=true) nebo ne]
	 */
	public boolean turnFaceDown() 
	{
		if(!this.isturned) 
		{
			return false;
		}
		this.isturned = false;
		return true;
	}
		
	/**
	 * vyctovy typ reprezentujici barvu karty
	 */
	public static enum Color 
	{
		SPADES("S"), DIAMONDS("D"), HEARTS("H"), CLUBS("C");
			
		private String type;

		/**
		 * [kontruktor]
		 * @param  typ [typ barvy]
		 */
		private Color(String typ) 
		{
			this.type = typ;
		}
		
		/**
		 * [barva do strigu]
		 * @return [pismeno reprezentujici barvu]
		 */
		public String toString() 
		{
			return this.type;
		}
	}
	
	/**
	 * [metoda vraci hodnotu karty]
	 * @return [hodnota karty]
	 */
	public int value() 
	{
		return this.value;
	}
	  
	/**
	 * [testuje, zda je karta otocena licem nahoru]
	 * @return [vysledek testu, true = karta je otocena licem nahoru]
	 */
	public boolean isTurnedFaceUp() 
	{
		return this.isturned;
	}
	  
	/**
	 * [otoci kartu licem nahoru, pokud tak uz je, nedela nic]
	 * @return [informace, zda doslo k otoceni karty (=true) nebo ne]
	 */
	public boolean turnFaceUp() 
	{
		if(this.isturned == false) 
		{
			this.isturned = true;
			return true;
		}
		return false;
	}
	  
	/**
	 * [metoda testuje barvu karty]
	 * @return [barva karty]
	 */
	public Card.Color color() 
	{
		return this.barva;
	}
	  
	/**
	 * [testuje, zda ma karta podobnou barvu jako karta zadana, podobnou barvou se mysli cerna (piky, kule) a cervena (kary a srdce)]
	 * @param  c [karta pro srovnani]
	 * @return   [informace o shode barev karet]
	 */
	public boolean similarColorTo(Card c) 
	{
		if(((c.color() == Card.Color.CLUBS) || (c.color() == Card.Color.SPADES)) && ((this.color() == Card.Color.CLUBS) || (this.color() == Card.Color.SPADES))) 
		{
			return true;
		}
		else if(((c.color() == Card.Color.DIAMONDS) || (c.color() == Card.Color.HEARTS)) && ((this.color() == Card.Color.DIAMONDS) || (this.color() == Card.Color.HEARTS))) 
		{
			return true;
		}
		return false;
	}

	/**
	 * [porovna hodnotu karty se zadanou kartou c, pokud jsou stejne, vraci 0, pokud je karta vetsi nez zadana c, vraci kladny rozdil hodnot]
	 * @param  c [karta s kterou se porovnava]
	 * @return   [vraci rozdil hodnot karet]
	 */
	public int compareValue(Card c) 
	{
		if(c.value == this.value) 
		{
			return 0;
		}
		else if(this.value > c.value) 
		{
			int difference =  this.value - c.value;
			return difference;
		}
		else 
		{
			int difference =  this.value - c.value;
			return difference;
		}
	}

	/**
	 * [prepsani metody toString, pro korektni vypis na vystup]
	 * @return [vraci string ve formatu [hodnota karty]([barva karty])]
	 */
	public String toString() 
	{
		String convert = "1";
			
		if(this.value > 1 && this.value < 11) 
		{
			convert = Integer.toString(this.value);
		}
		else 
		{
			switch(value) 
			{
				case 1: convert = "A";
				break;
				case 11: convert = "J";
				break;
				case 12: convert = "D";
				break;
				case 13: convert = "K";
				break;  
			}
		}
		return (convert + "(" + this.barva.toString() + ")"); 
	}
	
	/**
	 * [prepsani metody equals, pro korektni kontrolu podobnosti dvou karet]
	 * @param  obj [objekt pro porovnani]
	 * @return	 [true = je podobny]
	 */
	public boolean equals(Object obj) 
	{
		if(obj == this) return true;
		if(!(obj instanceof Card)) 
		{
			return false;
		}
		Card card = (Card)obj;
		return value == card.value && Objects.equals(barva, card.barva);
	}

	/**
	 * [prepsani metody hashCode, nutnost pri prepsani equals]
	 * @return [true = hash hodnota]
	 */
	public int hashCode() 
	{
		return Objects.hash(value, barva);
	}
}
