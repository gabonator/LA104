
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <vector>
#include <library.h>

void _main(void);

std::vector< unsigned char > pixels( BIOS::LCD::Width * BIOS::LCD::Height * 4, 0 );

void setPixel(int x, int y, int c)
{
    y = BIOS::LCD::Height-1-y;
    assert(x >= 0 && x < BIOS::LCD::Width);
    assert(y >= 0 && y < BIOS::LCD::Height);
    
    const unsigned int offset = ( BIOS::LCD::Width * 4 * y ) + x * 4;
    pixels[ offset + 0 ] = Get565B(c);
    pixels[ offset + 1 ] = Get565G(c);
    pixels[ offset + 2 ] = Get565R(c);
    pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;
}

SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Window* window;
bool running = true;
int lastKey = 0;

void sdl_init()
{
    SDL_Init( SDL_INIT_EVERYTHING );
    atexit( SDL_Quit );
    
    window = SDL_CreateWindow
    (
     "LA104 emulator",
     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
     BIOS::LCD::Width*3, BIOS::LCD::Height*3,
     SDL_WINDOW_SHOWN
     );
    
    renderer = SDL_CreateRenderer
    (
     window,
     -1,
     SDL_RENDERER_ACCELERATED
     );

    texture = SDL_CreateTexture
    (
     renderer,
     SDL_PIXELFORMAT_ARGB8888,
     SDL_TEXTUREACCESS_STREAMING,
     BIOS::LCD::Width, BIOS::LCD::Height
     );
}

bool sdl_running()
{
    return running;
}

int sdl_lastKey()
{
    int temp = lastKey;
    lastKey = 0;
    return temp;
}

void sdl_loop()
{
    SDL_Event event;
    
    const Uint64 start = SDL_GetPerformanceCounter();
    
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
    SDL_RenderClear( renderer );

    while( SDL_PollEvent( &event ) )
    {
        if (SDL_KEYDOWN == event.type)
            lastKey = event.key.keysym.scancode;

        if( ( SDL_QUIT == event.type ) ||
           ( SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode ) )
        {
            running = false;
            break;
        }
    }
    
    SDL_UpdateTexture
    (
     texture,
     NULL,
     &pixels[0],
     BIOS::LCD::Width * 4
     );
    
    SDL_RenderCopy( renderer, texture, NULL, NULL );
    SDL_RenderPresent( renderer );
    
    const Uint64 end = SDL_GetPerformanceCounter();
    const static Uint64 freq = SDL_GetPerformanceFrequency();
    const double seconds = ( end - start ) / static_cast< double >( freq );
    std::cout << "Frame time: " << seconds * 1000.0 << "ms" << std::endl;
    if (running)
        SDL_Delay(100);
}

void sdl_deinit()
{
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
}

int main( int argc, char** argv )
{
    sdl_init();
    sdl_loop();
    _main();
    sdl_deinit();
}
