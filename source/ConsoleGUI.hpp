#ifndef CONSOLEGUI_HPP
#define CONSOLEGUI_HPP

#include <iterator>
#include <stack>
#include <string>
#include <vector>
#include <ncurses.h>
#include "GUInterface.hpp"


class ConsoleGUI : public GUInterface
{
private:
    class ConsoleWindow
    {
    private:
        int windowWidth{0};
        int windowHeight{0};
    public:
        explicit ConsoleWindow();
        ConsoleWindow(const ConsoleWindow&) = delete;
        ConsoleWindow(ConsoleWindow&&) = delete;
        ConsoleWindow& operator=(const ConsoleWindow&) = delete;
        ConsoleWindow& operator=(ConsoleWindow&&) = delete;
        ~ConsoleWindow();
        int GetHeight() const { return windowHeight; }
        int GetWidth() const { return windowWidth; }
    };

    class BorderedWindow
    {
    private:
        WINDOW* border;
        WINDOW* window;
    public:
        BorderedWindow() = delete;
        explicit BorderedWindow(int width, int height, int startX, int startY);
        BorderedWindow(const BorderedWindow&) = delete;
        BorderedWindow(BorderedWindow&&) = delete;
        BorderedWindow& operator=(const BorderedWindow&) = delete;
        BorderedWindow& operator=(BorderedWindow&) = delete;
        ~BorderedWindow();
        WINDOW* GetWindow(){ return window; }
        // WINDOW* GetBorder(){ return border; }
        void ResizeWindow(int width, int height, int startX, int startY);
        int GetMinX();
        int GetMinY();
        int GetMaxX();
        int GetMaxY();
        void Refresh();
        void Erase();
    };

    class ChoiceWindow
    {
    private:
        std::vector<std::string> choices;
        std::vector<std::string>::iterator highlight;
        int windowWidth{0};
        int windowHeight{0};
        WINDOW* window;
    public:
        ChoiceWindow() = delete;
        explicit ChoiceWindow(std::vector<std::string> menuChoices);
        ChoiceWindow(const ChoiceWindow&) = delete;
        ChoiceWindow(ChoiceWindow&&) = delete;
        ChoiceWindow& operator=(const ChoiceWindow&) = delete;
        ChoiceWindow& operator=(ChoiceWindow&) = delete;
        ~ChoiceWindow();

        int GetWidth(){ return windowWidth; }
        int GetHeight(){ return windowHeight; }
        void SetPosition(int startX, int startY);
        void IncreaseHighlight();
        void DecreaseHighlight();
        int GetChoice(){ return std::distance(choices.begin(), highlight); }
        void Refresh();
        void Erase();
    };

    class MenuWindow
    {
    private:
        int windowWidth;
        int windowHeight;
        BorderedWindow menuW;
        ChoiceWindow choiceW;
    public:
        MenuWindow() = delete;
        explicit MenuWindow(int width, int height, int startX, int startY, std::vector<std::string> menuChoices);
        MenuWindow(const MenuWindow&) = delete;
        MenuWindow(MenuWindow&&) = delete;
        MenuWindow& operator=(const MenuWindow&) = delete;
        MenuWindow& operator=(MenuWindow&) = delete;
        ~MenuWindow() = default;

        WINDOW* GetWindow(){ return menuW.GetWindow(); }
        int GetChoice();
        void Refresh();
        void Erase();
    };

    class GameWindow
    {
    private:
        BorderedWindow playgroundW;
        BorderedWindow scoreboardW;
        std::vector<std::vector<PixelType>> pixelData{};
        int score{0};
        int level{0};
    public:
        GameWindow() = delete;
        explicit GameWindow(int width, int height, int startX, int startY);
        GameWindow(const GameWindow&) = delete;
        GameWindow(GameWindow&&) = delete;
        GameWindow& operator=(const GameWindow&) = delete;
        GameWindow& operator=(GameWindow&) = delete;
        ~GameWindow() = default;

        void SetScore(int s){ score = s; }
        void SetLevel(int l){ level = l; }
        void SetPixelData(const Position & p, const PixelType type);
        void SetWindowSize(int width, int height, int startX, int startY);
        void RefreshStatic();
        void RefreshDynamic();
        void Erase();
    };

    ConsoleWindow consoleW{};
    int consoleWidth{consoleW.GetWidth()};
    int consoleHeight{consoleW.GetHeight()};
    GameWindow gameW{0, 0, 0, 0};

    static Input WaitForUserInput();
public:
    explicit ConsoleGUI() = default;
    ConsoleGUI(const ConsoleGUI&) = delete;
    ConsoleGUI(ConsoleGUI&&) = delete;
    ConsoleGUI& operator=(const ConsoleGUI&) = delete;
    ConsoleGUI& operator=(ConsoleGUI&&) = delete;
    ~ConsoleGUI() = default;

    MainMenuChoice MainMenu() override;
    GameSettings SettingsMenu() override;
    void HelpMenu() override;
    void SetGameBoardSize(int width, int height) override;
    void UpdatePixel(const Position & p, const PixelType type) override;
    void UpdateGameData(const int score, const int level) override;
    void RefreshGameScreen() override;
    void StartScreen() override;
    void PauseScreen() override;
    bool GameOverScreen() override;
    Input GetUserInput() override;
};

#endif // CONSOLEGUI_HPP