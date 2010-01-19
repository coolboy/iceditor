package com.pitt.softengr.icbuilder.output.fsm;

import java.io.IOException;

import java.util.List;
import com.pitt.softengr.icbuilder.model.fsm.ICSystem;
import com.pitt.softengr.icbuilder.model.fsm.IndexCell;
import com.pitt.softengr.icbuilder.model.fsm.State;
import com.pitt.softengr.icbuilder.model.fsm.StateTransition;
import com.pitt.softengr.icbuilder.model.fsm.CellTransition;
import com.pitt.softengr.icbuilder.model.fsm.Message;
import com.pitt.softengr.icbuilder.model.fsm.Parameter;

import com.pitt.softengr.icbuilder.util.CommonWriter;

public class IndexCellSystemXmlWriter extends CommonWriter<List<ICSystem>> {
    
    public void writeHeader() throws IOException{
        // no header needed at this point
    }
    public void writeContent() throws IOException{

        List<ICSystem> systemList = this.getData();
        for(ICSystem system : systemList){
            writeSystem(system);
            for(IndexCell cell : system.getCells()){        
                writeCell(cell);
                for(State state : cell.getStates()){
                    writeState(state);
                }
                for(StateTransition transition : cell.getTransitions()){
                    writeStateTransition(transition);
                    writeMessage(transition.getTransitionMessage());
                    writeln("\t\t</transition>");
                }
                writeln("\t</indexCell>");
            }
            for(CellTransition transition : system.getTransitions()){
                writeCellTransition(transition);
                writeln("\t</transition>");
            }
            writeln("</indexSystem>");
        }
    }
    private void writeSystem(ICSystem system) throws IOException{
        write("<indexSystem id=\""+system.getID()+"\"");
        write(" name=\""+system.getName()+"\"");
        write(" description=\""+system.getDescription()+"\"");
        writeln(">");
    }
    private void writeCell(IndexCell cell) throws IOException{
        write("\t<indexCell ");
        write("id=\""+cell.getID()+"\" ");
        write("maxLifeTime=\""+cell.getMaxLifeTime()+"\" ");
        write("name=\""+cell.getName()+"\"");
        writeln(">");

    }
    private void writeState(State state) throws IOException{
        write("\t\t<state ");
        write("id=\""+state.getID()+"\" ");
        write("type=\""+state.getType()+"\" ");
        write("name=\""+state.getName()+"\" ");
        writeln("/>");
    }
    private void writeStateTransition(StateTransition transition) throws IOException{
        write("\t\t<transition ");
        write("id=\""+transition.getID()+"\" ");
        write("type=\""+transition.getTransitionType()+"\" ");
        write("source=\""+transition.getSourceState().getID()+"\" ");
        write("target=\""+transition.getTargetState().getID()+"\" ");
        writeln(">");
    }
    private void writeCellTransition(CellTransition transition) throws IOException{
        write("\t<transition ");
        write("id=\""+transition.getID()+"\" ");
        write("type=\""+transition.getTransitionType()+"\" ");
        write("source=\""+transition.getSourceCell().getID()+"\" ");
        write("target=\""+transition.getTargetCell().getID()+"\" ");
        writeln(">");        
    }
    private void writeMessage(Message msg) throws IOException{
        write("\t\t\t<message ");
        write("type=\""+msg.getType()+"\" ");
        write("id=\""+msg.getID()+"\" ");
        write("name=\""+msg.getName()+"\" ");
        writeln(">");
        if(msg.getTarget()!=null){
            writeln("<targetIC>"+msg.getTarget()+"</targetIC>");
        }
        for(Parameter param : msg.getParameter()){
            write("\t\t\t\t<parameter ");
            write("dataType=\""+param.getDataType()+"\" ");
            write("dataValue=\""+param.getDataValue()+"\" ");
            write("name=\""+param.getName()+"\" ");
            writeln("/>");
        }
        writeln("\t\t\t</message>");
    }
}
