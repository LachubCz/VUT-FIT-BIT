/*
 * CardStack: Trida reprezentujici balicek karet
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import java.util.ArrayList;
import java.util.Objects;

public class CardStack implements CardStackInterface 
{
	private CardDeck source = new CardDeck(); 
	private ArrayList<Card> karty;
	private int capacity;
	private Card Cardcolor;
	
	/**
	 * [konstruktor]
	 */
	public CardStack() 
	{
		this.capacity = 52;
		this.Cardcolor = null;
		this.karty = new ArrayList<Card>(52);
	}
	
	/**
	 * [konstruktor]
	 * @param  size [velikost vytvareneho balicku]
	 */
	public CardStack(int size) 
	{
		this.capacity = size;
		this.Cardcolor = null;
		this.karty = new ArrayList<Card>(size);
	}

	/**
	 * [metoda odebere z vrcholu balicku karty a zahodi je]
	 * @param pop [karta od ktere se karty zahazuji]
	 */
	public void emplaceStack(Card pop) 
	{
		this.karty.add(pop);
	}
	
	/**
	 * [metoda vlozi kartu na vrchol bez kontroly podminek]
	 * @param card [vkladana karta]
	 */
	public void InitPut(Card card)	  
	{	   
		this.Cardcolor = new Card(card.color(), 13);		
		this.karty.add(card);	   
	}
	
	/**
	 * [metoda pro vytvoreni pracujiciho pole o velikosti 19]
	 * @return [pracovni pole]
	 */
	public static CardStack createWorkingPack() 
	{
		CardStack stack = new CardStack(19);
		return stack;
	}
	
	/**
	 * [metoda vklada kartu na vrchol balicku]
	 * @param  card [vkladana karta]
	 * @return	  [pokud se karta vlozila vraci true]
	 */
	public boolean put(Card card) 
	{
		if((this.isEmpty()) && (card.value() == 13)) 
		{
			this.Cardcolor = new Card(card.color(), 13);
			this.karty.add(card);
			return true;
		}
		if (!this.isEmpty())
		{
			if((this.CanPut() == card.value()) && (this.Cardcolor != null)) 
			{
				 if(!(card.similarColorTo(this.Cardcolor))) 
				 {
					this.Cardcolor = new Card(card.color(), 13);
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
		else 
		{
			return false;
		}
	}
	
	/**
	 * [metoda vraci hodnotu karty ktera muze byt vlozena]
	 * @return [hodnota mozne vkladane karty]
	 */
	public int CanPutStack()
	{
		Card card = (Card)karty.get(karty.size() - 1);
		return (card.value() - 1); 
	}
	
	/**
	 * [metoda kontroluje zdali muze byt na balicek vlozena karta]
	 * @param  card [vkladana karta]
	 * @return	  [pokud muze byt karta vlozena vraci true]
	 */
	public boolean canPutCard(Card card) 
	{
		if((this.isEmpty()) && (card.value() == 13)) 
		{
				return true;
		}
		if (!this.isEmpty())
		{
			if((this.CanPutStack() == card.value()) && (this.Cardcolor != null)) 
			{
				 if(!(card.similarColorTo(this.Cardcolor))) 
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
		else 
		{
			return false;
		}
	}
	
	/**
	 * [metoda ziska kartu z hlavniho balicku o danem indexu]
	 * @param  index [index karty]
	 * @return	   [chtena karta]
	 */
	public Card getFromStack(int index)
	{
		if (!this.karty.isEmpty())
			return this.karty.get(index);
		else
			return null;
	}
   
	/**
	* [vlozi na vrchol balicek karet s kontrolou podminek]
	 * @param  stack [vkladany balicek karet]
	 * @return	   [pokud probehlo vlozeni uspesne vraci true]
	*/
	public boolean put(CardStack stack) 
	{
		if((stack == null) || (stack.isEmpty() == true)) 
		{
			return false;
		}
		karty.addAll(stack.karty);
		stack.karty.clear();
		return true;
	}

	/**
	 * [vlozi na vrchol balicek karet bez kontroly podminek]
	 * @param  stack [vkladany balicek karet]
	 * @return	   [vzdy vraci true]
	 */
	public boolean InitPut(CardStack stack) 
	{
		karty.addAll(stack.karty);
		stack.karty.clear();
		return true;
	}
	
	/**
	 * [metoda vraci kartu z vrcholu hlavniho balicku]
	 * @return [karta z vrcholu]
	 */
	public Card topStack() 
	{
		if(!this.karty.isEmpty())
			return this.karty.get(this.karty.size()-1);
		return null;
	}
	
   /**
	* [test zdali je balicek prazdny]
	* @return [pokud je prazdny vraci true]
	*/
	public boolean isEmpty() 
	{
		return this.karty.isEmpty();
	}
	
	/**
	 * [metoda vraci velikost balicku]
	 * @return [velikost balicku]
	 */
	public int size() 
	{
		return this.karty.size();
	}
	
	/**
	 * [vraci kartu z hlavniho balicku o danem indexu]
	 * @param  index [index karty]
	 * @return	   [chtena karta]
	 */
	public Card getStack(int index) 
	{
		return this.karty.get(index);
	}
	
	/**
	 * [metoda zjistuje jakou hodnotu musi mit karta aby sla polozit]
	 * @return [hodnota pokladane karty]
	 */
	public int CanPut()
	{
		Card card = (Card)karty.get(karty.size() - 1);
		return (card.value() - 1); 
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
		if(this.isEmpty())	  
		{	   
			return Card;		
		}	   
		else		
		{	   
			Card Temp = (Card)karty.remove(karty.size() - 1);	   
			Temp.turnFaceUp();	  
			this.InitPut(Temp);	 
		}	   
		return Card;
	}
	
	/**
	 * [metoda odebere ze zasobniku sekvenci karet od zadane karty az po vrchol zasobniku, pokud je hledana karta na vrcholu, bude v sekvenci pouze jedna karta]
	 * @param  card [hledana karta]
	 * @return	  [zasobnik karet obsahujici odebranou sekvenci, pokud hledana karta v zasobniku neni, vraci null]
	 */
	public CardStack pop(Card card) 
	{
		int lastindex = this.karty.lastIndexOf(card);
		if(lastindex == -1) 
		{
			return null;
		}
		CardStack pomocny = new CardStack(this.size());
		if(lastindex == (this.karty.size()-1)) 
		{
			pomocny.karty.add(this.karty.get(this.size()-1));
		}
		else 
		{
			pomocny.karty.addAll((this.karty.subList(lastindex, this.size())));
		}
		if(lastindex == (this.karty.size()-1)) 
		{
			this.karty.remove(lastindex);
		}
		else 
		{
			this.karty.subList(lastindex, this.karty.size()).clear();
		}
		return pomocny;
	}
	
	/**
	 * [add description]
	 * @param deck  [description]
	 * @param count [description]
	 */
	public void add(CardDeck deck, int count) 
	{
		if(count > this.capacity - this.size()) 
		{
			throw new IllegalArgumentException("Not enough space in the stack");
		}
		if(count > deck.size()) 
		{
			throw new IllegalArgumentException("Deck does not have enough cards");
		}
		for(;count > 0; count--) 
		{
			this.karty.add(deck.pop());
		}
	}
	
	/**
	 * [prepise metodu equals, pro korektni kontrolu podobnosti objektu]
	 * @param  obj [objekt]
	 * @return	 [pokud jsou objekty podobne vraci true]
	 */
	public boolean equals(Object obj) 
	{
		if(obj == this) 
			return true;
		if(!(obj instanceof CardStack)) 
		{
			return false;
		}
		CardStack stack = (CardStack)obj;
		if(this.karty.isEmpty() != stack.karty.isEmpty()) 
		{
			return false; 
		}
		if(this.karty.size() != stack.karty.size()) 
		{
			return false;
		}
		if(this.capacity != stack.capacity) 
		{
			return false;
		}
		if(!(stack.karty.containsAll(this.karty))) 
		{
			return false;
		}
		return true;
	}
	
	/**
	 * [prepsani metody hashcode, nutnost pri prepsani equals]
	 * @return [true = hash hodnota]
	 */
	public int hashCode() 
	{
		return Objects.hash(karty, capacity);
	}

	/**
	 * [metoda presune kartu z vrcholu hlavniho balicku na source balicek]
	 * @return [pokud metoda probehla uspesne vraci true]
	 */
	public boolean showNext() 
	{
		if(this.size() > 0) 
		{
			Card card = this.karty.remove(this.size() - 1);
			card.turnFaceUp();
			return this.source.karty.add(card);
		}
		return false;
	}

	/**
	 * [metoda otoci v source balicku vrchni kartu licem dolu]
	 * @return [pokud metoda probehla uspesne vraci true]
	 */
	public boolean hideTopCard() 
	{
		if(!this.source.karty.isEmpty()) 
		{
			Card card = this.source.pop();
			card.turnFaceDown();
			return this.karty.add(card);
		}
		return false;
	}

	/**
	 * [metoda presune vsechny karty ze source balicku do hlavniho balicku]
	 * @return [pokud metoda probehla uspesne vraci true]
	 */
	public boolean MoveBackToStack() 
	{
		if(this.sizeOfStack() == 0) 
		{
			for(int i = this.source.size() - 1; i >= 0; i--) 
			{
				this.karty.add(this.source.karty.remove(i));
				this.karty.get(this.sizeOfStack() - 1).turnFaceDown();
			}
		}
		return false;
	}
	
	/**
	 * [metoda vraci velikost balicku]
	 * @return [velikost balicku]
	 */
	public int sizeOfStack() 
	{
		return this.size();
	}
	
	/**
	 * [metoda vraci source balicek]
	 * @return [source balicek]
	 */
	public CardDeck getDeck() 
	{
		return this.source;
	}
	
	/**
	 * [metoda zjisti zdali je source balicek prazdny]
	 * @return [pokud je source balicek prazdny vraci true]
	 */
	public boolean isSourceEmpty() 
	{
		return this.source.isEmpty();
	}

	/**
	 * [metoda vrati kartu daneho indexu ze source balicku]
	 * @param  index [index]
	 * @return	   [karta o danem indexu]
	 */
	public Card get(int index)
	{
		return this.source.get(index);
	}

	/**
	 * [metoda odebere kartu ze source balicku]
	 * @param remove [odebirana karta]
	 */
	public void emplace(Card remove) 
	{
		this.source.emplace(remove);
	}

	/**
	 * [metoda vrati kartu z vrcholu source balicku]
	 * @return [karta z vrcholu source balicku]
	 */
	public Card top() 
	{
		return this.source.top();
	}

	/**
	 * [metoda zjisti zdali jde karta polozit na source balicek]
	 * @param  card [pokladana karta]
	 * @return	  [pokud lze polozit vraci true]
	 */
	public boolean canPut(Card card) 
	{
		return this.source.canPut(card);
	}

	/**
	 * [metoda zjisti zdali source balicek obsahuje danou kartu]
	 * @param  card [hledana karta]
	 * @return	  [pokud source balicek kartu obsahuje vraci true]
	 */
	public boolean contains(Card card) 
	{
		return this.source.contains(card);
	}
}
