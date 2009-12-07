package com.pitt.softengr.icbuilder.model.fsm;

public class Action {

    private String id;
    private String name;
    private String body;
    private String target;

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
    public String getBody(){
        return this.body;
    }
    public void setBody(String body){
        this.body=body;
    }
}
