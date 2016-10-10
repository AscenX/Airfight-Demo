//
//  AudioControlScene.cpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#include "AudioControlScene.hpp"
#include "ui/cocosGUI.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.hpp"
#include "Tools.hpp"


using namespace ui;
using namespace CocosDenshion;

bool AudioControl::init(){
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* bg1 = Sprite::create("bg5.png");
    bg1->setPosition(visibleSize.width/2, visibleSize.height/2);
    Tools::adapt(bg1, visibleSize);
    this->addChild(bg1, -1);
    
    //背景音乐
    auto musicText = Text::create("背景音乐", "Arial", 48);
    musicText->::AudioControl::setPosition(visibleSize.width * 0.25, visibleSize.height * 0.7);
    this->addChild(musicText);
    
    auto musicSlider = Slider::create();
    musicSlider->loadBarTexture("sliderTrack.png");
    musicSlider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", " ");
    musicSlider->loadProgressBarTexture("sliderProgress.png");
    float musicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");
    if(musicPercent == 0.0f)
        musicPercent = 100.0f;
    
    //设置背景音乐滑动条的初始值
    musicSlider->setPercent(musicPercent);
    musicSlider->::AudioControl::setPosition(visibleSize.width * 0.6, visibleSize.height * 0.7);
    musicSlider->addEventListener([=](Ref* pSender, Slider::EventType type){
        if(type == Slider::EventType::ON_PERCENTAGE_CHANGED){
            int percent = musicSlider->getPercent();
            SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(percent) / 100);
            UserDefault::getInstance()->setFloatForKey("musicPercent", percent);
        }
    });
    this->addChild(musicSlider);
    
    
    //音效
    auto soundText = Text::create("音效音量", "Arial", 48);
    soundText->::AudioControl::setPosition(visibleSize.width * 0.25, visibleSize.height * 0.5);
    this->addChild(soundText);
    
    //创建一个滑动条
    auto effectSlider = Slider::create();
    effectSlider->loadBarTexture("sliderTrack.png");
    effectSlider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", " ");
    effectSlider->loadProgressBarTexture("sliderProgress.png");
    float effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
    if(effectPercent == 0.0f)
        effectPercent = 100.0f;
    
    //设置背景音乐滑动条的初始值
    effectSlider->setPercent(effectPercent);
    effectSlider->::AudioControl::setPosition(visibleSize.width * 0.6, visibleSize.height * 0.5);
    effectSlider->addEventListener([=](Ref* pSender, Slider::EventType type){
        if(type == Slider::EventType::ON_PERCENTAGE_CHANGED){
            int percent = effectSlider->getPercent();
            SimpleAudioEngine::getInstance()->setEffectsVolume(float(percent) / 100);
            UserDefault::getInstance()->setFloatForKey("effectPercent", percent);
        }
    });
    this->addChild(effectSlider);
    
    //返回
    auto returnButton = Button::create("b1.png");
    returnButton->::AudioControl::setPosition(visibleSize.width - returnButton->getContentSize().width/2 - 50, visibleSize.height - 3 * returnButton->getContentSize().height/2 - 25);
    returnButton->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            //返回到上一个界面
            auto transition = TransitionSlideInR::create(0.5, GameMenu::createScene());
            Director::getInstance()->replaceScene(transition);
        }
    });
    Tools::adapt(returnButton, Size(100,50));
    this->addChild(returnButton);
    
    
    
    return true;
}

Scene* AudioControl::createScene(){
    auto scene = Scene::create();
    auto layer = AudioControl::create();
    scene->addChild(layer);
    return scene;
}
