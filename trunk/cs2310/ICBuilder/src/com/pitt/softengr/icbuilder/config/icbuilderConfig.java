package com.pitt.softengr.icbuilder.config;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.FileInputStream;

import java.util.Properties;

import org.apache.log4j.Logger;


public class icbuilderConfig {
    
    private static final Logger logger = Logger.getLogger(icbuilderConfig.class);
    
    private static final String INTERACTION_XML_LOCATION = "input.data.ic.interactions";
    
    private static final String IC_DATABASE_LOCATION = "input.data.ic.database";
    
    private static final String ICE_IC_DATABASE = "input.data.ic.ice";
    
    private static final String SYSTEM_TYPE = "system.indexcell.type";
    
    private static final String INPUT_FORMAT = "input.data.ic.format";
    
    private static final String OUTPUT_DIRECTORY = "output.dir";
    
    private File configSourceFile;
    
    private String outputDirectory;
    
    private String icInteractions;
    
    private String icDatabase;
    
    private String icICEDatabase;
    
    private String systemType;
    
    private String inputFormat;
    
    private static icbuilderConfig currentConfig;
    
    private icbuilderConfig() {}
    
    public static icbuilderConfig loadConfiguration() throws RuntimeException{
        if (logger.isInfoEnabled()) {
            logger.info("Current working directory is " + (new File(".")).getAbsolutePath());
        }
        String configlocation = System.getProperty("icbuilder.conf");
        if(configlocation == null || configlocation.length()==0){
            if(logger.isInfoEnabled()){
                logger.info("Config file not specified...");
            }
            return null;
        }
        File configFile = new File(configlocation);
        return loadConfiguration(configFile);
    }
    public static icbuilderConfig loadConfiguration(File configFile)throws RuntimeException {
        
        InputStream is = null;
        try{
            is = new FileInputStream(configFile);
        } catch(IOException e) {
            logger.error("Unable to open config file."+e);
            throw new RuntimeException("Unable to open config file.", e);
        }
        if(is == null){
            logger.error("Unable to read config file...");
            throw new RuntimeException("Unable to read config file...");
        }
        Properties configProps = new Properties();
        try{
            configProps.load(is);
            is.close();
        } catch (IOException e){
            logger.error("Error reading config file..."+e);
            throw new RuntimeException("Error reading config file...",e);
        }
        icbuilderConfig ibc = new icbuilderConfig();
        ibc.load(configFile,configProps);
        setCurrentConfig(currentConfig);
        return ibc;
    }
    private String loadProperty(Properties properties, String key){
        String value = properties.getProperty(key);
        if(logger.isDebugEnabled()){
            logger.debug("Loaded config property: " + key + " = " + value);
        }
        return value;
    }
    private String loadRequiredProperty(Properties properties, String key){
        String value = properties.getProperty(key);
        if(logger.isDebugEnabled()){
            logger.debug("Loaded config property: " + key + " = " + value);
        }
        if(value == null) {
            throw new RuntimeException("Required configuration property " + key + " not found.");
        }
        return value;
    }
    private void load(File sourceFile, Properties properties) throws RuntimeException {
        if(logger.isDebugEnabled());
            logger.debug("Loading properties from config file");
        setConfigSourceFile(sourceFile);
        //load properties
        setOutputDirectory(loadRequiredProperty(properties,OUTPUT_DIRECTORY));
        setInteractionDataLocation(loadRequiredProperty(properties,INTERACTION_XML_LOCATION));
        setIcDatabaseDataLocation(loadRequiredProperty(properties,IC_DATABASE_LOCATION));
        setIcICEDataBaseLocation(loadProperty(properties,ICE_IC_DATABASE));
        setSystemType(loadProperty(properties,SYSTEM_TYPE));
        setInputFormat(loadRequiredProperty(properties,INPUT_FORMAT));
        //load required properties
        
        if(logger.isDebugEnabled()){
            logger.debug("Configuration settings loaded successfully.");
        }
    }
    public void setConfigSourceFile(File configSourceFile){
        this.configSourceFile=configSourceFile;
    }
    public File getConfigSourceFile(){
        return this.configSourceFile;
    }
    public  void setOutputDirectory(String outputDirectory){
        this.outputDirectory=outputDirectory;
    }
    public String getOutputDirectory(){
        return this.outputDirectory;
    }
    public void setInteractionDataLocation(String icInteractions){
        this.icInteractions=icInteractions;
    }
    public String getInteractionDataLocation(){
        return this.icInteractions;
    }
    public void setIcDatabaseDataLocation(String icDatabase) {
        this.icDatabase=icDatabase;
    }
    public String getIcDatabaseDataLocation(){
        return this.icDatabase;
    }
    public String getIcICEDatabaseLocation(){
        return this.icICEDatabase;
    }
    public void setIcICEDataBaseLocation(String icICEDatabase){
        this.icICEDatabase=icICEDatabase;
    }
    public String getSystemType(){
        return this.systemType;
    }
    public void setSystemType(String systemType){
        this.systemType=systemType;
    }
    public String getInputFormat(){
        return this.inputFormat;
    }
    public void setInputFormat(String inputFormat){
        this.inputFormat=inputFormat;
    }
    public static void setCurrentConfig(icbuilderConfig cfg){
        currentConfig = cfg;
    }
    public static icbuilderConfig getCurrentConfig(){
        return currentConfig;
    }
}
