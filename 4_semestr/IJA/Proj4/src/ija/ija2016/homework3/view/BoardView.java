/* 
 * BoardView: Hlavni component pro tvorbu boardu - jedna trida, jedna boarda v GUI
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */

package ija.ija2016.homework3.view;

import ija.ija2016.homework3.controller.CommandBuilder;
import ija.ija2016.homework3.model.cards.Card;
import ija.ija2016.homework3.model.cards.CardBoard;
import ija.ija2016.homework3.model.cards.CardBoardInterface;
import ija.ija2016.homework3.model.cards.CardDeck;
import ija.ija2016.homework3.model.cards.CardStack;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.MouseListener;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import ija.ija2016.homework3.model.cards.PleaseRepaint;
import java.awt.Font;
import java.util.ArrayList;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;


/**
 * Trida predstavujici jeden board v GUI
 * @author Holajz
 */
public class BoardView extends JPanel implements PleaseRepaint {
 
	private CardDeck selectedSourceDeck = null;
        private CardStack selectedSourceStack = null;
	private Card selectedMultipleCard = null;

        private CardView selectedSourceCard = null;

	private static final long serialVersionUID = 1L;

	private final CommandBuilder commander;
	private final CardBoardInterface cardBoard;
	private CardDecknSourceView mainCardPicker; //standard deck and source

	private ArrayList<CardDeckView> decks = new ArrayList<>(); //target pack
	private ArrayList<CardStackView> stacks = new ArrayList<>(); //working pack
        private CardView hint;

        private boolean hintNeeded = false;
        

        /**
         * Konstruktor teto boardy
         * @param newCardBoard model teto boardy
         */
	public BoardView(CardBoard newCardBoard) {
		commander = new CommandBuilder(newCardBoard);
		this.setLayout(null);
		cardBoard = newCardBoard;
		newCardBoard.registerObserver((PleaseRepaint)this);
                this.CreateAll();
	}

        /**
         * Vytvori a nabinduje vsechny komponenty teto boardy
         */
	private void CreateAll() {
                JButton buttonSave = new JButton("Save");
                buttonSave.setBounds(0, 0, 80, 25);
                buttonSave.setBackground(Color.ORANGE);
                buttonSave.setForeground(Color.BLACK);
                this.add(buttonSave);
                
		JButton buttonUndo = new JButton("Undo");
                buttonUndo.setBounds(80, 0, 80, 25);
                buttonUndo.setBackground(Color.ORANGE);
                buttonUndo.setForeground(Color.BLACK);
                this.add(buttonUndo);

                JButton buttonLoad = new JButton("Load");
                buttonLoad.setBounds(160, 0, 80, 25);
                buttonLoad.setBackground(Color.ORANGE);
                buttonLoad.setForeground(Color.BLACK);
                this.add(buttonLoad);
                
                JButton buttonClose = new JButton("Close");
                buttonClose.setBounds(240, 0, 80, 25);
                buttonClose.setBackground(Color.ORANGE);
                buttonClose.setForeground(Color.BLACK);
                this.add(buttonClose);
                
                JButton buttonHint;
                if(hintNeeded) {
                    buttonHint = new JButton("Hint On");
                }
                else {
                    buttonHint = new JButton("Hint Off");
                }
                buttonHint.setBounds(320, 0, 80, 25);
                buttonHint.setBackground(Color.ORANGE);
                buttonHint.setForeground(Color.BLACK);
                this.add(buttonHint);
                
                
                int basicValue = this.getHeight();
 

                
                int cardSpace = (int)(basicValue / 4.6);
                
                CardDecknSourceView packPicker = new CardDecknSourceView();
                packPicker.setModel(cardBoard.getSourcePack());
                packPicker.setXY(cardSpace * (1), 30);
                packPicker.setPanel(this);
                packPicker.paint();
                
                mainCardPicker = packPicker;
                this.stacks = new ArrayList<>();
                this.decks = new ArrayList<>();
                
                for(int i = 0; i < 7; i++) {
                    CardStackView stack = new CardStackView();
                    stack.setModel(cardBoard.getStack(i));
                    stack.setXY(cardSpace * (i+1), (int)(basicValue / 2.70 )  );
                    //System.out.println((cardSpace * (i+1)));
                    //System.out.println((basicValue / 2.70 ));
                    stack.setPanel(this);
                    stack.paint();
                    stacks.add(stack);
		}
                
                //System.out.println("");
                
                for(int i = 0; i < 4; i++) {
                    CardDeckView deck = new CardDeckView();
                    deck.setModel(cardBoard.getDeck(i));
                    deck.setXY(cardSpace * (i+4), 30);
                    //System.out.println((cardSpace * (i+4)));
                    deck.setPanel(this);
                    deck.paint();
                    decks.add(deck);
		}
                
                buttonSave.addActionListener((ActionEvent e) -> {
                    String fileName = JOptionPane.showInputDialog(null, "Vlozte jmeno souboru:", "Vlozte jmeno souboru", JOptionPane.WARNING_MESSAGE);
                    if(fileName.length() > 0){
                        getCommandBuilder().save("examples/" + fileName);
                    }
                });

                

                buttonHint.addActionListener((ActionEvent e) -> {
                    if(hintNeeded) {
                        hintNeeded = false;
                        removeHint();
                        buttonHint.setText("Hint Off");
                    }
                    else {
                        hintNeeded = true;
                        buttonHint.setText("Hint On");
                        setSelectedSource(selectedSourceDeck, selectedSourceStack, selectedSourceCard);
                    }
                });
                
		buttonUndo.addActionListener((ActionEvent e) -> {
                    //CommandInterface command = new CommandControl("undo");
                    //commander.execute(command);
                    getCommandBuilder().undo();
                    getCommandBuilder().Update();
                });
                
                buttonLoad.addActionListener((ActionEvent e) -> {
                    loadFile();
                });

                buttonClose.addActionListener((ActionEvent e) -> {
                    closeThisBoard();
                });
	}
        
        /**
         * Premaluje vsechny komponenty teto boardy
         */
        @Override
        public void repaint() {
            removeComponents();
            this.removeAll();
            
            if(this.cardBoard != null) {
                if(this.cardBoard.isGameOver()) {
                     CreateGameOver();
                }
                else {
                    this.CreateAll();
                }
            }
            super.repaint();
            this.revalidate();
        }
        
        /**
         * Vytvori obrazovku konce hry
         */
        public void CreateGameOver(){
            ImageIcon imageIcon = new ImageIcon(new ImageIcon(BoardView.class.getResource("/ija/textures/win.png"))
                .getImage().getScaledInstance(1920, 1080, Image.SCALE_SMOOTH));
            JLabel label = new JLabel(imageIcon);
            label.setBounds(0, 0, 1920, 1040);
            label.setFont(new Font("Serif", Font.PLAIN, 90));
            this.add(label);
            
            JButton buttonClose= new JButton("");
            buttonClose.setBounds(0, 0, 1920, 1040);
            buttonClose.setOpaque(false);
            buttonClose.setContentAreaFilled(false);
            buttonClose.setBorderPainted(false);
            this.add(buttonClose); 
            
            buttonClose.addActionListener((ActionEvent e) -> {
                    closeThisBoard();
            });
	}
        
        
        /**
         * Vytvori komponent pro vizualizaci zpracovani nacitani souboru
         */
        public void loadFile() {
            File[] FileList = new File("examples").listFiles((File dir, String filename) -> filename.endsWith(".XXX"));
                
		if(FileList == null) {
                    JOptionPane.showMessageDialog(null, "Neexistuji zadne ulozene hry, ujistete se, ze je vytvorena slozka examples s prilozenymi soubory.", "Chyba pri vyhledavani ulozenych her.", JOptionPane.INFORMATION_MESSAGE);
                    return;
		}
                
                String[] fileString = new String[FileList.length];
                
                int i = 0;
		for (File file : FileList) {
                   fileString[i] = file.getName().replaceAll("\\.XXX$", "");
                   i++;
		}		
		
                /*
                public static Object showInputDialog(Component parentComponent,
                     Object message,
                     String title,
                     int messageType,
                     Icon icon,
                     Object[] selectionValues,
                     Object initialSelectionValue)
                */
                
                Object[] fileNamesArray = new Object[fileString.length];
		fileNamesArray = (Object[])fileString;
                
		String fileToLoad = (String)JOptionPane.showInputDialog(
		                    this,
		                    "Vyber hru, kterou chces hrat",
		                    "Nacist hru",
		                    JOptionPane.PLAIN_MESSAGE,
		                    null,
		                    fileNamesArray,
		                    "loadDialog");

		if (fileToLoad != null) {
			commander.load("examples/" + fileToLoad);
			commander.Update();
		}
        }
        
        /**
         * Metoda oodstranujici board z GUI
         */
        public void closeThisBoard() {
            MainView mainWindow = (MainView)this.getTopLevelAncestor();
            mainWindow.removeBoard(this);
        }
        
        /**
         * Odstrani veskere komponenty z boardy
         */
        private void removeComponents() {
            Component [] components = this.getComponents();
            for(Component component: components) {
                MouseListener[] listeners = component.getMouseListeners();
                for(MouseListener listener: listeners) {
                    this.removeMouseListener(listener);
                }
            }
        }

        /**
         * Metoda ziskavajici CommandBuilder teto boardy
         * @return CommandBuilder teto boardy
         */
        public CommandBuilder getCommandBuilder() {
            return this.commander;
        }
        
        /**
         * Metoda ziskavajici model target balicku, ktery je momentalne vybran
         * @return target balicek nebo source balicek
         */
        public CardDeck getSelectedSourceDeck() {
            return this.selectedSourceDeck;
        }
        
        /**
         * Metoda ziskavajici model working balicku, ktery je momentalne vybran
         * @return working balicek
         */
        public CardStack getSelectedSourceStack() {
            return this.selectedSourceStack;
        }
        
        /**
         * Nastavi momentalne vybrany target balicek/source balicek nebo working balicek
         * @param deck - vybrany target balicek nebo source balicek
         * @param stack - vybrany working balicek 
         */
        public void setSelectedSource(CardDeck deck, CardStack stack) {
            this.selectedSourceDeck = deck;
            this.selectedSourceStack = stack;
        }
        
        /**
         * Nastavi momentalne vybrany balicek/source balicek nebo working balicek a CardView predstavujici vybranou kartu v techto baliccich
         * @param deck - vybrany target balicek nebo source balicek
         * @param stack - vybrany working balicek
         * @param sourceCard - vybrana karta
         */
        public void setSelectedSource(CardDeck deck, CardStack stack,  CardView sourceCard) {
            if(this.selectedSourceCard != null) {
                this.selectedSourceCard.setSelected(false);
            }
            this.selectedSourceStack = stack;
            this.selectedSourceDeck = deck;
            this.selectedSourceCard = sourceCard;
            this.setSelectedtMultipleMoveCard(null);
            
		if(sourceCard != null){
		sourceCard.setSelected(true);
                    if(this.hintNeeded) {
                        this.createHints();
                    }
		}
        }
        
        /**
         * Zrusi vyber balicku a karty
         */
        public void unselectSelectedSource(){
            this.setSelectedSource(null, null, null);
	}
        
        /**
         * Metoda pro zjisteni, zda je target/source balicek nebo working balicek vybrany
         * @return pravda , nepravda
         */
        public boolean isSourceDeckorStackSelected() {
            return this.selectedSourceDeck != null || this.selectedSourceStack != null;
        }
        
        /**
         * Metoda pro ziskani karty z balicek, ze ktereho se presunou karty az po tuto kartu
         * @return karta
         */
        public Card getSelectedtMultipleMoveCard(){
            return this.selectedMultipleCard;
	}
        
        /**
         * Nastavi kartu z balicku, ze ktereho se presunou karty az po tuto kartu
         * @param card - karta
         */
	public void setSelectedtMultipleMoveCard(Card card) {
            this.selectedMultipleCard = card;
	}
        
        /**
         * Metoda pro ziskani rady. Radu pro hrace ziskava z modulu, podle vracene hodnoty ziska mozny cil, vrchol je nasledne zvyraznen pro hrace.
         */
        public void createHints() {
            removeHint();
            
            int cardHint = this.cardBoard.createHint(this.selectedSourceCard.toCard());
        
            if(cardHint == -1)
            {
                //System.out.println("Nic jsem nenasel");
                return;
            }
        
            if(cardHint < 10)
            {
        	CardDeckView deck = this.decks.get(cardHint);
                //System.out.println("Nasel jsem neco v target packu cislo \\\\" + (hint+1) + "\\\\");
                CardView card = deck.top();
                this.hint = card;
                card.setHint(true);

                
            }
            else
            {
        	//hint = cislo workingpacku - 10
        	cardHint = cardHint - 10;
                CardStackView stack = this.stacks.get(cardHint);
                //System.out.println("Nasel jsem neco ve working packu cislo \\\\" + (hint+1) + "\\\\");
                CardView card = stack.top();
                this.hint = card;
                card.setHint(true);

                
            }
	}
        
        /**
         * Odstrani zvyrazneni karty. Invokace pri zmeneni karty, presunu nebo pri opetovnem stlaceni tlacitka hint.
         */
        private void removeHint() {
            if(this.hint != null) {
                this.hint.setHint(false);
            }
        }
        
        /**
         * Vykresli tento komponent
         * @param g - graficky context
         */
        @Override
        protected void paintComponent(final Graphics g) {
            super.paintComponent(g);
            Image image = null;
            try {
                URL url = LayoutVisualization.class.getResource("/ija/textures/background.png");
                image = ImageIO.read(url);
            } catch (IOException ex) {
                Logger.getLogger(BoardView.class.getName()).log(Level.SEVERE, null, ex);
            }
            g.drawImage(image, 0, 0, null);
        }

}
