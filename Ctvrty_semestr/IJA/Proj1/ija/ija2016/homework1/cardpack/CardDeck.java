package ija.ija2016.homework1.cardpack;

public class CardDeck
{
	private int size;
	private Card [] deck;
	
	public CardDeck(int size) 
	{
		this.deck = new Card[size];
		this.size = 0;
	}
	
	public static CardDeck createStandardDeck() 
	{
		CardDeck deck = new CardDeck(52);
        for (int i = 1; i <= 13; i++) 
        { 
        	Card c1 = new Card(Card.Color.CLUBS, i);
        	deck.put(c1);
        }
        for (int i = 1; i <= 13; i++) 
        { 
        	Card c1 = new Card(Card.Color.DIAMONDS, i);
        	deck.put(c1);
        }
        for (int i = 1; i <= 13; i++) 
        { 
        	Card c1 = new Card(Card.Color.HEARTS, i);
        	deck.put(c1);
        }
        for (int i = 1; i <= 13; i++) 
        { 
        	Card c1 = new Card(Card.Color.SPADES, i);
        	deck.put(c1);
        }
		return deck;
	}
	
	public int size()  
	{
		return this.size;
	}
	
	public void put(Card card) 
	{
		this.deck[this.size] = card;
		this.size+=1;
	}
	
	public Card pop() 
	{
		Card temp;
		if (this.size == 0)
			return null;
		else
		{
			this.size-=1;
			temp = this.deck[this.size];
			this.deck[this.size] = null;
			return temp;
		}
	}
}