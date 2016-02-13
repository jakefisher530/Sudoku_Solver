#include "sudoku.hpp"
#include <iomanip>
#include <cmath>
#include <cstdlib>


Sudoku::Sudoku(){
	fillEmpty();
}

Sudoku::Sudoku(std::string fileName){
	fillEmpty();
	this->fileFill(fileName);
}

Sudoku::~Sudoku(){}

bool Sudoku::fileFill(std::string fileName){
	std::ifstream fin(fileName.c_str());
	if(!fin){
		return false;
	}
	char rowC;
	int rowI, column, given;
	while (fin >> rowC >> column >> given){
		rowI = quantifyRow(rowC);
		if(rowI < 1 || rowI > 9){
			return false;
		}
		if (column < 1 || column > 9){
			return false;
		}
		addNum(given, rowI-1, column-1);
		numEmpty_--;
	}
	return true;
}

void Sudoku::print(std::ostream &out) const{
	for (int i=0; i < 9; i++){
		for (int j=0; j < 9; j++){
			out << grid_[i][j].getVal() << " ";
		}
		out << std::endl;
	}
	out << std::endl;
}

bool Sudoku::solve(){
	bool isNumAdded = true;
	while (isNumAdded){
		isNumAdded = boxCheck() || rowCheck() || columnCheck() || notCheck();
	}
	return (numEmpty_ == 0);		
}

bool Sudoku::addNum(short newNum, short rowIdx, short colIdx){
	if(grid_[rowIdx][colIdx].setVal(newNum)){
		for(int i=0; i<9; i++){
			grid_[rowIdx][i].notPos(newNum);
			grid_[i][colIdx].notPos(newNum);
		}
		for(int i=rowIdx/3; i<((rowIdx/3)+3); i++){
			for(int j=colIdx/3; j<((colIdx+3)+3); j++){
				grid_[i][j].notPos(newNum);
			}
		}
		return true;
	}
	return false;
}

int Sudoku::quantifyRow(char rowC) const{
	if (rowC < 90)
		return rowC - 64;
	return rowC - 96;
}

void Sudoku::fillEmpty(){
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			addNum(0, i, j);
		}
	}
	numEmpty_ = 81;	
}

bool Sudoku::boxCheck(){
}

bool Sudoku::rowCheck(){
}

bool Sudoku::columnCheck(){
}

bool Sudoku::notCheck(){
}

Sudoku::Space::Space(){
	val_ = -32768;
}

Sudoku::Space::~Space(){}

bool Sudoku::Space::isSet() const{
	return (val_ > 0);
}

bool Sudoku::Space::setVal(short setVal){
	if(1 <= setVal && setVal <= 9){
		val_ = setVal;
		return true;
	}
	return false;
}

short Sudoku::Space::getVal() const{
	if(val_ > 0){
		return val_;
	}
	return 0;
}

bool Sudoku::Space::notPos(short posVal){
	if(1 <= posVal && posVal <= 9 && val_ < 0){
		val_ += pow(2,(posVal-1));
		return true;
	}
	return false;
}

bool Sudoku::Space::isPos(short posVal) const{
	if(1 <= posVal && posVal <= 9){
		short check = pow(2,(posVal-1));
		if(check & val_){
			return true;
		}
	}
	return false;
}

