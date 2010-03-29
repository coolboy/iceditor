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
  		    remove the found PRODUCT , add JOIN with condition A=B 
  		    at the same place
  		    		
  		 else if(condition A.C=B.D)
  		     similar to condition A=B, but simplier because we
  		     now know the table name
  		     
  		 else if(condition combination of above using AND,OR,NOT)   	
  		 	  if AND,
  		 	    break the select into two simple selects
  		 	    and do the similar things above
  		 	    
  		 else if (condition has an OR)
  		 	  	
  		 	  	do nothing
  		 	  	
  		 else if (condition has an NOT)
  		    
  		      do similar as first and second condition, but
  		      use different cost famular? 
  	}
}

/* step 2(step 4 in the slide): choose the join order and method*/

for each node
{
	 choose join order based on js , how?  some child has select sub-child should be first  //there is an alternative: page543
	 choose algorithm based on index available, whether sorted or not 
}

/* step 3(step 5 in the slide): put down the project node as low as possible 
while keeping the relevant fields */

for each node 
{
	 if(node.type = SCAN)
	 	  retrive the table in the SCAN, and traverse the querytree backwards to find all 
	 	  attribute of the table involved in all ancestors of this SCAN node. 
	 	  these attributes form a list L. Add project(L) node above the SCAN node as its parent. 
	 	 
}

/* step 4(calculate the cost) */
    
    
   calculate the cost, using formulas on page 536(for select, project can be also
   calculated?),  539(for join), 509(for union, need to figure out details) and 
   542/543(materialized cost, namely, how the execution of first node effects the 
   cost of subsequent nodes.)
   
   
   