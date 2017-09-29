/*
 * CommandBuilder: Hlavni trida pro vykonavani jednotlivych prikazu v okne
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.controller;

import ija.ija2016.homework3.model.cards.Card;
import ija.ija2016.homework3.model.cards.CardBoard;
import ija.ija2016.homework3.model.cards.CardBoardInterface;
import ija.ija2016.homework3.model.cards.CardDeck;
import ija.ija2016.homework3.model.cards.CardStack;

import java.util.ArrayList;

public class CommandBuilder 
{
	CardBoardInterface cardBoard = null;
	ArrayList<CommandInterface> undoStack;

	/**
	 * [konstruktor]
	 * @param  newCardBoard [hraci deska]
	 */
	public CommandBuilder(CardBoard newCardBoard) 
	{
		this.undoStack = new ArrayList<>();
		this.cardBoard = newCardBoard;
	}

	/**
	 * [metoda pro vykonani prikazu]
	 * @param command [druh prikazu]
	 */
	public void execute(CommandInterface command) 
	{
		if(command.canExecute())
		{
			command.execute();
			this.push(command);
			this.cardBoard.update();
		}
	}

	/**
	 * [nahrani prikazu na stack]
	 * @param command [prikaz k nahrani]
	 */
	private void push(CommandInterface command)
	{
		this.undoStack.add(command);
	}

	/**
	 * [vyjmuti prikazu ze stacku]
	 * @param  stack [stack ze ktereho se ma prikaz vyjmout]
	 * @return	   [vraci CommandInterface bez posledniho prikazu na stacku]
	 */
	private CommandInterface pop(ArrayList<CommandInterface> stack)
	{
		if(!undoStack.isEmpty())
			return undoStack.remove(undoStack.size() - 1);
		return null;
	}
	   
	/**
	 * [operace zpet, vyvola unexecute u posledniho prikazu na stacku]
	 * @return [pokud bylo vykonani uspesne vraci true]
	 */
	public boolean undo()
	{
		if(!undoStack.isEmpty())
		{
			CommandInterface command = this.pop(undoStack);
			command.unexecute();
			return true;
		}
		return false;
	}
	
	/**
	 * [ulozeni hry]
	 * @param fileName [jmeno pod kterym se ma hra ulozit]
	 */
	public void save(String fileName)
	{
		cardBoard.SaveGame(fileName);
	}
	
	/**
	 * [nacteni ulozene hry]
	 * @param fileName [jmeno ulozene hry]
	 */
	public void load(String fileName)
	{
		cardBoard.LoadGame(fileName);
	}
	
	/**
	 * [spoustec varianty prikazu move]
	 * @param source	  [zdroj]
	 * @param destination [cil]
	 */
	public void move(CardDeck source, CardDeck destination)
	{
		this.execute(new CommandMove(source, destination));
	}
	
	/**
	 * [spoustec varianty prikazu move]
	 * @param source	  [zdroj]
	 * @param destination [cil]
	 */
	public void move(CardStack source, CardDeck destination)
	{
		this.execute(new CommandMove(source, destination));
	}
	
	/**
	 * [spoustec varianty prikazu move]
	 * @param source	  [zdroj]
	 * @param destination [cil]
	 */
	public void move(CardStack source, CardStack destination)
	{
		this.execute(new CommandMove(source, destination));
	}
	
	/**
	 * [spoustec varianty prikazu move]
	 * @param source	  [zdroj]
	 * @param destination [cil]
	 */
	public void move(CardDeck source, CardStack destination)
	{
		this.execute(new CommandMove(source, destination));
	}
	
	/**
	 * [spoustec prikazu next]
	 * @param stack [stack pro vykonani prikazu]
	 */
	public void next(CardStack stack)
	{
		this.execute(new CommandNext(stack));
	}
	
	/**
	 * [spoustec prikazu renew]
	 * @param source [stack pro vykonani prikazu]
	 */
	public void renew(CardStack source)
	{
		this.execute(new CommandRenew(source));
	}
	/**
	 * [spoustec prikazu movemultiple]
	 * @param source	  [zdroj]
	 * @param destination [cil]
	 * @param card		[zacatek od ktereho se ma odebirat]
	 */
	public void movemultiple(CardStack source, CardStack destination, Card card)
	{
		this.execute(new CommandMoveMultiple(source, destination, card));
	}

	/**
	 * [aktualizace vykresleni na displej]
	 */
	public void Update()
	{
		this.cardBoard.update();
	}
}
