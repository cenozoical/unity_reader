//
// Created by vuk on 10/26/25.
//
#include "scenes_handler.h"
#include "utility.h"
#include <fstream>
#include <thread>
#include <iostream>

#include "scripts_handler.h"

scenesHandler &scenesHandler::getSceneHandler() {
    static scenesHandler handler;
    return handler;
}


template<typename T>
void scenesHandler::async_deque<T>::push_back(T &el) {
    lock.lock();
    list.push_back(el);
    lock.unlock();
}
template<typename T>
deque<T> &scenesHandler::async_deque<T>::getList() {
    return list;
}


void scenesHandler::scene::readScene()
{
    std::ifstream file_stream(input_path);
    std::string line;
    long fileID;
    entry_type type;
    // Read the file line by line
    while (std::getline(file_stream, line)) {
        auto line_tokens = utilities::splitString(line, " ");
        if (line_tokens[0] == "---") {
            auto type_str = utilities::splitString(line_tokens[1], "!")[1];
            {
                auto fileID_str = utilities::splitString(line_tokens[2], "&")[0];
                fileID = std::stol(fileID_str, nullptr, 10);
            }
            if (type_str == "1") {
                type = entry_type::GAME_OBJECT;
            }
            else if (type_str == "4") {
                type = entry_type::TRANSFORM;
                unity_element & transform = objects[fileID];
                transform.TransformTrue();
                //if not marked as someone's child already, add it to roots
                if (transform.isRoot()) roots.insert(fileID);
            }
            else if (type_str == "114") {
                type = entry_type::MONO_BEHAVIOUR;
            }
            else type = entry_type::ELSE;
        }
        else if (type == entry_type::GAME_OBJECT && line_tokens[0] == "m_Name:")
            objects[fileID].setName(line.substr(10, line.length()-10));
        else if (type == entry_type::GAME_OBJECT && line_tokens[0] == "-" && line_tokens[1] == "component:") {
            line_tokens.back().pop_back();//remove '}'
            long componentID = std::stol(line_tokens.back(), nullptr, 10);
            unity_element &component = objects[componentID];
            component.setTransformParent(fileID);
        }
        else if (type == entry_type::TRANSFORM && line_tokens[0] == "-") {
            unity_element &transform = objects[fileID];
            line_tokens.back().pop_back();//remove '}'
            long childID = std::stol(line_tokens.back(), nullptr, 10);
            transform.addChild(childID);
            unity_element &child_ref = objects[childID];
            child_ref.TransformTrue();
            child_ref.rootFalse();
            if (roots.contains(childID)) roots.erase(childID);
        }
        else if (type == entry_type::MONO_BEHAVIOUR && line_tokens[0] == "m_Script:") {
            line_tokens[4].pop_back();//remove comma
            getSceneHandler().used_script_guids.push_back(line_tokens[4]);
        }
    }
    file_stream.close();
}

void scenesHandler::scene::printScene() {
    std::ofstream output(getSceneHandler().output_directory/output_filename);
    if (!output)throw runtime_error("File not created");
    for (long root:roots) {
        deque<long> objectIndex_stack;
        deque<int> depth_stack;
        depth_stack.push_back(0);
        objectIndex_stack.push_back(root);
        while (!objectIndex_stack.empty()) {
            auto & transform = objects[objectIndex_stack.back()];
            auto &gameObject = objects[transform.getTransformParent()];
            int depth = depth_stack.back();
            objectIndex_stack.pop_back();
            depth_stack.pop_back();
            for (int i = 0; i < depth;i++) output << "--";
            output << gameObject.getName() << endl;
            output.flush();
            for (long child: transform.getChildren()) {
                objectIndex_stack.push_back(child);
                depth_stack.push_back(depth + 1 );
            }
        }
    }
    output.flush();
    output.close();
}


void scenesHandler::processScenes(const std::filesystem::path& input_directory,std::filesystem::path& output_directory) {
    scenesHandler::output_directory = output_directory;
    deque<thread *> scene_readers;
    deque <thread *> scene_writers;

    /*
     *In scene reader lambda thread, capture by value option is specified, so a pointer to
     *scene_writers is used to reference it there.
    */
    deque <thread *> * scene_writers_pointer = &scene_writers;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(input_directory))
        if (entry.is_regular_file() && entry.path().extension() == ".unity") {
            scene current_scene;
            current_scene.output_filename = entry.path().filename().string().append(".dump");
            current_scene.input_path = entry.path();
            thread *scene_reader = new thread([=]() mutable {
                current_scene.readScene();

                thread *scene_writer = new thread([=]() mutable {
                    current_scene.printScene();
                });

                scene_writers_pointer->push_back(scene_writer);
       });
            scene_readers.push_back(scene_reader);

        }
    for (thread *t:scene_readers) {
        t->join();
        delete t;
    }
    scriptsHandler::getScriptsHandler().signalScenesRead();
    for (thread *t:scene_writers) {
        t->join();
        delete t;
    }

}



deque<string> &scenesHandler::getUsedScriptGuids() {
    return used_script_guids.getList();
}

