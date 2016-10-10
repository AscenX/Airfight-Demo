//
//  PreLoadScene.cpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#include "PreLoadScene.hpp"
#include "SimpleAudioEngine.h"
#include "GameScene.hpp"
#include "Tools.hpp"

using namespace CocosDenshion;

Scene* PreLoad::createScene(){
    auto scene = Scene::create();
    auto layer = PreLoad::create();
    scene->addChild(layer);
    return scene;
}

bool PreLoad::init(){
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* bg1 = Sprite::create("bg5.png");
    bg1->setPosition(visibleSize.width/2, visibleSize.height/2);
    Tools::adapt(bg1, visibleSize);
    this->addChild(bg1, -1);
    
    auto label = Label::createWithSystemFont("正在加载中...", "Arial", 48);
    label->setPosition(visibleSize.width / 2, visibleSize.height * 0.7);
    this->addChild(label);
    
    //创建一个进度条
    auto barSprite = Sprite::create("loadingbar.png");
    _progress = ProgressTimer::create(barSprite);
    _progress->setPercentage(0.0f);
    _progress->setScale(0.5f);
    _progress->setMidpoint(Vec2(0.0f,0.5f));
    _progress->setBarChangeRate(Vec2(1.0f, 0.f));
    _progress->setType(ProgressTimer::Type::BAR);
    _progress->setPosition(visibleSize.width/2, visibleSize.height/2);
    Tools::adapt(_progress, Size(visibleSize.width - barSprite->getContentSize().width, 20));
    this->addChild(_progress);
    
    return true;
}

void PreLoad::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
    
    ValueMap map = FileUtils::getInstance()->getValueMapFromFile("preloadResources.plist");
    ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
    ValueVector effects = map.at("Sounds").asValueVector();
    ValueVector music = map.at("Musics").asValueVector();
    
    _sourceCount = spriteSheets.size() + effects.size() + music.size();
    
    _progressInterval = 100 / _sourceCount;
    
    loadMusic(music);
    loadEffect(effects);
    loadSpriteSheets(spriteSheets);
}

void PreLoad::loadMusic(ValueVector musicFiles){
    for(unsigned int i = 0;i != musicFiles.size();++i){
        Value value = musicFiles.at(i);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(value.asString().c_str());
        progressUpdate();
    }
}
void PreLoad::loadEffect(ValueVector effectFiles){
    for(unsigned int i = 0;i != effectFiles.size();++i){
        Value value = effectFiles.at(i);
        SimpleAudioEngine::getInstance()->preloadEffect(value.asString().c_str());
        progressUpdate();
    }
}

void PreLoad::loadSpriteSheets(ValueVector spriteSheets){
    for(unsigned int i = 0;i != spriteSheets.size();++i){
        Value value = spriteSheets.at(i);
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(value.asString().c_str());
        progressUpdate();
    }
}

void PreLoad::progressUpdate(){
    if(--_sourceCount){
        _progress->setPercentage(100.0f - (_progressInterval * _sourceCount));
    } else {
        auto pft = ProgressFromTo::create(0.5f, _progress->getPercentage(), 100);
        auto callFunc = CallFunc::create([=]{
            auto delay = DelayTime::create(2.0f);
            auto callFunc = CallFunc::create([]{
                Director::getInstance()->replaceScene(Game::createScene());
            });
            auto action = Sequence::create(delay, callFunc, NULL);
            this->runAction(action);
        });
        auto action = Sequence::create(pft, callFunc, NULL);
        _progress->runAction(action);
    }
}

