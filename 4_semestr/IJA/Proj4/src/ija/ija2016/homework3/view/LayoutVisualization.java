/* LayoutVisualization - ziskava textury a korektne meni jejich velikost
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */

package ija.ija2016.homework3.view;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

/**
 * Trida ziskavajici textury
 * @author Holajz
 */
public class LayoutVisualization {
    private BufferedImage stateHint;
    private BufferedImage stateHintSmall;
    private BufferedImage stateSelected;
    private BufferedImage stateSelectedSmall;
    
    static LayoutVisualization VIEW = null;
    private ArrayList<ImageIcon> cards = new ArrayList<>();
    private ArrayList<ImageIcon> smallCards = new ArrayList<>();
    private boolean useSmallCards = false;
    
    /**
     * konstruktor naplni sva pole texturami pro vyuziti v programu, ziskava dve verze, mensi a vetsi
     */
    LayoutVisualization() {
        ImageIcon imageEmpty = new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/empty.png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_SMOOTH));
        ImageIcon imageEmptySmall = new ImageIcon(imageEmpty.getImage().getScaledInstance(82, 160, Image.SCALE_SMOOTH));
        
        cards.add(imageEmpty);
        smallCards.add(imageEmptySmall);
        
        for(int i = 1; i <= 13; i++) {
            ImageIcon imageCard = new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/S" + i + ".png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_FAST));
            
            ImageIcon imageCardSmall = new ImageIcon(imageCard.getImage().getScaledInstance(82, 160, Image.SCALE_FAST));
            
            cards.add(imageCard);
            smallCards.add(imageCardSmall);
        }
        
        for(int i = 1; i <= 13; i++) {
            ImageIcon imageCard = new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/D" + i + ".png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_FAST));
            
            ImageIcon imageCardSmall = new ImageIcon(imageCard.getImage().getScaledInstance(82, 160, Image.SCALE_FAST));
            
            cards.add(imageCard);
            smallCards.add(imageCardSmall);
        }
        
        for(int i = 1; i <= 13; i++) {
            ImageIcon imageCard = new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/H" + i + ".png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_FAST));
            
            ImageIcon imageCardSmall = new ImageIcon(imageCard.getImage().getScaledInstance(82, 160, Image.SCALE_FAST));
            
            cards.add(imageCard);
            smallCards.add(imageCardSmall);
        }
        
        for(int i = 1; i <= 13; i++) {
            ImageIcon imageCard = new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/C" + i + ".png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_FAST));
            
            ImageIcon imageCardSmall = new ImageIcon(imageCard.getImage().getScaledInstance(82, 160, Image.SCALE_FAST));
            
            cards.add(imageCard);
            smallCards.add(imageCardSmall);
        }
        
        ImageIcon imageBack= new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/card_back.png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_SMOOTH));
        ImageIcon imageBackSmall = new ImageIcon(imageBack.getImage().getScaledInstance(82, 160, Image.SCALE_SMOOTH));
        
        cards.add(imageBack);
        smallCards.add(imageBackSmall);
        
        cards.add(imageEmpty);
        smallCards.add(imageEmptySmall);
        
        ImageIcon imageReasamble= new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/reassemble.png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_SMOOTH));
        ImageIcon imageReasambleSmall = new ImageIcon(imageReasamble.getImage().getScaledInstance(82, 160, Image.SCALE_SMOOTH));
        
        cards.add(imageReasamble);
        smallCards.add(imageReasambleSmall);
        
        ImageIcon imageNone_S= new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/Sempty.png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_SMOOTH));
        ImageIcon imageNone_S_Small = new ImageIcon(imageNone_S.getImage().getScaledInstance(82, 160, Image.SCALE_SMOOTH));
        
        cards.add(imageNone_S);
        smallCards.add(imageNone_S_Small);
        
        ImageIcon imageNone_D= new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/Dempty.png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_SMOOTH));
        ImageIcon imageNone_D_Small = new ImageIcon(imageNone_D.getImage().getScaledInstance(82, 160, Image.SCALE_SMOOTH));
        
        cards.add(imageNone_D);
        smallCards.add(imageNone_D_Small);
        
        ImageIcon imageNone_H= new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/Hempty.png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_SMOOTH));
        ImageIcon imageNone_H_Small = new ImageIcon(imageNone_H.getImage().getScaledInstance(82, 160, Image.SCALE_SMOOTH));
        
        cards.add(imageNone_H);
        smallCards.add(imageNone_H_Small);
        
        ImageIcon imageNone_C= new ImageIcon(new ImageIcon(LayoutVisualization.class.getResource("/ija/textures/Cempty.png"))
                .getImage().getScaledInstance(165, 320, Image.SCALE_SMOOTH));
        ImageIcon imageNone_C_Small = new ImageIcon(imageNone_C.getImage().getScaledInstance(82, 160, Image.SCALE_SMOOTH));
        
        cards.add(imageNone_C);
        smallCards.add(imageNone_C_Small);
        
        setState();
        
        
    }
    
    /**
     * vraci instanci teto tridy
     * @return instanci teto tridy
     */
    public static LayoutVisualization get() {
	if(VIEW == null) {
            VIEW = new LayoutVisualization();
        }
        return VIEW;
    }

    /**
     * Metoda nastavujici textury pri vyberu karty a cile rady
     */
    public void setState() {
        try {
            URL url = LayoutVisualization.class.getResource("/ija/textures/small_hint.png");
            this.stateHintSmall = ImageIO.read(url);
        }
        catch(IOException e) {
            this.stateHintSmall = new BufferedImage(150, 200, BufferedImage.TYPE_INT_ARGB);
        }
        
        try {
            URL url = LayoutVisualization.class.getResource("/ija/textures/small_selected.png");
            this.stateSelectedSmall = ImageIO.read(url);
        }
        catch(IOException e) {

            this.stateSelectedSmall = new BufferedImage(150, 200, BufferedImage.TYPE_INT_ARGB);
        }
        
        try {
            URL url = LayoutVisualization.class.getResource("/ija/textures/hint.png");
            this.stateHint = ImageIO.read(url);
        }
        catch(IOException e) {
            this.stateHint = new BufferedImage(150, 200, BufferedImage.TYPE_INT_ARGB);
        }
        
        try {
            URL url = LayoutVisualization.class.getResource("/ija/textures/selected.png");
            this.stateSelected = ImageIO.read(url);
        }
        catch(IOException e) {

            this.stateSelected = new BufferedImage(150, 200, BufferedImage.TYPE_INT_ARGB);
        }

        
    }
        
    /**
     * Metoda ziskavajici textury pri vyberu karty a cile rady
     * @param state stav karty, ktery se pozaduje
     * @return textura daneho stavu
     */
    public BufferedImage getState(CardView.CardViewState state) {
        if(isLayoutChanged()) {
            if(null == state) {
                return new BufferedImage(150, 200, BufferedImage.TYPE_INT_ARGB);
        }
        else switch (state) {
            case HINT_CARD:
                return this.stateHintSmall;
            case SELECTED_CARD:
                return this.stateSelectedSmall;
            default:
                return new BufferedImage(150, 200, BufferedImage.TYPE_INT_ARGB);
        }
            
        }
        if(null == state) {
            return new BufferedImage(150, 200, BufferedImage.TYPE_INT_ARGB);
        }
        else switch (state) {
            case HINT_CARD:
                return this.stateHint;
            case SELECTED_CARD:
                return this.stateSelected;
            default:
                return new BufferedImage(150, 200, BufferedImage.TYPE_INT_ARGB);
        }
    }
    
    /**
     * Metoda pro praci s polem textur
     * @param color - pozadovana barva karty
     * @param value - pozadovana hodnota karty
     * @return textura dane karty
     */
    public ImageIcon getCardImage(CardView.CardViewColor color, int value) {
        ArrayList<ImageIcon> card;
        if(this.useSmallCards) {
            card = this.smallCards;
        }
        else {
            card = this.cards;
        }
        
        switch(color) {
            case SPADES:
                return card.get(value);
            case DIAMONDS:
                return card.get(13 + value);
            case HEARTS:
                return card.get(26 + value);
            case CLUBS:
                return card.get(39 + value);
            case BACK:
                return card.get(52 + value + 1);
            case NONE:
                return card.get(52 + value + 2);
            case NEW_DECK:
                return card.get(52 + value + 3);
            case NONE_S:
                return card.get(52 + value + 4);
            case NONE_D:
                return card.get(52 + value + 5);
            case NONE_H:
                return card.get(52 + value + 6);
            case NONE_C:
                return card.get(52 + value + 7);
            default:
                return card.get(52 + value + 1);
            
        }
        
        
    }
    
    /**
     * nastavi pouzivani pole s malymi texturami, pri zmene pohledu
     * @param value - pouzivat nebo nepouzivat male textury
     */
    public void setUsingSmallCards(boolean value) {
        this.useSmallCards = value;
    }
    
    
    /**
     * Metoda tazajici se o pouzivani malych textur karet
     * @return - stav
     */
    public boolean isLayoutChanged() {
        return this.useSmallCards;
    }
    


}
