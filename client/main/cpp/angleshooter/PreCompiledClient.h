#pragma once

#include <angleshooter/PreCompiledHeaders.h>

#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "util/ClientContext.h"
#include "util/Keybinding.h"
#include "managers/InputManager.h"
#include "managers/OptionsManager.h"
#include "states/State.h"
#include "managers/StateManager.h"
#include "managers/AudioManager.h"
#include "menu/Widget.h"
#include "menu/Button.h"
#include "menu/Label.h"
#include "menu/Slider.h"
#include "menu/WidgetContainer.h"
#include "rendering/shaders/PostProcessing.h"
#include "rendering/shaders/BloomProcessing.h"
#include "rendering/WorldRenderer.h"
#include "states/MenuState.h"
#include "states/GameState.h"
#include "states/SettingsState.h"
#include "states/OnboardingState.h"
#include "states/GameOverState.h"
#include "states/PauseState.h"
#include "states/SplashState.h"

#include "AngleShooterClient.h"