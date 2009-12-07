package com.pitt.softengr.icbuilder.util;

import java.io.File;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.FileReader;
import java.util.List;
import java.util.ArrayList;

import com.pitt.softengr.icbuilder.util.event.BuilderObservable;
import com.pitt.softengr.icbuilder.util.event.MessageEvent;
import com.pitt.softengr.icbuilder.util.event.MessageEvent.Level;

/**
 * Contains functionality that is common to most parser implementations.
 * 
 * @author Lou Angelucci
 */
public abstract class GenericParserImpl<E> extends BuilderObservable implements GenericParser<E> {
    
    /** The file being parsed. */
    private File file;
    
    /** Input stream access. */
    private BufferedReader reader;
    
    /** Current line number, used primarily for error reporting. */ 
    private int lineCounter;
    
    private int errorCount = 0;
    
    private List<MessageEvent> errorList = new ArrayList<MessageEvent>();
    
    /**
     * Defines the modes of operation for the parser.
     * 
     * @author Lou Angelucci
     *
     */
    public static enum RunMode {
        
        /**
         * This mode will suppress all warning and error messages. These 
         * messages are still accessible after parsing is complete with
         * the {@link GenericParserImpl#getErrorList()} method.
         */
        SILENT,
        
        /**
         * This mode will propagate all warnings and errors through the
         * event handlers.
         */
        VERBOSE,
        
        /**
         * This mode will cause an exception to be thrown on the first error.
         */
        FAILFAST;
    }
    
    //Initialize the run mode as silent
    private RunMode runMode = RunMode.SILENT;
    
    public synchronized final E parse(File f) throws IOException, IllegalArgumentException{
        
        if(f==null || !f.exists()){
            throw new IllegalArgumentException("Invalid file: " + f);
        }
        //Reset the error log
        clearLog();
        //Set a reference to the file being parsed.
        setFile(f);
        //Open the input stream to the file
        openReader();
        //Execute the parser implementation method.
        E parsed = parse();
        //Close the input stream.
        closeReader();
        //Remove the reference to the file that was parsed.
        setFile(null);
        //Return the resultant object that represents what was parsed
        return parsed;
        
    }
    
    protected abstract E parse() throws IOException;
    
    /**
     * Returns the current line number. 
     * @return The current line number.
     */
    protected int getCurrentLine() {
        return lineCounter;
    }
    
    /**
     * Opens an input stream to the file.
     * @throws IOException on file error.
     */
    protected void openReader() throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(getFile()));
        setReader(reader);
        lineCounter=0;
    }
    protected void closeReader() throws IOException {
        if(getReader() != null){
            //close the reader
            getReader().close();
            //remove the reference.
            setReader(null);
        }
    }
    protected String readLine() throws IOException {
        String line = getReader().readLine();
        if(line != null){
            // Increment the line counter
            lineCounter++;
        }
        return line;
    }
    protected final void logError(String messageId, String err) {
        MessageEvent msgEvt = new MessageEvent(this, messageId, Level.WARN, err, getFile().getAbsolutePath(), getFile().getName(), getCurrentLine());
        errorCount++;
        errorList.add(msgEvt);
        
        if (runMode == RunMode.VERBOSE) {
            fireBuilderEvent(msgEvt);
        } else if (runMode == RunMode.FAILFAST) {
            throw new RuntimeException(msgEvt.toString());
        }
    }
    protected final void forceError(String messageId, String err) {
        MessageEvent msgEvt = new MessageEvent(this, messageId, Level.WARN, err, getFile().getAbsolutePath(), getFile().getName(), getCurrentLine());
        errorCount++;
        errorList.add(msgEvt);

        throw new RuntimeException(msgEvt.toString());
    }
    public final void clearLog() {
        this.errorCount=0;
        this.errorList.clear();
    }
    public void setRunMode(RunMode mode){
        this.runMode=mode;
    }
    public List<MessageEvent> getErrorList(){
        return  this.errorList;
    }
    protected File getFile(){
        return file;
    }
    protected void setFile(File file){
        this.file=file;
    }
    protected BufferedReader getReader(){
        return this.reader;
    }
    private void setReader(BufferedReader reader){
        this.reader=reader;
    }
    protected int getLineCounter() {
        return lineCounter;
    }

    protected void setLineCounter(int lineCounter) {
        this.lineCounter = lineCounter;
    }
}