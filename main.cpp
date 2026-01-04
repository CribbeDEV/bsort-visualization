#include "raylib.h"
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct ColorRect {
    Color color;
    Rectangle rect;
};

static Color GenerateRandomColor()
{
    return Color{
        static_cast<unsigned char>(GetRandomValue(0, 255)),
        static_cast<unsigned char>(GetRandomValue(0, 255)),
        static_cast<unsigned char>(GetRandomValue(0, 255)),
        255
    };
}

static vector<ColorRect> generateRectangles(int count)
{
    std::vector<ColorRect> rectangles;
    rectangles.reserve(count);
    
    int box_width = GetScreenWidth() / count;
    
    for (int i = 0; i < count; ++i)
    {
        int height = GetRandomValue(10, GetScreenHeight());
        
        ColorRect cr;
        cr.color = GenerateRandomColor();
        cr.rect = Rectangle{
            static_cast<float>(i * box_width),
            static_cast<float>(GetScreenHeight() - height),
            static_cast<float>(box_width),
            static_cast<float>(height)
        };
        
        rectangles.push_back(cr);
    }
    
    return rectangles;
}

void repositionRectangles(vector<ColorRect>& v) {
    float box_width = static_cast<float>(GetScreenWidth()) / v.size();
    
    for (size_t i = 0; i < v.size(); ++i) {
        v[i].rect.x = i * box_width;
        v[i].rect.width = box_width;
    }
}

void swap_rectangles(vector<ColorRect>& rectangles, int j)
{
	swap(rectangles[j], rectangles[j+1]);
	repositionRectangles(rectangles);
}

void okRect(ColorRect* rect, Sound* sound)
{
    rect->color = Color{
        0,
        255,
        0,
        255
    };
    PlaySound(*sound);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <count>", argv[0]);
        return 1;
    }

    constexpr int screenWidth = 1920;
    constexpr int screenHeight = 1080;
    int box_count = stoi(argv[1]);

    InitWindow(screenWidth, screenHeight, "Bubblesort Algorithm Visualization");

    auto rectangles = generateRectangles(box_count);

    double lastGenerateTime = 0.0;
    const double cooldown = 0.5;

    double last = 0.0;
    const float animSpeed = 0.0;
    int i = 0;
    int j = 0;

    InitAudioDevice();
    Sound incWav = LoadSound("resources/inc.wav");
    Sound okOgg = LoadSound("resources/ok.ogg");
                        
    SetMasterVolume(0.2f);
    printf("Volume: %f", GetMasterVolume());                
    
    SetTargetFPS(60);
    
    if (!IsAudioDeviceReady()) {
        printf("Audio device failed to initialize!\n");
    } else {
        printf("Audio device ready!\n");
    }

    bool finished = false;
    
    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_SPACE) && (GetTime() - lastGenerateTime) >= cooldown)
        {
            rectangles = generateRectangles(box_count);
            lastGenerateTime = GetTime();
            i = 0;
            j = 0;
            finished = false;
	} 
        
        if ((GetTime() - last) >= animSpeed)
        {
	    if (i < (box_count - 1))
            {
                if (j < (box_count - i - 1))
                {
                    if (j < rectangles.size() - 1 && 
                        rectangles[j].rect.height > rectangles[j + 1].rect.height) {
                        SetSoundPitch(incWav, (1.0f + (0.01f*(float)j)));
                        PlaySound(incWav);
                        swap_rectangles(rectangles, j);
                    }
                    
                    j++;
                } else {
                    okRect(&rectangles[box_count-i-1], &okOgg);
                    j = 0;
                    i++;
                }
            } else {
                if (!finished)
                {
                    finished = true;
                    okRect(&rectangles[0], &okOgg);
                }
            }
            last = GetTime();
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (const auto& rect: rectangles)
            {
                DrawRectangleRec(rect.rect, rect.color);
            }
        
        EndDrawing();
    }

    UnloadSound(incWav);
    UnloadSound(okOgg);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
