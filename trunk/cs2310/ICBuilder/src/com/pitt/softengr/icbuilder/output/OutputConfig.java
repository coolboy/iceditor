package com.pitt.softengr.icbuilder.output;

import java.io.File;
import java.io.IOException;
import java.util.List;

import com.pitt.softengr.icbuilder.model.fsm.ICSystem;

import com.pitt.softengr.icbuilder.output.fsm.IndexCellSystemXmlWriter;

public class OutputConfig {

    public void process(File outputDir, List<ICSystem> icSystemList){
        
        writeICSystemXML(outputDir,icSystemList);
    }
    private void writeICSystemXML(File outputDir, List<ICSystem> icSystemList){
        
        IndexCellSystemXmlWriter icSystemWriter = new IndexCellSystemXmlWriter();
        File icSystemXMLFile = new File(outputDir.getAbsoluteFile()+"/ICSystem.xml");
        try{
            icSystemWriter.writeFile(icSystemXMLFile, icSystemList);
        } catch (IOException e){
            throw new RuntimeException(e);
        }
    }
}
