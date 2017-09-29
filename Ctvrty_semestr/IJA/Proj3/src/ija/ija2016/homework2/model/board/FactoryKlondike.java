package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;
import ija.ija2016.homework2.model.cards.ICard;
import ija.ija2016.homework2.model.cards.ICardDeck;
import ija.ija2016.homework2.model.cards.ICardStack;

/**
 * Trida implementuje abstraktni class AbstractFactorySolitaire (tovarnu na karty)
 */
public class FactoryKlondike extends AbstractFactorySolitaire {

	/**
	 * [Vytvari objekt reprezentujici balicek karet.]
	 * @return [Balicek karet.]
	 */
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
	
	/**
	 * [Vytvori objekt reprezentujici kartu.]
	 * @param  color [Barva karty.]
	 * @param  value [Hodnota karty v rozsahu 1 az 13.]
	 * @return       [Objekt karty. Pokud je nektery z parametru neplatny (objekt nelze vytvorit), vraci null.]
	 */
	public Card createCard(Card.Color color, int value)
	{
		if (value > 13 || value < 1 )
			return null;
		Card c = new ICard(color, value);
		return c;
	}

	/**
	 * [Vytvari objekt reprezentujici cilovy balicek. Cilem hrace je vlozit vsechny karty zadane barvy do ciloveho balicku.]
	 * @param  color [Barva celeho balicku.]
	 * @return       [Cilovy balicek.]
	 */
	public CardDeck createTargetPack(Card.Color color)
	{
		CardDeck deck = new ICardDeck(13, 2, color);
		return deck;
	}
	
	/**
	 * [Vytvari objekt reprezentujici pracovni pole pro karty.]
	 * @return [Pracovni pole.]
	 */
	public CardStack createWorkingPack()
	{
		CardStack stack = new ICardStack(13);
		return stack;
	}
}
