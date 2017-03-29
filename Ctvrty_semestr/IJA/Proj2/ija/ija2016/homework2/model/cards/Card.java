package ija.ija2016.homework2.model.cards;

public interface Card 
{	
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
	
	public int value();
	
	public boolean isTurnedFaceUp();
	
	public boolean turnFaceUp();
	
	public Card.Color color();
	
	public boolean similarColorTo(Card c);
	
	public int compareValue(Card c);
}
