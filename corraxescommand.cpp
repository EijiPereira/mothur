/*
 *  corraxescommand.cpp
 *  Mothur
 *
 *  Created by westcott on 12/22/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "corraxescommand.h"

//********************************************************************************************************************
//sorts highes to lowest
inline bool compareSpearman(spearmanRank left, spearmanRank right){
	return (left.score > right.score);	
} 
//**********************************************************************************************************************
vector<string> CorrAxesCommand::getValidParameters(){	
	try {
		string Array[] =  {"axes","shared","relabund","numaxes","label","groups","method","metadata","outputdir","inputdir"};
		vector<string> myArray (Array, Array+(sizeof(Array)/sizeof(string)));
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "getValidParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
vector<string> CorrAxesCommand::getRequiredParameters(){	
	try {
		string Array[] =  {"axes"};
		vector<string> myArray (Array, Array+(sizeof(Array)/sizeof(string)));
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "getRequiredParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
CorrAxesCommand::CorrAxesCommand(){	
	try {
		abort = true;
		//initialize outputTypes
		vector<string> tempOutNames;
		outputTypes["corr.axes"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "CorrAxesCommand");
		exit(1);
	}
}

//**********************************************************************************************************************
vector<string> CorrAxesCommand::getRequiredFiles(){	
	try {
		vector<string> myArray;
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "getRequiredFiles");
		exit(1);
	}
}
//**********************************************************************************************************************
CorrAxesCommand::CorrAxesCommand(string option)  {
	try {
		abort = false;
		globaldata = GlobalData::getInstance();
		
		//allow user to run help
		if(option == "help") { help(); abort = true; }
		
		else {
			//valid paramters for this command
			string Array[] =  {"axes","shared","relabund","numaxes","label","groups","method","metadata","outputdir","inputdir"};
			vector<string> myArray (Array, Array+(sizeof(Array)/sizeof(string)));
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
			
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			vector<string> tempOutNames;
			outputTypes["corr.axes"] = tempOutNames;
			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.validFile(parameters, "inputdir", false);		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("axes");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["axes"] = inputDir + it->second;		}
				}
				
				it = parameters.find("shared");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["shared"] = inputDir + it->second;		}
				}
				
				it = parameters.find("relabund");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["relabund"] = inputDir + it->second;		}
				}
				
				it = parameters.find("metadata");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["metadata"] = inputDir + it->second;		}
				}
			}
			
			
			//check for required parameters
			axesfile = validParameter.validFile(parameters, "axes", true);
			if (axesfile == "not open") { abort = true; }
			else if (axesfile == "not found") { axesfile = ""; m->mothurOut("axes is a required parameter for the corr.axes command."); m->mothurOutEndLine(); abort = true;  }	
			
			sharedfile = validParameter.validFile(parameters, "shared", true);
			if (sharedfile == "not open") { abort = true; }
			else if (sharedfile == "not found") { sharedfile = ""; }
			else { inputFileName = sharedfile; }
			
			relabundfile = validParameter.validFile(parameters, "relabund", true);
			if (relabundfile == "not open") { abort = true; }
			else if (relabundfile == "not found") { relabundfile = ""; }
			else { inputFileName = relabundfile; }
			
			metadatafile = validParameter.validFile(parameters, "metadata", true);
			if (metadatafile == "not open") { abort = true; }
			else if (metadatafile == "not found") { metadatafile = ""; }
			
			groups = validParameter.validFile(parameters, "groups", false);			
			if (groups == "not found") { groups = "";  pickedGroups = false;  }
			else { 
				pickedGroups = true;
				m->splitAtDash(groups, Groups);	
			}			
			globaldata->Groups = Groups;
			
			outputDir = validParameter.validFile(parameters, "outputdir", false);		if (outputDir == "not found"){	outputDir = m->hasPath(inputFileName);	}
			
			label = validParameter.validFile(parameters, "label", false);			
			if (label == "not found") { label = ""; m->mothurOut("You did not provide a label, I will use the first label in your inputfile."); m->mothurOutEndLine(); label=""; }	
			
			if ((relabundfile == "") && (sharedfile == "")) { m->mothurOut("You must provide either a shared or relabund file."); m->mothurOutEndLine(); abort = true;  }
			
			if ((relabundfile != "") && (sharedfile != "")) { m->mothurOut("You may not use both a shared and relabund file."); m->mothurOutEndLine(); abort = true;  }
			
			string temp;
			temp = validParameter.validFile(parameters, "numaxes", false);		if (temp == "not found"){	temp = "3";				}
			convert(temp, numaxes); 
			
			method = validParameter.validFile(parameters, "method", false);		if (method == "not found"){	method = "pearson";		}
			
			if ((method != "pearson") && (method != "spearman") && (method != "kendall")) { m->mothurOut(method + " is not a valid method. Valid methods are pearson, spearman, and kendall."); m->mothurOutEndLine(); abort = true; }
			
		}
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "CorrAxesCommand");		
		exit(1);
	}
}
//**********************************************************************************************************************

void CorrAxesCommand::help(){
	try {

	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "help");	
		exit(1);
	}
}

//**********************************************************************************************************************

CorrAxesCommand::~CorrAxesCommand(){}

//**********************************************************************************************************************

int CorrAxesCommand::execute(){
	try {
		
		if (abort == true) { return 0; }
		
		/*************************************************************************************/
		// use smart distancing to get right sharedRabund and convert to relabund if needed  //
		/************************************************************************************/
		if (sharedfile != "") {  
			getShared(); 
			if (m->control_pressed) {  for (int i = 0; i < lookup.size(); i++) {  delete lookup[i];  } return 0; }
			if (lookup[0] == NULL) { m->mothurOut("[ERROR] reading shared file."); m->mothurOutEndLine(); return 0; }
			
			//fills lookupFloat with relative abundance values from lookup
			convertToRelabund();
			
			for (int i = 0; i < lookup.size(); i++) {  delete lookup[i];  } 
		}else { 
			getSharedFloat(); 
			if (m->control_pressed) {  for (int i = 0; i < lookupFloat.size(); i++) {  delete lookupFloat[i];  } return 0; }
			if (lookupFloat[0] == NULL) { m->mothurOut("[ERROR] reading relabund file."); m->mothurOutEndLine(); return 0; }
			
			if (pickedGroups) { eliminateZeroOTUS(lookupFloat); }
		}
		
		if (m->control_pressed) {  for (int i = 0; i < lookupFloat.size(); i++) {  delete lookupFloat[i];  } return 0; }
		
		//this is for a sanity check to make sure the axes file and shared file match
		for (int i = 0; i < lookupFloat.size(); i++) { names.insert(lookupFloat[i]->getGroup()); }
		
		/*************************************************************************************/
		// read axes file and check for file mismatches with shared or relabund file         //
		/************************************************************************************/
		
		//read axes file
		map<string, vector<float> > axes = readAxes();
		
		if (m->control_pressed) {  for (int i = 0; i < lookupFloat.size(); i++) {  delete lookupFloat[i];  } return 0; }
		
		//sanity check, the read only adds groups that are in the shared or relabund file, but we want to make sure the axes file isn't missing anyone
		if (axes.size() != lookupFloat.size()) { 
			map<string, vector<float> >::iterator it;
			for (int i = 0; i < lookupFloat.size(); i++) {
				it = axes.find(lookupFloat[i]->getGroup());
				if (it == axes.end()) { m->mothurOut(lookupFloat[i]->getGroup() + " is in your shared of relabund file but not in your axes file, please correct."); m->mothurOutEndLine(); }
			}
			m->control_pressed = true;
		}
		
		if (m->control_pressed) {  for (int i = 0; i < lookupFloat.size(); i++) {  delete lookupFloat[i];  } return 0; }
		
		/*************************************************************************************/
		// calc the r values																//
		/************************************************************************************/
		
		string outputFileName = outputDir + m->getRootName(m->getSimpleName(inputFileName)) + "corr.axes";
		outputNames.push_back(outputFileName); outputTypes["corr.axes"].push_back(outputFileName);	
		ofstream out;
		m->openOutputFile(outputFileName, out);
		out.setf(ios::fixed, ios::floatfield); out.setf(ios::showpoint);
		
		//output headings
		out << "OTU\t";
		for (int i = 0; i < numaxes; i++) { out << "axis" << (i+1) << '\t'; }
		out << endl;
		
		if (method == "pearson")		{  calcPearson(axes, out);	}
		else if (method == "spearman")	{  calcSpearman(axes, out); }
		else if (method == "kendall")	{  calcKendall(axes, out);	}
		else { m->mothurOut("[ERROR]: Invalid method."); m->mothurOutEndLine(); }
		
		out.close();
		for (int i = 0; i < lookupFloat.size(); i++) {  delete lookupFloat[i];  }
		
		if (m->control_pressed) {  return 0; }

		m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}
		m->mothurOutEndLine();
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "execute");	
		exit(1);
	}
}
//**********************************************************************************************************************
int CorrAxesCommand::calcPearson(map<string, vector<float> >& axes, ofstream& out) {
   try {
	   
	   //find average of each axis - X
	   vector<float> averageAxes; averageAxes.resize(numaxes, 0.0);
	   for (map<string, vector<float> >::iterator it = axes.begin(); it != axes.end(); it++) {
		   vector<float> temp = it->second;
		   for (int i = 0; i < temp.size(); i++) {
			   averageAxes[i] += temp[i];  
		   }
	   }
	   
	   for (int i = 0; i < averageAxes.size(); i++) {  averageAxes[i] = averageAxes[i] / (float) axes.size(); }
	   
	   //for each otu
	   for (int i = 0; i < lookupFloat[0]->getNumBins(); i++) {
		   
		   out << i+1 << '\t';
		   
		   //find the averages this otu - Y
		   float sumOtu = 0.0;
		   for (int j = 0; j < lookupFloat.size(); j++) {
			   sumOtu += lookupFloat[j]->getAbundance(i);
		   }
		   float Ybar = sumOtu / (float) lookupFloat.size();
		   
		   //find r value for each axis
		   for (int k = 0; k < averageAxes.size(); k++) {
			   
			   double r = 0.0;
			   double numerator = 0.0;
			   double denomTerm1 = 0.0;
			   double denomTerm2 = 0.0;
			   for (int j = 0; j < lookupFloat.size(); j++) {
				   float Yi = lookupFloat[j]->getAbundance(i);
				   float Xi = axes[lookupFloat[j]->getGroup()][k];
				   
				   numerator += ((Xi - averageAxes[k]) * (Yi - Ybar));
				   denomTerm1 += ((Xi - averageAxes[k]) * (Xi - averageAxes[k]));
				   denomTerm2 += ((Yi - Ybar) * (Yi - Ybar));
			   }
			   
			   double denom = (sqrt(denomTerm1) * sqrt(denomTerm2));
			   
			   r = numerator / denom;
			   
			   out << r << '\t'; 
		   }
		   
		   out << endl;
	   }
	   	   
	   return 0;
   }
   catch(exception& e) {
	   m->errorOut(e, "CorrAxesCommand", "calcPearson");
	   exit(1);
   }
}
//**********************************************************************************************************************
int CorrAxesCommand::calcSpearman(map<string, vector<float> >& axes, ofstream& out) {
	try {
		
		//format data
		vector< vector<spearmanRank> > scores; scores.resize(numaxes);
		for (map<string, vector<float> >::iterator it = axes.begin(); it != axes.end(); it++) {
			vector<float> temp = it->second;
			for (int i = 0; i < temp.size(); i++) {
				spearmanRank member(it->first, temp[i]);
				scores[i].push_back(member);  
			}
		}
		
		//sort each axis
		for (int i = 0; i < numaxes; i++) {  sort(scores[i].begin(), scores[i].end(), compareSpearman); }
		
		//find ranks of xi in each axis
		map<string, vector<float> > rankAxes;
		for (int i = 0; i < numaxes; i++) {
			
			vector<spearmanRank> ties;
			int rankTotal = 0;
			for (int j = 0; j < scores[i].size(); j++) {
				rankTotal += j;
				ties.push_back(scores[i][j]);
				
				if (j != scores.size()-1) { // you are not the last so you can look ahead
					if (scores[i][j].score != scores[i][j+1].score) { // you are done with ties, rank them and continue
						for (int k = 0; k < ties.size(); k++) {
							float thisrank = rankTotal / (float) ties.size();
  							rankAxes[ties[k].name].push_back(thisrank);
						}
						ties.clear();
						rankTotal = 0;
					}
				}else { // you are the last one
					for (int k = 0; k < ties.size(); k++) {
						float thisrank = rankTotal / (float) ties.size();
						rankAxes[ties[k].name].push_back(thisrank);
					}
				}
			}
		}
		
				
		
		//for each otu
		for (int i = 0; i < lookupFloat[0]->getNumBins(); i++) {
			
			out << i+1 << '\t';
			
			//find the ranks of this otu - Y
			vector<spearmanRank> otuScores;
			for (int j = 0; j < lookupFloat.size(); j++) {
				spearmanRank member(lookupFloat[j]->getGroup(), lookupFloat[j]->getAbundance(i));
				otuScores.push_back(member);
			}
			
			sort(otuScores.begin(), otuScores.end(), compareSpearman);
			
			map<string, float> rankOtus;
			vector<spearmanRank> ties;
			int rankTotal = 0;
			for (int j = 0; j < otuScores.size(); j++) {
				rankTotal += j;
				ties.push_back(otuScores[j]);
				
				if (j != scores.size()-1) { // you are not the last so you can look ahead
					if (otuScores[j].score != otuScores[j+1].score) { // you are done with ties, rank them and continue
						for (int k = 0; k < ties.size(); k++) {
							float thisrank = rankTotal / (float) ties.size();
  							rankOtus[ties[k].name] = thisrank;
						}
						ties.clear();
						rankTotal = 0;
					}
				}else { // you are the last one
					for (int k = 0; k < ties.size(); k++) {
						float thisrank = rankTotal / (float) ties.size();
						rankOtus[ties[k].name] = thisrank;
					}
				}
			}
			
			//calc spearman ranks for each axis for this otu
			for (int j = 0; j < numaxes; j++) {
				
				double di = 0.0;
				for (int k = 0; k < lookupFloat.size(); k++) {
					
					float xi = rankAxes[lookupFloat[k]->getGroup()][j];
					float yi = rankOtus[lookupFloat[k]->getGroup()];
					
					di += ((xi - yi) * (xi - yi));
				}
				
				int n = lookupFloat.size();
				double p = 1.0 - ((6 * di) / (float) (n * ((n*n) - 1)));
				
				out << p << '\t';
			}
			
			
			out << endl;
		}
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "calcSpearman");
		exit(1);
	}
}
//**********************************************************************************************************************
int CorrAxesCommand::calcKendall(map<string, vector<float> >& axes, ofstream& out) {
	try {
		
		//format data
		vector< vector<spearmanRank> > scores; scores.resize(numaxes);
		for (map<string, vector<float> >::iterator it = axes.begin(); it != axes.end(); it++) {
			vector<float> temp = it->second;
			for (int i = 0; i < temp.size(); i++) {
				spearmanRank member(it->first, temp[i]);
				scores[i].push_back(member);  
			}
		}
		
		//sort each axis
		for (int i = 0; i < numaxes; i++) {  sort(scores[i].begin(), scores[i].end(), compareSpearman); }
		
		//find ranks of xi in each axis
		map<string, vector<float> > rankAxes;
		for (int i = 0; i < numaxes; i++) {
			
			vector<spearmanRank> ties;
			int rankTotal = 0;
			for (int j = 0; j < scores[i].size(); j++) {
				rankTotal += j;
				ties.push_back(scores[i][j]);
				
				if (j != scores.size()-1) { // you are not the last so you can look ahead
					if (scores[i][j].score != scores[i][j+1].score) { // you are done with ties, rank them and continue
						for (int k = 0; k < ties.size(); k++) {
							float thisrank = rankTotal / (float) ties.size();
  							rankAxes[ties[k].name].push_back(thisrank);
						}
						ties.clear();
						rankTotal = 0;
					}
				}else { // you are the last one
					for (int k = 0; k < ties.size(); k++) {
						float thisrank = rankTotal / (float) ties.size();
						rankAxes[ties[k].name].push_back(thisrank);
					}
				}
			}
		}
		
		
		
		//for each otu
		for (int i = 0; i < lookupFloat[0]->getNumBins(); i++) {
			
			out << i+1 << '\t';
			
			//find the ranks of this otu - Y
			vector<spearmanRank> otuScores;
			for (int j = 0; j < lookupFloat.size(); j++) {
				spearmanRank member(lookupFloat[j]->getGroup(), lookupFloat[j]->getAbundance(i));
				otuScores.push_back(member);
			}
			
			sort(otuScores.begin(), otuScores.end(), compareSpearman);
			
			map<string, float> rankOtus;
			vector<spearmanRank> ties;
			int rankTotal = 0;
			for (int j = 0; j < otuScores.size(); j++) {
				rankTotal += j;
				ties.push_back(otuScores[j]);
				
				if (j != scores.size()-1) { // you are not the last so you can look ahead
					if (otuScores[j].score != otuScores[j+1].score) { // you are done with ties, rank them and continue
						for (int k = 0; k < ties.size(); k++) {
							float thisrank = rankTotal / (float) ties.size();
  							rankOtus[ties[k].name] = thisrank;
						}
						ties.clear();
						rankTotal = 0;
					}
				}else { // you are the last one
					for (int k = 0; k < ties.size(); k++) {
						float thisrank = rankTotal / (float) ties.size();
						rankOtus[ties[k].name] = thisrank;
					}
				}
			}
			
			//calc kendall coeffient for each axis for this otu
			for (int j = 0; j < numaxes; j++) {
				
				int numConcordant = 0;
				int numDiscordant = 0;
				
				for (int f = 0; f < j; f++) {
					
					for (int k = 0; k < lookupFloat.size(); k++) {
						
						float xi = rankAxes[lookupFloat[k]->getGroup()][j];
						float yi = rankOtus[lookupFloat[k]->getGroup()];
						
						for (int h = 0; h < k; h++) {
							
							float xj = rankAxes[lookupFloat[k]->getGroup()][f];
							float yj = rankOtus[lookupFloat[h]->getGroup()];
							
							if ( ((xi > xj) && (yi < yj)) || ((xi < xj) && (yi > yj)) ){  numDiscordant++;  }
							if ( ((xi > xj) && (yi > yj)) || ((xi < xj) && (yi < yj)) ){  numConcordant++;  }
						}
					}
				}
				
				int n = lookupFloat.size();
				double p = (numConcordant - numDiscordant) / (float) (0.5 * n * (n - 1));
				
				out << p << '\t';
			}
			
			
			out << endl;
		}
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "calcKendall");
		exit(1);
	}
}
//**********************************************************************************************************************
int CorrAxesCommand::getShared(){
	try {
		InputData* input = new InputData(sharedfile, "sharedfile");
		lookup = input->getSharedRAbundVectors();
		string lastLabel = lookup[0]->getLabel();
		
		if (label == "") { label = lastLabel; delete input; return 0; }
		
		//if the users enters label "0.06" and there is no "0.06" in their file use the next lowest label.
		set<string> labels; labels.insert(label);
		set<string> processedLabels;
		set<string> userLabels = labels;
		
		//as long as you are not at the end of the file or done wih the lines you want
		while((lookup[0] != NULL) && (userLabels.size() != 0)) {
			if (m->control_pressed) {  delete input; return 0;  }
			
			if(labels.count(lookup[0]->getLabel()) == 1){
				processedLabels.insert(lookup[0]->getLabel());
				userLabels.erase(lookup[0]->getLabel());
				break;
			}
			
			if ((m->anyLabelsToProcess(lookup[0]->getLabel(), userLabels, "") == true) && (processedLabels.count(lastLabel) != 1)) {
				string saveLabel = lookup[0]->getLabel();
				
				for (int i = 0; i < lookup.size(); i++) {  delete lookup[i];  } 
				lookup = input->getSharedRAbundVectors(lastLabel);
				
				processedLabels.insert(lookup[0]->getLabel());
				userLabels.erase(lookup[0]->getLabel());
				
				//restore real lastlabel to save below
				lookup[0]->setLabel(saveLabel);
				break;
			}
			
			lastLabel = lookup[0]->getLabel();			
			
			//get next line to process
			//prevent memory leak
			for (int i = 0; i < lookup.size(); i++) {  delete lookup[i];  } 
			lookup = input->getSharedRAbundVectors();
		}
		
		
		if (m->control_pressed) { delete input; return 0;  }
		
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
		
		//run last label if you need to
		if (needToRun == true)  {
			for (int i = 0; i < lookup.size(); i++) {  if (lookup[i] != NULL) {	delete lookup[i];	} } 
			lookup = input->getSharedRAbundVectors(lastLabel);
		}	
		
		delete input;
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "getShared");	
		exit(1);
	}
}
//**********************************************************************************************************************
int CorrAxesCommand::getSharedFloat(){
	try {
		InputData* input = new InputData(relabundfile, "relabund");
		lookupFloat = input->getSharedRAbundFloatVectors();
		string lastLabel = lookupFloat[0]->getLabel();
		
		if (label == "") { label = lastLabel; delete input; return 0; }
		
		//if the users enters label "0.06" and there is no "0.06" in their file use the next lowest label.
		set<string> labels; labels.insert(label);
		set<string> processedLabels;
		set<string> userLabels = labels;
		
		//as long as you are not at the end of the file or done wih the lines you want
		while((lookupFloat[0] != NULL) && (userLabels.size() != 0)) {
			
			if (m->control_pressed) {  delete input; return 0;  }
			
			if(labels.count(lookupFloat[0]->getLabel()) == 1){
				processedLabels.insert(lookupFloat[0]->getLabel());
				userLabels.erase(lookupFloat[0]->getLabel());
				break;
			}
			
			if ((m->anyLabelsToProcess(lookupFloat[0]->getLabel(), userLabels, "") == true) && (processedLabels.count(lastLabel) != 1)) {
				string saveLabel = lookupFloat[0]->getLabel();
				
				for (int i = 0; i < lookupFloat.size(); i++) {  delete lookupFloat[i];  } 
				lookupFloat = input->getSharedRAbundFloatVectors(lastLabel);
				
				processedLabels.insert(lookupFloat[0]->getLabel());
				userLabels.erase(lookupFloat[0]->getLabel());
				
				//restore real lastlabel to save below
				lookupFloat[0]->setLabel(saveLabel);
				break;
			}
			
			lastLabel = lookupFloat[0]->getLabel();			
			
			//get next line to process
			//prevent memory leak
			for (int i = 0; i < lookupFloat.size(); i++) {  delete lookupFloat[i];  } 
			lookupFloat = input->getSharedRAbundFloatVectors();
		}
		
		
		if (m->control_pressed) { delete input; return 0;  }
		
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
		
		//run last label if you need to
		if (needToRun == true)  {
			for (int i = 0; i < lookupFloat.size(); i++) {  if (lookupFloat[i] != NULL) {	delete lookupFloat[i];	} } 
			lookupFloat = input->getSharedRAbundFloatVectors(lastLabel);
		}	
		
		delete input;
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "getSharedFloat");	
		exit(1);
	}
}
/*****************************************************************/
int CorrAxesCommand::convertToRelabund(){
	try {
		
		vector<SharedRAbundFloatVector*> newLookup;
		for (int i = 0; i < lookup.size(); i++) {
			SharedRAbundFloatVector* temp = new SharedRAbundFloatVector();
			temp->setLabel(lookup[i]->getLabel());
			temp->setGroup(lookup[i]->getGroup());
			newLookup.push_back(temp);
		}
		
		for (int i = 0; i < lookup.size(); i++) {
			
			for (int j = 0; j < lookup[i]->getNumBins(); j++) {
				
				if (m->control_pressed) { return 0; }
				
				int abund = lookup[i]->getAbundance(j);
				
				float relabund = abund / (float) lookup[i]->getNumSeqs();
				
				newLookup[i]->push_back(relabund, lookup[i]->getGroup());
			}
		}
		
		if (pickedGroups) { eliminateZeroOTUS(newLookup); }
		
		lookupFloat = newLookup;
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "convertToRelabund");	
		exit(1);
	}
}
//**********************************************************************************************************************
int CorrAxesCommand::eliminateZeroOTUS(vector<SharedRAbundFloatVector*>& thislookup) {
	try {
		
		vector<SharedRAbundFloatVector*> newLookup;
		for (int i = 0; i < thislookup.size(); i++) {
			SharedRAbundFloatVector* temp = new SharedRAbundFloatVector();
			temp->setLabel(thislookup[i]->getLabel());
			temp->setGroup(thislookup[i]->getGroup());
			newLookup.push_back(temp);
		}
		
		//for each bin
		for (int i = 0; i < thislookup[0]->getNumBins(); i++) {
			if (m->control_pressed) { for (int j = 0; j < newLookup.size(); j++) {  delete newLookup[j];  } return 0; }
			
			//look at each sharedRabund and make sure they are not all zero
			bool allZero = true;
			for (int j = 0; j < thislookup.size(); j++) {
				if (thislookup[j]->getAbundance(i) != 0) { allZero = false;  break;  }
			}
			
			//if they are not all zero add this bin
			if (!allZero) {
				for (int j = 0; j < thislookup.size(); j++) {
					newLookup[j]->push_back(thislookup[j]->getAbundance(i), thislookup[j]->getGroup());
				}
			}
		}
		
		for (int j = 0; j < thislookup.size(); j++) {  delete thislookup[j];  }
		
		thislookup = newLookup;
		
		return 0;
		
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "eliminateZeroOTUS");
		exit(1);
	}
}
/*****************************************************************/
map<string, vector<float> > CorrAxesCommand::readAxes(){
	try {
		map<string, vector<float> > axes;
		
		ifstream in;
		m->openInputFile(axesfile, in);
		
		string headerLine = m->getline(in); m->gobble(in);
		
		//count the number of axis you are reading
		bool done = false;
		int count = 0;
		while (!done) {
			int pos = headerLine.find("axis");
			if (pos != string::npos) {
				count++;
				headerLine = headerLine.substr(pos+4);
			}else { done = true; }
		}
		
		while (!in.eof()) {
			
			if (m->control_pressed) { in.close(); return axes; }
			
			string group = "";
			in >> group; m->gobble(in);
			
			vector<float> thisGroupsAxes;
			for (int i = 0; i < count; i++) {
				float temp = 0.0;
				in >> temp; 
				
				//only save the axis we want
				if (i < numaxes) {  thisGroupsAxes.push_back(temp); }
			}
			
			//save group if its one the user selected
			if (names.count(group) != 0) {
				map<string, vector<float> >::iterator it = axes.find(group);
				
				if (it == axes.end()) {
					axes[group] = thisGroupsAxes;
				}else {
					m->mothurOut(group + " is already in your axes file, using first definition."); m->mothurOutEndLine();
				}
			}
			
			m->gobble(in);
		}
		in.close();
		
		return axes;
	}
	catch(exception& e) {
		m->errorOut(e, "CorrAxesCommand", "convertToRelabund");	
		exit(1);
	}
}
/*****************************************************************/





