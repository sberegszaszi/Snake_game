#include "Game.hpp"

Game::Game(GameSettings & settings, GUInterface & programGui):
    playground{settings.GetWidth(), settings.GetHeight()},
    snake{Position{playground.GetWidth()/2,playground.GetHeight()/2}},
    fruit{Position{(playground.GetWidth()-1), (playground.GetHeight()-1)}},
    snakeSpeed{settings.GetSpeed()},
    updateTime{1000 / (3 + snakeSpeed)},
    gui{programGui}
{ }

void Game::EatFruit()
{
    snake.IncreaseLength();
    while( !snake.NotInBody(fruit.GetPosition()) || fruit.GetPosition() == snake.GetNextHeadPosition()){
        fruit.Regenerate();
    }
    ++gameScore;
    return;
}

bool Game::IsGameOver(const Position & nextHeadPos) const
{
    return !(playground.IsInside(nextHeadPos) && snake.NotInBody(nextHeadPos));
}

void Game::SendDataToGui() const
{
    playground.SendPositionDataTo(gui);
    fruit.SendPositionDataTo(gui);
    snake.SendPositionDataTo(gui);
    gui.UpdateGameData(gameScore, snakeSpeed);
    return;
}

bool Game::Play()
{
    Timer timer;
    int delay = updateTime;
    fruit.Regenerate();
    SendDataToGui();
    gui.StartScreen();
    while(true){
        timer.Start();
        userInput = gui.GetUserInput();
        if(userInput == Input::pause){
            timer.Stop();
            gui.PauseScreen();
            userInput = Input::none;
            timer.Start();
        }
        if(userInput == Input::quit){
            break;
        }
        snake.UpdateDirection(userInput);
        if(snake.GetNextHeadPosition() == fruit.GetPosition()){
            EatFruit();
        }
        if(IsGameOver(snake.GetNextHeadPosition()) == true){
            break;
        }
        if(snakeSpeed != 10 && snake.GetLength() == (snakeSpeed * 10)){
            ++snakeSpeed;
            updateTime = (1000 / (3 + snakeSpeed));
        }
        snake.Move();
        SendDataToGui();
        gui.RefreshGameScreen();
        timer.Stop();
        delay = updateTime - timer.ElapsedTime();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    return gui.GameOverScreen();
}