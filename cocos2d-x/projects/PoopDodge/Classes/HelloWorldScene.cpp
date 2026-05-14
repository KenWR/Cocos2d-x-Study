#include "HelloWorldScene.h"

#include <algorithm>
#include <string>

USING_NS_CC;

namespace
{
constexpr float PLAYER_SPEED = 260.0f;
constexpr float PLAYER_WIDTH = 34.0f;
constexpr float PLAYER_HEIGHT = 70.0f;
constexpr float POOP_SIZE = 42.0f;
constexpr float POOP_START_SPEED = 180.0f;
constexpr float POOP_SPEED_PER_SECOND = 8.0f;
constexpr int MAX_POOP_COUNT = 7;
constexpr float POOP_COUNT_INTERVAL = 7.0f;
}

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    createBackground();
    createPlayer();
    createPoops();
    createLabels();
    setupKeyboard();

    scheduleUpdate();

    return true;
}

void HelloWorld::createBackground()
{
    auto background = LayerColor::create(Color4B(35, 38, 44, 255));
    addChild(background, 0);
}

void HelloWorld::createPlayer()
{
    _player = DrawNode::create();
    _player->drawSolidCircle(Vec2::ZERO, PLAYER_WIDTH * 0.5f, 0.0f, 48, Color4F(0.2f, 0.55f, 1.0f, 1.0f));
    _player->setScaleY(PLAYER_HEIGHT / PLAYER_WIDTH);
    _player->setPosition(Vec2(
        _origin.x + _visibleSize.width * 0.5f,
        _origin.y + 72.0f));
    addChild(_player, 2);
}

void HelloWorld::createPoops()
{
    _poops.reserve(MAX_POOP_COUNT);

    for (int i = 0; i < MAX_POOP_COUNT; ++i)
    {
        auto poop = createPoopNode();
        poop->setVisible(false);
        _poops.push_back(poop);
        addChild(poop, 2);
    }
}

cocos2d::DrawNode* HelloWorld::createPoopNode()
{
    auto poop = DrawNode::create();

    Vec2 points[] = {
        Vec2(0.0f, POOP_SIZE * 0.55f),
        Vec2(-POOP_SIZE * 0.5f, -POOP_SIZE * 0.45f),
        Vec2(POOP_SIZE * 0.5f, -POOP_SIZE * 0.45f),
    };
    poop->drawSolidPoly(points, 3, Color4F(0.45f, 0.24f, 0.08f, 1.0f));

    return poop;
}

void HelloWorld::createLabels()
{
    _titleLabel = Label::createWithSystemFont("Poop Dodge", "Arial", 34);
    _titleLabel->setPosition(Vec2(
        _origin.x + _visibleSize.width * 0.5f,
        _origin.y + _visibleSize.height * 0.65f));
    addChild(_titleLabel, 3);

    _guideLabel = Label::createWithSystemFont("Press Enter to Start", "Arial", 22);
    _guideLabel->setPosition(Vec2(
        _origin.x + _visibleSize.width * 0.5f,
        _origin.y + _visibleSize.height * 0.55f));
    addChild(_guideLabel, 3);

    _actionButtonLabel = Label::createWithSystemFont("Start", "Arial", 24);
    auto actionButton = MenuItemLabel::create(_actionButtonLabel, [this](Ref*) {
        if (_state != GameState::Playing)
        {
            startGame();
        }
    });

    _actionMenu = Menu::create(actionButton, nullptr);
    _actionMenu->setPosition(Vec2(
        _origin.x + _visibleSize.width * 0.5f,
        _origin.y + _visibleSize.height * 0.43f));
    addChild(_actionMenu, 3);

    _scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 22);
    _scoreLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
    _scoreLabel->setPosition(Vec2(
        _origin.x + 16.0f,
        _origin.y + _visibleSize.height - 16.0f));
    _scoreLabel->setVisible(false);
    addChild(_scoreLabel, 3);
}

void HelloWorld::setupKeyboard()
{
    auto listener = EventListenerKeyboard::create();

    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event*) {
        if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
            keyCode == EventKeyboard::KeyCode::KEY_A)
        {
            _moveDirection = -1.0f;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
                 keyCode == EventKeyboard::KeyCode::KEY_D)
        {
            _moveDirection = 1.0f;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER ||
                 keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER)
        {
            if (_state != GameState::Playing)
            {
                startGame();
            }
        }
    };

    listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event*) {
        if ((keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
             keyCode == EventKeyboard::KeyCode::KEY_A) &&
            _moveDirection < 0.0f)
        {
            _moveDirection = 0.0f;
        }
        else if ((keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
                  keyCode == EventKeyboard::KeyCode::KEY_D) &&
                 _moveDirection > 0.0f)
        {
            _moveDirection = 0.0f;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::startGame()
{
    _state = GameState::Playing;
    _score = 0.0f;
    _poopSpeed = POOP_START_SPEED;
    _moveDirection = 0.0f;

    _player->setPosition(Vec2(
        _origin.x + _visibleSize.width * 0.5f,
        _origin.y + 72.0f));

    _titleLabel->setVisible(false);
    _guideLabel->setVisible(false);
    _actionMenu->setVisible(false);
    _scoreLabel->setVisible(true);

    for (auto poop : _poops)
    {
        poop->setVisible(false);
    }
    resetPoop(_poops.front());
    updateScore(0.0f);
}

void HelloWorld::endGame()
{
    _state = GameState::Dead;
    _moveDirection = 0.0f;

    _titleLabel->setString("Game Over");
    _titleLabel->setVisible(true);

    _guideLabel->setString("Survived: " + std::to_string(static_cast<int>(_score)) + "s   Press Enter to Retry");
    _guideLabel->setVisible(true);

    _actionButtonLabel->setString("Retry");
    _actionMenu->setVisible(true);
}

void HelloWorld::resetPoop(DrawNode* poop, float yOffset)
{
    const float minX = _origin.x + POOP_SIZE;
    const float maxX = _origin.x + _visibleSize.width - POOP_SIZE;
    const float randomX = RandomHelper::random_real(minX, maxX);

    poop->setPosition(Vec2(randomX, _origin.y + _visibleSize.height + POOP_SIZE + yOffset));
    poop->setVisible(true);
}

void HelloWorld::update(float delta)
{
    if (_state != GameState::Playing)
    {
        return;
    }

    updatePlayer(delta);
    updatePoops(delta);
    updateScore(delta);

    if (isColliding())
    {
        endGame();
    }
}

void HelloWorld::updatePlayer(float delta)
{
    auto position = _player->getPosition();
    position.x += _moveDirection * PLAYER_SPEED * delta;

    const float halfWidth = PLAYER_WIDTH * 0.5f;
    const float minX = _origin.x + halfWidth;
    const float maxX = _origin.x + _visibleSize.width - halfWidth;
    position.x = std::max(minX, std::min(position.x, maxX));

    _player->setPosition(position);
}

void HelloWorld::updatePoops(float delta)
{
    _poopSpeed = POOP_START_SPEED + _score * POOP_SPEED_PER_SECOND;
    const int activeCount = getActivePoopCount();

    for (int i = 0; i < activeCount; ++i)
    {
        auto poop = _poops[i];
        if (!poop->isVisible())
        {
            resetPoop(poop, static_cast<float>(i) * 120.0f);
        }

        auto position = poop->getPosition();
        position.y -= _poopSpeed * delta;
        poop->setPosition(position);

        if (position.y < _origin.y - POOP_SIZE)
        {
            resetPoop(poop);
        }
    }
}

void HelloWorld::updateScore(float delta)
{
    _score += delta;
    _scoreLabel->setString("Survived: " + std::to_string(static_cast<int>(_score)) + "s");
}

int HelloWorld::getActivePoopCount() const
{
    const int count = 1 + static_cast<int>(_score / POOP_COUNT_INTERVAL);
    return std::max(1, std::min(count, MAX_POOP_COUNT));
}

bool HelloWorld::isColliding() const
{
    const Rect playerBox(
        _player->getPositionX() - PLAYER_WIDTH * 0.5f,
        _player->getPositionY() - PLAYER_HEIGHT * 0.5f,
        PLAYER_WIDTH,
        PLAYER_HEIGHT);

    for (auto poop : _poops)
    {
        if (!poop->isVisible())
        {
            continue;
        }

        const Rect poopBox(
            poop->getPositionX() - POOP_SIZE * 0.5f,
            poop->getPositionY() - POOP_SIZE * 0.5f,
            POOP_SIZE,
            POOP_SIZE);

        if (playerBox.intersectsRect(poopBox))
        {
            return true;
        }
    }

    return false;
}
