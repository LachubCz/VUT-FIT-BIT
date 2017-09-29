package ija.ija2016.homework2.model.cards;

public class ICard implements Card 
{
	protected int value;
	
	protected ICard.Color c;
	
	protected boolean TurnedFaceUp;

	public ICard(Card.Color c, int value)
	{
		if (value > 13 || value < 1 )
			System.exit(0);
		
		this.c = c;
		this.value = value;
		this.TurnedFaceUp = false;
	}
	
	public int value()
	{
		return value;
	}
	
	public ICard.Color color()
	{
		return c;
	}
	
	public String toString()
	{
		String str = null;
		
		if (this.value > 13)
			System.exit(0);
		
		if (this.value < 11 && this.value != 1)
		{
			str = String.format ("%d", this.value);
			str+="(";
			str+=this.c.toString();
			str+=")";
			return str;
		}
		if(this.value == 1)
		{
			str = "A";
		}	
		if(this.value == 11)
		{
			str = "J";
		}	
		if(this.value == 12)
		{
			str = "Q";
		}	
		if(this.value == 13)
		{
			str = "K";
		}	
			str+="(";
			str+=this.c.toString();
			str+=")";
			return str;
	}
	
	public boolean equals(Object o)
	{
        if (o == this) return true;
        if (!(o instanceof Card)) {
            return false;
        }

        ICard card = (ICard) o;

        return card.c.equals(c) &&
                card.value == value;
    }
	
    public int hashCode() 
    {
        int result = 17;
        result = 31 * result + c.hashCode();
        result = 31 * result + value;
        return result;
    }
	
	public boolean isTurnedFaceUp()
	{
		if (this.TurnedFaceUp == true)
			return true;
		else
			return false;
	}
	
	public boolean turnFaceUp()
	{
		if (this.isTurnedFaceUp())
		{
			return false;
		}
		else
		{
			this.TurnedFaceUp = true;
			return true;
		}
			
	}
	
	public boolean similarColorTo(Card c)
	{
		if(c.color().equals(Card.Color.HEARTS) || c.color().equals(Card.Color.DIAMONDS))
			if(this.color().equals(Card.Color.HEARTS) || this.color().equals(Card.Color.DIAMONDS))
				return true;
		if(c.color().equals(Card.Color.CLUBS) || c.color().equals(Card.Color.SPADES))
			if(this.color().equals(Card.Color.CLUBS) || this.color().equals(Card.Color.SPADES))
				return true;
		return false;
	}
	
	public int compareValue(Card c)
	{
		return this.value() - c.value();
	}
	
	public boolean color_compare(Card c)
	{
		if(this.color().equals(Card.Color.HEARTS) && c.color().equals(Card.Color.HEARTS))
			return true;
		if(this.color().equals(Card.Color.CLUBS) && c.color().equals(Card.Color.CLUBS))
			return true;
		if(this.color().equals(Card.Color.DIAMONDS) && c.color().equals(Card.Color.DIAMONDS))
			return true;
		if(this.color().equals(Card.Color.SPADES) && c.color().equals(Card.Color.SPADES))
			return true;
		return false;
	}
}

