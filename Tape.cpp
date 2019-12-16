#include "Tape.h"

#define TAPE_CELL  Tape::TapeCell


Tape::Tape(string str, char blank) : Tape(blank) {
	insertStringToTape(str);
}

Tape::Tape(const Tape& other) : blank_symbol(other.blank_symbol){
	head = copyOther(other);
}

Tape& Tape::operator=(const Tape& other) {
	if (this != &other) {
		deleteContent();
		head = copyOther(other);
		blank_symbol = other.blank_symbol;
	}
	return *this;
}

Tape::~Tape() {
	deleteContent();
}

void Tape::goLeft() {
	if (!head->left) {
		return;
	}
	head = head->left;
}

void Tape::goRight() {
	if (!head->right) {
		head->right = new TapeCell(head, nullptr, blank_symbol);
	}
	head = head->right;
}

void Tape::replaceSymbol(char new_symbol) {
	head->symbol = new_symbol;
}

char Tape::getSymbol() const {
	return head->symbol;
}

Tape::operator string() const {
	TapeCell* ptr = getFirst();
	string* str = new string();
	while (ptr != nullptr) {
		*str += ptr->symbol;
		ptr = ptr->right;
	}
	return *str;
}


void Tape::insertStringToTape(const string& str) {
	TapeCell* begin = head;
	for (char c : str) {
		head->symbol = c;
		if (head->right == nullptr) {
			head->right = new TapeCell(head, nullptr, blank_symbol);
		}
		head = head->right;
	}
	head = begin;
}

TAPE_CELL* Tape::copyOther(const Tape& other) {
	TapeCell* curr_head = new TapeCell(nullptr, nullptr, blank_symbol);
	TapeCell* other_head = other.getFirst();
	TapeCell* head_place = nullptr;
	while (other_head != nullptr) {
		curr_head->symbol = other_head->symbol;
		if (other_head == other.head) {
			head_place = curr_head;
		}
		other_head = other_head->right;

		curr_head->right = (other_head == nullptr) ? nullptr : new TapeCell(curr_head, nullptr, blank_symbol);
		curr_head = curr_head->right;
	}
	return head_place;
}

void Tape::deleteContent() {
	head = getFirst();
	while (head != nullptr) {
		TapeCell* next = head->right;
		delete head;
		head = next;
	}
}

TAPE_CELL* Tape::getFirst() const{
	TapeCell* ptr = head;
	while (ptr->left != nullptr) {
		ptr = ptr->left;
	}
	return ptr;
}
