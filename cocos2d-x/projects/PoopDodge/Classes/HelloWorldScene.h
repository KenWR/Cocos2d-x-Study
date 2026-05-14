/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include <vector>

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void update(float delta) override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    enum class GameState
    {
        Ready,
        Playing,
        Dead
    };

    void createBackground();
    void createPlayer();
    void createPoops();
    cocos2d::DrawNode* createPoopNode();
    void createLabels();
    void setupKeyboard();
    void startGame();
    void endGame();
    void resetPoop(cocos2d::DrawNode* poop, float yOffset = 0.0f);
    void updatePlayer(float delta);
    void updatePoops(float delta);
    void updateScore(float delta);
    int getActivePoopCount() const;
    bool isColliding() const;

    GameState _state = GameState::Ready;
    cocos2d::DrawNode* _player = nullptr;
    std::vector<cocos2d::DrawNode*> _poops;
    cocos2d::Label* _titleLabel = nullptr;
    cocos2d::Label* _guideLabel = nullptr;
    cocos2d::Label* _scoreLabel = nullptr;
    cocos2d::Label* _actionButtonLabel = nullptr;
    cocos2d::Menu* _actionMenu = nullptr;
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    float _moveDirection = 0.0f;
    float _score = 0.0f;
    float _poopSpeed = 180.0f;
};

#endif // __HELLOWORLD_SCENE_H__
