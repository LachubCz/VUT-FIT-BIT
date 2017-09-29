package ija.ija2016.homework1.cardpack;

public class Card 
{
	protected int value;
	
	protected Card.Color c;
	
	public enum Color 
	{
		CLUBS("C"), DIAMONDS("D"), HEARTS("H"), SPADES("S");
	    
		private final String name;       

	    private Color(String s) {
	        name = s;
	    }
	    public String toString() {
	        return this.name;
	     }
	}

	public Card(Card.Color c, int value)
	{
		if (value > 13 || value < 1 )
			System.exit(0);
		
		this.c = c;
		this.value = value;
	}
	
	public int value()
	{
		return value;
	}
	
	public Card.Color color()
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

        Card card = (Card) o;

        return card.c.equals(c) &&
                card.value == value;
    }
	
    public int hashCode() {
        int result = 17;
        result = 31 * result + c.hashCode();
        result = 31 * result + value;
        return result;
    }
}

