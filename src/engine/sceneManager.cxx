#include "sceneManager.hxx"
#include "fstream"
#include "core.hxx"
#include "iostream"
#include "json.hpp"
using json = nlohmann::json;

sceneManager::sceneManager(core* coreInstance) : engineCore(coreInstance), currentSceneIndex(0){}

void sceneManager::getScenes(const std::string& filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Failed to open scene config file: " << filePath << std::endl;
        return;
    }
    json data;
    file>>data;
    for(auto& i : data["scenes"]){
        sceneName.push_back(i["sceneName"]);
        bgConfigFile.push_back(i["bgConfig"]);
        levelConfigFile.push_back(i["levelConfig"]);
    }
}

void sceneManager::loadScene(int sceneIndex){
    if(sceneIndex<0||sceneIndex>=sceneName.size()){
        std::cout<<"Couldn't load level, invalid index\n";
        return;
    }
    currentSceneIndex=sceneIndex;
    engineCore->loadLevel(levelConfigFile[currentSceneIndex]);
    engineCore->addBackgroundLayersForParallax(bgConfigFile[currentSceneIndex]);
}

void sceneManager::loadNextScene(){
    if(currentSceneIndex<sceneName.size())  currentSceneIndex+=1;
    loadScene(currentSceneIndex);
}

int sceneManager::getCurrentSceneIndex(){
    return currentSceneIndex;
}
