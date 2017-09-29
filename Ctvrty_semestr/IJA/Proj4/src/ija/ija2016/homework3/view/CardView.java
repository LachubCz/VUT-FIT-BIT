/* CardView - zpracovava pohled karty
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */

package ija.ija2016.homework3.view;


import ija.ija2016.homework3.model.cards.Card;
import java.awt.AlphaComposite;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.image.RenderedImage;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.SwingConstants;

/**
 * Trida reprezentujici jednu kartu v GUI
 * @author Holajz
 */
public class CardView extends JLabel{
    
    private Card.Color color;
    int value;
    private boolean isSelected;
    private boolean isHint;
    private int x;
    private int y;
    
    /**
     * Kontruktor pro vytvoreni pohledu karty 
     * @param cardColor - barva karty
     * @param cardValue - hodnota karty
     * @param x - pozice x v GUI karty
     * @param y - pozice y v GUI karty
     */
    CardView(CardView.CardViewColor cardColor, int cardValue, int x, int y) {
        this.color = CardViewColor.toColor(cardColor);
        this.value = cardValue;
        this.x = x;
        this.y = y;
        
        ImageIcon icon = LayoutVisualization.get().getCardImage(cardColor, cardValue);
        this.setIcon(icon);
        this.setHorizontalAlignment(SwingConstants.CENTER);
        this.setBounds(x, y, icon.getIconWidth(), icon.getIconHeight());
    }

        /**
         * Mozne barvy pohledu karty
         */
        public static enum CardViewColor {
            SPADES("S"), DIAMONDS("D"), HEARTS("H"), CLUBS("C"), BACK("BACK"), NONE("NONE"), NEW_DECK("NEWDECK"),
            NONE_S("Sempty"), NONE_D("Dempty"), NONE_H("Hempty"), NONE_C("Cempty");
            
            
            private final String cardValue;
            
            private CardViewColor(String value) {
                this.cardValue = value;
            }
            
            public static CardView.CardViewColor ColorView(Card.Color color) {
                    switch(color) {
                        case SPADES:
                            return SPADES;
                        case DIAMONDS:
                            return DIAMONDS;
                        case HEARTS:
                            return HEARTS;
                        case CLUBS:
                            return CLUBS;
                    }
                    return SPADES;
            }
            
            public static Card.Color toColor(CardView.CardViewColor color) {
                    switch(color) {
                        case SPADES:
                            return Card.Color.SPADES;
                        case DIAMONDS:
                            return Card.Color.DIAMONDS;
                        case HEARTS:
                            return Card.Color.HEARTS;
                        case CLUBS:
                            return Card.Color.CLUBS;
                    }
                    return Card.Color.SPADES;
            }
            
        }
        
        /**
         * Mozne stavy karty
         */
        public static enum CardViewState {
		NONE("NONE"),
		SELECTED_CARD("SELECTED CARD"),
		HINT_CARD("HINT TARGET"),
                NONE_S("Sempty"),
                NONE_D("Dempty"),
                NONE_H("Hempty"),
                NONE_C("Cempty");

		private final String cardValue;

		private CardViewState(String value){
			this.cardValue = value;
		}
	}
        
        /**
         * Nastavi tuto kartu jako vybranou, nebo naopak zrusi vyber
         * @param isSelected - invokace vybrani
         */
        public void setSelected(boolean isSelected) {
            this.isSelected = isSelected;
            this.repaint();
        }
        
        /**
         * Konvertuje tuto kartu do modelu Card
         * @return vraci konvertovanou kartu
         */
        public Card toCard() {
            return new Card(this.color, this.value);
        }
        
        /**
         * Nastavi tuto kartu jako cil rady, nebo naopak zrusi cil
         * @param isHinterino - 
         */
        public void setHint(boolean isHinterino) {
            this.isHint = isHinterino;
            this.repaint();
        }
        
        /**
         * Metoda ziskavajici stav rady karty
         * @return je target rady nebo ne
         */
        public boolean getHint() {
            return this.isHint;
        }
        
        /**
         * Metoda ziskavajici stav vybrani karty
         * @return je vybrana nebo ne
         */
        public boolean getSelected() {
            return this.isSelected;
        }
        
        /**
         * Metoda pro vykresleni karty
         * @param g - graficky kontext
         */
        @Override
        public void paintComponent(final Graphics g) {
		super.paintComponent(g);
                
                Graphics2D g2 = (Graphics2D)g;
                BufferedImage im = null;
                
		if(this.getSelected()){
                    im =  LayoutVisualization.get().getState(CardView.CardViewState.SELECTED_CARD);
		}
                if(this.getHint()) {
                    im = LayoutVisualization.get().getState(CardView.CardViewState.HINT_CARD);
                }
                if(this.getSelected() || this.getHint()) {
                    g2.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, 0.6f));
                    g2.drawRenderedImage((RenderedImage)im, AffineTransform.getTranslateInstance(0, 0));
                    g2.dispose();
                }
        }
        
        
    
    
    
}
