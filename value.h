/*
 * value.h
 */

#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <iostream>
using namespace std;

// object holds boolean, integer, or string, and remembers which it holds
class Value {
	bool		bval;
	int		ival;
	string	sval;
	enum VT { isBool, isInt, isString, isTypeError } type;

public:
	Value() : bval(false), ival(0), type(isTypeError) {}
	Value(bool bval) : bval(bval), ival(0), type(isBool) {}
	Value(int ival) : bval(false), ival(ival), type(isInt) {}
	Value(string sval) : bval(false), ival(0), sval(sval), type(isString) {}

	// in the case of an error, I use the value to hold the error message
	Value(string sval, bool isError) : bval(false), ival(0), sval(sval), type(isTypeError) {}

	bool isBoolType() const { return type == VT::isBool; }
	bool isIntType() const { return type == VT::isInt; }
	bool isStringType() const { return type == VT::isString; }
	bool isError() const { return type == VT::isTypeError; }
	bool hasMessage() const { return isError() && sval.size() > 0; }

	bool isTrue() const { return isBoolType() && bval; }
	bool getBoolean() const {
		if( !isBoolType() )
			throw "Not boolean valued";
		return bval;
	}

	int getInteger() const {
		if( !isIntType() )
			throw "Not integer valued";
		return ival;
	}

	string getString() const {
		if( !isStringType() )
			throw "Not string valued";
		return sval;
	}

	string getMessage() const {
		if( !hasMessage() )
			throw "No message";
		return sval;
	}


	friend ostream& operator<<(ostream& out, const Value& v) {
		if( v.isBoolType()) out << (v.bval ? "True" : "False");
		else if( v.isIntType() ) out << v.getInteger();
		else if( v.isStringType() ) out << v.getString();
		else if( v.sval.size() > 0 ) out << std::to_string(1) + ": RUNTIME ERROR " << v.getMessage();
		else out << "TYPE ERROR";
		return out;
	}
    //good
	Value operator+(const Value& v)
    {
        bool checkInt = this->isIntType();
        bool checkString = this->isStringType();
        if(checkInt&& v.isIntType()){return Value(this->getInteger()+v.getInteger());}
        
        else if(checkString&&v.isStringType()){
            string valueString = this->getString() + v.getString();
            return Value(valueString);
        }
        else
        {           
            return Value("error", true);
        }
    }
    
    //good
	Value operator-(const Value& v){
        if(this->isIntType()&&v.isIntType()){return Value(this->getInteger()-v.getInteger());}
        
        if(v.isStringType() && this->isStringType()){
            string stringL = v.getString();
            string subString = this->getString();
            auto it = stringL.find(subString);
           
            stringL.replace(it, subString.length(),"");
            return stringL;
        }else{
            return Value("error", true);
        }
        return Value("error", true);
    }
    //good
	Value operator*(const Value& v)
    {
        
        if(this->isIntType()&&v.isIntType()){
            int integerValue = this->getInteger() * v.getInteger();
            return Value(integerValue);           
        }
        
        else if(v.isBoolType()&&this->isIntType()&&this->getInteger()==-1)
        {
            return !(v.getBoolean());
        }
        else if(this->isIntType()&&v.isStringType())
        {
            if(this->getInteger()>-1) 
            {
                string answer = "";
                for(int i=0; i<this->getInteger(); ++i)
                    answer+=v.getString();
                return Value(answer);
            }            
            return Value("error", true);
        }
        else if(v.isIntType() && this->isStringType()){
             if(v.getInteger()>-1){
                 string stringL = "";
                 for(int i = 0; i<v.getInteger(); i++){
                     stringL += this->getString();
                 }
                 return stringL;
             }
             else{
                 return Value("error", true);
             }
           
        }
        else
        {
            return Value("error", true);
        }
    }
	//good
	Value operator/(const Value& v)
    {
        bool checkInt = this->isIntType();
        if(checkInt&&v.isIntType()){return Value(this->getInteger()/v.getInteger());}            
        else
        {
            return Value("error", true);
        }
    }
    //good
	Value operator<(const Value& v){
        bool checkInt = this->isIntType();
        bool checkString = this->isStringType();
        if(checkInt&&v.isIntType()){return Value(this->getInteger()<v.getInteger());}            
        else if(checkString&&v.isStringType()){return Value(this->getString()<v.getString());}            
        else
        {
            return Value("error", true);
        }
    }
	//good
	Value operator<=(const Value& v)
    {
        bool checkInt = this->isIntType();
        bool checkString = this->isStringType();
        if(checkInt&&v.isIntType()){return Value(this->getInteger()<=v.getInteger());}
        else if(checkString&&v.isStringType()){return Value(this->getString()<=v.getString());}           
        else
        {           
            return Value("error", true);
        }
        
    }
	//good
	Value operator>(const Value& v)
    {
        bool checkInt = this->isIntType();
        bool checkString = this->isStringType();
        if(checkInt&&v.isIntType()){return Value(this->getInteger()>v.getInteger());}
        else if(checkString&&v.isStringType()){return Value(this->getString()>v.getString());}
        else
        {
            return Value("error", true);
        }
    }
	//good
	Value operator>=(const Value& v){
        bool checkInt = this->isIntType();
        bool checkString = this->isStringType();
        if(checkInt&&v.isIntType()){return Value(this->getInteger()>=v.getInteger());}           
        else if(checkString&&v.isStringType()){return Value(this->getString()>=v.getString());}
        else
        {            
            return Value("error", true);
        }
    }
    //good
	Value operator==(const Value& v){
        bool check = this->isIntType();
        //bool checkAgain = this->getInteger();
        if(check&&v.isIntType()){return Value(this->getInteger()==v.getInteger());}
        else if(this->isStringType()&&v.isStringType()){return Value(this->getString()==v.getString());}
        else if(this->isBoolType()&&v.isBoolType()){return Value(this->getBoolean()==v.getBoolean());}        
        else{
            return Value("error", true);
        }
    }
    //good
    Value operator||(const Value& v){
        bool check = this->isBoolType();
        //bool checkAgain = this->getBoolean();
        if(check){if(this->getBoolean()==true){return Value(true);}}            
        if(check&&v.isBoolType()){return Value(this->getBoolean()||v.getBoolean()); }
        return Value("error", true);
    }
    //good
    Value operator&&(const Value& v)
    {
        bool check = this->isBoolType();
        bool checkAgain = this->getBoolean();
        if(check==true){if(checkAgain==false){return Value(false);}}
        if(check&&v.isBoolType()){return Value(check&&v.getBoolean());}          
        return Value("error", true);
    }
    //given
	Value operator!=(const Value& v) {
		Value ans = this->operator==(v);
		ans.bval = !ans.bval;
		return ans;
	}
};

#endif /* VALUE_H_ */
