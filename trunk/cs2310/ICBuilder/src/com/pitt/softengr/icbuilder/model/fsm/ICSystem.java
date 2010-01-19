package com.pitt.softengr.icbuilder.model.fsm;

import java.util.List;
import java.util.ArrayList;
import java.util.Collections;

import com.pitt.softengr.icbuilder.model.fsm.IndexCell;

public class ICSystem {
    private String id;
    private String name;
    private String description;
    private List<IndexCell> cells = new ArrayList<IndexCell>();
    private List<CellTransition> transitions = new ArrayList<CellTransition>();
    
    public String getID(){
        return this.id;
    }
    public void setID(String id){
        this.id=id;
    }
    public String getName(){
        return this.name;
    }
    public void setName(String name){
        this.name=name;
    }
    public String getDescription(){
        return this.description;
    }
    public void setDescription(String description){
        this.description=description;
    }
    public IndexCell getCellById(String id){
        for(IndexCell cell : cells){
            if(id.equals(cell.getID())){
                return cell;
            }
        }
        return null;
    }
    public IndexCell getCellByName(String name){
        for(IndexCell cell : cells){
            if(name.equals(cell.getName())){
                return cell;
            }
        }
        return null;
    }
    public List<IndexCell> getCells(){
        if(cells!=null && !cells.isEmpty()){
            return cells;
        }
        return null;
    }
    public void setCells(List<IndexCell> cells){
        if (cells!=null && !cells.isEmpty()){
            this.cells=cells;
        } else {
            this.cells=null;
        }
    }
    public void addCell(IndexCell cell){
        if(cell!=null){
            this.cells.add(cell);
        }
    }
    public void addAllCell(List<IndexCell> cells){
        if(cells!=null && !cells.isEmpty()){
            this.cells.addAll(cells);
        }
    }
    public void removeCell(IndexCell cell){
        if(cell!=null){
            for(IndexCell c : cells){
                if(cell.equals(c)){
                    cells.remove(cell);
                }
            }
        }
    }
    public List<CellTransition> getTransitions(){
        if(this.transitions!=null && !this.transitions.isEmpty()){
            return this.transitions;
        }
        return Collections.EMPTY_LIST;
    }
    public void addTransition(CellTransition transition){
        if(transition != null){
            this.transitions.add(transition);
        }
    }
    public void addAllTransitions(List<CellTransition> transitions){
        if(transitions!=null && !transitions.isEmpty()){
            this.transitions=transitions;
        } else {
            this.transitions=Collections.EMPTY_LIST;
        }
    }
}
