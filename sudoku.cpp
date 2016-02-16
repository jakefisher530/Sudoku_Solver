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
		for(int i=((rowIdx/3)*3); i<(((rowIdx/3)*3)+3); i++){
			for(int j=((colIdx/3)*3); j<(((colIdx/3)*3)+3); j++){
				grid_[i][j].notPos(newNum);			
			}
		}
		numEmpty_--;
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
	bool isNumAdded = false;
	short totPos = 0, numPos = 0, rowIdx = 0, colIdx = 0; 	
	for(int i=0; i<=6; i+=3){
		for(int j=0; j<=6; j+=3){
			for(int num=1; num<=9; num++){
				for(int r=i; r<i+3; r++){
					for(int c=j; c<j+3; c++){
						if(grid_[r][c].isPos(num)){
							totPos++;
							numPos = num;
							rowIdx = r;
							colIdx = c;
						}
					}
				}
				if(totPos == 1){
					isNumAdded = addNum(numPos, rowIdx, colIdx);
				}
				totPos = 0;
				numPos = 0;
				rowIdx = 0;	
				colIdx = 0;
			}
		}
	}
	return isNumAdded;
}

bool Sudoku::rowCheck(){
	bool isNumAdded = false;
	short totPos = 0, numPos = 0, posIdx = 0; 	
	for(int i=0; i<9; i++){
			for(int num=1; num<=9; num++){
				for(int j=0; j<9; j++){
					if(grid_[i][j].isPos(num)){
						totPos++;
						numPos = num;
						posIdx = j;
					}
				}
				if(totPos == 1){
					isNumAdded = addNum(numPos, i, posIdx);
				}
				totPos = 0;
				numPos = 0;
				posIdx = 0;				
			}
	}
	return isNumAdded;
}

bool Sudoku::columnCheck(){
	bool isNumAdded = false;
	short totPos = 0, numPos = 0, posIdx = 0; 	
	for(int j=0; j<9; j++){
			for(int num=1; num<=9; num++){
				for(int i=0; i<9; i++){
					if(grid_[i][j].isPos(num)){
						totPos++;
						numPos = num;
						posIdx = i;
					}
				}
				if(totPos == 1){
					isNumAdded = addNum(numPos, posIdx, j);
				}
				totPos = 0;
				numPos = 0;
				posIdx = 0;				
			}
	}
	return isNumAdded;
}

bool Sudoku::notCheck(){
	bool isNumAdded = false;
	short totPos = 0, numPos = 0; 
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			for(short num=1; num<=9; num++){
				if(grid_[i][j].isPos(num)){
					totPos++;
					numPos = num;
				}
			}
			if(totPos == 1){
				isNumAdded = addNum(numPos, i, j);
			}
			totPos = 0;
			numPos = 0;
		}
	}
	return isNumAdded;
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
	if(1 <= posVal && posVal <= 9 && val_ <= 0){
		if(this->Space::isPos(posVal)){
			val_ += pow(2,(posVal-1));			
		}
		return true;
	}
	return false;
}

bool Sudoku::Space::isPos(short posVal) const{
	if(1 <= posVal && posVal <= 9 && val_ <= 0){
		short check = pow(2,(posVal-1));
		if(!(check & val_)){
			return true;
		}
	}
	return false;
}

