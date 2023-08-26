#include <SFML/Graphics.hpp>
#include <math.h>
#include <time.h>
#include <string>

#include <stdio.h>
#include <iostream>

float deltaTime = 0.f;


int main(int argc, char *argv[]){

    std::string Path = "bin/source/animate.png";

    if (argc > 1){
        Path = argv[1];
    }


    int windowWidth     =   800;
    int windowHeight    =   800;

    std::string windowTitle = "GIFinator FPS: ";
    
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle + "12");

    window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2-windowWidth/2, sf::VideoMode::getDesktopMode().height/2-windowHeight/2));

    sf::CircleShape pauseMark(50);
    sf::CircleShape fpsMark(50);



    sf::Vector2u txtSize;
    sf::Texture texture;
    if (!texture.loadFromFile(Path)){
        printf("FAILED TO LOAD FILE %s\n", Path.c_str());
        window.close();
    }

    txtSize = texture.getSize();

    if (!texture.loadFromFile(Path, sf::IntRect(0, 0, txtSize.y, txtSize.y))){
        printf("FAILED TO LOAD FILE %s\n", Path.c_str());
        window.close();
    }

    sf::Sprite dream;
    dream.setTexture(texture);

    dream.setScale(sf::Vector2f((windowWidth / txtSize.y), (windowWidth / txtSize.y)));

    int curPos = 0;

    int mouseDelta = 0;

    float fps = 12;
    int paused = 0;


    sf::Clock updateAgo;

    sf::Time dt;
    sf::Clock deltaClock;

    updateAgo.restart();

    while (window.isOpen()){

        mouseDelta = 0;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelMoved){
                mouseDelta = event.mouseWheel.delta;
            }

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Space){
                    paused = paused ? 0 : 1;
                    window.setTitle(windowTitle + std::to_string((int)fps) + (paused ? "  [ PAUSED ]" : ""));
                }
            }
        }

        if (mouseDelta){
            fps += mouseDelta;
            if (fps < 0){
                fps = 0;
            }

            window.setTitle(windowTitle + std::to_string((int)fps));
        }

        printf("FPS: %g\n", fps);

        if (updateAgo.getElapsedTime().asSeconds() >= 1/fps && !paused){
            updateAgo.restart();

            if (((curPos+1)*txtSize.y) >= txtSize.x){
                curPos = 0;
            }else{
                ++curPos;
            }

            // printf("x: %d\ny: %d\t\t[ %d ]\t\t[ %d ]\n\n", txtSize.x, txtSize.y, curPos, (curPos+1)*txtSize.y);

            if (!texture.loadFromFile(Path, sf::IntRect(curPos*txtSize.y, 0, txtSize.y, txtSize.y))){
                printf("FAILED TO LOAD FILE %s\n", Path.c_str());
                window.close();
            }
            dream.setTexture(texture);

        }

        window.clear(sf::Color(15, 15, 15));

        window.draw(dream);
        //window.draw(text);

        window.display();

        dt = deltaClock.restart();
        deltaTime = dt.asSeconds();
    }

    return 0;
}