#include <filesystem>
#include <iostream>
#include <thread>

#include "scripts_handler.h"
#include "scenes_handler.h"
using namespace std;





int main(const int argc, const char * argv[]) {
    try {
        if (argc < 2 ) throw runtime_error("No unity project or output directory specified");
        if (argc != 3) throw runtime_error("No output directory specified");

        filesystem::path output_dir = string(argv[2]);
        filesystem::path project_dir = string(argv[1]);
        if (!exists(project_dir) || !is_directory(project_dir))
            throw runtime_error("Specified project directory does not exist");
        if (!exists(output_dir) || !is_directory(output_dir))
            throw runtime_error("Specified output directory does not exist");
        filesystem::path scripts = (project_dir/"Assets")/"Scripts";
        filesystem::path scenes = (project_dir/"Assets")/"Scenes";


        thread scripts_reader = thread([&]() {
           scriptsHandler::getScriptsHandler().readScripts(scripts);
            scriptsHandler::getScriptsHandler().waitForSceneReaders();
            scriptsHandler::getScriptsHandler().printUnusedScripts(output_dir);
       });
        thread scenes_reader = thread([&]() {
            scenesHandler::getSceneHandler().processScenes(scenes, output_dir);
        });



        scenes_reader.join();
        scripts_reader.join();


    }
    catch (runtime_error &err) {
    cerr<<err.what();
        exit(9);
    }
}