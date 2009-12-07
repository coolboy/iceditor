package com.pitt.softengr.icbuilder.util.event;

import java.util.EventObject;

/**
 * Common superclass for events.
 * 
 * @author Lou Angelucci
 *
 */

public abstract class BuilderEvent extends EventObject {
    
    private static final long serialVersionUID = -271124196950429989L;
    
    public BuilderEvent(Object source){
        super(source);
    }

}
