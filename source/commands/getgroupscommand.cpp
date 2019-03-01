/*
 *  getgroupscommand.cpp
 *  Mothur
 *
 *  Created by westcott on 11/10/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "getgroupscommand.h"
#include "sequence.hpp"
#include "listvector.hpp"

#include "inputdata.h"
#include "designmap.h"

//**********************************************************************************************************************
vector<string> GetGroupsCommand::setParameters(){	
	try {
		CommandParameter pfasta("fasta", "InputTypes", "", "", "none", "none", "FNGLT","fasta",false,false, true); parameters.push_back(pfasta);
		CommandParameter pshared("shared", "InputTypes", "", "", "none", "sharedGroup", "none","shared",false,false, true); parameters.push_back(pshared);
        CommandParameter pname("name", "InputTypes", "", "", "NameCount", "none", "none","name",false,false, true); parameters.push_back(pname);
        CommandParameter pcount("count", "InputTypes", "", "", "NameCount-CountGroup", "none", "none","count",false,false, true); parameters.push_back(pcount);
		CommandParameter pgroup("group", "InputTypes", "", "", "CountGroup", "sharedGroup", "FNGLT","group",false,false, true); parameters.push_back(pgroup);		
        CommandParameter pdesign("design", "InputTypes", "", "", "none", "sharedGroup", "FNGLT","design",false,false, true); parameters.push_back(pdesign);
		CommandParameter plist("list", "InputTypes", "", "", "none", "none", "FNGLT","list",false,false, true); parameters.push_back(plist);
		CommandParameter ptaxonomy("taxonomy", "InputTypes", "", "", "none", "none", "FNGLT","taxonomy",false,false, true); parameters.push_back(ptaxonomy);
        CommandParameter pphylip("phylip", "InputTypes", "", "", "none", "PhylipColumn", "none","phylip",false,false,true); parameters.push_back(pphylip);
        CommandParameter pcolumn("column", "InputTypes", "", "", "none", "PhylipColumn", "none","column",false,false,true); parameters.push_back(pcolumn);
		CommandParameter paccnos("accnos", "InputTypes", "", "", "none", "none", "none","",false,false); parameters.push_back(paccnos);
		CommandParameter pgroups("groups", "String", "", "", "", "", "","",false,false); parameters.push_back(pgroups);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string GetGroupsCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The get.groups command selects sequences from a specfic group or set of groups from the following file types: fasta, name, group, count, list, taxonomy, design, phylip, column or shared file.\n";
		helpString += "It outputs a file containing the sequences in the those specified groups, or a sharedfile containing only those groups.\n";
		helpString += "The get.groups command parameters are accnos, fasta, name, group, count, list, taxonomy, shared, design, phylip, column and groups. The group or count parameter is required, unless you have a current group or count file, or are using a shared file.\n";
		helpString += "You must also provide an accnos containing the list of groups to get or set the groups parameter to the groups you wish to select.\n";
		helpString += "The groups parameter allows you to specify which of the groups in your groupfile you would like.  You can separate group names with dashes.\n";
		helpString += "The get.groups command should be in the following format: get.groups(accnos=yourAccnos, fasta=yourFasta, group=yourGroupFile).\n";
		helpString += "Example get.groups(accnos=amazon.accnos, fasta=amazon.fasta, group=amazon.groups).\n";
		helpString += "or get.groups(groups=pasture, fasta=amazon.fasta, group=amazon.groups).\n";
		;
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string GetGroupsCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "fasta")            {   pattern = "[filename],pick,[extension]";    }
        else if (type == "taxonomy")    {   pattern = "[filename],pick,[extension]";    }
        else if (type == "name")        {   pattern = "[filename],pick,[extension]";    }
        else if (type == "group")       {   pattern = "[filename],pick,[extension]";    }
        else if (type == "count")       {   pattern = "[filename],pick,[extension]";    }
        else if (type == "phylip")       {   pattern = "[filename],pick,[extension]";    }
        else if (type == "column")       {   pattern = "[filename],pick,[extension]";    }
        else if (type == "list")        {   pattern = "[filename],[tag],pick,[extension]";    }
        else if (type == "shared")      {   pattern = "[filename],[tag],pick,[extension]";    }
        else if (type == "design")      {   pattern = "[filename],pick,[extension]";    }
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "GetGroupsCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
GetGroupsCommand::GetGroupsCommand(){	
	try {
		abort = true; calledHelp = true;
		setParameters();
		vector<string> tempOutNames;
		outputTypes["fasta"] = tempOutNames;
		outputTypes["taxonomy"] = tempOutNames;
		outputTypes["name"] = tempOutNames;
		outputTypes["group"] = tempOutNames;
		outputTypes["list"] = tempOutNames;
		outputTypes["shared"] = tempOutNames;
        outputTypes["design"] = tempOutNames;
        outputTypes["count"] = tempOutNames;
        outputTypes["phylip"] = tempOutNames;
        outputTypes["column"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "GetGroupsCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
GetGroupsCommand::GetGroupsCommand(string option)  {
	try {
		abort = false; calledHelp = false;   
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string,string>::iterator it;
			
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (!validParameter.isValidParameter(it->first, myArray, it->second)) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["fasta"] = tempOutNames;
			outputTypes["taxonomy"] = tempOutNames;
			outputTypes["name"] = tempOutNames;
			outputTypes["group"] = tempOutNames;
			outputTypes["list"] = tempOutNames;
			outputTypes["shared"] = tempOutNames;
            outputTypes["design"] = tempOutNames;
            outputTypes["count"] = tempOutNames;
            outputTypes["phylip"] = tempOutNames;
            outputTypes["column"] = tempOutNames;
			
			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.valid(parameters, "outputdir");		if (outputDir == "not found"){	outputDir = "";		}
			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.valid(parameters, "inputdir");		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("fasta");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["fasta"] = inputDir + it->second;		}
				}
				
				it = parameters.find("accnos");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["accnos"] = inputDir + it->second;		}
				}
				
				it = parameters.find("list");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["list"] = inputDir + it->second;		}
				}
				
				it = parameters.find("name");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["name"] = inputDir + it->second;		}
				}
				
				it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
				
				it = parameters.find("taxonomy");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["taxonomy"] = inputDir + it->second;		}
				}
				
				it = parameters.find("shared");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["shared"] = inputDir + it->second;		}
				}
                
                it = parameters.find("design");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["design"] = inputDir + it->second;		}
				}
                
                it = parameters.find("count");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["count"] = inputDir + it->second;		}
				}
                
                it = parameters.find("phylip");
                //user has given a template file
                if(it != parameters.end()){
                    path = util.hasPath(it->second);
                    //if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {	parameters["phylip"] = inputDir + it->second;		}
                }
                
                it = parameters.find("column");
                //user has given a template file
                if(it != parameters.end()){
                    path = util.hasPath(it->second);
                    //if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {	parameters["column"] = inputDir + it->second;		}
                }

			}
			
			
			//check for required parameters
			accnosfile = validParameter.validFile(parameters, "accnos");
			if (accnosfile == "not open") { abort = true; }
			else if (accnosfile == "not found") {  accnosfile = ""; }
			else { current->setAccnosFile(accnosfile); }
			
			fastafile = validParameter.validFile(parameters, "fasta");
			if (fastafile == "not open") { fastafile = ""; abort = true; }
			else if (fastafile == "not found") {  fastafile = "";  }
			else { current->setFastaFile(fastafile); }
			
            phylipfile = validParameter.validFile(parameters, "phylip");
            if (phylipfile == "not open") { phylipfile = ""; abort = true; }
            else if (phylipfile == "not found") { phylipfile = ""; }
            else { 	current->setPhylipFile(phylipfile); }
            
            columnfile = validParameter.validFile(parameters, "column");
            if (columnfile == "not open") { columnfile = ""; abort = true; }
            else if (columnfile == "not found") { columnfile = ""; }
            else {  current->setColumnFile(columnfile);	}

			namefile = validParameter.validFile(parameters, "name");
			if (namefile == "not open") { namefile = ""; abort = true; }
			else if (namefile == "not found") {  namefile = "";  }	
			else { current->setNameFile(namefile); }
			
			listfile = validParameter.validFile(parameters, "list");
			if (listfile == "not open") { abort = true; }
			else if (listfile == "not found") {  listfile = "";  }
			else { current->setListFile(listfile); }
			
			taxfile = validParameter.validFile(parameters, "taxonomy");
			if (taxfile == "not open") { taxfile = ""; abort = true; }
			else if (taxfile == "not found") {  taxfile = "";  }
			else { current->setTaxonomyFile(taxfile); }
			
			groups = validParameter.valid(parameters, "groups");			
			if (groups == "not found") { groups = ""; }
			else { util.splitAtDash(groups, Groups);
                    if (Groups.size() != 0) { if (Groups[0]== "all") { Groups.clear(); } } }
			
			sharedfile = validParameter.validFile(parameters, "shared");
			if (sharedfile == "not open") { sharedfile = ""; abort = true; }
			else if (sharedfile == "not found") {  sharedfile = "";  }
			else { current->setSharedFile(sharedfile); }
			
			groupfile = validParameter.validFile(parameters, "group");
			if (groupfile == "not open") { groupfile = ""; abort = true; }
			else if (groupfile == "not found") {  	groupfile = "";	}
			else { current->setGroupFile(groupfile); }
            
            designfile = validParameter.validFile(parameters, "design");
			if (designfile == "not open") { designfile = ""; abort = true; }
			else if (designfile == "not found") {  	designfile = "";	}
			else { current->setDesignFile(designfile); }
            
            countfile = validParameter.validFile(parameters, "count");
            if (countfile == "not open") { countfile = ""; abort = true; }
            else if (countfile == "not found") { countfile = "";  }	
            else { current->setCountFile(countfile); }
            
            if ((namefile != "") && (countfile != "")) {
                m->mothurOut("[ERROR]: you may only use one of the following: name or count."); m->mothurOutEndLine(); abort = true;
            }
            
            if ((groupfile != "") && (countfile != "")) {
                m->mothurOut("[ERROR]: you may only use one of the following: group or count."); m->mothurOutEndLine(); abort=true;
            }

			
			if ((sharedfile == "") && (groupfile == "") && (designfile == "") && (countfile == "")) { 
				//is there are current file available for any of these?
				if ((namefile != "") || (fastafile != "") || (listfile != "") || (taxfile != "")) {
					//give priority to group, then shared
					groupfile = current->getGroupFile(); 
					if (groupfile != "") {  m->mothurOut("Using " + groupfile + " as input file for the group parameter."); m->mothurOutEndLine(); }
					else { 
						sharedfile = current->getSharedFile(); 
						if (sharedfile != "") { m->mothurOut("Using " + sharedfile + " as input file for the shared parameter."); m->mothurOutEndLine(); }
						else { 
							countfile = current->getCountFile(); 
                            if (countfile != "") { m->mothurOut("Using " + countfile + " as input file for the count parameter."); m->mothurOutEndLine(); }
                            else { 
                                m->mothurOut("You have no current groupfile, countfile or sharedfile and one is required."); m->mothurOutEndLine(); abort = true;
                            }
						}
					}
				}else {
					//give priority to shared, then group
					sharedfile = current->getSharedFile(); 
					if (sharedfile != "") {  m->mothurOut("Using " + sharedfile + " as input file for the shared parameter."); m->mothurOutEndLine(); }
					else { 
						groupfile = current->getGroupFile(); 
						if (groupfile != "") { m->mothurOut("Using " + groupfile + " as input file for the group parameter."); m->mothurOutEndLine(); }
						else { 
							designfile = current->getDesignFile(); 
                            if (designfile != "") { m->mothurOut("Using " + designfile + " as input file for the design parameter."); m->mothurOutEndLine(); }
                            else { 
                                countfile = current->getCountFile(); 
                                if (countfile != "") { m->mothurOut("Using " + countfile + " as input file for the count parameter."); m->mothurOutEndLine(); }
                                else { 
                                    m->mothurOut("You have no current groupfile, designfile, countfile or sharedfile and one is required."); m->mothurOutEndLine(); abort = true;
                                }

                            }
						}
					}
				}
			}
			
			if ((accnosfile == "") && (Groups.size() == 0)) { m->mothurOut("You must provide an accnos file or specify groups using the groups parameter."); m->mothurOutEndLine(); abort = true; }
			
			if ((phylipfile == "") && (columnfile == "") && (fastafile == "") && (namefile == "") && (countfile == "") && (groupfile == "")  && (designfile == "") && (sharedfile == "") && (listfile == "") && (taxfile == ""))  { m->mothurOut("You must provide at least one of the following: fasta, name, taxonomy, group, shared, design, count, phylip, column or list."); m->mothurOutEndLine(); abort = true; }
			if (((groupfile == "") && (countfile == "")) && ((namefile != "") || (fastafile != "") || (listfile != "") || (taxfile != "")))  { m->mothurOut("If using a fasta, name, taxonomy, group or list, then you must provide a group or count file."); m->mothurOutEndLine(); abort = true; }
            
            if (countfile == "") {
                if ((namefile == "") && ((fastafile != "") || (taxfile != ""))){
                    vector<string> files; files.push_back(fastafile); files.push_back(taxfile);
                    if (!current->getMothurCalling())  {  parser.getNameFile(files);  }
                }
            }
		}
		
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "GetGroupsCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

int GetGroupsCommand::execute(){
	try {
		
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
		//get groups you want to remove
		if (accnosfile != "") { util.readAccnos(accnosfile, Groups);  }
		
		if (groupfile != "") {
			groupMap = new GroupMap(groupfile);
			groupMap->readMap();
			
			//fill names with names of sequences that are from the groups we want to remove 
			fillNames();
			
			delete groupMap;
		}else if (countfile != ""){
            if ((fastafile != "") || (listfile != "") || (taxfile != "")) { 
                m->mothurOut("\n[NOTE]: The count file should contain only unique names, so mothur assumes your fasta, list and taxonomy files also contain only uniques.\n\n");
            }
            CountTable ct;
            ct.readTable(countfile, true, false);
            if (!ct.hasGroupInfo()) { m->mothurOut("[ERROR]: your count file does not contain group info, aborting.\n"); return 0; }
                
            for (int i = 0; i < Groups.size(); i++) {
                vector<string> thisGroupsSeqs = ct.getNamesOfSeqs(Groups[i]);
                for (int j = 0; j < thisGroupsSeqs.size(); j++) { names.insert(thisGroupsSeqs[j]); }
            }
        }
		
		if (m->getControl_pressed()) { return 0; }
		
		//read through the correct file and output lines you want to keep
		if (namefile != "")			{		readName();		}
		if (fastafile != "")		{		readFasta();	}
		if (groupfile != "")		{		readGroup();	}
        if (countfile != "")		{		readCount();	}
		if (listfile != "")			{		readList();		}
		if (taxfile != "")			{		readTax();		}
		if (sharedfile != "")		{		readShared();	}
        if (designfile != "")		{		readDesign();	}
        if (phylipfile != "")		{		readPhylip();	}
        if (columnfile != "")		{		readColumn();	}
		
		if (m->getControl_pressed()) { for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } return 0; }
		
		
		if (outputNames.size() != 0) {
			m->mothurOut("\nOutput File names:\n");
			for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]+"\n"); 	}
			m->mothurOutEndLine();
			
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
			
			itTypes = outputTypes.find("taxonomy");
			if (itTypes != outputTypes.end()) {
				if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setTaxonomyFile(currentName); }
			}
			
			itTypes = outputTypes.find("shared");
			if (itTypes != outputTypes.end()) {
				if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setSharedFile(currentName); }
			}
            
            itTypes = outputTypes.find("design");
			if (itTypes != outputTypes.end()) {
				if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setDesignFile(currentName); }
			}
            
            itTypes = outputTypes.find("count");
			if (itTypes != outputTypes.end()) {
				if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setCountFile(currentName); }
			}
            
            itTypes = outputTypes.find("phylip");
            if (itTypes != outputTypes.end()) {
                if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setPhylipFile(currentName); }
            }
            
            itTypes = outputTypes.find("column");
            if (itTypes != outputTypes.end()) {
                if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setColumnFile(currentName); }
            }
		}
		
		return 0;		
	}
	
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "execute");
		exit(1);
	}
}

//**********************************************************************************************************************
int GetGroupsCommand::readFasta(){
	try {
		string thisOutputDir = outputDir;
		if (outputDir == "") {  thisOutputDir += util.hasPath(fastafile);  }
        map<string, string> variables; 
        variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(fastafile));
        variables["[extension]"] = util.getExtension(fastafile);
		string outputFileName = getOutputFileName("fasta", variables);
		
		ofstream out;
		util.openOutputFile(outputFileName, out);
		
		ifstream in;
		util.openInputFile(fastafile, in);
		string name;
		
		bool wroteSomething = false;
		int selectedCount = 0;
		
		while(!in.eof()){
			if (m->getControl_pressed()) { in.close();  out.close();  util.mothurRemove(outputFileName);  return 0; }
			
			Sequence currSeq(in);
			name = currSeq.getName();
			
			if (name != "") {
				//if this name is in the accnos file
				if (names.count(name) != 0) {
					wroteSomething = true;
					
					currSeq.printSequence(out);
					selectedCount++;
				}else{
					//if you are not in the accnos file check if you are a name that needs to be changed
					map<string, string>::iterator it = uniqueToRedundant.find(name);
					if (it != uniqueToRedundant.end()) {
						wroteSomething = true;
						currSeq.setName(it->second);
						currSeq.printSequence(out);
						selectedCount++;
					}
				}
			}
			util.gobble(in);
		}
		in.close();	
		out.close();
		
		if (wroteSomething == false) {  m->mothurOut("Your file does NOT contain sequences from the groups you wish to get."); m->mothurOutEndLine();  }
		outputTypes["fasta"].push_back(outputFileName);  outputNames.push_back(outputFileName);
		
		m->mothurOut("Selected " + toString(selectedCount) + " sequences from your fasta file."); m->mothurOutEndLine();

		
		return 0;
		
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "readFasta");
		exit(1);
	}
}
//**********************************************************************************************************************
int GetGroupsCommand::readShared(){
	try {
		string thisOutputDir = outputDir;
		if (outputDir == "") {  thisOutputDir += util.hasPath(sharedfile);  }
		
		InputData input(sharedfile, "sharedfile", Groups);
		SharedRAbundVectors* lookup = input.getSharedRAbundVectors();
        map<string, string> variables; 
        variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(sharedfile));
        variables["[extension]"] = util.getExtension(sharedfile);
		
		bool wroteSomething = false;
        bool printHeaders = true;
		while(lookup != NULL) {
			
            variables["[tag]"] = lookup->getLabel();
            string outputFileName = getOutputFileName("shared", variables);
			
			ofstream out;
			util.openOutputFile(outputFileName, out);
			outputTypes["shared"].push_back(outputFileName);  outputNames.push_back(outputFileName);
			
            if (m->getControl_pressed()) { out.close();  util.mothurRemove(outputFileName);  delete lookup; return 0; }
			
            lookup->print(out, printHeaders);
            wroteSomething = true;
			
			//get next line to process
			//prevent memory leak
			delete lookup;
			lookup = input.getSharedRAbundVectors();
			
			out.close();
		}
		
		if (wroteSomething == false) {  m->mothurOut("Your file contains only the groups you wish to remove."); m->mothurOutEndLine();  }
		
		string groupsString = "";
		for (int i = 0; i < Groups.size()-1; i++) {	groupsString += Groups[i] + ", "; }
		groupsString += Groups[Groups.size()-1];
		
		m->mothurOut("Selected groups: " + groupsString + " from your shared file."); m->mothurOutEndLine();
		
		return 0;
		
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "readShared");
		exit(1);
	}
}
//**********************************************************************************************************************
int GetGroupsCommand::readList(){
	try {
		string thisOutputDir = outputDir;
		if (outputDir == "") {  thisOutputDir += util.hasPath(listfile);  }
        map<string, string> variables; 
        variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(listfile));
        variables["[extension]"] = util.getExtension(listfile);
        InputData input(listfile, "list", nullVector);
        ListVector* list = input.getListVector();
        
		bool wroteSomething = false;
		int selectedCount = 0;
		
        while(list != NULL) {
			
            selectedCount = 0;
            
            variables["[tag]"] = list->getLabel();
            string outputFileName = getOutputFileName("list", variables);
			
			ofstream out;
			util.openOutputFile(outputFileName, out);
			outputTypes["list"].push_back(outputFileName);  outputNames.push_back(outputFileName);
            
            vector<string> binLabels = list->getLabels();
            vector<string> newBinLabels;
			
			//make a new list vector
			ListVector newList;
			newList.setLabel(list->getLabel());
			
			//for each bin
			for (int i = 0; i < list->getNumBins(); i++) {
				if (m->getControl_pressed()) {  out.close();  util.mothurRemove(outputFileName);  return 0; }
				
				//parse out names that are in accnos file
				string binnames = list->get(i);
                vector<string> thisBinNames;
                util.splitAtComma(binnames, thisBinNames);
				
				string newNames = "";
                for (int j = 0; j < thisBinNames.size(); j++) {
                    string name = thisBinNames[j];
                    
                    //if that name is in the .accnos file, add it
					if (names.count(name) != 0) {  newNames += name + ",";  selectedCount++;  }
					else{
						//if you are not in the accnos file check if you are a name that needs to be changed
						map<string, string>::iterator it = uniqueToRedundant.find(name);
						if (it != uniqueToRedundant.end()) {
							newNames += it->second + ",";
							selectedCount++;
						}
					}
                }
								
				//if there are names in this bin add to new list
				if (newNames != "") {  
					newNames = newNames.substr(0, newNames.length()-1); //rip off extra comma
					newList.push_back(newNames);
                    newBinLabels.push_back(binLabels[i]);
				}
			}
			
			//print new listvector
			if (newList.getNumBins() != 0) {
				wroteSomething = true;
                newList.setLabels(newBinLabels);
				newList.print(out, false);
			}
			
            out.close();
            
            delete list;
            list = input.getListVector();
		}
		
		
		if (wroteSomething == false) {  m->mothurOut("Your file does NOT contain sequences from the groups you wish to get."); m->mothurOutEndLine();  }
		
		m->mothurOut("Selected " + toString(selectedCount) + " sequences from your list file."); m->mothurOutEndLine();
		
		return 0;
		
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "readList");
		exit(1);
	}
}
//**********************************************************************************************************************
int GetGroupsCommand::readName(){
	try {
		string thisOutputDir = outputDir;
		if (outputDir == "") {  thisOutputDir += util.hasPath(namefile);  }
        map<string, string> variables; 
		variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(namefile));
        variables["[extension]"] = util.getExtension(namefile);
		string outputFileName = getOutputFileName("name", variables);
		
		ofstream out;
		util.openOutputFile(outputFileName, out);
		
		ifstream in;
		util.openInputFile(namefile, in);
		string name, firstCol, secondCol;
		
		bool wroteSomething = false;
		int selectedCount = 0;
		
		while(!in.eof()){
			if (m->getControl_pressed()) { in.close();  out.close();  util.mothurRemove(outputFileName);  return 0; }
			
			in >> firstCol;		util.gobble(in);		
			in >> secondCol;			
			
			vector<string> parsedNames;
			util.splitAtComma(secondCol, parsedNames);
			
			vector<string> validSecond;  validSecond.clear();
			for (int i = 0; i < parsedNames.size(); i++) {
				if (names.count(parsedNames[i]) != 0) {
					validSecond.push_back(parsedNames[i]);
				}
			}
			
			selectedCount += validSecond.size();
			
			//if the name in the first column is in the set then print it and any other names in second column also in set
			if (names.count(firstCol) != 0) {
				
				wroteSomething = true;
				
				out << firstCol << '\t';
				
				//you know you have at least one valid second since first column is valid
				for (int i = 0; i < validSecond.size()-1; i++) {  out << validSecond[i] << ',';  }
				out << validSecond[validSecond.size()-1] << endl;
				
				//make first name in set you come to first column and then add the remaining names to second column
			}else {
				
				//you want part of this row
				if (validSecond.size() != 0) {
					
					wroteSomething = true;
					
					out << validSecond[0] << '\t';
					
					//you know you have at least one valid second since first column is valid
					for (int i = 0; i < validSecond.size()-1; i++) {  out << validSecond[i] << ',';  }
					out << validSecond[validSecond.size()-1] << endl;
					uniqueToRedundant[firstCol] = validSecond[0];
				}
			}
			
			util.gobble(in);
		}
		in.close();
		out.close();
		
		if (wroteSomething == false) {  m->mothurOut("Your file does NOT contain sequences from the groups you wish to get."); m->mothurOutEndLine();  }
		outputTypes["name"].push_back(outputFileName); outputNames.push_back(outputFileName);
		
		m->mothurOut("Selected " + toString(selectedCount) + " sequences from your name file."); m->mothurOutEndLine();

		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "readName");
		exit(1);
	}
}

//**********************************************************************************************************************
int GetGroupsCommand::readGroup(){
	try {
		string thisOutputDir = outputDir;
		if (outputDir == "") {  thisOutputDir += util.hasPath(groupfile);  }
        map<string, string> variables; 
		variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(groupfile));
        variables["[extension]"] = util.getExtension(groupfile);
		string outputFileName = getOutputFileName("group", variables);
		
		ofstream out;
		util.openOutputFile(outputFileName, out);
		
		ifstream in;
		util.openInputFile(groupfile, in);
		string name, group;
		
		bool wroteSomething = false;
		int selectedCount = 0;
		
		while(!in.eof()){
			if (m->getControl_pressed()) { in.close();  out.close();  util.mothurRemove(outputFileName);  return 0; }
			
			in >> name;				//read from first column
			in >> group;			//read from second column
			
			//if this name is in the accnos file
			if (names.count(name) != 0) {
				wroteSomething = true;
				out << name << '\t' << group << endl;
				selectedCount++;
			}
			
			util.gobble(in);
		}
		in.close();
		out.close();
		
		if (wroteSomething == false) {  m->mothurOut("Your file does NOT contain sequences from the groups you wish to get."); m->mothurOutEndLine();  }
		outputTypes["group"].push_back(outputFileName); outputNames.push_back(outputFileName);
		
		m->mothurOut("Selected " + toString(selectedCount) + " sequences from your group file."); m->mothurOutEndLine();

		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "readGroup");
		exit(1);
	}
}
//**********************************************************************************************************************
int GetGroupsCommand::readCount(){
	try {
		string thisOutputDir = outputDir;
		if (outputDir == "") {  thisOutputDir += util.hasPath(countfile);  }
        map<string, string> variables; 
		variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(countfile));
        variables["[extension]"] = util.getExtension(countfile);
		string outputFileName = getOutputFileName("count", variables);

        CountTable ct; ct.readTable(countfile, true, false, Groups); //reads only groups found in Groups
				
        int selectedCount = ct.getNumUniqueSeqs();
        
		if (selectedCount == 0) {  m->mothurOut("Your file does NOT contain sequences from the groups you wish to get.\n");   }
        else {
            ct.printTable(outputFileName);
            outputTypes["count"].push_back(outputFileName); outputNames.push_back(outputFileName);
        }
		
		m->mothurOut("Selected " + toString(selectedCount) + " sequences from your count file.\n");
        
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "readCount");
		exit(1);
	}
}
//**********************************************************************************************************************
int GetGroupsCommand::readDesign(){
	try {
		string thisOutputDir = outputDir;
		if (outputDir == "") {  thisOutputDir += util.hasPath(designfile);  }
        map<string, string> variables; 
		variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(designfile));
        variables["[extension]"] = util.getExtension(designfile);
		string outputFileName = getOutputFileName("design", variables);
		
        DesignMap designMap(designfile);
        
        bool wroteSomething = false;
        
        ofstream out;
        util.openOutputFile(outputFileName, out);

        int numGroupsFound = designMap.printGroups(out, Groups);
        
        if (numGroupsFound > 0) { wroteSomething = true; }
				
        out.close();
		
		if (wroteSomething == false) {  m->mothurOut("Your file does NOT contain groups from the groups you wish to get."); m->mothurOutEndLine();  }
		outputTypes["design"].push_back(outputFileName); outputNames.push_back(outputFileName);
		
		m->mothurOut("Selected " + toString(numGroupsFound) + " groups from your design file."); m->mothurOutEndLine();
        
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "readDesign");
		exit(1);
	}
}


//**********************************************************************************************************************
int GetGroupsCommand::readTax(){
	try {
		string thisOutputDir = outputDir;
		if (outputDir == "") {  thisOutputDir += util.hasPath(taxfile);  }
        map<string, string> variables; 
		variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(taxfile));
        variables["[extension]"] = util.getExtension(taxfile);
		string outputFileName = getOutputFileName("taxonomy", variables);
		
		ofstream out;
		util.openOutputFile(outputFileName, out);
		
		ifstream in;
		util.openInputFile(taxfile, in);
		string name, tax;
		
		bool wroteSomething = false;
        int selectedCount = 0;
		
		while(!in.eof()){
			if (m->getControl_pressed()) { in.close();  out.close();  util.mothurRemove(outputFileName);  return 0; }
			
            in >> name; util.gobble(in);
            tax = util.getline(in); util.gobble(in);
			
			//if this name is in the accnos file
			if (names.count(name) != 0) {
				wroteSomething = true;
                out << name << '\t' << tax << endl; selectedCount++;
			}else{
				//if you are not in the accnos file check if you are a name that needs to be changed
				map<string, string>::iterator it = uniqueToRedundant.find(name);
				if (it != uniqueToRedundant.end()) {
					wroteSomething = true;
					out << it->second << '\t' << tax << endl; selectedCount++;
				}
			}
		}
		in.close();
		out.close();
		
		if (wroteSomething == false) {  m->mothurOut("Your file does NOT contain sequences from the groups you wish to get.\n");  }
		outputTypes["taxonomy"].push_back(outputFileName); outputNames.push_back(outputFileName);
        
        m->mothurOut("Selected " + toString(selectedCount) + " sequences from your taxonomy file.\n");
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "readTax");
		exit(1);
	}
}
//**********************************************************************************************************************
int GetGroupsCommand::readPhylip(){
    try {
        string thisOutputDir = outputDir;
        if (outputDir == "") {  thisOutputDir += util.hasPath(phylipfile);  }
        map<string, string> variables;
        variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(phylipfile));
        variables["[extension]"] = util.getExtension(phylipfile);
        string outputFileName = getOutputFileName("phylip", variables);
        
        ifstream in;
        util.openInputFile(phylipfile, in);
        
        float distance;
        int square, nseqs;
        string name;
        unsigned int row;
        set<unsigned int> rows; //converts names in names to a index
        row = 0;
        
        string numTest;
        in >> numTest >> name;
        
        if (!util.isContainingOnlyDigits(numTest)) { m->mothurOut("[ERROR]: expected a number and got " + numTest + ", quitting."); m->mothurOutEndLine(); exit(1); }
        else { convert(numTest, nseqs); }
        
        if (names.count(name) != 0) { rows.insert(row); }
        row++;
        
        //is the matrix square?
        char d;
        while((d=in.get()) != EOF){
            
            if(isalnum(d)){
                square = 1;
                in.putback(d);
                for(int i=0;i<nseqs;i++){
                    in >> distance;
                }
                break;
            }
            if(d == '\n'){
                square = 0;
                break;
            }
        }
        
        //map name to row/column
        if(square == 0){
            for(int i=1;i<nseqs;i++){
                in >> name;
                if (names.count(name) != 0) { rows.insert(row); }
                row++;
                
                for(int j=0;j<i;j++){
                    if (m->getControl_pressed()) {  in.close(); return 0;  }
                    in >> distance;
                }
            }
        }
        else{
            for(int i=1;i<nseqs;i++){
                in >> name;
                if (names.count(name) != 0) { rows.insert(row); }
                row++;
                for(int j=0;j<nseqs;j++){
                    if (m->getControl_pressed()) {  in.close(); return 0;  }
                    in >> distance;
                }
            }
        }
        in.close();
        
        if (m->getControl_pressed()) {  return 0; }
        
        //read through file only printing rows and columns of seqs in names
        ifstream inPhylip;
        util.openInputFile(phylipfile, inPhylip);
        
        inPhylip >> numTest;
        
        ofstream out;
        util.openOutputFile(outputFileName, out);
        outputTypes["phylip"].push_back(outputFileName);  outputNames.push_back(outputFileName);
        out << names.size() << endl;
        
        unsigned int count = 0;
        if(square == 0){
            for(int i=0;i<nseqs;i++){
                inPhylip >> name;
                bool ignoreRow = false;
                
                if (names.count(name) == 0) { ignoreRow = true; }
                else{ out << name << '\t'; count++; }
                
                for(int j=0;j<i;j++){
                    if (m->getControl_pressed()) {  inPhylip.close(); out.close();  return 0;  }
                    inPhylip >> distance;
                    if (!ignoreRow) {
                        //is this a column we want
                        if(rows.count(j) != 0) {  out << distance << '\t';  }
                    }
                }
                if (!ignoreRow) { out << endl; }
            }
        }
        else{
            for(int i=0;i<nseqs;i++){
                inPhylip >> name;
                
                bool ignoreRow = false;
                
                if (names.count(name) == 0) { ignoreRow = true; }
                else{ out << name << '\t'; count++; }
                
                for(int j=0;j<nseqs;j++){
                    if (m->getControl_pressed()) {  inPhylip.close(); out.close(); return 0;  }
                    inPhylip >> distance;
                    if (!ignoreRow) {
                        //is this a column we want
                        if(rows.count(j) != 0) {  out << distance << '\t';  }
                    }
                }
                if (!ignoreRow) { out << endl; }
            }
        }
        inPhylip.close();
        out.close();
        
        if (count == 0) {  m->mothurOut("Your file does NOT contain distances related to groups or sequences listed in the accnos file."); m->mothurOutEndLine();  }
        else if (count != names.size()) {
            m->mothurOut("[WARNING]: Your accnos file contains " + toString(names.size()) + " groups or sequences, but I only found " + toString(count) + " of them in the phylip file."); m->mothurOutEndLine();
            //rewrite with new number
            util.renameFile(outputFileName, outputFileName+".temp");
            ofstream out2;
            util.openOutputFile(outputFileName, out2);
            out2 << count << endl;
            
            ifstream in3;
            util.openInputFile(outputFileName+".temp", in3);
            in3 >> nseqs; util.gobble(in3);
            char buffer[4096];
            while (!in3.eof()) {
                in3.read(buffer, 4096);
                out2.write(buffer, in3.gcount());
            }
            in3.close();
            out2.close();
            util.mothurRemove(outputFileName+".temp");
        }
        
        m->mothurOut("Selected " + toString(count) + " groups or sequences from your phylip file."); m->mothurOutEndLine();
        
        return 0;
        
    }
    catch(exception& e) {
        m->errorOut(e, "GetGroupsCommand", "readPhylip");
        exit(1);
    }
}
//**********************************************************************************************************************
int GetGroupsCommand::readColumn(){
    try {
        string thisOutputDir = outputDir;
        if (outputDir == "") {  thisOutputDir += util.hasPath(columnfile);  }
        map<string, string> variables;
        variables["[filename]"] = thisOutputDir + util.getRootName(util.getSimpleName(columnfile));
        variables["[extension]"] = util.getExtension(columnfile);
        string outputFileName = getOutputFileName("column", variables);
        outputTypes["column"].push_back(outputFileName);  outputNames.push_back(outputFileName);
        
        ofstream out;
        util.openOutputFile(outputFileName, out);
        
        ifstream in;
        util.openInputFile(columnfile, in);
        
        set<string> foundNames;
        string firstName, secondName;
        float distance;
        while (!in.eof()) {
            
            if (m->getControl_pressed()) { out.close(); in.close(); return 0; }
            
            in >> firstName >> secondName >> distance; util.gobble(in);
            
            //are both names in the accnos file
            if ((names.count(firstName) != 0) && (names.count(secondName) != 0)) {
                out << firstName << '\t' << secondName << '\t' << distance << endl;
                foundNames.insert(firstName);
                foundNames.insert(secondName);
            }
        }
        in.close();
        out.close();
        
        if (foundNames.size() == 0) {  m->mothurOut("Your file does NOT contain distances related to groups or sequences listed in the accnos file."); m->mothurOutEndLine();  }
        else if (foundNames.size() != names.size()) {
            m->mothurOut("[WARNING]: Your accnos file contains " + toString(names.size()) + " groups or sequences, but I only found " + toString(foundNames.size()) + " of them in the column file."); m->mothurOutEndLine();
        }
        
        m->mothurOut("Selected " + toString(foundNames.size()) + " groups or sequences from your column file."); m->mothurOutEndLine();
        
        return 0;
        
    }
    catch(exception& e) {
        m->errorOut(e, "GetGroupsCommand", "readColumn");
        exit(1);
    }
}
//**********************************************************************************************************************
int GetGroupsCommand::fillNames(){
	try {
		vector<string> seqs = groupMap->getNamesSeqs();
		
		for (int i = 0; i < seqs.size(); i++) {
			
			if (m->getControl_pressed()) { return 0; }
			
			string group = groupMap->getGroup(seqs[i]);
			
			if (util.inUsersGroups(group, Groups)) { names.insert(seqs[i]); }
		}
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetGroupsCommand", "fillNames");
		exit(1);
	}
}

//**********************************************************************************************************************


