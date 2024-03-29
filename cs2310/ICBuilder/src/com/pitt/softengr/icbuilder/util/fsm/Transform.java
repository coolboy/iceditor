package com.pitt.softengr.icbuilder.util.fsm;

import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;

import com.pitt.softengr.icbuilder.model.fsm.ICSystem;
import com.pitt.softengr.icbuilder.model.fsm.IndexCell;
import com.pitt.softengr.icbuilder.model.fsm.State;
import com.pitt.softengr.icbuilder.model.fsm.StateTransition;
import com.pitt.softengr.icbuilder.model.fsm.CellTransition;
import com.pitt.softengr.icbuilder.model.fsm.Message;
import com.pitt.softengr.icbuilder.model.fsm.Parameter;
import com.pitt.softengr.icbuilder.model.ic.IcCard;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntry;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntryList;
import com.pitt.softengr.icbuilder.util.fsm.ICSystemModes;
import com.pitt.softengr.icbuilder.util.fsm.StateType;
import com.pitt.softengr.icbuilder.util.fsm.StateTransitionType;
import com.pitt.softengr.icbuilder.util.fsm.CellTransitionType;

/**
 * Currently this class has one purpose.  Which is to take that resulting merging of the icdb and interactions xml file
 * and provide a means to convert to a finite state machine.
 * @author Lou Angelucci
 *
 */
public class Transform {
    
    private IcCardEntryList cardSystem;
    
    public void setCardSystem(IcCardEntryList cardSystem){
        this.cardSystem=cardSystem;
    }
    /**
     * Returns an index cell converted from a card list database
     * @param cardSystem the card system to be converted
     * @param icIDs The ic card ids of the start of a new index cell.  In other words the card that
     *        is identified and all of it's children become their own index cell. 
     * @return The FSM index cell.
     */
    public ICSystem icSystemToIndexCellSystem(String mode){
        
        List<ICSystem> icSystems = new ArrayList<ICSystem>();
        ICSystem system = null;
        
        //list of all cards 
        List<IcCard> cards = new ArrayList<IcCard>();
        
        //list of all entries
        List<IcCardEntry> entries = new ArrayList<IcCardEntry>();
        
        //grab all entries
        entries.addAll(cardSystem.getIcEntryList());
        
        //grab all the cards
        for(IcCardEntry entry : cardSystem.getIcEntryList()){
            cards.addAll(entry.getIcCardList());
        }
                
        // if no default cards for index cells are set assume that the inputed cardSystem is one index cell
        if(mode.toLowerCase().equals(ICSystemModes.GROSS.toString().toLowerCase())){
            // make it all one system
            IcCardEntry rootEntry = cardSystem.getRootEntry();
            
            system = new ICSystem();
            system.setID(rootEntry.getEntryId());
            system.setName(rootEntry.getEntryName());
            system.setDescription("n/a");
            
            IndexCell cell = new IndexCell();
            cell.setID(rootEntry.getEntryId());
            cell.setName(rootEntry.getEntryName());
            cell.setMaxLifeTime("infinity");

 //           List<IcCardEntry> entries = cardSystem.getIcEntryList();
            // now create the states
            List<State> states = buildStates(entries);
            // now create the transitions
            List<StateTransition> transistions = buildStateTransitions(entries,states);
            
            cell.addStates(states);
            //set the start state and make it the current state
            for(IcCard card : rootEntry.getIcCardList()){
                State startState = findStateById(card.getID(),states);
                cell.setStartState(startState);
                cell.setCurrentState(startState);                    
            }
            cell.addAllTransitions(transistions);
            
            system.addCell(cell);
            icSystems.add(system);
            
        // if the length of the ics named is the same as the number of iccards then we must have a finegrained system    
        } else if(mode.toLowerCase().equals(ICSystemModes.FINE.toString().toLowerCase())){
            // make it all one system
            IcCardEntry rootEntry = cardSystem.getRootEntry();
            
            system = new ICSystem();
            system.setID(rootEntry.getEntryId());
            system.setName(rootEntry.getEntryName());
            system.setDescription("n/a");
            
            List<IndexCell> cells = new ArrayList<IndexCell>();
            
            for(IcCardEntry entry : entries){
                IndexCell cell = new IndexCell();
                cell.setID(entry.getEntryId());
                cell.setName(entry.getEntryName());
                cell.setMaxLifeTime("infinity");
                cell.setCurrentState(null);
                cell.setStartState(null);
                cells.add(cell);
                
            }
            system.addAllCell(cells);
            
            List<CellTransition> transitions = buildCellTransitions(entries,cells);
            system.addAllTransitions(transitions);
        } else if(mode.toLowerCase().equals(ICSystemModes.CUSTOM.toString().toLowerCase())){
            IcCardEntry rootEntry = cardSystem.getRootEntry();            

            system = new ICSystem();    
            system.setID(rootEntry.getEntryId());
            system.setName(rootEntry.getEntryName());
            system.setDescription("n/a");
            
            List<String> icIDs = cardSystem.getAllIndexIds();
            List<IndexCell> cells = new ArrayList<IndexCell>();
            for(String cellId : icIDs){
                IndexCell cell = new IndexCell();
                Set<IcCardEntry> temp = new HashSet<IcCardEntry>();
                for(IcCard card : cards){
                    if(cellId.toLowerCase().equals(card.getGroup().toLowerCase())){
                        IcCardEntry entry = cardSystem.findEntryCardByCardID(card.getID());
                        temp.add(entry);
                    }
                }
                List<IcCardEntry> stateEntries = new ArrayList<IcCardEntry>(temp);   
                List<State> states = buildStates(stateEntries);
                if(states.size()>1){
                    List<StateTransition> stateTransitions = buildStateTransitions(stateEntries,states);
                    cell.addAllTransitions(stateTransitions);
                }            
                cell.addStates(states);
                cell.setMaxLifeTime("infinity");
//                cell.setStartState(null);
                cell.setCurrentState(null);
                
//                cell.setID(cell.getStartState().getID());
//                cell.setName(cell.getStartState().getName());
                cell.setID(cellId);
                cells.add(cell);
            }
            List<CellTransition> cellTransitions = buildCellTransitions(entries,cells);
            system.addAllTransitions(cellTransitions);
            system.addAllCell(cells);
        } else {
            throw new RuntimeException("UNKNOW SYSTEM SCHEME \""+mode+"\" .... PLEASE CHECK CONFIGURATION FILE!!! ... ABORTING.");
        }
        return system;
    }
    private List<State> buildStates(List<IcCardEntry> stateEntries){
        List<State> states = new ArrayList<State>();
        for(IcCardEntry entry : stateEntries){
            State state = buildState(entry);
            states.add(state);
        }
        return states;
    }
    private State buildState(IcCardEntry entryCard){
        State state = new State();
        state.setID(entryCard.getEntryId());
        state.setName(entryCard.getEntryName());
        state.setType(StateType.internal);
        return state;
    }
    private List<CellTransition> buildCellTransitions(List<IcCardEntry> entries, List<IndexCell> cells){
        List<CellTransition> transitions = new ArrayList<CellTransition>();
        int countTrans=0;
        int countMsg=0;
        for(IcCardEntry entry : entries){
            for(IcCard card : entry.getIcCardList()){
                if(card.getIcOtherId()!=null && !card.getIcOtherId().isEmpty()){
                    if(Integer.parseInt(card.getIcOtherId())>0){
                        CellTransition trans = new CellTransition();
                        IndexCell source = null;
                        if(card.getIcParentId().equals("-1")){
                            source = findCellById(card.getID(),cells);
                        } else {
                            source = findCellById(card.getIcParentId(),cells);
                        }                       
                        
                        trans.setSourceCell(source);
                       
                        Message msg = new Message();
                        msg.setID(""+countMsg);
                        trans.setID(""+countTrans);
  
                        IndexCell target = findCellById(card.getIcOtherId(),cells);
                        trans.setTargetCell(target);
                        trans.setTransitionType(CellTransitionType.internal);
                        msg.setName(card.getIcOtherTask());
                        msg.setMessageString(card.getIcOtherMessage());
                        trans.setTransitionMessage(msg);
                        transitions.add(trans);
                        countTrans++;
                        countMsg++;
                    }
                }
            }
        }
        return transitions;
    }
    private List<StateTransition> buildStateTransitions(List<IcCardEntry> stateEntries, List<State> states){
        //the return list
        List<StateTransition> transitions = new ArrayList<StateTransition>();
        List<StateTransition> tempList = new ArrayList<StateTransition>();
        int countTrans=0;
        int countMsg=0;
        for(IcCardEntry entry : stateEntries){
            for(IcCard card : entry.getIcCardList()){
                if(!card.getIcOtherId().isEmpty() && !card.getIcParentId().isEmpty()){
                         StateTransition trans = new StateTransition();
                         
                         State source = null;
                         if(card.getIcParentId().equals("-1")){
                             source = findStateById(card.getID(),states);
                         } else {
                             source = findStateById(card.getIcParentId(),states);
                         }
                             
                         trans.setSourceState(source);
                         
                         Message msg = new Message();
                         msg.setID(""+countMsg);
                         trans.setID(""+countTrans);
                               
                         State target =  null;
                         if(card.getIcOtherId().equals("-1")){
                             target=findStateById(card.getID(),states);
                         } else {
                             target=findStateById(card.getIcOtherId(),states);
                         }
                         trans.setTargetState(target);
                         trans.setTransitionType(StateTransitionType.internal);
                         
                         msg.setName(card.getIcOtherTask());
                         msg.setMessageString(card.getIcOtherMessage());
                         msg.setType("input");
                         
                         Parameter par = new Parameter();
                         par.setDataValue(card.getTimeCriticalCondition());
                         par.setDataType("time");
                         par.setName("Tc");
                         
                         msg.addParameter(par);
                         trans.setTransitionMessage(msg);
                         
                         tempList.add(trans);
                         countTrans++;
                         countMsg++;
                    }
 //               }
            }
        }
        for(StateTransition trans : tempList){
//            System.out.println("TRANSITION ADDED "+trans);
//            if(trans.getSourceState()!=null && trans.getTargetState()!=null){
                transitions.add(trans);
//            }
       }
        return transitions;
    }
    private String findEntryIdByIcCardId(String ID){
        for(IcCardEntry entry : cardSystem.getIcEntryList()){
            for(IcCard card : entry.getIcCardList()){
                if(ID.equals(card.getID())){
                    return entry.getEntryId();
                }             
            }
        }
        return null;
    }
    private State findStateById(String ID, List<State> states){
//        String entryCardId = findEntryIdByIcCardId(ID);
        //if not found in the states it's not in this cell
//        if(!isStateInList(entryCardId,states)){
//            return null;
//        }
//        return getStateByEntryId(ID,states);
        for(State state : states){
            if((findEntryIdByIcCardId(ID)!=null) && isStateInList(findEntryIdByIcCardId(ID),states) && (state.getID().toLowerCase().equals(findEntryIdByIcCardId(ID).toLowerCase()))){
//                System.out.println("1 "+(findEntryIdByIcCardId(ID)!=null+"\n"+
//                        "2 "+isStateInList(findEntryIdByIcCardId(ID),states)+"\n"+
//                        "3 "+state.getID().toLowerCase().equals(findEntryIdByIcCardId(ID).toLowerCase()))+"\n"+
//                        "STATE "+state);
                return state;
            } else if((ID!=null) && isStateInList(ID,states) && (state.getID().toLowerCase().equals(ID.toLowerCase()))){
                return state;
            }
        }
        return null;
    }
    private boolean isStateInList(String ID, List<State> states){
        for(State s : states){
            if(ID.toLowerCase().equals(s.getID().toLowerCase())){
                return true;
            }
        }
        return false;
    }
    private State getStateByEntryId(String ID, List<State> states){
        for(State s : states){
            if(ID.toLowerCase().equals(s.getID().toLowerCase())){
                return s;
            }
        }
        return null;
    }
    private IndexCell findCellById(String ID, List<IndexCell> cells){
 //       System.out.println("ID "+ID+" ID FOUND "+(findEntryIdByIcCardId(ID)));
        for(IndexCell cell : cells){
  //          System.out.println("CELL "+cell.getID());
            if((findEntryIdByIcCardId(ID)!=null) && (cell.getID().toLowerCase().equals(findEntryIdByIcCardId(ID).toLowerCase()))){
                return cell;
            } else if((ID!=null) && (cell.getID().toLowerCase().equals(ID.toLowerCase()))){
                return cell;
            }
            for(State state : cell.getStates()){
                if((findEntryIdByIcCardId(ID)!=null) && (state.getID().toLowerCase().equals(findEntryIdByIcCardId(ID).toLowerCase()))){
                    return cell;
                } else if((ID!=null) && (state.getID().toLowerCase().equals(ID.toLowerCase()))){
                    return cell;
                }
            }
        }
        return null;
    }
}
