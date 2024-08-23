#ifndef INCLUDE_DUI_H_
#define INCLUDE_DUI_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct __DIM_vector2 {
  int x;
  int y;
} DIM_vector2;

typedef struct __DIM_rect {
  int x, y;
  int w, h;
} DIM_rect;

typedef struct __DIM_color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} DIM_color;

#define DIM_BLACK                                                              \
  (DIM_color) { 0x00, 0x00, 0x00, 0xFF }
#define DIM_WHITE                                                              \
  (DIM_color) { 0XFF, 0xFF, 0xFF, 0xFF }
#define DIM_GRAY                                                               \
  (DIM_color) { 0XAA, 0xAA, 0xAA, 0xFF }

typedef struct __DIM_button {
  DIM_rect pos_and_size;
  void *text;
  bool border;
  int border_width;
  DIM_color background_color;
  DIM_color text_color;
  DIM_color border_color;
} DIM_button;

SDL_Color DIM_color_to_sdl_color(DIM_color *color) {
  return (SDL_Color){color->r, color->g, color->b, color->a};
}

SDL_Rect DIM_rect_to_sdl_rect(DIM_rect *rect) {
  return (SDL_Rect){rect->x, rect->y, rect->w, rect->h};
}

bool DIM_draw_button_sdl(SDL_Renderer *renderer, DIM_button *button,
                         TTF_Font *font) {
  SDL_Rect rect = DIM_rect_to_sdl_rect(&button->pos_and_size);
  SDL_Color background_color =
      DIM_color_to_sdl_color(&button->background_color);
  SDL_Surface *text_surface = TTF_RenderText_Blended(
      font, (char *)button->text, DIM_color_to_sdl_color(&button->text_color));
  if (text_surface == NULL) {
    fprintf(stderr, "could not create a text surface for button text\n");
    fprintf(stderr, "%s\n", TTF_GetError());
    return false;
  }
  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(renderer, text_surface);
  if (text_texture == NULL) {
    fprintf(stderr, "could not create a text texture for button text\n");
    SDL_FreeSurface(text_surface);
    return false;
  }
  SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g,
                         background_color.b, background_color.a);
  SDL_RenderFillRect(renderer, &rect);
  SDL_RenderCopy(renderer, text_texture, NULL, &rect);
  return true;
}

#endif // INCLUDE_DUI_H_
