//
// Created by vuk on 10/27/25.
//

#ifndef UNITY_READER_SCRIPT_READER_H
#define UNITY_READER_SCRIPT_READER_H
#include <filesystem>
#include <mutex>
#include "hash_128.h"

class scriptsHandler {

    hash_128 scripts_map;
    std::filesystem::path scripts_directory;

    mutex scenesRead;
    void readSingleScript(const std::filesystem::path &input);
    scriptsHandler();
public:
    void readScripts(std::filesystem::path& directory);
    void printUnusedScripts(const std::filesystem::path& output_dir);
    void waitForSceneReaders();
    void signalScenesRead();

    static scriptsHandler& getScriptsHandler();
    scriptsHandler(const scriptsHandler&) = delete;
    scriptsHandler& operator=(const scriptsHandler&) = delete;
    scriptsHandler(scriptsHandler&&) = delete;
    scriptsHandler& operator=(scriptsHandler&&) = delete;
};
#endif //UNITY_READER_SCRIPT_READER_H