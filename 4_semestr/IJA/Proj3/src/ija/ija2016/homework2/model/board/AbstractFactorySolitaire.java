package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;

/**
 * Abstraktni tovarna na karty
 */
public abstract class AbstractFactorySolitaire {

	/**
	 * [Vytvari objekt reprezentujici balicek karet.]
	 * @return [Balicek karet.]
	 */
	public abstract CardDeck createCardDeck();
	
	/**
	 * [Vytvori objekt reprezentujici kartu.]
	 * @param  color [Barva karty.]
	 * @param  value [Hodnota karty v rozsahu 1 az 13.]
	 * @return       [Objekt karty. Pokud je nektery z parametru neplatny (objekt nelze vytvorit), vraci null.]
	 */
	public abstract Card createCard(Card.Color color, int value);
	
	/**
	 * [Vytvari objekt reprezentujici cilovy balicek. Cilem hrace je vlozit vsechny karty zadane barvy do ciloveho balicku.]
	 * @param  color [Barva celeho balicku.]
	 * @return       [Cilovy balicek.]
	 */
	public abstract CardDeck createTargetPack(Card.Color color);
	
	/**
	 * [Vytvari objekt reprezentujici pracovni pole pro karty.]
	 * @return [Pracovni pole.]
	 */
	public abstract CardStack createWorkingPack();
}
