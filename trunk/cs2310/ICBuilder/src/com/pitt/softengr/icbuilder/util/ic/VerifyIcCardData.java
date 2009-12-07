package com.pitt.softengr.icbuilder.util.ic;

import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.log4j.Logger;

import com.pitt.softengr.icbuilder.model.ic.IcCardEntryList;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntry;
import com.pitt.softengr.icbuilder.model.ic.IcCard;

public class VerifyIcCardData {
    
    private static final Logger logger = Logger.getLogger(VerifyIcCardData.class);
    
    // set up a unified input to the verifier to handle multiple ways of verification
    public void process(IcCardEntryList entryList){
        verifyInputs(entryList);
    }
    private void verifyInputs(IcCardEntryList entryList){
        if(entryList==null){
            return;
        }
        for(IcCardEntry entry : entryList.getIcEntryList()){
            validateEntry(entry);
            for(IcCard card : entry.getIcCardList()){
                validateCard(card);
            }
        }
    }
    private void validateEntry(IcCardEntry entry){
        if(entry.getEntryId()==null || entry.getEntryId().isEmpty()){
            String entryID=constructConsoleMsg("Missing ID for card entry "+entry.getEntryName()+" Please enter ID: ");
            if(entryID!=null){
                entry.setEntryId(entryID);
            }
        } else if(entry.getEntryName()==null || entry.getEntryName().isEmpty()){
            String entryName=constructConsoleMsg("Missing NAME for card entry "+entry.getEntryId()+" Please enter Name: ");
            if(entryName!=null){
                entry.setEntryId(entryName);
            }
        }
    }
    private void validateCard(IcCard card){
        card.getIcOtherId();
        if(card.getID()==null || card.getID().isEmpty()||card.getID().equals("")){
            String cardID=constructConsoleMsg("Missing ID for card "+card.getIcName()+" Please enter ID: ");
            if(cardID!=null){
                card.setID(cardID);
            }            
        } else if((card.getIcParentId()==null || card.getIcParentId().isEmpty())||card.getIcParentId().equals("")){
            String parentID=constructConsoleMsg("Missing PARENT ID for card entry "+card.getIcName()+" Please enter PARENT ID: ");
            if(parentID!=null){
                card.setIcParentId(parentID);
            }   
        }
/*        } else if((card.getIcOtherId()!=null && !card.getIcOtherId().isEmpty())){
           
            if((Integer.parseInt(card.getIcOtherId())>0)&&(card.getIcOtherMessage()==null && card.getIcOtherMessage().isEmpty() && card.getIcOtherMessage().equals(""))){
                String otherMessage=constructConsoleMsg("Missing OTHER MESSAGE for card ID: "+card.getID()+" card NAME: "+card.getIcName()+" Please enter OTHER MESSAGE: ");
                if(otherMessage!=null){
                    card.setIcOtherMessage(otherMessage);
                }            
            } else if((Integer.parseInt(card.getIcOtherId())<0)&&(card.getIcOtherMessage()!=null && !card.getIcOtherMessage().isEmpty() && !card.getIcOtherMessage().equals(""))){
                String otherID=constructConsoleMsg("Missing OTHER ID for card ID: "+card.getID()+" card NAME: "+card.getIcName()+" Please enter ID: ");
                if(otherID!=null){
                    card.setIcOtherId(otherID);
                }            
            } 

        } else if((card.getIcOtherId()==null && card.getIcOtherId().isEmpty())&&(card.getIcOtherMessage()!=null && !card.getIcOtherMessage().isEmpty() && !card.getIcOtherMessage().equals(""))){
            String otherID=constructConsoleMsg("Missing OTHER ID for card ID: "+card.getID()+" card NAME: "+card.getIcName()+" Please enter ID: ");
            if(otherID!=null){
                card.setIcOtherId(otherID);
            }            
        }
 */
    }
    private String constructConsoleMsg(String msg){
        
        System.out.println(msg);
        Console console = System.console();
        String userInput = null;
        if (console == null) {
            // Console is null in a "headless" JVM, which happens in eclipse...
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            try {
                userInput = br.readLine();
            } catch (IOException e){
                throw new RuntimeException(e);
            }
        } else {
            userInput = String.valueOf(console.readPassword());
        }
        return userInput;
    }
}
