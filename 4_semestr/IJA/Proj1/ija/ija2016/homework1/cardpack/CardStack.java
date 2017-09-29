package ija.ija2016.homework1.cardpack;
import java.util.Arrays;

public class CardStack 
{
	private int size;
	private Card [] stack;
	
	public CardStack(int size) 
	{
		this.stack = new Card[size];
		this.size = 0;
	}
	
	public void put(Card card) 
	{
		this.stack[this.size] = card;
		this.size+=1;
	}
	
	public void put(CardStack stack) 
	{
		for(int i = 0; i < stack.size; i++)
		{
			this.stack[this.size] = stack.stack[i];
			this.size+=1;
		}
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
	
	public CardStack takeFrom(Card card) 
	{
		int i;
		int temp = this.size;

		for(i = 0; i < this.size; i++)
		{
			if(this.stack[i].equals(card))
			{
				int stacksize = this.size - i ;
				CardStack stack = new CardStack(stacksize);
				while(i < temp)
				{
					stack.put(this.stack[i]);
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
        if (!(o instanceof CardStack)) 
        {
            return false;
        }

        CardStack stack1 = (CardStack) o;
        return Arrays.equals(stack1.stack, stack) && stack1.size == size;
    }
	
    public int hashCode() 
    {
        int result = 17;
        result = 31 * result + stack.hashCode();
        result = 31 * result + size;
        return result;
    }
}