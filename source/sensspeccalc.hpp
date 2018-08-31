//
//  sensspeccalc.hpp
//  Mothur
//
//  Created by Sarah Westcott on 1/22/18.
//  Copyright © 2018 Schloss Lab. All rights reserved.
//

#ifndef sensspeccalc_hpp
#define sensspeccalc_hpp

#include "mothurout.h"
#include "optimatrix.h"

class SensSpecCalc {
    
public:
    SensSpecCalc(OptiMatrix& matrix, ListVector* list);
    ~SensSpecCalc(){}
    
    void getResults(OptiMatrix& matrix, long long& tp, long long& tn, long long& fp, long long& fn);
    
private:
    Utils util;
    MothurOut* m;
    vector<vector< int> > otus;
};



#endif /* sensspeccalc_hpp */
