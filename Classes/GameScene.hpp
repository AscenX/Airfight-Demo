//
//  GameScene.hpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "FKSprite.hpp"

USING_NS_CC;

class Game : public Layer {
private:
    
    int _screenWidth, _screenHeight;
    
    Sprite* _bg1;//背景
    Sprite* _bg2;//背景
    
    Sprite* _plane;//飞机
    
    void update(float delta);//更新函数
    void updateBackground();//更新背景，让背景滚动
    
    int _count;
    Vector<FKSprite*> _enemyVector;
    
    void enemyPlaneDive(float delta);//敌机俯冲
    void removeEnemy(float delta);//移除敌机
    
    Vector<Sprite*> _bulletVector;//射出的子弹数组
    void shootbullet(float delta);//射出子弹
    void removeBullet(float delta);//移除子弹
    void collisionDetection(float delta);//碰撞检测
    void playBmobAnimate(std::string name, Vec2 position);//播放爆炸动画
    void gameOver(std::string message);//游戏结束
    void restart(int tag);//重新开始
    
    Label* _scoreLabel;//得分标签
    int _scoreValue;//分数
    
    FKSprite* _boss;//Boss
    Vector<Sprite*> _bossBulletVector;//Boss子弹数组
    bool _isStart, _isMove, _isShoot;//开始移动和射击
    
    void startBoss();//开始Boss
    void moveBoss();//移动Boss
    void shootBoss(float delta);//射击
    void updateHUD(float delta);//更新Boss血条
    
public:
    virtual bool init();
    static Scene* createScene();
    virtual void onEnter();
    Animation* getAnimationByName(std::string animName, float delay, int animNum);
    
    CREATE_FUNC(Game);
};

#endif /* GameScene_hpp */
