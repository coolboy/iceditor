package com.pitt.softengr.icbuilder.util.fsm;

import java.util.List;
import java.util.ArrayList;

import com.pitt.softengr.icbuilder.model.fsm.ICSystem;
import com.pitt.softengr.icbuilder.model.fsm.IndexCell;
import com.pitt.softengr.icbuilder.model.fsm.State;
import com.pitt.softengr.icbuilder.model.fsm.StateTransition;
import com.pitt.softengr.icbuilder.model.fsm.CellTransition;
import com.pitt.softengr.icbuilder.model.fsm.Message;
import com.pitt.softengr.icbuilder.model.ic.IcCard;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntry;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntryList;
import com.pitt.softengr.icbuilder.util.fsm.ICSystemModes;
import com.pitt.softengr.icbuilder.util.ic.StateType;

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
            List<State> states = buildStates(cards);
            // now create the transitions
            List<StateTransition> transistions = buildStateTransitions(cards,states);
            
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
            List<IcCardEntry> entries = cardSystem.getIcEntryList();
            
            system = new ICSystem();
            system.setID(rootEntry.getEntryId());
            system.setName(rootEntry.getEntryName());
            system.setDescription("n/a");
            
            List<IndexCell> cells = new ArrayList<IndexCell>();
            
            for(IcCardEntry entry : entries){
                for(IcCard card : entry.getIcCardList()){
                    IndexCell cell = new IndexCell();
                    cell.setID(card.getID());
                    cell.setMaxLifeTime("infinity");
                    cell.setName(card.getIcName());
                    cell.setCurrentState(null);
                    cell.setStartState(null);
                    cells.add(cell);
                }
            }
            system.addAllCell(cells);
            
            List<CellTransition> transitions = buildCellTransitions(cards,cells);
            system.addAllTransitions(transitions);
            
            
        } else if(mode.toLowerCase().equals(ICSystemModes.CUSTOM.toString().toLowerCase())){
            IcCardEntry rootEntry = cardSystem.getRootEntry();
            List<IcCardEntry> entries = cardSystem.getIcEntryList();
            
            system = new ICSystem();    
            system.setID(rootEntry.getEntryId());
            system.setName(rootEntry.getEntryName());
            system.setDescription("n/a");

            List<String> icIDs = cardSystem.getAllIndexIds("1");
            List<IndexCell> cells = new ArrayList<IndexCell>();
            System.out.println(icIDs.size());
            for(String cellId : icIDs){
                System.out.println(cellId);
                IndexCell cell = new IndexCell();
                cell.setID(cellId);
                cell.setMaxLifeTime("infinity");
                List<IcCard> stateCards = new ArrayList<IcCard>();
                for(IcCard card : cards){
                    if(cellId.toLowerCase().equals(card.getGroup().toLowerCase())){
                        stateCards.add(card);
                    }
                }
                List<State> states = buildStates(stateCards);
                List<StateTransition> trans = buildStateTransitions(stateCards,states);
                cell.addStates(states);
                cell.addAllTransitions(trans);
                cells.add(cell);
                System.out.println(cells);
            }
            system.addAllCell(cells);
            List<CellTransition> transitions = buildCellTransitions(cards,cells);
            System.out.println(transitions);
            system.addAllTransitions(transitions);
        } else {
            throw new RuntimeException("UNKNOW SYSTEM SCHEME \""+mode+"\" .... PLEASE CHECK CONFIGURATION FILE!!! ... ABORTING.");
        }
        return system;
    }
    private void buildSystems(){
        
    }
    private void buildSystem(){
        
    }
    private void buildCells(){
        
    }
    private void buildCell(IcCard card){
    }
 /*   private List<State> buildStates(List<IcCardEntry> entries){
        List<State> states = new ArrayList<State>();
        for(IcCardEntry entry : entries){
            for(IcCard card : entry.getIcCardList()){
                State state = buildState(card);
                if(state!=null){
                    states.add(state);
                }
            }
        }
        return states;
    }
 */
    private List<State> buildStates(List<IcCard> cards){
        List<State> states = new ArrayList<State>();
        for(IcCard card : cards){
            State state = buildState(card);
            if(state!=null){
                states.add(state);
            }
        }
        return states;
    }
    private State buildState(IcCard card){
        State state = null;
        if(card.isEndCard()){
            state = new State();
            state.setID(card.getID());
            state.setName(card.getIcName());
            state.setType(StateType.ENDING);
        } else if(card.isStartCard()){
            state = new State();
            state.setID(card.getID());
            state.setName(card.getIcName());
            state.setType(StateType.ENTERING);
        } else {
            state = new State();
            state.setID(card.getID());
            state.setName(card.getIcName());
            state.setType(StateType.INTERNAL);
        }
        return state;
    }
/*    private List<CellTransition> buildCellTransitions(List<IcCardEntry> entries, List<IndexCell> cells){
        List<CellTransition> transitions = new ArrayList<CellTransition>();
        for(IcCardEntry entry : entries){
            int countTrans=0;
            int countMsg=0;
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
                        msg.setID("msg"+countMsg);
                        trans.setID(""+countTrans);
                     
                        IndexCell target = findCellById(card.getIcOtherId(),cells);
                        trans.setTargetCell(target);
                        trans.setTransitionType("internal");
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
*/    
    private List<CellTransition> buildCellTransitions(List<IcCard> cards, List<IndexCell> cells){
        List<CellTransition> transitions = new ArrayList<CellTransition>();
        int countTrans=0;
        int countMsg=0;
        for(IcCard card : cards){
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
                    msg.setID("msg"+countMsg);
                    trans.setID(""+countTrans);
                 
                    IndexCell target = findCellById(card.getIcOtherId(),cells);
                    trans.setTargetCell(target);
                    trans.setTransitionType("internal");
                    msg.setName(card.getIcOtherTask());
                    msg.setMessageString(card.getIcOtherMessage());
                    trans.setTransitionMessage(msg);
                    transitions.add(trans);
                    countTrans++;
                    countMsg++;
                }
            }
        }
        return transitions;
    }

/*    private List<StateTransition> buildStateTransitions(List<IcCardEntry> entries, List<State> states){
        //the return list
        List<StateTransition> transitions = new ArrayList<StateTransition>();
        //now build all the transitions
        for(IcCardEntry entry : entries){
            int countTrans=0;
            int countMsg=0;
            for(IcCard card : entry.getIcCardList()){
                if(card.getIcOtherId()!=null && !card.getIcOtherId().isEmpty()){
                    //if this is the case transition is present
                    if(Integer.parseInt(card.getIcOtherId())>0){
                         StateTransition trans = new StateTransition();
                         State source = null;
                         if(card.getIcParentId().equals("-1")){
                             source = findStateById(card.getID(),states);
                         } else {
                             source  = findStateById(card.getIcParentId(),states);
                         }
                         trans.setSourceState(source);
                         Message msg = new Message();
                         msg.setID("msg"+countMsg);
                         trans.setID(""+countTrans);
                         State target=findStateById(card.getIcOtherId(),states);
                         trans.setTargetState(target);
                         trans.setTransitionType("internal");
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
*/
    private List<StateTransition> buildStateTransitions(List<IcCard> cards, List<State> states){
        //the return list
        List<StateTransition> transitions = new ArrayList<StateTransition>();
        //now build all the transitions
        int countTrans=0;
        int countMsg=0;
        for(IcCard card : cards){
            if(card.getIcOtherId()!=null && !card.getIcOtherId().isEmpty()){
                //if this is the case transition is present
                if(Integer.parseInt(card.getIcOtherId())>0){
                     StateTransition trans = new StateTransition();
                     State source = null;
                     if(card.getIcParentId().equals("-1")){
                         source = findStateById(card.getID(),states);
                     } else {
                         source  = findStateById(card.getIcParentId(),states);
                     }
                     trans.setSourceState(source);
                     Message msg = new Message();
                     msg.setID("msg"+countMsg);
                     trans.setID(""+countTrans);
                     State target=findStateById(card.getIcOtherId(),states);
                     trans.setTargetState(target);
                     trans.setTransitionType("internal");
                     msg.setName(card.getIcOtherTask());
                     msg.setMessageString(card.getIcOtherMessage());
                     trans.setTransitionMessage(msg);
                     transitions.add(trans);
                     countTrans++;
                     countMsg++;
                }
            }
        }
        return transitions;
    }
    private State findStateById(String ID, List<State> states){
        for(State state : states){
            if(state.getID().toLowerCase().equals(ID.toLowerCase())){
                return state;
            }
        }
        return null;
    }
    private IndexCell findCellById(String ID, List<IndexCell> cells){
        for(IndexCell cell : cells){
            if(cell.getID().toLowerCase().equals(ID.toLowerCase())){
                return cell;
            }
        }
        return null;
    }
}
