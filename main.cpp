#include "sudoku.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(){
	Sudoku s;
	if(s.fileFill()){
		s.print();
		if(s.solve()){
			cout << "Solved! " << endl;
			s.print();
		} else{
			cout << "Unable to solve " << endl;
			s.print();
		}
	} else{
		cout << "Unable to read " << endl;
	}
	return EXIT_SUCCESS;
}
