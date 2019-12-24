#pragma once
#include <string>

using std::string;

class Tape {
public:
	Tape(char blank = ' ') : blank_symbol(blank), head(new TapeCell(nullptr, nullptr, blank)) {};
	Tape(const Tape& other);
	Tape(string, char = ' ');
	Tape& operator= (const Tape& other);
	~Tape();

	void goLeft();
	void goRight();
	void replaceSymbol(char new_symbol);
	char getSymbol() const;
	char getBlank() const { return blank_symbol; }
	void insertStringToTape(const string& str);

	operator string() const;

private:
	struct TapeCell {
		char symbol;
		TapeCell* left; 
		TapeCell* right;
		TapeCell(TapeCell* left, TapeCell* right, char symbol) : left(left), right(right), symbol(symbol) {};
	};

	TapeCell* head;


	TapeCell* copyOther(const Tape&);
	TapeCell* getFirst() const;
	void deleteContent();
	char blank_symbol = ' ';


};