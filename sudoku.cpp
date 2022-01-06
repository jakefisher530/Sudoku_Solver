#include "sudoku.hpp"
#include <iomanip>
#include <cmath>
#include <cstdlib>

Sudoku::Sudoku() {
//	fillEmpty();
}

Sudoku::~Sudoku() {}

Sudoku& Sudoku::operator=(const Sudoku& s) {
	if (this == &s) {
        return *this;
	}
	this->m_numEmpty = s.m_numEmpty;
//	std::cout << static_cast<int>(m_readyQueue.size()) << std::endl;
	this->m_readyQueue = s.m_readyQueue;
	for (uint8_t i = 0U; i < 9U; i++) {
		for (uint8_t j = 0U; j < 9U; j++) {
//			this->m_solution[i][j] = s.m_solution[i][j];
			this->m_grid[i][j] = s.m_grid[i][j];
		}
	}
	return *this;
}

bool Sudoku::fileFill(std::string fileName, bool simple) {
	std::ifstream fin(fileName.c_str());
	if (!fin) {
		return false;
	}
	char rowC;
	int rowI, column, given;
	while (fin >> rowC >> column >> given) {
		rowI = quantifyRow(rowC);
		if (rowI < 1 || rowI > 9) {
			return false;
		}
		if (column < 1 || column > 9) {
			return false;
		}
		if (simple) {
			if (!m_grid[rowI-1][column-1].setVal(given)) {
				return false;
			}
		} else {
			addNumSuper(given, rowI-1, column-1);
		}
	}
	return true;
}

void Sudoku::print(std::ostream &out) const {
	for (int i=0; i < 9; i++) {
		for (int j=0; j < 9; j++) {
//			int val = 0U;
//			if (isSolved()) {
//				val = m_solution[i][j];
//			} else {
//				val = m_grid[i][j].getVal();
//			}
			out << static_cast<int>(m_grid[i][j].getVal()) << " ";
		}
		out << std::endl;
	}
//	out << static_cast<unsigned int>(m_numEmpty) << std::endl;
	out << std::endl;
}

//void Sudoku::setSolution() {
//	for (uint8_t i = 0U; i < 9U; i++) {
//		for (uint8_t j = 0U; j < 9U; j++) {
//			m_solution[i][j] = m_grid[i][j].getVal();
//		}
//	}
//}

bool Sudoku::solve() {
	if (isSolved()) {
//		setSolution();
		return true;
	} else {
		for (uint8_t i = 0U; i < 9U; i++) {
			for (uint8_t j = 0U; j < 9U; j++) {
				if (!m_grid[i][j].isSet()) { // (getNumPos() == 1U)?
					for (uint8_t v = 1U; v <= 9U; v++) {
						if (m_grid[i][j].isPos(v)) {
							Sudoku guess = *this;
							if (!guess.addNumSuper(v, i, j)) {
								continue;
							}
							if (guess.solve()) {
								*this = guess;
								return true;
							}
						}
					}
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

bool Sudoku::isSolved() const {
	return (m_numEmpty == 0U);
}

bool Sudoku::addNumSuper(uint8_t newNum, uint8_t rowIdx, uint8_t colIdx) {
	if (!addNum(newNum, rowIdx, colIdx)) {
		return false;
	}
	while (!m_readyQueue.empty()) {
		Coordinate readyCoord = m_readyQueue.front();
		if (!m_grid[readyCoord.i][readyCoord.j].isSet()) {
			uint8_t valToSet = 0U;
			if (!m_grid[readyCoord.i][readyCoord.j].getValToSet(valToSet)) {
				return false;
			}
			if (!addNum(valToSet, readyCoord.i, readyCoord.j)) {
				return false;
			}
		}
		m_readyQueue.pop();
	}
//	print(std::cout);
	return true;
}

bool Sudoku::addNum(uint8_t newNum, uint8_t rowIdx, uint8_t colIdx) {
	if (m_grid[rowIdx][colIdx].setVal(newNum)) {
		for (int i=0; i<9; i++) {
			notPos(newNum, rowIdx, i);
			notPos(newNum, i, colIdx);
		}
		for (int i=((rowIdx/3)*3); i<(((rowIdx/3)*3)+3); i++) {
			for (int j=((colIdx/3)*3); j<(((colIdx/3)*3)+3); j++) {
				notPos(newNum, i, j);
			}
		}
//		std::cout << static_cast<unsigned int>(m_numEmpty) << "\t";
		m_numEmpty--;
//		std::cout << static_cast<unsigned int>(m_numEmpty) << std::endl;
		return true;
	}
	return false;
}

bool Sudoku::notPos(uint8_t posVal, uint8_t rowIdx, uint8_t colIdx) {
	uint8_t prevNumPos = m_grid[rowIdx][colIdx].getNumPos();
	if (m_grid[rowIdx][colIdx].notPos(posVal)) {
		if ((prevNumPos == 2U) && (m_grid[rowIdx][colIdx].getNumPos() == 1U)) {
			m_readyQueue.push({ .i = rowIdx, .j = colIdx });
		}
		return true;
	}
	return false;
}

int Sudoku::quantifyRow(char rowC) const {
	if (rowC < 'Z') {
		return rowC - 'A' + 1;
	}
	return rowC - 'a' + 1;
}

//void Sudoku::fillEmpty() {
//	for (int i=0; i<9; i++) {
//		for (int j=0; j<9; j++) {
//			m_grid[i][j].setI(i);
//			m_grid[i][j].setJ(j);
//			addNum(0U, i, j);
//		}
//	}
//	m_numEmpty = 81U;
//}

Sudoku::Space::Space() {
	m_val = 0U;
	m_numPos = 9U;
	m_isSet = false;
//	m_i = UINT8_MAX;
//	m_j = UINT8_MAX;
}

Sudoku::Space::~Space() {}

Sudoku::Space& Sudoku::Space::operator=(const Sudoku::Space& s) {
	if (this == &s) {
        return *this;
	}
	this->m_val = s.m_val;
	this->m_numPos = s.m_numPos;
	this->m_isSet = s.m_isSet;
//	this->m_i = s.m_i;
//	this->m_j = s.m_j;
	return *this;
}

bool Sudoku::Space::isSet() const {
	return m_isSet;
}

bool Sudoku::Space::setVal(uint8_t val) {
	if (m_isSet) {
		return false;
	}
	if ((val >= 1U) && (val <= 9U)) {
		m_val = val;
		m_numPos = 1U;
		m_isSet = true;
		return true;
//	} else if (val == 0U) {
//		m_val = val;
//		m_numPos = 9U;
//		m_isSet = true;
//		return true;
	}
	return false;
}

//bool Sudoku::Space::setI(uint8_t i) {
//	if ((i >= 0U) && (i <= 8U)) {
//		m_i = i;
//		return true;
//	}
//	return false;
//}
//
//bool Sudoku::Space::setJ(uint8_t j) {
//	if ((j >= 0U) && (j <= 8U)) {
//		m_j = j;
//		return true;
//	}
//	return false;
//}

uint8_t Sudoku::Space::getVal() const {
	if (isSet()) {
		return m_val;
	}
	return 0U;
}

uint8_t Sudoku::Space::getNumPos() const {
	return m_numPos;
}

//uint8_t Sudoku::Space::getI() const {
//	return m_i;
//}
//
//uint8_t Sudoku::Space::getJ() const {
//	return m_j;
//}

bool Sudoku::Space::notPos(uint8_t posVal) {
	if ((!isSet()) && (posVal >= 1U) && (posVal <= 9U)) {
		if (isPos(posVal)) {
			m_val |= (1U << (posVal - 1U));
			m_numPos--;
		}
		return true;
	}
	return false;
}

bool Sudoku::Space::isPos(uint8_t posVal) const {
	if ((!isSet()) && (posVal >= 1U) && (posVal <= 9U)) {
		if ((m_val & (1U << (posVal - 1U))) == 0U) {
			return true;
		}
	}
	return false;
}

bool Sudoku::Space::getValToSet(uint8_t &val) {
	if ((!isSet()) && (m_numPos == 1U)) {
		for (uint8_t i = 0U; i < 9U; i++) {
			if ((m_val & (1U << i)) == 0U) {
				val = i + 1U;
				return true;
			}
		}
	}
	return false;
}

