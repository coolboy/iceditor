package com.pitt.softengr.icbuilder.util;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import com.pitt.softengr.icbuilder.util.event.BuilderObservable;

/**
 * A common superclass for generating files.
 * @param <E> Capture for the data model used to generate the file.
 * @author Lou Angelucci
 */
public abstract class CommonWriter<E> extends BuilderObservable {
    
    /** File to be written to. */
    private File file;

    /** The writer with an output stream to the file. */
    private BufferedWriter writer;
    
    /** Data model. */
    private E data;
    
    /**
     * Writes to the given file based on the given data model object.
     * @param file The file to write to.
     * @param data The data model.
     * @throws IOException On file error.
     */
    public final void writeFile(File file, E data) throws IOException {
        this.data = data;
        // Open the output stream.
        openWriter(file);
        
        try {
            // Write the boilerplate and audit data.
            writeHeader();
            // Write the file contents.
            writeContent();
        } catch (IOException e) {

            // Flush the output buffer and close the stream.
            try {
                writer.flush();
                writer.close();
            } catch (IOException e2) {
                // This is very bad. Flush can fail here if the 
                // buffer to flush is too big.
                
                // Print the stack trace of the initial problem to the 
                // standard output stream so it does not fail silently.
                e.printStackTrace();
                // Rethrow the more unexpected one.
                throw e2;
            }
            // Continue to propigate the exception.
            throw e;
        }
        // Close the stream and throw away the writer.
        closeWriter();
    }
    
    /**
     * Opens the underlying buffered writer to the specified file.
     * @param file The file to write to.
     * @throws IOException On file error.
     */
    protected void openWriter(File file) throws IOException {
        this.file = file;
        writer = new BufferedWriter(new FileWriter(file));
    }
    
    /**
     * Closes the underlying buffered writer.
     * @return The file the writer was writing to.
     * @throws IOException On file error.
     */
    protected final void closeWriter() throws IOException {
        writer.close();
        writer = null;
        this.file = null;
    }
    
    /**
     * Writes the file contents.
     * @throws IOException On file error.
     */
    protected abstract void writeContent() throws IOException;
    
    /**
     * Writes the standard copyright notice and header data.
     * @throws IOException On file error.
     */
    protected abstract void writeHeader() throws IOException;
    
    /**
     * Writes a string value to the file.
     * @param s The string to write.
     * @throws IOException On file error.
     */
    protected void write(String s) throws IOException {
        writer.write(s);
    }
    
    /**
     * Writes a new-line character to the file.
     * @throws IOException On file error.
     */
    protected void writeln() throws IOException {
        writer.write(Constants.NEW_LINE);
    }
    
    /**
     * Writes a string value and new-line character to the file.
     * @param s The string to write.
     * @throws IOException On file error.
     */
    protected void writeln(String s) throws IOException {
        write(s);
        writeln();
    }
    
    /**
     * Gets the data model object.
     * @return Data model.
     */
    protected E getData() {
        return this.data;
    }
    
    /**
     * Gets the current file.
     * @return The current file.
     */
    protected File getFile() {
        return this.file;
    }
}
