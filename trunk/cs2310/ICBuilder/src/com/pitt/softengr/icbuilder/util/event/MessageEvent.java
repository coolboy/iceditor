package com.pitt.softengr.icbuilder.util.event;

public class MessageEvent extends BuilderEvent {

    private static final long serialVersionUID = -4261882463969112937L;
    
    private final java.util.Date dateTimeStamp = new java.util.Date();

    public enum Level {
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        TRACE;
    }
    
    private String messageId;
    
    private Level level;
    
    private String description;

    private String fileAbsolutePath;

    private String fileName;

    private int lineNumber;
    
    public MessageEvent(Object source, String messageId,  Level level, String description) {
        this(source, messageId, level, description, "", "", 0);
    }
    
    public MessageEvent(Object source, String messageId, Level level, String description, String fileAbsolutePath, String fileName, int lineNumber) {
        super(source);
        this.setLevel(level);
        this.setDescription(messageId + ": " + description);
        this.setFileAbsolutePath(fileAbsolutePath);
        this.setFileName(fileName);
        this.setLineNumber(lineNumber);
    }

    public java.util.Date getDateTimeStamp() {
        return this.dateTimeStamp;
    }
 
    public Level getLevel() {
        return level;
    }

    public void setLevel(Level level) {
        this.level = level;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getFileAbsolutePath() {
        return fileAbsolutePath;
    }

    public void setFileAbsolutePath(String fileAbsolutePath) {
        this.fileAbsolutePath = fileAbsolutePath;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public int getLineNumber() {
        return lineNumber;
    }

    public void setLineNumber(int lineNumber) {
        this.lineNumber = lineNumber;
    }
    
    @Override
    public String toString() {

        return this.getDateTimeStamp() + " " + this.getLevel().toString() + " [" + this.getFileName() + ":" + this.getLineNumber() + "] " + this.getDescription();
    }

    public void setMessageId(String messageId) {
        this.messageId = messageId;
    }

    public String getMessageId() {
        return messageId;
    }
}
