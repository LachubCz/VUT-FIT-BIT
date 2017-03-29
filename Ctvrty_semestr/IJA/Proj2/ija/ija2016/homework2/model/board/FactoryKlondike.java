package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;
import ija.ija2016.homework2.model.cards.ICard;
import ija.ija2016.homework2.model.cards.ICardDeck;
import ija.ija2016.homework2.model.cards.ICardStack;

public class FactoryKlondike extends AbstractFactorySolitaire {
	
	public CardDeck createCardDeck()
	{
		CardDeck deck = new ICardDeck(52, 1);
        for (int i = 1; i <= 13; i++) 
        { 
        	Card c1 = new ICard(ICard.Color.CLUBS, i);
        	deck.put(c1);
        }
        for (int i = 1; i <= 13; i++) 
        { 
        	Card c1 = new ICard(ICard.Color.DIAMONDS, i);
        	deck.put(c1);
        }
        for (int i = 1; i <= 13; i++) 
        { 
        	Card c1 = new ICard(ICard.Color.HEARTS, i);
        	deck.put(c1);
        }
        for (int i = 1; i <= 13; i++) 
        { 
        	Card c1 = new ICard(ICard.Color.SPADES, i);
        	deck.put(c1);
        }
		return deck;
	}
	
	public Card createCard(Card.Color color, int value)
	{
		if (value > 13 || value < 1 )
			return null;
		Card c = new ICard(color, value);
		return c;
	}
	
	public CardDeck createTargetPack(Card.Color color)
	{
		CardDeck deck = new ICardDeck(13, 2, color);
		return deck;
	}
	
	public CardStack createWorkingPack()
	{
		CardStack stack = new ICardStack(13);
		return stack;
	}
}
