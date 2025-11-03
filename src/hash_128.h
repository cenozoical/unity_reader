//
// Created by vuk on 10/26/25.
//

#ifndef UNITY_READER_HASH_128_H
#define UNITY_READER_HASH_128_H

#include <array>
#include <string>
#include <unordered_map>
#include <deque>
#include <mutex>
using namespace std;
//used for mapping 32 hex character guid strings to script names
class hash_128 {

    struct ArrayHash {
        std::size_t operator()(const std::array<uint8_t, 16>& arr) const ;
    };

    static std::array<uint8_t,16> *guid_to_arr(const string &);

    static uint8_t readHexChar(char);
    mutex lock;//only used when script_reader threads map the script guids/names

public:
        struct scriptMeta {
            string name, guid;
        };
        void printsNames() const;
        void removeScript(const string&);
        void add_element(const string & guid, const  string & name);
        bool element_mapped(string &);
        scriptMeta retrieve(string&);
        deque<scriptMeta> getMapping() const;
private :
    unordered_map<array<uint8_t, 16>, scriptMeta, ArrayHash> mapping;

};


#endif //UNITY_READER_HASH_128_H