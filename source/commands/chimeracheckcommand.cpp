/*
 *  chimeracheckcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 3/31/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "chimeracheckcommand.h"

//**********************************************************************************************************************
vector<string> ChimeraCheckCommand::setParameters(){	
	try {
		CommandParameter ptemplate("reference", "InputTypes", "", "", "none", "none", "none","",false,true,true); parameters.push_back(ptemplate);
		CommandParameter pfasta("fasta", "InputTypes", "", "", "none", "none", "none","chimera",false,true,true); parameters.push_back(pfasta);
		CommandParameter pname("name", "InputTypes", "", "", "none", "none", "none","",false,false,true); parameters.push_back(pname);
		CommandParameter psvg("svg", "Boolean", "", "F", "", "", "","",false,false); parameters.push_back(psvg);
		CommandParameter pincrement("increment", "Number", "", "10", "", "", "","",false,false); parameters.push_back(pincrement);
		CommandParameter pksize("ksize", "Number", "", "7", "", "", "","",false,false); parameters.push_back(pksize);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);

		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "ChimeraCheckCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string ChimeraCheckCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The chimera.check command reads a fastafile and referencefile and outputs potentially chimeric sequences.\n";
		helpString += "This command was created using the algorithms described in CHIMERA_CHECK version 2.7 written by Niels Larsen. \n";
		helpString += "The chimera.check command parameters are fasta, reference, processors, ksize, increment, svg and name.\n";
		helpString += "The fasta parameter allows you to enter the fasta file containing your potentially chimeric sequences, and is required unless you have a valid current fasta file. \n";
		helpString += "You may enter multiple fasta files by separating their names with dashes. ie. fasta=abrecovery.fasta-amzon.fasta \n";
		helpString += "The reference parameter allows you to enter a reference file containing known non-chimeric sequences, and is required. \n";
		helpString += "The increment parameter allows you to specify how far you move each window while finding chimeric sequences, default is 10.\n";
		helpString += "The ksize parameter allows you to input kmersize, default is 7. \n";
		helpString += "The svg parameter allows you to specify whether or not you would like a svg file outputted for each query sequence, default is False.\n";
		helpString += "The name parameter allows you to enter a file containing names of sequences you would like .svg files for.\n";
		helpString += "You may enter multiple name files by separating their names with dashes. ie. fasta=abrecovery.svg.names-amzon.svg.names \n";
        helpString += "The chimera.check command should be in the following format: \n";
		helpString += "chimera.check(fasta=yourFastaFile, reference=yourTemplateFile, processors=yourProcessors, ksize=yourKmerSize) \n";
		helpString += "Example: chimera.check(fasta=AD.fasta, reference=core_set_aligned,imputed.fasta, processors=4, ksize=8) \n";
			
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "ChimeraCheckCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string ChimeraCheckCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "chimera") {  pattern = "[filename],chimeracheck.chimeras"; } 
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "ChimeraCheckCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
ChimeraCheckCommand::ChimeraCheckCommand(){	
	try {
		abort = true; calledHelp = true;
		setParameters();
		vector<string> tempOutNames;
		outputTypes["chimera"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "ChimeraCheckCommand", "ChimeraCheckCommand");
		exit(1);
	}
}
//***************************************************************************************************************
ChimeraCheckCommand::ChimeraCheckCommand(string option)  {
	try {
		abort = false; calledHelp = false;  
        
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter("chimera.check");
			map<string,string>::iterator it;
			
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (!validParameter.isValidParameter(it->first, myArray, it->second)) {  abort = true;  }
			}
			
			vector<string> tempOutNames;
			outputTypes["chimera"] = tempOutNames;
		
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.valid(parameters, "inputdir");		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				it = parameters.find("reference");
				//user has given a template file
				if(it != parameters.end()){ 
					string path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["reference"] = inputDir + it->second;		}
				}
			}
			
			//check for required parameters
			fastafile = validParameter.valid(parameters, "fasta");
			if (fastafile == "not found") { 				
				//if there is a current fasta file, use it
				string filename = current->getFastaFile(); 
				if (filename != "") { fastaFileNames.push_back(filename); m->mothurOut("Using " + filename + " as input file for the fasta parameter."); m->mothurOutEndLine(); }
				else { 	m->mothurOut("You have no current fastafile and the fasta parameter is required."); m->mothurOutEndLine(); abort = true; }
			}else { 
				util.splitAtDash(fastafile, fastaFileNames);
				
				//go through files and make sure they are good, if not, then disregard them
				for (int i = 0; i < fastaFileNames.size(); i++) {
					
					bool ignore = false;
					if (fastaFileNames[i] == "current") { 
						fastaFileNames[i] = current->getFastaFile(); 
						if (fastaFileNames[i] != "") {  m->mothurOut("Using " + fastaFileNames[i] + " as input file for the fasta parameter where you had given current."); m->mothurOutEndLine(); }
						else { 	
							m->mothurOut("You have no current fastafile, ignoring current."); m->mothurOutEndLine(); ignore=true; 
							//erase from file list
							fastaFileNames.erase(fastaFileNames.begin()+i);
							i--;
						}
					}
					
                    if (!ignore) {
                        if (util.checkLocations(fastaFileNames[i], current->getLocations())) { current->setFastaFile(fastaFileNames[i]); }
                        else { fastaFileNames.erase(fastaFileNames.begin()+i); i--; } //erase from file list
                    }
                }
				
				//make sure there is at least one valid file left
				if (fastaFileNames.size() == 0) { m->mothurOut("no valid files."); m->mothurOutEndLine(); abort = true; }
			}
			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.valid(parameters, "outputdir");		if (outputDir == "not found"){	outputDir = "";	}
			
			namefile = validParameter.valid(parameters, "name");
			if (namefile == "not found") { namefile = ""; }
			else { 
				util.splitAtDash(namefile, nameFileNames);
				
				//go through files and make sure they are good, if not, then disregard them
				for (int i = 0; i < nameFileNames.size(); i++) {
					
					bool ignore = false;
					if (nameFileNames[i] == "current") { 
						nameFileNames[i] = current->getNameFile();
						if (nameFileNames[i] != "") {  m->mothurOut("Using " + nameFileNames[i] + " as input file for the name parameter where you had given current."); m->mothurOutEndLine(); }
						else { 	
							m->mothurOut("You have no current namefile, ignoring current."); m->mothurOutEndLine(); ignore=true; 
							//erase from file list
							nameFileNames.erase(nameFileNames.begin()+i);
							i--;
						}
					}
					
                    if (!ignore) {
                        if (util.checkLocations(nameFileNames[i], current->getLocations())) { current->setNameFile(nameFileNames[i]); }
                        else { nameFileNames.erase(nameFileNames.begin()+i); i--; } //erase from file list
                    }
                }
				
				//make sure there is at least one valid file left
				if (nameFileNames.size() != 0) {
					if (nameFileNames.size() != fastaFileNames.size()) { 
						 m->mothurOut("Different number of valid name files and fasta files, aborting command."); m->mothurOutEndLine(); 
						 abort = true;
					}
				}
			}
			
			//this has to go after save so that if the user sets save=t and provides no reference we abort
			templatefile = validParameter.validFile(parameters, "reference");
			if (templatefile == "not found") {  m->mothurOut("[ERROR]: The reference parameter is a required, aborting.\n"); abort = true;
			}else if (templatefile == "not open") { abort = true; }	
			
			string temp = validParameter.valid(parameters, "ksize");			if (temp == "not found") { temp = "7"; }
			util.mothurConvert(temp, ksize);
			
			temp = validParameter.valid(parameters, "svg");				if (temp == "not found") { temp = "F"; }
			svg = util.isTrue(temp);
			if (nameFileNames.size() != 0) { svg = true; }
			
			temp = validParameter.valid(parameters, "increment");		if (temp == "not found") { temp = "10"; }
			util.mothurConvert(temp, increment);			
		}
	}
	catch(exception& e) {
		m->errorOut(e, "ChimeraCheckCommand", "ChimeraCheckCommand");
		exit(1);
	}
}
//***************************************************************************************************************

int ChimeraCheckCommand::execute(){
	try{
		
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
		for (int i = 0; i < fastaFileNames.size(); i++) {
				
			m->mothurOut("Checking sequences from " + fastaFileNames[i] + " ..." ); m->mothurOutEndLine();
			
			long start = time(NULL);	
			
			string thisNameFile = "";
			if (nameFileNames.size() != 0) { thisNameFile = nameFileNames[i]; }
			
			chimera = new ChimeraCheckRDP(fastaFileNames[i], templatefile, thisNameFile, svg, increment, ksize, outputDir);			

			if (m->getControl_pressed()) { delete chimera;	return 0;	}
			
			if (outputDir == "") { outputDir = util.hasPath(fastaFileNames[i]);  }//if user entered a file with a path then preserve it
            map<string, string> variables;
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(fastaFileNames[i]));
			string outputFileName = getOutputFileName("chimera", variables);
			outputNames.push_back(outputFileName); outputTypes["chimera"].push_back(outputFileName);
			
			numSeqs = driver(outputFileName, fastaFileNames[i]);
            
            if (m->getControl_pressed()) { for (int j = 0; j < outputNames.size(); j++) {	util.mothurRemove(outputNames[j]);	} outputTypes.clear();  delete chimera; return 0; }
			//break up file
				
			delete chimera;
			
			m->mothurOutEndLine(); m->mothurOut("This method does not determine if a sequence is chimeric, but allows you to make that determination based on the IS values."); m->mothurOutEndLine(); 
			m->mothurOutEndLine(); m->mothurOut("It took " + toString(time(NULL) - start) + " secs to check " + toString(numSeqs) + " sequences.");	m->mothurOutEndLine(); m->mothurOutEndLine();

		}
		
		m->mothurOut("\nOutput File Names: \n"); 
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}	
		m->mothurOutEndLine();
	
		return 0;
		
	}
	catch(exception& e) {
		m->errorOut(e, "ChimeraCheckCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************

int ChimeraCheckCommand::driver(string outputFName, string filename){
	try {
		ofstream out;
		util.openOutputFile(outputFName, out);
		
		ofstream out2;
		
		ifstream inFASTA;
		util.openInputFile(filename, inFASTA);

		int count = 0;
	
		while (!inFASTA.eof()) {

			if (m->getControl_pressed()) {	return 1;	}
		
			Sequence* candidateSeq = new Sequence(inFASTA);  util.gobble(inFASTA);
				
			if (candidateSeq->getName() != "") { //incase there is a commented sequence at the end of a file
				//find chimeras
				chimera->getChimeras(candidateSeq);
				
				if (m->getControl_pressed()) {	delete candidateSeq; return 1;	}
	
				//print results
				chimera->print(out, out2);
                count++;
			}
			delete candidateSeq;
			
			//report progress
			if((count) % 100 == 0){	m->mothurOutJustToScreen("Processing sequence: " + toString(count) + "\n");		}
		}
		//report progress
		if((count) % 100 != 0){	m->mothurOutJustToScreen("Processing sequence: " + toString(count) + "\n");	}
		
		out.close();
		inFASTA.close();
				
		return count;
	}
	catch(exception& e) {
		m->errorOut(e, "ChimeraCheckCommand", "driver");
		exit(1);
	}
}
/**************************************************************************************************/


