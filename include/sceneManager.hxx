#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class core;
class sceneManager{
    private:
        core* engineCore;
        int currentSceneIndex;
        std::vector<std::string> sceneName,bgConfigFile,levelConfigFile;
    
    public:
        sceneManager(core*);
        void getScenes(const std::string&);
        void loadScene(int);
        void loadNextScene();
        int getCurrentSceneIndex();
};