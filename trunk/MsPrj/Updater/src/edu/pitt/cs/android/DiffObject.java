/**
 * 
 */
package edu.pitt.cs.android;

import java.io.Serializable;

/**
 * @author Cool
 * 
 * this class stands for an different object in two zip file
 * A -> B
 * old -> new
 */
public class DiffObject implements Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = -5218339389948393467L;

	enum State {
		Add, //this file is new in B 
		Delete, //this file is missing in B
		Modify, //this file has been changed
		Same, //this file stays the same
		UnDef
	}
	
	public String path = null;//the path inside the zip file
	public State state = State.UnDef;//the diff state
	
	//1. the diff data for modify
	//2. the whole data for newly added
	public Object binary = null;
}
