// GameEngine.cpp
#include "GameEngine.hpp"
#include <random>
#include <sstream>
#include <iomanip>

GameEngine::GameEngine()
{
    LOG_INFO("Initializing Game Engine...");

    try
    {
        initializeWindow();
        initializeViews();
        initializeGrid();
        initializeBackground();
        initializeUI();
        initializeTextures();

        // Создание игрового мира
        world = std::make_unique<GameWorld>(window);

        LOG_INFO("Game Engine initialized successfully");
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Failed to initialize Game Engine: " + std::string(e.what()));
        throw;
    }
}

void GameEngine::initializeWindow()
{
    window.create(
        sf::VideoMode(GC::WINDOW_WIDTH, GC::WINDOW_HEIGHT),
        "Asteroids - SFML Game",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(GC::FPS_LIMIT);
    window.setKeyRepeatEnabled(false);

    LOG_INFO("Window created: " + std::to_string(GC::WINDOW_WIDTH) +
             "x" + std::to_string(GC::WINDOW_HEIGHT));
}

void GameEngine::initializeViews()
{
    // Основная камера
    camera.setSize(
        static_cast<sf::Vector2f>(window.getSize()) * GC::DEFAULT_CAMERA_ZOOM);
    camera.setCenter(0, 0);

    // Вид мини-карты
    miniMapView.setCenter(0, 0);
    miniMapView.setSize(GC::MAP_SIZE, GC::MAP_SIZE);
    miniMapView.setViewport(sf::FloatRect(
        GC::MINIMAP_VIEWPORT_X,
        GC::MINIMAP_VIEWPORT_Y,
        GC::MINIMAP_VIEWPORT_WIDTH,
        GC::MINIMAP_VIEWPORT_HEIGHT));

    // Рамка мини-карты
    miniMapBorder.setSize(sf::Vector2f(
        GC::MINIMAP_BORDER_SIZE,
        GC::MINIMAP_BORDER_SIZE));
    miniMapBorder.setPosition(
        GC::WINDOW_WIDTH - GC::MINIMAP_BORDER_SIZE,
        GC::WINDOW_HEIGHT - GC::MINIMAP_BORDER_SIZE);
    miniMapBorder.setFillColor(sf::Color::Transparent);
    miniMapBorder.setOutlineColor(sf::Color::White);
    miniMapBorder.setOutlineThickness(GC::MINIMAP_BORDER_THICKNESS);
}

void GameEngine::initializeGrid()
{
    grid.setPrimitiveType(sf::Lines);

    // Вертикальные линии
    for (int x = -GC::MAP_SIZE; x <= GC::MAP_SIZE; x += GC::GRID_CELL_SIZE)
    {
        sf::Color lineColor = (x < -GC::DEAD_ZONE_SIZE || x > GC::DEAD_ZONE_SIZE)
                                  ? GC::GRID_DEAD_ZONE_COLOR
                                  : GC::GRID_COLOR;

        grid.append(sf::Vertex(sf::Vector2f(x, -GC::MAP_SIZE), lineColor));
        grid.append(sf::Vertex(sf::Vector2f(x, GC::MAP_SIZE), lineColor));
    }

    // Горизонтальные линии
    for (int y = -GC::MAP_SIZE; y <= GC::MAP_SIZE; y += GC::GRID_CELL_SIZE)
    {
        sf::Color lineColor = (y < -GC::DEAD_ZONE_SIZE || y > GC::DEAD_ZONE_SIZE)
                                  ? GC::GRID_DEAD_ZONE_COLOR
                                  : GC::GRID_COLOR;

        grid.append(sf::Vertex(sf::Vector2f(-GC::MAP_SIZE, y), lineColor));
        grid.append(sf::Vertex(sf::Vector2f(GC::MAP_SIZE, y), lineColor));
    }

    LOG_INFO("Grid initialized with " + std::to_string(grid.getVertexCount() / 2) + " lines");
}

void GameEngine::initializeBackground()
{
    generateStarBackground();

    // Создание спрайта фона
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setOrigin(
        GC::BACKGROUND_TEXTURE_SIZE / 2.0f,
        GC::BACKGROUND_TEXTURE_SIZE / 2.0f);
}

void GameEngine::initializeUI()
{
    // Загрузка шрифта
    if (!uiFont.loadFromFile(GC::FONT_FILE))
    {
        LOG_WARN("Failed to load UI font, using default");
        uiFont.loadFromFile("C:/Windows/Fonts/Arial.ttf");
    }

    // FPS текст
    fpsText.setFont(uiFont);
    fpsText.setCharacterSize(16);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setPosition(10.0f, 40.0f);

    // Статистика
    statsText.setFont(uiFont);
    statsText.setCharacterSize(14);
    statsText.setFillColor(sf::Color::Cyan);
    statsText.setPosition(10.0f, 60.0f);
}

void GameEngine::initializeTextures()
{
    // Можно добавить предзагрузку текстур здесь
}

void GameEngine::generateStarBackground()
{
    sf::Image backgroundImage;
    backgroundImage.create(
        GC::BACKGROUND_TEXTURE_SIZE,
        GC::BACKGROUND_TEXTURE_SIZE);

    // Генератор случайных чисел
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> colorDist(0, 255);
    std::uniform_int_distribution<int> alphaDist(50, 200);
    std::uniform_real_distribution<float> starDist(0.0f, 1.0f);

    // Заполнение фона
    for (unsigned int y = 0; y < GC::BACKGROUND_TEXTURE_SIZE; ++y)
    {
        for (unsigned int x = 0; x < GC::BACKGROUND_TEXTURE_SIZE; ++x)
        {
            // Базовый цвет фона
            sf::Color color = GC::BACKGROUND_COLOR_MIN;

            // Добавляем "звёзды" (случайные яркие пиксели)
            if (starDist(rng) < 0.001f)
            {                                                // 0.1% шанс звезды
                int brightness = colorDist(rng) % 155 + 100; // 100-255
                int alpha = alphaDist(rng);
                color = sf::Color(brightness, brightness, brightness, alpha);
            }

            backgroundImage.setPixel(x, y, color);
        }
    }

    if (!backgroundTexture.loadFromImage(backgroundImage))
    {
        LOG_ERROR("Failed to create background texture");
        throw std::runtime_error("Background texture creation failed");
    }

    LOG_INFO("Star background generated");
}

sf::Color GameEngine::getRandomStarColor() const
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(100, 255);

    int value = dist(rng);
    int alpha = dist(rng) % 156 + 100; // 100-255

    // Разные оттенки для разнообразия
    int r = value;
    int g = std::min(255, value + 20);
    int b = std::min(255, value + 40);

    return sf::Color(r, g, b, alpha);
}

void GameEngine::run()
{
    LOG_INFO("Starting main game loop");
    gameClock.restart();

    sf::Clock deltaClock;

    while (isRunning())
    {
        // Вычисление deltaTime
        float deltaTime = deltaClock.restart().asSeconds();

        // Ограничение deltaTime для предотвращения спайков
        if (deltaTime > GC::MAX_DELTA_TIME)
        {
            deltaTime = GC::MAX_DELTA_TIME;
        }

        // Обновление общего времени игры
        totalPlayTime += deltaTime;

        // Обработка событий
        processEvents();

        // Обновление игры (если не на паузе)
        if (!isPaused)
        {
            update(deltaTime);
        }

        // Отрисовка
        render();

        // Обновление FPS
        updateFPS(deltaTime);
    }

    LOG_INFO("Game loop ended. Total play time: " +
             std::to_string(totalPlayTime) + " seconds");
}

void GameEngine::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            LOG_INFO("Window closed by user");
            window.close();
            break;

        case sf::Event::KeyPressed:
            handleKeyPressed(event.key.code);
            break;

        case sf::Event::Resized:
            // Обновление видов при изменении размера окна
            camera.setSize(static_cast<sf::Vector2f>(window.getSize()));
            break;

        case sf::Event::MouseWheelScrolled:
            handleMouseWheelScrolled(event.mouseWheelScroll);
            break;

        case sf::Event::LostFocus:
            if (!isPaused)
            {
                togglePause();
                LOG_INFO("Game paused (window lost focus)");
            }
            break;

        case sf::Event::GainedFocus:
            if (isPaused)
            {
                togglePause();
                LOG_INFO("Game resumed (window gained focus)");
            }
            break;

        default:
            break;
        }
    }
}

void GameEngine::handleKeyPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Escape:
        LOG_INFO("Window closed by ESC key");
        window.close();
        break;

    case sf::Keyboard::P:
    case sf::Keyboard::Space:
        togglePause();
        break;

    case sf::Keyboard::F1:
        toggleDebugInfo();
        break;

    case sf::Keyboard::F2:
        toggleMiniMap();
        break;

    case sf::Keyboard::R:
        if (world)
        {
            world->reset();
            LOG_INFO("Game reset");
        }
        break;

    case sf::Keyboard::F11:
        // Переключение полноэкранного режима (упрощённо)
        window.close();
        window.create(
            sf::VideoMode::getDesktopMode(),
            "Asteroids - SFML Game",
            sf::Style::Fullscreen);
        window.setFramerateLimit(GC::FPS_LIMIT);
        initializeViews();
        LOG_INFO("Switched to fullscreen mode");
        break;

    default:
        break;
    }
}

void GameEngine::update(float deltaTime)
{
    // Установка deltaTime для всех объектов
    if (world)
    {
        world->setDeltaTimeForAllObjects(deltaTime);
        world->update();
    }

    // Обновление камеры
    updateCamera();

    // Обновление фона
    updateBackground();

    // Проверка зоны смерти
    checkDeadZone();

    // Обновление статистики
    updateStats();
}

void GameEngine::updateCamera()
{
    if (world && world->getShip())
    {
        camera.setCenter(world->getShip()->getPosition());
        // Применяем текущий зум
        camera.setSize(
            static_cast<sf::Vector2f>(window.getSize()) * cameraZoom);
    }
}

void GameEngine::zoomCamera(float factor)
{
    cameraZoom *= factor;
    cameraZoom = std::clamp(cameraZoom, GC::MIN_CAMERA_ZOOM, GC::MAX_CAMERA_ZOOM);
    LOG_INFO("Camera zoom: " + std::to_string(cameraZoom) + "x");
}

// GameEngine.cpp - альтернативная реализация
void GameEngine::handleMouseWheelScrolled(sf::Event::MouseWheelScrollEvent &event)
{
    if (event.delta > 0)
    {
        // Колесико вверх - приближение
        zoomCamera(0.9f);
    }
    else if (event.delta < 0)
    {
        // Колесико вниз - отдаление
        zoomCamera(1.1f);
    }
}

void GameEngine::updateBackground()
{
    if (world && world->getShip())
    {
        sf::Vector2f shipPos = world->getShip()->getPosition();
        // Позиция спрайта фона
        backgroundSprite.setPosition(shipPos);
    }
}

void GameEngine::updateFPS(float deltaTime)
{
    frameCount++;

    if (fpsClock.getElapsedTime().asSeconds() >= 0.5f)
    {
        fps = frameCount / fpsClock.restart().asSeconds();
        frameCount = 0;

        fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
    }
}

void GameEngine::updateStats()
{
    if (!showDebugInfo)
    {
        statsText.setString("");
        return;
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);

    if (world && world->getShip())
    {
        auto ship = world->getShip();
        sf::Vector2f pos = ship->getPosition();
        sf::Vector2f vel = ship->getVelocity();
        float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);

        ss << "Pos: (" << pos.x << ", " << pos.y << ")\n";
        ss << "Vel: (" << vel.x << ", " << vel.y << ")\n";
        ss << "Speed: " << speed << "\n";
        ss << "Time: " << totalPlayTime << "s";
    }

    statsText.setString(ss.str());
}

void GameEngine::checkDeadZone()
{
    if (world && world->getShip())
    {
        auto ship = world->getShip();
        sf::Vector2f pos = ship->getPosition();

        if (std::abs(pos.x) > GC::DEAD_ZONE_SIZE ||
            std::abs(pos.y) > GC::DEAD_ZONE_SIZE)
        {

            if (warningClock.getElapsedTime().asSeconds() > 1.0f)
            {
                LOG_WARN("Ship in dead zone! Position: (" +
                         std::to_string(pos.x) + ", " +
                         std::to_string(pos.y) + ")");
                warningClock.restart();
            }
        }
    }
}

void GameEngine::render()
{
    window.clear(sf::Color::Black);

    // Основной вид (игровой мир)
    window.setView(camera);
    renderMainView();

    // Мини-карта
    if (showMiniMap)
    {
        window.setView(miniMapView);
        renderMiniMap();
    }

    // UI поверх всего
    window.setView(window.getDefaultView());
    renderUI();

    window.display();
}

void GameEngine::renderMainView()
{
    // Рисуем спрайт фона (звёзды)
    window.draw(backgroundSprite);
    // Рисуем сетку
    window.draw(grid);

    // Рисуем игровые объекты
    if (world)
    {
        world->draw();
    }
}

void GameEngine::renderMiniMap()
{
    // Сетка на мини-карте
    window.draw(grid);

    // Объекты на мини-карте
    if (world)
    {
        world->draw();
    }
}

void GameEngine::renderUI()
{
    // FPS
    window.draw(fpsText);

    // Статистика (если включена)
    if (showDebugInfo)
    {
        window.draw(statsText);
    }

    // Рамка мини-карты
    if (showMiniMap)
    {
        window.draw(miniMapBorder);
    }

    // Текст паузы
    if (isPaused)
    {
        sf::Text pauseText;
        pauseText.setFont(uiFont);
        pauseText.setCharacterSize(48);
        pauseText.setFillColor(sf::Color::Yellow);
        pauseText.setString("PAUSED");
        pauseText.setOrigin(pauseText.getLocalBounds().width / 2,
                            pauseText.getLocalBounds().height / 2);
        pauseText.setPosition(GC::WINDOW_WIDTH / 2.0f,
                              GC::WINDOW_HEIGHT / 2.0f);
        window.draw(pauseText);
    }
}

void GameEngine::togglePause()
{
    isPaused = !isPaused;
    LOG_INFO(isPaused ? "Game paused" : "Game resumed");
}

void GameEngine::toggleDebugInfo()
{
    showDebugInfo = !showDebugInfo;
    LOG_INFO(showDebugInfo ? "Debug info enabled" : "Debug info disabled");
}

void GameEngine::toggleMiniMap()
{
    showMiniMap = !showMiniMap;
    LOG_INFO(showMiniMap ? "Mini-map enabled" : "Mini-map disabled");
}