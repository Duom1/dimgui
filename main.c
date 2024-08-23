#include "dim.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Clean_on_exit {
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
  SDL_Surface *text_surface;
  SDL_Texture *text_texture;
  bool sdl_text_surface;
  bool sdl_text_texture;
  bool sdl_ttf_init_successful;
  bool sdl_ttf_font_set;
  bool sdl_window_set;
  bool sdl_init_successful;
  bool sdl_renderer_set;
} clean_on_exit;

void process_on_exit() {
  if (clean_on_exit.sdl_window_set) {
    SDL_DestroyWindow(clean_on_exit.window);
    printf("destryed window\n");
  }
  if (clean_on_exit.sdl_renderer_set) {
    SDL_DestroyRenderer(clean_on_exit.renderer);
    printf("destryed renderer\n");
  }
  if (clean_on_exit.sdl_ttf_font_set) {
    TTF_CloseFont(clean_on_exit.font);
    printf("cleaned font\n");
  }
  if (clean_on_exit.sdl_text_surface) {
    SDL_FreeSurface(clean_on_exit.text_surface);
    printf("freed text surface");
  }
  if (clean_on_exit.sdl_text_texture) {
    SDL_DestroyTexture(clean_on_exit.text_texture);
    printf("destryed texture\n");
  }
  if (clean_on_exit.sdl_init_successful) {
    SDL_Quit();
    printf("quit sdl\n");
  }
  if (clean_on_exit.sdl_ttf_init_successful) {
    TTF_Quit();
    printf("quit ttf\n");
  }
}

int main(void) {
  atexit(process_on_exit);

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "failed to inittialize sdl\n");
    exit(EXIT_FAILURE);
  }
  clean_on_exit.sdl_init_successful = true;

  if (TTF_Init() == -1) {
    fprintf(stderr, "failed to inittialize ttf\n");
    exit(EXIT_FAILURE);
  }
  clean_on_exit.sdl_ttf_init_successful = true;

  SDL_Window *window = SDL_CreateWindow("hello", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 800, 800, 0);
  if (window == NULL) {
    fprintf(stderr, "failed to inittialize window\n");
    exit(EXIT_FAILURE);
  }
  clean_on_exit.window = window;
  clean_on_exit.sdl_window_set = true;

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  if (window == NULL) {
    fprintf(stderr, "failed to create a rendering context\n");
    exit(EXIT_FAILURE);
  }
  clean_on_exit.renderer = renderer;
  clean_on_exit.sdl_renderer_set = true;

  TTF_Font *font = TTF_OpenFont("./OpenSans-Regular.ttf", 120);
  if (font == NULL) {
    fprintf(stderr, "failed to load font\n");
    exit(EXIT_FAILURE);
  }
  clean_on_exit.sdl_ttf_font_set = true;
  clean_on_exit.font = font;

  SDL_Surface *text_surface = TTF_RenderText_Blended(
      font, "hello world", (SDL_Color){0x0, 0x0, 0x0, 0xff});
  if (text_surface == NULL) {
    fprintf(stderr, "could not create a text surface\n");
    exit(EXIT_FAILURE);
  }

  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(renderer, text_surface);
  if (text_texture == NULL) {
    fprintf(stderr, "could not create a text texture\n");
    exit(EXIT_FAILURE);
  }
  clean_on_exit.sdl_text_texture = true;
  clean_on_exit.text_texture = text_texture;

  SDL_Rect render_quad = {10, 10, text_surface->w, text_surface->h};
  SDL_FreeSurface(text_surface);
  clean_on_exit.sdl_text_surface = false;

  char button_text[] = "hello";
  DIM_button button = {(DIM_rect){100, 100, 200, 100},
                       button_text,
                       false,
                       0,
                       DIM_GRAY,
                       DIM_WHITE,
                       DIM_BLACK};

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &render_quad);
    DIM_draw_button_sdl(renderer, &button, font);
    SDL_RenderPresent(renderer);
  }
  return EXIT_SUCCESS;
}
