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
		bool isSet() const;
		bool setVal(short setVal);
		short getVal() const;
		bool notPos(short posVal);
		bool isPos(short posVal) const;
	private:
		short val_;
	};
	Space grid_[9][9];
	int numEmpty_;
	
	bool addNum(short newNum, short rowIdx, short colIdx);
	int quantifyRow(char rowC) const;
	void fillEmpty();
	bool boxCheck();
	bool rowCheck();
	bool columnCheck();
	bool notCheck();		
};
#endif
