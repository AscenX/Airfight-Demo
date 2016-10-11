//
//  GameScene.cpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#include "GameScene.hpp"
#include "Tools.hpp"
#include "SimpleAudioEngine.h"
#include "MenuScene.hpp"

using namespace CocosDenshion;

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
    
    //缓存好飞行和爆炸的动画
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
    
    //游戏背景
    _bg2 = Sprite::create("bg2.png");
    _bg2->setAnchorPoint(Vec2::ZERO);
    _bg2->getTexture()->setAliasTexParameters();
    _bg2->setPosition(0, _screenHeight);
    Tools::adapt(_bg2, Size(_screenWidth,_screenHeight));
    this->addChild(_bg2, -1);
    
    //玩家飞机
    _plane = Sprite::createWithSpriteFrameName("plane0.png");
    _plane->setPosition(Vec2(_screenWidth/2, _plane->getContentSize().height/ 2+5));
    this->addChild(_plane);
    
    //飞机飞行效果
    auto planeFlyAnimation = AnimationCache::getInstance()->getAnimation("fly");
    auto animate = Animate::create(planeFlyAnimation);
    auto planeFlyAction = RepeatForever::create(animate);
    _plane->runAction(planeFlyAction);
    
    //飞机的事件监听者
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
    
    //调用更新方法
    this->scheduleUpdate();
    
    //播放背景音乐
    SimpleAudioEngine::getInstance()->playBackgroundMusic("s3.wav", true);
    
    //创建得分标签
    _scoreLabel = Label::createWithSystemFont("00", "Arial", 32);
    _scoreLabel->setPosition(50, _screenHeight * 0.98);
    this->addChild(_scoreLabel);
    _scoreValue = 0;
    
    //调用更新的方法
    this->schedule(schedule_selector(Game::shootbullet));
    this->schedule(schedule_selector(Game::collisionDetection));
    this->schedule(schedule_selector(Game::shootBoss));
}

//设置名字延迟来获得动画
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

//调用的更新方法
void Game::update(float delta){
    ++_count;
    updateBackground();
    enemyPlaneDive(delta);
    removeEnemy(delta);
    shootbullet(delta);
    removeBullet(delta);
    collisionDetection(delta);
    updateHUD(delta);
}

//更新背景，让背景可以滚动起来
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

//敌机俯冲
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

//移除敌机
void Game::removeEnemy(float delta){
    for(unsigned int i = 0;i < _enemyVector.size();++i){
        auto enemy = _enemyVector.at(i);
        if(enemy->getPosition().y <= -enemy->getContentSize().height){
            this->removeChild(enemy, true);
            _enemyVector.eraseObject(enemy);
        }
    }
}

//射出子弹
void Game::shootbullet(float delta){
    Vec2 pos = _plane->getPosition();
    if(_count % 8 == 0){
        auto bullet = Sprite::createWithSpriteFrameName("bullet.png");
        bullet->setPosition(pos.x, pos.y + _plane->getContentSize().height / 2 + bullet->getContentSize().height);
        auto moveBy = MoveBy::create(0.4f, Vec2(0, _screenHeight - bullet->getPosition().y));
        bullet->runAction(moveBy);
        this->addChild(bullet, 4);
        _bulletVector.pushBack(bullet);
    }
}

//移除子弹
void Game::removeBullet(float delta){
    for(unsigned int i = 0;i < _bulletVector.size();++i){
        auto bullet = _bulletVector.at(i);
        if(bullet->getPosition().y >= _screenHeight){
            this->removeChild(bullet, true);
            _bulletVector.eraseObject(bullet);
        }
    }
}


//碰撞检测
void Game::collisionDetection(float delta){
    for(unsigned int i = 0;i < _enemyVector.size();++i){
        FKSprite* enemy = _enemyVector.at(i);
        if(_plane->getBoundingBox().intersectsRect(enemy->getBoundingBox())){
            playBmobAnimate("blast", enemy->getPosition());
            
            if(enemy->getTag() != BOSSTAG){
                _enemyVector.eraseObject(enemy);
                this->removeChild(enemy, true);
            }
            
            
            _plane->stopAllActions();
            _plane->setVisible(false);
            gameOver("重新开始");
        } else {
            for(unsigned int i = 0;i < _bulletVector.size();++i){
                auto bullet = _bulletVector.at(i);
                if(enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox())){
                    SimpleAudioEngine::getInstance()->playEffect("bullet.mp3");
                    _bulletVector.eraseObject(bullet);
                    this->removeChild(bullet, true);
                    
                    enemy->setLifeValue(enemy->getLifeValue() - 1);
                    
                    if(enemy->getTag() == E1TAG || enemy->getTag() == BOSSTAG){
                        if(enemy->getHP() != nullptr){
                            enemy->getHP()->setPercent(enemy->getHpInterval() * enemy->getLifeValue());
                        }
                    }
                    
                    if(enemy->getLifeValue() <= 0){
                        
                        if(enemy->getTag() == BOSSTAG){
                            _boss->stopAllActions();
                            playBmobAnimate("bomb", enemy->getPosition());
                            SimpleAudioEngine::getInstance()->playEffect("b1.mp3");
                            
                            _scoreLabel->setString(StringUtils::format("%d", _scoreValue += 10000));
                            gameOver("游戏胜利");
                        } else {
                            //播放爆炸动画
                            playBmobAnimate("blast", enemy->getPosition());
                            SimpleAudioEngine::getInstance()->playEffect("b0.mp3");
                            
                            //判断得分
                            if(enemy->getTag() == E1TAG){
                                _scoreLabel->setString(StringUtils::format("%d", _scoreValue += 500));
                            } else {
                                _scoreLabel->setString(StringUtils::format("%d", _scoreValue += 100));
                            }
                            
                            _enemyVector.eraseObject(enemy);
                            this->removeChild(enemy, true);
                        }
                        
                        if(_scoreValue >= 1000 && !_isStart){
                            startBoss();
                            _isStart = true;
                            break;
                        }
                    }
                    break;
                }
            }
            
            if(_isShoot){
                for(unsigned int i = 0; i < _bossBulletVector.size();++i){
                    auto bossBullet = _bossBulletVector.at(i);
                    if(_plane->getBoundingBox().intersectsRect(bossBullet->getBoundingBox())){
                        _plane->stopAllActions();
                        _plane->setVisible(false);
                        playBmobAnimate("blast", enemy->getPosition());
                        SimpleAudioEngine::getInstance()->playEffect("b0.mp3");
                        gameOver("重新开始");
                    }
                }
            }
        }
    }
}

//根据名字来播放爆炸动画
void Game::playBmobAnimate(std::string name, cocos2d::Vec2 position){
    //    Animation* blastAnimation;
    //    if(name.compare("bomb") == 0){
    //        blastAnimation = AnimationCache::getInstance()->getAnimation("bomb");
    //    } else {
    //        blastAnimation = AnimationCache::getInstance()->getAnimation("blast");
    //    }
    std::string bombName = name + "0.png";
    
    //创建一个精灵
    auto blast = Sprite::createWithSpriteFrameName(bombName);
    blast->setPosition(position);
    
    //获得动画帧
    auto animation = getAnimationByName(name, 0, 4);
    
    auto animate = Animate::create(animation);
    auto callFunc = CallFunc::create([=]{
        this->removeChild(blast, true);
    });
    
    auto action = Sequence::create(animate, callFunc, NULL);
    blast->runAction(action);
    this->addChild(blast);
}

//游戏结束
void Game::gameOver(std::string message){
    
    //停止调用更新函数
    this->unscheduleUpdate();
    
    this->unschedule(schedule_selector(Game::shootbullet));
    this->unschedule(schedule_selector(Game::collisionDetection));
    this->unschedule(schedule_selector(Game::shootBoss));
    
    //重新开始按钮
    auto restartButton = Button::create("button.png");
    restartButton->setTitleText("重新开始");
    restartButton->setTitleFontSize(16);
    restartButton->::Game::setPosition(_screenWidth / 2, _screenHeight * 0.6);
    restartButton->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            this->restart(1);
        }
    });
    Tools::adapt(restartButton, Size(200,100));
    this->addChild(restartButton);
    
    //返回按钮
    auto returnButton = Button::create("button.png");
    returnButton->setTitleText("返回主菜单");
    returnButton->setTitleFontSize(16);
    returnButton->::Game::setPosition(_screenWidth / 2, _screenHeight * 0.4);
    returnButton->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            this->restart(0);
        }
    });
    Tools::adapt(returnButton, Size(200,100));
    this->addChild(returnButton);
}

//重新开始
void Game::restart(int tag){
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    if(tag == 1)
        Director::getInstance()->replaceScene(Game::createScene());
    else
        Director::getInstance()->replaceScene(GameMenu::createScene());
}

//开始打Boss
void Game::startBoss(){
    for(unsigned int i = 0;i < _enemyVector.size();++i){
        auto enemy = _enemyVector.at(i);
        if(enemy->getTag() == E1TAG)
            enemy->getHP()->setPercent(0);
        this->removeChild(enemy, true);
    }
    
    //清空敌机数组
    _enemyVector.clear();
    
    //创建Boss
    _boss = FKSprite::createWithSpriteFrameName("e-10.png");
    _boss->setPosition(_screenWidth / 2,_screenHeight + _boss->getContentSize().height);
    _boss->setTag(BOSSTAG);
    _boss->setLifeValue(10);
    
    //血条
    auto loadingBar = LoadingBar::create("planeHP.png");
    loadingBar->setScale(0.4);
    
    //设置进度条方向
    loadingBar->setDirection(LoadingBar::Direction::LEFT);
    loadingBar->setPercent(100);
    
    //设置血条
    _boss->setHP(loadingBar);
    this->addChild(_boss->getHP(), 1);
    _boss->setHPInterval(100 / _boss->getLifeValue());
    
    _enemyVector.pushBack(_boss);
    this->addChild(_boss, 4);
    
    auto MoveTo = MoveTo::create(2, Vec2(_screenWidth / 2, _screenHeight - _boss->getContentSize().height));
    auto action = Sequence::create(MoveTo, CallFunc::create([=]{
        _isMove = true;
        moveBoss();
    }), NULL);
    
    _boss->runAction(action);
}

//Boss移动方法
void Game::moveBoss(){
    _isShoot = true;
    Size bossContentSize = _boss->getContentSize();
    auto moveLeft = MoveTo::create(2.5, Vec2(bossContentSize.width / 2, _screenHeight - bossContentSize.height));
    auto moveRiht = MoveTo::create(2.5, Vec2(bossContentSize.width / 2, _screenHeight - bossContentSize.height));
    auto delay = DelayTime::create(1);
    auto sequence = Sequence::create(delay, moveLeft, moveRiht, NULL);
    auto action = RepeatForever::create(sequence);
    _boss->runAction(action);
}

//Boss射击
void Game::shootBoss(float delta){
    if(_isShoot){
        if(_count % 80 == 0){
            for(int i = 6;i <= 180;i += 18){
                float startX = _boss->getPositionX();
                float startY = _boss->getPositionY();
                
                Sprite* bullet = Sprite::create("enemy_bullet1.png");
                bullet->setPosition(startX, startY);
                
                this->addChild(bullet);
                
                float r2 = _screenHeight + bullet->getContentSize().height / 2;
                float endX = startX - r2 * cosf(i * M_PI / 180);
                float endY = startY - r2 * sinf(i * M_PI / 180);
                
                MoveTo* moveTo = MoveTo::create(3.0f, Vec2(endX, endY));
                _bossBulletVector.pushBack(bullet);
                bullet->runAction(moveTo);
            }
        }
    }
}

//更新Boss的血条
void Game::updateHUD(float delta){
    if(_boss != NULL && _boss->getLifeValue() > 0){
        Vec2 pos = _boss->getPosition();
        _boss->getHP()->setPosition(Vec2(pos.x, pos.y + 100));
    }
}
