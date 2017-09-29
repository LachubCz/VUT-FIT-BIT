/* CardStackView - zpracovava pohled working balicku
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
import ija.ija2016.homework3.controller.CommandMoveMultiple;
import ija.ija2016.homework3.model.cards.Card;
import ija.ija2016.homework3.model.cards.CardDeck;
import ija.ija2016.homework3.model.cards.CardStack;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

/**
 * Trida reprezentujici a zpracujici pohled working balicku
 * @author Holajz
 */
public class CardStackView {
    public int x;
    public int y;
    BoardView board;
    CardStack stack;
    CardView topCard;

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
     * Funkce nastavujici CardStack model tohoto pohledu
     * @param newStack - nastavi model working balicku
     */
    public void setModel(CardStack newStack) {
        stack = newStack;
    }
    
     /**
     * Funkce vracejici vrchol working balicku
     * @return pohled karty
     */
    public CardView top() {
        return this.topCard;
    }

    void paint() {
        int distance;
        if(LayoutVisualization.get().isLayoutChanged()) {
            distance = 20;
        }
        else {
            distance = 40;
        }
        
        
        for(int index = stack.size() - 1; index >= 0; index--) {
            Card card = stack.getFromStack(index);
            CardView cardView;
            
            if(card.isTurnedFaceUp()) {
                cardView = new CardView(CardView.CardViewColor.ColorView(card.color()), card.value(), x, y + distance * index);
            }
            else {
                cardView = new CardView(CardView.CardViewColor.BACK, 0, x, y + distance * index);
            }
            board.add(cardView);
            
            if(index == stack.size() - 1 ) {
                //if last card in the stack, create move event
		cardView.addMouseListener(new MouseAdapter() {  
                    @Override
                    public void mouseReleased(MouseEvent e) {  
                            moveStackorSelect(cardView);
                    }  
		});
            this.topCard = cardView;
            }
            else {
                if(card.isTurnedFaceUp()) {
                    cardView.addMouseListener(new MouseAdapter() {  
                        @Override
                        public void mouseReleased(MouseEvent e) {  
                            board.setSelectedSource(null, stack, cardView);
                            board.setSelectedtMultipleMoveCard(card);
                        }  
                    });
                }
            }
        }
        
        if(stack.size() == 0) {
            CardView card = new CardView(CardView.CardViewColor.NONE, 0, x, y);
            board.add(card);
            
            card.addMouseListener(new MouseAdapter() {
                public void mouseReleased(MouseEvent e) {
                    if(board.isSourceDeckorStackSelected()) {
                        moveStackorSelect(card);
                    }
                }
            });
            this.topCard = card;
        }

    }
    
    public void moveStackorSelect(CardView card) {
        CommandInterface command = null;
        if(board.isSourceDeckorStackSelected()) {
            if(board.getSelectedtMultipleMoveCard() == null) {
                CardDeck source = board.getSelectedSourceDeck();
                if(source == null) {
                    CardStack sourceStack = board.getSelectedSourceStack();
                    command = new CommandMove(sourceStack, stack);
                }
                else {
                    command = new CommandMove(source, stack);
                }
                board.unselectSelectedSource();
            }
            else {
                CardStack source = board.getSelectedSourceStack();
                if(source == null) {
                
                }
                else {
                    command = new CommandMoveMultiple(source, stack, board.getSelectedtMultipleMoveCard());
                }
            board.unselectSelectedSource();
            }
            
            if(command != null) {
                if(command.canExecute()) {
                    board.getCommandBuilder().execute(command);
                }
            }
            else {
                board.setSelectedSource(null, stack, card);
            }
        }
        else {
            board.setSelectedSource(null, stack, card);
        }

        
    }
}
