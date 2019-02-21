//
//  classifysharedcommand.h
//  Mothur
//
//  Created by Abu Zaher Md. Faridee on 8/13/12.
//  Copyright (c) 2012 Schloss Lab. All rights reserved.
//

#ifndef __Mothur__classifyrfsharedcommand__
#define __Mothur__classifyrfsharedcommand__

#include "command.hpp"
#include "inputdata.h"
#include "designmap.h"

class ClassifyRFSharedCommand : public Command {
public:
  ClassifyRFSharedCommand();
  ClassifyRFSharedCommand(string);
  ~ClassifyRFSharedCommand() {};
  
  vector<string> setParameters();
  string getCommandName()			{ return "classify.rf";     }
  string getCommandCategory()		{ return "OTU-Based Approaches";		}  
  string getHelpString();	
  string getOutputPattern(string);
  string getCitation() { return "http://www.mothur.org/wiki/Classify.rf\n"; }
  string getDescription()		{ return "implements the random forest machine learning algorithm to identify OTUs that can be used to differentiate between various groups of samples"; }
  int execute();
  
  void help() { m->mothurOut(getHelpString()); }

private:
    bool abort, allLines, useTiming, doPruning, discardHighErrorTrees;
    string outputDir, sharedfile, designfile, sets, treeSplitCriterion, optimumFeatureSubsetSelectionCriteria;
    vector<string> outputNames, Groups, Sets;
    double pruneAggressiveness, highErrorTreeDiscardThreshold, featureStandardDeviationThreshold;
    int numDecisionTrees, processors;
    set<string> labels;
    DesignMap designMap;
    
    void processSharedAndDesignData(vector<SharedRAbundVector*> lookup);
};

#endif /* defined(__Mothur__classifyrfsharedcommand__) */
