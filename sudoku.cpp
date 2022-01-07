#include "sudoku.hpp"
#include <iomanip>
#include <cmath>
#include <cstdlib>

Sudoku::Sudoku() {
	m_numEmpty = 81U;
}

Sudoku::~Sudoku() {}

Sudoku& Sudoku::operator=(const Sudoku& s) {
	if (this == &s) {
        return *this;
	}
	this->m_numEmpty = s.m_numEmpty;
	this->m_readyQueue = s.m_readyQueue;
	for (uint8_t i = 0U; i < 9U; i++) {
		for (uint8_t j = 0U; j < 9U; j++) {
			this->m_grid[i][j] = s.m_grid[i][j];
		}
	}
	return *this;
}

bool Sudoku::fileFill(std::string fileName, bool startSolve) {
	char rowC = '@';
	int rowI = -1;
	int column = -1;
	int given = -1;
	std::ifstream fin(fileName.c_str());
	if (!fin) {
		return false;
	}
	while (fin >> rowC >> column >> given) {
		if (rowC < 'Z') {
			rowI = rowC - 'A';
		} else {
			rowI = rowC - 'a';
		}
		if ((rowI < 0) || (rowI > 8)) {
			return false;
		}
		if ((column < 1) || (column > 9)) {
			return false;
		}
		column--;
		if ((given < 1) || (given > 9)) {
			return false;
		}
		if (startSolve) {
			if (!addNumSuper(given, rowI, column)) {
				return false;
			}
		} else {
			if (!m_grid[rowI][column].setVal(given)) {
				return false;
			}
		}
	}
	return true;
}

void Sudoku::print(std::ostream& out) const {
	for (uint8_t i = 0U; i < 9U; i++) {
		for (uint8_t j = 0U; j < 9U; j++) {
			out << static_cast<int>(m_grid[i][j].getVal()) << " ";
		}
		out << std::endl;
	}
	out << std::endl;
}

bool Sudoku::solve() {
	if (isSolved()) {
		return true;
	} else {
		for (uint8_t i = 0U; i < 9U; i++) {
			for (uint8_t j = 0U; j < 9U; j++) {
				if (!m_grid[i][j].isSet()) {
					for (uint8_t val = 1U; val <= 9U; val++) {
						if (m_grid[i][j].isPos(val)) {
							Sudoku guess = *this;
							if (!guess.addNumSuper(val, i, j)) {
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
	return true;
}

bool Sudoku::addNum(uint8_t newNum, uint8_t rowIdx, uint8_t colIdx) {
	if (!m_grid[rowIdx][colIdx].setVal(newNum)) {
		return false;
	}
	for (uint8_t i = 0U; i < 9U; i++) {
		if (!notPos(newNum, rowIdx, i)) {
			return false;
		}
		if (!notPos(newNum, i, colIdx)) {
			return false;
		}
	}
	for (uint8_t i = ((rowIdx / 3) * 3); i < (((rowIdx / 3) * 3) + 3); i++) {
		for (uint8_t j = ((colIdx / 3) * 3); j < (((colIdx / 3) * 3) + 3); j++) {
			if (!notPos(newNum, i, j)) {
				return false;
			}
		}
	}
	m_numEmpty--;
	return true;
}

bool Sudoku::notPos(uint8_t posVal, uint8_t rowIdx, uint8_t colIdx) {
	uint8_t prevNumPos = m_grid[rowIdx][colIdx].getNumPos();
	if (!m_grid[rowIdx][colIdx].notPos(posVal)) {
		return false;
	}
	if ((prevNumPos == 2U) && (m_grid[rowIdx][colIdx].getNumPos() == 1U)) {
		m_readyQueue.push({ .i = rowIdx, .j = colIdx });
	}
	return true;
}

Sudoku::Space::Space() {
	m_val = 0U;
	m_numPos = 9U;
	m_isSet = false;
}

Sudoku::Space::~Space() {}

Sudoku::Space& Sudoku::Space::operator=(const Sudoku::Space& s) {
	if (this == &s) {
        return *this;
	}
	this->m_val = s.m_val;
	this->m_numPos = s.m_numPos;
	this->m_isSet = s.m_isSet;
	return *this;
}

bool Sudoku::Space::isSet() const {
	return m_isSet;
}

bool Sudoku::Space::setVal(uint8_t val) {
	if ((val < 1U) || (val > 9U)) {
		return false;
	}
	if (isSet()) {
		return (val == m_val);
	}
	m_val = val;
	m_numPos = 1U;
	m_isSet = true;
	return true;
}

uint8_t Sudoku::Space::getVal() const {
	if (isSet()) {
		return m_val;
	}
	return 0U;
}

uint8_t Sudoku::Space::getNumPos() const {
	return m_numPos;
}

bool Sudoku::Space::notPos(uint8_t posVal) {
	if ((posVal < 1U) || (posVal > 9U)) {
		return false;
	}
	if (isPos(posVal)) {
		m_val |= (1U << (posVal - 1U));
		m_numPos--;
	}
	return true;
}

bool Sudoku::Space::isPos(uint8_t posVal) const {
	if (isSet() || (posVal < 1U) || (posVal > 9U)) {
		return false;
	}
	return ((m_val & (1U << (posVal - 1U))) == 0U);
}

bool Sudoku::Space::getValToSet(uint8_t& val) {
	if (isSet() || (m_numPos != 1U)) {
		return false;
	}
	for (uint8_t i = 0U; i < 9U; i++) {
		if ((m_val & (1U << i)) == 0U) {
			val = i + 1U;
			return true;
		}
	}
	return false;
}

