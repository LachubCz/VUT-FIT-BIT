/* CardDeckView - Zpracovava pohled target balicku
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */

package ija.ija2016.homework3.view;

import ija.ija2016.homework3.controller.CommandInterface;
import ija.ija2016.homework3.controller.CommandMove;
import ija.ija2016.homework3.model.cards.Card;
import ija.ija2016.homework3.model.cards.CardDeck;
import ija.ija2016.homework3.model.cards.CardStack;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

/**
 * Trida reprezentujici pohled target balicku
 * @author Holajz
 */
public class CardDeckView {

    public int x,y;
    public BoardView board;
    private CardView card;
    CardDeck deck;
    
    /**
     * Metoda nastavujici pozice x a y v GUI
     * @param newX - pozice x
     * @param newY - pozice y
     */
    public void setXY(int newX, int newY) {
	x = newX;
	y = newY;
    }
    
    /**
     * Metoda nastavujici hlavni pohled
     * @param newBoard - boarda
     */
    public void setPanel(BoardView newBoard) {
	board = newBoard;
    }

    /**
     * Funkce vracejici vrchol target balicku
     * @return pohled karty
     */
    public CardView top() {
        return this.card;
    }
    
    /**
     * Funkce nastavujici target CardDeck model tohoto pohledu
     * @param newDeck - model pod timto pohledem, zpracujici logiku
     */
    public void setModel(CardDeck newDeck) {
        deck = newDeck;
    }

    /**
     * Funkce zpracujici vykresleni a zpracovani eventu od uzivatele
     */
    void paint() {
        CardView cardView;
        Card stackCard = deck.top();

        CardView card;
        if(stackCard != null) {
            this.card = new CardView(CardView.CardViewColor.ColorView(stackCard.color()), stackCard.value(), x, y );
            card = this.card;
        }
        else {
            if(deck.getDeckColor() == Card.Color.CLUBS) {
                this.card = new CardView(CardView.CardViewColor.NONE_C, 0, x, y );
            }
            else if(deck.getDeckColor() == Card.Color.SPADES) {
                this.card = new CardView(CardView.CardViewColor.NONE_S, 0, x, y );
            }
            else if(deck.getDeckColor() == Card.Color.HEARTS) {
                this.card = new CardView(CardView.CardViewColor.NONE_H, 0, x, y );
            }
            else if(deck.getDeckColor() == Card.Color.DIAMONDS) {
                this.card = new CardView(CardView.CardViewColor.NONE_D, 0, x, y );
            }
            card = this.card;
        }

	board.add(this.card);	
			
	card.addMouseListener(new MouseAdapter() {  
		public void mouseReleased(MouseEvent e)  
		    {  
			if(board.isSourceDeckorStackSelected()){
                            CommandInterface command;
                            CardDeck source = board.getSelectedSourceDeck();
                            if(source == null) {
                                CardStack sourceStack = board.getSelectedSourceStack();
                                command = new CommandMove(sourceStack, deck);
                            }
                            else {
                                command = new CommandMove(source, deck);
                            }
                            if(command.canExecute()) {
                                board.getCommandBuilder().execute(command);
                                board.unselectSelectedSource();
                            }
                            else if(!deck.isEmpty())  {
                                board.setSelectedSource(deck, null, card);
                            }
			}		
			else if(!deck.isEmpty()) {
                            board.setSelectedSource(deck, null, card);
                        }
		    }
		}); 
    }

}
