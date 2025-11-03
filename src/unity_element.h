//
// Created by vuk on 10/27/25.
//

#ifndef UNITY_READER_GAME_OBJECT_H
#define UNITY_READER_GAME_OBJECT_H

#include <deque>
#include <string>
/*
 *one unity_element instance can represent either GameObject or a GameObject component (e.g. transform component)
 */
class unity_element {
    bool root = true; //only applies for transform components instances which have no parents
    long transform_parent = 0;//if a unity_element instance represents transform component, this field holds GameObject fileID it applies to
    bool isTransform = false;//does unity_element instance represent transform component
    std::deque<long> children;//if unity_element instance represents transform component , this is a list of its children
    std::string name; //name of the GameObject
public:
    void addChild(long);
    void rootFalse();
    void setTransformParent(long);
    std::string& getName();
    void setName(std::string&&);
    bool isRoot() const;
    long getTransformParent() const;
    void TransformTrue();
    bool isItTransform() const;
    std::deque<long> &getChildren();
};


#endif //UNITY_READER_GAME_OBJECT_H