#include "json.hpp"

int main(int argc, char **argv) {

    json::Parser parser;
    auto root = parser.from_json("./test.json");
    std::cout << "-----------------DEBUG-----------------" << std::endl;
    parser.debug();
    std::cout << "---------------------------------------" << std::endl;

    auto obj = root->as_object();;
    
    std::cerr << obj->str("name_1") << std::endl;
    std::cerr << obj->number("id_1") << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    for(auto & item: *obj) {
//        if(item.second->get_type() == json::type::ARRAY) {
//            std::cerr <<"FOUND ARRAY"<< std::endl;
//            for(auto &ttt: *(item.second->as_array())) {
//                std::cerr<< ttt->as_array()->number() << std::endl;
//            }
//        }
//        if(item.second->get_type() == json::type::OBJECT) {
//                std::cerr<< item.second->as_object()->str() << std::endl;
//        }
        std::cerr << item.first << ":" << item.second->str() << std::endl;
//        std::cerr << item.first << std::endl;
    } 
}

