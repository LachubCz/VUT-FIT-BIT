package ija.ija2016.homework2.model.cards;

public interface CardStack extends CardDeck 
{
	public boolean put(CardStack stack);
	
	public CardStack pop(Card card);
}
