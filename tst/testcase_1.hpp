#ifndef TESTCASE_1
#define TESTCASE_1

TEST_CASE("create_simple_json" , "[json]") {
   
    json::Object doc;
    json::Object doc2;
    json::Array arr;
    arr.push_back(4);
    arr.push_back(5);
    arr.push_back(6);

    std::string w = "value1";
    doc["str1"] = w;
    doc["bool1"] = true;
    
    doc2["str2"] = "value2";
    
    doc["obj1"] = doc2;
    doc["obj2"] = arr;

    std::string str = R"({"bool1":true,"obj1":{"str2":"value2"},"obj2":[4,5,6],"str1":"value1"})";
    REQUIRE( str == doc.to_json() );
}

TEST_CASE("create_nested_json" , "[json]") {
    
    json::Object root;
    json::Object doc1;
    json::Object doc2;
    json::Array arr;

    doc1["str1"] = "val1";
    doc2["str2"] = "val2";
    arr.push_back(doc1);
    arr.push_back(doc2);

    root["doc"] = arr; 

    std::string str = R"({"doc":[{"str1":"val1"},{"str2":"val2"}]})";
    REQUIRE( str == root.to_json() );
}

TEST_CASE("parse_simple_json" , "[json]") {

    std::string str = R"({"Hello":"there"})";
    auto root = json::parse(str);
    REQUIRE(root->to_json() == str);    
}

TEST_CASE("parse_bool_json" , "[json]") {

    std::string str = R"({"Hello":true})";
    auto root = json::parse(str);
    REQUIRE(root->to_json() == str);    
}

TEST_CASE("parse_number_json" , "[json]") {

    std::string str = R"({"Hello":4})";
    auto root = json::parse(str);
    REQUIRE(root->to_json() == str);    
}

TEST_CASE("parse_array_json" , "[json]") {
    
    std::string str = R"({"bool1":true,"obj1":{"str2":true},"obj2":[{"a":4},{"b":5}],"str1":"value1"})";
    auto root = json::parse(str);
    REQUIRE(root->to_json() == str);    
}
TEST_CASE("parse_value_array_of_strings_json" , "[json]") {
    
    std::string jj = R"({"arr1":["hello","world"],"bool1":true})";
    auto root = json::parse(jj);
    REQUIRE(root->to_json() == jj);    
}

TEST_CASE("parse_complex_json" , "[json]") {
    
    std::string str = R"({"glossary":{"GlossDiv":{"GlossList":{"GlossEntry":{"Abbrev":"ISO8879:1986","Acronym":"SGML","GlossDef":{"GlossSeeAlso":["GML","XML"],"para":"A meta-markup language, used to create markup languages such as DocBook."},"GlossSee":"markup","GlossTerm":"Standard Generalized  Markup Language","ID":"SGML","SortAs":"SGML"}},"title":"S"},"title":"example glossary"}})";
    auto root = json::parse(str);
    REQUIRE(root->to_json() == str);    
}

TEST_CASE("iterate_json" , "[json]") {
    
    json::Object root;
    std::string w1="w1";
    std::string w2="w2";
    std::string w3="w3";
    std::string w4="w4";
    root["hello1"] = w1;
    root["hello2"] = w2;
    root["hello3"] = w3;
    root["hello4"] = "w4";
    root["bbbb"] = true;
    root["nn"] = 222;

    for(auto & item: root) {
        std::cerr << item.first << ":" << item.second->str() << std::endl;
    }

    REQUIRE(root["hello4"].str() == w4);
}
#endif
