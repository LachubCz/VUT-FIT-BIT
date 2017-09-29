/*
 * CommandInterface: Rozhrani definujici metody pro jednotlive prikazy
 * @author Petr Buchal, xbucha02
 * @author Tomas Holik, xholik13
 * @version 1.0
 * Project: Medieval Klondike
 * University: Brno University of Technology
 * Course: IJA
 */
package ija.ija2016.homework3.controller;

import java.io.Serializable;

public interface CommandInterface extends Serializable
{	
	/**
	 * [metoda vykona prikaz]
	 */
	public void execute();
	
	/**
	 * [metoda pro vraceni do stavu pred provedenim prikazu]
	 */
	public void unexecute();
	
	/**
	 * [metoda zjisti zda-li jde prikaz vykonat]
	 * @return [vraci true pokud lze prikaz provest]
	 */
	public boolean canExecute();
}
