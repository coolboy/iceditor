package com.pitt.softengr.icbuilder;

import java.io.PrintStream;
import java.io.File;
import java.io.IOException;

import java.util.List;
import java.util.ArrayList;

import javax.xml.parsers.ParserConfigurationException;

import org.apache.log4j.Logger;

import com.pitt.softengr.icbuilder.config.icbuilderConfig;
import com.pitt.softengr.icbuilder.model.ic.IcCard;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntryList;
import com.pitt.softengr.icbuilder.model.fsm.ICSystem;
import com.pitt.softengr.icbuilder.util.InputType;
import com.pitt.softengr.icbuilder.util.ic.Merger;
import com.pitt.softengr.icbuilder.util.ic.VerifyIcCardData;
import com.pitt.softengr.icbuilder.util.ic.icdb.IcDataParserXML;
import com.pitt.softengr.icbuilder.util.ic.ice.ICEParserXML;
import com.pitt.softengr.icbuilder.util.ic.icinteraction.IcInteractionParserXML;
import com.pitt.softengr.icbuilder.util.fsm.Transform;
import com.pitt.softengr.icbuilder.output.OutputConfig;

public class commandLine {

    private static final Logger logger = Logger.getLogger(commandLine.class);

    static {
        // Initialize on class load.
        init();
    }   
    /**
     * Initializes the application.
     */
    private static void init() {
        // Initialize the Log4j logging system.
        System.setProperty("log4j.configuration", "log4j.properties");
        if (logger.isInfoEnabled()) {
            logger.info("Logging initialized.");
        }
    }
    
    public static void main(String args[]){
           run(System.out);
    }
    public static void run(PrintStream out){
        
        logger.info("Reading Configuration File");
        icbuilderConfig configuration = icbuilderConfig.loadConfiguration();
        if(configuration == null){
            //config file not loaded
            out.println("ERROR: Config file not found.");
            return;
        }
        logger.info("Configuration File Import Complete!");
        
        //set the output directory
        File outputDir = new File(configuration.getOutputDirectory());
        if(!outputDir.exists()){
            if(!outputDir.mkdir()){
                throw new RuntimeException("Unable to create directory "+outputDir.getName()+".");
            }
        } else {
            if(!outputDir.isDirectory()){
                throw new RuntimeException("Expected directory, but I got "+outputDir.getName());
            }
        }

        // the master list 
        IcCardEntryList ICEntries=null;

        if(InputType.ICE.toString().toLowerCase().equals(configuration.getInputFormat().toLowerCase())){
            // begin yang fix
            IcCardEntryList ICEList = new IcCardEntryList();
            File ICEFile = new File(configuration.getIcICEDatabaseLocation());
            logger.info("Loading ICE data file... "+ICEFile);
            if(ICEFile != null && ICEFile.exists()){
                ICEParserXML iceParser = null;
                try{
                    iceParser = new ICEParserXML();
                } catch (ParserConfigurationException e){
                    logger.error("FAILED to load ICE FILE: "+e);
                    throw new RuntimeException(e);
                }
                logger.debug("Successfully loaded parser... ");
                try{
                    ICEList = iceParser.parse(ICEFile);
                } catch (IOException e){
                    logger.error("FAILED to load ICE File: "+e);
                    throw new RuntimeException(e);
                }
                logger.debug("Successfully parsed file "+ICEFile);
                ICEntries =  new IcCardEntryList(ICEList);
            }
        } else if(InputType.STANDARD.toString().toLowerCase().equals(configuration.getInputFormat().toLowerCase())){
            //begin parsing icdb
            IcCardEntryList ICDBList = new IcCardEntryList();
            File icdbFile = new File(configuration.getIcDatabaseDataLocation());
            logger.info("Loading ICDB data file..."+icdbFile);
            if(icdbFile != null && icdbFile .exists()){
                IcDataParserXML icdbParser = null;     
                try {
                    icdbParser = new IcDataParserXML();
                } catch (ParserConfigurationException e){
                    logger.error("FAILED to load ICDB: "+e);
                    throw new RuntimeException(e);
                }
                try{
                    ICDBList=icdbParser.parse(icdbFile);  
                } catch (IOException e){
                    logger.error("FAILED to load ICDB: "+e);
                    throw new RuntimeException(e);
                }
            }
            logger.info("Successfully loaded ICDB!");
            
            //begin parsing the interactions file
            List<IcCard> interactList = new ArrayList<IcCard>();
            File icInteractionsFile = new File(configuration.getInteractionDataLocation());
            logger.info("Loading icCardInteractions data file..."+icInteractionsFile);
            if(icInteractionsFile != null && icInteractionsFile.exists()){
                IcInteractionParserXML icInteractParser = null;
                try{
                    icInteractParser = new IcInteractionParserXML();
                } catch(ParserConfigurationException e){
                    logger.error("FAILED to load icCardInteractions: "+e);
                    throw new RuntimeException(e);
                }
                try{
                    interactList=icInteractParser.parse(icInteractionsFile);
                } catch(IOException e){
                    logger.error("FAILED to load icCardInteractions: "+e);
                    throw new RuntimeException(e);                
                }
            }
            // merge the two parsed files
            logger.info("Merging data from both files...");
            ICEntries = Merger.cardAndInterations(ICDBList, interactList);
            if(ICEntries==null){
                logger.error("Merging of data failed...");
                throw new RuntimeException("Merging of data failed...");
            }
            logger.info("Merging Complete!");
        }
        
        logger.info("Verifiying all index card data is available...");
        VerifyIcCardData verify = new VerifyIcCardData();
        verify.process(ICEntries);
        logger.info("Verification complete!");
        
        //convert using merger from Index card to FSM
        logger.info("Converting Index Card Data to Finite State Machine");
        Transform transform = new Transform();
        transform.setCardSystem(ICEntries);
//        for(IcCardEntry entry : ICEntries.getIcEntryList()){
//            System.out.println(entry);
//        }
        ICSystem system = transform.icSystemToIndexCellSystem(configuration.getSystemType());
        List<ICSystem> systems = new ArrayList<ICSystem>();
        systems.add(system);
        logger.info("Convertion process complete!");
        logger.info("Writing XML system file...");
        OutputConfig fsmOutputConfig = new OutputConfig();
        fsmOutputConfig.process(outputDir, systems);
    }
}
