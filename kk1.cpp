/* 
   KenKen Solver with Backtracking	

   @author: Christian Idylle
   @date: 09/29/11
   @description: Authored for CSCI561 USC 

*/

#include <iostream> 
#include <fstream>

using namespace std;

 
typedef struct cell { 
	int cage;  //what cage/block the cell belongs to. User defined.
	int cageSize; //number of cells in a block/cage. 
	int target; //the target value of the block/cage.
	char operand; //the operation used in the block/cage.
	int value; //the set of possible values assigned to the cell.
	
} cell;  

cell * X;
int n;

void backtrack(int index);
void print_solution();
bool check_constraints(int index,int candidate);
bool check_row(int index,int candidate);
bool check_col(int index,int candidate);
bool check_cage(int index,int candidate);
bool check_add(int index,int candidate);
bool check_sub(int index,int candidate); 
bool check_multiply(int index,int candidate);
bool check_div(int index,int candidate);
int max(int a, int b);
int min(int a, int b);

int main() { 
	
	char filename[50];

	
	cout<<"Welcome to CSCI561 Programming Assignment 1" <<endl;
	cout<<"KenKen Solve using backtracking by @Christian Idylle"<<endl;
	cout<<" "<<endl;
	cout << "Enter n (For a board of size nxn): ";
	cin >> n; 
	cout << "Enter filename of board inputs (eg. kkboard1.txt): ";
	cin >> filename;

	//Create the array of cells and initializing values.
	X = new cell[n*n];
		
	// Reading txt file to construct the board. 
	// format: one line per cell: cage#, cageSize, target value, operator.
	ifstream in(filename);
	if(!in) { cout << "Cannot open file! \n"; return 1; }

	for(int i=0;i<(n*n);i++)
	{ 
		in >> X[i].cage;
		in >> X[i].cageSize;
		in >> X[i].target;
		in >> X[i].operand;
	}

	in.close();

	//Launching backtracking to find solutions 
	int index=0;
	backtrack(index);
			
	//clean up
	delete [] X;  
	return 0;

}

			
void backtrack(int index)
{

	if(index==(n*n))      
	{print_solution();}
	else
	{
		for(int i=1;i<=n;i++)
		{
			int candidate = i;
			
			if(check_constraints(index,candidate)) 
			{
				X[index].value=candidate;				
				backtrack(index+1);
			}
		}
	}
}


bool check_constraints(int index, int candidate)
{
	if(!check_row(index,candidate)) return false;
	if(!check_col(index,candidate)) return false;
	if(!check_cage(index,candidate)) return false;

	return true;
}

  
bool check_cage(int index, int candidate)
{
	int checked_cells=0;
	for(int i=0;i<=index;i++)
	{
		if(X[i].cage==X[index].cage) checked_cells++;
	}

	
	if(checked_cells==X[index].cageSize) {		
	switch(X[index].operand)
	{	
		case '=': 
		if(candidate == X[index].target) 
		{return true; break;}
		else {return false;break;}
		
		case '+': 
		if(check_add(index,candidate)) 
		{return true; break;}
		else {return false; break;}

		case '-': 
		if(check_sub(index,candidate)) 
		{return true;break;}
		else {return false; break;}

		case '*': 
		if(check_multiply(index,candidate)) 
		{return true; break;}
		else {return false; break;}

		case '/': 
		if(check_div(index,candidate)) 
		{return true; break;}
		else{return false; break;}
	}
	}
	else {return true;}


}


bool check_row(int index,int candidate)
{
	int row = int(index/n);
	for(int i=(n*row);i<(n*row)+(index%n);i++)
	{
		if(X[i].value==candidate&&i!=index) return false;
	}

	return true;		
}


bool check_col(int index,int candidate)
{
	int col = index%n;
	for(int i=col;i<index;i=i+n)
	{
		if(X[i].value==candidate&&i!=index) return false;
	}

	return true;		
}



bool check_add(int index, int candidate)
{	
	//checks to see if all the added values of a cage add up correctly.
	int sum=candidate;
	for(int i=0;i<index;i++)
	{
		if(X[i].cage==X[index].cage) sum+=X[i].value;
	}
	
	if(sum!=X[index].target) return false;
	else return true;
}

bool check_sub(int index,int candidate)
{
	//checks in the substraction between the 2 cells in the block make the target value. 		The min value should always be substracted to max value.
	int compared_value;
	for(int i=0;i<index;i++)
	{
		if(X[i].cage==X[index].cage) compared_value=X[i].value;
	}

	if((max(compared_value,candidate) - min(compared_value,candidate)) != X[index].target) return false;
	else return true;
}


bool check_multiply(int index,int candidate)
{
	//Same thing as add, except multiplication is used. 2+ cells possible per cage.
	int product=candidate;
	for(int i=0;i<index;i++)
	{
		if(X[i].cage==X[index].cage) product*=X[i].value;
	}
	
	if(product!=X[index].target) return false;
	else return true;
}



bool check_div(int index, int candidate)
{
	//Should check that the div between the max value and the min value of the two cells in the div cage make the target value.
	//Only 2 cells possible per cage. 
	int compared_value;
	for(int i=0;i<index;i++)
	{
		if(X[i].cage==X[index].cage) compared_value=X[i].value;
	}

	if((max(compared_value,candidate)/min(compared_value,candidate)) != X[index].target) return false;
	else return true;
}

int max(int a, int b)
{
	if(a>b) return a;
	else return b;
}

int min(int a, int b)
{
	if(a<b) return a;
	else return b;
}

void print_solution()
{
		for(int i = 0; i<n; i++)
	{
		for(int j=i*n;j<(i*n)+n;j++)
		{						
			cout<<"{"<< X[j].value << "}";
		}
		cout<<endl;
	}
	
	cout << " " << endl;
}
