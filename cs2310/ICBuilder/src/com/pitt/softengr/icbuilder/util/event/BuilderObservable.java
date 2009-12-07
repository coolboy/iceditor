package com.pitt.softengr.icbuilder.util.event;

import java.util.Observable;

import com.pitt.softengr.icbuilder.util.event.MessageEvent.Level;
import com.pitt.softengr.icbuilder.util.event.MessageEvent;
import com.pitt.softengr.icbuilder.util.event.BuilderListener;
/**
 * Provides basic methods to alert ProgressListeners of a ProgressEvent.
 * 
 * @author Lou Angelucci
 */
public abstract class BuilderObservable extends Observable {

    protected void fireBuilderEvent(BuilderEvent event){
        // Mark that a change has occurred
        this.setChanged();
        // Alert observers of the new event.
        this.notifyObservers(event);
    }
    /**
     * Utility method for firing message events.
     * @param level The level of severity.
     * @param msg The message contents.
     */
    protected void fireMessageEvent(String messageId, Level level, String msg) {
        this.fireBuilderEvent(new MessageEvent(this, messageId, level, msg));
    }

    /**
     * Adds the progress listener as an observer of ProgressEvent events.
     * @param l The progress listener to add.
     */
    public void addProgressListener(BuilderListener l) {
        this.addObserver(l);
    }
    
    /**
     * Removes a listener.
     * @param l The progress listener to remove.
     */
    public void removeProgressListener(BuilderListener l) {
        this.deleteObserver(l);
    }
}
