#ifndef SUDOKU_H
#define SUDOKU_H

class Sudoku{
public:
	Sudoku();
	~Sudoku();
	bool fileFill();
	void print() const;
	bool solve();	
private:
	int grid_[9][9];
	int numEmpty_;
	
	int quantifyRow (char rowC) const;
	bool boxCheck ();
	bool rowCheck ();
	bool columnCheck ();
	bool notCheck ();		
};
#endif
