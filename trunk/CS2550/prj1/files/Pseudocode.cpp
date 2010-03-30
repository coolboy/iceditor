/*  Pseudocode for SQL Optimizer  */

// step 1(step1,2,3 in the slides):  
/* Yang's job */
foreach node in the querytree
{
  if (node.type="select")
  	{
  		 if(condition A='B'||A=NUMBER||A<NUMBER||A>NUMBER)
  		   search all children whose type is scan[table], and find
  	     all the fields in the table associated with the scan.
		 
  	     if there is one field match A,
  	     then, move this select node and make it becomes
  	     the scan[table]\'s direct parent.
  		 
  		 else if(condition T.B='C'||....)  
  		 	 search all children whose type is scan[table], 
  	     if table matches T,  remove this select node such as 
  	     it becomes the scan[table]\'s direct parent.
  		 
  		 else if(condition A = B) //T1 and T2 omitted
  		    search all children whose type is PRODUCT,
  		    for every searched PRODUCT, find its left and
  		    right child, if left and right child(or its
  		    	child, child\'child)contains A and B, then move
  		    	the select and make it is the parent of this PRODUCT,
				
  		    	if left child doesn't have A or B, go to its right child
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
/* Yong's job */
/*
Join selectivity:
|(R JOIN S)| / (|R| * |S|)
Assume 
If A is a key of R, then |(R Join S)| <= |S|, so js <= (1/|R|).

R.A is unique so that R Join S will be max as S, because 
like 
R.A  S.B
123  123
456  123
789  456

the joined table will be 3, the worst case is that every S.B can be found in R.A
where R.A is unque so that the most is S.B * 1.
*/
for each node {
	 choose join order based on join selectivity, how?  some child has select sub-child should be first  //there is an alternative: page543
	 choose algorithm based on index available, whether sorted or not 
}

/* step 3(step 5 in the slide): put down the project node as low as possible 
while keeping the relevant fields */
/* Yang's job */
for each node {
	 if(node.type == SCAN)
	 	  retrive the table name in the SCAN, and traverse the querytree backwards to find all 
	 	  fields of this table that be used in all of his ancestor node.
		  
	 	  These fields form a list L. Add project(L) node above the SCAN node as its parent.
}

/* step 4(calculate the cost) */
/* Yongg's job */
calculate the cost, using formulas on page 536(for select, project can be also
calculated?),  539(for join), 509(for union, need to figure out details) and 
542/543( materialized cost, namely, how the execution of first node effects the 
cost of subsequent nodes.)