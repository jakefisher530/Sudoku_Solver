#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>
#include <fstream>
#include <iostream>
#include <queue>

class Sudoku {
public:
	Sudoku();
	~Sudoku();
	Sudoku& operator=(const Sudoku& s);
	bool fileFill(std::string fileName, bool startSolve);
	void print(std::ostream& out) const;
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
		bool getValToSet(uint8_t& val);
		bool setVal(uint8_t val);
		uint8_t getVal() const;
		uint8_t getNumPos() const;
		bool notPos(uint8_t posVal);
		bool isPos(uint8_t posVal) const;
	private:
		uint16_t m_val;
		uint8_t m_numPos;
		bool m_isSet;
	};
	Space m_grid[9U][9U];
	uint8_t m_numEmpty;
	std::queue<Coordinate> m_readyQueue;
	bool addNumSuper(uint8_t newNum, uint8_t rowIdx, uint8_t colIdx);
	bool addNum(uint8_t newNum, uint8_t rowIdx, uint8_t colIdx);
	bool notPos(uint8_t posVal, uint8_t rowIdx, uint8_t colIdx);
	bool isSolved() const;
};
#endif // SUDOKU_H

