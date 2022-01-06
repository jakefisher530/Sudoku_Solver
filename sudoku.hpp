#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <queue>

class Sudoku {
public:
	Sudoku();
	~Sudoku();
	Sudoku& operator=(const Sudoku& s);
	bool fileFill(std::string fileName, bool simple);
	void print(std::ostream &out) const;
	bool solve();	
private:
	struct Coordinate {
		uint8_t i;
		uint8_t j;
	};
	class Space {
	public:
		Space();
		~Space();
		Space& operator=(const Space& s);
		bool isSet() const;
		bool getValToSet(uint8_t &val);
		bool setVal(uint8_t val);
//		bool setI(uint8_t i);
//		bool setJ(uint8_t j);
		uint8_t getVal() const;
		uint8_t getNumPos() const;
//		uint8_t getI() const;
//		uint8_t getJ() const;
		bool notPos(uint8_t posVal);
		bool isPos(uint8_t posVal) const;
	private:
		uint16_t m_val;
		uint8_t m_numPos;
		bool m_isSet;
//		uint8_t m_i;
//		uint8_t m_j;
	};
	Space m_grid[9U][9U];
//	uint8_t m_solution[9U][9U];
	uint8_t m_numEmpty = 81U;
	std::queue<Coordinate> m_readyQueue;
	
	bool addNumSuper(uint8_t newNum, uint8_t rowIdx, uint8_t colIdx);
	bool addNum(uint8_t newNum, uint8_t rowIdx, uint8_t colIdx);
	bool notPos(uint8_t posVal, uint8_t rowIdx, uint8_t colIdx);
	int quantifyRow(char rowC) const;
//	void fillEmpty();
	bool isSolved() const;
//	void setSolution();
};
#endif // SUDOKU_H

