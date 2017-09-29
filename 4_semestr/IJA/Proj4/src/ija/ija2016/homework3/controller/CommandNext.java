/*
 * CommandNext: Trida prikazu, ktery otaci vrchni kartu ze zdrojoveho balicku a vklada ji na odebiraci balicek
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.controller;

import ija.ija2016.homework3.model.cards.CardStack;

public class CommandNext implements CommandInterface
{
	CardStack stack = null;

	/**
	 * [CommandNext description]
	 * @param  stack [cil]
	 */
	public CommandNext(CardStack stack)
	{
		this.stack = stack;
	}

	/**
	 * [metoda vykona prikaz]
	 */
	public void execute()
	{
		if(this.canExecute()) 
		{
			stack.showNext();
		}
	}

	/**
	 * [metoda pro vraceni do stavu pred provedenim prikazu]
	 */
	public void unexecute()
	{
		stack.hideTopCard();
	}

	/**
	 * [metoda zjisti zda-li jde prikaz vykonat]
	 * @return [vraci true pokud lze prikaz provest]
	 */
	public boolean canExecute()
	{
		return !this.stack.isEmpty();
	}
}
