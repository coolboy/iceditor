package com.pitt.softengr.icbuilder.model.ic;

import java.util.List;
import java.util.ArrayList;

public class IcCardEntry {
    
    private String entryId;
    private String entryName;
    private List<IcCard> icCards = new ArrayList<IcCard>();
    
    public String getEntryId(){
        return this.entryId;
    }
    public void setEntryId(String entryId){
        this.entryId=entryId;
    }
    public String getEntryName(){
        return this.entryName;
    }
    public void setEntryName(String entryName){
        this.entryName=entryName;
    }
/*
      public IcCard getParentIc(){
 
        return this.parent;
    }
    public void setParentIc(IcCard parent){
        this.parent=parent;
    }
    public IcCard getPartnerIc(){
        return this.partner;
    }
    public void setPartnerIc(IcCard partner){
        this.partner=partner;
    }
    public String getScenario(){
        return this.scenario;
    }
    public void setScenario(String scenario){
        this.scenario=scenario;
    }
*/
    public List<IcCard> getIcCardList(){
        return this.icCards;
    }
    public void addIcCardtoEntries(IcCard icCard){
        this.icCards.add(icCard);
    }
    public String toString(){
        return "ENTRY ID "+entryId+
               " ENTRY Name "+entryName;          
    }
}
