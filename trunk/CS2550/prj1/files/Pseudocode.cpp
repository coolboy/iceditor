/*  Pseudocode for SQL Optimizer  */

// step 1(step1,2,3 in the slides):  

foreach node in the querytree
{
  if (node.type="select")
  	{
  		 if(condition A='B'||A=NUMBER||A<NUMBER||A>NUMBER)
  		   search all children whose type is scan[table], and find
  	     all the attributes in the table associated with the scan.
  	     if there is one attribute match A,
  	     then, remove this select node such as it becomes
  	     the scan[table]\'s direct parent.   
  		 
  		 else if(condition A.B='C'||....)  
  		 	 search all children whose type is scan[table], 
  	     if table matches A,  remove this select node such as 
  	     it becomes the scan[table]\'s direct parent.
  		 
  		 else if(condition A=B)
  		    search all children whose type is PRODUCT,
  		    for every searched PRODUCT, find its left and
  		    right child, if left and right child(or its
  		    	child, child\'child)contains A and B, then remove
  		    	the select such that it is the parent of this PRODUCT,
  		    	if left child has no A or B, go to its right child
  		    		and continue until find a PRODUCT such that
  		    		the PRODUCT\'s left and right child contain
  		    		A and B respectively 
  		    	if right child has no A or B, go to left and do the 
  		    		similar thing
  		    		
  		 else if(condition A.C=B.D)
  		     similar to condition A=B, but simplier because we
  		     now know the table name
  		     
  		 else if(condition combination of above using AND,OR,NOT)   	
  		 	  if AND,
  		 	    break the select into two simple selects
  		 	    and do the similar things above
  		 	    
  		 	  else if OR,
  		 	  	
  		 	  	do nothing
  		 	  	
  		 	  else if NOT,
  		    
  		      do similar as first and second condition, but
  		      use different cost famular? 
  	}
}