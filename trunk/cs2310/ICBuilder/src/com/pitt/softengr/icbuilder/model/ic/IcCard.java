package com.pitt.softengr.icbuilder.model.ic;

/**
 * A simple class to define an iccard
 * @author Lou Angelucci
 *
 */
public class IcCard {

    private String id;
    private String name;
    private String otherName;
    private String description;
    private String pattern;
    private String task;
    private String otherTask;
    private String timeCriticalCondition;
    private String icParentId;
    private String icOtherId;
    private String icOtherMessage;
    private String senario;
    private String system;
    private String group;
    private String entryId;
    private String entryName;
        
    public String getID(){
        return this.id;
    }
    public void setID(String id){
        this.id=id;
    }
    public String getIcDescription(){
        return this.description;
    }
    public void setIcDescription(String description){
        this.description=description;
    }
    public String getIcName(){
        return this.name;
    }
    public void setIcName(String name){
        this.name=name;
    }
    public String getOtherIcName(){
        return this.otherName;
    }
    public void setOtherIcName(String otherName){
        this.otherName=otherName;
    }
    public String getIcPatternType(){
        return this.pattern;
    }
    public void setIcPatternType(String pattern){
        this.pattern=pattern;
    }
    public String getIcTask(){
        return this.task;
    }
    public void setTask(String task){
        this.task=task;
    }
    public String getIcOtherTask(){
        return this.otherTask;
    }
    public void setIcOtherTask(String otherTask){
        this.otherTask=otherTask;
    }
    public String getTimeCriticalCondition(){
        return this.timeCriticalCondition;
    }
    public void setTimeCriticalCondition(String timeCriticalCondition){
        this.timeCriticalCondition=timeCriticalCondition;
    }
    public String getIcParentId(){
        return this.icParentId;
    }
    public void setIcParentId(String icParentId){
        this.icParentId = icParentId;
    }
    public String getIcOtherId(){
        return this.icOtherId;
    }
    public void setIcOtherId(String icOtherId){
        this.icOtherId=icOtherId;
    }
    public String getIcOtherMessage(){
        return this.icOtherMessage;
    }
    public void setIcOtherMessage(String icOtherMessage){
        this.icOtherMessage = icOtherMessage;
    }
    public String getSenario(){
        return this.senario;
    }
    public void setSenario(String senario){
        this.senario=senario;
    }
    public String getSystem(){
        return this.system;
    }
    public void setSystem(String system){
        this.system=system;
    }
    public String getGroup(){
        return this.group;
    }
    public void setGroup(String group){
        this.group=group;
    }
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
    public boolean isEndCard(){
        int otherId = Integer.parseInt(this.getIcOtherId());
        if(otherId<0){
            return true;
        }
        return false;
    }
    public boolean isStartCard(){
        int parentId = Integer.parseInt(this.getIcParentId());
        if(parentId<0){
            return true;
        }
        return false;
    }
    public String toString(){
         return "IC ID : "+id+
                " IC NAME : "+name+
                " IC PARENT : "+icParentId+
                " IC DESCRITPTION : "+description+
                " IC PATTERN : "+pattern+
                " MY TASK : "+task+
                " TCC : "+timeCriticalCondition+
                " IC OTHER ID : "+icOtherId+
                " IC OTHER NAME : "+otherName+
                " IC OTHER TASK : "+otherTask+
                " IC OTHER MSG : "+icOtherMessage+
                " IC SENARIO : "+senario+
                " IC GROUP : "+group+
                " IC SYSTEM : "+system;
    }
}
