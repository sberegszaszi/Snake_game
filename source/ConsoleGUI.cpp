#include <algorithm>
#include <iostream>
#include <string>
#include "ConsoleGUI.hpp"

using namespace std::literals::string_literals;

Input KeyToInput(const int key)
{
    Input userInput;
    switch(key){
        case KEY_LEFT:
            userInput = Input::left;
            break;
        case KEY_UP:
            userInput = Input::up;
            break;
        case KEY_RIGHT:
            userInput = Input::right;
            break;
        case KEY_DOWN:
            userInput = Input::down;
            break;
        case 32: // Space
            userInput = Input::pause;
            break;
        case 113: case 81: // q, Q
            userInput = Input::quit;
            break;
        case 10: // ENTER
            userInput = Input::enter;
            break;
        default:
            userInput = Input::none;
    }
    return userInput;
}

// ConsoleWindow

ConsoleGUI::ConsoleWindow::ConsoleWindow()
{
    if(initscr()==NULL){
        throw("Error while initializing graphics (ncurses library).\n"s);
    }
    if(!has_colors()){
        endwin(); // this is not necessary because of distructor?
        throw("No color support.\n"s);
    }
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // playground border
    init_pair(2, COLOR_WHITE, COLOR_BLACK); // scoreboard border
    init_pair(3, COLOR_WHITE, COLOR_BLACK); // scoreboard text
    init_pair(4, COLOR_RED, COLOR_BLACK); // game over

    init_pair(11, COLOR_BLACK, COLOR_BLUE); // snake head
    init_pair(12, COLOR_BLACK, COLOR_CYAN); // snake
    init_pair(13, COLOR_BLACK, COLOR_RED); // fruit
    init_pair(14, COLOR_BLACK, COLOR_WHITE); // snake head dead
    init_pair(15, COLOR_WHITE, COLOR_BLACK); // snake dead

    getmaxyx(stdscr, windowHeight, windowWidth);
}

ConsoleGUI::ConsoleWindow::~ConsoleWindow()
{
    endwin();
}


// BorderedWindow

ConsoleGUI::BorderedWindow::BorderedWindow(int width, int height, int startX, int startY):
    border{newwin(height + 2, width + 2, startY, startX)},
    window{derwin(border, height, width, 1, 1)}
{}

void ConsoleGUI::BorderedWindow::ResizeWindow(int width, int height, int startX, int startY)
{
    delwin(window);
    delwin(border);
    border = newwin(height + 2, width + 2, startY, startX);
    window = derwin(border, height, width, 1, 1);
    return;
}

int ConsoleGUI::BorderedWindow::GetMinX()
{
    int x, y;
    getbegyx(border, y, x);
    static_cast<void>(y);
    return x;
}

int ConsoleGUI::BorderedWindow::GetMinY()
{
    int x, y;
    getbegyx(border, y, x);
    return y;
}

int ConsoleGUI::BorderedWindow::GetMaxX()
{
    int x, y;
    getmaxyx(border, y, x);
    static_cast<void>(y);
    return GetMinX() + x;
}

int ConsoleGUI::BorderedWindow::GetMaxY()
{
    int x, y;
    getmaxyx(border, y, x);
    return GetMinY() + y;
}

void ConsoleGUI::BorderedWindow::Refresh()
{
    box(border,0,0);
    wrefresh(border);
    wrefresh(window);
    return;
}

void ConsoleGUI::BorderedWindow::Erase()
{
    werase(border);
    werase(window);
    wrefresh(border);
    wrefresh(window);
    return;
}

ConsoleGUI::BorderedWindow::~BorderedWindow()
{
    delwin(window);
    delwin(border);
}


// ChoiceWindow

ConsoleGUI::ChoiceWindow::ChoiceWindow(std::vector<std::string> menuChoices):
    choices{menuChoices},
    highlight{choices.begin()},
    window{newwin(0, 0, 0, 0)}
{
    auto s = std::max_element(choices.begin(), choices.end(), [](std::string s1, std::string s2){ return s1.size() < s2.size(); });
    auto size = s->size();
    windowWidth = size + 4;
    windowHeight = (2 * choices.size()) - 1;
    delwin(window);
    window = newwin(windowHeight, windowWidth, 0, 0);
}

void ConsoleGUI::ChoiceWindow::SetPosition(int startX, int startY)
{
    delwin(window);
    window = newwin(windowHeight, windowWidth, startY, startX);
    return;
}

void ConsoleGUI::ChoiceWindow::IncreaseHighlight()
{
    if(highlight != choices.end()-1){
        ++highlight;
    } else {
        highlight =  choices.begin();
    }
    return;
}

void ConsoleGUI::ChoiceWindow::DecreaseHighlight()
{
    if(highlight != choices.begin()){
        --highlight;
    } else {
        highlight = choices.end()-1;
    }
    return;
}

void ConsoleGUI::ChoiceWindow::Refresh()
{
    int x = 0, y = 0;
    std::string text;
    for(auto i = choices.begin(); i != choices.end(); ++i){
        text = *i;
        for(auto j = 0; j < (windowWidth - static_cast<int>(i->size()))/2; ++j){
            text = ' ' + text + ' ';
        }
        if(i == highlight){
            wattron(window, A_REVERSE);
            mvwprintw(window, y, x, "%s", text.c_str());
            wattroff(window, A_REVERSE);
        } else {
            mvwprintw(window, y, x, "%s", text.c_str());
        }
        y += 2;
    }
    wrefresh(window);
    return;
}

void ConsoleGUI::ChoiceWindow::Erase()
{
    werase(window);
    wrefresh(window);
    return;
}

ConsoleGUI::ChoiceWindow::~ChoiceWindow()
{
    delwin(window);
}


// MenuWindow

ConsoleGUI::MenuWindow::MenuWindow(int width, int height, int startX, int startY, std::vector<std::string> menuChoices):
    windowWidth{width},
    windowHeight{height},
    menuW{width, height, startX, startY},
    choiceW{menuChoices}
{
    int w = menuW.GetMinX() + ((width - choiceW.GetWidth() + 2) / 2);
    int h = menuW.GetMaxY() - choiceW.GetHeight() - 2;
    choiceW.SetPosition(w, h);
}

int ConsoleGUI::MenuWindow::GetChoice()
{
    Input userInput = Input::none;
    int result = 0;
    while(userInput != Input::enter){
        choiceW.Refresh();
        userInput = WaitForUserInput();
        switch(userInput){
            case Input::none: case Input::right: case Input::left: case Input::quit: case Input::pause:
                break;
            case Input::up:
                choiceW.DecreaseHighlight();
                break;
            case Input::down:
                choiceW.IncreaseHighlight();
                break;
            case Input::enter:
                result = choiceW.GetChoice();
                break;
        }
    }
    return result;
}

void ConsoleGUI::MenuWindow::Refresh()
{
    refresh();
    menuW.Refresh();
    choiceW.Refresh();
    return;
}

void ConsoleGUI::MenuWindow::Erase()
{
    menuW.Erase();
    choiceW.Erase();
    return;
}

// GameWindow

ConsoleGUI::GameWindow::GameWindow(int width, int height, int startX, int startY):
    playgroundW{width, height, startX, startY},
    scoreboardW{width, 1, playgroundW.GetMinX() + 1, playgroundW.GetMaxY() + 1},
    pixelData(width, std::vector<PixelType>(height, PixelType::empty))
{}

void ConsoleGUI::GameWindow::RefreshStatic()
{
    wattrset(scoreboardW.GetWindow(),COLOR_PAIR(3) | A_DIM);
    mvwprintw(scoreboardW.GetWindow(),0,1,"Score:");
    mvwprintw(scoreboardW.GetWindow(),0,scoreboardW.GetMaxX()-scoreboardW.GetMinX()-12,"Level:");
    playgroundW.Refresh();
    scoreboardW.Refresh();
}

void ConsoleGUI::GameWindow::RefreshDynamic()
{
    werase(playgroundW.GetWindow());
    int x = 0, y = 0;
    for(auto i = pixelData.begin(); i != pixelData.end(); ++i){
        for(auto j = (*i).begin(); j != (*i).end(); ++j){
            if(*j != PixelType::empty || *j != PixelType::border){
                x = std::distance(pixelData.begin(), i);
                y = std::distance((*i).begin(), j);
            }
            switch(*j){
                case PixelType::empty: case PixelType::border:
                    break;
                case PixelType::snake_head:
                    wattron(playgroundW.GetWindow(), COLOR_PAIR(11));
                    mvwaddch(playgroundW.GetWindow(), y, x, ' ');
                    wattroff(playgroundW.GetWindow(), COLOR_PAIR(11));
                    break;
                case PixelType::snake_body:
                    wattron(playgroundW.GetWindow(), COLOR_PAIR(12));
                    mvwaddch(playgroundW.GetWindow(), y, x, ' ');
                    wattroff(playgroundW.GetWindow(), COLOR_PAIR(12));
                    break;
                case PixelType::fruit:
                    wattron(playgroundW.GetWindow(), COLOR_PAIR(13));
                    mvwaddch(playgroundW.GetWindow(), y, x, ' ');
                    wattroff(playgroundW.GetWindow(), COLOR_PAIR(13));
                    break;
            }
        }
    }
    werase(scoreboardW.GetWindow());
    wattrset(scoreboardW.GetWindow(),COLOR_PAIR(3) | A_DIM);
    mvwprintw(scoreboardW.GetWindow(),0,1,"Score:");
    mvwprintw(scoreboardW.GetWindow(),0,8,"%d ", score);
    mvwprintw(scoreboardW.GetWindow(),0,scoreboardW.GetMaxX()-scoreboardW.GetMinX()-12,"Level:");
    if(level == 10){
        mvwprintw(scoreboardW.GetWindow(),0,scoreboardW.GetMaxX()-scoreboardW.GetMinX()-5,"%d", level);
    } else {
        mvwprintw(scoreboardW.GetWindow(),0,scoreboardW.GetMaxX()-scoreboardW.GetMinX()-5,"0%d", level);
    }
    wrefresh(playgroundW.GetWindow());
    wrefresh(scoreboardW.GetWindow());
    return;
}

void ConsoleGUI::GameWindow::SetPixelData(const Position & p, const PixelType type)
{
    auto maxX = static_cast<int>(pixelData.size()) / 2;
    auto maxY = static_cast<int>(pixelData.at(0).size());
    if(p.GetX() >= 0 && p.GetX() < maxX &&
        p.GetY() >= 0 && p.GetY() < maxY){
            pixelData.at(2 * p.GetX()).at(p.GetY()) = type;
            pixelData.at(2 * p.GetX() + 1).at(p.GetY()) = type;
    }
}

void ConsoleGUI::GameWindow::SetWindowSize(int width, int height, int startX, int startY)
{
    playgroundW.ResizeWindow(width, height, startX, startY);
    scoreboardW.ResizeWindow(width, 1, playgroundW.GetMinX(), playgroundW.GetMaxY());
    pixelData = std::vector<std::vector<PixelType>>(width, std::vector<PixelType>(height, PixelType::empty));
    return;
}

void ConsoleGUI::GameWindow::Erase()
{
    playgroundW.Erase();
    scoreboardW.Erase();
    return;
}

// ConsoleGUI

Input ConsoleGUI::WaitForUserInput()
{
    int key = getch();
    Input userInput = KeyToInput(key);
    return userInput;
}

MainMenuChoice ConsoleGUI::MainMenu()
{
    std::vector<std::string> choices = {"Start Game", "Settings", "Help", "Quit"};
    int width = 18;
    int height = 2 * static_cast<int>(choices.size()) + 1;
    MenuWindow mainmenuW(width, height, ((consoleWidth - width - 4) / 2), ((consoleHeight - height - 4) / 2), choices);
    mainmenuW.Refresh();
    int choice = mainmenuW.GetChoice();
    MainMenuChoice result;
    switch (choice){
        case 0:
            result = MainMenuChoice::start_game;
            break;
        case 1:
            result = MainMenuChoice::settings;
            break;
        case 2:
            result = MainMenuChoice::help;
            break;
        case 3:
            result = MainMenuChoice::quit;
            break;
        default:
            result = MainMenuChoice::quit;
            break;
    }
    mainmenuW.Erase();
    return result;
}

GameSettings ConsoleGUI::SettingsMenu()
{
    std::string text = "Playground size";
    int width = static_cast<int>(text.size()) + 4;
    int height = 11;
    std::vector<std::string> choices = {"15 x 15", "20 x 20", "25 x 25", "30 x 30"};
    MenuWindow sizeSettingsW(width, height, ((consoleWidth - width- 4) / 2), ((consoleHeight - height - 4) / 2), choices);
    mvwprintw(sizeSettingsW.GetWindow(),1,2,text.c_str());
    sizeSettingsW.Refresh();
    int sizeChoice = sizeSettingsW.GetChoice();
    GameSettings settings;
    switch(sizeChoice){
        case 0:
            settings.SetSize(15, 15);
            break;
        case 1:
            settings.SetSize(20, 20);
            break;
        case 2:
            settings.SetSize(25, 25);
            break;
        case 3:
            settings.SetSize(30, 30);
            break;
    }
    sizeSettingsW.Erase();
    text = "Snake speed";
    width = static_cast<int>(text.size()) + 4;
    height = 23;
    choices = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10"};
    MenuWindow levelSettingsW(width, height, ((consoleWidth - width- 4) / 2), ((consoleHeight - height - 4) / 2), choices);
    mvwprintw(levelSettingsW.GetWindow(),1,2,text.c_str());
    levelSettingsW.Refresh();
    int levelChoice = levelSettingsW.GetChoice();
    settings.SetSpeed(levelChoice + 1);
    levelSettingsW.Erase();
    return settings;
}

void ConsoleGUI::HelpMenu()
{
    std::vector<std::string> choices = {"Return"};
    std::vector<std::string> text = {
        "This is a simple Snake game. Goal of the game",
        "is to eat as many fruits as you can. Game ends",
        "if the snake collides with itself or the walls.",
        "",
        "[Arrow keys] = navigation",
        "[Space] = pause",
        "[Q] = quit" };
    int width = 50;
    int height = 2 * choices.size() + text.size() + 2;
    MenuWindow helpW(width, height, ((consoleWidth - width- 4) / 2), ((consoleHeight - height - 4) / 2), choices);
    int y = 1;
    for(auto & i : text){
        mvwprintw(helpW.GetWindow(),y,2,i.c_str());
        ++y;
    }
    helpW.Refresh();
    helpW.GetChoice();
    helpW.Erase();
    return;
}

void ConsoleGUI::SetGameBoardSize(int width, int height)
{
    gameW.SetWindowSize(2 * width, height,  ((consoleWidth - (2 * width) - 2) / 2), ((consoleHeight - height - 2) / 2));
}

void ConsoleGUI::UpdatePixel(const Position & p, const PixelType type)
{
    gameW.SetPixelData(p, type);
}

void ConsoleGUI::UpdateGameData(const int score, const int level)
{
    gameW.SetScore(score);
    gameW.SetLevel(level);
    return;
}

void ConsoleGUI::RefreshGameScreen()
{
    gameW.RefreshDynamic();
    return;
}

void ConsoleGUI::StartScreen()
{
    gameW.RefreshStatic();
    gameW.RefreshDynamic();
    std::string text = "Press [Space] to start";
    int width = static_cast<int>(text.size()) + 4;
    int height = 1;
    BorderedWindow startW{width, height, ((consoleWidth - width - 2) / 2), ((consoleHeight - height - 2) / 2) - 5};
    mvwprintw(startW.GetWindow(),0,2,text.c_str());
    startW.Refresh();
    Input userInput = Input::none;
    while(userInput != Input::pause){
        userInput = WaitForUserInput();
    }
    startW.Erase();
    gameW.RefreshStatic();
    gameW.RefreshDynamic();
    return;
}

void ConsoleGUI::PauseScreen()
{
    std::string text = "Press [Space] to continue";
    int width = static_cast<int>(text.size()) + 4;
    int height = 1;
    BorderedWindow pauseW{width, height, ((consoleWidth - width - 2) / 2), ((consoleHeight - height - 2) / 2)};
    mvwprintw(pauseW.GetWindow(),0,2,text.c_str());
    pauseW.Refresh();
    Input userInput = Input::none;
    while(userInput != Input::pause){
        userInput = WaitForUserInput();
    }
    pauseW.Erase();
    return;
}

bool ConsoleGUI::GameOverScreen()
{
    std::vector<std::string> choices = {"Restart", "Main Menu"};
    std::string text = "GAME OVER";
    int width = static_cast<int>(text.size()) + 8;
    int height = 7;
    MenuWindow gameoverW(width, height, ((consoleW.GetWidth() - width) / 2), ((consoleW.GetHeight()- height) / 2 - 1), choices);
    int x = (width - text.size()) / 2;
    wattron(gameoverW.GetWindow(), COLOR_PAIR(4) | A_BOLD);
    mvwprintw(gameoverW.GetWindow(),1,x,text.c_str());
    wattroff(gameoverW.GetWindow(), COLOR_PAIR(4) | A_BOLD);
    gameoverW.Refresh();
    int choice = gameoverW.GetChoice();
    bool result;
    switch(choice){
        case 0:
            result = true;
            break;
        case 1:
            result = false;
            break;
    }
    gameW.Erase();
    gameoverW.Erase();
    return result;
}

Input ConsoleGUI::GetUserInput()
{
    int key;
    std::stack<int> keyBuffer;
    while((key = getch()) != ERR) {
        keyBuffer.push(key);
    }
    Input userInput = Input::none;
    if(!keyBuffer.empty()){
        userInput = KeyToInput(keyBuffer.top());
    }
    return userInput;
}