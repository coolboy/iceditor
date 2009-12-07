package com.pitt.softengr.icbuilder.util.ic.icinteraction;

import java.io.File;
import java.io.IOException;

import java.util.List;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.pitt.softengr.icbuilder.model.ic.IcCard;

import com.pitt.softengr.icbuilder.util.GenericParserImpl;
import com.pitt.softengr.icbuilder.util.event.MessageEvent;
import com.pitt.softengr.icbuilder.util.event.ProgressEvent;
import com.pitt.softengr.icbuilder.util.event.MessageEvent.Level;

public class IcInteractionParserXML extends GenericParserImpl<List<IcCard>> implements  IcInteractionParser{
    
    // String constants found in xml data file
    private static final String IC_INTERACTIONS = "icInteractions";
    private static final String IC_INTERACTION = "icInteraction";
    private static final String IC_CARD = "icCard";
    
    // String constants for icCards with in interactions
    private static final String IC_ID = "id";
    private static final String IC_NAME = "icName";
    private static final String IC_PARENT_ID = "parentId";
    private static final String IC_OTHER_IC_ID = "otherIcId";
    private static final String IC_SCENARIO = "scenario";
    private static final String IC_SYSTEM = "icSystem";
    private static final String IC_GROUP = "icGroup";
    
    /** The SAX parser */
    private DocumentBuilder documentBuilder;
    /** The DOM */
    private Document document;
    
    public IcInteractionParserXML() throws ParserConfigurationException{
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        documentBuilder=dbf.newDocumentBuilder();
    }
    
    public List<IcCard> parse(){
        Element root = getDocument().getDocumentElement();
        if(!IC_INTERACTIONS.equals(root.getNodeName())){
            return null;
        }
        List<IcCard> interacts = new ArrayList<IcCard>();
        
        fireBuilderEvent(new MessageEvent(this, "", Level.INFO, "Parsing IC Interactions..."));
        
        NodeList interactionNodes = root.getChildNodes();
        for(int x = 0; x < interactionNodes.getLength(); x++){
            Node interactionNode = interactionNodes.item(x);
            if(IC_INTERACTION.equals(interactionNode.getNodeName())){
                if(interactionNode.hasChildNodes()){
                    NodeList icCards = interactionNode.getChildNodes();
                    for(int y=0;y<icCards.getLength();y++){
                        Node icCard = icCards.item(y);
                        if(IC_CARD.equals(icCard.getNodeName())){
                            NamedNodeMap attributes = icCard.getAttributes();
                            String id = attributes.getNamedItem(IC_ID).getNodeValue();
                            String name = attributes.getNamedItem(IC_NAME).getNodeValue();
                            String parent = attributes.getNamedItem(IC_PARENT_ID).getNodeValue();
                            String other = attributes.getNamedItem(IC_OTHER_IC_ID).getNodeValue();
                            String senario = attributes.getNamedItem(IC_SCENARIO).getNodeValue();
                            String system = attributes.getNamedItem(IC_SYSTEM).getNodeValue();
                            String group = attributes.getNamedItem(IC_GROUP).getNodeValue();
                            IcCard card = new IcCard();
                            card.setID(id);
                            card.setIcName(name);
                            card.setIcParentId(parent);
                            card.setIcOtherId(other);
                            card.setSenario(senario);
                            card.setSystem(system);
                            card.setGroup(group);
                            interacts.add(card);
                        }
                    }
                }
            }
        }
        return interacts;
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
