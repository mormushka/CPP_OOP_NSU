// GameWorld.cpp
#include "GameWorld.hpp"
#include "SFMLGameObjectFactory.hpp"
#include <algorithm>
#include <cstdlib>
#include <cmath>

GameWorld::GameWorld(sf::RenderWindow &win) : window(win), score(0), isGameOver(false)
{
    // Используем константы из GameConstants
    objects.push_back(factory.createShip());
    objects.push_back(factory.createAsteroid(
        GC::ASTEROID_LARGE_RADIUS,
        GC::ASTEROID_1_POSITION,
        GC::ASTEROID_1_VELOCITY));
    objects.push_back(factory.createAsteroid(
        GC::ASTEROID_MEDIUM_RADIUS,
        GC::ASTEROID_2_POSITION,
        GC::ASTEROID_2_VELOCITY));

    font.loadFromFile(GC::FONT_FILE);
    scoreText.setFont(font);
    scoreText.setCharacterSize(GC::SCORE_TEXT_SIZE);
    scoreText.setFillColor(GC::SCORE_TEXT_COLOR);
    scoreText.setPosition(GC::SCORE_TEXT_POSITION);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(GC::GAME_OVER_TEXT_SIZE);
    gameOverText.setFillColor(GC::GAME_OVER_TEXT_COLOR);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(GC::GAME_OVER_TEXT_POSITION);
}

void GameWorld::setDeltaTimeForAllObjects(float deltaTime)
{
    for (auto &obj : objects)
    {
        if (IMovable *movable = dynamic_cast<IMovable *>(obj.get()))
        {
            movable->setDeltaTimeFactor(deltaTime);
        }
    }
}

bool GameWorld::isObjectOffScreen(const IGameObject *obj) const
{
    const sf::FloatRect bounds = obj->getBounds();
    sf::Vector2f pos = {bounds.left + bounds.width / 2, bounds.top + bounds.height / 2};

    float halfMap = GC::MAP_SIZE / 2.0f;
    float margin = GC::OFFSCREEN_MARGIN;

    return (pos.x < -halfMap - margin ||
            pos.x > halfMap + margin ||
            pos.y < -halfMap - margin ||
            pos.y > halfMap + margin);
}

void GameWorld::reset()
{
    objects.clear();
    score = 0;
    isGameOver = false;
    objects.push_back(factory.createShip());
    objects.push_back(factory.createAsteroid(40.f, {200.f, 200.f}, {1.f, 0.5f}));
    objects.push_back(factory.createAsteroid(25.f, {600.f, 400.f}, {-0.7f, 1.2f}));
    scoreText.setString("Score: 0");
}

Ship *GameWorld::getShip()
{
    return dynamic_cast<Ship *>(objects[0].get());
}

void GameWorld::update()
{
    if (isGameOver)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            reset();
        }
        return;
    }

    for (auto &obj : objects)
        obj->update();

    // удаляем пули и астероиды за картой
    objects.erase(std::remove_if(objects.begin(), objects.end(),
                                 [&](const std::unique_ptr<IGameObject> &obj)
                                 {
                                     if (dynamic_cast<Bullet *>(obj.get()) ||
                                         dynamic_cast<Asteroid *>(obj.get()))
                                     {
                                         return isObjectOffScreen(obj.get());
                                     }
                                     return false;
                                 }),
                  objects.end());

    handleCollisions();
    spawnAsteroidsIfNeeded();

    controller.handleInput(*getShip(), window, objects);

    scoreText.setString("Score: " + std::to_string(score));
}

void GameWorld::draw()
{
    for (auto &obj : objects)
    {
        if (IDrawable *drawableObj = dynamic_cast<IDrawable *>(obj.get()))
            drawableObj->draw(window);
    }
    window.setView(window.getDefaultView());
    window.draw(scoreText);

    if (isGameOver)
    {
        window.draw(gameOverText);
        sf::Text finalScore;
        finalScore.setFont(font);
        finalScore.setCharacterSize(32);
        finalScore.setFillColor(sf::Color::Yellow);
        finalScore.setString("Final Score: " + std::to_string(score));
        finalScore.setPosition(220.f, 320.f);
        window.draw(finalScore);
    }
}

void GameWorld::handleCollisions()
{
    // Проверка столкновения корабля с астероидами
    Ship *ship = getShip();
    if (ship)
    {
        for (auto &obj : objects)
        {
            Asteroid *asteroid = dynamic_cast<Asteroid *>(obj.get());
            if (asteroid && ship->getBounds().intersects(asteroid->getBounds()))
            {
                isGameOver = true;
                return;
            }
        }
    }

    // Сначала собираем объекты для удаления
    std::vector<size_t> indicesToRemove;

    // Проверка столкновений пуль с астероидами
    for (size_t i = 0; i < objects.size(); ++i)
    {
        Bullet *bullet = dynamic_cast<Bullet *>(objects[i].get());
        if (!bullet)
            continue;

        for (size_t j = 0; j < objects.size(); ++j)
        {
            if (i == j)
                continue;

            Asteroid *asteroid = dynamic_cast<Asteroid *>(objects[j].get());
            if (asteroid && bullet->getBounds().intersects(asteroid->getBounds()))
            {
                float radius = asteroid->getBounds().width / 2.f;

                // если большой астероид — разбиваем на два маленьких
                if (radius > 30.f)
                {
                    sf::Vector2f pos = asteroid->getBounds().getPosition();
                    sf::Vector2f vel = asteroid->getVelocity();
                    objects.push_back(factory.createAsteroid(20.f, pos, {vel.x-GC::ASTEROID_SPLIT_PARAMETR, vel.y}));
                    objects.push_back(factory.createAsteroid(20.f, pos, {vel.x, vel.y-GC::ASTEROID_SPLIT_PARAMETR}));
                }

                // увеличиваем счёт
                score += 10;

                // Сохраняем индексы для удаления (если еще не добавлены)
                if (std::find(indicesToRemove.begin(), indicesToRemove.end(), i) == indicesToRemove.end())
                    indicesToRemove.push_back(i);
                if (std::find(indicesToRemove.begin(), indicesToRemove.end(), j) == indicesToRemove.end())
                    indicesToRemove.push_back(j);

                break; // Пуля может уничтожить только один астероид
            }
        }
    }

    // Удаляем объекты в обратном порядке
    std::sort(indicesToRemove.begin(), indicesToRemove.end(), std::greater<size_t>());
    indicesToRemove.erase(std::unique(indicesToRemove.begin(), indicesToRemove.end()), indicesToRemove.end());

    for (size_t idx : indicesToRemove)
    {
        if (idx < objects.size())
        {
            objects.erase(objects.begin() + idx);
        }
    }
}

void GameWorld::spawnAsteroidsIfNeeded()
{
    int asteroidCount = 0;
    for (auto &obj : objects)
        if (dynamic_cast<Asteroid *>(obj.get()))
            asteroidCount++;

    if (asteroidCount < GC::MAX_ASTEROIDS && (rand() % 100 < GC::ASTEROID_SPAWN_CHANCE))
    {
        int side = rand() % 4;

        sf::Vector2f position;
        sf::Vector2f baseVelocity;

        // Базовые направления в зависимости от стороны
        switch (side)
        {
        case 0: // сверху
            position.x = (rand() % static_cast<int>(GC::MAP_SIZE)) - GC::HALF_MAP_SIZE;
            position.y = -GC::HALF_MAP_SIZE - GC::ASTEROID_SPAWN_MARGIN;
            baseVelocity = {0.0f, 1.0f}; // вниз
            break;

        case 1: // справа
            position.x = GC::HALF_MAP_SIZE + GC::ASTEROID_SPAWN_MARGIN;
            position.y = (rand() % static_cast<int>(GC::MAP_SIZE)) - GC::HALF_MAP_SIZE;
            baseVelocity = {-1.0f, 0.0f}; // влево
            break;

        case 2: // снизу
            position.x = (rand() % static_cast<int>(GC::MAP_SIZE)) - GC::HALF_MAP_SIZE;
            position.y = GC::HALF_MAP_SIZE + GC::ASTEROID_SPAWN_MARGIN;
            baseVelocity = {0.0f, -1.0f}; // вверх
            break;

        case 3: // слева
            position.x = -GC::HALF_MAP_SIZE - GC::ASTEROID_SPAWN_MARGIN;
            position.y = (rand() % static_cast<int>(GC::MAP_SIZE)) - GC::HALF_MAP_SIZE;
            baseVelocity = {1.0f, 0.0f}; // вправо
            break;
        }

        // Случайный размер
        float radius;
        float sizeRand = static_cast<float>(rand() % 100);
        if (sizeRand < 30)
            radius = GC::ASTEROID_LARGE_RADIUS; // 30% большие
        else if (sizeRand < 70)
            radius = GC::ASTEROID_MEDIUM_RADIUS; // 40% средние
        else
            radius = GC::ASTEROID_SMALL_RADIUS; // 30% маленькие

        // Случайная скорость
        float speed = GC::ASTEROID_MIN_SPAWN_SPEED +
                      (rand() % static_cast<int>(GC::ASTEROID_MAX_SPAWN_SPEED -
                                                 GC::ASTEROID_MIN_SPAWN_SPEED));

        // Добавляем случайное отклонение направления (до 45 градусов)
        float angle = (rand() % 90 - 45) * 3.14159f / 180.0f; // от -45 до +45 градусов
        sf::Vector2f velocity;
        velocity.x = baseVelocity.x * cos(angle) - baseVelocity.y * sin(angle);
        velocity.y = baseVelocity.x * sin(angle) + baseVelocity.y * cos(angle);
        velocity *= speed;

        // Создаем астероид
        objects.push_back(factory.createAsteroid(radius, position, velocity));
    }
}
