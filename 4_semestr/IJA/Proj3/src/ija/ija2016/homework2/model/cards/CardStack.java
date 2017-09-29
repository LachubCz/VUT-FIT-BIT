package ija.ija2016.homework2.model.cards;

/**
 * Rozhrani CardStack
 */
public interface CardStack extends CardDeck 
{
	/**
	 * [Vlozi karty ze zasobniku stack na vrchol zasobniku. Karty vklada ve stejnem poradi, v jakem jsou uvedeny v zasobniku stack.]
	 * @param  stack [Zasobnik vkladanych karet.]
	 * @return       [Uspesnost akce.]
	 */
	public boolean put(CardStack stack);
	
	/**
	 * [Metoda odebere ze zasobniku sekvenci karet od zadane karty az po vrchol zasobniku. Pokud je hledana karta na vrcholu, bude v sekvenci pouze jedna karta.]
	 * @param  card [Hledana karta.]
	 * @return      [Zasobnik karet obsahujici odebranou sekvenci. Pokud hledana karta v zasobniku neni, vraci null.]
	 */
	public CardStack pop(Card card);
}
