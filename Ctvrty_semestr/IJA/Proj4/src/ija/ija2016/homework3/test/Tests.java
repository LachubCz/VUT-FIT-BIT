/*
 * Tests: Trida testu logickych casti aplikace
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.test;

import ija.ija2016.homework3.model.board.AbstractFactorySolitaire;
import ija.ija2016.homework3.model.board.FactoryKlondike;
import ija.ija2016.homework3.model.cards.Card;
import ija.ija2016.homework3.model.cards.CardDeck;
import ija.ija2016.homework3.model.cards.CardStack;
import ija.ija2016.homework3.model.cards.CardBoard;

import java.util.HashSet;
import java.util.Set;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class Tests {
    
    protected AbstractFactorySolitaire factory;
    
    /**
     * Priprava pro testy
     */
    @Before
    public void setUp() {
         factory = new FactoryKlondike();
    }
    
    /**
     * Konec testu
     */
    @After
    public void tearDown() {
    }

    /**
     * Test karet
     */
    @Test
    public void testCard() {
        Card c1 = factory.createCard(Card.Color.CLUBS, 0);
        Card c2 = factory.createCard(Card.Color.DIAMONDS, 11);
        Card c3 = factory.createCard(Card.Color.SPADES, 5);
        Card c4 = factory.createCard(Card.Color.CLUBS, 6);
        Card c5 = factory.createCard(Card.Color.CLUBS, 6);
        
        Assert.assertNull("(Card.Color.CLUBS, 0) nelze", c1);
        Assert.assertFalse("Karta c2 nema byt otocena licem nahoru.", c2.isTurnedFaceUp());
        Assert.assertTrue("Otoceni karty c2.", c2.turnFaceUp());
        Assert.assertTrue("Karta c2 ma byt otocena licem nahoru.", c2.isTurnedFaceUp());
        Assert.assertFalse("Otoceni karty c2.", c2.turnFaceUp());
        Assert.assertTrue("Stejny typ barvy u karet", c3.similarColorTo(c4));
        Assert.assertFalse("Stejny typ barvy u karet", c3.similarColorTo(c2));
        Assert.assertEquals("Shoda objektu.", c4, c5);
        Assert.assertTrue("Rozdil hodnot karet.", c2.compareValue(c3) == 6);
    }

    /**
     * Test Balicku karet
     */
    @Test
    public void testCardDeck() {
        CardDeck deck = factory.createCardDeck();

        Assert.assertEquals("Pocet karet je 52", 52, deck.size());

        Set<Card> testCards = new HashSet<>();
        for (int i = 1; i <= 13; i++) { testCards.add(factory.createCard(Card.Color.CLUBS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(factory.createCard(Card.Color.DIAMONDS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(factory.createCard(Card.Color.HEARTS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(factory.createCard(Card.Color.SPADES,i)); }

        for (int i = 0; i < 52; i++) {
            Card c = deck.pop();
            Assert.assertTrue("Spravna karta.", testCards.remove(c));
        }

        Assert.assertTrue("Testova sada musi byt prazdna", testCards.isEmpty());
    }

    /**
     * test ciloveho balicku
     */
    @Test
    public void testTargetPack() {
        
        CardDeck pack = factory.createTargetPack(Card.Color.CLUBS);
        
        Card c1 = factory.createCard(Card.Color.DIAMONDS, 11);
        Card c2 = factory.createCard(Card.Color.DIAMONDS, 1);
        Card c3 = factory.createCard(Card.Color.CLUBS, 11);
        Card c4 = factory.createCard(Card.Color.CLUBS, 1);
        Card c5 = factory.createCard(Card.Color.CLUBS, 2);

        Assert.assertFalse("Nelze vlozit kartu", pack.put(c1));
        Assert.assertEquals("Pocet karet v baliku je 0.", 0, pack.size());
        Assert.assertFalse("Nelze vlozit kartu", pack.put(c2));
        Assert.assertEquals("Pocet karet v baliku je 0.", 0, pack.size());
        Assert.assertFalse("Nelze vlozit kartu", pack.put(c3));
        Assert.assertTrue("Pocet karet v baliku je 0.", pack.isEmpty());
        Assert.assertTrue("Lze vlozit kartu", pack.put(c4));
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, pack.size());
        Assert.assertFalse("Nelze vlozit kartu", pack.put(c3));
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, pack.size());
        Assert.assertTrue("Lze vlozit kartu", pack.put(c5));
        Assert.assertEquals("Pocet karet v baliku je 2.", 2, pack.size());
        
        Card c55 = factory.createCard(Card.Color.CLUBS, 2);
        Card c44 = factory.createCard(Card.Color.CLUBS, 1);
        
        Assert.assertEquals("Na vrcholu je karta c55", c55, pack.get());
        Assert.assertEquals("Pocet karet v baliku je 2.", 2, pack.size());
        Assert.assertEquals("Vyber karty z vrcholu", c55, pack.pop());
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, pack.size());
        Assert.assertEquals("Na vrcholu je karta c44", c44, pack.get());
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, pack.size());
    }
    
    /**
     * test pracujiciho pole
     */
    @Test
    public void testWorkingPack() {
        
        CardStack pack = factory.createWorkingPack();
           
        Card c1 = factory.createCard(Card.Color.DIAMONDS, 11);
        Card c2 = factory.createCard(Card.Color.DIAMONDS, 13);
        Card c3 = factory.createCard(Card.Color.HEARTS, 12);
        Card c4 = factory.createCard(Card.Color.CLUBS, 12);
        Card c5 = factory.createCard(Card.Color.SPADES, 11);
        Card c6 = factory.createCard(Card.Color.HEARTS, 11);

        Assert.assertEquals("Pracovni balicek je prazdny.", 0, pack.size());
        Assert.assertFalse("Na prazdny pracovni balicek lze vlozit pouze krale.", pack.put(c1));
        Assert.assertTrue("Na prazdny pracovni balicek vkladame krale.", pack.put(c2));
        Assert.assertFalse("Na cerveneho krale lze vlozit pouze cernou damu.", pack.put(c3));
        Assert.assertEquals("Pracovni balicek obsahuje 1 kartu.", 1, pack.size());
        Assert.assertTrue("Na cerveneho krale vkladame cernou damu.", pack.put(c4));
        Assert.assertEquals("Pracovni balicek obsahuje 2 karty.", 2, pack.size());

        Assert.assertFalse("Na cernou damu lze vlozit pouze cerveneho kluka.", pack.put(c5));
        Assert.assertEquals("Pracovni balicek obsahuje 2 karty.", 2, pack.size());
        Assert.assertTrue("Na cernou damu vkladame cerveneho kluka.", pack.put(c6));
        Assert.assertEquals("Pracovni balicek obsahuje 3 karty.", 3, pack.size());
        
        CardStack s = pack.pop(factory.createCard(Card.Color.CLUBS, 12));
        Assert.assertEquals("Pracovni balicek obsahuje 1 kartu.", 1, pack.size());
        Assert.assertEquals("Pocet odebranych karet je 2.", 2, s.size());
        
        Assert.assertEquals("Na vrcholu je H(11).", factory.createCard(Card.Color.HEARTS, 11), s.pop());
        Assert.assertEquals("Na vrcholu je C(12).", factory.createCard(Card.Color.CLUBS, 12), s.pop());
        Assert.assertEquals("Odebrany balicek je prazdny.", 0, s.size());   
    }
    
    /**
     * Druhy test pracujiciho pole
     */
    @Test
    public void testWorkingPack2() {
        
        CardStack pack1 = factory.createWorkingPack();
        CardStack pack2 = factory.createWorkingPack();
           
        pack1.put(factory.createCard(Card.Color.DIAMONDS, 13));
        pack1.put(factory.createCard(Card.Color.CLUBS, 12));
        pack1.put(factory.createCard(Card.Color.HEARTS, 11));
     
        CardStack s = pack1.pop(factory.createCard(Card.Color.CLUBS, 12));
        
        Assert.assertTrue("Vkladame cerveneho krale na prazdny balicek.", 
                pack2.put(factory.createCard(Card.Color.HEARTS, 13)));

        Assert.assertTrue("Vkladame odebranou mnozinu.", pack2.put(s));
        
        Assert.assertEquals("Pracovni balicek c. 2 obsahuje 3 karty.", 3, pack2.size());
    }    
    
    /**
     * Test poctu generovanych karet ve tride CardBoard
     */
    @Test
    public void testCountsCardBoard() {
        
    	CardBoard board = new CardBoard();

        CardStack teststack0 = board.getStack(0);
        Assert.assertEquals("Pracovni balicek c. 0 obsahuje 1 karty.", 1, teststack0.size());
        Card c1 = teststack0.pop();
        Assert.assertTrue("Karta c1 ma byt otocena licem nahoru.", c1.isTurnedFaceUp());
        CardStack teststack1 = board.getStack(1);
        Assert.assertEquals("Pracovni balicek c. 0 obsahuje 2 karty.", 2, teststack1.size());
        CardStack teststack2 = board.getStack(2);
        Assert.assertEquals("Pracovni balicek c. 0 obsahuje 3 karty.", 3, teststack2.size());
        Card c2 = teststack2.pop();
        Assert.assertTrue("Karta c2 ma byt otocena licem nahoru.", c2.isTurnedFaceUp());
        Card c3 = teststack2.pop();
        Assert.assertTrue("Karta c3 ma byt otocena licem nahoru.", c3.isTurnedFaceUp());
    }
    
    /**
     * Test poctu generovanych karet ve tride CardBoard
     */
    @Test
    public void testSaveLoadGame() {
        
    	CardBoard board1 = new CardBoard();
        
    	board1.SaveGame("testsave.dat");
    	CardStack teststack1 = board1.getStack(0);
    	Card c1 = teststack1.pop();
    	
    	CardBoard board2 = new CardBoard();
    	board2.LoadGame("testsave.dat");
    	CardStack teststack2 = board2.getStack(0);
    	Card c2 = teststack2.pop();
    	
    	Assert.assertEquals("Shoda objektu.", c1, c2);
    }
    
    /**
     * Test na brzkou vyhru
     */
    @Test
    public void testSaveAndHint() {
        
    	CardBoard board = new CardBoard();
    	
        for (int i = 0; i < 7; i++)
        {
            for(int u = 0; u <= i; u++)
            {
            	board.getStack(i).pop();
            }
        }
        
        for (int i = 0; i < 7; i++)
        {
        	Assert.assertEquals("Pracovni balicek c. 0 obsahuje 0 karty.", 0, board.getStack(i).size());
        }
        
        for (int i = 0; i < 24; i++)
        {
        	board.getSourcePack().pop();
        }
        
        Assert.assertEquals("Pracovni balicek c. 0 obsahuje 0 karty.", 0, board.getSourcePack().size());
        
		for (int i = 1; i < 14; i++) board.getDeck(0).put(new Card(Card.Color.DIAMONDS, i));
		for (int i = 1; i < 14; i++) board.getDeck(1).put(new Card(Card.Color.HEARTS, i));
		for (int i = 1; i < 14; i++) board.getDeck(2).put(new Card(Card.Color.SPADES, i));
		for (int i = 1; i < 12; i++) board.getDeck(3).put(new Card(Card.Color.CLUBS, i));
		
        for (int i = 0; i < 3; i++)
        {
        	Assert.assertEquals("Pracovni balicek c. 0 obsahuje 0 karty.", 13, board.getDeck(i).size());
        }
        
        Assert.assertEquals("Pracovni balicek c. 0 obsahuje 0 karty.", 11, board.getDeck(3).size());
        
        board.getStack(2).InitPut(new Card(Card.Color.CLUBS, 12));
        board.getStack(5).InitPut(new Card(Card.Color.CLUBS, 13));
        
        board.getStack(2).getFromStack(0).turnFaceUp();
        board.getStack(5).getFromStack(0).turnFaceUp();
        
        board.SaveGame("AlmostWin");
        
        Assert.assertEquals("Pracovni balicek c. 0 obsahuje 0 karty.", 10, board.createHint(board.getStack(5).pop()));
        
        Assert.assertEquals("Pracovni balicek c. 0 obsahuje 0 karty.", 3, board.createHint(board.getStack(2).pop()));
    }
}
