package ija.ija2016.homework2.model.cards;

public interface CardDeck 
{
	public int size();
	
	public boolean put(Card card);
	
	public Card pop();
	
	public Card get();
	
	public Card get(int index);
	
	public boolean isEmpty();
}
