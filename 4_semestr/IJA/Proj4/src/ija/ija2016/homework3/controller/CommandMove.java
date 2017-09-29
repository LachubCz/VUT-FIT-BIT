/*
 * CommandMove: Trida prikazu, ktery presouva karty mezi jednotlivymi balicky
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.controller;

import ija.ija2016.homework3.model.cards.CardDeck;
import ija.ija2016.homework3.model.cards.CardStack;

public class CommandMove implements CommandInterface
{
	CardDeck source = null;
	CardStack sourceStack = null;
	CardDeck destination = null;
	CardStack destinationStack = null;
	boolean isTurnedUp = false;
	
	/**
	 * [konstruktor]
	 * @param  source	  [zdroj]
	 * @param  destination [cil]
	 */
	public CommandMove(CardDeck source, CardDeck destination) 
	{
		this.source = source;
		this.destination = destination;
	}
	
	/**
	 * [konstruktor]
	 * @param  source	  [zdroj]
	 * @param  destination [cil]
	 */
	public CommandMove(CardStack source, CardDeck destination) 
	{
		this.sourceStack = source;
		this.destination = destination;
	}
	
	/**
	 * [konstruktor]
	 * @param  source	  [zdroj]
	 * @param  destination [cil]
	 */
	public CommandMove(CardStack source, CardStack destination) 
	{
		this.sourceStack = source;
		this.destinationStack = destination;
	}
	
	/**
	 * [konstruktor]
	 * @param  source	  [zdroj]
	 * @param  destination [cil]
	 */
	public CommandMove(CardDeck source, CardStack destination) 
	{
		this.source = source;
		this.destinationStack = destination;
	}

	/**
	 * [metoda vykona prikaz]
	 */
	public void execute() 
	{
		if(this.canExecute()) 
		{
			isTurnedUp = false;
			if (this.source != null)
			{
				if(this.source.size() != 1)
				{
					if(destination != null && source != null)
					{
						this.isTurnedUp = this.source.get(this.source.size()-2).isTurnedFaceUp();
						this.destination.put(source.pop());
					}
					
					if(destinationStack != null && source != null)
					{
						this.isTurnedUp = this.source.get(this.source.size()-2).isTurnedFaceUp();
						this.destinationStack.put(source.pop());
					}
				}
				else
				{
					if(destination != null && source != null)
					{
						this.isTurnedUp = true;
						this.destination.put(source.pop());
					}
					
					if(destinationStack != null && source != null)
					{
						this.isTurnedUp = true;
						this.destinationStack.put(source.pop());
					}
				}
			}
			if (this.sourceStack != null)
			{
				if(this.sourceStack.size() != 1)
				{
					if(destinationStack != null && sourceStack != null)
					{
						this.isTurnedUp = this.sourceStack.getFromStack(this.sourceStack.size()-2).isTurnedFaceUp();
						this.destinationStack.put(sourceStack.pop());
					}
					if(destination != null && sourceStack != null)
					{
						this.isTurnedUp = this.sourceStack.getFromStack(this.sourceStack.size()-2).isTurnedFaceUp();
						this.destination.put(sourceStack.pop());
					}
				}
				else
				{
					if(destinationStack != null && sourceStack != null)
					{
						this.isTurnedUp = true;
						this.destinationStack.put(sourceStack.pop());
					}
					if(destination != null && sourceStack != null)
					{
						this.isTurnedUp = true;
						this.destination.put(sourceStack.pop());
					}
				}
			}
		}
	}

	/**
	 * [metoda pro vraceni do stavu pred provedenim prikazu]
	 */
	public void unexecute() 
	{
		if(!this.isTurnedUp) 
		{
			if(this.source != null)
			{
				this.source.top().turnFaceDown();
			}
			if(this.sourceStack != null)
			{
				this.sourceStack.topStack().turnFaceDown();
			}
		}
		if(this.source != null)
		{
			if(this.destination != null)
			{
				this.source.emplace(this.destination.pop());
			}
			if(this.destinationStack != null)
			{
				this.source.emplace(this.destinationStack.pop());
			}
		}
		if(this.sourceStack != null)
		{
			if(this.destination != null)
			{
				this.sourceStack.InitPut(this.destination.pop());
			}
			if(this.destinationStack != null)
			{
				this.sourceStack.InitPut(this.destinationStack.pop());
			}
		}
	}

	/**
	 * [metoda zjisti zda-li jde prikaz vykonat]
	 * @return [vraci true pokud lze prikaz provest]
	 */
	public boolean canExecute() 
	{
		if (destination != null)
		{
			if(source != null)
			{
				if(!destination.canPut(source.top())) 
				{
					return false;
				}
				return true;
			}
			if(sourceStack != null)
			{
				if(!destination.canPut(sourceStack.getStack(sourceStack.size() - 1))) 
				{
					return false;
				}
				return true;
			}
		}
		if (destinationStack != null)
		{
			if(source != null)
			{
				if(!destinationStack.canPutCard(source.top())) 
				{
					return false;
				}
				return true;
			}
			if(sourceStack != null)
			{
				if(!destinationStack.canPutCard(sourceStack.getStack(sourceStack.size() - 1))) 
				{
					return false;
				}
				return true;
			}
		}
		return false;
	}
}
