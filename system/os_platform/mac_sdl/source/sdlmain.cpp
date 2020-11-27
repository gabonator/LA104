#include <SDL.h>
#include <SDL_render.h>
#include <iostream>
#include <vector>
#include "sdlhal.h"

//CHal* gHal{nullptr};
CSdlHal sdlHal;
CHal* gHal = &sdlHal;

void _main(void);
const int pixelSize = 2;

std::vector< unsigned char > pixels( BIOS::LCD::Width * BIOS::LCD::Height * 4, 0 );
void sdl_blit();

int nc = 0;
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
	
	// show drawing artifacts
	if (0 && nc++ > 200)
	{
		nc = 0;
		//sdl_blit();

	}
}

int getPixel(int x, int y)
{
    y = BIOS::LCD::Height-1-y;
    assert(x >= 0 && x < BIOS::LCD::Width);
    assert(y >= 0 && y < BIOS::LCD::Height);
    
    const unsigned int offset = ( BIOS::LCD::Width * 4 * y ) + x * 4;
    int b = pixels[ offset + 0 ];
    int g = pixels[ offset + 1 ];
    int r = pixels[ offset + 2 ];
    return RGB565RGB(r, g, b);
}

SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Window* window;
bool running = true;
int lastKey = 0;

void sdl_init()
{
	BIOS::FAT::Init();
    SDL_Init( SDL_INIT_EVERYTHING );
    atexit( SDL_Quit );
    
    window = SDL_CreateWindow
    (
     "LA104 emulator",
     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
     BIOS::LCD::Width*pixelSize, BIOS::LCD::Height*pixelSize,
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

//     gHal = new CSdlHal;
}

bool sdl_running()
{
    return running;
}

int sdl_lastKey()
{
    sdl_blit();
    int temp = lastKey;
    lastKey = 0;
    return temp;
}

void sdl_blit()
{
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
	SDL_RenderClear( renderer );
	
	SDL_UpdateTexture
	(
	 texture,
	 NULL,
	 &pixels[0],
	 BIOS::LCD::Width * 4
	 );
	
	SDL_RenderCopy( renderer, texture, NULL, NULL );
	SDL_RenderPresent( renderer );
}

void sdl_loop()
{
    SDL_Event event;
    
    //const Uint64 start = SDL_GetPerformanceCounter();
    
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
	
	sdl_blit();
	
    //const Uint64 end = SDL_GetPerformanceCounter();
    //const static Uint64 freq = SDL_GetPerformanceFrequency();
    //const double seconds = ( end - start ) / static_cast< double >( freq );
    //std::cout << "Frame time: " << seconds * 1000.0 << "ms" << std::endl;
    if (running)
        SDL_Delay(20);
}

void sdl_deinit()
{
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();

//    delete gHal;
}

int main( int argc, char** argv )
{
    sdl_init();
    sdl_loop();
    _main();
    sdl_deinit();
}
