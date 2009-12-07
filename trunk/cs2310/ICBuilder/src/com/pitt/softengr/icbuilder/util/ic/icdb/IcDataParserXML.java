package com.pitt.softengr.icbuilder.util.ic.icdb;

import java.io.File;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.pitt.softengr.icbuilder.model.ic.IcCardEntry;
import com.pitt.softengr.icbuilder.model.ic.IcCard;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntryList;
import com.pitt.softengr.icbuilder.model.ic.IcPattern;

import com.pitt.softengr.icbuilder.util.GenericParserImpl;
import com.pitt.softengr.icbuilder.util.event.MessageEvent;
import com.pitt.softengr.icbuilder.util.event.ProgressEvent;
import com.pitt.softengr.icbuilder.util.event.MessageEvent.Level;

public class IcDataParserXML extends GenericParserImpl<IcCardEntryList> implements IcDataParser{

    // String constants found in xml  data file
    private static final String IC_CARD_LIST = "icCardList";
    private static final String IC_CARD_ENTRY = "icCardEntry";
    private static final String IC_CARD = "icCard";
    private static final String IC_OTHER = "icOther";
    
    // String constants for icCard Entries
    private static final String IC_ENTRY_ID = "icEntryId";
    private static final String IC_ENTRY_NAME="icEntryName";
    
    //String constants for individual icCards
    private static final String IC_ID = "icId";
    private static final String IC_NAME = "icName";
    private static final String IC_DESCRIPTION = "icDescription";
    private static final String IC_INT_PATTERN = "icIntPattern";
    private static final String IC_MY_TASK = "icMyTask";
    private static final String IC_TIME_CRITICAL_CONDITION = "icTimeCriticalCondition";
    private static final String IC_NUMBER_CURRENT = "icNumberCurrent";
    private static final String IC_NUMBER_TOTAL = "icNumberTotal";
    
    //String constants for individual card messaging
    private static final String IC_OTHER_NAME = "icOtherName";
    private static final String IC_OTHER_MESSAGE = "icOtherMessage";
    private static final String IC_OTHER_TASK = "icOtherTask";
    private static final String IC_OTHER_ID = "otherId";
    
    /** The SAX parser */
    private DocumentBuilder documentBuilder;
    /** The DOM */
    private Document document;
    
    public IcDataParserXML() throws ParserConfigurationException{
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        documentBuilder = dbf.newDocumentBuilder();
    }
    public IcCardEntryList parse(){
        Element root = getDocument().getDocumentElement();
        if(!root.getNodeName().equals(IC_CARD_LIST)){
            return null;
        }
        IcCardEntryList entryList = new IcCardEntryList();
        
        fireBuilderEvent(new MessageEvent(this,"",Level.INFO,"Parsing ICDB..."));

        // get the entries
        NodeList entries = root.getChildNodes();
//        parseNodes(entries, entryList);
        for(int x=0; x<entries.getLength();x++){
            //get the cards
            Node entryNode = entries.item(x);
            if(IC_CARD_ENTRY.equals(entryNode.getNodeName())){
                IcCardEntry entryCard=new IcCardEntry();
                parseNodeAttributes(entryNode,entryCard);
                entryList.addIcCardEntry(entryCard);
                if(entryNode.hasChildNodes()){
                    //get the other cards
                    NodeList entryNodeList = entryNode.getChildNodes();
                    for(int y=0; y<entryNodeList.getLength();y++){
                        Node icCardNode = entryNodeList.item(y);
                        if(IC_CARD.equals(icCardNode.getNodeName())){
                            IcCard icCard = new IcCard();
                            parseNodeAttributes(icCardNode,icCard);
                            if(icCardNode.hasChildNodes()){
                                NodeList icCardNodeList = icCardNode.getChildNodes();
                                for(int z=0; z<icCardNodeList.getLength(); z++){
                                    Node icOtherNode = icCardNodeList.item(z);
                                    if(IC_OTHER.equals(icOtherNode.getNodeName())){
                                        parseNodeAttributes(icOtherNode,icCard);
                                    }
                                }
                            }
                            entryCard.addIcCardtoEntries(icCard);
                       }
                    }
                }
            }
        }
        return entryList;
    }
    protected void openReader() throws IOException {
        documentBuilder.reset();       
        File xmlFile = getFile();
        Document doc = null;
        try{
            //Let the sax parser do all the hard work...
            doc = documentBuilder.parse(xmlFile);
        } catch (SAXException e){
            throw new RuntimeException("File "+xmlFile.getName() + " caused exception during XML parsing.",e);
            
        } catch (IOException e){
            throw new RuntimeException("File "+xmlFile.getName() + " caused exception during XML parsing.",e);
        }
        if(doc == null){
            throw new RuntimeException("XML parsing failed on file "+xmlFile.getName());
        }
        setDocument(doc);
    }
    /**
     * {@inheritDoc}
     */
    protected void closeReader() throws IOException {
        // not used
    }
    
    protected Document getDocument() {
        return document;
    }
    
    protected void setDocument(Document d) {
        document = d;
    }
/*    private void parseNodes(NodeList entries, IcCardEntryList entryList){
        for(int x=0;x<entries.getLength();x++){
            Node node = entries.item(x);
            IcCardEntry entry = parseNodeAttributes(node, entryList);
            if (entry != null){
                entryList.addIcCardEntry(entry);
            }
            if(node.hasChildNodes()){
                parseNodeAttributes(node, entry);
                parseNodes(node.getChildNodes(), entryList);
            }
            fireBuilderEvent(new ProgressEvent(this,0,entries.getLength(),x));
        }
    }
 */
    private void parseNodeAttributes(Node node, IcCardEntry entry){
            NamedNodeMap attributes = node.getAttributes();
            String entryId = attributes.getNamedItem(IC_ENTRY_ID).getNodeValue();
            String entryName = attributes.getNamedItem(IC_ENTRY_NAME).getNodeValue();
            entry.setEntryId(entryId);
            entry.setEntryName(entryName);
 //           System.out.println("ENTRY ID : "+entryId+
 //                              " NAME : "+entryName);  
    }
    private void parseNodeAttributes(Node node, IcCard card){
        
        if(IC_CARD.equals(node.getNodeName())){
            NamedNodeMap attributes = node.getAttributes();
            String icId = attributes.getNamedItem(IC_ID).getNodeValue();
            String icName = attributes.getNamedItem(IC_NAME).getNodeValue();
            String icDescription = attributes.getNamedItem(IC_DESCRIPTION).getNodeValue();
            String icIntPattern = attributes.getNamedItem(IC_INT_PATTERN).getNodeValue();
            String icMyTask = attributes.getNamedItem(IC_MY_TASK).getNodeValue();
            String icTimeCriticalCondition = attributes.getNamedItem(IC_TIME_CRITICAL_CONDITION).getNodeValue();
            String icNumberCurrent = attributes.getNamedItem(IC_NUMBER_CURRENT).getNodeValue();
            String icNumberTotal = attributes.getNamedItem(IC_NUMBER_TOTAL).getNodeValue();
            card.setID(icId);
            card.setIcName(icName);
            card.setIcDescription(icDescription);
            card.setIcPatternType(icIntPattern);
            card.setTask(icMyTask);
            card.setTimeCriticalCondition(icTimeCriticalCondition);
        
//        System.out.println("IC ID : "+icId+
//                           " IC NAME : "+icName+
//                           " IC DESCRITPTION : "+icDescription+
//                           " IC PATTERN : "+icIntPattern+
//                           " MY TASK : "+icMyTask+
//                           " TCC : "+icTimeCriticalCondition+
//                           " CURRENT NUM : "+icNumberCurrent+
//                           " TOTAL NUM : "+icNumberTotal);
        } else if(IC_OTHER.equals(node.getNodeName())){
            NamedNodeMap attributes = node.getAttributes();
            String icOtherName=attributes.getNamedItem(IC_OTHER_NAME).getNodeValue();
            String icOtherMessage=attributes.getNamedItem(IC_OTHER_MESSAGE).getNodeValue();
            String icOtherTask=attributes.getNamedItem(IC_OTHER_TASK).getNodeValue();
            String otherId=attributes.getNamedItem(IC_OTHER_ID).getNodeValue();
            card.setIcOtherId(otherId);
            card.setOtherIcName(icOtherName);
            card.setIcOtherMessage(icOtherMessage);
            card.setIcOtherTask(icOtherTask);
//        System.out.println("IC OTHER ID : "+icOtherName+
//                           " IC OTHER MSG : "+icOtherMessage+
//                           " IC OTHER TASK : "+icOtherTask+
//                           " IC OTHER ID : "+otherId);   
        }
    }
}
