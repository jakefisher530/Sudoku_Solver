#include "sudoku.hpp"
#include <cstdlib>

using namespace std;

int main(){
	cout << "Select file from which to read " << endl;
	string fileName;
	cin >> fileName;
	cout << endl;
	Sudoku s(fileName);
	s.print(cout);
	/*if(s.solve()){
		cout << "Solved! " << endl;
		s.print(cout);
	} else{
		cout << "Unable to solve " << endl;
		s.print(cout);
	}*/
	return EXIT_SUCCESS;
}
