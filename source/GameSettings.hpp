#ifndef GAMESETTINGS_HPP
#define GAMESETTINGS_HPP

class GameSettings
{
private:
    int playgroundWidth{20};
    int playgroundHeight{20};
    int snakeSpeed{1};
public:
    explicit GameSettings() = default;
    explicit GameSettings(int w, int h, int s): playgroundWidth{w}, playgroundHeight{h}, snakeSpeed{s} {}
    GameSettings(const GameSettings &) = delete;
    GameSettings & operator=(const GameSettings &) = delete;
    GameSettings(GameSettings &&) = default;
    GameSettings & operator=(GameSettings &&) = default;
    ~GameSettings() = default;

    void SetSize(int w, int h)
    {
        playgroundWidth = w;
        playgroundHeight = h;
        return;
    }
    void SetSpeed(int s){ snakeSpeed = s; return; }
    inline int GetWidth() const { return playgroundWidth; }
    inline int GetHeight() const { return playgroundHeight; }
    inline int GetSpeed() const { return snakeSpeed; }
};

#endif // GAMESETTINGS_HPP