/*
 * CardBoard: Trida reprezentujici hraci desku
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.model.cards;

import ija.ija2016.homework3.model.board.AbstractFactorySolitaire;
import ija.ija2016.homework3.model.board.FactoryKlondike;
import ija.ija2016.homework3.model.cards.CardDeck;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class CardBoard implements CardBoardInterface 
{
	private boolean gameOver = false;
	private boolean game = true;
	protected CardStack SourcePack = null;
	protected CardDeck StandardDeck = null;
	protected ArrayList<CardStack> WorkingStacks;
	protected ArrayList<CardDeck> TargetDecks;
	protected ArrayList<PleaseRepaint> observers;
	public static final int stackCount = 7;
	public static final int deckCount = 4;
	public static final String saveExtension = ".XXX";
	
	/**
	 * [konstruktor]
	 */
	public CardBoard() 
	{
		this.TargetDecks = new ArrayList<>();
		this.WorkingStacks = new ArrayList<>();
		this.observers = new ArrayList<>();
		AbstractFactorySolitaire Maker = new FactoryKlondike(); 
		this.StandardDeck = Maker.createCardDeck();
		this.StandardDeck = RandomSwap(StandardDeck);
		for (int i = 0; i < 7; i++)
		{
			this.WorkingStacks.add(Maker.createWorkingPack());
			for(int u = 0; u <= i; u++)
			{
				if (u == i)
				{
					Card c = this.StandardDeck.pop();
					c.turnFaceUp();
					this.WorkingStacks.get(i).InitPut(c);
				}
				else
				{
					this.WorkingStacks.get(i).InitPut(this.StandardDeck.pop());
				}
			}
		}
		this.SourcePack = Maker.createSourcePack(StandardDeck);
		this.TargetDecks.add(Maker.createTargetPack(Card.Color.DIAMONDS));
		this.TargetDecks.add(Maker.createTargetPack(Card.Color.HEARTS));
		this.TargetDecks.add(Maker.createTargetPack(Card.Color.SPADES));
		this.TargetDecks.add(Maker.createTargetPack(Card.Color.CLUBS));
	}
	
	/**
	 * [registerObserver description]
	 * @param repaintInterface [description]
	 */
	public void registerObserver(PleaseRepaint repaintInterface) 
	{
		observers.add(repaintInterface);
	}
	
	/**
	 * [metoda vrati zdrojovy balicek]
	 * @return [zdrojovy balicek]
	 */
	public CardStack getSourcePack() 
	{
		return this.SourcePack;
	}
	
	/**
	 * [metoda vrati cilovy balicek]
	 * @param  index [cislo ciloveho balicku]
	 * @return	   [cilovy balicek]
	 */
	public CardDeck getDeck(int index)
	{
		return this.TargetDecks.get(index);
	}
	
	/**
	 * [metoda vrati pracovni balicek]
	 * @param  index [cislo pracovniho balicku]
	 * @return	   [pracovni balicek]
	 */
	public CardStack getStack(int index)
	{
		return this.WorkingStacks.get(index);
	}
	
	/**
	 * [metoda nacte ulozenou instanci cardboard]
	 * @param  filename [jmeno ulozene hry]
	 * @return		  [pokud se nacteni povedlo vraci true]
	 */
	public boolean LoadGame (String filename)
	{
		try 
		{
			FileInputStream fin = new FileInputStream(filename + saveExtension);
			ObjectInputStream ois = new ObjectInputStream(fin);
			this.StandardDeck = (CardDeck)  ois.readObject();
			this.SourcePack = (CardStack)  ois.readObject();
			this.WorkingStacks = (ArrayList<CardStack>)  ois.readObject();
			this.TargetDecks = (ArrayList<CardDeck>)  ois.readObject();
			fin.close();
			ois.close();
		}
		catch (Exception e)
		{ 
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	/**
	 * [metoda ulozi aktualni instanci cardboard]
	 * @param  filename [jmeno ukladane hry]
	 * @return		  [pokud se ulozeni povedlo vraci true]
	 */
	public boolean SaveGame (String filename)
	{
		try 
		{
			FileOutputStream fout = new FileOutputStream(filename + saveExtension);
			ObjectOutputStream oos = new ObjectOutputStream(fout);
			oos.writeObject(this.StandardDeck);
			oos.writeObject(this.SourcePack);
			oos.writeObject(this.WorkingStacks);
			oos.writeObject(this.TargetDecks);
			fout.close();
			oos.close();
		}
		catch (Exception e) 
		{   
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	/**
	 * [metoda nahodne promicha balicek karet]
	 * @param  Deck [balicek na promichani]
	 * @return	  [promichany balicek]
	 */
	public CardDeck RandomSwap(CardDeck Deck)
	{
		Random NumberGenerator = new Random();
		for (int i = 0; i < 52; i++)
		{
			int RandomCard = NumberGenerator.nextInt(51);
			Collections.swap(Deck.karty, i, RandomCard);
		}
		return Deck;
	}

	/**
	 * [metoda vytvori napovedu pro vybranou kartu]
	 * @param  Card [karta pro napovedu]
	 * @return	  [cislo ciloveho (0,1,2,3) / pracovniho (10,11,12,13,14,15,16) balicku]
	 */
	public int createHint(Card Card)
	{
		CardHint hint = hintForCard(Card);
		if (!(hint.getCardDecks().isEmpty()))
		{
			for (int i = 0; i < 4; i++)
			{
				if (hint.getCardDeck(0).equals(this.getDeck(i)))
				{
					return i;
				}
			}
		}
		if (!(hint.getCardStacks().isEmpty()))
		{
			for (int i = 0; i < 7; i++)
			{
				if (hint.getCardStack(0).equals(this.getStack(i)))
				{
					return 10 + i;
				}
			}
		}
		return -1;
	}

	/**
	 * [pomocna metoda pro createHint - najde balicky, kam muze byt karta polozena]
	 * @param  Card [karta na polozeni]
	 * @return	  [instance tridy CardHint ve ktere jsou ulozeny balicky na ktere se da karta polozit]
	 */
	public CardHint hintForCard(Card Card)
	{
		CardHint hint = new CardHint();
		for (int i = 0; i < 7; i++)
		{
			if (this.getStack(i).canPutCard(Card))
			{
				hint.add(this.getStack(i));
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (this.getDeck(i).canPutCard(Card))
			{
				hint.add(this.getDeck(i));
			}
		}
		return hint;
	}
	
	/**
	 * [metoda aktualizuje zobrazovanou plochu]
	 */
	public void update() 
	{
		this.game = this.isGameOver();
		this.observers.forEach((observer) -> {observer.repaint();});
	}
	
	/**
	 * [metoda zjistuje zda-li nastal konec hry]
	 * @return [pokud nastal konec hry vraci true]
	 */
	public boolean isGameOver()
	{
		for(int i = 0; i < 7; i++)
		{
			if(this.getStack(i).isEmpty() == false)
				return false;
			if(!this.getSourcePack().isEmpty() || !this.getSourcePack().isSourceEmpty())
				return false;
		}
		return true;
	}
}
