package com.pitt.softengr.icbuilder.util.ic;

import java.util.List;

import com.pitt.softengr.icbuilder.model.ic.IcCardEntry;
import com.pitt.softengr.icbuilder.model.ic.IcCard;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntryList;


/**
 * This class currently only merges the icdb and interactions xml information together.  
 * Essentially this class can be used for other merging needs for the index cards later down the road.
 * 
 * @author Lou Angelucci
 *
 */
public class Merger {

    /**
     * Returns an updated the  ICDBList with fields from the interactions list.  
     * This static class call searches based on iccard ID number.
     * @param ICDBList  The ic card list to be updated.
     * @param interactions The ic card list full of interaction information to be integrated.
     */
    public static IcCardEntryList cardAndInterations(IcCardEntryList ICDBList, List<IcCard> interactions){
        IcCardEntryList mergedEntryList = new IcCardEntryList();
        // nasty way of doing it i know but what else is there.
          for(IcCardEntry entry : ICDBList.getIcEntryList()){
              IcCardEntry mergedEntry = new IcCardEntry();
              mergedEntry.setEntryId(entry.getEntryId());
              mergedEntry.setEntryName(entry.getEntryName());
              for(IcCard card : entry.getIcCardList()){
                  for(IcCard interCard : interactions){
                    if(card.getID().equals(interCard.getID())){
                        IcCard mergedCard = new IcCard();
                        //trade the values of the interCard to regular ic list 
                        // this is a manual merge and seems to be the most effective way
                        mergedCard.setID(card.getID());
                        mergedCard.setIcName(card.getIcName());
                        mergedCard.setIcDescription(card.getIcDescription());
                        mergedCard.setTask(card.getIcTask());
                        mergedCard.setTimeCriticalCondition(card.getTimeCriticalCondition());
                        mergedCard.setIcPatternType(card.getIcPatternType());
                        mergedCard.setIcOtherId(card.getIcOtherId());
                        mergedCard.setIcOtherMessage(card.getIcOtherMessage());
                        mergedCard.setIcOtherTask(card.getIcOtherTask());
                        mergedCard.setIcParentId(interCard.getIcParentId());
                        mergedCard.setSenario(interCard.getSenario());
                        mergedCard.setSystem(interCard.getSystem());       
                        mergedCard.setGroup(interCard.getGroup());
                        mergedEntry.addIcCardtoEntries(mergedCard);
                    }
                }
            }
            mergedEntryList.addIcCardEntry(mergedEntry);
        }
        return(mergedEntryList);  
    }
}
