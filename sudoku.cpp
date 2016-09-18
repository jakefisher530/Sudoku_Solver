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
	short ctr = 0;
	while(isNumAdded){
		pairCheck();
		if(!(boxCheck() || rowCheck() || columnCheck() || notCheck())){
			ctr++;
		}else{
			ctr = 0;
		}
		if(ctr >= 5){
			isNumAdded = false;
		}
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
			grid_[i][j].setI(i);
			grid_[i][j].setJ(j);
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

void Sudoku::pairCheck(){
	Space set[9];
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			set[j] = grid_[i][j];
		}
		pairCheckHelper(&set[9]);
		for(int j=0; j<9; j++){
			set[j] = grid_[j][i];
		}
		pairCheckHelper(&set[9]);
	}
	short ctr = 0;
	for(int i=0; i<=6; i+=3){
		for(int j=0; j<=6; j+=3){
			for(int r=i; r<i+3; r++){
				for(int c=j; c<j+3; c++){
					set[ctr] = grid_[r][c];
					ctr++;
				}
			}
			pairCheckHelper(&set[9]);
			ctr = 0;
		}
	}
	return;
}

void Sudoku::pairCheckHelper(Space set[9]){	
	short ctr = 0, ctr2 = 0;
	short idx[9], nums[3];
	for(int i=0; i<9; i++){
		if(set[i].getNumPos() <= 3 && set[i].getNumPos() != 1){
			idx[ctr] = i;
			ctr++;
		}
	}
	for(int i=0; i<ctr; i++){
		short j = idx[i];
		if(set[j].getNumPos() == 2){
			for(int k=1; k<=9; k++){
				if(set[j].isPos(k)){
					nums[ctr2] = k;
					ctr2++;
				}
			}
			for(int w=i; w<ctr; w++){
				short x = idx[w];
				if(set[x].getNumPos() == 2){
					if(set[x].isPos(nums[0]) && set[x].isPos(nums[1])){
						for(int z=0; z<9; z++){
							if(z != j && z != x){
								grid_[set[z].getI()][set[z].getJ()].notPos(nums[0]);
								grid_[set[z].getI()][set[z].getJ()].notPos(nums[1]);	
							}	
						}
						if((x/3) == (j/3)){
							//interaction
							if((set[0].getI() == set[8].getI()) || (set[0].getJ() == set[8].getJ())){
								//row or column
								for(int m=(set[j].getI()/3); m<((set[j].getI()/3)+3); m++){
									for(int n=(set[j].getJ()/3); n<((set[j].getJ()/3)+3); n++){
										if((m != set[j].getI() || n != set[j].getJ()) && (m != set[x].getI() || n != set[x].getJ())){
											grid_[m][n].notPos(nums[0]);
											grid_[m][n].notPos(nums[1]);
										}
									}	
								}
							}else{
								//box horizontal
								for(int z=0; z<9; z++){
									if((z != set[j].getJ()) && (z != set[x].getJ())){
										grid_[set[j].getI()][z].notPos(nums[0]);
										grid_[set[j].getI()][z].notPos(nums[1]);
									}
								}
							}	
						}else if(((x%3) == (j%3)) && (set[0].getI() != set[8].getI()) && (set[0].getJ() != set[8].getJ())){
							//box vertical
							for(int z=0; z<9; z++){
								if((z != set[j].getI()) && (z != set[x].getI())){
									grid_[z][set[j].getJ()].notPos(nums[0]);
									grid_[z][set[j].getJ()].notPos(nums[1]);
								}
							}
						}
					}
				}
			}
		}
	}
	return;
}

Sudoku::Space::Space(){
	val_ = -32768;
	numPos_ = 9;
	i_ = -1;
	j_ = -1;
}

Sudoku::Space::~Space(){}

bool Sudoku::Space::isSet() const{
	return (val_ > 0);
}

bool Sudoku::Space::setVal(short setVal){
	if(1 <= setVal && setVal <= 9){
		val_ = setVal;
		numPos_ = 1;
		return true;
	}
	return false;
}

bool Sudoku::Space::setI(short i){
	if(0 <= i && i <= 8){
		i_ = i;
		return true;
	}
	return false;
}

bool Sudoku::Space::setJ(short j){
	if(0 <= j && j <= 8){
		j_ = j;
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

short Sudoku::Space::getNumPos() const{
	return numPos_;
}

short Sudoku::Space::getI() const{
	return i_;
}

short Sudoku::Space::getJ() const{
	return j_;
}

bool Sudoku::Space::notPos(short posVal){
	if(1 <= posVal && posVal <= 9 && val_ <= 0){
		if(this->Space::isPos(posVal)){
			val_ += pow(2,(posVal-1));
			numPos_--;			
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

