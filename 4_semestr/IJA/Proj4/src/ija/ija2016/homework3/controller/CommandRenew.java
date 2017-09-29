/*
 * CommandRenew: Trida prikazu, ktery obrati balicek karet z odebiraciho balicku na zdrojovy balicek
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.controller;

import ija.ija2016.homework3.model.cards.CardStack;

public class CommandRenew implements CommandInterface{
	CardStack stack = null;

	/**
	 * [konstruktor]
	 * @param  source [cil]
	 */
	public CommandRenew(CardStack source)
	{
		this.stack = source;
	}

	/**
	 * [metoda vykona prikaz]
	 */
	public void execute()
	{
		if(this.canExecute())
		{
			stack.MoveBackToStack();
		}	   
	}

	/**
	 * [metoda pro vraceni do stavu pred provedenim prikazu]
	 */
	public void unexecute()
	{
		while(!stack.isEmpty())
			stack.showNext();
	}

	/**
	 * [metoda zjisti zda-li jde prikaz vykonat]
	 * @return [vraci true pokud lze prikaz provest]
	 */
	public boolean canExecute()
	{
		return this.stack.isEmpty();
	}
}
