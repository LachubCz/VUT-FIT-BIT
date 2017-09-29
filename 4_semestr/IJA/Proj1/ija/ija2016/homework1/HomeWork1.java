/*
 * Testovaci trida pro prvni ukol z predmetu IJA.
 */
package ija.ija2016.homework1;

import ija.ija2016.homework1.cardpack.Card;
import ija.ija2016.homework1.cardpack.CardDeck;
import ija.ija2016.homework1.cardpack.CardStack;
import java.util.HashSet;
import java.util.Set;

/**
 *
 * @author koci
 */
public class HomeWork1 {

    /**
     * Spusti testy.
     * @param args Argumenty pøi spuštìní - nejsou využity.
     */
    public static void main(String[] args) {
        boolean assertion = false;
        assert assertion = true;
        if (! assertion) {
            System.out.println("Spustte s volbou -ea");
            return;
        }
        testCard();
        testCardDeck();
        testCardStack();
        System.out.println("OK");
    }

    /**
     * Test vytvoreni karty.
     */
    private static void testCard() {
        Card c1 = new Card(Card.Color.CLUBS, 9);
        Card c2 = new Card(Card.Color.HEARTS, 11);
        Card c3 = new Card(Card.Color.CLUBS, 9);

        assert c1.color().equals(Card.Color.CLUBS) : "Test barvy karty";
        assert Card.Color.HEARTS.toString().equals("H") : "Test textove reprezentace barvy";
        assert c2.value() == 11 : "Test hodnoty karty";
        
        assert c1.toString().equals("9(C)") : "Retezec reprezentujici kartu 9(C)";
        assert c2.toString().equals("J(H)") : "Retezec reprezentujici kartu J(H)";
    }

    /**
     * Test vytvoreni balicku karet.
     */
    private static void testCardDeck() {
        CardDeck deck = CardDeck.createStandardDeck();
        
        assert deck.size() == 52 : "Pocet karet je 52";
        
        Set<Card> testCards = new HashSet<>();
        for (int i = 1; i <= 13; i++) { testCards.add(new Card(Card.Color.CLUBS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(new Card(Card.Color.DIAMONDS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(new Card(Card.Color.HEARTS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(new Card(Card.Color.SPADES,i)); }
        
        for (int i = 0; i < 52; i++) {
            Card c = deck.pop();
            assert testCards.remove(c) : "Spravna karta";
        }
        
        assert testCards.isEmpty() : "Testova sada musi byt prazdna";       
    }

    /**
     * Test vytvoreni zasobniku karet.
     */
    private static void testCardStack() {
        CardStack stack1 = new CardStack(5);
        CardStack stack2 = new CardStack(3);
        CardStack stack3;
        
        stack1.put(new Card(Card.Color.CLUBS,2));
        stack1.put(new Card(Card.Color.CLUBS,11));
        stack1.put(new Card(Card.Color.DIAMONDS,5));
        stack1.put(new Card(Card.Color.HEARTS,7));
        stack1.put(new Card(Card.Color.SPADES,9));

        stack2.put(new Card(Card.Color.DIAMONDS,5));
        stack2.put(new Card(Card.Color.HEARTS,7));
        stack2.put(new Card(Card.Color.SPADES,9));

        stack3 = stack1.takeFrom(new Card(Card.Color.DIAMONDS,5));
        
        assert stack1.size() == 2 : "Pocet karet na zasobniku je 2";
        assert stack3.size() == 3 : "Pocet odebranych karet je 3";
        assert stack2.equals(stack3) : "Zasobniky musi byt shodne";
    }
}
