#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include<fstream>
#include <vector>


using namespace std;

const int mazeRows = 12;
const int mazeCols = 12;
int const width = 800;
int const height = 800;
int score = -1;
// so that initial score is 0 even though pacman is standing on a cherry
float ghostSpeed = 1.0f;


int getHighScore() {
    ifstream read("highscore.txt");
    int highscore = 0;
    read >> highscore;
    read.close();
    return highscore;
}


void updateHighScore(int newscore) {
    int current = getHighScore();
    if (newscore > current) {
        ofstream write("highscore.txt");
        write << newscore;
        write.close();
    }
}

int padding = 32;

void moveGhost(sf::RectangleShape& ghost, float cellSize, float& ghostSpeed, sf::Vector2f& ghostDirection, int maze[][mazeCols]) {
    sf::Vector2f ghostPrevPosition = ghost.getPosition();
    if (ghostDirection == sf::Vector2f(0.f, 0.f)) {
        // Initialize with a random direction
        int randomDirection = rand() % 4; // 0 up, 1 down, 2 left, 3 right

        switch (randomDirection) {
        case 0:
            ghostDirection = sf::Vector2f(0.f, -ghostSpeed);
            break;
        case 1:
            ghostDirection = sf::Vector2f(0.f, ghostSpeed);
            break;
        case 2:
            ghostDirection = sf::Vector2f(-ghostSpeed, 0.f);
            break;
        case 3:
            ghostDirection = sf::Vector2f(ghostSpeed, 0.f);
            break;
        }
    }

    float speed = 0.5f;
    ghost.move(ghostDirection * speed);

    sf::FloatRect ghostBounds = ghost.getGlobalBounds();

    int ghostTileX = static_cast<int>((ghost.getPosition().x + ghostBounds.width / 2) / cellSize);
    int ghostTileY = static_cast<int>((ghost.getPosition().y + ghostBounds.height / 2) / cellSize);

    if (ghostTileX >= 0 && ghostTileX < mazeCols && ghostTileY >= 0 && ghostTileY < mazeRows) {
        if (maze[ghostTileY][ghostTileX] == 1) {
            // Handle collision with maze wall
            ghost.setPosition(ghostPrevPosition);
            ghostDirection = sf::Vector2f(0.f, 0.f);
        }
    }
    else {
        // Handle out-of-bounds
        ghost.setPosition(ghostPrevPosition);
        ghostDirection = sf::Vector2f(0.f, 0.f);
    }

    // Adjust for boundary collisions
    if (ghost.getPosition().x < padding) {
        ghost.setPosition(padding, ghost.getPosition().y);
        ghostDirection = sf::Vector2f(ghostSpeed, 0.f);
    }
    if (ghost.getPosition().y < padding) {
        ghost.setPosition(ghost.getPosition().x, padding);
        ghostDirection = sf::Vector2f(0.f, ghostSpeed);
    }
    if (ghost.getPosition().x + ghostBounds.width > (mazeCols * cellSize) - padding) {
        ghost.setPosition((mazeCols * cellSize) - padding - ghostBounds.width, ghost.getPosition().y);
        ghostDirection = sf::Vector2f(-ghostSpeed, 0.f);
    }
    if (ghost.getPosition().y + ghostBounds.height > (mazeRows * cellSize) - padding) {
        ghost.setPosition(ghost.getPosition().x, (mazeRows * cellSize) - padding - ghostBounds.height);
        ghostDirection = sf::Vector2f(0.f, -ghostSpeed);
    }
}



int main() {



    sf::RenderWindow windowStart(sf::VideoMode(width, height), "Game-Start");
    windowStart.setFramerateLimit(120);

    sf::Texture textureSpace1;
    if (!textureSpace1.loadFromFile("welcome.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape startspace(sf::Vector2f(width, height));
    startspace.setTexture(&textureSpace1);

    sf::Texture textureStart;
    if (!textureStart.loadFromFile("yellowstart.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape shape(sf::Vector2f(500, 500));
    shape.setTexture(&textureStart);
    shape.setPosition(155, 300);


    sf::SoundBuffer Buffer;


    if (!Buffer.loadFromFile("begsound.wav")) {
        // Print an error message along with the file path
        cout << "Error: Could not load sound file: " << endl;
        return 0;
    }

    sf::Sound beg;
    beg.setBuffer(Buffer);
    beg.play();

    while (windowStart.isOpen()) {
        sf::Event event;
        while (windowStart.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowStart.close();
                beg.stop();

            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(windowStart);
                sf::Vector2f shapeSize = shape.getSize();
                if (mousePosition.x >= shape.getPosition().x && mousePosition.x <= shape.getPosition().x + shapeSize.x &&
                    mousePosition.y >= shape.getPosition().y && mousePosition.y <= shape.getPosition().y + shapeSize.y) {
                    windowStart.close();
                    beg.stop();

                }
            }
        }

        windowStart.clear();
        windowStart.draw(startspace);
        windowStart.draw(shape);
        windowStart.display();
    }

    label:


    const int width = 800;
    const int height = 800;

    int pelleteaten = 0;
    int maze[12][12] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2 ,2 ,2 ,2 ,2 ,2 ,2 ,2, 2, 2, 1},
        {1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1},
        {1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1},
        {1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1},
        {1, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1},
        {1, 2, 1, 2, 2, 1, 2, 1, 2, 2, 2, 1},
        {1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
        {1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    float cellSize = 60.0f;

    sf::RenderWindow window(sf::VideoMode(width, height), "Pac-Man");
    window.setFramerateLimit(120);



    sf::Texture textureWall;
    if (!textureWall.loadFromFile("BRICK.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape wall(sf::Vector2f(cellSize, cellSize));
    wall.setTexture(&textureWall);

    sf::RectangleShape ghost1(sf::Vector2f(cellSize, cellSize));
    sf::Texture textureghost1;
    if (!textureghost1.loadFromFile("ghost1.png")) {
        cout << "error";
        return 0;
    }
    ghost1.setTexture(&textureghost1);
    ghost1.setFillColor(sf::Color::Red);

    sf::RectangleShape ghost2(sf::Vector2f(cellSize, cellSize));
    sf::Texture textureghost2;
    if (!textureghost2.loadFromFile("ghost4.png")) {
        cout << "error";
        return 0;
    }
    ghost2.setTexture(&textureghost2);
    ghost2.setFillColor(sf::Color::Magenta);

    sf::RectangleShape ghost3(sf::Vector2f(cellSize, cellSize));
    sf::Texture textureghost3;
    if (!textureghost3.loadFromFile("ghost3.png")) {
        cout << "error";
        return 0;
    }
    ghost3.setTexture(&textureghost3);
    ghost3.setFillColor(sf::Color::Blue);



    sf::RectangleShape ghost4(sf::Vector2f(cellSize, cellSize));
    sf::Texture textureghost4;
    if (!textureghost4.loadFromFile("ghost4.png")) {
        cout << "error";
        return 0;
    }
    ghost4.setTexture(&textureghost4);
    ghost4.setFillColor(sf::Color::Yellow);


    sf::Texture textureSpace;
    if (!textureSpace.loadFromFile("grasss.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape space(sf::Vector2f(cellSize, cellSize));
    space.setTexture(&textureSpace);

    sf::Texture texture;
    if (!texture.loadFromFile("pac.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape pacman(sf::Vector2f(60.0f, 60.0f));
    pacman.setTexture(&texture);
    pacman.setPosition(55, 55);


    sf::Texture textureCherry;
    if (!textureCherry.loadFromFile("cherry.png")) {
        cout << "error";
        return 0;
    }

    sf::CircleShape pellet(15.0f);
    pellet.setTexture(&textureCherry);


    sf::Texture heartTexture;
    if (!heartTexture.loadFromFile("heart.png")) {
        cout << "error";
        return 0;
    }

    sf::Sprite heartSprite;
    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(0.1f, 0.1f);
    sf::Texture melon;
    if (!melon.loadFromFile("melon.png")) {
        cout << "error";
        return 0;
    }

    sf::Sprite melonSprite;
    melonSprite.setTexture(melon);
    melonSprite.setScale(0.03f, 0.03f);
    melonSprite.setPosition(180, 55);

    sf::Vector2f melonPositions[4];

    // Initialize melon positions inside the array
    melonPositions[0] = sf::Vector2f(482, 59);
    melonPositions[1] = sf::Vector2f(610, 480);
    melonPositions[2] = sf::Vector2f(190, 355);
    melonPositions[3] = sf::Vector2f(300, 600);

    int score = -1;

    ghost1.setPosition(250, 175);
    ghost2.setPosition(165, 480);
    ghost3.setPosition(62, 310);
    ghost4.setPosition(600, 100);
    //////////sound area////////////////


    sf::SoundBuffer eatBuffer;
    if (!eatBuffer.loadFromFile("pacmaneating.wav")) {
        cout << "Error" << endl;
        return 0;
    }

    sf::Sound eatSound;
    eatSound.setBuffer(eatBuffer);

    sf::SoundBuffer collideBuffer;
    if (!collideBuffer.loadFromFile("ghostcollision.wav")) {
        cout << "Error" << endl;
        return 0;
    }

    sf::Sound collideSound;
    collideSound.setBuffer(collideBuffer);


    sf::SoundBuffer endBuffer;
    if (!endBuffer.loadFromFile("gameend.wav")) {
        cout << "Error" << endl;
        return 0;
    }

    sf::Sound gameEndSound;
    gameEndSound.setBuffer(endBuffer);

    ////////////////////////////////////



    sf::Texture textureSettings;
    if (!textureSettings.loadFromFile("level.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape settings(sf::Vector2f(800, 800));
    settings.setTexture(&textureSettings);
    sf::Texture textureEasy;
    if (!textureEasy.loadFromFile("ez.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape easy(sf::Vector2f(500, 300));
    easy.setTexture(&textureEasy);
    easy.setPosition(150, 100);
    easy.setFillColor(sf::Color::Yellow);
    sf::Texture textureMedium;
    if (!textureMedium.loadFromFile("me.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape medium(sf::Vector2f(500, 300));
    medium.setTexture(&textureMedium);
    medium.setPosition(150, 250);
    medium.setFillColor(sf::Color::Yellow);


    sf::Texture textureHard;
    if (!textureHard.loadFromFile("ha.png")) {
        cout << "error";
        return 0;
    }

    sf::RectangleShape hard(sf::Vector2f(500, 300));
    hard.setTexture(&textureHard);
    hard.setPosition(150, 400);
    hard.setFillColor(sf::Color::Yellow);


    sf::Texture textureDone;
    if (!textureDone.loadFromFile("plaay.png")) {
        cout << "error";
        return 0;
    }

    sf::RectangleShape done(sf::Vector2f(400, 300));
    done.setTexture(&textureDone);
    done.setPosition(200, 500);

    sf::RenderWindow windowCustomize(sf::VideoMode(width, height), "Levels");
    windowCustomize.setFramerateLimit(120);

    while (windowCustomize.isOpen()) {
        sf::Event event;
        while (windowCustomize.pollEvent(event)) {
            while (windowCustomize.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    windowCustomize.close();


                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(windowCustomize);

                    if (mousePosition.x >= easy.getPosition().x && mousePosition.x <= easy.getPosition().x + easy.getSize().x &&
                        mousePosition.y >= easy.getPosition().y && mousePosition.y <= easy.getPosition().y + easy.getSize().y) {
                        ghostSpeed = 0.5f;
                       easy.setFillColor(sf::Color::Green);
                        medium.setFillColor(sf::Color::Yellow);
                        hard.setFillColor(sf::Color::Yellow);
                      
                    }

                    sf::Vector2f mediumSize = medium.getSize();
                    if (mousePosition.x >= medium.getPosition().x && mousePosition.x <= medium.getPosition().x + mediumSize.x &&
                        mousePosition.y >= medium.getPosition().y && mousePosition.y <= medium.getPosition().y + mediumSize.y) {
                        ghostSpeed = 1.5f;
                      
                       easy.setFillColor(sf::Color::Yellow);
                        medium.setFillColor(sf::Color::Green);
                        hard.setFillColor(sf::Color::Yellow);
                       
                    }

                    sf::Vector2f hardSize = hard.getSize();
                    if (mousePosition.x >= hard.getPosition().x && mousePosition.x <= hard.getPosition().x + hardSize.x &&
                        mousePosition.y >= hard.getPosition().y && mousePosition.y <= hard.getPosition().y + hardSize.y) {
                        ghostSpeed = 3.0f;
                    
                        easy.setFillColor(sf::Color::Yellow);
                        medium.setFillColor(sf::Color::Yellow);
                        hard.setFillColor(sf::Color::Green);
                        

                    }


                    sf::Vector2f doneSize = done.getSize();
                    if (mousePosition.x >= done.getPosition().x && mousePosition.x <= done.getPosition().x + doneSize.x &&
                        mousePosition.y >= done.getPosition().y && mousePosition.y <= done.getPosition().y + doneSize.y) {
                        windowCustomize.close();

                    }

                }
            }
        }

        windowCustomize.clear();
        windowCustomize.draw(settings);
        windowCustomize.draw(easy);
        windowCustomize.draw(medium);
        windowCustomize.draw(hard);
        windowCustomize.draw(done);
        windowCustomize.display();
    }


    sf::Font font;
    if (!font.loadFromFile("Battle Bots.ttf")) {
        std::cout << "Error ";
        return 0;
    }

    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(width - 150, 10);


    int lives = 5;


    int highscore = getHighScore();
    sf::Text HighscoreText;
    HighscoreText.setFont(font);
    HighscoreText.setCharacterSize(24);
    HighscoreText.setFillColor(sf::Color::Yellow);

    sf::Vector2f ghost1Direction(0.f, 0.f);
    sf::Vector2f ghost2Direction(0.f, 0.f);
    sf::Vector2f ghost3Direction(0.f, 0.f);
    sf::Vector2f ghost4Direction(0.f, 0.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        int padding = 32; 
        int pacmanTileX = static_cast<int>((pacman.getPosition().x + pacman.getGlobalBounds().width / 2) / cellSize);
        int pacmanTileY = static_cast<int>((pacman.getPosition().y + pacman.getGlobalBounds().height / 2) / cellSize);

        sf::Vector2f pacmanPrevPosition = pacman.getPosition();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            pacmanTileX = static_cast<int>((pacman.getPosition().x - padding + pacman.getGlobalBounds().width / 2) / cellSize);
            if (maze[pacmanTileY][pacmanTileX] != 1) {
                pacman.move(-2, 0);
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            pacmanTileX = static_cast<int>((pacman.getPosition().x + padding + pacman.getGlobalBounds().width / 2) / cellSize);

            if (maze[pacmanTileY][pacmanTileX] != 1) {
                pacman.move(2, 0);
            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            pacmanTileY = static_cast<int>((pacman.getPosition().y - padding + pacman.getGlobalBounds().height / 2) / cellSize);

            if (maze[pacmanTileY][pacmanTileX] != 1) {
                pacman.move(0, -2);
            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            pacmanTileY = static_cast<int>((pacman.getPosition().y + padding + pacman.getGlobalBounds().height / 2) / cellSize);

            if (maze[pacmanTileY][pacmanTileX] != 1) {
                pacman.move(0, 2);
            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            // if user presses esc key from keyboard game ends and score is displayed
            window.close();
        }



        //  Ghosts
        sf::Vector2f ghost1PrevPosition = ghost1.getPosition();
        sf::Vector2f ghost2PrevPosition = ghost2.getPosition();
        sf::Vector2f ghost3PrevPosition = ghost3.getPosition();



        ///// ghost movement 
        moveGhost(ghost1, cellSize, ghostSpeed,ghost1Direction,maze);
        moveGhost(ghost3, cellSize, ghostSpeed,ghost2Direction,maze);
        moveGhost(ghost2,  cellSize, ghostSpeed,ghost3Direction,maze);
        moveGhost(ghost4, cellSize, ghostSpeed,ghost4Direction,maze);



        if (pacman.getGlobalBounds().intersects(ghost1.getGlobalBounds()) ||
            pacman.getGlobalBounds().intersects(ghost2.getGlobalBounds()) ||
            pacman.getGlobalBounds().intersects(ghost3.getGlobalBounds()) ||
            pacman.getGlobalBounds().intersects(ghost4.getGlobalBounds())) {

            pacman.setPosition(55, 55);
            // reset position of pacman
            lives--;

            if (lives <= 0) {

                window.close();
            }
            collideSound.play();

        }



        window.clear();





        ///////////////////// Draw the maze /////////////////////////////
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                //  grass texture as background
                space.setPosition(j * cellSize, i * cellSize);
                window.draw(space);

                if (maze[i][j] == 1) {
                    wall.setPosition(j * cellSize, i * cellSize);
                    window.draw(wall);
                }
                else if (maze[i][j] == 2) {  // 2 represents the pellet
                    //  grass texture as background again
                    space.setPosition(j * cellSize, i * cellSize);
                    window.draw(space);

                    // pellet on top of the grass texture
                    pellet.setPosition(j * cellSize + cellSize / 2 - pellet.getRadius(),
                        i * cellSize + cellSize / 2 - pellet.getRadius());
                    window.draw(pellet);

                    if (pacman.getGlobalBounds().intersects(pellet.getGlobalBounds())) {
                        // because pacman is standing on a pellet and we 
                        // dont want sound to play first because that pellet is not even visible
                         // score variable could also be used in turn
                        if (pelleteaten > 0)
                        {
                            eatSound.play();
                        }
                        pelleteaten++;

                        // Pac-Man collided with a pellet
                        score++;

                        // Removing pellet from maze
                        maze[i][j] = 0;

                    }
                }
            }
        } ////////  watermelon
        for (int i = 0; i < 5; i++) {
            // just like we initilaize array to a new value so that prev value doesnt remain
            if (melonPositions[i] != sf::Vector2f(0, 0)) {
                melonSprite.setPosition(melonPositions[i]);
                window.draw(melonSprite);

                bool melonCollision = false;


                if (pacman.getGlobalBounds().intersects(melonSprite.getGlobalBounds()) && !melonCollision) {

                    score = score + 5;
                    melonCollision = true;
                    melonPositions[i] = sf::Vector2f(0, 0);
                    // melon should disappear 
                }

                //  "+5" message  after melon collision
                if (melonCollision) {
                    sf::Text bonusText;
                    bonusText.setFont(font);
                    bonusText.setCharacterSize(50);
                    bonusText.setFillColor(sf::Color::Yellow);
                    bonusText.setString("+5");
                    bonusText.setPosition(pacman.getPosition().x, pacman.getPosition().y - 30);

                    window.draw(bonusText);
                    window.display();

                    // Introduce a delay by looping for a short duration
                    sf::Clock delayClock;
                    while (delayClock.getElapsedTime().asSeconds() < 0.3) {
                        // Empty loop to pause execution
                    }

                    melonCollision = false; // Reset melon collision flag
                }

            }
        }

        for (int i = 0; i < lives; ++i) {
            heartSprite.setPosition(width - (i + 1) * 50, 10);
            window.draw(heartSprite);
        }

        /// highscore
        HighscoreText.setString("High Score: " + std::to_string(highscore));
        HighscoreText.setPosition(500, 675);
        window.draw(HighscoreText);
        /////////

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setPosition(10, 10);

        window.draw(scoreText);

        ////// checking if score is greater than highscore then highscore is updated in file also.
        if (score > highscore) {
            highscore = score;
            updateHighScore(highscore);
        }
        if (score == 85)
        {
            window.close();
        }

        // Draw Pac-Man and ghostss
        window.draw(ghost1);
        window.draw(ghost2);
        window.draw(ghost3);
        window.draw(ghost4);
        window.draw(melonSprite);
        window.draw(pacman);
        window.display();

    }
    if(score<85)
    {
    sf::RenderWindow windowGameOver(sf::VideoMode(width, height), "Game-Over");
    windowGameOver.setFramerateLimit(120);

    sf::Texture textureOver;
    if (!textureOver.loadFromFile("gameover.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape gameover(sf::Vector2f(600, 200));
    gameover.setTexture(&textureOver);
    gameover.setPosition(110, 200);

    while (windowGameOver.isOpen()) {
        sf::Event event;
        while (windowGameOver.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                windowGameOver.close();
        }
        sf::Text GameOver;
        GameOver.setFont(font);
        GameOver.setCharacterSize(50);
        GameOver.setFillColor(sf::Color::Red);
        GameOver.setString("Total Score : " + std::to_string(score));
        GameOver.setPosition(200, 500);
        windowGameOver.draw(GameOver);
        windowGameOver.draw(gameover);
        windowGameOver.display();
        gameEndSound.play();
        sf::Clock delayClock;
        while (delayClock.getElapsedTime().asSeconds() < 3.0f) {

            //3 second window open
        }

        windowGameOver.close();
        gameEndSound.stop();

    }
}
    if (score == 85) {

        sf::RenderWindow windowWinner(sf::VideoMode(width, height), "Winner");
        windowWinner.setFramerateLimit(120);

        sf::Texture textureWin;
        if (!textureWin.loadFromFile("winner.png")) {
            cout << "error";
            return 0;
        }
        sf::RectangleShape win(sf::Vector2f(width, height));
        win.setTexture(&textureWin);
       

        while (windowWinner.isOpen()) {
            sf::Event event;
            while (windowWinner.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    windowWinner.close();
            }

            windowWinner.draw(win);

            windowWinner.display();
            gameEndSound.play();
            sf::Clock delayClock;
            while (delayClock.getElapsedTime().asSeconds() < 5.0f) {

                // to keep the window open for 2 seconds
            }

            windowWinner.close();
            gameEndSound.stop();

        }

    }


    sf::RenderWindow windowEnd(sf::VideoMode(width, height), "End-Page");
    windowEnd.setFramerateLimit(120);
    sf::Texture textureEnd;
    if (!textureEnd.loadFromFile("playagain.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape end(sf::Vector2f(width, height));
    end.setTexture(&textureEnd);

    sf::Texture textureYes;
    if (!textureYes.loadFromFile("yesss.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape yes(sf::Vector2f(100, 100));
    yes.setTexture(&textureYes);
    yes.setPosition(150, 600);
    sf::Texture textureNo;
    if (!textureNo.loadFromFile("noo.png")) {
        cout << "error";
        return 0;
    }
    sf::RectangleShape no(sf::Vector2f(100, 100));
    no.setTexture(&textureNo);
    no.setPosition(550, 600);


    while (windowEnd.isOpen()) {
        sf::Event event;
        while (windowEnd.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                windowEnd.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(windowEnd);
            sf::Vector2f shapeSize = no.getSize();
            if (mousePosition.x >= no.getPosition().x && mousePosition.x <= no.getPosition().x + shapeSize.x &&
                mousePosition.y >= no.getPosition().y && mousePosition.y <= no.getPosition().y + shapeSize.y) {
                windowEnd.close();
            }
            sf::Vector2f shapeSize1 = yes.getSize();
            if (mousePosition.x >= yes.getPosition().x && mousePosition.x <= yes.getPosition().x + shapeSize1.x &&
                mousePosition.y >= yes.getPosition().y && mousePosition.y <= yes.getPosition().y + shapeSize1.y) {
                goto label;

            }
        }

        windowEnd.draw(end);
        windowEnd.draw(yes);
        windowEnd.draw(no);
        windowEnd.display();

    }
    return 0;
}
