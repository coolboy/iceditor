package com.pitt.softengr.icbuilder.model.fsm;

import java.util.List;
import java.util.ArrayList;

import com.pitt.softengr.icbuilder.model.fsm.Parameter;

public class Message {

    private String type;
    private String id;
    private String name;
    private String msg;
//    private String predicate;
//    private String target;
//    private List<Parameter> parameters = new ArrayList<Parameter>();
    
    public String getType(){
        return this.type;
    }
    public void setType(String type){
        this.type=type;
    }
/*
      public String getPredicate(){
 
        return this.predicate;
    }
    public void setPredicate(String predicate){
        this.predicate=predicate;
    }
    public String getTarget(){
        return this.target;
    }
    public void setTarget(String target){
        this.target=target;
    }
*/
    public String getID(){
        return this.id;
    }
    public void setID(String id){
        this.id=id;
    }
    public String getName(){
        return this.name;
    }
    public void setName(String name){
        this.name=name;
    }
    public String getMessageString(){
        return this.msg;
    }
    public void setMessageString(String msg){
        this.msg=msg;
    }
/*
      public List<Parameter> getParameter(){
 
        return this.parameters;
    }
    public void setParameter(List<Parameter> parameters){
        this.parameters=parameters;
    }
    public boolean isInput(){
        if(target==null){
            return true;
        }
        return false;
    }
    public boolean isOutput(){
        if(predicate==null){
            return true;
        }
        return false;
    }
*/
    public String toString(){
        return ("\n MESSAGE NAME:"+this.name+" ID:"+this.id+" TYPE:"+this.type+" MSG:"+this.msg);
    }
}

