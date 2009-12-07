package com.pitt.softengr.icbuilder.model.fsm;

import java.util.List;
import java.util.ArrayList;

import com.pitt.softengr.icbuilder.model.fsm.State;
import com.pitt.softengr.icbuilder.model.fsm.StateTransition;
import com.pitt.softengr.icbuilder.model.fsm.Message;

public class IndexCell {
    private String id;
    //time in seconds? milliseconds?
    private String maxLifeTime;
    private String name;
    private State currentState;
    private State startState;
    private Message message;
    private List<State> states = new ArrayList<State>();
    private List<StateTransition> transitions = new ArrayList<StateTransition>();
    
    public String getID(){
        return this.id;
    }
    public void setID(String id){
        this.id=id;
    }
    public String getMaxLifeTime(){
        return this.maxLifeTime;
    }
    public void setMaxLifeTime(String maxLifeTime){
        this.maxLifeTime=maxLifeTime;
    }
    public String getName(){
        return this.name;
    }
    public void setName(String name){
        this.name=name;
    }
    public Message getMessage(){
        return this.message;
    }
    public void setMessage(Message message){
        this.message=message;
    }
    public State getCurrentState(){
        return this.currentState;
    }
    public void setCurrentState(State currentState){
        this.currentState=currentState;
    }
    public State getStartState(){
        return this.startState;
    }
    public void setStartState(State startState){
        this.startState=startState;
    }
    public void addState(State state){
        this.states.add(state);
    }
    public List<State> getStates(){
        return this.states;
    }
    public void addStates(List<State> states){
        if(states!=null && !states.isEmpty()){
            this.states.addAll(states);
        }
    }
    public State findStateById(String ID){
        for(State state : states){
            if(state.getID().toLowerCase().equals(ID.toLowerCase())){
                return state;
            }
        }
        return null;
    }
    public void addTransition(StateTransition transition){
        this.transitions.add(transition);
    }
    public void addAllTransitions(List<StateTransition> transitions){
        if(transitions!=null && !transitions.isEmpty()){
            this.transitions.addAll(transitions);
        }
    }
    public List<StateTransition> getTransitions(){
        return this.transitions;
    }
    public String toString(){
        return ("INDEX ID:"+this.id+" NAME:"+this.name+" LIFETIME:"+this.maxLifeTime+"\n STATES: "+this.states+"\n TRANSITIONS "+this.transitions);
    }
}
