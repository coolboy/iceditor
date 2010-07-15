/**
 * 
 */
package edu.pitt.cs.android.diff;

import java.io.*;
import java.util.*;
import java.util.Map.Entry;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

import android.util.Log;

/**
 * @author Cool
 *
 */
public class DiffFactory {
	private static final int BUFFER_SIZE = 2048; 
	
	public static Map<String, DiffObject> getDiff(String aPath, String bPath) throws IOException{
		Map<String, DiffObject> ret = new HashMap<String, DiffObject>();
		
		//TODO
		//a.crc
		//b.crc
		
		//get amap
		Map<String, java.lang.Long> aMap = new HashMap<String, java.lang.Long>();
		
		ZipFile aFile = new ZipFile(aPath);

		Enumeration<? extends ZipEntry> aEntries = aFile.entries();
		
		while (aEntries.hasMoreElements()) {
			ZipEntry entry = aEntries.nextElement();
			aMap.put(entry.getName(), entry.getCrc());
		}
		
		//get bmap
		Map<String, java.lang.Long> bMap = new HashMap<String, java.lang.Long>();
		
		ZipFile bFile = new ZipFile(bPath);

		Enumeration<? extends ZipEntry> bEntries = bFile.entries();
		
		while (bEntries.hasMoreElements()) {
			ZipEntry entry = bEntries.nextElement();
			bMap.put(entry.getName(), entry.getCrc());
		}
		
		//compare amap to bmap
		//1. both exist //Modify or same
		//2. in a but not b -> deleted in the newer version //Delete
		for(Entry<String, Long> pair: aMap.entrySet()){
			String key = pair.getKey();
			Long crc = pair.getValue();
			
			Object bcrc = bMap.get(key); 
			
			if (bcrc == null){// not in b, so this file is deleted in b
				DiffObject obj = new DiffObject();
				
				obj.path = key;
				obj.state = DiffObject.State.Delete;
				
				ret.put(key, obj);
			}else{
				if (bcrc.equals(crc)){//have same file
					DiffObject obj = new DiffObject();
					
					obj.path = key;
					obj.state = DiffObject.State.Same;
				
					ret.put(key, obj); 
				}else{//have same file, but diff in content
					DiffObject obj = new DiffObject();
					
					obj.path = key;
					obj.state = DiffObject.State.Modify;
					
					//TODO diff data
					ZipEntry ze = bFile.getEntry(key);
					int entrySize = (int) ze.getSize();
					byte[] buff = new byte[entrySize];
					InputStream in = bFile.getInputStream(ze);
					
					int readed = in.read(buff);
					
					assert(readed == entrySize);
					
					obj.binary = buff;
				
					ret.put(key, obj); 
				}
			}
		}
		
		//compare bmap to amap
		//1. check exist, pre compared in a
		//2. in b but not in a -> newly added in b //Add
		for(Entry<String, Long> pair: bMap.entrySet()){
			String key = pair.getKey();
			
			if (ret.containsKey(key))
				continue;//already compared
			
			Object acrc = aMap.get(key); 
			
			if (acrc == null){//this is a newly added file
				DiffObject obj = new DiffObject();
				
				obj.path = key;
				obj.state = DiffObject.State.Add;
				
				ZipEntry ze = bFile.getEntry(key);
				int entrySize = (int) ze.getSize();
				byte[] buff = new byte[entrySize];
				InputStream in = bFile.getInputStream(ze);
				
				int readed = in.read(buff);
				
				assert(readed == entrySize);
				
				obj.binary = buff;
				
				ret.put(key, obj);
			}else
				assert(false);
		}
		
		//delete entry with Same state
//		Vector<String> dupPaths = new Vector<String>();
//		
//		for (Entry<String, DiffObject> pair:ret.entrySet()){
//			if (pair.getValue().state == DiffObject.State.Same)
//				dupPaths.add(pair.getKey());
//		}
//		
//		for (String dupPath: dupPaths){
//			ret.remove(dupPath);
//		}
		
		return ret;
	}
	
	@SuppressWarnings("unchecked")
	public static File createFromDiff(String srcFileParh, 
			String diffFilePath, 
			String outFilePath) 
	throws IOException, ClassNotFoundException{		
		ZipFile aFile = new ZipFile(srcFileParh);
		
		ObjectInputStream in = new ObjectInputStream(new FileInputStream(diffFilePath));
		Map<String, DiffObject> diff = (Map<String, DiffObject>) in.readObject();
		
		File outFile = new File (outFilePath);
	    FileOutputStream dest = new FileOutputStream(outFile); 
	    ZipOutputStream out = new ZipOutputStream(new BufferedOutputStream(dest)); 
		
		for (Entry<String, DiffObject> pair: diff.entrySet()){
			String entryPath = pair.getKey();
			DiffObject dobj = pair.getValue();
			
			ZipEntry aEntry = aFile.getEntry(entryPath);
			BufferedInputStream origin = null;
			
			byte buf[] = new byte[BUFFER_SIZE]; 
			
			switch(dobj.state){
			case Delete:
				break;
			case Modify:{
				//TODO diff data
				InputStream ai = new ByteArrayInputStream((byte[]) dobj.binary);
				origin = new BufferedInputStream(ai, BUFFER_SIZE);
				
				ZipEntry newEntry = new ZipEntry(entryPath);
				out.putNextEntry(newEntry);
				
		        int count; 
		        while ((count = origin.read(buf, 0, BUFFER_SIZE)) != -1) { 
		          out.write(buf, 0, count); 
		        } 
		        origin.close(); 
			}
				break;
			case Add:{
				InputStream ai = new ByteArrayInputStream((byte[]) dobj.binary);
				origin = new BufferedInputStream(ai, BUFFER_SIZE);
				
				ZipEntry newEntry = new ZipEntry(entryPath);
				out.putNextEntry(newEntry);
				
		        int count; 
		        while ((count = origin.read(buf, 0, BUFFER_SIZE)) != -1) { 
		          out.write(buf, 0, count); 
		        } 
		        origin.close(); 
			}
		        
				break;
				
			case Same:{
				InputStream ai = aFile.getInputStream(aEntry);
				origin = new BufferedInputStream(ai, BUFFER_SIZE);
				
				ZipEntry newEntry = new ZipEntry(entryPath);
				out.putNextEntry(newEntry);
				
		        int count; 
		        while ((count = origin.read(buf, 0, BUFFER_SIZE)) != -1) { 
		          out.write(buf, 0, count); 
		        } 
		        origin.close(); 
			}
		        
				break;
				
			case UnDef:
				Log.v("Err", "UnDef in DiffObject");
				break;
			default:
				Log.v("Err", "Default in switch");
			}
		}
		
		out.close();
		
		return outFile;
	}
}
