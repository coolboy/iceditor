/* ex4: Arrays */
program ex4;
class c4
{
	declarations
		int[] x=int[10];
		int[] ia[]={3, 5, 7};
	enddeclarations
	method void main()
	declarations
		int[] y=int [25];
		int z ;
	enddeclarations
	{
	y[3] := 17; 
	system.println('x.length='); 
	system.println(x.length); 
	z := y[3] + 3;
	ia[2] := 27 ; 
	system.println('z=');
	system.println(z); 
	system.println('ia[0]=');
	system.println(ia[0]);
	system.println('z=');
	system.println(z); 
	system.println('ia[1]=');
	system.println(ia[1]); 
	system.println('ia[2]=');
	system.println(ia[2]);
        z := -1;
	while (z<2)
	{
	  ia[z+1] := 1 + z*2 + z*z;
	  z:=z+1;
	};
        z := 0;
	while (z<=2)
	{
	  system.println(ia[z]);
	  z:=z+1;
	};
	}
}
