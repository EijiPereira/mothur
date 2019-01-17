/*
 *  splitabundcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 5/17/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "splitabundcommand.h"


//**********************************************************************************************************************
vector<string> SplitAbundCommand::setParameters(){	
	try {		
		CommandParameter pfasta("fasta", "InputTypes", "", "", "none", "none", "none","fasta",false,true,true); parameters.push_back(pfasta);
        CommandParameter pname("name", "InputTypes", "", "", "NameCount", "FNGLT", "none","name",false,false,true); parameters.push_back(pname);
        CommandParameter pcount("count", "InputTypes", "", "", "NameCount-CountGroup", "none", "none","count",false,false); parameters.push_back(pcount);
		CommandParameter pgroup("group", "InputTypes", "", "", "CountGroup", "none", "none","group",false,false); parameters.push_back(pgroup);
		CommandParameter plist("list", "InputTypes", "", "", "none", "FNGLT", "none","list",false,false,true); parameters.push_back(plist);
		CommandParameter plabel("label", "String", "", "", "", "", "","",false,false); parameters.push_back(plabel);
		CommandParameter pcutoff("cutoff", "Number", "", "0", "", "", "","",false,true); parameters.push_back(pcutoff);
		CommandParameter pgroups("groups", "String", "", "", "", "", "","",false,false); parameters.push_back(pgroups);
		CommandParameter paccnos("accnos", "Boolean", "", "F", "", "", "","",false,false); parameters.push_back(paccnos);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string SplitAbundCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The split.abund command reads a fasta file and a list or a names file splits the sequences into rare and abundant groups. \n";
		helpString += "The split.abund command parameters are fasta, list, name, count, cutoff, group, label, groups, cutoff and accnos.\n";
		helpString += "The fasta and a list or name or count parameter are required, and you must provide a cutoff value.\n";
		helpString += "The cutoff parameter is used to qualify what is abundant and rare.\n";
		helpString += "The group parameter allows you to parse a group file into rare and abundant groups.\n";
		helpString += "The label parameter is used to read specific labels in your listfile you want to use.\n";
		helpString += "The accnos parameter allows you to output a .rare.accnos and .abund.accnos files to use with the get.seqs and remove.seqs commands.\n";
		helpString += "The groups parameter allows you to parse the files into rare and abundant files by group.  \n";
		helpString += "For example if you set groups=A-B-C, you will get a .A.abund, .A.rare, .B.abund, .B.rare, .C.abund, .C.rare files.  \n";
		helpString += "If you want .abund and .rare files for all groups, set groups=all.  \n";
		helpString += "The split.abund command should be used in the following format: split.abund(fasta=yourFasta, list=yourListFile, group=yourGroupFile, label=yourLabels, cutoff=yourCutoff).\n";
		helpString += "Example: split.abund(fasta=abrecovery.fasta, list=abrecovery.fn.list, group=abrecovery.groups, label=0.03, cutoff=2).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "getHelpString");
		exit(1);
	}
}

//**********************************************************************************************************************
string SplitAbundCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "fasta")        {  pattern = "[filename],[tag],[tag2],fasta-[filename],[tag],[group],[tag2],fasta";            } 
        else if (type == "list")    {   pattern = "[filename],[tag],[tag2],list-[filename],[group],[tag],[tag2],list";            }
        else if (type == "name")    {   pattern = "[filename],[tag],names-[filename],[group],[tag],names";           }
        else if (type == "count")   {   pattern = "[filename],[tag],[tag2],count_table-[filename],[tag],count_table";     }
        else if (type == "group")   {   pattern = "[filename],[tag],[tag2],groups-[filename],[tag],[group],[tag2],groups";          }
        else if (type == "accnos")  {   pattern = "[filename],[tag],[tag2],accnos-[filename],[tag],[group],[tag2],accnos";          }
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "SplitAbundCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
SplitAbundCommand::SplitAbundCommand(){	
	try {
		abort = true; calledHelp = true; 
		setParameters();
		vector<string> tempOutNames;
		outputTypes["list"] = tempOutNames;
		outputTypes["name"] = tempOutNames;
        outputTypes["count"] = tempOutNames;
		outputTypes["accnos"] = tempOutNames;
		outputTypes["group"] = tempOutNames;
		outputTypes["fasta"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "SplitAbundCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
SplitAbundCommand::SplitAbundCommand(string option)  {
	try {
		abort = false; calledHelp = false;   
		allLines = 1;
			
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
		
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (!validParameter.isValidParameter(it->first, myArray, it->second)) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["list"] = tempOutNames;
			outputTypes["name"] = tempOutNames;
			outputTypes["accnos"] = tempOutNames;
			outputTypes["group"] = tempOutNames;
			outputTypes["fasta"] = tempOutNames;	
            outputTypes["count"] = tempOutNames;
												
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.valid(parameters, "inputdir");		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("list");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["list"] = inputDir + it->second;		}
				}
				
				it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
				
				it = parameters.find("fasta");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["fasta"] = inputDir + it->second;		}
				}
				
				it = parameters.find("name");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["name"] = inputDir + it->second;		}
				}

                it = parameters.find("count");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["count"] = inputDir + it->second;		}
				}
			}

			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.valid(parameters, "outputdir");		if (outputDir == "not found"){	outputDir = "";	}

			//check for required parameters
			listfile = validParameter.validFile(parameters, "list");
			if (listfile == "not open") { abort = true; }
			else if (listfile == "not found") { listfile = ""; }
			else{ inputFile = listfile; current->setListFile(listfile); }	
			
			namefile = validParameter.validFile(parameters, "name");
			if (namefile == "not open") { abort = true; }
			else if (namefile == "not found") { namefile = ""; }	
			else{ inputFile = namefile; current->setNameFile(namefile); }	
		
			fastafile = validParameter.validFile(parameters, "fasta");
			if (fastafile == "not open") { abort = true; }
			else if (fastafile == "not found") { 				
				fastafile = current->getFastaFile(); 
				if (fastafile != "") { m->mothurOut("Using " + fastafile + " as input file for the fasta parameter."); m->mothurOutEndLine(); }
				else { 	m->mothurOut("You have no current fastafile and the fasta parameter is required."); m->mothurOutEndLine(); abort = true; }
			}else { current->setFastaFile(fastafile); }	
			
			groupfile = validParameter.validFile(parameters, "group");
			if (groupfile == "not open") {  groupfile = ""; abort = true; }	
			else if (groupfile == "not found") { groupfile = ""; }
			else {  
				int error = groupMap.readMap(groupfile);
				if (error == 1) { abort = true; }
				current->setGroupFile(groupfile);
			}
			
            countfile = validParameter.validFile(parameters, "count");
			if (countfile == "not open") { countfile = ""; abort = true; }
			else if (countfile == "not found") { countfile = "";  }	
			else {
                current->setCountFile(countfile); 
                ct.readTable(countfile, true, false);
            }
            
            if ((namefile != "") && (countfile != "")) {
                m->mothurOut("[ERROR]: you may only use one of the following: name or count."); m->mothurOutEndLine(); abort = true;
            }
			
            if ((groupfile != "") && (countfile != "")) {
                m->mothurOut("[ERROR]: you may only use one of the following: group or count."); m->mothurOutEndLine(); abort=true;
            }
            
			groups = validParameter.valid(parameters, "groups");		
			if (groups == "not found") { groups = ""; }
			else { util.splitAtDash(groups, Groups); if (Groups.size() != 0) { if (Groups[0]== "all") { Groups.clear(); } } }
			
			if (((groupfile == "") && (countfile == ""))&& (groups != "")) {  m->mothurOut("You cannot select groups without a valid group or count file, I will disregard your groups selection. "); m->mothurOutEndLine(); groups = "";  Groups.clear(); }
			
            if (countfile != "") {
                if (!ct.hasGroupInfo()) { m->mothurOut("You cannot pick groups without group info in your count file; I will disregard your groups selection."); m->mothurOutEndLine(); groups = "";  Groups.clear(); }
            }
            
			//do you have all files needed
			if ((listfile == "") && (namefile == "") && (countfile == "")) { 
				namefile = current->getNameFile(); 
				if (namefile != "") { m->mothurOut("Using " + namefile + " as input file for the name parameter."); m->mothurOutEndLine(); }
				else { 				
					listfile = current->getListFile(); 
					if (listfile != "") { m->mothurOut("Using " + listfile + " as input file for the list parameter."); m->mothurOutEndLine(); }
					else { 	
                        countfile  = current->getCountFile(); 
                        if (countfile != "") { m->mothurOut("Using " + countfile + " as input file for the count parameter."); m->mothurOutEndLine(); }
                        else { 	m->mothurOut("You have no current list, count or namefile and one is required."); m->mothurOutEndLine(); abort = true; }
                    }
				}
			}
            
			//check for optional parameter and set defaults
			// ...at some point should added some additional type checking...
			label = validParameter.valid(parameters, "label");			
			if (label == "not found") { label = "";  allLines = 1; }
			else { 
				if(label != "all") {  util.splitAtDash(label, labels);  allLines = 0;  }
				else { allLines = 1;  }
			}
			
			string temp = validParameter.valid(parameters, "accnos");		if (temp == "not found") { temp = "F"; }
			accnos = util.isTrue(temp); 
			
			temp = validParameter.valid(parameters, "cutoff");				if (temp == "not found") { temp = "0"; }
			util.mothurConvert(temp, cutoff); 

			if (cutoff == 0) {  m->mothurOut("[ERROR]: You must provide a cutoff to qualify what is abundant for the split.abund command. \n");  abort = true;  }
		}

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "SplitAbundCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
SplitAbundCommand::~SplitAbundCommand(){}
//**********************************************************************************************************************
int SplitAbundCommand::execute(){
	try {
	
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
		if (listfile != "") { //you are using a listfile to determine abundance
			if (outputDir == "") { outputDir = util.hasPath(listfile); }
			
			//if the users enters label "0.06" and there is no "0.06" in their file use the next lowest label.
			set<string> processedLabels;
			set<string> userLabels = labels;	
			
			InputData input(listfile, "list", Groups);
			ListVector* list = input.getListVector();
			string lastLabel = list->getLabel();
			
			//do you have a namefile or do we need to similate one?
			if (namefile != "") {  readNamesFile();		}
			else				{ createNameMap(list);	}
			
			if (m->getControl_pressed()) { delete list; for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } return 0; }
			
			while((list != NULL) && ((allLines == 1) || (userLabels.size() != 0))) {
			
				if (m->getControl_pressed()) {  delete list; for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } return 0; }
				
				if(allLines == 1 || labels.count(list->getLabel()) == 1){
						
						m->mothurOut(list->getLabel()); m->mothurOutEndLine();
						splitList(list);
											
						processedLabels.insert(list->getLabel());
						userLabels.erase(list->getLabel());
				}
				
				if ((util.anyLabelsToProcess(list->getLabel(), userLabels, "") ) && (processedLabels.count(lastLabel) != 1)) {
						string saveLabel = list->getLabel();
						
						delete list;
						list = input.getListVector(lastLabel); //get new list vector to process
						
						m->mothurOut(list->getLabel()); m->mothurOutEndLine();
						splitList(list);
						
						processedLabels.insert(list->getLabel());
						userLabels.erase(list->getLabel());
						
						//restore real lastlabel to save below
						list->setLabel(saveLabel);
				}
				
			
				lastLabel = list->getLabel();
					
				delete list;
				list = input.getListVector(); //get new list vector to process
			}
			
			if (m->getControl_pressed()) {  for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } return 0; }
			
			//output error messages about any remaining user labels
			set<string>::iterator it;
			bool needToRun = false;
			for (it = userLabels.begin(); it != userLabels.end(); it++) {  
				m->mothurOut("Your file does not include the label " + *it); 
				if (processedLabels.count(lastLabel) != 1) {
					m->mothurOut(". I will use " + lastLabel + "."); m->mothurOutEndLine();
					needToRun = true;
				}else {
					m->mothurOut(". Please refer to " + lastLabel + "."); m->mothurOutEndLine();
				}

			}
			
			if (m->getControl_pressed()) {  for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } return 0; }
			
			//run last label if you need to
			if (needToRun )  {
				if (list != NULL) {	delete list;	}
				list = input.getListVector(lastLabel); //get new list vector to process
				
				m->mothurOut(list->getLabel()); m->mothurOutEndLine();
				splitList(list);		
				
				delete list;
			}
			
			if (m->getControl_pressed()) { for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); }	return 0;	}
									
		}else if (namefile != "") { //you are using the namefile to determine abundance
			if (outputDir == "") { outputDir = util.hasPath(namefile); }
			
			splitNames(); 
			writeNames();
			
			string tag = "";
			if (groupfile != "")				{  parseGroup(tag);		}
			if (accnos)							{  writeAccnos(tag);	}
			if (fastafile != "")				{  parseFasta(tag);		}
		}else {
            //split by countfile
            string tag = "";
            splitCount();
            
			if (accnos)							{  writeAccnos(tag);	}
			if (fastafile != "")				{  parseFasta(tag);		}
        }
		
		//set fasta file as new current fastafile
		string currentName = "";
		itTypes = outputTypes.find("fasta");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setFastaFile(currentName); }
		}
		
		itTypes = outputTypes.find("name");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setNameFile(currentName); }
		}
		
		itTypes = outputTypes.find("group");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setGroupFile(currentName); }
		}
		
		itTypes = outputTypes.find("list");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setListFile(currentName); }
		}
		
		itTypes = outputTypes.find("accnos");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setAccnosFile(currentName); }
		}
        
        itTypes = outputTypes.find("count");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setCountFile(currentName); }
		}
		
		m->mothurOut("\nOutput File Names: \n"); 
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i] +"\n"); 	} m->mothurOutEndLine();
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "execute");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::splitList(ListVector* thisList) {
	try {
		rareNames.clear();
		abundNames.clear();
		
		//get rareNames and abundNames
        int numRareBins = 0;
		for (int i = 0; i < thisList->getNumBins(); i++) {
			if (m->getControl_pressed()) { return 0; }
			
			string bin = thisList->get(i);
						
			vector<string> names;
			util.splitAtComma(bin, names);  //parses bin into individual sequence names
			int size = names.size();
            
            //if countfile is not blank we assume the list file is unique, otherwise we assume it includes all seqs
            if (countfile != "") {
                size = 0;
                for (int j = 0; j < names.size(); j++) {  size += ct.getNumSeqs(names[j]); }
            }
            
			if (size <= cutoff) {
                numRareBins++;
				for (int j = 0; j < names.size(); j++) {  rareNames.insert(names[j]);  }
			}else{
				for (int j = 0; j < names.size(); j++) {  abundNames.insert(names[j]);  }
			}
		}//end for

		
		string tag = thisList->getLabel();
       
		writeList(thisList, tag, numRareBins);
    
		if (groupfile != "")				{  parseGroup(tag);		}
		if (accnos)							{  writeAccnos(tag);	}
		if (fastafile != "")				{  parseFasta(tag);		}
        if (countfile != "")				{  parseCount(tag);		}
        
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "splitList");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::writeList(ListVector* thisList, string tag, int numRareBins) { 
	try {
		
		if (Groups.size() == 0) {
			int numAbundBins = thisList->getNumBins() - numRareBins;

			ofstream aout;
			ofstream rout;
			
            map<string, string> variables; 
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(listfile));
            variables["[tag]"] = tag;
            variables["[tag2]"] = "rare";
			string rare = getOutputFileName("list",variables);
			util.openOutputFile(rare+".temp", rout);
			outputNames.push_back(rare); outputTypes["list"].push_back(rare);
			
            variables["[tag2]"] = "abund";
			string abund = getOutputFileName("list",variables);
			util.openOutputFile(abund+".temp", aout);
			outputNames.push_back(abund); outputTypes["list"].push_back(abund);

			if (rareNames.size() != 0)	{  rout << thisList->getLabel() << '\t' << numRareBins;		}
			if (abundNames.size() != 0) {	aout << thisList->getLabel() << '\t' << numAbundBins;	}
            
            vector<string> binLabels = thisList->getLabels();
            string rareHeader = "label\tnumOtus"; string abundHeader = "label\tnumOtus";
			for (int i = 0; i < thisList->getNumBins(); i++) {
				if (m->getControl_pressed()) { break; }
			
				string bin = thisList->get(i); 
                vector<string> names;
                util.splitAtComma(bin, names);
                
				int size = names.size();
                if (countfile != "") {
                    size = 0;
                    for (int j = 0; j < names.size(); j++) {  size += ct.getNumSeqs(names[j]); }
                }
			
				if (size <= cutoff) {  rout  << '\t' << bin;  rareHeader += '\t' + binLabels[i]; }
				else				{  aout  << '\t' << bin;  abundHeader += '\t' + binLabels[i]; }
			}
			
			if (rareNames.size() != 0)	{ rout << endl; }
			if (abundNames.size() != 0) { aout << endl; }
			
			rout.close();
			aout.close();
            
            //add headers
            ofstream r;
            util.openOutputFile(rare, r);
            r << rareHeader << endl;
            r.close();
            util.appendFiles(rare+".temp", rare);
            util.mothurRemove(rare+".temp");
            
            ofstream a;
            util.openOutputFile(abund, a);
            a << abundHeader << endl;
            a.close();
            util.appendFiles(abund+".temp", abund);
            util.mothurRemove(abund+".temp");
			
		}else{ //parse names by abundance and group
			string fileroot =  outputDir + util.getRootName(util.getSimpleName(listfile));

			//map<string, bool> wroteFile;
			map<string, string> files; //group -> filename
			map<string, string>::iterator it3;

			for (int i=0; i<Groups.size(); i++) {
                map<string, string> variables; 
                variables["[filename]"] = fileroot;
                variables["[tag]"] = tag;
                variables["[tag2]"] = "rare";
                variables["[group]"] = Groups[i];
                string rareGroupFileName = getOutputFileName("list",variables);
                variables["[tag2]"] = "abund";
                string abundGroupFileName = getOutputFileName("list",variables);
                files[Groups[i]+".rare"] = rareGroupFileName;
                files[Groups[i]+".abund"] = abundGroupFileName;
                ofstream temp1, temp2;
                util.openOutputFile(rareGroupFileName, temp1); temp1.close();
				util.openOutputFile(abundGroupFileName, temp2); temp2.close();
				outputNames.push_back(rareGroupFileName); outputTypes["list"].push_back(rareGroupFileName);
				outputNames.push_back(abundGroupFileName); outputTypes["list"].push_back(abundGroupFileName);
			}
			
			map<string, string> groupVector;
            map<string, string> groupLabels;
			map<string, string>::iterator itGroup;
			map<string, int> groupNumBins;
		
			for (it3 = files.begin(); it3 != files.end(); it3++) {
				groupNumBins[it3->first] = 0;
				groupVector[it3->first] = "";
                groupLabels[it3->first] = "label\tnumOtus";
			}
            vector<string> binLabels = thisList->getLabels();
			for (int i = 0; i < thisList->getNumBins(); i++) {
				if (m->getControl_pressed()) { break; }
			
				map<string, string> groupBins;
				string bin = thisList->get(i); 
			
				vector<string> names;
				util.splitAtComma(bin, names);  //parses bin into individual sequence names
			
				//parse bin into list of sequences in each group
				for (int j = 0; j < names.size(); j++) {
					string rareAbund;
					if (rareNames.count(names[j]) != 0) { //you are a rare name
						rareAbund = ".rare";
					}else{ //you are a abund name
						rareAbund = ".abund";
					}
					
                    if (countfile == "") {
                        string group = groupMap.getGroup(names[j]);
                        
                        if (util.inUsersGroups(group, Groups)) { //only add if this is in a group we want
                            itGroup = groupBins.find(group+rareAbund);
                            if(itGroup == groupBins.end()) {
                                groupBins[group+rareAbund] = names[j];  //add first name
                                groupNumBins[group+rareAbund]++;
                            }else{ //add another name
                                groupBins[group+rareAbund] +=  "," + names[j];
                            }
                        }else if(group == "not found") {
                            m->mothurOut(names[j] + " is not in your groupfile. Ignoring."); m->mothurOutEndLine();
                        }
                    }else {
                        vector<string> thisSeqsGroups = ct.getGroups(names[j]);
                        for (int k = 0; k < thisSeqsGroups.size(); k++) {
                            if (util.inUsersGroups(thisSeqsGroups[k], Groups)) { //only add if this is in a group we want
                                itGroup = groupBins.find(thisSeqsGroups[k]+rareAbund);
                                if(itGroup == groupBins.end()) {
                                    groupBins[thisSeqsGroups[k]+rareAbund] = names[j];  //add first name
                                    groupNumBins[thisSeqsGroups[k]+rareAbund]++;
                                }else{ //add another name
                                    groupBins[thisSeqsGroups[k]+rareAbund] +=  "," + names[j];
                                }
                            }
                        }
                    }
				}
			
			
				for (itGroup = groupBins.begin(); itGroup != groupBins.end(); itGroup++) {
					groupVector[itGroup->first] +=  '\t' + itGroup->second;
                    groupLabels[itGroup->first] += '\t' + binLabels[i];
				}
			}
			
			//end list vector
			for (it3 = files.begin(); it3 != files.end(); it3++) {
                ofstream out;
                util.openOutputFileAppend(it3->second, out);
                out << groupLabels[it3->first] << endl;
				out << thisList->getLabel() << '\t' << groupNumBins[it3->first] << groupVector[it3->first] << endl;  // label numBins  listvector for that group
				out.close();
			}
		}
		
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "writeList");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::splitCount() { //countfile
	try {
		rareNames.clear();
		abundNames.clear();	
        
		vector<string> allNames = ct.getNamesOfSeqs();
        for (int i = 0; i < allNames.size(); i++) {
            
            if (m->getControl_pressed()) { return 0; }
            
            int size = ct.getNumSeqs(allNames[i]);
            nameMap[allNames[i]] = allNames[i];
            
			if (size <= cutoff) {
				rareNames.insert(allNames[i]); 
			}else{
				abundNames.insert(allNames[i]); 
			}
		}
        
        //write out split count files
        parseCount("");
		
		return 0;  
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "splitCount");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::splitNames() { //namefile
	try {
		
		rareNames.clear();
		abundNames.clear();	
			
		//open input file
		ifstream in;
		util.openInputFile(namefile, in);
		
		while (!in.eof()) {
			if (m->getControl_pressed()) { break; }
			
			string firstCol, secondCol;
			in >> firstCol >> secondCol; util.gobble(in);
			
			nameMap[firstCol] = secondCol;
			
			int size = util.getNumNames(secondCol);
				
			if (size <= cutoff) {
				rareNames.insert(firstCol); 
			}else{
				abundNames.insert(firstCol); 
			}
		}
		in.close();
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "splitNames");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::readNamesFile() { 
	try {
		//open input file
		ifstream in;
		util.openInputFile(namefile, in);
		
		while (!in.eof()) {
			if (m->getControl_pressed()) { break; }
			
			string firstCol, secondCol;
			in >> firstCol >> secondCol; util.gobble(in);
			
			nameMap[firstCol] = secondCol;
		}
		in.close();
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "readNamesFile");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::createNameMap(ListVector* thisList) {
	try {
		
		if (thisList != NULL) {
			for (int i = 0; i < thisList->getNumBins(); i++) {
				if (m->getControl_pressed()) { return 0; }
				
				string bin = thisList->get(i);
							
				vector<string> names;
				util.splitAtComma(bin, names);  //parses bin into individual sequence names
				
				for (int j = 0; j < names.size(); j++) {  nameMap[names[j]] = names[j];  }
			}//end for
		}
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "createNameMap");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::parseCount(string tag) { //namefile
	try {
        
		if (Groups.size() == 0) {
            map<string, string> variables; 
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(countfile));
            variables["[tag]"] = tag;
            variables["[tag2]"] = "rare";
			string rare = getOutputFileName("count",variables);
            outputNames.push_back(rare); outputTypes["count"].push_back(rare);
			variables["[tag2]"] = "abund";
			string abund = getOutputFileName("count",variables);
			outputNames.push_back(abund); outputTypes["count"].push_back(abund);
			
            CountTable rareTable;
            CountTable abundTable;
            if (ct.hasGroupInfo()) {  
                vector<string> ctGroups = ct.getNamesOfGroups();
                for (int i = 0; i < ctGroups.size(); i++) {  rareTable.addGroup(ctGroups[i]);  abundTable.addGroup(ctGroups[i]); }
            }
            
			if (rareNames.size() != 0) {
				for (set<string>::iterator itRare = rareNames.begin(); itRare != rareNames.end(); itRare++) {
                    if (ct.hasGroupInfo()) {
                        vector<int> groupCounts = ct.getGroupCounts(*itRare);
                        rareTable.push_back(*itRare, groupCounts);
                    }else {
                        int groupCounts = ct.getNumSeqs(*itRare);
                        rareTable.push_back(*itRare, groupCounts);
                    }
				}
                if (rareTable.hasGroupInfo()) {
                    vector<string> ctGroups = rareTable.getNamesOfGroups();
                    for (int i = 0; i < ctGroups.size(); i++) { 
                        if (rareTable.getGroupCount(ctGroups[i]) == 0) { rareTable.removeGroup(ctGroups[i]); }
                    }
                }
                rareTable.printTable(rare);
			}
			
            
			if (abundNames.size() != 0) {
				for (set<string>::iterator itAbund = abundNames.begin(); itAbund != abundNames.end(); itAbund++) {
					if (ct.hasGroupInfo()) {
                        vector<int> groupCounts = ct.getGroupCounts(*itAbund);
                        abundTable.push_back(*itAbund, groupCounts);
                    }else {
                        int groupCounts = ct.getNumSeqs(*itAbund);
                        abundTable.push_back(*itAbund, groupCounts);
                    }
				}
                if (abundTable.hasGroupInfo()) {
                    vector<string> ctGroups = abundTable.getNamesOfGroups();
                    for (int i = 0; i < ctGroups.size(); i++) { 
                        if (abundTable.getGroupCount(ctGroups[i]) == 0) { abundTable.removeGroup(ctGroups[i]); }
                    }
                }
                abundTable.printTable(abund);
			}
			
		}else{ //parse names by abundance and group
			map<string, CountTable*> countTableMap;
			map<string, CountTable*>::iterator it3;
            
			for (int i=0; i<Groups.size(); i++) {
				CountTable* rareCt = new CountTable();
                rareCt->addGroup(Groups[i]);
				countTableMap[Groups[i]+".rare"] = rareCt;
				CountTable* abundCt = new CountTable();
                abundCt->addGroup(Groups[i]);
				countTableMap[Groups[i]+".abund"] = abundCt;
			}
			
            vector<string> allNames = ct.getNamesOfSeqs();
			for (int i = 0; i < allNames.size(); i++) {				
				string rareAbund;
				if (rareNames.count(allNames[i]) != 0) { //you are a rare name
                    rareAbund = ".rare";
				}else{ //you are a abund name
                    rareAbund = ".abund";
				}
				
                vector<string> thisSeqsGroups = ct.getGroups(allNames[i]);
                for (int j = 0; j < thisSeqsGroups.size(); j++) {
                    if (util.inUsersGroups(thisSeqsGroups[j], Groups)) { //only add if this is in a group we want
                        int num = ct.getGroupCount(allNames[i], thisSeqsGroups[j]);
                        vector<int> nums; nums.push_back(num);
                        countTableMap[thisSeqsGroups[j]+rareAbund]->push_back(allNames[i], nums); 
                    }
                }
			}
			
			
			for (it3 = countTableMap.begin(); it3 != countTableMap.end(); it3++) { 
                string fileroot =  outputDir + util.getRootName(util.getSimpleName(countfile));
                map<string, string> variables; 
                variables["[filename]"] = fileroot;
                variables["[tag]"] = it3->first;
                string filename = getOutputFileName("count",variables);
                outputNames.push_back(filename);  outputTypes["count"].push_back(filename);
                (it3->second)->printTable(filename);
				delete it3->second;
			}
		}
        
		return 0;
        
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "parseCount");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::writeNames() { //namefile
	try {
		
		if (Groups.size() == 0) {
			ofstream aout;
			ofstream rout;
			
            map<string, string> variables;
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(namefile));
            variables["[tag]"] = "rare";
			string rare = getOutputFileName("name", variables);
			util.openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["name"].push_back(rare);
			
            variables["[tag]"] = "abund";
			string abund = getOutputFileName("name", variables);
			util.openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["name"].push_back(abund);
			
			if (rareNames.size() != 0) {
				for (set<string>::iterator itRare = rareNames.begin(); itRare != rareNames.end(); itRare++) {
					rout << (*itRare) << '\t' << nameMap[(*itRare)] << endl;
				}
			}
			rout.close();
			
			if (abundNames.size() != 0) {
				for (set<string>::iterator itAbund = abundNames.begin(); itAbund != abundNames.end(); itAbund++) {
					aout << (*itAbund) << '\t' << nameMap[(*itAbund)] << endl;
				}
			}
			aout.close();
			
		}else{ //parse names by abundance and group
			string fileroot =  outputDir + util.getRootName(util.getSimpleName(namefile));
			map<string, string> files; //group -> filename
			map<string, string>::iterator it3;

			for (int i=0; i<Groups.size(); i++) {
                map<string, string> variables;
                variables["[filename]"] = fileroot;
                variables["[tag]"] = "rare";
                variables["[group]"] = Groups[i];
                string rareGroupFileName = getOutputFileName("name",variables);
                variables["[tag]"] = "abund";
                string abundGroupFileName = getOutputFileName("name",variables);
                
                ofstream temp1, temp2;
                files[Groups[i]+".rare"] = rareGroupFileName;
                files[Groups[i]+".abund"] = abundGroupFileName;
                util.openOutputFile(rareGroupFileName, temp1); temp1.close();
				util.openOutputFile(abundGroupFileName, temp2); temp2.close();
			}
			
			for (map<string, string>::iterator itName = nameMap.begin(); itName != nameMap.end(); itName++) {				
				vector<string> names;
				util.splitAtComma(itName->second, names);  //parses bin into individual sequence names
				
				string rareAbund;
				if (rareNames.count(itName->first) != 0) { //you are a rare name
						rareAbund = ".rare";
				}else{ //you are a abund name
						rareAbund = ".abund";
				}
				
				map<string, string> outputStrings;
				map<string, string>::iterator itout;
				for (int i = 0; i < names.size(); i++) {
					
					string group = groupMap.getGroup(names[i]);
					
					if (util.inUsersGroups(group, Groups)) { //only add if this is in a group we want
						itout = outputStrings.find(group+rareAbund);
						if (itout == outputStrings.end()) {  
							outputStrings[group+rareAbund] = names[i] + '\t' + names[i];
						}else {   outputStrings[group+rareAbund] += "," + names[i]; }
					}else if(group == "not found") {
						m->mothurOut(names[i] + " is not in your groupfile. Ignoring."); m->mothurOutEndLine();
					}
				}
				
				for (itout = outputStrings.begin(); itout != outputStrings.end(); itout++) {
                    ofstream out;
                    util.openOutputFileAppend(files[itout->first], out);
                    out << itout->second << endl;
                    out.close();
                }
			}
			
			
			for (it3 = files.begin(); it3 != files.end(); it3++) {
                map<string, string> variables;
                variables["[filename]"] = fileroot;
                variables["[tag]"] = it3->first;
				outputNames.push_back(getOutputFileName("name",variables));  outputTypes["name"].push_back(getOutputFileName("name",variables));
			}
		}
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "writeNames");
		exit(1);
	}
}
/**********************************************************************************************************************/
//just write the unique names - if a namesfile is given
int SplitAbundCommand::writeAccnos(string tag) { 
	try {
		
		if (Groups.size() == 0) {
			ofstream aout;
			ofstream rout;
			
            map<string, string> variables;
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputFile));
            variables["[tag]"] = tag;
            variables["[tag2]"] = "rare";
			string rare = getOutputFileName("accnos",variables);
			util.openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["accnos"].push_back(rare); 
			
			for (set<string>::iterator itRare = rareNames.begin(); itRare != rareNames.end(); itRare++) {
				rout << (*itRare) << endl;
			}
			rout.close();
		
            variables["[tag2]"] = "abund";
			string abund = getOutputFileName("accnos",variables);
			util.openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["accnos"].push_back(abund);
			
			for (set<string>::iterator itAbund = abundNames.begin(); itAbund != abundNames.end(); itAbund++) {
				aout << (*itAbund) << endl;
			}
			aout.close();
			
		}else{ //parse names by abundance and group
			string fileroot =  outputDir + util.getRootName(util.getSimpleName(inputFile));
			
			map<string, string> files;
			map<string, string>::iterator it3;
			
			for (int i=0; i<Groups.size(); i++) {
                map<string, string> variables;
                variables["[filename]"] = fileroot;
                variables["[tag]"] = tag;
                variables["[tag2]"] = "rare";
                variables["[group]"] = Groups[i];
                ofstream temp1, temp2;
                string rareAccnosFileName = getOutputFileName("accnos",variables);
                util.openOutputFile(rareAccnosFileName, temp1); temp1.close();
                variables["[tag2]"] = "abund";
                string abundAccnosFileName = getOutputFileName("accnos",variables);
				util.openOutputFile(abundAccnosFileName, temp2); temp2.close();
                files[Groups[i]+".rare"] = rareAccnosFileName;
                files[Groups[i]+".abund"] = abundAccnosFileName;
			}
			
			//write rare
			for (set<string>::iterator itRare = rareNames.begin(); itRare != rareNames.end(); itRare++) {
					string group = groupMap.getGroup(*itRare);
					
					if (util.inUsersGroups(group, Groups)) { //only add if this is in a group we want
                        ofstream out;
                        util.openOutputFileAppend(files[group+".rare"], out);
                        out << *itRare << endl; out.close();
					}
			}
				
			//write abund	
			for (set<string>::iterator itAbund = abundNames.begin(); itAbund != abundNames.end(); itAbund++) {
					string group = groupMap.getGroup(*itAbund);
					
					if (util.inUsersGroups(group, Groups)) { //only add if this is in a group we want
                        ofstream out;
                        util.openOutputFileAppend(files[group+".abund"], out);
						out << *itAbund << endl; out.close();
					}
			}
			
			//close files
			for (it3 = files.begin(); it3 != files.end(); it3++) {
                map<string, string> variables;
                variables["[filename]"] = fileroot;
                variables["[tag]"] = tag;
                variables["[tag2]"] = it3->first;
				outputNames.push_back(getOutputFileName("accnos",variables));  outputTypes["accnos"].push_back(getOutputFileName("accnos",variables));
			}
		}
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "writeAccnos");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::parseGroup(string tag) { //namefile
	try {
		
		if (Groups.size() == 0) {
			ofstream aout;
			ofstream rout;
			
            map<string, string> variables;
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(groupfile));
            variables["[tag]"] = tag;
            variables["[tag2]"] = "rare";
			string rare = getOutputFileName("group",variables);
			util.openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["group"].push_back(rare);
		
            variables["[tag2]"] = "abund";
			string abund = getOutputFileName("group",variables);
;
			util.openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["group"].push_back(abund);
			
			for (map<string, string>::iterator itName = nameMap.begin(); itName != nameMap.end(); itName++) {				
				vector<string> names;
				util.splitAtComma(itName->second, names);  //parses bin into individual sequence names
				
				for (int i = 0; i < names.size(); i++) {
				
					string group = groupMap.getGroup(names[i]);
				
					if (group == "not found") { 
						m->mothurOut(names[i] + " is not in your groupfile, ignoring, please correct."); m->mothurOutEndLine();
					}else {
						if (rareNames.count(itName->first) != 0) { //you are a rare name
							rout << names[i] << '\t' << group << endl;
						}else{ //you are a abund name
							aout << names[i] << '\t' << group << endl;
						}
					}
				}
			}
			
			rout.close(); 
			aout.close(); 

		}else{ //parse names by abundance and group
			string fileroot =  outputDir + util.getRootName(util.getSimpleName(groupfile));

            map<string, string> files;
			map<string, string>::iterator it3;

			for (int i=0; i<Groups.size(); i++) {
                map<string, string> variables;
                variables["[filename]"] = fileroot;
                variables["[tag]"] = tag;
                variables["[tag2]"] = "rare";
                variables["[group]"] = Groups[i];
                ofstream temp1, temp2;
                string rareGroupFile = getOutputFileName("group",variables);
                util.openOutputFile(rareGroupFile, temp1); temp1.close();
                variables["[tag2]"] = "abund";
                string abundGroupFile = getOutputFileName("group",variables);
                util.openOutputFile(abundGroupFile, temp2); temp2.close();
                files[Groups[i]+".rare"] = rareGroupFile;
                files[Groups[i]+".abund"] = abundGroupFile;
                
			}
			
			for (map<string, string>::iterator itName = nameMap.begin(); itName != nameMap.end(); itName++) {				
				vector<string> names;
				util.splitAtComma(itName->second, names);  //parses bin into individual sequence names
				
				string rareAbund;
				if (rareNames.count(itName->first) != 0) { //you are a rare name
					rareAbund = ".rare";
				}else{ //you are a abund name
					rareAbund = ".abund";
				}
				
				for (int i = 0; i < names.size(); i++) {
				
					string group = groupMap.getGroup(names[i]);
									
					if (util.inUsersGroups(group, Groups)) { //only add if this is in a group we want
                        ofstream out;
                        util.openOutputFileAppend(files[group+rareAbund], out);
                        out << names[i] << '\t' << group << endl; out.close();
					}
				}
			}
			
			for (it3 = files.begin(); it3 != files.end(); it3++) {
                map<string, string> variables;
                variables["[filename]"] = fileroot;
                variables["[tag]"] = tag;
                variables["[tag2]"] = it3->first;
				outputNames.push_back(getOutputFileName("group",variables));  outputTypes["group"].push_back(getOutputFileName("group",variables));
			}
		}
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "parseGroups");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::parseFasta(string tag) { //namefile
	try {
		if (Groups.size() == 0) {
			ofstream aout;
			ofstream rout;
			
            map<string, string> variables;
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(fastafile));
            variables["[tag]"] = tag;
            variables["[tag2]"] = "rare";
			string rare = getOutputFileName("fasta",variables);
			util.openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["fasta"].push_back(rare);
		
            variables["[tag2]"] = "abund";
			string abund = getOutputFileName("fasta",variables);
			util.openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["fasta"].push_back(abund);
		
			//open input file
			ifstream in;
			util.openInputFile(fastafile, in);
	
			while (!in.eof()) {
				if (m->getControl_pressed()) { break; }
		
				Sequence seq(in); util.gobble(in);
				
				if (seq.getName() != "") { 
					
					map<string, string>::iterator itNames;
					
					itNames = nameMap.find(seq.getName());
					
					if (itNames == nameMap.end()) {
						m->mothurOut(seq.getName() + " is not in your names or list file, ignoring."); m->mothurOutEndLine();
					}else{
						if (rareNames.count(seq.getName()) != 0) { //you are a rare name
							seq.printSequence(rout);
						}else{ //you are a abund name
							seq.printSequence(aout);
						}
					}
				}
			}
			in.close();
			rout.close(); 
			aout.close(); 

		}else{ //parse names by abundance and group
			string fileroot =  outputDir + util.getRootName(util.getSimpleName(fastafile));
            
            map<string, string> files;
			map<string, string>::iterator it3;

			for (int i=0; i<Groups.size(); i++) {
                map<string, string> variables;
                variables["[filename]"] = fileroot;
                variables["[tag]"] = tag;
                variables["[tag2]"] = "rare";
                variables["[group]"] = Groups[i];
                ofstream temp1, temp2;
                string rareFastaFile = getOutputFileName("fasta",variables);
                util.openOutputFile(rareFastaFile, temp1); temp1.close();
                variables["[tag2]"] = "abund";
                string abundFastaFile = getOutputFileName("fasta",variables);
                util.openOutputFile(abundFastaFile, temp2); temp2.close();
                files[Groups[i]+".rare"] = rareFastaFile;
                files[Groups[i]+".abund"] = abundFastaFile;
			}
			
			//open input file
			ifstream in;
			util.openInputFile(fastafile, in);
	
			while (!in.eof()) {
				if (m->getControl_pressed()) { break; }
		
				Sequence seq(in); util.gobble(in);
				
				if (seq.getName() != "") { 
					map<string, string>::iterator itNames = nameMap.find(seq.getName());
					
					if (itNames == nameMap.end()) {
						m->mothurOut(seq.getName() + " is not in your names or list file, ignoring."); m->mothurOutEndLine();
					}else{
						vector<string> names;
						util.splitAtComma(itNames->second, names);  //parses bin into individual sequence names
				
						string rareAbund;
						if (rareNames.count(itNames->first) != 0) { //you are a rare name
							rareAbund = ".rare";
						}else{ //you are a abund name
							rareAbund = ".abund";
						}
                        
                        if (countfile == "") {
                            for (int i = 0; i < names.size(); i++) {
                                string group = groupMap.getGroup(seq.getName());
                                
                                if (util.inUsersGroups(group, Groups)) { //only add if this is in a group we want
                                    ofstream out;
                                    util.openOutputFileAppend(files[group+rareAbund], out);
                                    seq.printSequence(out); out.close();
                                }else if(group == "not found") {
                                    m->mothurOut(seq.getName() + " is not in your groupfile. Ignoring."); m->mothurOutEndLine();
                                }
                            }
                        }else {
                            vector<string> thisSeqsGroups = ct.getGroups(names[0]); //we only need names[0], because there is no namefile
                            for (int i = 0; i < thisSeqsGroups.size(); i++) {
                                if (util.inUsersGroups(thisSeqsGroups[i], Groups)) { //only add if this is in a group we want
                                    ofstream out;
                                    util.openOutputFileAppend(files[thisSeqsGroups[i]+rareAbund], out);
                                    seq.printSequence(out); out.close();
                                }
                            }
                        }
					}
				}
			}
			in.close();
			
			for (it3 = files.begin(); it3 != files.end(); it3++) {
                map<string, string> variables;
                variables["[filename]"] = fileroot;
                variables["[tag]"] = tag;
                variables["[tag2]"] = it3->first;
				outputNames.push_back(getOutputFileName("fasta",variables));  outputTypes["fasta"].push_back(getOutputFileName("fasta",variables));
			}
		}
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "parseFasta");
		exit(1);
	}
}
/**********************************************************************************************************************/

