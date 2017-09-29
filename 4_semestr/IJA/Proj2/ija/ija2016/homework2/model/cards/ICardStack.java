package ija.ija2016.homework2.model.cards;
import java.util.Arrays;

public class ICardStack implements CardStack 
{
	private int size;
	private boolean top;
	private Card [] stack;
	
	public ICardStack(int size) 
	{
		this.stack = new ICard[size];
		this.size = 0;
		this.top = true;
	}
	
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
	
	public boolean isEmpty() 
	{
		if(this.stack[0] == null)
			return true;
		else
			return false;
	}
	
	public int size() 
	{
		return this.size;
	}
	
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
	
    public int hashCode() 
    {
        int result = 17;
        result = 31 * result + stack.hashCode();
        result = 31 * result + size;
        return result;
    }

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

	public Card get() 
	{
		if (this.size() == 0)
			return null;
		Card c = this.stack[(this.size() - 1)];
		return c; 
	}

	public Card get(int index) 
	{
		if (this.size() == 0)
			return null;
		Card c = this.stack[(index)];
		return c;
	}

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

	public void changetop ()
	{
		this.top = false;
	}
}

