#include "AsteroidsGame.h"
#include <iostream>

#include "IRenderer.h"
#include "IInputEvent.h"
#include "IClock.h"
#include "IAudioManager.h"

#include "FakeAudioManager.h"

#include "SFMLRenderer.h"
#include "SFMLInputEvent.h"
#include "SFMLClock.h"
#include "SFMLAudioManager.h"

#include "Logger.hpp"

int main()
{
    auto renderer = std::make_shared<SFMLRenderer>();
    renderer->CreateWindow(800, 800, "Asteroids");
    renderer->SetFramerateLimit(60);

    auto input = std::make_shared<SFMLInputEvent>(renderer->GetWindow());
    auto clock = std::make_shared<SFMLClock>();
    auto audio = std::make_shared<FakeAudioManager>();

    auto game = std::make_shared<AsteroidsGame>(renderer, input, clock, audio);
    game->run();
}