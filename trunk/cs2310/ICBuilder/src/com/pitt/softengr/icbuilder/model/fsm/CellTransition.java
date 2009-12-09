package com.pitt.softengr.icbuilder.model.fsm;

import java.util.List;

import com.pitt.softengr.icbuilder.model.fsm.IndexCell;
import com.pitt.softengr.icbuilder.model.fsm.Message;

import com.pitt.softengr.icbuilder.util.fsm.CellTransitionType;


public class CellTransition {

    private String id;
    private IndexCell source;
    private IndexCell target;
    private CellTransitionType type;
    private Message msg;
    private List<Action> actions;
    
    public String getID(){
        return this.id;
    }
    public void setID(String id){
        this.id=id;
    }
    public IndexCell getSourceCell(){
        return this.source;
    }
    public void setSourceCell(IndexCell source){
        this.source=source;
    }
    public IndexCell getTargetCell(){
        return this.target;
    }
    public void setTargetCell(IndexCell target){
        this.target=target;
    }
    public CellTransitionType getTransitionType(){
        return this.type;
    }
    public void setTransitionType(CellTransitionType type){
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
