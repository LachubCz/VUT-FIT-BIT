package ija.ija2016.homework2.model.cards;
import java.util.Arrays;

/**
 * Implementace rozhrani CardStack
 */
public class ICardStack implements CardStack 
{
	private int size;
	private boolean top;
	private Card [] stack;
	
	/**
	 * [Konstruktor ICardStack.]
	 * @param  size [Velikost ICardStack.]
	 * @return      [Void.]
	 */
	public ICardStack(int size) 
	{
		this.stack = new ICard[size];
		this.size = 0;
		this.top = true;
	}
	
	/**
	 * [Vlozi kartu na vrchol zasobniku.]
	 * @param  card [card - Vkladani karta.]
	 * @return      [Vrati true, pokud byla karta vlozena.]
	 */
	public boolean put(Card card) 
	{
		if (this.top == true)
		{
			if (card == null)
				return false;
			
			if (this.size() == 0)
			{
				if (card.value() == 13)
				{
					this.stack[this.size] = card;
					this.size+=1;
					return true;
				}
				else
					return false;
			}
			
			Card c = this.get();
			
			if (!c.similarColorTo(card))
			{
				if (c.compareValue(card) == 1)
				{
					this.stack[this.size] = card;
					this.size+=1;
					return true;
				}		
			}
			return false;
		}
		else
		{
			this.stack[this.size] = card;
			this.size+=1;
			return true;
		}
	}
	
	/**
	 * [Vlozi karty ze zasobniku stack na vrchol zasobniku. Karty vklada ve stejnem poradi, v jakem jsou uvedeny v zasobniku stack. Karta na vrcholu zasobniku vkladanych karet stack bude i na vrcholu tohoto zasobniku.]
	 * @param  pastestack [Zasobnik vkladanych karet.]
	 * @return            [Vrati true, pokud byl zasobnik vlozen.]
	 */
	public boolean put(CardStack pastestack) 
	{
		if (this.top == true)
		{
			for(int i = 0; i < pastestack.size(); i++)
			{
				if (this.put(pastestack.get(i)) == false)
					return false;
			}
			return true;
		}
		else 
			return false;
	}
	
	/**
	 * [Test, zda je zasobnik prazdny.]
	 * @return [Vraci true, pokud je zasobnik prazdny.]
	 */
	public boolean isEmpty() 
	{
		if(this.stack[0] == null)
			return true;
		else
			return false;
	}
	
	/**
	 * [Vrati aktualni pocet karet v zasobniku.]
	 * @return [Aktualni pocet karet v zasobniku.]
	 */
	public int size() 
	{
		return this.size;
	}
	
	/**
	 * [Metoda odebere ze zasobniku sekvenci karet od zadane karty az po vrchol zasobniku. Pokud je hledana karta na vrcholu, bude v sekvenci pouze jedna karta.]
	 * @param  card [Hledana karta.]
	 * @return      [Zasobnik karet obsahujici odebranou sekvenci. Pokud hledana karta v zasobniku neni, vraci null.]
	 */
	public CardStack takeFrom(ICard card) 
	{
		int i;
		int temp = this.size();

		for(i = 0; i < this.size(); i++)
		{
			if(this.stack[i].equals(card))
			{
				int stacksize = this.size - i ;
				CardStack stack = new ICardStack(stacksize);
				while(i < temp)
				{
					stack.put(this.get(i));
					this.size-=1;
					i+=1;
				}
				return stack;
			}
		}
		return null;
	}
	
	/**
	 * [Metoda pro porovnani dvou objektu typu cardstack.]
	 * @param  o [Porovnavany objekt.]
	 * @return   [Vraci true, pokud jsou objekty shodne.]
	 */
	public boolean equals(Object o)
	{
        if (o == this) return true;
        if (!(o instanceof ICardStack)) 
        {
            return false;
        }

        ICardStack stack1 = (ICardStack) o;
        return Arrays.equals(stack1.stack, stack) && stack1.size == size;
    }
	
	/**
	 * [Metoda hashCode pro objekt typu cardstack.]
	 * @return [Vraci zahashovany int z hashovaci metody.]
	 */
    public int hashCode() 
    {
        int result = 17;
        result = 31 * result + stack.hashCode();
        result = 31 * result + size;
        return result;
    }

	/**
	 * [Odebere kartu z vrcholu balicku. Pokud je balicek prazdny, vraci null.]
	 * @return [Karta z vrcholu balicku.]
	 */
	public Card pop() 
	{
		Card temp;
		if (this.size == 0)
			return null;
		else
		{
			this.size-=1;
			temp = this.stack[this.size];
			this.stack[this.size] = null;
			return temp;
		}
	}

	/**
	 * [Vrati kartu z vrcholu zasobniku (karta zustava na zasobniku). Pokud je balicek prazdny, vraci null.]
	 * @return [Karta z vrcholu balicku.]
	 */
	public Card get() 
	{
		if (this.size() == 0)
			return null;
		Card c = this.stack[(this.size() - 1)];
		return c; 
	}

	/**
	 * [Vrati kartu na uvedenem indexu. Spodni karta je na indexu 0, vrchol je na indexu size()-1. Pokud je balicek prazdny, nebo index mimo rozsah, vraci null.]
	 * @param  index [Pozice karty v balicku.]
	 * @return       [Karta z vrcholu balicku.]
	 */
	public Card get(int index) 
	{
		if (this.size() == 0)
			return null;
		Card c = this.stack[(index)];
		return c;
	}

	/**
	 * [Vyhleda kartu a odebere ji a karty pod ni z balicku.]
	 * @param  card [Hledana karta.]
	 * @return      [Vrati carstack obsahujici hledanou kartu a karty pod ni.]
	 */
	public CardStack pop(Card card) 
	{
		int i;
		int temp = this.size();

		for(i = 0; i < this.size(); i++)
		{
			if(this.stack[i].equals(card))
			{
				int stacksize = this.size - i;
				CardStack stack = new ICardStack(stacksize);
				while(i < temp)
				{
					((ICardStack)stack).changetop();
					stack.put(this.stack[i]);
					this.size-=1;
					i+=1;
				}
				return stack;
			}
		}
		return null;
	}    

	/**
	 * [Zmeni property top (typ cardstacku).]
	 */
	public void changetop ()
	{
		this.top = false;
	}
}

