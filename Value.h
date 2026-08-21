#include "String.h"

enum class ValueType
{
    Invalid,
    Int,
    String,
    Bool
};

class Value
{
public:
    virtual ~Value() = default;
    virtual ValueType Type(){return ValueType::Invalid;};
    virtual String AsString(){return String{};}
};


class StringValue : public Value
{
public:
StringValue(String NewString):
    _String(NewString)
    {}
    
    String GetString(){ return _String; }

    // Overrides
    virtual ValueType Type() override { return ValueType::String; }
    virtual String AsString() override { return _String; }
    
private:
    String _String;
};

class BoolValue : public Value
{
public:
    BoolValue(const bool& NewBool):
    _Bool(NewBool)
    {}
    
    bool GetBool(){ return _Bool; }

    // Overrides
    virtual ValueType Type() override {return ValueType::Bool;}
    virtual String AsString() override {return String(_Bool ? "True" : "False");}

private:
    bool _Bool;
};

class IntValue : public Value
{
public:
    IntValue(const int& NewInt):
    Int(NewInt)
    {}
    
    const int&  GetInt(){ return Int; }

    // Overrides
    virtual ValueType Type() override {return ValueType::Int;}
    virtual String AsString() override {return String(std::to_string(Int));}

    constexpr operator int() const
    {
        return Int;
    }
private:
    int Int;
};
