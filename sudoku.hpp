#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>
#include <fstream>
#include <iostream>

class Sudoku{
public:
	Sudoku();
	Sudoku(std::string fileName);
	~Sudoku();
	bool fileFill(std::string fileName);
	void print(std::ostream &out) const;
	bool solve();	
private:
	class Space{
	public:
		Space();
		~Space();
		bool isSet();
		bool setVal(short setVal);
		short getVal();
		bool notPos(int posVal);
		bool isPos(int posVal);
	private:
		short val_;
	};
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
