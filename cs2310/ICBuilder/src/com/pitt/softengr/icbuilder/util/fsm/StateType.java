package com.pitt.softengr.icbuilder.util.fsm;

public enum StateType {

        internal(0,"internal"),
        ending(1,"ending"),
        entering(2,"entering");
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
