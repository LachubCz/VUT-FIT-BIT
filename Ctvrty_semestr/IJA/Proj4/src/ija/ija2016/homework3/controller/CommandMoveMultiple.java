/*
 * CommandMoveMultiple: Trida prikazu, ktery presouva balicky karet mezi jednotlivymi balicky karet na hraci desce
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.controller;

import ija.ija2016.homework3.model.cards.Card;
import ija.ija2016.homework3.model.cards.CardStack;

public class CommandMoveMultiple implements CommandInterface
{
	CardStack source = null;
	CardStack destination = null;
	Card card = null;
	boolean isTurnedUp = false;

	/**
	 * [konstruktor]
	 * @param  source	  [zdroj]
	 * @param  destination [cil]
	 * @param  card		[zacatek od ktereho se ma odebirat]
	 */
	public CommandMoveMultiple(CardStack source, CardStack destination, Card card)
	{
		this.source = source;
		this.destination = destination;
		this.card = card;
	}

	/**
	 * [metoda vykona prikaz]
	 */
	public void execute()
	{
		if(this.canExecute())
		{
			isTurnedUp = false;
			this.destination.put(this.source.pop(card));
			if(!this.source.isEmpty())
			{
				this.isTurnedUp = this.source.topStack().isTurnedFaceUp();
				this.source.topStack().turnFaceUp();
			}
			isTurnedUp = true;
		}
	}

	/**
	 * [metoda pro vraceni do stavu pred provedenim prikazu]
	 */
	public void unexecute()
	{
		if(!this.isTurnedUp)
			this.source.topStack().turnFaceDown();
		this.source.InitPut(this.destination.pop(card));
	}

	/**
	 * [metoda zjisti zda-li jde prikaz vykonat]
	 * @return [vraci true pokud lze prikaz provest]
	 */
	public boolean canExecute()
	{
		return this.destination.canPutCard(this.card);
	}
}
