#include "sudoku.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(){
	cout << "Select file from which to read " << endl;
	string fileName;
	cin >> fileName;
	cout << endl;
	Sudoku s(fileName);
	s.print();
	if(s.solve()){
		cout << "Solved! " << endl;
		s.print();
	} else{
		cout << "Unable to solve " << endl;
		s.print();
	}
	return EXIT_SUCCESS;
}
