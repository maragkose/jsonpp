#ifndef JSON_HPP__
#define JSON_HPP__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <iomanip>
#include <memory>

namespace json {

const char        JSON_QUOTE             = '"';
const char        JSON_COLON             = ':';
const char        JSON_LEFTBRACKET       = '[';
const char        JSON_RIGHTBRACKET      = ']';
const char        JSON_LEFTBRACE         = '{';
const char        JSON_RIGHTBRACE        = '}';
const char        JSON_COMMA             = ',';
const std::string JSON_COMMA_STR         { JSON_COMMA };
const std::string JSON_QUOTE_STR         { JSON_QUOTE }; 
const std::string JSON_COLON_STR         { JSON_COLON };
const std::string JSON_LEFTBRACKET_STR   { JSON_LEFTBRACKET };
const std::string JSON_RIGHTBRACKET_STR  { JSON_RIGHTBRACKET };
const std::string JSON_LEFTBRACE_STR     { JSON_LEFTBRACE };
const std::string JSON_RIGHTBRACE_STR    { JSON_RIGHTBRACE };

//──────────────────────────────────────────────────────────────
enum class type : unsigned char {
//──────────────────────────────────────────────────────────────
    JNULL,
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    BOOL,
    JSON_ELEMENT,
    QUOTE,
    COLON,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    UNKNOWN
};
 
std::vector<char> JSON_ELEMENTS  { 
                      JSON_COLON, 
                      JSON_LEFTBRACKET, 
                      JSON_RIGHTBRACKET, 
                      JSON_LEFTBRACE, 
                      JSON_RIGHTBRACE, 
                      JSON_COMMA
};

std::vector<char> WHITE_SPACE { ' ', '\t', '\n' };



struct Array;
struct Object;
//──────────────────────────────────────────────────────────────
struct Json : public std::enable_shared_from_this<Json> {
//──────────────────────────────────────────────────────────────

    Json() {
    }   
    virtual ~Json() {
    }   
    Json & operator [] (std::string val) {
        key = val;
        return *this;
    }

    virtual std::vector<std::shared_ptr<Json>> & array() {
        throw(std::runtime_error("arr err"));
    }   
    
    virtual std::map<std::string, std::shared_ptr<Json>> & object() {
        throw(std::runtime_error("obj err"));
    }   
    
    virtual std::string str() { 
        return ""; 
    }   

    virtual int number() {
        throw(std::runtime_error("num err"));
    }   
    
    virtual bool boolean() {
        throw(std::runtime_error("bool err"));
    }   
    
    virtual void  set_type(type t) {
        m_type = t;
    }   
    virtual type get_type() {
        return m_type;
    }   
   
    virtual std::string quote(std::string val) {
        return '"' + val + '"';
    }   
    
    
    virtual std::string to_json() {
        throw(std::runtime_error("to_json err"));
    }   
    
    virtual std::shared_ptr<Array> as_array() {
        return std::static_pointer_cast<Array>(shared_from_this());
    }

    virtual std::shared_ptr<Object> as_object() {
        return std::static_pointer_cast<Object>(shared_from_this());
    }   
   
protected:
    
    std::string m_document;
    type        m_type;
    std::string key;
};

//──────────────────────────────────────────────────────────────
struct Bool : public Json {
//──────────────────────────────────────────────────────────────

    Bool(bool val = true) : m_value(val) {
        std::cout << "+++ new bool" << std::endl;
        m_type = type::BOOL;
    }

    virtual bool  boolean () {
        return m_value;
    }

    virtual std::string str() {
        return m_value==true ? "true" : "false";
    }

    virtual std::string to_json() {
        std::string rt;
        m_value ? rt="true" : rt="false";
        return rt;
    }

    bool m_value;
};

//──────────────────────────────────────────────────────────────
struct Number : public Json {
//──────────────────────────────────────────────────────────────

    Number(int val = 0) : m_value(val) {
        std::cout << "+++ new number" << std::endl;
    }

    virtual int  number () {
        return m_value;
    }

    virtual std::string str() {
        return std::to_string(m_value);
    }

    virtual std::string to_json() {
        return std::to_string(m_value);
    }

    int m_value;
};
//──────────────────────────────────────────────────────────────
struct String : public Json {
//──────────────────────────────────────────────────────────────

    String(const std::string& val = "") : m_value(val) {
        m_type = type::STRING;
        std::cout << "+++ new string" << std::endl;
    }

    virtual std::string str () {
        return m_value;
    }

    virtual std::string to_json() {
        return quote(m_value);
    }

    std::string m_value;
};

// Forward declare Object since it is needed in Array and Object needs Array  
struct Object;

//──────────────────────────────────────────────────────────────
struct Array : public Json {
//──────────────────────────────────────────────────────────────

    inline Array();
    inline Array(const Array &rhs);

    using iterator = std::vector<std::shared_ptr<Json>>::iterator;

    inline void push_back(std::string val);
    inline void push_back(bool val);
    inline void push_back(int val);
    inline void push_back(Object& val);
    inline void push_back(std::shared_ptr<Json> val);

    inline iterator begin() { return m_value.begin();}
    inline iterator end()   { return m_value.end(); }

    inline virtual std::vector<std::shared_ptr<Json>> & array ();
    inline virtual std::string to_json();

    std::vector<std::shared_ptr<Json>> m_value;
};
//──────────────────────────────────────────────────────────────
struct Object : public Json {
//──────────────────────────────────────────────────────────────

    inline Object();
    inline Object(Object &rhs);
    
    using iterator = std::map<std::string, std::shared_ptr<Json>>::iterator;

    template <typename T>
    void operator= (T && val) {
        this->m_value[key] = std::make_shared<String>(std::forward<T>(val));

    }
    //inline void operator= (std::string val);
    inline void operator= (bool val);
    inline void operator= (int val);
    inline void operator= (Object & val);
    inline void operator= (Array & val);

    inline virtual std::map<std::string, std::shared_ptr<Json>> & object ();
    inline virtual std::string to_json();
    virtual void   from_json(const std::string j);

    std::string str(const std::string &key) {
        return m_value[key]->str();
    }
    bool boolean(const std::string &key) {
        return m_value[key]->boolean();
    }
    int number(const std::string &key) {
        return m_value[key]->number();
    }

    iterator begin() { return m_value.begin();}
    iterator end() { return m_value.end(); }
    std::map<std::string, std::shared_ptr<Json>> m_value;

};

//──────────────────────────────────────────────────────────────
// --ARRAY-- INLINE FUNCTIONS IMPLEMENTATION
//──────────────────────────────────────────────────────────────

//-------------------------------------------------------------
Array::Array()  {
//-------------------------------------------------------------
    m_type = type::ARRAY;
    std::cout << "+++ new array" << std::endl;
}

//-------------------------------------------------------------
Array::Array(const Array &rhs) : m_value(rhs.m_value){
//-------------------------------------------------------------
}

//-------------------------------------------------------------
void Array::push_back(std::string val) {
//-------------------------------------------------------------
    m_value.push_back(std::make_shared<String>(val));
}

//-------------------------------------------------------------
void Array::push_back(bool val) {
//-------------------------------------------------------------
    m_value.push_back(std::make_shared<Bool>(val));
}

//-------------------------------------------------------------
void Array::push_back(int val) {
//-------------------------------------------------------------
    m_value.push_back(std::make_shared<Number>(val));
}

//-------------------------------------------------------------
void Array::push_back(Object& val) {
//-------------------------------------------------------------
    m_value.push_back(std::make_shared<Object>(val));
}
//-------------------------------------------------------------
void Array::push_back(std::shared_ptr <Json> val) {
//-------------------------------------------------------------
    m_value.push_back(val);
}

//-------------------------------------------------------------
std::vector<std::shared_ptr<Json>> & Array::array () {
//-------------------------------------------------------------
    return m_value;
}

//-------------------------------------------------------------
std::string Array::to_json() {
//-------------------------------------------------------------
    std::string json_string {};
    json_string.append("[");
    for (auto iter = m_value.begin(); iter != m_value.end(); iter++){
        json_string.append((*iter)->to_json());
        if(std::next(iter) != m_value.end()) {
            json_string.append(",");
        }
    }
    json_string.append("]");
    return json_string;
};

//──────────────────────────────────────────────────────────────
// --OBJECT-- INLINE FUNCTIONS IMPLEMENTATION
//──────────────────────────────────────────────────────────────

//-------------------------------------------------------------
Object::Object() {
//-------------------------------------------------------------
    m_type = type::OBJECT;
    std::cout << "+++ new object" << std::endl;
}

//-------------------------------------------------------------
Object::Object(Object &rhs) : m_value(rhs.m_value){
//-------------------------------------------------------------
}


//-------------------------------------------------------------
//void Object::operator= (std::string && val) {
//-------------------------------------------------------------
//    std::cerr<< "YEAP";
//    this->m_value[key] = std::make_shared<String>(std::forward<std::string>(val));
//}

//-------------------------------------------------------------
void Object::operator= (bool val) {
//-------------------------------------------------------------
    this->m_value[key] = std::make_shared<Bool>(val);
}

//-------------------------------------------------------------
void Object::operator= (int val) {
//-------------------------------------------------------------
    this->m_value[key] = std::make_shared<Number>(val);
}

//-------------------------------------------------------------
void Object::operator= (Object & val) {
//-------------------------------------------------------------
    this->m_value[key] = std::make_shared<Object>(val);
}

//-------------------------------------------------------------
void Object::operator= (Array & val) {
//-------------------------------------------------------------
    this->m_value[key] = std::make_shared<Array>(val);
}

//-------------------------------------------------------------
std::map<std::string, std::shared_ptr<Json>> & Object::object () {
//-------------------------------------------------------------
    return m_value;
}

//-------------------------------------------------------------
std::string Object::to_json() {
//-------------------------------------------------------------
    m_document.append("{");

    for (auto iter = m_value.begin(); iter != m_value.end(); iter++){
        m_document.append(quote(iter->first));
        m_document.append(":");
        m_document.append(iter->second->to_json());
        if(std::next(iter) != m_value.end())
            m_document.append(",");
    }
    m_document.append("}");
    return m_document;
}

//-------------------------------------------------------------
void Object::from_json(const std::string j) {
//-------------------------------------------------------------


}

//──────────────────────────────────────────────────────────────
class Parser {
//──────────────────────────────────────────────────────────────

public:

    void tokenize();
    std::shared_ptr<Json> from_json(const std::string & filename);
    void debug();

private:
    
    std::shared_ptr<Json> parse();
    std::shared_ptr<Json> parse_object();
    std::shared_ptr<Json> parse_array();
    
    std::string read_string();
    std::string read_number();
    std::string read_bool();
    std::string read_null();

    void consume(unsigned int length);
    void consume_whitespace();
    void consume_token();
    std::pair<std::string, type>  front_token();
    bool is_json_element();
    type what_type(const std::string &key);
    type find_type(const std::string &token);
    std::string show_type(type value);
    inline bool to_bool(const std::string &str);

    std::ifstream              _file;
    std::string                _contents;
    std::vector<std::string>   _tokens;
    std::vector<std::pair<std::string, type>>   _tokens_map;
    bool is_root = false; 
    std::unordered_map<std::string, std::string>         _root;
};

//--------------------------------------------------------------
std::shared_ptr<Json> Parser::from_json(const std::string &filename) {
//--------------------------------------------------------------

    std::ifstream file(filename);
    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    _contents = contents;

    tokenize();
    
    auto root = parse();
    return root;
    //return std::static_pointer_cast<Object>(root);
}

//--------------------------------------------------------------
void Parser::tokenize() {
//--------------------------------------------------------------
    std::string lex_string {};

    try {
        while(_contents.size()) {
            std::cout << "contents " << std::quoted(_contents, '|') << std::endl;
            
            lex_string = read_string();
            if(!lex_string.empty()) {
                _tokens.push_back(lex_string);
                _tokens_map.push_back(std::make_pair(lex_string,type::STRING));
                std::cout << "string "<<lex_string << std::endl;
                continue;
            }
            
            lex_string = read_number();
            if(!lex_string.empty()) {
                _tokens.push_back(lex_string);
                _tokens_map.push_back(std::make_pair(lex_string,type::NUMBER));
                std::cout << "number "<<lex_string;
                continue;
            }
            
            lex_string = read_bool();
            if(!lex_string.empty()) {
                _tokens_map.push_back(std::make_pair(lex_string,type::BOOL));
                _tokens.push_back(lex_string);
                continue;
            }
            
            lex_string = read_null();
            if(!lex_string.empty()) {
                _tokens_map.push_back(std::make_pair(lex_string,type::JNULL));
                _tokens.push_back(lex_string);
                continue;
            }

            consume_whitespace();

            if(is_json_element()) {
                std::string element{};
                element.push_back(_contents.at(0));
                _tokens.push_back(element);
                auto t = find_type(element);
                _tokens_map.push_back(std::make_pair(element,t));
                std::cout << "json element:"<<element<<std::endl;
                consume(1);
            }
        }
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range exception caught! Aborting..." << std::endl;
    }
}

//--------------------------------------------------------------
std::shared_ptr<Json> Parser::parse() {
//--------------------------------------------------------------
    auto token = front_token();

    if(is_root and token.second != type::LEFT_BRACKET) {
        throw(std::runtime_error("json parsing error. Expected a {."));
    }
   
    consume_token(); 

    if(token.second == type::LEFT_BRACKET) {
        parse_array();
    } else if(token.second == type::LEFT_BRACE) {
        return parse_object();
    } else if(token.second == type::NUMBER) {
        auto num = std::stoi(token.first.c_str());
        return std::make_shared<Number>(num);
    } else if(token.second == type::BOOL) {
        auto res = to_bool(token.first);
        return std::make_shared<Bool>(res);
    } else {
        return std::make_shared<String>(token.first);

    }
    return std::make_shared<Json>();
}

//--------------------------------------------------------------
std::shared_ptr<Json> Parser::parse_object() {
//--------------------------------------------------------------
    
    auto json_object = std::make_shared<Object>();
    auto token = front_token();

    if(token.second == type::RIGHT_BRACE) { 
        return json_object;
        //std::make_shared<Object>();
    }

    while(true) {

        auto json_key = front_token();

        if(json_key.second == type::STRING) {
            consume_token();
        } else {
            throw(std::runtime_error("json parsing error. Expected string"));
        }

        auto json_next = front_token();

        if(json_next.second != type::COLON) {
            throw(std::runtime_error("json parsing error. Expected (:)"));
        }

        consume_token();
        auto json_value = parse();
        json_object->m_value[json_key.first] = json_value;

        auto token = front_token();

        if(token.second == type::RIGHT_BRACE) {
            consume_token();
            return json_object;
        } else if(token.second != type::COMMA) {
            throw(std::runtime_error("json parsing error. Expected (,)"));
        }

        consume_token();
    } //end  while
    throw(std::runtime_error("json parsing error. Expected end of object (})"));
}

//--------------------------------------------------------------
std::shared_ptr<Json> Parser::parse_array() {
//--------------------------------------------------------------

    auto json_array = std::make_shared<Array>();
    auto token = front_token();
    
    if(token.second == type::RIGHT_BRACKET){
        consume_token();
        return json_array;
    }

    while(true) {
        auto json = parse();
        json_array->push_back(json);
        
        auto token = front_token();
        if(token.second == type::RIGHT_BRACKET) {
            consume_token();
            return json_array;
        } else if (token.second != type::COMMA) {
            throw(std::runtime_error("json parsing error. Expected (,) in array"));
        } else {
            consume_token();
        } 
    }
    throw(std::runtime_error("json parsing error. Expected (])"));
}

//--------------------------------------------------------------
void Parser::debug() {
//--------------------------------------------------------------
    std::cout << "=====TOKENS====="<<std::endl;
        for (auto &token: _tokens) {
        std::cout << "+ " <<  std::quoted(token) << std::endl;
    };
    std::cout << "=====MAP====="<<std::endl;
        for (auto &token: _tokens_map) {
        std::cout << "+ " <<  std::quoted(token.first) << " is " << show_type(token.second) << std::endl;
    };
}

//--------------------------------------------------------------
std::string Parser::read_string() {
//--------------------------------------------------------------

    std::string json_str;

    if(_contents.at(0) == JSON_QUOTE) {
        consume(1);
    } else {
        return {};
    }

    for(auto c: _contents) {
        if(c == JSON_QUOTE) {
            consume(json_str.size() + 1);
            return json_str;  
        } else {
            json_str += c;
        }
    }
    return {};
}

//--------------------------------------------------------------
std::string Parser::read_number() {
//--------------------------------------------------------------

    std::string json_num {};

    for(auto c: _contents) {
        if(std::isdigit(c)) {
            json_num+=c;
        } else {
            break;
        }
    }

    if(json_num.empty()) {
        return {};
    } else {
        consume(json_num.size());
        return json_num;
    }
   
    return {}; 
}

//--------------------------------------------------------------
std::string Parser::read_bool() {
//--------------------------------------------------------------
    return {};
}
//--------------------------------------------------------------
std::string Parser::read_null() {
//--------------------------------------------------------------
    return {};
}


//--------------------------------------------------------------
void Parser::consume(unsigned int length) {
//--------------------------------------------------------------
    if(_contents.empty()) {
        return;
    }
    _contents.erase(0, length);
}

//--------------------------------------------------------------
void Parser::consume_whitespace() {
//--------------------------------------------------------------
    if(_contents.empty()) {
        return;
    }
    if(std::find(WHITE_SPACE.begin(), WHITE_SPACE.end(), _contents.at(0)) != std::end(WHITE_SPACE)){
        _contents.erase(0,1);
    }
}
    
//--------------------------------------------------------------
void Parser::consume_token() {
//--------------------------------------------------------------
    _tokens_map.erase(_tokens_map.begin());
}

//--------------------------------------------------------------
std::pair<std::string, type> Parser::front_token() {
//--------------------------------------------------------------
    std::cout << "Parsing " << std::quoted(_tokens_map[0].first, '|') <<  " which is " << show_type(_tokens_map[0].second) << std::endl;
    return _tokens_map[0];
}

//--------------------------------------------------------------
type Parser::what_type(const std::string &key) {
//--------------------------------------------------------------
    for(auto &item: _tokens_map) {
        if(item.first == key) return item.second;
    }
    return type::UNKNOWN;
}
//--------------------------------------------------------------
type Parser::find_type(const std::string &token) {
//--------------------------------------------------------------
    if(token == JSON_COMMA_STR) return type::COMMA; 
    if(token == JSON_QUOTE_STR) return type::QUOTE;
    if(token == JSON_COLON_STR) return type::COLON;
    if(token == JSON_LEFTBRACKET_STR) return type::LEFT_BRACKET;
    if(token == JSON_RIGHTBRACKET_STR) return type::RIGHT_BRACKET;
    if(token == JSON_LEFTBRACE_STR) return type::LEFT_BRACE;
    if(token == JSON_RIGHTBRACE_STR) return type::RIGHT_BRACE;
        
    return type::UNKNOWN;
}

//--------------------------------------------------------------
bool Parser::is_json_element() {
//--------------------------------------------------------------
    if(_contents.empty()) {
        return false;
    }
    return (std::find(JSON_ELEMENTS.begin(), JSON_ELEMENTS.end(), _contents.at(0)) != std::end(JSON_ELEMENTS));
}

//--------------------------------------------------------------
std::string Parser::show_type(type value) {
//--------------------------------------------------------------

    switch(value) {
        case type::QUOTE:
        case type::COLON:
        case type::LEFT_BRACKET:
        case type::RIGHT_BRACKET:
        case type::LEFT_BRACE:
        case type::RIGHT_BRACE:
        case type::COMMA:
        case type::JSON_ELEMENT: return "json syntax"; break;
        case type::STRING:       return "string"; break;
        case type::NUMBER:       return "number"; break;
        case type::BOOL:         return "boolean"; break;
        case type::JNULL:        return "null"; break;
    };
    return "Unknown";
}

//----------------------------------
inline bool Parser::to_bool(const std::string &str) {
//----------------------------------
    if (str == "true" || str == "1" || str == "True" || str == "TRUE") {
        return true;
    } else if (str ==  "false" || str == "False" || str == "FALSE" || str == "0") {
        return false;
    } else {
        return false;
    }
}   

// how to use it:
//StringList list;
//int var = list.get<int>(0);
//float var2 = list[0];

}//end namespace json

#endif
