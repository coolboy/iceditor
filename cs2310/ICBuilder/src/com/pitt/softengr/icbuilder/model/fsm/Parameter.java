package com.pitt.softengr.icbuilder.model.fsm;

import com.pitt.softengr.icbuilder.model.fsm.IndexCell;

public class Parameter {
    
    private String dataType;
    private String dataValue;
    private String name;
    private IndexCell targetIC;
    
    public String getDataType(){
        return this.dataType;
    }
    public void setDataType(String dataType){
        this.dataType=dataType;
    }
    public String getDataValue(){
        return this.dataValue;
    }
    public void setDataValue(String dataValue){
        this.dataValue=dataValue;
    }
    public String getName(){
        return this.name;
    }
    public void setName(String name){
        this.name=name;
    }
    public IndexCell getTargeIC(){
        return this.targetIC;
    }
    public void setTargetIC(IndexCell targetIC){
        this.targetIC=targetIC;
    }
}
