/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include "value.h"
#include <vector>
#include <map>
#include <string>
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE, IDENTTYPE};

// a "forward declaration" for a class to hold values
class Value;

class ParseTree {
public:
	int			linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLinenum() const { return linenum; }
    virtual string GetID() const { return "NO WORKING"; }
	virtual NodeType GetType() const { return ERRTYPE; }

	int LeafCount() const {
		int lc = 0;
		if( left ) lc += left->LeafCount();
		if( right ) lc += right->LeafCount();
		if( left == 0 && right == 0 )
			lc++;
		return lc;
	}

	virtual bool IsIdent() const { return false; }
	virtual bool IsString() const { return false; }

	virtual string GetId() const { return ""; }
    
    virtual Value Eval(map<string, Value> *symbolMap) { 
        return Value();
    }

	int IdentCount() const {
		int cnt = 0;
		if( left ) cnt += left->IdentCount();
		if( right ) cnt += right->IdentCount();
		if( IsIdent() )
			cnt++;
		return cnt;
	}

	int StringCount() const {
		int cnt = 0;
		if( left ) cnt += left->StringCount();
		if( right ) cnt += right->StringCount();
		if( IsString() )
			cnt++;
		return cnt;
	}

	void GetVars(map<string,int>& var) {
		if( left ) left->GetVars(var);
		if( right ) right->GetVars(var);
		if( IsIdent() )
			var[ this->GetId() ]++;
	}
};

class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    
    Value Eval(map<string, Value> *sMap) { 
        Value leftVal = left->Eval(sMap);
        if(leftVal.hasMessage())
        {
           cout<<to_string(GetLinenum())+": RUNTIME ERROR " + leftVal.getMessage()<<endl;
        
        }
        else if(right!=0)
        {

            Value rightValue = right->Eval(sMap);
            if(rightValue.hasMessage()){
                cout<<to_string(GetLinenum())+": RUNTIME ERROR " + rightValue.getMessage()<<endl;
                
            }
        }
        return Value();
    }

};

class IfStatement : public ParseTree {
public:
	IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    
    Value Eval(map<string, Value> *sMap) {
        Value expr = left->Eval(sMap);
        if(expr.isBoolType())
        {
            if(expr.getBoolean()){
                Value statement = right->Eval(sMap);
                return statement;
            } 
            
            return Value();
        }        
        return Value("error", true);
    }
};

class Assignment : public ParseTree {
public:
	Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}
    
    Value Eval(map<string, Value> *sMap) {
        Value aValue = right->Eval(sMap);
        if(left->GetType()==IDENTTYPE)
        {
            (*sMap)[left->GetID()]=aValue;
            return Value();
        }
        else{
            return Value("error", true);
        }
    }
};

class PrintStatement : public ParseTree {
public:
	PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    
    Value Eval(map<string, Value> *sMap) {
        Value Msg = left->Eval(sMap);
        
        if(left->GetType()==IDENTTYPE){
            auto it = sMap->find(left->GetID());
            if(it == sMap->end()){
                return Value("error", true);
            }
        }
        if(Msg.isError()){
            return Value(Msg.getMessage(), true);
        }
        cout<<Msg<<endl;
        return Value();
    }
};

class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {
        auto R = right->Eval(sMap);
        auto L = left->Eval(sMap);
        return L+R;
    }
};

class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {        
        auto R = right->Eval(sMap);
        auto L = left->Eval(sMap);
        return L-R;
    }
};

class TimesExpr : public ParseTree {
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {         
        auto R = right->Eval(sMap);
        auto L = left->Eval(sMap);
        return L*R;
    }
};

class DivideExpr : public ParseTree {
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {       
        auto R = right->Eval(sMap);
        auto L = left->Eval(sMap);
        return L/R;
    }
};

class LogicAndExpr : public ParseTree {
public:
	LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {        
         return left->Eval(sMap) && right->Eval(sMap);
    }

	NodeType GetType() const { return BOOLTYPE; }
};

class LogicOrExpr : public ParseTree {
public:
	LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {       
         return left->Eval(sMap) || right->Eval(sMap);            
    }
    
	NodeType GetType() const { return BOOLTYPE; }
};

class EqExpr : public ParseTree {
public:
	EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {         
         return left->Eval(sMap) == right->Eval(sMap);
    }

	NodeType GetType() const { return BOOLTYPE; }
};

class NEqExpr : public ParseTree {
public:
	NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap)
    {     
         return left->Eval(sMap) != right->Eval(sMap);
    }

	NodeType GetType() const { return BOOLTYPE; }
};

class LtExpr : public ParseTree {
public:
	LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {         
         return  left->Eval(sMap) < right->Eval(sMap);
    }

	NodeType GetType() const { return BOOLTYPE; }
};

class LEqExpr : public ParseTree {
public:
	LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {         
         return left->Eval(sMap) <= right->Eval(sMap);
    }

	NodeType GetType() const { return BOOLTYPE; }
};

class GtExpr : public ParseTree {
public:
	GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    Value Eval(map<string, Value> *sMap) 
    {        
         return left->Eval(sMap) > right->Eval(sMap);
    }

	NodeType GetType() const { return BOOLTYPE; }
};

class GEqExpr : public ParseTree {
public:
	GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
     Value Eval(map<string, Value> *sMap) 
     {
         return left->Eval(sMap) >= right->Eval(sMap);
     }

	NodeType GetType() const { return BOOLTYPE; }
};

class IConst : public ParseTree {
	int val;

public:
	IConst(int l, int i) : ParseTree(l), val(i) {}
	IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    
    NodeType GetType() const { return INTTYPE; }
    
    Value Eval(map<string, Value> *sMap) 
    {
        return Value(val);
    }

	
};

class BoolConst : public ParseTree {
	bool val;

public:
	BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}
    NodeType GetType() const { return BOOLTYPE; }
    
    Value Eval(map<string, Value> *sMap) 
    {        
        return Value(val);
    }	
};

class SConst : public ParseTree {
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}

	NodeType GetType() const { return STRTYPE; }
	bool IsString() const { return true; }
    
    Value Eval(map<string, Value> *sMap) 
    {
        return Value(val);
    }
};

class Ident : public ParseTree {
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}
    
    NodeType GetType() const { return IDENTTYPE; }
    
    Value Eval(map<string, Value> *sMap) 
    {
        if(sMap->count(id)){return sMap->at(id);}
        else{return Value("error", true);}            
    }

	bool IsIdent() const { return true; }
	string GetID() const { return id; }
};

#endif /* PARSETREE_H_ */
