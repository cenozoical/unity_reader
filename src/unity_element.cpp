//
// Created by vuk on 10/27/25.
//

#include "unity_element.h"
void unity_element::addChild(long fileID) {
    children.push_back(fileID);
}

void unity_element::rootFalse() {
    root = false;
}
void unity_element::setTransformParent(long fileID) {
    transform_parent = fileID;

}
void unity_element::TransformTrue(){
    isTransform = true;
}

bool unity_element::isItTransform() const {
    return isTransform;
}
std::deque<long> & unity_element::getChildren() {
    return children;
}
bool unity_element::isRoot() const {
    return root;
}

long unity_element::getTransformParent() const {
    return transform_parent;
}
std::string& unity_element::getName() {
    return name;
}
void unity_element::setName(std::string&& name) {
    this->name = name;
}



