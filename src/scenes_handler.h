//
// Created by vuk on 10/27/25.
//

#ifndef UNITY_READER_SCENES_READER_H
#define UNITY_READER_SCENES_READER_H
#include  "hash_128.h"
#include <filesystem>
#include <deque>
#include <unordered_map>
#include <mutex>

#include <unordered_set>
#include "unity_element.h"
class scenesHandler {
    class scene {
        void readScene();
        void printScene();
        //map of game_objects and their components
        unordered_map<long, unity_element> objects;
        //set of transform components which have no parent
        unordered_set<long> roots;
    public:

        filesystem::path output_filename, input_path;
        friend class scenesHandler;
    };
    template <typename T>
    class async_deque  {
        deque<T> list;
        mutex lock;
        public:
        void push_back(T &el);
        deque<T>& getList();

    };
   async_deque<string> used_script_guids;
    enum class entry_type {
        GAME_OBJECT,
        TRANSFORM,
        MONO_BEHAVIOUR,
        ELSE

    };
    std::filesystem::path output_directory;

    scenesHandler() = default;
public:
    void processScenes(const std::filesystem::path& input_directory,std::filesystem::path&output_directory);
    deque<string> & getUsedScriptGuids();

    static scenesHandler& getSceneHandler();
    scenesHandler(const scenesHandler&) = delete;
    scenesHandler& operator=(const scenesHandler&) = delete;
    scenesHandler(scenesHandler&&) = delete;
    scenesHandler& operator=(scenesHandler&&) = delete;


};
#endif //UNITY_READER_SCENES_READER_H