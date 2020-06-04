#ifndef GUINTERFACE_HPP
#define GUINTERFACE_HPP

#include <string>
#include <vector>
#include "GameSettings.hpp"
#include "Position.hpp"

enum class PixelType { border, empty, fruit, snake_head, snake_body };
enum class Input { none, up, right, down, left, quit, enter, pause };
enum class MainMenuChoice { start_game, settings, help, quit };

class GUInterface{
public:
    explicit GUInterface() = default;
    GUInterface(const GUInterface&) = delete;
    GUInterface(GUInterface&&) = default;
    GUInterface& operator=(const GUInterface&) = delete;
    GUInterface& operator=(GUInterface&&) = delete;
    virtual ~GUInterface() = default;

    virtual MainMenuChoice MainMenu() = 0;
    virtual GameSettings SettingsMenu() = 0;
    virtual void HelpMenu() = 0;
    virtual void SetGameBoardSize(int width, int height) = 0;
    virtual void UpdatePixel(const Position & p, const PixelType type) = 0;
    virtual void UpdateGameData(const int score, const int level) = 0;
    virtual void RefreshGameScreen() = 0;
    virtual void StartScreen() = 0;
    virtual void PauseScreen() = 0;
    virtual bool GameOverScreen() = 0;
    virtual Input GetUserInput() = 0;
};

#endif // GUINTERFACE_HPP