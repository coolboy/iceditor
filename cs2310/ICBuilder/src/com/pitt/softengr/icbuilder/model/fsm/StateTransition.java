package com.pitt.softengr.icbuilder.model.fsm;

import java.util.List;

import com.pitt.softengr.icbuilder.model.fsm.State;
import com.pitt.softengr.icbuilder.model.fsm.Message;

import com.pitt.softengr.icbuilder.util.fsm.StateTransitionType;

public class StateTransition {

    private String id;
    private State source;
    private State target;
    private StateTransitionType type;
    private Message msg;
    private List<Action> actions;
    
    public String getID(){
        return this.id;
    }
    public void setID(String id){
        this.id=id;
    }
    public State getSourceState(){
        return this.source;
    }
    public void setSourceState(State source){
        this.source=source;
    }
    public State getTargetState(){
        return this.target;
    }
    public void setTargetState(State target){
        this.target=target;
    }
    public StateTransitionType getTransitionType(){
        return this.type;
    }
    public void setTransitionType(StateTransitionType type){
        this.type=type;
    }
    public Message getTransitionMessage(){
        return this.msg;
    }
    public void setTransitionMessage(Message msg){
        this.msg=msg;
    }
    public List<Action> getActions(){
        return this.actions;
    }
    public void addAction(Action action){
        if(action!=null){
            this.actions.add(action);
        }
    }
    public String toString(){
        return (" \nTRANSITION ID:"+this.getID()+"\n SOURCE:"+this.source+"\n TARGET:"+this.target+" TYPE:"+this.type+" MESSAGE:"+this.msg+"\n");
    }
}
