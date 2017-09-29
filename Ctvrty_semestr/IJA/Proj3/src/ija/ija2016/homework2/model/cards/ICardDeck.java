package ija.ija2016.homework2.model.cards;

/**
 * Implementace rozhrani CardDeck
 */
public class ICardDeck implements CardDeck 
{
	private int size;
	
	private int type;
	
	private Card [] deck;
	
	protected ICard.Color c;
	
	/**
	 * [Konstruktor ICardDeck.]
	 * @param  size [Velikost balicku.]
	 * @param  type [Typ balicku.]
	 * @return      [Void.]
	 */
	public ICardDeck(int size, int type) 
	{
		this.type = type;
		this.deck = new Card[size];
		this.size = 0;
	}
	
	/**
	 * [Konstruktor ICardDeck.]
	 * @param  size  [Velikost balicku.]
	 * @param  type  [Typ balicku.]
	 * @param  color [Barva balicku.]
	 * @return       [Void.]
	 */
	public ICardDeck(int size, int type, Card.Color color) 
	{
		this.type = type;
		this.deck = new Card[size];
		this.size = 0;
		this.c = color;
	}
	
	/**
	 * [Vrati aktualni pocet karet v balicku.]
	 * @return [Aktualni pocet karet v balicku.]
	 */
	public int size()  
	{
		return this.size;
	}
	
	/**
	 * [Vlozi kartu na vrchol balicku.]
	 * @param  card [Vkladana karta.]
	 * @return      [Uspesnost akce.]
	 */
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
	
	/**
	 * [Metoda zjisti barvu balicku.]
	 * @return [Barva balicku.]
	 */
	public ICard.Color color()
	{
		return this.c;
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
			temp = this.deck[this.size];
			this.deck[this.size] = null;
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
		Card c = this.deck[(this.size() - 1)];
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
		Card c = this.deck[(index)];
		return c;
	}
	
	/**
	 * [Test, zda je balicek karet prazdny.]
	 * @return [Vraci true, pokud je balicek prazdny.]
	 */
	public boolean isEmpty()
	{
		return true;
	}
}
