#include "sudoku.hpp"
#include <cstdlib>

using namespace std;

#define NUM_FILES (10U)

int main() {
	string fileNames[NUM_FILES] = {"givens_07-05-2016.txt", "givens_15-08-2016.txt", "givens_easy.txt", "givens_expert.txt", "givens_extreme.txt", "givens_hard.txt", "givens_medium.txt", "givens_original.txt", "givens_nyt_hard.txt", "givens_nyt_medium.txt"};
	Sudoku s;
	for (uint8_t i = 0U; i < NUM_FILES; i++) {
		string fileName = fileNames[i];
		cout << "=====================" << endl << fileName << endl << "=====================" << endl << endl;
		if (!s.fileFill(fileName, true)) {
			cout << "Error with file!" << endl;
			return EXIT_FAILURE;
		}
		s.print(cout);
		s = Sudoku();
		if (!s.fileFill(fileName, false)) {
			cout << "Error while solving!" << endl;
			return EXIT_FAILURE;
		}
		if (!s.solve()) {
			cout << "Unable to solve!" << endl;
			return EXIT_FAILURE;
		}
		s.print(cout);
		s = Sudoku();
		cout << endl << endl;
	}
	return EXIT_SUCCESS;
}

