//
//  GameScene.cpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#include "GameScene.hpp"
#include "Tools.hpp"

//敌机的标记
static int EOTAG = 10;
static int E1TAG = 11;
static int E2TAG = 12;
static int BOSSTAG = 100;

Scene* Game::createScene(){
    auto scene = Scene::create();
    auto layer = Game::create();
    scene->addChild(layer);
    return scene;
}

bool Game::init(){
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _screenWidth = visibleSize.width;
    _screenHeight = visibleSize.height;
    
    
    AnimationCache::getInstance()->addAnimation(getAnimationByName("plane", 0.08, 2), "fly");
    AnimationCache::getInstance()->addAnimation(getAnimationByName("blast", 0.09, 2), "blast");
    AnimationCache::getInstance()->addAnimation(getAnimationByName("bomb", 10.9, 2), "bomb");
    
    
    return true;
}

void Game::onEnter(){
    Layer::onEnter();
    
    //游戏背景
    _bg1 = Sprite::create("bg2.png");
    _bg1->setAnchorPoint(Vec2::ZERO);
    _bg1->getTexture()->setAliasTexParameters();
    Tools::adapt(_bg1, Size(_screenWidth,_screenHeight)); 
    this->addChild(_bg1, -1);
    
    _bg2 = Sprite::create("bg2.png");
    _bg2->setAnchorPoint(Vec2::ZERO);
    _bg2->getTexture()->setAliasTexParameters();
    _bg2->setPosition(0, _screenHeight);
    Tools::adapt(_bg2, Size(_screenWidth,_screenHeight));
    this->addChild(_bg2, -1);
    
    
    
    std::string bgName;
    
    
    //飞机
    _plane = Sprite::createWithSpriteFrameName("plane0.png");
    _plane->setPosition(Vec2(_screenWidth/2, _plane->getContentSize().height/ 2+5));
    this->addChild(_plane);
    
    //飞机移动效果
    auto planeFlyAnimation = AnimationCache::getInstance()->getAnimation("fly");
    auto animate = Animate::create(planeFlyAnimation);
    auto planeFlyAction = RepeatForever::create(animate);
    _plane->runAction(planeFlyAction);
    
    auto planeListener = EventListenerTouchOneByOne::create();
    planeListener->onTouchBegan = [](Touch* touch, Event* event){
        auto target = event->getCurrentTarget();
        Vec2 location = touch->getLocation();
        Vec2 locationInNode = target->convertToNodeSpace(location);
        Size size = target->getContentSize();
        Rect rect = Rect(0,0,size.width, size.height);
        if(rect.containsPoint(locationInNode)){
            return true;
        }
        return false;
    };
    planeListener->onTouchMoved = [](Touch* touch, Event* event){
        auto target = event->getCurrentTarget();
        target->setPosition(target->getPosition() + touch->getDelta());
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(planeListener, _plane);
    
    this->scheduleUpdate();
}

Animation* Game::getAnimationByName(std::string animName, float delay, int animNum){
    auto animation = Animation::create();
    
    for(unsigned int i = 0;i < animNum;++i){
        std::string frameName = animName;
        frameName.append(StringUtils::format("%d", i)).append(".png");
        
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str()));
    }
    animation->setDelayPerUnit(delay);
    animation->setRestoreOriginalFrame(true);
    return animation;
}

void Game::update(float delta){
    ++_count;
    updateBackground();
    enemyPlaneDive(delta);
    removeEnemy(delta);
}

void Game::updateBackground(){
    Vec2 p1 = _bg1->getPosition();
    p1.y = p1.y - 5;
    if(p1.y <= -_screenHeight){
        p1.y = _screenHeight;
    }
    
    _bg1->setPosition(p1);
    
    Vec2 p2 = _bg2->getPosition();
    p2.y = p2.y - 5;
    if(p2.y <= -_screenHeight){
        p2.y = _screenHeight;
    }
    _bg2->setPosition(p2);
}

void Game::enemyPlaneDive(float delta){
    bool flag = false;
    int lifeValue;
    FKSprite* enemyPlane;
    int randX = arc4random() % (_screenWidth - 40) + 20;
    float durationTime = arc4random() %2 + 2;
    if(_count % 30 == 0){
        int rand = arc4random() % 2;
        switch (rand) {
            case 0:
                enemyPlane = FKSprite::createWithSpriteFrameName("e0.png");
                enemyPlane->setTag(EOTAG);
                break;
                
            case 1:
                enemyPlane = FKSprite::createWithSpriteFrameName("e2.png");
                enemyPlane->setTag(E2TAG);
                
            default:
                break;
        }
        
        lifeValue = 1;
        enemyPlane->setLifeValue(lifeValue);
        enemyPlane->setScale(0.6);
        flag = true;
    } else {
        if(_count % 200 == 0){
            enemyPlane = FKSprite::createWithSpriteFrameName("e1.png");
            enemyPlane->setTag(E1TAG);
            enemyPlane->setScale(0.8);
            lifeValue = 10;
            enemyPlane->setLifeValue(lifeValue);
            flag = true;
        }
    }
    
    if(flag){
        //设置敌机的位置
        enemyPlane->setPosition(randX, _screenHeight + enemyPlane->getContentSize().height);
        auto moveBy = MoveBy::create(durationTime, Vec2(0, -enemyPlane->getPosition().y - enemyPlane->getContentSize().height));
        enemyPlane->runAction(moveBy);
        _enemyVector.pushBack(enemyPlane);
        this->addChild(enemyPlane);
        if(enemyPlane->getTag() == E1TAG){
            //创建一个loadingbar
            auto loadingBar = LoadingBar::create("planeHP.png");
            loadingBar->setScale(0.15);
            
            loadingBar->setDirection(LoadingBar::Direction::LEFT);
            loadingBar->setPercent(100);
            Point pos = enemyPlane->getPosition();
            
            loadingBar->setPosition(Vec2(pos.x, pos.y+48));
            this->addChild(loadingBar, 1);
            auto moveBy2 = MoveBy::create(durationTime, Vec2(0, -pos.y - enemyPlane->getContentSize().height));
            loadingBar->runAction(moveBy2);
            
            enemyPlane->setHP(loadingBar);
            enemyPlane->setHPInterval(100.0f / enemyPlane->getLifeValue());
        }
    }
}

void Game::removeEnemy(float delta){
    for(unsigned int i = 0;i < _enemyVector.size();++i){
        auto enemy = _enemyVector.at(i);
        if(enemy->getPosition().y <= -enemy->getContentSize().height){
            this->removeChild(enemy, true);
            _enemyVector.eraseObject(enemy);
        }
    }
}

