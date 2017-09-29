package ija.ija2016.homework2.model.cards;

public class ICardDeck implements CardDeck 
{
	private int size;
	
	private int type;
	
	private Card [] deck;
	
	protected ICard.Color c;
	
	public ICardDeck(int size, int type) 
	{
		this.type = type;
		this.deck = new Card[size];
		this.size = 0;
	}
	
	public ICardDeck(int size, int type, Card.Color color) 
	{
		this.type = type;
		this.deck = new Card[size];
		this.size = 0;
		this.c = color;
	}
	
	public int size()  
	{
		return this.size;
	}
	
	public boolean put(Card card) 
	{
		if (card == null)
			return false;
		
		if (this.type == 1)
		{
			this.deck[this.size] = card;
			this.size+=1;
			return true;
		}
		else
		{
			if(card.color() == this.color())
				;
			else if(card.color() == this.color())
				;
			else if(card.color() == this.color())
				;
			else if(card.color() == this.color())
				;
			else
				return false;

			if(this.size() == (card.value()-1))
			{
				this.deck[this.size] = card;
				this.size+=1;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	public ICard.Color color()
	{
		return this.c;
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
	
	public Card get()
	{
		if (this.size() == 0)
			return null;
		Card c = this.deck[(this.size() - 1)];
		return c; 
	}
	
	public Card get(int index)
	{
		if (this.size() == 0)
			return null;
		Card c = this.deck[(index)];
		return c;
	}
	
	public boolean isEmpty()
	{
		return true; //not true
	}
}
