#include "stdafx.h"
#include "InputManager.h"
#include "Framework.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TextGameObj.h"
#include <sstream>
#include <DataTableManager.h>
#include <ResourceManager.h>

Framework::Framework(int width, int height, const std::string& title)
    :screenWidth(width), screenHeight(height), title(title)
{
}

void Framework::Init(int width, int height, const std::string& title)
{
    window.create(sf::VideoMode(width, height), title);

    DATATABLE_MANAGER.LoadAll();
    RESOURCE_MANAGER.Init();
    SCENE_MANAGER.Init();
}

void Framework::Release()
{
    SCENE_MANAGER.Release();
    DATATABLE_MANAGER.ReleaseAll();
    //cursor->Release();
}

void Framework::Update(float deltaTime)
{
    SCENE_MANAGER.Update(deltaTime);
    //cursor->Update(deltaTime);
}

void Framework::Draw()
{
    SCENE_MANAGER.Draw(window);
    //cursor->Draw(window);
}

void Framework::Run()
{
    Init(screenWidth, screenHeight, title);

    clock.restart();


    //Cursor
    //sf::Image cursorImg;
    //cursorImg.loadFromFile("graphics/crosshair.png");
    //if (cursor.loadFromPixels(cursorImg.getPixelsPtr(), cursorImg.getSize(), { cursorImg.getSize().x / 2, cursorImg.getSize().y / 2 }))
    //{
    //    window.setMouseCursor(cursor);
    //}


    //FPS Viewer
    TextGameObj fpsViewer("fonts/NanumGothic.ttf", 20, "fpsViewer");
    sf::Font systemFont;
    systemFont.loadFromFile("fonts/NanumGothic.ttf");
    fpsViewer.SetFont(systemFont);
    fpsViewer.SetCharacterSize(20);
    fpsViewer.SetOrigin(Origins::TL);
    fpsViewer.SetPosition(0.0f, 0.0f);
    fpsViewer.SetActive(false);

    float fpsTime = 0.0f;
    int fps = 0;


    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        INPUT.Update(dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                SCENE_MANAGER.GetCurrentScene()->Exit();
                window.close();
                break;
            case sf::Event::GainedFocus:
                window.setMouseCursor(cursor);
                break;
            }

            INPUT.UpdateEvent(event);
        }

        if (window.isOpen())
        {
            //SystemEvent
            if (INPUT.GetKeyDown(sf::Keyboard::Tilde)) 
            {
                fpsViewer.SetActive(!fpsViewer.IsActive());
            }
            if (fpsViewer.IsActive())
            {
                fps++;
                fpsTime += dt;
                if (fpsTime > 1.0f)
                {
                    stringstream ss;
                    ss << fps;
                    fpsViewer.SetString(ss.str());

                    fps = 0;
                    fpsTime = 0.0f;
                }
            }

            Update(dt);

            window.clear();

            Draw();
            if (fpsViewer.IsActive())
            {
                fpsViewer.Draw(window);
            }

            window.display();

            if (isGameExitButton == true)
            {
                return;
            }
        }
    }

    Release();
}

void Framework::SetCursor(const Uint8* pixels, Vector2u size, Vector2u hotspot)
{
    if (cursor.loadFromPixels(pixels, size, hotspot))
    {
        window.setMouseCursor(cursor);
    }
}

sf::Vector2f Framework::GetWindowSize()
{
    return Vector2f(screenWidth, screenHeight);
}

sf::RenderWindow& Framework::GetWindow()
{
    return window;
}

void Framework::ExitButton(bool exit)
{
    isGameExitButton = true;
}

float Framework::GetGravity()
{
    return gravity;
}

float Framework::GetDPM()
{
    return dotPerMeter;
}