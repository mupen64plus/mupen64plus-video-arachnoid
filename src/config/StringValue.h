#ifndef STRING_VALUE_H_
#define STRING_VALUE_H_
#include <cstdlib>
#include <string>
#include <sstream>

//! Represents numeric value(s) with a string using conversion operators 
struct StringValue
{
    StringValue() {data = "";}
    StringValue(const StringValue& v) { data=v.data; }
    StringValue(const std::string& s) { data=s; }
    StringValue(const char *s) { data = s; }

    StringValue(bool b) 
    {
        std::stringstream s;
        s << b;
        data= s.str();
    }

    StringValue(int i) { 
        std::stringstream s;
        s << i;
        data= s.str();
    }

    StringValue(float f) {
        std::stringstream s;
        s << f;
        data= s.str();
    }

    StringValue(double f) {
        std::stringstream s;
        s << f;
        data= s.str();
    }

    // assignment operators
    StringValue& operator=(const char* s) { data=s; return *this; }
    StringValue& operator=(const std::string& s) { data=s; return *this; }
    StringValue& operator=(const StringValue& v) { data=v.data; return *this; }
    template<class T> StringValue& operator=(T x)
    {
        std::string y;
        std::stringstream ss;
        ss << x;
        ss >> y;
        data = y;
        return *this;
    }
    // comparison operators
    bool operator==(const char* s) const { return data==s; }
    bool operator==(const std::string& s) const { return data==s; }
    bool operator==(const StringValue& x) const { return data==x.data; }
    template<class T> bool operator==(T x) const { return (x == T(*this)); }
    // conversion operators
    operator bool() const {
        if (data=="false" || data=="0") return false;
        return true;
    }
    operator float() const {
		return (float) atof(data.c_str());
    }
    operator double() const {
		return atof(data.c_str());
    }
    operator char() const {
        char x;
        std::stringstream ss; ss << data; ss >> x;
        return x;
    }
    operator unsigned char() const {
        unsigned char x;
        std::stringstream ss; ss << data; ss >> x;
        return x;
    }
    operator int() const {
		return atoi(data.c_str());
    }
    operator short() const {
		return (short) atoi(data.c_str());
    }
    operator long() const {
		return atol(data.c_str());
    }
    operator unsigned short() const {
        unsigned short x;
        std::stringstream ss; ss << data; ss >> x;
        return x;
    }
    operator unsigned int() const {
        unsigned int x;
        std::stringstream ss; ss << data; ss >> x;
        return x;
    }
    operator unsigned long() const {
        unsigned long x;
        std::stringstream ss; ss << data; ss >> x;
        return x;
    }
    operator const char *() const {
        return data.c_str();
    }

    std::string str() const { return data; }
private:
    std::string data;
};

#endif
