/*   
     Файл:    table-gen-for-main-parser.cpp
     Создано: 30 января 2016г.
     Автор:   Гаврилов Владимир Сергеевич
     E-mails: vladimir.s.gavrilov@gmail.com
              gavrilov.vladimir.s@mail.ru
              gavvs1977@yandex.ru
*/
#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <iomanip>
#include "char_conv.h"

enum Category : uint8_t {
    Spaces,        Other,       Percent,
    After_percent, Id_begin,    Id_body,
    Delimiters,    Double_quote
};

const char32_t* after_percent_chars = U"acdhiklmnst" ;
const char32_t* id_begin_chars = 
    U"_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char32_t* id_body_chars = 
    U"_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const char32_t* delimiters_chars = U",:{}";

std::map<char32_t, uint8_t> table;

void insert_char(const char32_t ch, Category category){
    auto it = table.find(ch);
    if(it != table.end()){
        table[ch] |= 1U << category; 
    }else{
        table[ch] =  1U << category;
    }
}

void add_category(const char32_t* p, Category category){
    while(char32_t ch = *p++){
        insert_char(ch, category);
    }
}

std::u32string spaces_str(){
    std::u32string s;
    for(char32_t c = 1; c <= ' '; c++){
        s += c;
    }
    return s;
}

void fill_table(){
    std::u32string s = spaces_str();
    
    add_category(s.c_str(), Spaces);
    add_category(after_percent_chars, After_percent);
    add_category(id_begin_chars, Id_begin);
    add_category(id_body_chars, Id_body);
    add_category(delimiters_chars, Delimiters);
    add_category(U"%", Percent);
    add_category(U"\"", Double_quote);
}

std::string show_table_elem(const std::pair<char32_t, uint8_t> e){
    std::ostringstream oss;
    oss << "{";
    auto c = e.first;
    if(c <= ' '){
        oss << std::setw(4) << static_cast<uint16_t>(c);
    }else{
        oss << "U'" << char32_to_utf8(e.first) << "'";
    }
    oss <<  ", " << std::setw(3) << static_cast<uint16_t>(e.second) << "}";
    return oss.str();
}

std::string show_table(){
    std::string s = R"~(enum Category : uint8_t {
    Spaces,        Other,       Percent,
    After_percent, Id_begin,    Id_body,
    Delimiters,    Double_quote
};

std::map<char32_t, uint8_t> categories_table = {
)~";
    #define ELEMS_IN_GROUP 5
    size_t num_of_elems   = table.size();
    size_t num_of_triples = num_of_elems / ELEMS_IN_GROUP;
    size_t rem            = num_of_elems % ELEMS_IN_GROUP;
    auto it = table.begin();
    for(size_t i = 0; i < num_of_triples; i++){
        s += "    ";
        for(int j = 0; j < ELEMS_IN_GROUP; j++, it++){
            s += show_table_elem(*it) + ",  ";
        }
        s += "\n";
    }
    s += "    ";
    for(unsigned j = 0; j < rem; j++, it++){
        s += show_table_elem(*it) + ",  ";
    }
    s.pop_back();
    s.pop_back();
    s += "\n};";
    return s;
}
void print(){
    std::string s = show_table();
    printf("%s\n", s.c_str());
}

int main(void) {
    fill_table();
    print();
    return 0;
}
