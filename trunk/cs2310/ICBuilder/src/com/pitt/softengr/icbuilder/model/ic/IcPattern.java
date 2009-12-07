package com.pitt.softengr.icbuilder.model.ic;

public enum IcPattern {

        QUIETSTATE(0,"quietstate"),
        MYSELFNONE(1,"myselfnone"),
        MYSELFWITH(2,"myselfwith"),
        OTHERSNONE(3,"othersnone"),
        OTHERSWITH(4,"otherswith"),
        MIXEDSTATE(5,"mixedstate");
        
        private int patternValue;
        
        private String patternName;
        
        IcPattern(int patternValue, String patternName){
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
