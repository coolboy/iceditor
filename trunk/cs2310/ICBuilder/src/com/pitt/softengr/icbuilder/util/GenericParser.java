package com.pitt.softengr.icbuilder.util;

import java.io.File;
import java.io.IOException;
import java.io.Reader;
import java.util.List;

import com.pitt.softengr.icbuilder.util.GenericParserImpl.RunMode;
import com.pitt.softengr.icbuilder.util.event.MessageEvent;

public interface GenericParser<E> {

    /**
     * Parse the file into object E. 
     * @param f The file to be parsed.
     * @return The object representation of the file's data.
     * @throws IOException from the underlying {@link Reader} implementation.
     * @throws IllegalArgumentException if the file is null or does not exist.
     */
    public E parse(File f) throws IOException, IllegalArgumentException;
    
    
    public void setRunMode(RunMode mode);
    
    /**
     * Returns a list of error messages gathered while parsing the export file.
     * @return A list of errors.
     */
    public List<MessageEvent> getErrorList();

    /**
     * Clears the list of error messages to free memory.
     */
    public void clearLog();
}
