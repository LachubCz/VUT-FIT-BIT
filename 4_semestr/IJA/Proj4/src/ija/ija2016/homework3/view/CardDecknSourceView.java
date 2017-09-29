/* CardDecknSourceView - Zpracovava pohled source a standard balicku 
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.view;

import ija.ija2016.homework3.controller.CommandInterface;
import ija.ija2016.homework3.controller.CommandNext;
import ija.ija2016.homework3.controller.CommandRenew;
import ija.ija2016.homework3.model.cards.Card;
import ija.ija2016.homework3.model.cards.CardStack;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

/**
 * Trida reprezentujici a zpracujici pohled source a standard balicku
 * @author Holajz
 */
class CardDecknSourceView {
    public int x,y;
    public BoardView board;
    public String name;
    
    private CardStack pack; //source pack
    private CardView picker;
    private CardView putAside;
    
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
     * @param newDeck 
     */
    public void setModel(CardStack sourcePack) {
        this.pack = sourcePack;
    }
    
    /**
     * Funkce zpracujici vykresleni a zpracovani eventu od uzivatele
     */
    public void paint() {
        if(!pack.isEmpty()) {
            CardView card = new CardView(CardView.CardViewColor.BACK, 0, 0, y  );
            board.add(card);
            picker = card;	
			
            card.addMouseListener(new MouseAdapter() {  
             public void mouseReleased(MouseEvent e) {  
                    CommandInterface command = new CommandNext(pack);
                    board.getCommandBuilder().execute(command);
		}  
            }); 
        }
        else {
            CardView card = new CardView(CardView.CardViewColor.NEW_DECK, 0, 0, y  );
            board.add(card);
            picker = card;
            
            card.addMouseListener(new MouseAdapter() {  
            public void mouseReleased(MouseEvent e) {  
                    CommandInterface command = new CommandRenew(pack);
                    board.getCommandBuilder().execute(command);
		}  
            }); 
        }
        
        if(!pack.isSourceEmpty()) {
            Card modelCard = pack.top();
            CardView card = new CardView(CardView.CardViewColor.ColorView(modelCard.color()), modelCard.value(), x, y  );
            board.add(card);
            putAside = card; 
			
            card.addMouseListener(new MouseAdapter() {  
		public void mouseReleased(MouseEvent e) {  
                    board.setSelectedSource(pack.getDeck(), null, card);
		}  
            }); 
        }
        else {
            CardView card = new CardView(CardView.CardViewColor.NONE, 0, x, y  );
            board.add(card);
            putAside = card;
        }
        
    }


    
}
