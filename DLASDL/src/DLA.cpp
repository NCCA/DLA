/* a basic window in SDL use the command line
g++ -Wall -g  PutPixel.cpp -o PutPixel `sdl-config --cflags --libs`

or

clang++ -Wall -g PutPixel.cpp -o PutPixel `sdl-config --cflags --libs`

to compile the program
*/

#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include "RGBA.h"

struct Vec2
{
     int m_x;
     int m_y;
};
//-----------------------------------------------------------------------------
/// @brief the width of the window
//-----------------------------------------------------------------------------
const int width = 800;
//-----------------------------------------------------------------------------
/// @brief the height of the window
//-----------------------------------------------------------------------------
const int height = 800;

//-----------------------------------------------------------------------------
/// @brief simple SDL clear screen function
/// @param [in] _screen a pointer to the SDL screen structure
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255
//-----------------------------------------------------------------------------
void clearScreen(SDL_Renderer *_ren,char _r,char _g,char _b);

//-----------------------------------------------------------------------------
/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
//-----------------------------------------------------------------------------
void SDLErrorExit(const std::string &_msg);


int main()
{
  //-----------------------------------------------------------------------------
  // First thing we need to do is initialise SDL in this case we are
  // setting up just the video subsystem if we need audio or timer etc
  // we would | (or) the flags together see http://www.libsdl.org/intro.en/usinginit.html
  // we check the return value and if not 0 it is an error
  //-----------------------------------------------------------------------------
  if (SDL_Init( SDL_INIT_VIDEO ) !=0)
  {
    SDLErrorExit("error initialising SDL");
  }
  //-----------------------------------------------------------------------------
  // next we create a window and make sure it works
  //-----------------------------------------------------------------------------
  SDL_Window *win = nullptr;
  win = SDL_CreateWindow("DLA", 100, 100, width, height, SDL_WINDOW_SHOWN);
  if (win == nullptr)
  {
      SDLErrorExit("Error creating Window");
  }
  //-----------------------------------------------------------------------------
  // now we associate a renderer with the window
  //-----------------------------------------------------------------------------
  SDL_Renderer *renderer = nullptr;
  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr)
  {
    SDLErrorExit("error creating renderer");
  }
  auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

  clearScreen(renderer,0,0,0);
  SDL_RenderPresent(renderer);
//  std::unique_ptr<unsigned char []> map=std::make_unique<unsigned char []>(width*height*3);
  std::unique_ptr<RGBA []> map=std::make_unique<RGBA []>(width*height);

  auto setPixel=[&map](unsigned int _x, unsigned int _y, unsigned char _r,
      unsigned char _g, unsigned  char _b){
    size_t index=(width *_y)+_x;
    map[index].set(_r,_g,_b,255);

  };
  unsigned char r,g,b;
  auto getPixel=[&map,&r,&g,&b](unsigned int _x, unsigned int _y){
    size_t index=(width *_y)+_x;
    r=map[index].red();
    g=map[index].green();
    b=map[index].blue();

  };

  std::random_device rd;  //Will be used to obtain a seed for the random number engine

  std::mt19937 rng(rd()); //Standard mersenne_twister_engine seeded with rd()
  //rng.seed(time(nullptr));
  rng.seed(1234);
  std::uniform_int_distribution<unsigned int> imageWRange(2,width-2);
  std::uniform_int_distribution<unsigned int> imageHRange(2,height-2);
  std::uniform_int_distribution<int> walkDirection(-1,1);

  for(unsigned int i=0; i<width*height; ++i)
  {
      map[i].set(255,255,255,255);

  }


  Vec2 walker;
  // set initial walker position
  walker.m_x=imageWRange(rng);
  walker.m_y=imageWRange(rng);

  for(int i=0; i<10; ++i)
  {
  auto sX=imageWRange(rng);
  auto sY=imageHRange(rng);

  setPixel(sX,sY,0,0,0);
  std::cout<<"Seed Pixel "<<sX<<' '<<sY<<'\n';
  }
/*
  for(int i=0; i<width; ++i)
  {

  setPixel(i,height/2,0,0,0);
//  std::cout<<"Seed Pixel "<<sX<<' '<<sY<<'\n';
  }
*/
  SDL_Event event;
  bool quit=false;
  bool pause=false;
  // now we loop until the quit flag is set to true
  while(!quit)
  {
    bool walking=true;
    walker.m_x=imageWRange(rng);
    walker.m_y=imageWRange(rng);

    while(walking && !pause)
    {
      // else move to a new point

      if(walker.m_x == 0 || walker.m_x == width-1 ||
         walker.m_y == 0 || walker.m_y == height-1 )
      {
          walking=false;
          goto FinishedWalking; // this will exit the loop
      }// check bounds.


    // see if we have a hit
    for(int y=-1; y<=1; ++y )
    {
      for(int x=-1; x<=1; ++x)
      {
          getPixel(walker.m_x+x,walker.m_y+y);
          if(r==0)
          {
              // were adjacent so set the pixel to black
              setPixel(walker.m_x,walker.m_y,0,0,0);
              // clear the red pixels
                for(unsigned int cr=0; cr<width*height; ++cr)
                {
                    if(map[cr].red() == 255)
                      map[cr].set(255,255,255,255);
                 }
              walking=false;
              goto FinishedWalking;
          }// end if black
      }// end x loop
    } // end y loop
    // draw current pixel
    setPixel(walker.m_x,walker.m_y,255,0,0);
    // update walker dir
      walker.m_x+=walkDirection(rng);
      walker.m_y+=walkDirection(rng);

//    SDL_UpdateTexture(texture,nullptr,map.get(),width*sizeof(unsigned int));
//    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

//    // finally we need to tell SDL to update the screen
//    SDL_RenderPresent(renderer);
//    SDL_PollEvent(&event);
    } // end while walking

  FinishedWalking : ;
    SDL_UpdateTexture(texture,nullptr,map.get(),width*sizeof(unsigned int));
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // finally we need to tell SDL to update the screen
    SDL_RenderPresent(renderer);

    // process SDL events, in this case we are looking for keys
    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_SPACE : pause^=true; break;
            default : break;
          }// key
        } // end key down
      } // end process event
    }
  } // end processing loop

  // finally when we are done we need to tidy up SDL by calling SDL_Quit
  // sometime this is added as the atexit function to make it happen
  // automatically
  SDL_Quit();

  return EXIT_SUCCESS;
}



//-----------------------------------------------------------------------------
void clearScreen(SDL_Renderer *_ren,char _r,char _g,char _b	)
{
  SDL_SetRenderDrawColor(_ren, _r,_g,_b,255);
  SDL_RenderClear(_ren);
}




//-----------------------------------------------------------------------------
void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<'\n';
  std::cerr<<SDL_GetError()<<'\n';
  SDL_Quit();
  exit(EXIT_FAILURE);
}
