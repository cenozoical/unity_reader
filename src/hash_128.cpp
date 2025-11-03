//
// Created by vuk on 10/26/25.
//

#include "hash_128.h"

#include <assert.h>
#include <iostream>
#include <utility>

std::size_t hash_128::ArrayHash::operator()(const std::array<uint8_t, 16> &arr) const {

    std::size_t h = 0;
    // Combine the hashes of individual bytes.
    // The algorithm for combining hashes is important for distribution.
    for (const unsigned char byte : arr) h ^= std::hash<unsigned char>{}(byte) + 0x9e3779b9 + (h << 6) + (h >> 2);

    return h;
}
uint8_t hash_128::readHexChar(const char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return c - 'A' + 10;

}

std::array<uint8_t,16> * hash_128::guid_to_arr(const std::string &guid){
    assert(guid.size() == 32);
    const auto ret = new std::array<uint8_t,16> ();
    for (int i = 0; i < 16; i++) {
        uint8_t el = 0;
        el = readHexChar(guid[2*i]);
        el <<= 4;
        el |= readHexChar(guid[2*i+1]);
        (*ret)[i] = el;
    }
    return ret;

}
void hash_128::add_element(const string &s, const string &name) {
    const array<uint8_t,16> *arr = hash_128::guid_to_arr(s);
    scriptMeta el ;
    {
        el.name = name;
        el.guid = s;
    }
    lock.lock();
    mapping[*arr] = el;
    lock.unlock();
    delete arr;
}
bool hash_128::element_mapped(string &s) {
    bool result = false;
    const array<uint8_t,16> *arr = hash_128::guid_to_arr(s);
    if (mapping.find(*arr) != mapping.end()) result = true;
    delete arr;
    return true;
}
hash_128::scriptMeta hash_128::retrieve(string &s) {
    scriptMeta el;
    const array<uint8_t,16> *arr = hash_128::guid_to_arr(s);
     if (mapping.find(*arr) != mapping.end()) el = mapping[*arr];
    delete arr;
    return el;
}
void hash_128::printsNames() const {
    for (const auto& el: mapping) {
        std::cout << el.second.name<< std::endl;
    }
}
void hash_128::removeScript(const string &s) {
    const array<uint8_t,16> *arr = hash_128::guid_to_arr(s);
    if (mapping.find(*arr)!= mapping.end()) mapping.erase(mapping.find(*arr));
    delete arr;
}
deque<hash_128::scriptMeta> hash_128::getMapping() const {
    deque<scriptMeta> result;
    for (const auto& el:mapping)
        result.push_back(el.second);
    return result;
}





