#include "sudoku.hpp"
#include <iomanip>
#include <iostream>
#include <fstream>
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
	std::ifstream fin (fileName.c_str());
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
		grid_[rowI-1][column-1] = given;
		numEmpty_--;
	}
	return true;
}

void Sudoku::print() const{
	for (int i=0; i < 9; i++){
		for (int j=0; j < 9; j++){
			std::cout << grid_[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool Sudoku::solve(){
	bool isNumAdded = true;
	while (isNumAdded){
		isNumAdded = boxCheck() || rowCheck() || columnCheck() || notCheck();
	}
	return (numEmpty_ == 0);		
}


int Sudoku::quantifyRow(char rowC) const{
	if (rowC < 90)
		return rowC - 64;
	return rowC - 96;
}

void Sudoku::fillEmpty(){
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			grid_[i][j] = 0;
		}
	}
	numEmpty_ = 81;	
}

bool Sudoku::boxCheck()
{
	bool isNumAdded = false;
	for (int startColumn = 0; startColumn <= 6; startColumn +=3)
	{
		for (int startRow = 0; startRow <= 6; startRow +=3)
		{
			for (int currentNum = 1; currentNum <= 9; currentNum++)
			{
				bool isFound = false;
				bool possible [3][3];
				for (int i=0; i < 3; i++)
					for (int j=0; j < 3; j++)
						possible [i][j] = true;
				int numPossible = 9;
				for (int currentRow = startRow; currentRow < startRow + 3; currentRow++)
				{
					for (int currentColumn = startColumn; currentColumn < startColumn + 3; currentColumn++)
					{
						if (grid_[currentRow][currentColumn] != 0)
						{
							possible [currentRow-startRow][currentColumn-startColumn] = false;
							numPossible--;
						}
						if (grid_[currentRow][currentColumn] == currentNum)
							isFound = true;
					}
				}
				if (!isFound && numPossible > 1)
				{
					int cluesFound = 0, clueRow1, clueRow2;
					for (int scanRow = startRow; scanRow <= startRow + 2; scanRow++)
					{
						for (int scanColumn = 0; scanColumn < 9; scanColumn++)
						{
							if (grid_[scanRow][scanColumn] == currentNum && cluesFound == 1)
							{
								cluesFound++;
								clueRow2 = scanRow - startRow;
							}
							if (grid_[scanRow][scanColumn] == currentNum && cluesFound == 0)
							{
								cluesFound++;
								clueRow1 = scanRow - startRow;
							}	
						}
					}
					if (cluesFound > 0)
					{
						for (int j=0; j<3; j++)
						{
							numPossible -= possible [clueRow1][j];
							possible [clueRow1][j] = false;
						}
						if (cluesFound == 2)
						{
							for (int j=0; j<3; j++)
							{
								numPossible -= possible [clueRow2][j];
								possible [clueRow2][j] = false;
							}
						}
					}
					cluesFound = 0;
					int clueColumn1, clueColumn2;
					for (int scanColumn = startColumn; scanColumn <= startColumn + 2; scanColumn++)
					{
						for (int scanRow = 0; scanRow < 9; scanRow++)
						{
							if (grid_[scanRow][scanColumn] == currentNum && cluesFound == 1)
							{
								cluesFound++;
								clueColumn2 = scanColumn - startColumn;
							}
							if (grid_[scanRow][scanColumn] == currentNum && cluesFound == 0)
							{
								cluesFound++;
								clueColumn1 = scanColumn - startColumn;
							}
						}
					}
					if (cluesFound > 0)
					{
						for (int i=0; i<3; i++)
						{
							numPossible -= possible [i][clueColumn1];
							possible [i][clueColumn1] = false;
						}
						if (cluesFound == 2)
						{
							for (int i=0; i<3; i++)
							{
								numPossible -= possible [i][clueColumn2];
								possible [i][clueColumn2] = false;
							}
						}
					}	
				}
				if (!isFound && numPossible == 1)
				{
					for (int i=0; i<3; i++)
						for (int j=0; j<3; j++)
							if (possible [i][j])
								grid_[startRow + i][startColumn + j] = currentNum;
					numEmpty_--;
					isNumAdded = true;					
				}
			}
		}
	}
	return isNumAdded;
}

bool Sudoku::rowCheck()
{
	bool isNumAdded = false, isFound = false;
	bool possible [9];
	for (int row = 0; row < 9; row++)
	{
		for (int currentNum = 1; currentNum <= 9; currentNum++)
		{
			isFound = false;
			for (int i=0; i < 9; i++)
				possible [i] = true;
			int numPossible = 9;
			for (int i=0; i<9; i++)
			{
				if (grid_[row][i] != 0)
				{
					possible [i] = false;
					numPossible--;
				}
				if (grid_[row][i] == currentNum)
					isFound = true;
			}
			for (int i=0; i<9; i++)
				for (int j=0; j<9; j++)
					if (grid_[i][j] == currentNum)
					{
						numPossible -= possible [j];
						possible [j] = false;
					}
			int startRow = (row/3)*3;
			for (int rowCheck = startRow; rowCheck <= startRow + 2; rowCheck++)
			{
				for (int i=0; i<3; i++)
					if (grid_[rowCheck][i] == currentNum)
						for (int j=0; j<3; j++)
						{
							numPossible -= possible [j];
							possible [j] = false;
						}
				for (int i=3; i<6; i++)
					if (grid_[rowCheck][i] == currentNum)
						for (int j=3; j<6; j++)
						{
							numPossible -= possible [j];
							possible [j] = false;
						}
				for (int i=6; i<9; i++)
					if (grid_[rowCheck][i] == currentNum)
						for (int j=6; j<9; j++)
						{
							numPossible -= possible [j];
							possible [j] = false;
						}
			}
			if (!isFound && numPossible == 1)
			{
				for (int i=0; i<9; i++)
					if (possible [i])
						grid_[row][i] = currentNum;
				numEmpty_--;
				isNumAdded = true;
			}
		}
	}
	return isNumAdded;
}

bool Sudoku::columnCheck()
{
	bool isNumAdded = false, isFound = false;
	bool possible [9];
	for (int column = 0; column < 9; column++)
	{
		for (int currentNum = 1; currentNum <= 9; currentNum++)
		{
			isFound = false;
			for (int i=0; i < 9; i++)
				possible [i] = true;
			int numPossible = 9;
			for (int i=0; i<9; i++)
			{
				if (grid_[i][column] != 0)
				{
					possible [i] = false;
					numPossible--;
				}
				if (grid_[i][column] == currentNum)
					isFound = true;
			}
			for (int i=0; i<9; i++)
				for (int j=0; j<9; j++)
					if (grid_[i][j] == currentNum)
					{
						numPossible -= possible [i];
						possible [i] = false;
					}
			int startColumn = (column/3)*3;
			for (int columnCheck = startColumn; columnCheck <= startColumn + 2; columnCheck++)
			{
				for (int i=0; i<3; i++)
					if (grid_[i][columnCheck] == currentNum)
						for (int j=0; j<3; j++)
						{
							numPossible -= possible [j];
							possible [j] = false;
						}
				for (int i=3; i<6; i++)
					if (grid_[i][columnCheck] == currentNum)
						for (int j=3; j<6; j++)
						{
							numPossible -= possible [j];
							possible [j] = false;
						}
				for (int i=6; i<9; i++)
					if (grid_[i][columnCheck] == currentNum)
						for (int j=6; j<9; j++)
						{
							numPossible -= possible [j];
							possible [j] = false;
						}
			}
			if (!isFound && numPossible == 1)
			{
				for (int i=0; i<9; i++)
					if (possible [i])
						grid_[i][column] = currentNum;
				numEmpty_--;
				isNumAdded = true;
			}
		}
	}
	return isNumAdded;
}

bool Sudoku::notCheck()
{
	bool isNumAdded = false;
	bool possible [9];
	int numPossible = 9;
	for (int row=0; row<9; row++)
		for (int column=0; column<9; column++)
			if (grid_[row][column] == 0)
			{
				for (int i=0; i<9; i++)
					possible [i] = true;
				numPossible = 9;
				for (int rowScan=0; rowScan < 9; rowScan++)
					if (grid_[rowScan][column] != 0)
					{
						numPossible -= possible [grid_[rowScan][column] - 1];
						possible [grid_[rowScan][column] - 1] =  false;
					}
				for (int columnScan=0; columnScan < 9; columnScan++)
					if (grid_[row][columnScan] != 0)
					{
						numPossible -= possible [grid_[row][columnScan] - 1];
						possible [grid_[row][columnScan] - 1] = false;
					}
				int rowStart =(row/3)*3, columnStart =(column/3)*3; 
				for (int rowBox = rowStart; rowBox <= rowStart + 2; rowBox++)
					for (int columnBox = columnStart; columnBox <= columnStart + 2; columnBox++)
						if (grid_[rowBox][columnBox] != 0)
						{
							numPossible -= possible [grid_[rowBox][columnBox] - 1];
							possible [grid_[rowBox][columnBox] - 1] = false;
						}
				if (numPossible == 1)
				{
					for (int i=0; i<9; i++)
						if (possible [i])
							grid_[row][column] = i + 1;
					numEmpty_--;
					isNumAdded = true;							
				}
			}
	return isNumAdded;
}

