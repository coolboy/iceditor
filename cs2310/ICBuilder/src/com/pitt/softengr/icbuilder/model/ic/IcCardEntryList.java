package com.pitt.softengr.icbuilder.model.ic;

import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;


public class IcCardEntryList {

        private List<IcCardEntry> icEntryList = new ArrayList<IcCardEntry>();
        
        public IcCardEntryList(){
            
        }
        public IcCardEntryList(IcCardEntryList list){
            icEntryList.addAll(list.getIcEntryList());
        }
        public List<IcCardEntry> getIcEntryList(){
            return icEntryList;
        }
        public void addIcCardEntry(IcCardEntry icEntry){
            this.icEntryList.add(icEntry);
        }
        public void addAllIcCardEntry(List<IcCardEntry> icEntries){
            this.icEntryList.addAll(icEntries);
        }
        public String toString(){
            String temp="";
            for(IcCardEntry entry : icEntryList){
                if(entry.getEntryId()!=null)
                    temp+=entry+"\n";
                for(IcCard card : entry.getIcCardList()){
//                    if(card.getIcName()!=null){
                        temp+=card+"\n";
//                    }
                }
            }
            return temp;
        }
        public int size(){
            return icEntryList.size();
        }
        /**
         * Finds the root entry of the IC Card System
         * @return Returns the root card of the system.
         */
        public IcCardEntry getRootEntry(){
            // look for the root card which is the only card that should have a value of -1
            for(IcCardEntry entry : icEntryList){
                for(IcCard card : entry.getIcCardList()){
                    int parentId = Integer.parseInt(card.getIcParentId());
                    //this is assuming all the cards in the entry list are parent < 0 denoting the root of the IcCard System.
                    if(parentId<0){
                        return entry;
                    }
                }
            }
            return null;
        }
        /**
         * Finds all ending cards.  If none are found returns null;
         * @return Null : no end cards were found.  Otherwise a list of end cards is returned.
         */
        public List<IcCard> getEndCards(){
            List<IcCard> endCards = new ArrayList<IcCard>();
            for(IcCardEntry entry : icEntryList){
                for(IcCard card : entry.getIcCardList()){
                    int otherId = Integer.parseInt(card.getIcOtherId());
                    if(otherId<0){
                        endCards.add(card);
                    }
                }
            }
            return endCards;
        }
        public IcCardEntry findEntryCardByEntryID(String ID){
            for(IcCardEntry entry : icEntryList){
                if(ID.equals(entry.getEntryId())){
                    return entry;
                }
            }
            return null;
        }
        public IcCardEntry findEntryCardByCardID(String ID){
            for(IcCardEntry entry : icEntryList){
                for(IcCard card : entry.getIcCardList()){
                    if(ID.equals(card.getID())){
                        return entry;
                    }
                }
            }
            return null;
        }
        public IcCard findCardByID(IcCard cardFind){
            for(IcCardEntry entry : icEntryList){
                for(IcCard card : entry.getIcCardList()){
                    if(cardFind.getID().equals(card.getID())){
                        return card;
                    }
                }
            }
            return null;
        }
        public List<String> getAllIndexIds(String system){
            Set<String> ids = new HashSet<String>();
            for(IcCardEntry entry : icEntryList){
                for(IcCard card : entry.getIcCardList()){
                    System.out.println(card.getGroup());
                    ids.add(card.getGroup());
                }
            }
            return new ArrayList<String>(ids);
        }
}
