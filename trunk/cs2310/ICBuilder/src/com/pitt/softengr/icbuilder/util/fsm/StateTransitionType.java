package com.pitt.softengr.icbuilder.util.fsm;

public enum StateTransitionType {
    internal(0,"internal"),
    boundary(1,"boundary");
    private int patternValue;
    
    private String patternName;
    
    StateTransitionType(int patternValue, String patternName){
        this.patternValue=patternValue;
        this.patternName=patternName;
    }
    public int getPatternValue(){
        return this.patternValue;
    }
    public String getPatternName(){
        return this.patternName;
    }
    public void setPatternValue(int patternValue){
        this.patternValue=patternValue;
    }
    public void setPatternName(String patternName){
        this.patternName=patternName;
    }
}
