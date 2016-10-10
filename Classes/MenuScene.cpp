//
//  MenuScene.cpp
//  Airfight
//
//  Created by Ascen on 16/10/8.
//
//

#include "MenuScene.hpp"
#include "ui/CocosGUI.h"
#include "AudioControlScene.hpp"
#include "PreLoadScene.hpp"
#include "Tools.hpp"

using namespace ui;

Scene* GameMenu::createScene(){
    auto scene = Scene::create();
    auto layer = GameMenu::create();
    scene->addChild(layer);
    return scene;
}

bool GameMenu::init(){
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* bg1 = Sprite::create("bg5.png");
    bg1->setPosition(visibleSize.width/2, visibleSize.height/2);
    Tools::adapt(bg1, visibleSize);
    this->addChild(bg1, -1);
    
    //开始按钮
    auto startButton = Button::create("button.png");
    startButton->setTitleText("开始游戏");
    startButton->setTitleFontSize(16);
    startButton->::GameMenu::setPosition(visibleSize.width / 2, visibleSize.height * 0.7);
    startButton->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            auto transition = TransitionFade::create(0.5, PreLoad::createScene());
            Director::getInstance()->replaceScene(transition);
        }
    });
    Tools::adapt(startButton, Size(200,100));
    
    this->addChild(startButton);
    
    //设置按钮
    auto setButton = Button::create("button.png");
    setButton->setTitleText("游戏设置");
    setButton->setTitleFontSize(16);
    setButton->::GameMenu::setPosition(visibleSize.width / 2, visibleSize.height * 0.55);
    setButton->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            auto transition = TransitionSlideInL::create(0.5, AudioControl::createScene());
            Director::getInstance()->replaceScene(transition);
        }
    });
    Tools::adapt(setButton, Size(200,100));
    this->addChild(setButton);
    
    //退出按钮
    auto clostButton = Button::create("button.png");
    clostButton->setTitleText("退出游戏");
    clostButton->setTitleFontSize(16);
    clostButton->::GameMenu::setPosition(visibleSize.width / 2, visibleSize.height * 0.4);
    clostButton->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            Director::getInstance()->end();
            exit(0);
        }
    });
    Tools::adapt(clostButton, Size(200,100));
    this->addChild(clostButton);
    
    return true;
}
