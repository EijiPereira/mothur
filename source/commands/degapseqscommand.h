#ifndef DEGAPSEQSCOMMAND_H
#define DEGAPSEQSCOMMAND_H

/*
 *  degapseqscommand.h
 *  Mothur
 *
 *  Created by westcott on 6/21/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */


#include "command.hpp"
#include "sequence.hpp"

class DegapSeqsCommand : public Command {
public:
	DegapSeqsCommand(string);
	DegapSeqsCommand();
	~DegapSeqsCommand(){}
	
	vector<string> setParameters();
	string getCommandName()			{ return "degap.seqs";		}
	string getCommandCategory()		{ return "Sequence Processing";		}
	
	string getHelpString();	
    string getOutputPattern(string);	
	string getCitation() { return "http://www.mothur.org/wiki/Degap.seqs"; }
	string getDescription()		{ return "removes gap characters from sequences"; }

	int execute(); 
	void help() { m->mothurOut(getHelpString()); }	
	
private:
    
    int processors;
	bool abort;
	string fastafile, outputDir;
	vector<string> outputNames;
	vector<string> fastaFileNames;
    
    int createProcesses(string, string);
	
};
#endif


