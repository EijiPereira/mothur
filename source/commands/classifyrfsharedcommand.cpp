//
//  classifysharedcommand.cpp
//  Mothur
//
//  Created by Abu Zaher Md. Faridee on 8/13/12.
//  Copyright (c) 2012 Schloss Lab. All rights reserved.
//

#include "classifyrfsharedcommand.h"
#include "randomforest.hpp"
#include "decisiontree.hpp"
#include "rftreenode.hpp"


//**********************************************************************************************************************
vector<string> ClassifyRFSharedCommand::setParameters(){	
	try {
        CommandParameter pshared("shared", "InputTypes", "", "", "none", "none", "none","summary",false,true,true); parameters.push_back(pshared);		
        CommandParameter pdesign("design", "InputTypes", "", "", "none", "none", "none","",false,true,true); parameters.push_back(pdesign);	
        CommandParameter potupersplit("otupersplit", "Multiple", "log2-squareroot", "log2", "", "", "","",false,false); parameters.push_back(potupersplit);
        CommandParameter psplitcriteria("splitcriteria", "Multiple", "gainratio-infogain", "gainratio", "", "", "","",false,false); parameters.push_back(psplitcriteria);
		CommandParameter pnumtrees("numtrees", "Number", "", "100", "", "", "","",false,false); parameters.push_back(pnumtrees);
        CommandParameter pdopruning("prune", "Boolean", "", "T", "", "", "", "", false, false); parameters.push_back(pdopruning);
        CommandParameter ppruneaggrns("pruneaggressiveness", "Number", "", "0.9", "", "", "", "", false, false); parameters.push_back(ppruneaggrns);
        CommandParameter pdiscardhetrees("discarderrortrees", "Boolean", "", "T", "", "", "", "", false, false); parameters.push_back(pdiscardhetrees);
        CommandParameter phetdiscardthreshold("errorthreshold", "Number", "", "0.4", "", "", "", "", false, false); parameters.push_back(phetdiscardthreshold);
        CommandParameter psdthreshold("stdthreshold", "Number", "", "0.0", "", "", "", "", false, false); parameters.push_back(psdthreshold);
         CommandParameter pgroups("groups", "String", "", "", "", "", "","",false,false); parameters.push_back(pgroups);
		CommandParameter plabel("label", "String", "", "", "", "", "","",false,false); parameters.push_back(plabel);
  		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "ClassifySharedCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string ClassifyRFSharedCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The classify.rf command allows you to ....\n";
		helpString += "The classify.rf command parameters are: shared, design, label, groups, otupersplit.\n";
        helpString += "The label parameter is used to analyze specific labels in your input.\n";
		helpString += "The groups parameter allows you to specify which of the groups in your designfile you would like analyzed.\n";
		helpString += "The classify.rf should be in the following format: \n";
		helpString += "classify.rf(shared=yourSharedFile, design=yourDesignFile)\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "ClassifySharedCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string ClassifyRFSharedCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "summary") {  pattern = "[filename],[distance],summary"; } //makes file like: amazon.0.03.fasta
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "ClassifySharedCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************

ClassifyRFSharedCommand::ClassifyRFSharedCommand() {
  try {
    abort = true; calledHelp = true;
    setParameters();
    vector<string> tempOutNames;
    outputTypes["summary"] = tempOutNames; 
  }
  catch(exception& e) {
    m->errorOut(e, "ClassifySharedCommand", "ClassifySharedCommand");
    exit(1);
  }
}

//**********************************************************************************************************************
ClassifyRFSharedCommand::ClassifyRFSharedCommand(string option) {
  try {
    abort = false; calledHelp = false;
    allLines = true;
      
      //allow user to run help
    if(option == "help") { help(); abort = true; calledHelp = true; }
    else if(option == "citation") { citation(); abort = true; calledHelp = true;}
    
    else {
        //valid paramters for this command
      vector<string> myArray = setParameters();
      
      OptionParser parser(option);
      map<string,string> parameters = parser.getParameters();
      
      ValidParameters validParameter;
      map<string,string>::iterator it;
        //check to make sure all parameters are valid for command
      for (it = parameters.begin(); it != parameters.end(); it++) {
        if (!validParameter.isValidParameter(it->first, myArray, it->second)) {  abort = true;  }
      }
        vector<string> tempOutNames;
        outputTypes["summary"] = tempOutNames;
      
        //if the user changes the input directory command factory will send this info to us in the output parameter
      string inputDir = validParameter.valid(parameters, "inputdir");
      if (inputDir == "not found"){	inputDir = "";		}
      else {
        string path;
        it = parameters.find("shared");
          //user has given a shared file
        if(it != parameters.end()){
          path = util.hasPath(it->second);
            //if the user has not given a path then, add inputdir. else leave path alone.
          if (path == "") {	parameters["shared"] = inputDir + it->second;		}
        }
        
        it = parameters.find("design");
          //user has given a design file
        if(it != parameters.end()){
          path = util.hasPath(it->second);
            //if the user has not given a path then, add inputdir. else leave path alone.
          if (path == "") {	parameters["design"] = inputDir + it->second;		}
        }
        
      }
        //check for parameters
        //get shared file, it is required
      sharedfile = validParameter.validFile(parameters, "shared");
      if (sharedfile == "not open") { sharedfile = ""; abort = true; }
      else if (sharedfile == "not found") {
          //if there is a current shared file, use it
        sharedfile = current->getSharedFile();
        if (sharedfile != "") { m->mothurOut("Using " + sharedfile + " as input file for the shared parameter."); m->mothurOutEndLine(); }
        else { 	m->mothurOut("You have no current sharedfile and the shared parameter is required."); m->mothurOutEndLine(); abort = true; }
      }else { current->setSharedFile(sharedfile); }
      
        //get design file, it is required
      designfile = validParameter.validFile(parameters, "design");
      if (designfile == "not open") { sharedfile = ""; abort = true; }
      else if (designfile == "not found") {
          //if there is a current shared file, use it
        designfile = current->getDesignFile();
        if (designfile != "") { m->mothurOut("Using " + designfile + " as input file for the design parameter."); m->mothurOutEndLine(); }
        else { 	m->mothurOut("You have no current designfile and the design parameter is required."); m->mothurOutEndLine(); abort = true; }
      }else { current->setDesignFile(designfile); }

      
        //if the user changes the output directory command factory will send this info to us in the output parameter
      outputDir = validParameter.valid(parameters, "outputdir");		if (outputDir == "not found"){
        outputDir = util.hasPath(sharedfile); //if user entered a file with a path then preserve it
      }
      
        // NEW CODE for OTU per split selection criteria
        string temp = validParameter.valid(parameters, "splitcriteria");
        if (temp == "not found") { temp = "gainratio"; }
        if ((temp == "gainratio") || (temp == "infogain")) {
            treeSplitCriterion = temp;
        } else { m->mothurOut("Not a valid tree splitting criterio. Valid tree splitting criteria are 'gainratio' and 'infogain'.");
            m->mothurOutEndLine();
            abort = true;
        }
        
        temp = validParameter.valid(parameters, "numtrees"); if (temp == "not found"){	temp = "100";	}
        util.mothurConvert(temp, numDecisionTrees);
        
            // parameters for pruning
        temp = validParameter.valid(parameters, "prune");
        if (temp == "not found") { temp = "f"; }
        doPruning = util.isTrue(temp);
        
        temp = validParameter.valid(parameters, "pruneaggressiveness");
        if (temp == "not found") { temp = "0.9"; }
        util.mothurConvert(temp, pruneAggressiveness);
        
        temp = validParameter.valid(parameters, "discarderrortrees");
        if (temp == "not found") { temp = "f"; }
        discardHighErrorTrees = util.isTrue(temp);
        
        temp = validParameter.valid(parameters, "errorthreshold");
        if (temp == "not found") { temp = "0.4"; }
        util.mothurConvert(temp, highErrorTreeDiscardThreshold);
        
        temp = validParameter.valid(parameters, "otupersplit");
        if (temp == "not found") { temp = "log2"; }
        if ((temp == "squareroot") || (temp == "log2")) {
            optimumFeatureSubsetSelectionCriteria = temp;
        } else { m->mothurOut("Not a valid OTU per split selection method. Valid OTU per split selection methods are 'log2' and 'squareroot'.");
            m->mothurOutEndLine();
            abort = true;
        }
        
        temp = validParameter.valid(parameters, "stdthreshold");
        if (temp == "not found") { temp = "0.0"; }
        util.mothurConvert(temp, featureStandardDeviationThreshold);
                        
            // end of pruning params
        
      string groups = validParameter.valid(parameters, "groups");
      if (groups == "not found") { groups = ""; }
      else { util.splitAtDash(groups, Groups); if (Groups.size() != 0) { if (Groups[0]== "all") { Groups.clear(); } } }
        
        //Commonly used to process list, rabund, sabund, shared and relabund files.  Look at "smart distancing" examples below in the execute function.
      string label = validParameter.valid(parameters, "label");
      if (label == "not found") { label = ""; }
      else {
        if(label != "all") {  util.splitAtDash(label, labels);  allLines = false;  }
        else { allLines = true;  }
      }
    }
    
  }
  catch(exception& e) {
    m->errorOut(e, "ClassifySharedCommand", "ClassifySharedCommand");
    exit(1);
  }
}
//**********************************************************************************************************************
int ClassifyRFSharedCommand::execute() {
  try {
    
    if (abort) { if (calledHelp) { return 0; }  return 2;	}
    
      
      //read design file
      designMap.read(designfile);
      
      InputData input(sharedfile, "sharedfile", Groups);
      SharedRAbundVectors* lookup = input.getSharedRAbundVectors();
      Groups = lookup->getNamesGroups();
      
      
    string lastLabel = lookup->getLabel();
    set<string> processedLabels;
    set<string> userLabels = labels;
    
      //as long as you are not at the end of the file or done wih the lines you want
    while((lookup != NULL) && ((allLines == 1) || (userLabels.size() != 0))) {
      
        if (m->getControl_pressed()) { delete lookup;  return 0; }
      
      if(allLines == 1 || labels.count(lookup->getLabel()) == 1){
        
        m->mothurOut(lookup->getLabel()+"\n"); 
        
          vector<SharedRAbundVector*> data = lookup->getSharedRAbundVectors();
          processSharedAndDesignData(data);
          for (int i = 0; i < data.size(); i++) { delete data[i]; } data.clear();
          
        processedLabels.insert(lookup->getLabel());
        userLabels.erase(lookup->getLabel());
      }
      
      if ((util.anyLabelsToProcess(lookup->getLabel(), userLabels, "") ) && (processedLabels.count(lastLabel) != 1)) {
        string saveLabel = lookup->getLabel();
        
          delete lookup;
        lookup = input.getSharedRAbundVectors(lastLabel);
        m->mothurOut(lookup->getLabel()+"\n"); 
          vector<SharedRAbundVector*> data = lookup->getSharedRAbundVectors();
          processSharedAndDesignData(data);
          for (int i = 0; i < data.size(); i++) { delete data[i]; } data.clear();
        
        processedLabels.insert(lookup->getLabel());
        userLabels.erase(lookup->getLabel());
        
          //restore real lastlabel to save below
        lookup->setLabels(saveLabel);
      }
      
      lastLabel = lookup->getLabel();
        //prevent memory leak
      delete lookup;
      
      if (m->getControl_pressed()) { return 0; }
      
        //get next line to process
      lookup = input.getSharedRAbundVectors();
    }
    
    if (m->getControl_pressed()) {  return 0; }
    
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
    if (needToRun )  {
      delete lookup;
      lookup = input.getSharedRAbundVectors(lastLabel);
      
      m->mothurOut(lookup->getLabel()+"\n"); 
      
        vector<SharedRAbundVector*> data = lookup->getSharedRAbundVectors();
        processSharedAndDesignData(data);
        for (int i = 0; i < data.size(); i++) { delete data[i]; } data.clear();
        
      delete lookup;
      
    }

      m->mothurOutEndLine();
      m->mothurOut("Output File Names: "); m->mothurOutEndLine();
      for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}
      m->mothurOutEndLine();
      
    return 0;
    
  }
  catch(exception& e) {
    m->errorOut(e, "ClassifySharedCommand", "execute");
    exit(1);
  }
}
//**********************************************************************************************************************

void ClassifyRFSharedCommand::processSharedAndDesignData(vector<SharedRAbundVector*> lookup){
    try {
  
        map<string, int> treatmentToIntMap;
        map<int, string> intToTreatmentMap;
        //vector<string> groups = designMap.getCategory();
        for (int i = 0; i < lookup.size(); i++) {
            string treatmentName = designMap.get(lookup[i]->getGroup());
            treatmentToIntMap[treatmentName] = i;
            intToTreatmentMap[i] = treatmentName;
        }
        
        int numSamples = lookup.size();
        int numFeatures = lookup[0]->getNumBins();
        
        int numRows = numSamples;
        int numColumns = numFeatures + 1;           // extra one space needed for the treatment/outcome
        
        vector< vector<int> > dataSet(numRows, vector<int>(numColumns, 0));
        
        vector<string> names;
        
        for (int i = 0; i < lookup.size(); i++) {
            string sharedGroupName = lookup[i]->getGroup();
            names.push_back(sharedGroupName);
            string treatmentName = designMap.get(sharedGroupName);
            
            int j = 0;
            for (; j < lookup[i]->getNumBins(); j++) {
                int otuCount = lookup[i]->get(j);
                dataSet[i][j] = otuCount;
            }
            dataSet[i][j] = treatmentToIntMap[treatmentName];
        }
        
        RandomForest randomForest(dataSet, numDecisionTrees, treeSplitCriterion, doPruning, pruneAggressiveness, discardHighErrorTrees, highErrorTreeDiscardThreshold, optimumFeatureSubsetSelectionCriteria, featureStandardDeviationThreshold);
        
        randomForest.populateDecisionTrees();
        
        randomForest.calcForrestErrorRate();
        
        randomForest.printConfusionMatrix(intToTreatmentMap);
        
        
        map<string, string> variables; 
        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(sharedfile)) + "RF.";
        variables["[distance]"] = lookup[0]->getLabel();
        string filename = getOutputFileName("summary", variables);
        outputNames.push_back(filename); outputTypes["summary"].push_back(filename);
        randomForest.calcForrestVariableImportance(filename);
        
        //
        map<string, string> variable; 
        variable["[filename]"] = outputDir + util.getRootName(util.getSimpleName(sharedfile)) + "misclassifications.";
        variable["[distance]"] = lookup[0]->getLabel();
        string mc_filename = getOutputFileName("summary", variable);
        outputNames.push_back(mc_filename); outputTypes["summary"].push_back(mc_filename);
        randomForest.getMissclassifications(mc_filename, intToTreatmentMap, names);
        //
        
        m->mothurOutEndLine();
    }
    catch(exception& e) {
        m->errorOut(e, "ClassifySharedCommand", "processSharedAndDesignData");
        exit(1);
    }
}
//**********************************************************************************************************************

