/* MainView - Hlavni GUI komponent, zozpovedny k vytvoreni boardy a zmeneni vizualizace
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */

package ija.ija2016.homework3.view;

import java.awt.EventQueue;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

import ija.ija2016.homework3.model.cards.CardBoard;
import java.awt.Color;
import static java.awt.Color.ORANGE;
import java.awt.Toolkit;
import java.io.IOException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.UnsupportedAudioFileException;

/**
 * Hlavni pohled, vytvari boardy a jejich usporadani
 * @author Holajz
 */
public class MainView extends JFrame{


	private static final long serialVersionUID = 1L;
	public static final int BOARD_LIMIT = 4;
	private final ArrayList<BoardView> boards = new ArrayList<>();
	private final JPanel mainPanel;
	private final GridLayout layoutFull;
	private final GridLayout layout4Tiles;
        boolean stopPlayback = false;
        public JButton stopButton;
        private static Clip clip;
        private static final ArrayList<String> songs = new ArrayList<>();
        private int songPlayed = 0;
	
        /**
         * Main metoda, vytvari hlavni pohled
         * @param args - argumenty prikazove radky
         */
	public static void main(String[] args) {
		EventQueue.invokeLater(() -> {
                    MainView frame = null;
                    try {
                        frame = new MainView();
                        frame.setVisible(true);
                        frame.addBoard();
                    } catch (LineUnavailableException | IOException | UnsupportedAudioFileException ex) {
                        Logger.getLogger(MainView.class.getName()).log(Level.SEVERE, null, ex);
                    }
                });
	}
	
        /**
         * Konstruktor vytvarejici hlavni pohled a jeji podrazene pohledy
         * @throws LineUnavailableException - muze vzniknout pri pousteni muziky
         * @throws IOException - muze vzniknout pri pousteni muziky
         * @throws UnsupportedAudioFileException - muze vzniknout pri pousteni muziky
         */
	public MainView() throws LineUnavailableException, IOException, UnsupportedAudioFileException {
		super("Medieval Klondike");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                setIcon();
		setBounds(100, 100, 1920, 1080);
		getContentPane().setLayout(null);
		
		layoutFull = new GridLayout(1,1,0,0);
		layout4Tiles = new GridLayout(2,2,0,0);
		
		JPanel topP = new JPanel(layoutFull);
		topP.setBounds(0, 0, 1920, 25);
		
		JPanel mainP = new JPanel(layoutFull);
		mainP.setBounds(0, 25, 1920, 1040);
		
		getContentPane().add(topP);
		getContentPane().add(mainP);
		
		this.mainPanel = mainP;
                mainPanel.setBackground(ORANGE);
                this.mainPanel.setVisible(true);
		
		JButton NewGameButton = new JButton("New Game");
                NewGameButton.setBounds(0, 0, 1000, 25);
                NewGameButton.setBackground(Color.ORANGE);
                NewGameButton.setForeground(Color.BLACK);
		topP.add(NewGameButton);
		
		NewGameButton.addActionListener((ActionEvent e) -> {
                    addBoard();
                });
                
                stopButton = new JButton("Mute Music");
                stopButton.setBounds(0, 0, 140, 25);
                stopButton.setBackground(Color.ORANGE);
                stopButton.setForeground(Color.BLACK);
		topP.add(stopButton);
                
                songs.add("/ija/music/m1.wav");
                songs.add("/ija/music/m2.wav");
                songs.add("/ija/music/m3.wav");
                music(songs.get(songPlayed));
                
                JButton ChangeSong = new JButton("Change Music");
                ChangeSong.setBounds(0, 0, 140, 25);
                ChangeSong.setBackground(Color.ORANGE);
                ChangeSong.setForeground(Color.BLACK);
		topP.add(ChangeSong);
		
		ChangeSong.addActionListener((ActionEvent e) -> {
                    if(stopButton.getText().equals("Mute Music")) {
                        songPlayed++;
                        if(songPlayed == 3) {
                            songPlayed = 0;
                        }
                        try {
                            clip.stop();
                            music(songs.get(songPlayed));
                        } catch (UnsupportedAudioFileException | IOException | LineUnavailableException ex) {
                            Logger.getLogger(MainView.class.getName()).log(Level.SEVERE, null, ex);
                        }
                    }
                });
                
                
		
		stopButton.addActionListener((ActionEvent e) -> {
                    String buttonText = (stopButton.getText());
                    
                    if(buttonText.equals("Unmute Music")) {
                        stopPlayback = false;
                        stopButton.setText("Mute Music");
                        clip.loop(Clip.LOOP_CONTINUOUSLY);
                    }
                    else {
                        stopPlayback = true;
                        stopButton.setText("Unmute Music");
                        clip.stop();
                        
                    }
                });
        }

        /**
         * Metoda pro vytvoreni boardy a pripadne zmeneni vizualizace 
         */
	public void addBoard() {
		if(boards.size() < BOARD_LIMIT) {
			if(boards.size() == 1) {
				this.changeView(layout4Tiles);
			}
		}
		if(boards.size() == BOARD_LIMIT) {
                    return;
                }
		//creates template board
		CardBoard cardBoard = new CardBoard();
		//creates view of the board
		BoardView boardView = new BoardView(cardBoard);
		//adds to view
		this.mainPanel.add(boardView);
		//adds to boards view list
		boards.add(boardView);
		
		this.DoRepainting();
        }
	
        /**
         * Metoda pro odstraneni boardy a pripadne zmeneni vizualizace
         * @param board - 
         */
	public void removeBoard(BoardView board) {
		if(boards.size() == 2) {
			//changes to full layout
			this.changeView(layoutFull);
		}
		
		//remove from the boards view list
		boards.remove(board);
		//remove from the view
		board.removeAll();
		this.mainPanel.remove(board);
		
		this.DoRepainting();
	}
	
	/**
         * prekresleni vsech komponentu
         */
	public void DoRepainting() {
		//repaint GUI
		this.repaint();
		this.revalidate();
		
	}
        
        /**
         * Metoda starajici se o zmenu zobrazeni, invokuje LayoutVisualization
         * @param layout - usporadani hraciho pole
         */
	public void changeView(GridLayout layout) {
		this.mainPanel.setLayout(layout);
		
		LayoutVisualization.get().setUsingSmallCards((layout == this.layout4Tiles));
                this.DoRepainting();
	}
        
        /**
         * Metoda spoustejici muziku
         * @param file - soubor pro spusteni
         * @throws UnsupportedAudioFileException - vznika pri spusteni muziky
         * @throws IOException - vznika pri spusteni muziky
         * @throws LineUnavailableException - vznika pri spusteni muziky
         */
        public static void music(String file) throws UnsupportedAudioFileException, IOException, LineUnavailableException{
        
        URL url = MainView.class.getResource(file);
        
        clip = AudioSystem.getClip();
        AudioInputStream inputStream = AudioSystem.getAudioInputStream(url);
        clip.open(inputStream);
        clip.loop(Clip.LOOP_CONTINUOUSLY); 
    

        }

        /**
         * nastaveni ikony hlavniho pohledu
         */
        private void setIcon() {
            URL url = MainView.class.getResource("/ija/textures/icon.png");
            setIconImage(Toolkit.getDefaultToolkit().getImage(url));
        }
    
}
