package com.pitt.softengr.icbuilder.model.fsm;

import java.util.List;

import com.pitt.softengr.icbuilder.model.fsm.StateTransition;
import com.pitt.softengr.icbuilder.util.fsm.StateType;

public class State {
 
    private String id;
    private StateType type;
    private String name;
    private List<StateTransition> transitionsIn;
    private List<StateTransition> transitionsOut;
    
    public String getID(){
        return this.id;
    }
    public void setID(String id){
        this.id=id;
    }
    public StateType getType(){
        return this.type;
    }
    public void setType(StateType type){
        this.type=type;
    }
    public String getName(){
        return this.name;
    }
    public void setName(String name){
        this.name=name;
    }
    public List<StateTransition> getTransitionsIn(){
        return this.transitionsIn;
    }
    public void setTransitionsIn(List<StateTransition> transitionsIn){
        this.transitionsIn=transitionsIn;
    }
    public List<StateTransition> getTransitionsOut(){
        return this.transitionsOut;
    }
    public void setTransitionsOut(List<StateTransition> transitionsOut){
        this.transitionsOut=transitionsOut;
    }
    public boolean isEndState(){
        if(StateType.ending.equals(this.type)){
            return true;
        }
        return false;
    }
    public boolean isStartState(){
        if(StateType.entering.equals(this.type)){
            return true;
        }
        return false;
    }
    public String toString(){
//        return ("STATE ID:"+id+" TYPE:"+type+" NAME:"+name+"\n TRANSITIONS IN:\n"+transitionsIn+"\n TRANSITIONS OUT:\n"+transitionsOut+"\n");
        return ("\nSTATE ID:"+id+" TYPE:"+type+" NAME:"+name);
    }
}
