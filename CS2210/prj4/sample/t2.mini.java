/* EXAMPLE 4: Two classes this time */
program x4;
class Point0 {
	 declarations
		int[] parr0 = int [12];
        int x , y = 5;
	 enddeclarations
	 method void f0()
	 {
		system.println('Before x=');
		system.println(x); 
	        x := x*x;   
		y := 1;
		while (y<12)
		{
			parr0[y] := parr0[y-1]+1;
			y := y+1;
		};
		system.println('After x=');
		system.println(x); 
	 }
}

class Point1 {
	declarations
	        int t1 = 12;
            Point0 p1;
	enddeclarations
	 method void f1()
	 {
		system.println('Before t1=');	
		system.println(t1);
	        t1   := 1024; 
		p1.f0(); 
		system.println('After t1=');
		system.println(t1);
	 }
}

class Point2 {
	declarations
            	Point1 p21;
		Point0 p20;
		int xy;
	enddeclarations		
        method void main()	
	     declarations	
		        int xx = 22;	
			Point1 px;
	     enddeclarations
	{
	   Point0.f0();
           p21.f1(); 
	   px.p1.x := xx;
	   px.p1.f0(); 
	   p21.t1 := 133;
	   p21.f1();
	   system.println('p21.p1.x=');
	   system.println(p21.p1.x);
	   p20.f0();
	   xy := 0;
	   while (xy <12)
	   {
		system.println('p20.parr0:');
		system.println(p20.parr0[xy]);
		xy := xy+1;
	   };
	   Point1.f1();
	   system.println('p20.x=');
	   system.println(p20.x); 
        }
}



