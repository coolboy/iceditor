package com.pitt.softengr.icbuilder.util.fsm;

public enum ICSystemModes {

    FINE(0,"fine"),
    GROSS(1,"gross"),
    CUSTOM(2,"custom");
    
    private int patternValue;
    private String patternName;
    
    ICSystemModes(int patternValue, String patternName){
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
