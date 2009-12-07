package com.pitt.softengr.icbuilder.util;

public enum InputType {

    ICE(0,"ice"),
    STANDARD(1,"standard");
    private int patternValue;
    private String patternName;
    
    InputType(int patternValue, String patternName){
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
