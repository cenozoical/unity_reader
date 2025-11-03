//
// Created by vuk on 10/27/25.
//
#include <filesystem>
#include "scenes_handler.h"
#include "scripts_handler.h"
#include "utility.h"
#include "hash_128.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <deque>
#include <thread>



scriptsHandler &scriptsHandler::getScriptsHandler() {
    static scriptsHandler handler;
    return handler;
}
scriptsHandler::scriptsHandler() {
    scenesRead.lock();
}

void scriptsHandler::readSingleScript(const std::filesystem::path &input) {
    std::ifstream file_stream(input);
    std::string line, guid;
    bool representsScript = true;

    // Read the file line by line
    while (representsScript && std::getline(file_stream, line)) {
        auto line_tokens = utilities::splitString(line, " ");
        if (line_tokens[0] == "folderAsset:") representsScript = false;
        else if (line_tokens[0] == "guid:") guid = line_tokens[1];
    }
    if (representsScript) {
        filesystem::path target_path = "Assets/Scripts";
        target_path /= filesystem::relative(input,scripts_directory).replace_extension("");
        scripts_map.add_element( guid, target_path);
    }


    file_stream.close();
}

void scriptsHandler::readScripts(std::filesystem::path& directory) {
    scripts_directory = directory;
    deque <thread *> script_readers;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
        if (entry.is_regular_file() && entry.path().extension() == ".meta") {
            thread *script_reader = new thread([=]() mutable {
                readSingleScript(entry.path());
       });
            script_readers.push_back(script_reader);

        }
    for (thread * t: script_readers) {
        t->join();
        delete t;
    }

}

void scriptsHandler::printUnusedScripts(const std::filesystem::path &output_dir) {
    for (string &script_guid: scenesHandler::getSceneHandler().getUsedScriptGuids()) {
        scripts_map.removeScript(script_guid);
    }

    std::ofstream output(output_dir/"UnusedScripts.csv");
    output << "Relative path, guid" << endl;
    deque<hash_128::scriptMeta> unused_scripts = scripts_map.getMapping();
    for (hash_128::scriptMeta &script:unused_scripts)
        output << script.name << ", " << script.guid << endl;
    output.flush();
    output.close();
}
void scriptsHandler::waitForSceneReaders() {
    scenesRead.lock();
}
void scriptsHandler::signalScenesRead() {
    scenesRead.unlock();
}


