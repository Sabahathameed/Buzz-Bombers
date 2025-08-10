#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include<fstream>
#include<string>
using namespace std;
using namespace sf;
//Global variables
const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;

// Game grid
int gameGrid[gameRows][gameColumns] = {};

// Function declarations
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock, int& remaining_sprays, Text& spraysText);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
void moveBees(float beeX[], float beeY[], float beeSpeedX[], float beeSpeedY[], float deltaTime, int numBees);
void drawBees(RenderWindow& window, Sprite& beeSprite, float beeX[], float beeY[], int numBees, float beeSpeedX[]);
void movePlayer(float& player_x, float& player_y, float deltaTime);
void drawHoneycomb(RenderWindow& window, Sprite& honeycombSprite, float honeycomb_x, float honeycomb_y);
bool bulletAndBeeCollision(float bullet_x, float bullet_y, float beeX, float beeY, float boxPixelsX);
bool bulletAndHoneycombCollision(float bullet_x, float bullet_y, float honeycomb_x, float honeycomb_y, float boxPixelsX);
bool beeAndGroundCollision(RenderWindow& window, float beeX[], float beeY[], int numBees,
    float flowerX[], float flowerY[], int& flowerCount, float groundY, Sprite& flowerSprite);
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, float flowerX[], float flowerY[], int flowerCount);
void drawLives(RenderWindow& window, Sprite& playerSprite, int& lives, int& remaining_sprays);
void display_menu(int& score);
void display_score(int& score);
void moveHummingbird(float deltaTime, float& hummingbird_x, float& hummingbird_y, float& hummingbird_speed_x, float& hummingbird_speed_y, bool& isHovering, Clock& hoverClock);
bool checkHummingbirdHoneycombCollision(float hummingbird_x, float hummingbird_y, float honeycomb_x, float honeycomb_y);
void drawHummingbird(RenderWindow& window, Sprite& hummingbirdSprite, float hummingbird_x, float hummingbird_y);
void beeAndCombCollision(float beeX[], float beeY[], int numBees, float honeycombX[], float honeycombY[], bool honeycombActive[], int maxHoneycombs, float beeSpeedX[]);
void level1();
void level2();
void level3();
int load_high_score();
void save_high_score(int score);



int main() {
    int score = 0;//initial score

    display_menu(score);//calling function to display menu

    int level = 1;

    bool gameOver = false;
    switch (level)//switch for trnasitioning between levels
    {

    case 1: level1();
        break;
    case 2: level2();
        break;
    case 3: level3();
        break;

    default:
        gameOver = true;
    }
    return 0;
}
void display_menu(int& score) {
    RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Main Menu - Buzz Bombers");

    // Load background texture
    Texture backgroundmenuTexture;
    Sprite backgroundmenu;//sprite for background
    if (!backgroundmenuTexture.loadFromFile("Textures/backgroundmenu.jpg")) {
        cout << "Error loading background texture!" << endl;
        return;
    }
    backgroundmenu.setTexture(backgroundmenuTexture);
    backgroundmenu.setPosition(0, 0);//position for background
    backgroundmenu.setScale(1.6f, 1.75f);//used  for sizing

    // Load font
    Font font;// for displaying the menu options
    if (!font.loadFromFile("font//Press_Start_2P/PressStart2P-Regular.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    // MAIN MENU ITEMS
    Text startGame("Start Game", font, 30);
    startGame.setPosition(350, 210);

    Text viewScore("View Score", font, 30);
    viewScore.setPosition(350, 260);

    Text levels("Levels", font, 30);
    levels.setPosition(350, 310);

    Text exitGame("Exit Game", font, 30);
    exitGame.setPosition(350, 360);

    int selectedIndex = 0;
    startGame.setFillColor(sf::Color::Yellow);
    viewScore.setFillColor(sf::Color::White);
    levels.setFillColor(sf::Color::White);
    exitGame.setFillColor(sf::Color::White);

    bool inLevelsMenu = false; // flag to check if we are in levels submenu

    // LEVELS MENU ITEMS
    Text level1Text("Level 1", font, 30);
    level1Text.setPosition(350, 190);

    Text level2Text("Level 2", font, 30);
    level2Text.setPosition(350, 240);

    Text level3Text("Level 3", font, 30);
    level3Text.setPosition(350, 290);

    Text backText("Back to Menu", font, 30);
    backText.setPosition(350, 340);

    int levelSelectedIndex = 0;
    level1Text.setFillColor(sf::Color::Yellow);
    level2Text.setFillColor(sf::Color::White);
    level3Text.setFillColor(sf::Color::White);
    backText.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                // ===== MAIN MENU =====
                if (!inLevelsMenu) {
                    if (event.key.code == sf::Keyboard::Up) {
                        if (selectedIndex == 0) selectedIndex = 3;
                        else selectedIndex--;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        if (selectedIndex == 3) selectedIndex = 0;
                        else selectedIndex++;
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (selectedIndex == 0) { // Start Game
                            window.close();
                            level1();
                        }
                        else if (selectedIndex == 1) { // View Score
                            window.close();
                            display_score(score);
                        }
                        else if (selectedIndex == 2) { // Levels Menu
                            inLevelsMenu = true;
                        }
                        else if (selectedIndex == 3) { // Exit
                            exit(0);
                        }
                    }

                    // Update highlight for main menu
                    startGame.setFillColor(selectedIndex == 0 ? sf::Color::Yellow : sf::Color::White);
                    viewScore.setFillColor(selectedIndex == 1 ? sf::Color::Yellow : sf::Color::White);
                    levels.setFillColor(selectedIndex == 2 ? sf::Color::Yellow : sf::Color::White);
                    exitGame.setFillColor(selectedIndex == 3 ? sf::Color::Yellow : sf::Color::White);
                }
                // ===== LEVELS MENU =====
                else {
                    if (event.key.code == sf::Keyboard::Up) {
                        if (levelSelectedIndex == 0) levelSelectedIndex = 3;
                        else levelSelectedIndex--;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        if (levelSelectedIndex == 3) levelSelectedIndex = 0;
                        else levelSelectedIndex++;
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (levelSelectedIndex == 0) { window.close(); level1(); }
                        else if (levelSelectedIndex == 1) { window.close(); level2(); }
                        else if (levelSelectedIndex == 2) { window.close(); level3(); }
                        else if (levelSelectedIndex == 3) { inLevelsMenu = false; }
                    }

                    // Update highlight for levels menu
                    level1Text.setFillColor(levelSelectedIndex == 0 ? sf::Color::Yellow : sf::Color::White);
                    level2Text.setFillColor(levelSelectedIndex == 1 ? sf::Color::Yellow : sf::Color::White);
                    level3Text.setFillColor(levelSelectedIndex == 2 ? sf::Color::Yellow : sf::Color::White);
                    backText.setFillColor(levelSelectedIndex == 3 ? sf::Color::Yellow : sf::Color::White);
                }
            }
        }

        window.clear();
        window.draw(backgroundmenu);

        if (!inLevelsMenu) {
            window.draw(startGame);
            window.draw(viewScore);
            window.draw(levels);
            window.draw(exitGame);
        }
        else {
            window.draw(level1Text);
            window.draw(level2Text);
            window.draw(level3Text);
            window.draw(backText);
        }

        window.display();
    }
}

void display_score(int& score) {
    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Scores - Buzz Bombers");

    // Load background texture for score display
    Texture backgroundscoreTexture;
    Sprite backgroundscore;
    if (!backgroundscoreTexture.loadFromFile("Textures/backgroundscore.jpeg")) {
        cout << "Error loading background texture!" << endl;
        return;
    }
    backgroundscore.setTexture(backgroundscoreTexture);
    backgroundscore.setPosition(0, 0);
    backgroundscore.setScale(1.5f, 4.0f);

    // Load font for score display
    Font font;
    if (!font.loadFromFile("font/arial.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    // Display current score
    Text scoreText("Your Score:", font, 40);
    scoreText.setPosition(280, 200);
    scoreText.setFillColor(sf::Color::Yellow);

    Text scoreValue(to_string(score), font, 40);
    scoreValue.setPosition(550, 200);
    scoreValue.setFillColor(Color::White);

    // Display high score from file
    int highScore = load_high_score();
    Text highscoreText("High Score:", font, 40);
    highscoreText.setPosition(280, 300);
    highscoreText.setFillColor(Color::Yellow);

    Text highscoreValue(to_string(highScore), font, 40);
    highscoreValue.setPosition(550, 300);
    highscoreValue.setFillColor(Color::White);

    // if the player presses escape, he exits from the menu
    Text escapeText("Press ESC to return to menu", font, 30);
    escapeText.setPosition(300, 500);
    escapeText.setFillColor(sf::Color::Yellow);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
                display_menu(score); // Return to main menu
            }
        }

        window.clear();//clearing the window before drawing
        window.draw(backgroundscore);
        window.draw(scoreText);
        window.draw(scoreValue);
        window.draw(highscoreText);
        window.draw(highscoreValue);
        window.draw(escapeText);
        window.display();
    }
}
void level1()//function for level 1
{
    srand(time(0));//for random number generation
    int score = 0;
    RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers- Level 1", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(500, 200));

    Texture backgroundTexture;
    Sprite background;
    if (!backgroundTexture.loadFromFile("Textures/background.jpg"))
    {
        cout << "Error loading background texture!" << endl;
        return; // Exit if texture cannot be loaded
    }
    background.setTexture(backgroundTexture);
    background.setPosition(0, 0);
    background.setScale(0.5f, 0.7f);

    // Background Music
    Music bgMusic;
    if (!bgMusic.openFromFile("Music/Music3.ogg")) {
        cout << "Error: Could not load music file!" << endl;
    }
    bgMusic.setVolume(50);
    bgMusic.setLoop(true);
    bgMusic.play();

    // Load textures for more sprites 
    Texture playerTexture, beeTexture, bulletTexture, honeycombTexture, flowerTexture;
    Sprite playerSprite, beeSprite, bulletSprite, honeycombSprite, flowerSprite;

    if (!playerTexture.loadFromFile("Textures/spray.png") ||
        !beeTexture.loadFromFile("Textures/Regular_bee.png") ||
        !bulletTexture.loadFromFile("Textures/bullet.png") ||
        !honeycombTexture.loadFromFile("Textures/honeycomb.png") ||
        !flowerTexture.loadFromFile("Textures/obstacles.png")) {
        cout << "Error loading textures!" << endl;
        return;
    }

    playerSprite.setTexture(playerTexture);
    beeSprite.setTexture(beeTexture);
    bulletSprite.setTexture(bulletTexture);
    honeycombSprite.setTexture(honeycombTexture);
    flowerSprite.setTexture(flowerTexture);
    flowerSprite.setScale(3, 3);
    bulletSprite.setScale(3, 3);

    Texture hummingbirdTexture;
    if (!hummingbirdTexture.loadFromFile("Textures/hummingbird.png")) {
        return;
    }
    Sprite hummingbirdSprite(hummingbirdTexture);
    hummingbirdSprite.setScale(2, 2);

    int level = 1;
   const int numBees = 20;
    int remaining_sprays = 56;
    int maxLives = 2;
    int lives = maxLives;
    int remainingBees = numBees;


    float hummingbird_x = 0;
    float hummingbird_y = resolutionY - 150;  // Start at bottom left corner
    float hummingbird_speed_x = 4000.0f; // Speed for horizontal movement
    float hummingbird_speed_y = -4000.0f; // Speed for vertical movement

    // Timer for hovering of hummingbird
    Clock hoverClock;
    bool isHovering = false;

    const int maxFlowers = 100; //greater size of array is taken so that all all the values would get into array
    float flowerX[maxFlowers];//to store position X of the flowers
    float flowerY[maxFlowers];//to store position Y of the flowers
    int flowerCount = 0; // Keep track of the number of flowers placed

    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;

    //initial position of bullet 
    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;
    Clock bulletClock;


    //initial position of flower
   //float flower_x = player_x;
  // float flower_y = player_y;



    // Ground Y position
    float groundY = resolutionY - 140;//rough idea about the height of ground(doing groundY-32-64 isn't giving desired results)
    int remainingSprays = 56;
    //maximum number of honeycombs that can exist in level 1
   const int maxHoneycombs = 23;
    //arrays for storing the positions of the honeycombs
    float honeycombX[maxHoneycombs], honeycombY[maxHoneycombs];
    bool honeycombActive[maxHoneycombs];
    int honeycombCount = 0;
    int pghoneycomb = 3;//pre-generated honeycombs

    for (int i = 0; i < pghoneycomb; i++) {
        honeycombX[honeycombCount] = rand() % (resolutionX - 32);  //Random X position
        honeycombY[honeycombCount] = rand() % ((resolutionY - 32) / 2);//Random Y position
        honeycombActive[honeycombCount] = true;//checking if the honeycomb is existing on the screen or not
        honeycombCount++;
    }
    Text spraysText;//object to display the number of sprays left
    Font font;
    if (!font.loadFromFile("font/arial.ttf"))
    {
        cout << "Error loading font!" << endl;
        return; // Exit if font cannot be loaded
    }
    spraysText.setFont(font);
    spraysText.setCharacterSize(24);
    spraysText.setFillColor(Color::White);
    spraysText.setPosition(650.0f, resolutionY - 40.0f);//positioning it at the bottom of the window
    spraysText.setStyle(Text::Bold);
    //object for displaying the score of the player
    Text scoreText;
    if (!font.loadFromFile("font/arial.ttf"))
    {
        cout << "Error loading font!" << endl;
        return; // Exit if font cannot be loaded
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(300.0f, resolutionY - 40.0f);
    scoreText.setStyle(Text::Bold);
    //the number of maximum levels	
    int maxLevels = 3;
    //arrays for storing the bee positions and speed
    float beeX[numBees], beeY[numBees], beeSpeedX[numBees], beeSpeedY[numBees];
    // setting directions for the movement of bees:
    for (int i = 0; i < numBees; i++) {
        if (i < numBees / 2) {
            // Bees starting from left side
            beeX[i] = i * (boxPixelsX + 10); // Space bees horizontally
            beeY[i] = 60;
            beeSpeedX[i] = 120.0f; // bees move to the right
        }
        else {
            // Bees starting from right side
            beeX[i] = resolutionX - (i - numBees / 2) * (boxPixelsX + 10); // Space bees horizontally 
            beeY[i] = 60; // 
            beeSpeedX[i] = -120.0f; // bees move to left
        }
        beeSpeedY[i] = 10.0f; //vertical speed
    }

    RectangleShape groundRectangle(Vector2f(resolutionX, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);

    Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        // shooting bullet
        if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists) {
            //intial position of bullet
            bullet_x = player_x;
            bullet_y = player_y;
            bullet_exists = true;
            bulletClock.restart();
            if (remaining_sprays > 0) {  // if sprays are available, then upon shooting they decrease
                remaining_sprays--;
            }
        }

        // Update bullet position
        if (bullet_exists) {
            moveBullet(bullet_y, bullet_exists, bulletClock, remaining_sprays, spraysText);//function for the upward movement of bullet
            for (int i = 0; i < numBees; i++) {
                if (bulletAndBeeCollision(bullet_x, bullet_y, beeX[i], beeY[i], boxPixelsX)) {
                    --remainingBees;// the number of bees decreases as the bullet hits a bee 

                    score += 100;//score earned 
                    // Add honeycomb at bee position
                    if (honeycombCount < maxHoneycombs) {
                        honeycombX[honeycombCount] = beeX[i];//assigning coordinates of bee to honeycomb
                        honeycombY[honeycombCount] = beeY[i];
                        honeycombActive[honeycombCount] = true;
                        honeycombCount++;
                    }
                    //setting coordinates of bee off-screen as the bee vanishes after being hit
                    beeX[i] = -1000;
                    beeY[i] = -1000;
                    bullet_exists = false;
                    break;

                }
            }
        }
        // collision of honeycomb and bullet
        if (bullet_exists) {
            for (int i = 0; i < honeycombCount; i++) {
                if (honeycombActive[i] && bulletAndHoneycombCollision(bullet_x, bullet_y, honeycombX[i], honeycombY[i], boxPixelsX)) {
                    //assigning off-screen coordinates to honeycomb as it also vanishes upon being hit
                    honeycombX[i] = -1000;
                    honeycombY[i] = -1000;
                    honeycombActive[i] = false;
                    bullet_exists = false;
                    break;
                }
            }
        }
        // Moving the hummingbird
        moveHummingbird(deltaTime, hummingbird_x, hummingbird_y, hummingbird_speed_x, hummingbird_speed_y, isHovering, hoverClock);

        //checking if it collides with the honeycomb
        for (int i = 0; i < honeycombCount; i++) {
            if (checkHummingbirdHoneycombCollision(hummingbird_x, hummingbird_y, honeycombX[i], honeycombY[i])) {
                //the humming bird eats the honeycomb, so sending it off-screen
                honeycombX[i] = -1000;
                honeycombY[i] = -1000;
                score += 1000;
            }
        }

        //Update and display score section
        scoreText.setString("SCORE: " + to_string(score));   // Update the score text
        spraysText.setString("Remaining Sprays: " + to_string(remaining_sprays)); // Update the number of remaining text

        movePlayer(player_x, player_y, deltaTime);
        moveBees(beeX, beeY, beeSpeedX, beeSpeedY, deltaTime, numBees);

        beeAndCombCollision(beeX, beeY, numBees, honeycombX, honeycombY, honeycombActive, maxHoneycombs, beeSpeedX);
        //checking if the honeybee reaches the ground
        bool collisionOccurred = beeAndGroundCollision(window, beeX, beeY, numBees, flowerX, flowerY, flowerCount, groundY, flowerSprite);
        //condition for the completion of a level
        bool islevelComplete = true;
        for (int i = 0; i < numBees; i++)
        {
            if (beeX[i] >= 0 && beeX[i] <= resolutionX && beeY[i] >= 0 && beeY[i] <= resolutionY) //if the bees are still present on the screen
            {
                islevelComplete = false;
            }
        }

        if (score > load_high_score()) {// if the score of the player is the greater than the highscore, it becomes the new highscore
            save_high_score(score);//updating the highscore
        }
        if (islevelComplete) {//level 1 ends
            bgMusic.stop();

            Text levelupText("Level " + to_string(level) + "Complete!!", font, 40);//displaying completion message
            levelupText.setFillColor(Color::Red);
            levelupText.setPosition(resolutionX / 2 - 150, resolutionY / 2 - 30);
            window.draw(levelupText);
            window.display();
            sleep(seconds(3));//displaying the message for 3 seconds
            window.close();
            level2();//transitioning to level 2
        }
        window.clear();
        window.draw(background);
        drawPlayer(window, player_x, player_y, playerSprite);
        drawBees(window, beeSprite, beeX, beeY, numBees, beeSpeedX);

        if (bullet_exists)
            drawBullet(window, bullet_x, bullet_y, bulletSprite);

        // Draw all active honeycombs
        for (int i = 0; i < honeycombCount; i++) {
            if (honeycombActive[i]) {
                drawHoneycomb(window, honeycombSprite, honeycombX[i], honeycombY[i]);
            }
        }
        window.draw(groundRectangle);
        drawHummingbird(window, hummingbirdSprite, hummingbird_x, hummingbird_y);
        drawFlowers(window, flowerSprite, flowerX, flowerY, flowerCount);
        window.draw(spraysText);
        window.draw(scoreText);
        drawLives(window, playerSprite, lives, remaining_sprays);//the remaining lives of the player
        window.display();
    }
}
void level2()//function of level 2
{
    srand(time(0));
    int score = 0;
    RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers-Level 2", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(500, 200));
    Texture backgroundTexture;
    Sprite background;
    if (!backgroundTexture.loadFromFile("Textures/backgroundlvl2.jpg"))
    {
        cout << "Error loading background texture!" << endl;
        return; // return if texture cannot be loaded
    }
    background.setTexture(backgroundTexture);
    background.setPosition(0, 0);
    background.setScale(1.5f, 1.65f);

    // Background Music
    Music bgMusic;
    if (!bgMusic.openFromFile("Music/risk.ogg")) {
        cout << "Error: Could not load music file!" << endl;
    }
    bgMusic.setVolume(50);
    bgMusic.setLoop(true);
    bgMusic.play();
    // Loading textures
    Texture playerTexture, beeTexture, bulletTexture, honeycombTexture, flowerTexture, killerbeeTexture, redhoneycombTexture;
    Sprite playerSprite, beeSprite, bulletSprite, honeycombSprite, flowerSprite, killerbeeSprite, redhoneycombSprite;
    //error message in case of failure of loading
    if (!playerTexture.loadFromFile("Textures/spray.png") ||
        !beeTexture.loadFromFile("Textures/Regular_bee.png") ||
        !bulletTexture.loadFromFile("Textures/bullet.png") ||
        !honeycombTexture.loadFromFile("Textures/honeycomb.png") ||
        !flowerTexture.loadFromFile("Textures/obstacles.png") ||
        !redhoneycombTexture.loadFromFile("Textures/honeycomb_red.png") ||
        !killerbeeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error loading textures!" << endl;
        return;
    }

    playerSprite.setTexture(playerTexture);
    beeSprite.setTexture(beeTexture);
    bulletSprite.setTexture(bulletTexture);
    redhoneycombSprite.setTexture(redhoneycombTexture);
    honeycombSprite.setTexture(honeycombTexture);
    flowerSprite.setTexture(flowerTexture);
    flowerSprite.setScale(3, 3);
    bulletSprite.setScale(3, 3);
    killerbeeSprite.setTexture(killerbeeTexture);

    Texture hummingbirdTexture;
    if (!hummingbirdTexture.loadFromFile("Textures/hummingbird.png")) {
        // Handle error
        return;
    }
    Sprite hummingbirdSprite(hummingbirdTexture);
    hummingbirdSprite.setScale(2, 2);

    int level = 2;
    const int numBees = 15;
    int remaining_sprays = 56;
    int maxLives = 2;
    int lives = maxLives;
    int remainingBees = numBees;
   const int killerBees = 5;

    //as the initial position of the humming bird is the bottom left corner
    float hummingbird_x = 0;
    float hummingbird_y = resolutionY - 150;
    float hummingbird_speed_x = 4000.0f;
    float hummingbird_speed_y = -4000.0f;

    // Timer for hovering
    Clock hoverClock;
    bool isHovering = false;

    const int maxFlowers = 100;
    float flowerX[maxFlowers];
    float flowerY[maxFlowers];
    int flowerCount = 0; // for keeping track of the number of flowers placed

    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;

    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;
    Clock bulletClock;

    float flower_x = player_x;
    float flower_y = player_y;

    // Ground Y position
    float groundY = resolutionY - 140;
    int remainingSprays = 56;
    const int maxHoneycombs = 29;//15 worker bees, 5 killer bees ,9 pre-generated honeycombs
    // Honeycomb positions
   const int maxredhoneycombs = 5;
    //arrays for storing the coordinates of the red honey combs and the yellow honeycombs
    float redhoneycombX[maxredhoneycombs], redhoneycombY[maxredhoneycombs];
    float honeycombX[maxHoneycombs], honeycombY[maxHoneycombs];
    //for checking if the honeycomb is on the screen or not
    bool redhoneycombActive[maxredhoneycombs];
    bool honeycombActive[maxHoneycombs];
    int redhoneycombCount = 0;
    int honeycombCount = 0;
    int pghoneycomb = 9;//pre-generated honeycombs

    //drawing the pre-generated honeycombs
    for (int i = 0; i < pghoneycomb; i++) {
        honeycombX[honeycombCount] = rand() % (resolutionX - 32);   //Randomly generating X position
        honeycombY[honeycombCount] = rand() % ((resolutionY - 32) / 2);//Randomly generating Y position
        honeycombActive[honeycombCount] = true;
        honeycombCount++;
    }
    Text spraysText;
    Font font;
    if (!font.loadFromFile("font/arial.ttf"))
    {
        cout << "Error loading font!" << endl;
        return; // return if font cannot be loaded
    }
    //text for displaying remaining sprays
    spraysText.setFont(font);
    spraysText.setCharacterSize(24);
    spraysText.setFillColor(Color::White);
    spraysText.setPosition(650.0f, resolutionY - 40.0f);
    spraysText.setStyle(Text::Bold);

    Text scoreText;
    if (!font.loadFromFile("font/arial.ttf"))
    {
        cout << "Error loading font!" << endl;
        return; // return if font cannot be loaded
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(300.0f, resolutionY - 40.0f);
    scoreText.setStyle(Text::Bold);
    int maxLevels = 3;
    //arrays for storing the coordinates and speeds of bees
    float beeX[numBees], beeY[numBees], beeSpeedX[numBees], beeSpeedY[numBees], killerBeeX[killerBees], killerBeeY[killerBees], killerSpeedX[killerBees], killerSpeedY[killerBees];
    //setting direcrtion of movement for workerbees:
    for (int i = 0; i < numBees; i++) {
        if (i < numBees / 2) {
            // Bees starting from left side
            beeX[i] = i * (boxPixelsX + 10);
            beeY[i] = 60;
            beeSpeedX[i] = 140.0f;
        }
        else {
            // Bees starting from right side
            beeX[i] = resolutionX - (i - numBees / 2) * (boxPixelsX + 10);
            beeY[i] = 60;
            beeSpeedX[i] = -140.0f;
        }
        beeSpeedY[i] = 15.0f;
    }
    //setting direction of movement for the killer bees
    for (int i = 0; i < killerBees; i++) {
        if (i < killerBees / 2) {
            // Bees starting from left side
            killerBeeX[i] = i * (boxPixelsX + 10);
            killerBeeY[i] = 90;
            killerSpeedX[i] = 180.0f;
        }
        else {
            // Bees starting from right side
            killerBeeX[i] = resolutionX - (i - killerBees / 2) * (boxPixelsX + 10);
            killerBeeY[i] = 90;
            killerSpeedX[i] = -180.0f;
        }
        killerSpeedY[i] = 20.0f;
    }
    RectangleShape groundRectangle(Vector2f(resolutionX, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);

    Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }
        // for shooting of bullet
        if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists) {
            bullet_x = player_x;
            bullet_y = player_y;
            bullet_exists = true;
            bulletClock.restart();
            if (remaining_sprays > 0) {  // Decrementing incase of availale sprays
                remaining_sprays--;
            }
        }
        // Update bullet coordinates
        if (bullet_exists) {
            moveBullet(bullet_y, bullet_exists, bulletClock, remaining_sprays, spraysText);
            for (int i = 0; i < numBees; i++) {
                if (bulletAndBeeCollision(bullet_x, bullet_y, beeX[i], beeY[i], boxPixelsX)) {
                    --remainingBees;

                    score += 100;
                    // placing honeycomb at bee position
                    if (honeycombCount < maxHoneycombs) {
                        honeycombX[honeycombCount] = beeX[i];
                        honeycombY[honeycombCount] = beeY[i];
                        honeycombActive[honeycombCount] = true;
                        honeycombCount++;
                    }
                    //sending bee off-screen
                    beeX[i] = -1000;
                    beeY[i] = -1000;
                    bullet_exists = false;
                    break;
                }
            }
        }
        if (bullet_exists)
        {
            for (int i = 0; i < killerBees; i++) {
                if (bulletAndBeeCollision(bullet_x, bullet_y, killerBeeX[i], killerBeeY[i], boxPixelsX)) {
                    --remainingBees;

                    score += 1000;
                    // Add honeycomb at bee position
                    if (redhoneycombCount < maxredhoneycombs) {
                        redhoneycombX[redhoneycombCount] = killerBeeX[i];//placing red honeycomb at the position of killer bee after it is killed 
                        redhoneycombY[redhoneycombCount] = killerBeeY[i];
                        redhoneycombActive[redhoneycombCount] = true;
                        redhoneycombCount++;
                    }
                    //sending killer bee off-screen
                    killerBeeX[i] = -1000;
                    killerBeeY[i] = -1000;
                    bullet_exists = false;
                    break;
                }
            }
        }
        // for collision between honeycomb and bullet i.e. bullet hitting honeycomb
        if (bullet_exists) {
            for (int i = 0; i < honeycombCount; i++) {
                if (honeycombActive[i] && bulletAndHoneycombCollision(bullet_x, bullet_y, honeycombX[i], honeycombY[i], boxPixelsX)) {
                    honeycombX[i] = -1000;
                    honeycombY[i] = -1000;
                    honeycombActive[i] = false;
                    bullet_exists = false;
                    break;
                }
            }
        }
        if (bullet_exists) {
            for (int i = 0; i < redhoneycombCount; i++) {
                if (redhoneycombActive[i] && bulletAndHoneycombCollision(bullet_x, bullet_y, redhoneycombX[i], redhoneycombY[i], boxPixelsX)) {
                    redhoneycombX[i] = -1000;
                    redhoneycombY[i] = -1000;
                    redhoneycombActive[i] = false;
                    bullet_exists = false;
                    break;
                }
            }
        }

        // Movement of hummingbird
        moveHummingbird(deltaTime, hummingbird_x, hummingbird_y, hummingbird_speed_x, hummingbird_speed_y, isHovering, hoverClock);

        // Check for collision with yellowhoneycomb
        for (int i = 0; i < honeycombCount; i++) {
            if (checkHummingbirdHoneycombCollision(hummingbird_x, hummingbird_y, honeycombX[i], honeycombY[i])) {
                // the honeycomb vanishes
                honeycombX[i] = -1000;
                honeycombY[i] = -1000;
                score += 1000;
            }
        }
        // Check for collision with redhoneycomb
        for (int i = 0; i < redhoneycombCount; i++) {
            if (checkHummingbirdHoneycombCollision(hummingbird_x, hummingbird_y, redhoneycombX[i], redhoneycombY[i])) {
                //redhoneycomb is hit
                redhoneycombX[i] = -1000;
                redhoneycombY[i] = -1000;
                score += 2000;
            }
        }

        //Update and display score section
        scoreText.setString("SCORE: " + to_string(score)); // Update the score text
        spraysText.setString("Remaining Sprays: " + to_string(remaining_sprays)); // Update the text for remaining sprays

        movePlayer(player_x, player_y, deltaTime);
        moveBees(beeX, beeY, beeSpeedX, beeSpeedY, deltaTime, numBees);
        moveBees(killerBeeX, killerBeeY, killerSpeedX, killerSpeedY, deltaTime, killerBees);
        beeAndCombCollision(beeX, beeY, numBees, honeycombX, honeycombY, honeycombActive, maxHoneycombs, beeSpeedX);
        beeAndCombCollision(beeX, beeY, numBees, redhoneycombX, redhoneycombY, redhoneycombActive, maxredhoneycombs, killerSpeedX);
        bool islevelComplete = true;

        // Check if any regular bee is on the screen
        for (int i = 0; i < numBees; i++) {
            if (beeX[i] >= 0 && beeX[i] <= resolutionX && beeY[i] >= 0 && beeY[i] <= resolutionY) {
                islevelComplete = false;
                break;  // Exit the loop if any bee is found on the screen
            }
        }

        // Check if any killer bee is on the screen
        for (int j = 0; j < killerBees; j++) {
            if (killerBeeX[j] >= 0 && killerBeeX[j] <= resolutionX && killerBeeY[j] >= 0 && killerBeeY[j] <= resolutionY) {
                islevelComplete = false;
                break;  // Exit the loop if  any killer bee is found on the screen
            }
        }
        bool collisionOccurred = beeAndGroundCollision(window, beeX, beeY, numBees, flowerX, flowerY, flowerCount, groundY, flowerSprite);
        if (score > load_high_score()) {
            save_high_score(score);
        }
        if (islevelComplete) {
            bgMusic.stop();

            Text levelupText("Level " + to_string(level) + " Complete!!", font, 40);
            levelupText.setFillColor(Color::Red);
            levelupText.setPosition(resolutionX / 2 - 150, resolutionY / 2 - 30);
            window.draw(levelupText);
            window.display();
            sleep(seconds(3));
            window.close();
            level3();//calling level 3 when level 2 is completed
        }

        // Draw everything
        window.clear();
        window.draw(background);
        drawPlayer(window, player_x, player_y, playerSprite);
        drawBees(window, beeSprite, beeX, beeY, numBees, beeSpeedX);
        drawBees(window, killerbeeSprite, killerBeeX, killerBeeY, killerBees, killerSpeedX);
        if (bullet_exists)
            drawBullet(window, bullet_x, bullet_y, bulletSprite);
        // Draw all active honeycombs
        for (int i = 0; i < honeycombCount; i++) {
            if (honeycombActive[i]) {
                drawHoneycomb(window, honeycombSprite, honeycombX[i], honeycombY[i]);
            }
        }
        for (int i = 0; i < redhoneycombCount; i++) {
            if (redhoneycombActive[i]) {
                drawHoneycomb(window, redhoneycombSprite, redhoneycombX[i], redhoneycombY[i]);
            }
        }
        window.draw(groundRectangle);
        drawHummingbird(window, hummingbirdSprite, hummingbird_x, hummingbird_y);
        drawFlowers(window, flowerSprite, flowerX, flowerY, flowerCount);
        window.draw(spraysText);
        window.draw(scoreText);
        drawLives(window, playerSprite, lives, remaining_sprays);
        window.display();
    }
}

void level3()
{
    srand(time(0));
    int score = 0;

    RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers-Level 3", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(500, 200));
    Texture backgroundTexture;
    Sprite background;
    if (!backgroundTexture.loadFromFile("Textures/background2.jpg"))
    {
        cout << "Error loading background texture!" << endl;
        return; // Exit if texture cannot be loaded
    }
    background.setTexture(backgroundTexture);
    background.setPosition(0, 0);
    background.setScale(1.2f, 1.5f);
    // Background Music
    Music bgMusic;
    if (!bgMusic.openFromFile("Music/magical_journey.ogg")) {
        cout << "Error: Could not load music file!" << endl;
    }
    bgMusic.setVolume(50);
    bgMusic.setLoop(true);
    bgMusic.play();
    // Load textures
    Texture playerTexture, beeTexture, bulletTexture, honeycombTexture, flowerTexture, killerbeeTexture, redhoneycombTexture;
    Sprite playerSprite, beeSprite, bulletSprite, honeycombSprite, flowerSprite, killerbeeSprite, redhoneycombSprite;

    if (!playerTexture.loadFromFile("Textures/spray.png") ||
        !beeTexture.loadFromFile("Textures/Regular_bee.png") ||
        !bulletTexture.loadFromFile("Textures/bullet.png") ||
        !honeycombTexture.loadFromFile("Textures/honeycomb.png") ||
        !flowerTexture.loadFromFile("Textures/obstacles.png") ||
        !redhoneycombTexture.loadFromFile("Textures/honeycomb_red.png") ||
        !killerbeeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error loading textures!" << endl;
        return;
    }
    playerSprite.setTexture(playerTexture);
    beeSprite.setTexture(beeTexture);
    bulletSprite.setTexture(bulletTexture);
    honeycombSprite.setTexture(honeycombTexture);
    flowerSprite.setTexture(flowerTexture);
    redhoneycombSprite.setTexture(redhoneycombTexture);
    killerbeeSprite.setTexture(killerbeeTexture);
    bulletSprite.setScale(3, 3);
    Texture hummingbirdTexture;
    if (!hummingbirdTexture.loadFromFile("Textures/hummingbird.png")) {
        // Handle error
        return;
    }
    Sprite hummingbirdSprite(hummingbirdTexture);
    hummingbirdSprite.setScale(2, 2);
    int level = 3;
    const int numBees = 20;
    int remaining_sprays = 56;
    int maxLives = 2;
    int lives = maxLives;
    int remainingBees = numBees;
   const int killerBees = 10;
    float hummingbird_x = 0;
    float hummingbird_y = resolutionY - 150;  // Start at bottom left corner
    float hummingbird_speed_x = 4000.0f; // Speed for horizontal movement
    float hummingbird_speed_y = -4000.0f; // Speed for vertical movement

    // Timer for hovering
    Clock hoverClock;
    bool isHovering = false;
    const int maxFlowers = 100;
    float flowerX[maxFlowers];
    float flowerY[maxFlowers];
    int flowerCount = 0;

    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;

    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;
    Clock bulletClock;

    float flower_x = player_x;
    float flower_y = player_y;
    // Ground Y position
    float groundY = resolutionY - 140;
    int remainingSprays = 56;
    const int maxredhoneycombs = 10;
    const int maxHoneycombs = 45;//20 worker bees,10 killer bees and 15 pre-generated combs
    // red and yellow Honeycomb positions
    float honeycombX[maxHoneycombs], honeycombY[maxHoneycombs];
    bool honeycombActive[maxHoneycombs];
    int honeycombCount = 0;
    float redhoneycombX[maxredhoneycombs], redhoneycombY[maxredhoneycombs];
    bool redhoneycombActive[maxredhoneycombs];
    int redhoneycombCount = 0;
    int pghoneycomb = 15;//number of pre-generated honeycombs
    //generating the pre-generated honeycombs
    for (int i = 0; i < pghoneycomb; i++) {
        honeycombX[honeycombCount] = rand() % (resolutionX - 32);
        honeycombY[honeycombCount] = rand() % ((resolutionY - 32) / 2);
        honeycombActive[honeycombCount] = true;
        honeycombCount++;
    }
    Text spraysText;
    Font font;
    if (!font.loadFromFile("font/arial.ttf"))
    {
        cout << "Error loading font!" << endl;
        return; // Exit if font cannot be loaded
    }
    spraysText.setFont(font);
    spraysText.setCharacterSize(24);
    spraysText.setFillColor(Color::White);
    spraysText.setPosition(650.0f, resolutionY - 40.0f);
    spraysText.setStyle(Text::Bold);
    Text scoreText;
    if (!font.loadFromFile("font/arial.ttf"))
    {
        cout << "Error loading font!" << endl;
        return;
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(300.0f, resolutionY - 40.0f);
    scoreText.setStyle(Text::Bold);
    //arrays for storing the positions of killer and worker bees
    float beeX[numBees], beeY[numBees], beeSpeedX[numBees], beeSpeedY[numBees], killerbeeX[killerBees], killerbeeY[killerBees], killerSpeedX[killerBees], killerSpeedY[killerBees];
    //setting directions for the movement of bees
    for (int i = 0; i < numBees; i++) {
        if (i < numBees / 2) {
            // Bees starting from left 
            beeX[i] = i * (boxPixelsX + 10);
            beeY[i] = 50;
            beeSpeedX[i] = 160.0f;
        }
        else {
            // Bees starting from right 
            beeX[i] = resolutionX - (i - numBees / 2) * (boxPixelsX + 10);
            beeY[i] = 50;
            beeSpeedX[i] = -160.0f;
        }
        beeSpeedY[i] = 15.0f;
    }
    //similar for the killer bees
    for (int i = 0; i < killerBees; i++) {
        if (i < killerBees / 2) {
            killerbeeX[i] = i * (boxPixelsX + 10);
            killerbeeY[i] = 70;
            killerSpeedX[i] = 210.0f;
        }
        else {
            killerbeeX[i] = resolutionX - (i - killerBees / 2) * (boxPixelsX + 10);
            killerbeeY[i] = 70;
            killerSpeedX[i] = -210.0f;
        }
        killerSpeedY[i] = 25.0f;
    }
    RectangleShape groundRectangle(Vector2f(resolutionX, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);
    Clock clock;

    while (window.isOpen()) {//game loop
        float deltaTime = clock.restart().asSeconds();
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists) {
            bullet_x = player_x;
            bullet_y = player_y;
            bullet_exists = true;
            bulletClock.restart();
            if (remaining_sprays > 0) {
                remaining_sprays--;
            }
        }
        if (bullet_exists) {
            moveBullet(bullet_y, bullet_exists, bulletClock, remaining_sprays, spraysText);
            for (int i = 0; i < numBees; i++) {
                if (bulletAndBeeCollision(bullet_x, bullet_y, beeX[i], beeY[i], boxPixelsX)) {
                    --remainingBees;
                    score += 100;
                    // Add honeycomb at bee position
                    if (honeycombCount < maxHoneycombs) {
                        honeycombX[honeycombCount] = beeX[i];
                        honeycombY[honeycombCount] = beeY[i];
                        honeycombActive[honeycombCount] = true;
                        honeycombCount++;
                    }
                    beeX[i] = -1000;
                    beeY[i] = -1000;
                    bullet_exists = false;
                    break;
                }
            }
        }

        if (bullet_exists) {
            moveBullet(bullet_y, bullet_exists, bulletClock, remaining_sprays, spraysText);
            for (int i = 0; i < killerBees; i++) {
                if (bulletAndBeeCollision(bullet_x, bullet_y, killerbeeX[i], killerbeeY[i], boxPixelsX)) {
                    --remainingBees;
                    score += 1000;
                    // put honeycomb at bee position
                    if (redhoneycombCount < maxredhoneycombs) {
                        redhoneycombX[redhoneycombCount] = killerbeeX[i];
                        redhoneycombY[redhoneycombCount] = killerbeeY[i];
                        redhoneycombActive[redhoneycombCount] = true;
                        redhoneycombCount++;
                    }
                    killerbeeX[i] = -1000;
                    killerbeeY[i] = -1000;
                    bullet_exists = false;
                    break;
                }
            }
        }
        if (bullet_exists) {
            for (int i = 0; i < honeycombCount; i++) {
                if (honeycombActive[i] && bulletAndHoneycombCollision(bullet_x, bullet_y, honeycombX[i], honeycombY[i], boxPixelsX)) {
                    honeycombX[i] = -1000;
                    honeycombY[i] = -1000;
                    honeycombActive[i] = false;
                    bullet_exists = false;
                    break;
                }
            }
        }
        if (bullet_exists) {
            for (int i = 0; i < redhoneycombCount; i++) {
                if (redhoneycombActive[i] && bulletAndHoneycombCollision(bullet_x, bullet_y, redhoneycombX[i], redhoneycombY[i], boxPixelsX)) {
                    redhoneycombX[i] = -1000;
                    redhoneycombY[i] = -1000;
                    redhoneycombActive[i] = false;
                    bullet_exists = false;
                    break;
                }
            }
        }


        moveHummingbird(deltaTime, hummingbird_x, hummingbird_y, hummingbird_speed_x, hummingbird_speed_y, isHovering, hoverClock);
        // Check for collision with honeycomb
        for (int i = 0; i < honeycombCount; i++) {
            if (checkHummingbirdHoneycombCollision(hummingbird_x, hummingbird_y, honeycombX[i], honeycombY[i])) {

                honeycombX[i] = -1000;
                honeycombY[i] = -1000;
                score += 1000;
            }
        }
        for (int i = 0; i < redhoneycombCount; i++) {
            if (checkHummingbirdHoneycombCollision(hummingbird_x, hummingbird_y, redhoneycombX[i], redhoneycombY[i])) {
                redhoneycombX[i] = -1000;
                redhoneycombY[i] = -1000;
                score += 2000;
            }
        }
        scoreText.setString("SCORE: " + to_string(score)); // Update the score text
        spraysText.setString("Remaining Sprays: " + to_string(remaining_sprays)); // Update the remaining sprays text
        movePlayer(player_x, player_y, deltaTime);
        moveBees(beeX, beeY, beeSpeedX, beeSpeedY, deltaTime, numBees);
        moveBees(killerbeeX, killerbeeY, killerSpeedX, killerSpeedY, deltaTime, killerBees);
        beeAndCombCollision(beeX, beeY, numBees, honeycombX, honeycombY, honeycombActive, maxHoneycombs, beeSpeedX);
        beeAndCombCollision(beeX, beeY, numBees, redhoneycombX, redhoneycombY, redhoneycombActive, maxredhoneycombs, killerSpeedX);
        bool islevelComplete = true;

        // Check if any regular bee is on the screen
        for (int i = 0; i < numBees; i++) {
            if (beeX[i] >= 0 && beeX[i] <= resolutionX && beeY[i] >= 0 && beeY[i] <= resolutionY) {
                islevelComplete = false;
                break;  // Exit the loop if any bee is found on the screen
            }
        }

        // Check if any killer bee is on the screen
        for (int j = 0; j < killerBees; j++) {
            if (killerbeeX[j] >= 0 && killerbeeX[j] <= resolutionX && killerbeeY[j] >= 0 && killerbeeY[j] <= resolutionY) {
                islevelComplete = false;
                break;  // Exit the loop if  any killer bee is found on the screen
            }
        }
        bool collisionOccurred = beeAndGroundCollision(window, beeX, beeY, numBees, flowerX, flowerY, flowerCount, groundY, flowerSprite);
        if (score > load_high_score()) {
            save_high_score(score);
        }
        if (islevelComplete) {
            bgMusic.stop();
            Text levelupText("Level " + to_string(level) + " Complete!!\n You have completed all the levels!", font, 40);
            levelupText.setFillColor(Color::Red);
            levelupText.setPosition(resolutionX / 2 - 230, resolutionY / 2 - 30);
            window.draw(levelupText);
            window.display();
            sleep(seconds(3));
            window.close();
            display_menu(score);
        }

        window.clear();
        window.draw(background);
        drawPlayer(window, player_x, player_y, playerSprite);
        drawBees(window, beeSprite, beeX, beeY, numBees, beeSpeedX);
        drawBees(window, killerbeeSprite, killerbeeX, killerbeeY, killerBees, killerSpeedX);
        if (bullet_exists)
            drawBullet(window, bullet_x, bullet_y, bulletSprite);

        for (int i = 0; i < honeycombCount; i++) {
            if (honeycombActive[i]) {
                drawHoneycomb(window, honeycombSprite, honeycombX[i], honeycombY[i]);
            }
        }
        for (int i = 0; i < redhoneycombCount; i++) {
            if (redhoneycombActive[i]) {
                drawHoneycomb(window, redhoneycombSprite, redhoneycombX[i], redhoneycombY[i]);
            }
        }
        window.draw(groundRectangle);
        drawHummingbird(window, hummingbirdSprite, hummingbird_x, hummingbird_y);
        drawFlowers(window, flowerSprite, flowerX, flowerY, flowerCount);
        window.draw(spraysText);
        window.draw(scoreText);
        drawLives(window, playerSprite, lives, remaining_sprays);
        window.display();
        if (lives <= 0)
        {
            Text gameOverText("Game Over!", font, 60);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setPosition(resolutionX / 2 - 150, resolutionY / 2);
            window.draw(gameOverText);
            window.display();
            sleep(seconds(3));
            window.close();
        }
    }
}
//function to control the movement of bullet
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock, int& remaining_sprays, Text& spraysText) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;
    bulletClock.restart();
    bullet_y -= 10;//bullet moving upward
    if (bullet_y < -32)
        bullet_exists = false;
}
//function to draw Bullet 
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet_x, bullet_y);
    window.draw(bulletSprite);
}

void moveBees(float beeX[], float beeY[], float beeSpeedX[], float beeSpeedY[], float deltaTime, int numBees) {
    for (int i = 0; i < numBees; i++) {
        beeX[i] += beeSpeedX[i] * deltaTime; // distance = speed * time

        if (beeX[i] < 0 || beeX[i] > resolutionX - boxPixelsX) { // edge cases
            beeY[i] += boxPixelsY; // moving one row down
            beeSpeedX[i] = -beeSpeedX[i]; // change direction

            // Keep inside window
            if (beeX[i] < 0) beeX[i] = 0;
            if (beeX[i] > resolutionX - boxPixelsX) beeX[i] = resolutionX - boxPixelsX;
        }
    }
}


void drawBees(RenderWindow& window, Sprite& beeSprite, float beeX[], float beeY[], int numBees, float beeSpeedX[]) {
    for (int i = 0; i < numBees; i++) {
        beeSprite.setPosition(beeX[i], beeY[i]);

        // Face right if beeSpeedX > 0, left if < 0
        if (beeSpeedX[i] > 0) {
            beeSprite.setScale(-1.f, 1.f);
            beeSprite.setOrigin(0.f, 0.f);
        }
        else {
            beeSprite.setScale(1.f, 1.f);
            beeSprite.setOrigin(beeSprite.getLocalBounds().width, 0.f);
        }

        window.draw(beeSprite);
    }
}

//function to draw player
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) {
    playerSprite.setPosition(player_x, player_y);
    window.draw(playerSprite);
}
//function to control horizontal movement of spray can
void movePlayer(float& player_x, float& player_y, float deltaTime) {
    //assigning keys for movement
    if (Keyboard::isKeyPressed(Keyboard::Left) && player_x > 0) player_x -= 150.0f * deltaTime;
    if (Keyboard::isKeyPressed(Keyboard::Right) && player_x < resolutionX - boxPixelsX) player_x += 150.0f * deltaTime;
}

void drawHoneycomb(RenderWindow& window, Sprite& honeycombSprite, float honeycomb_x, float honeycomb_y) {
    honeycombSprite.setPosition(honeycomb_x, honeycomb_y);
    window.draw(honeycombSprite);
}
//doing collision by the AABB method
bool bulletAndBeeCollision(float bullet_x, float bullet_y, float beeX, float beeY, float boxPixelsX) {
    return bullet_x < beeX + boxPixelsX && bullet_x + boxPixelsX > beeX &&
        bullet_y < beeY + boxPixelsY && bullet_y + boxPixelsY > beeY;
}

bool bulletAndHoneycombCollision(float bullet_x, float bullet_y, float honeycomb_x, float honeycomb_y, float boxPixelsX) {
    return bullet_x < honeycomb_x + boxPixelsX && bullet_x + boxPixelsX > honeycomb_x &&
        bullet_y < honeycomb_y + boxPixelsY && bullet_y + boxPixelsY > honeycomb_y;
}
bool beeAndGroundCollision(RenderWindow& window, float beeX[], float beeY[], int numBees,
    float flowerX[], float flowerY[], int& flowerCount, float groundY, Sprite& flowerSprite) {
    bool collisionOccurred = false;

    for (int i = 0; i < numBees; i++) {
        if (beeY[i] >= groundY) {  // If bee touches the ground
            if (flowerCount == 0) {  // If it's the first bee that touches the ground
                // Create two flowers at the edges of the screen
                flowerX[flowerCount] = 0;               // Flower at the leftmost edge
                flowerY[flowerCount] = groundY;         // Flower at ground level
                flowerCount++;

                flowerX[flowerCount] = resolutionX - boxPixelsX;  // Flower at the rightmost edge
                flowerY[flowerCount] = groundY;         // Flower at ground level
                flowerCount++;
            }
            else {
                // For other bees creating just one flower at the bee's position
                flowerX[flowerCount] = beeX[i];
                flowerY[flowerCount] = groundY;
                flowerCount++;
            }

            // Moving the bee off-screen after collision
            beeX[i] = -1000;
            beeY[i] = -1000;
            collisionOccurred = true;
        }
    }
    return collisionOccurred;
}

// Function to draw flowers on the screen
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, float flowerX[], float flowerY[], int flowerCount) {
    for (int i = 0; i < flowerCount; i++) {
        flowerSprite.setPosition(flowerX[i], flowerY[i]);
        window.draw(flowerSprite);
    }
}
//function to visibly display the number of lives left 
void drawLives(RenderWindow& window, Sprite& playerSprite, int& lives, int& remaining_sprays) {
    float x_Position = 10.0f;
    float y_Position = resolutionY - 68.0f;
    if (remaining_sprays == 0) {
        lives--;//automatically displaying the next can when one runs out of sprays
        remaining_sprays = 56;
    }

    for (int i = 0; i < lives; i++) {
        playerSprite.setPosition(x_Position, y_Position);
        window.draw(playerSprite);
        x_Position += 60.0f;  // Move the next life to the right
    }
}
void moveHummingbird(float deltaTime, float& hummingbird_x, float& hummingbird_y, float& hummingbird_speed_x, float& hummingbird_speed_y, bool& isHovering, Clock& hoverClock) {
    if (isHovering) {
        // Hummingbird is hovering
        if (hoverClock.getElapsedTime().asSeconds() > 2) { // Hover for 2 seconds
            isHovering = false;
            hoverClock.restart();
        }
    }
    else {
        // Random diagonal movement
        hummingbird_x += hummingbird_speed_x * deltaTime;
        hummingbird_y += hummingbird_speed_y * deltaTime;

        // Check boundaries and reverse direction
        if (hummingbird_x < 0 || hummingbird_x > resolutionX - boxPixelsX) {
            hummingbird_speed_x = -hummingbird_speed_x;
        }
        if (hummingbird_y < 0 || hummingbird_y > resolutionY - 150) {
            hummingbird_speed_y = -hummingbird_speed_y;
        }

        // Hover after moving to a new position
        if (rand() % 5 == 0) { // 1 in 5 chance to hover
            isHovering = true;
            hoverClock.restart();
        }
    }
}

// Function to check if hummingbird reaches honeycomb(similar collision logic)
bool checkHummingbirdHoneycombCollision(float hummingbird_x, float hummingbird_y, float honeycomb_x, float honeycomb_y) {
    return hummingbird_x < honeycomb_x + boxPixelsX && hummingbird_x + boxPixelsX > honeycomb_x &&
        hummingbird_y < honeycomb_y + boxPixelsX && hummingbird_y + boxPixelsX > honeycomb_y;
}

// Function to draw the hummingbird
void drawHummingbird(RenderWindow& window, Sprite& hummingbirdSprite, float hummingbird_x, float hummingbird_y) {
    hummingbirdSprite.setPosition(hummingbird_x, hummingbird_y);
    window.draw(hummingbirdSprite);
}
//function to detect collision between worker bee and honeycomb
void beeAndCombCollision(float beeX[], float beeY[], int numBees, float honeycombX[], float honeycombY[], bool honeycombActive[], int maxHoneycombs, float beeSpeedX[])
{
    float honeycombWidth = 64, honeycombHeight = 64;
    float beeWidth = 46, beeHeight = 22;
    for (int i = 0; i < numBees; i++)
    {
        for (int j = 0; j < maxHoneycombs; j++)
        {
            if (honeycombActive[j] &&
                !(beeX[i] + beeWidth < honeycombX[j] || honeycombX[j] + honeycombWidth < beeX[i] ||
                    beeY[i] + beeHeight < honeycombY[j] || honeycombY[j] + honeycombHeight < beeY[i])) {
                beeSpeedX[i] = -beeSpeedX[i];
                beeY[i] += boxPixelsY;
            }
        }
    }
}

int load_high_score() {
    std::ifstream file("highscore.txt");

    // If the file doesn't exist,create it and put the first entry zero
    if (!file.is_open()) {
        std::ofstream outFile("highscore.txt");
        outFile << 0;
        outFile.close();
        return 0;
    }

    int highScore;
    file >> highScore;  // Read the high score
    file.close();
    return highScore;
}

void save_high_score(int score) {
    std::ofstream file("highscore.txt");

    if (!file.is_open()) {
        std::cerr << "Error saving high score to file!" << std::endl;
        return;
    }

    file << score;  // Write the updated high score to the file
    file.close();
}





