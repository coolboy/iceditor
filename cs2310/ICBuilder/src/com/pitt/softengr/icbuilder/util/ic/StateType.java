package com.pitt.softengr.icbuilder.util.ic;

public enum StateType {

        INTERNAL(0,"internal"),
        ENDING(1,"ending"),
        ENTERING(2,"entering");
        private int patternValue;
        
        private String patternName;
        
        StateType(int patternValue, String patternName){
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
