#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>

class Sudoku{
public:
	Sudoku();
	Sudoku(std::string fileName);
	~Sudoku();
	bool fileFill(std::string fileName);
	void print() const;
	bool solve();	
private:
	int grid_[9][9];
	int numEmpty_;
	
	int quantifyRow(char rowC) const;
	void fillEmpty();
	bool boxCheck();
	bool rowCheck();
	bool columnCheck();
	bool notCheck();		
};
#endif
