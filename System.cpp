#include "System.h"

void System::operate() {
	const string openRegExpr = R"(^open \"((?:[^\"\s]|[ ])+)|(\"([^\"]+)\")\"$)";
	const string saveRegExpr = R"(^save ([1-9]\d*|0) \"((?:[^\"\s]|[ ])+)|(\"([^\"]+)\")\"$)";
	const string quitRegExpr = R"(^quit$)";
	const string readRegExpr = R"(^read$)";
	const string printRegExpr = R"(^print ([1-9]\d*|0)$)";
	const string startRegExpr = R"(^start ([1-9]\d*|0)$)";
	const string startForRegExpr = R"(^startFor ([1-9]\d*|0) \"([\w !@#$%^&*()+-_]*)\"$)";
	const string composeRegExpr = R"(^compose ([1-9]\d*|0) ([1-9]\d*|0)$)";
	const string whileRegExpr = R"(^while ([1-9]\d*|0) ([1-9]\d*|0)$)";
	const string ifRegExpr = R"(^if ([1-9]\d*|0) ([1-9]\d*|0) ([1-9]\d*|0)$)";

	const regex openPattern = regex(openRegExpr);
	const regex savePattern = regex(saveRegExpr);
	const regex readPattern = regex(readRegExpr);
	const regex quitPattern = regex(quitRegExpr);
	const regex printPattern = regex(printRegExpr);
	const regex composePattern = regex(composeRegExpr);
	const regex whilePattern = regex(whileRegExpr);
	const regex ifPattern = regex(ifRegExpr);
	const regex startPattern = regex(startRegExpr);
	const regex startForPattern = regex(startForRegExpr);

	cout << BRIGHT_BLUE_TEXT << INDENT << "- Welcome to TuringMachinizator -" << endl << RESET_COLORING;

	sregex_iterator iter;
	const sregex_iterator end; //end-of-sequence iterator used to determine whether matches were found
	string command;

	while (true) {
		cout << '>';
		getline(cin, command);
		if ((iter = sregex_iterator(command.begin(), command.end(), quitPattern)) != end) { //quit
			break;
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), openPattern)) != end) { //open
			string filename = (*iter)[1];
			ifstream ifs(filename);
			if (ifs.good()) {
				parser.parseFromFile(ifs);
				try {
					Machine m = builder.getMachine();
					machines.push_back(m);
				}
				catch (invalid_argument ia) {
				}
				ifs.close();
			}
			else {
				cerr << RED_TEXT << "Loading machine from \"" << filename << "\" was not successful!" << RESET_COLORING << endl;
			}
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), savePattern)) != end) { // save
			unsigned id = std::stoi((*iter)[1]);
			string filename = (*iter)[2];
			ofstream ofs(filename);
			if (ofs.good()) {
				try {
					Machine m = getMachine(id);
					m.save(ofs);
					cout << GREEN_TEXT << "Machine " << m.getId() << " was saved in \"" << filename << "\"!" << RESET_COLORING << endl;
					ofs.close();
				}
				catch (invalid_argument ia) {
					cerr << RED_TEXT << ia.what() << RESET_COLORING << endl;
					ofs.close();
					std::remove(filename.c_str());
				}
			}
			else {
				cerr << RED_TEXT << "Cannot save machine to \"" << filename << "\" !" << RESET_COLORING << endl;
				ofs.close();
				std::remove(filename.c_str());
			}
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), readPattern)) != end) { // read
			try {
			parser.parseFromConsole();
			Machine m = builder.getMachine();
			machines.push_back(m);
			}
			catch (invalid_argument ia) {

			}
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), printPattern)) != end) { // print
			unsigned id = std::stoi((*iter)[1]);
			try {
				getMachine(id).print();
			} catch (invalid_argument ia) {
				cerr << RED_TEXT << ia.what() << RESET_COLORING << endl;
			}
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), composePattern)) != end) { // compose
			unsigned id1 = std::stoi((*iter)[1]);
			unsigned id2 = std::stoi((*iter)[2]);
			try {
				Machine composed = getMachine(id1).compose(getMachine(id2));
				machines.push_back(composed);
				cout << GREEN_TEXT << "Composition machine with id " << composed.getId() << " was added!" << RESET_COLORING << endl;;
			}
			catch (invalid_argument ia) {
				cerr << RED_TEXT << ia.what() << RESET_COLORING << endl;
			}
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), whilePattern)) != end) { // while
			unsigned id1 = std::stoi((*iter)[1]);
			unsigned id2 = std::stoi((*iter)[2]);
			try {
				Machine whileMachine = getMachine(id1).whileMachine(getMachine(id2));
				machines.push_back(whileMachine);
				cout << GREEN_TEXT << "While machine with id " << whileMachine.getId() << " was added!" << RESET_COLORING << endl;
			}
			catch (invalid_argument ia) {
				cerr << RED_TEXT << ia.what() << RESET_COLORING << endl;
			}
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), startPattern)) != end) { // start
			unsigned id = std::stoi((*iter)[1]);
			try {
				cout << BRIGHT_BLUE_TEXT << "Tape: " << getMachine(id).executeAndGetTape() << RESET_COLORING << endl;
			}
			catch (invalid_argument ia) {
				cerr << RED_TEXT << ia.what() << RESET_COLORING << endl;
			}
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), startForPattern)) != end) { // startFor
			unsigned id = std::stoi((*iter)[1]);
			string tape_load = (*iter)[2];
			try {
				Machine m = getMachine(id);
				string res = m.executeAndGetTape(tape_load);
				cout << GREEN_TEXT << "Machine " + std::to_string(m.getId()) + " executed succesfully" << RESET_COLORING << endl;
				cout << BRIGHT_CYAN_TEXT << "Tape: " << res << RESET_COLORING << endl;
			}
			catch (invalid_argument ia) {
				cerr << RED_TEXT << ia.what() << RESET_COLORING << endl;
			}
		}
		else if ((iter = sregex_iterator(command.begin(), command.end(), ifPattern)) != end) { // if
			unsigned id1 = std::stoi((*iter)[1]);
			unsigned id2 = std::stoi((*iter)[2]);
			unsigned id3 = std::stoi((*iter)[3]);
			try {
				if (getMachine(id1).execute()) {
					cout << BRIGHT_BLUE_TEXT << "Tape: " << getMachine(id2).executeAndGetTape() << RESET_COLORING << endl;
				}
				else {
					cout << BRIGHT_BLUE_TEXT << "Tape: " << getMachine(id2).executeAndGetTape() << RESET_COLORING << endl;
				}
			}
			catch (invalid_argument ia) {
				cerr << RED_TEXT << ia.what() << RESET_COLORING << endl;
			}
		}
		else {
		cerr << RED_TEXT << "Ivalid command!" << RESET_COLORING << endl;
}
	}
}

Machine& System::getMachine(unsigned id) {
	for (auto& m : machines) {
		if (m.getId() == id) {
			return m;
		}
	}
	string err = "Machine with id " + std::to_string(id);
	err +=" does not exist!";
	throw invalid_argument(err);
}
