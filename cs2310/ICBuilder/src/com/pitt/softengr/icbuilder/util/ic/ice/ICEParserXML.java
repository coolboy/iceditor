package com.pitt.softengr.icbuilder.util.ic.ice;

import java.io.File;
import java.io.IOException;

import java.util.List;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.apache.log4j.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.pitt.softengr.icbuilder.model.ic.IcCardEntry;
import com.pitt.softengr.icbuilder.model.ic.IcCard;
import com.pitt.softengr.icbuilder.model.ic.IcCardEntryList;

import com.pitt.softengr.icbuilder.util.GenericParserImpl;
import com.pitt.softengr.icbuilder.util.event.MessageEvent;
import com.pitt.softengr.icbuilder.util.event.ProgressEvent;
import com.pitt.softengr.icbuilder.util.event.MessageEvent.Level;

public class ICEParserXML extends GenericParserImpl<IcCardEntryList> implements ICEParser{

    private static final Logger logger = Logger.getLogger(ICEParserXML.class);
    
    private static final String IC_CARD_LIST = "scards";
    private static final String IC_CARD_ITEM = "item";
    private static final String IC_CARD = "ic_";
    private static final String IC_CARD_ID = "Id";
    private static final String IC_CARD_PARENT = "parentId";
    private static final String IC_CARD_NAME = "Name";
    private static final String IC_CARD_DESCRIPTION = "Description";
    private static final String IC_CARD_PATTERN = "IntPattern";
    private static final String IC_CARD_TASK = "Task";
    private static final String IC_TIME_CRITICAL_CONDITION = "TimeCriticalCondition";
    private static final String IC_OTHER_NAME = "OtherName";
    private static final String IC_OTHER_ID = "otherId";
    private static final String IC_OTHER_MESSAGE = "OtherMessage";
    private static final String IC_OTHER_TASK = "OtherTask";
    private static final String IC_SENARIO = "scenario";
    private static final String IC_SYSTEM = "icSystem";
    private static final String IC_GROUP = "icGroup";
    private static final String IC_ENTRY_ID = "icEntryId";
    private static final String IC_ENTRY_NAME = "icEntryName";
    private static final String STRING_TAG = "str";
    
    /** The SAX parser */
    private DocumentBuilder documentBuilder;
    /** The DOM */
    private Document document;
    
    public ICEParserXML() throws ParserConfigurationException{
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        documentBuilder = dbf.newDocumentBuilder();
    }
    public IcCardEntryList parse(){
        NodeList root = getDocument().getElementsByTagName(IC_CARD_LIST);
        IcCardEntryList entryList = new IcCardEntryList();
        List<IcCard> cards = new ArrayList<IcCard>();
        fireBuilderEvent(new MessageEvent(this,"",Level.INFO,"Parsing ICE..."));
        for(int z=0; z<root.getLength();z++){
            Node rootNode = root.item(z);
            if(rootNode.hasChildNodes()){
                NodeList nodeEntries = rootNode.getChildNodes();
                for(int x=0; x<nodeEntries.getLength();x++){
                    Node entryNode = nodeEntries.item(x);
                    if(IC_CARD_ITEM.equals(entryNode.getNodeName())){
                        if(entryNode.hasChildNodes()){
                            NodeList entryNodeList = entryNode.getChildNodes();
                            for(int y=0; y<entryNodeList.getLength();y++){
                                Node icCardNode = entryNodeList.item(y);
                                if(IC_CARD.equals(icCardNode.getNodeName())){
                                    IcCard card = new IcCard();
                                    parseNodeElements(icCardNode,card);
                                    cards.add(card);
                                }
                            }
                        }
                    }
                }
            }
        }
        // add all cards to each entry
        for(IcCard card : cards){
            String entryId = card.getEntryId();
            IcCardEntry e = new IcCardEntry();
            e.setEntryId(card.getEntryId());
            e.setEntryName(card.getEntryName());
            if(entryList.findEntryCardByEntryID(entryId)==null){
                entryList.addIcCardEntry(e);
            }
        }
        for(IcCard card : cards){
            for(IcCardEntry e : entryList.getIcEntryList()){
                if(card.getEntryId().equals(e.getEntryId())){
                    e.addIcCardtoEntries(card);
                }
            }
        }
        return (entryList);
    }
    private void parseNodeElements(Node node, IcCard card){

        
        if(node.hasChildNodes()){
            NodeList cardValues = node.getChildNodes();
            for(int x=0; x<cardValues.getLength();x++){
                Node cardValue = cardValues.item(x);
                if(IC_CARD_ID.equals(cardValue.getNodeName())){
                    card.setID(cardValue.getTextContent());
                } else if(IC_CARD_PARENT.equals(cardValue.getNodeName())){
                    card.setIcParentId(cardValue.getTextContent());
                } else if(IC_CARD_PATTERN.equals(cardValue.getNodeName())) {
                    card.setIcPatternType(cardValue.getTextContent());
                } else if(IC_CARD_NAME.equals(cardValue.getNodeName())){
                    card.setIcName(getStringValue(cardValue,STRING_TAG));
                } else if(IC_CARD_DESCRIPTION.equals(cardValue.getNodeName())){
                    card.setIcDescription(getStringValue(cardValue,STRING_TAG));
                } else if(IC_CARD_TASK.equals(cardValue.getNodeName())){
                    card.setTask(getStringValue(cardValue,STRING_TAG));
                } else if(IC_TIME_CRITICAL_CONDITION.equals(cardValue.getNodeName())){
                    card.setTimeCriticalCondition(getStringValue(cardValue,STRING_TAG));
                } else if(IC_OTHER_NAME.equals(cardValue.getNodeName())){
                    card.setOtherIcName(getStringValue(cardValue,STRING_TAG));
                } else if(IC_OTHER_ID.equals(cardValue.getNodeName())){
                    card.setIcOtherId(cardValue.getTextContent());
                } else if(IC_OTHER_MESSAGE.equals(cardValue.getNodeName())){
                    card.setIcOtherMessage(getStringValue(cardValue,STRING_TAG));
                } else if(IC_OTHER_TASK.equals(cardValue.getNodeName())){
                    card.setIcOtherTask(getStringValue(cardValue,STRING_TAG));
                } else if(IC_SENARIO.equals(cardValue.getNodeName())){
                    card.setSenario(getStringValue(cardValue,STRING_TAG));
                } else if(IC_SYSTEM.equals(cardValue.getNodeName())){
                    card.setSystem(getStringValue(cardValue,STRING_TAG));
                } else if(IC_GROUP.equals(cardValue.getNodeName())){
                    card.setGroup(getStringValue(cardValue,STRING_TAG));
                } else if(IC_ENTRY_ID.equals(cardValue.getNodeName())){
                    card.setEntryId(getStringValue(cardValue,STRING_TAG));
                } else if(IC_ENTRY_NAME.equals(cardValue.getNodeName())){
                    card.setEntryName(getStringValue(cardValue,STRING_TAG));
                }
            }    
        }
    }
    private String getStringValue(Node node, String key){
        if(node.hasChildNodes()){
            NodeList stringValues = node.getChildNodes();
            for(int y=0; y<stringValues.getLength(); y++){
                Node stringNode = stringValues.item(y);
                if(key.equals(stringNode.getNodeName())){
                    return stringNode.getTextContent();
                }
            }
        }
        return null;
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

}
