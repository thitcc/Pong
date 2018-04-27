#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

enum KEYS{ UP, DOWN, W, S };

const int LARGURA_TELA = 1024;
const int ALTURA_TELA = 768;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonte2 = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_SAMPLE *score = NULL;
ALLEGRO_SAMPLE *game_over_sample = NULL;
ALLEGRO_SAMPLE *music = NULL;
ALLEGRO_SAMPLE *sudden_death = NULL;
ALLEGRO_SAMPLE_INSTANCE *songInstance = NULL;

struct {

  float raio;
  float x;
  float y;
  int dir_x;
  int dir_y;

} bola;

typedef struct {

  int largura;
  int altura;
  int x;
  int y;
  int score;

} Retangulos;

Retangulos p1, p2;

bool inicializar();

int main() {

  if (!inicializar()) return -1;

  bool sair = false;
  bool end = false;
  bool menu = true;
  bool game_over = false;
  bool sudden = false;
  bool keys[4] = { false, false, false, false };
  double vel = 1.5;
  double *p = &vel;

  bola.raio = 15.0;
  bola.x = LARGURA_TELA / 2;
  bola.y = ALTURA_TELA / 2;
  bola.dir_x = 1;
  bola.dir_y = 1;

  p1.largura = 10;
  p1.altura = 90;
  p1.x = 15;
  p1.y = ALTURA_TELA / 2;
  p1.score = 0;

  p2.largura = 10;
  p2.altura = 90;
  p2.x = 990;
  p2.y = ALTURA_TELA / 2;
  p2.score = 0;

  ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
  ALLEGRO_COLOR red = al_map_rgb(251, 102, 122);

  al_clear_to_color(al_map_rgb(68, 68, 68));

  while (!sair)
  {
    if (menu)
    {
      al_flip_display();

      al_draw_text(fonte, red, LARGURA_TELA / 2, ALTURA_TELA / 2 - 50, ALLEGRO_ALIGN_CENTRE, "Pong");
      al_draw_text(fonte2, white, LARGURA_TELA / 2, ALTURA_TELA / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Press ENTER to play");

      while(!al_is_event_queue_empty(fila_eventos))
      {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
          if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
          {
            menu = false;
          }
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
          sair = true;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP)
        {
          if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
          {
            sair = true;
          }
        }
      }
    }
    else if (game_over)
    {
      al_flip_display();

      if (p1.score > p2.score)
      {
        al_draw_text(fonte, red, LARGURA_TELA / 2, ALTURA_TELA / 2 - 100, ALLEGRO_ALIGN_CENTRE, "PLAYER 1 VENCEU");
      }
      else
      {
        al_draw_text(fonte, red, LARGURA_TELA / 2, ALTURA_TELA / 2 - 100, ALLEGRO_ALIGN_CENTRE, "PLAYER 2 VENCEU");
      }

      al_draw_text(fonte2, white, LARGURA_TELA / 2, ALTURA_TELA / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Press ENTER to play again");
      al_draw_text(fonte2, white, LARGURA_TELA / 2, ALTURA_TELA / 2 + 300, ALLEGRO_ALIGN_CENTRE, "Press ESC to exit");

      while(!al_is_event_queue_empty(fila_eventos))
      {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
          if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
          {
            game_over = false;
            end = true;
            al_clear_to_color(al_map_rgb(68, 68, 68));
            if (p1.score == 3 || p2.score == 3)
            {
              *p = 1.5;
              p1.score = 0;
              p2.score = 0;
            }
            sudden = false;
            keys[UP] = false;
            keys[DOWN] = false;
            keys[W] = false;
            keys[S] = false;
          }
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
          sair = true;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP)
        {
          if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
          {
            sair = true;
          }
        }
      }
    }
    else
    {
      if (p1.score == 3 || p2.score == 3)
      {
        al_play_sample(game_over_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        game_over = true;
      }

      if (end)
      {
        bola.raio = 15.0;
        bola.x = LARGURA_TELA / 2;
        bola.y = ALTURA_TELA / 2;
        bola.dir_y = 1;

        p1.largura = 10;
        p1.altura = 90;
        p1.x = 15;
        p1.y = ALTURA_TELA / 2;

        p2.largura = 10;
        p2.altura = 90;
        p2.x = 990;
        p2.y = ALTURA_TELA / 2;

        if (p1.score > p2.score)
        {
          bola.dir_x = -1;
        }
        else if (p1.score == p2.score && p1.score != 0 && p2.score != 0)
        {
          bola.dir_y = -1;
        }
        else
        {
          bola.dir_x = 1;
        }

        end = false;
      }

      if (p1.score == 2 && p2.score == 2 && sudden == false)
      {
        al_play_sample(sudden_death, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        sudden = true;
      }

      al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
      al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
      al_play_sample_instance(songInstance);


      while(!al_is_event_queue_empty(fila_eventos))
      {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
          switch(evento.keyboard.keycode)
          {
            case ALLEGRO_KEY_UP:
            keys[UP] = true;
            break;
            case ALLEGRO_KEY_DOWN:
            keys[DOWN] = true;
            break;
            case ALLEGRO_KEY_W:
            keys[W] = true;
            break;
            case ALLEGRO_KEY_S:
            keys[S] = true;
            break;
          }
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
          sair = true;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP)
        {
          switch(evento.keyboard.keycode)
          {
            case ALLEGRO_KEY_UP:
            keys[UP] = false;
            break;
            case ALLEGRO_KEY_DOWN:
            keys[DOWN] = false;
            break;
            case ALLEGRO_KEY_W:
            keys[W] = false;
            break;
            case ALLEGRO_KEY_S:
            keys[S] = false;
            break;
            case ALLEGRO_KEY_ESCAPE:
            sair = true;
            break;
          }
        }
      }

      al_draw_textf(fonte2, white, LARGURA_TELA / 2 - 150, ALTURA_TELA - 700, ALLEGRO_ALIGN_CENTRE, "%d", p1.score);
      al_draw_textf(fonte2, white, LARGURA_TELA / 2 + 150, ALTURA_TELA - 700, ALLEGRO_ALIGN_CENTRE, "%d", p2.score);
      al_draw_filled_rectangle(p1.x, p1.y, p1.x+20, p1.y+120, white);
      al_draw_filled_rectangle(p2.x, p2.y, p2.x+20, p2.y+120, white);
      al_draw_filled_circle(bola.x, bola.y, bola.raio, red);
      al_flip_display();
      al_clear_to_color(al_map_rgb(68, 68, 68));

      if (bola.x == LARGURA_TELA / 2 && bola.y == ALTURA_TELA / 2)
      {
        al_draw_textf(fonte2, white, LARGURA_TELA / 2 - 150, ALTURA_TELA - 700, ALLEGRO_ALIGN_CENTRE, "%d", p1.score);
        al_draw_textf(fonte2, white, LARGURA_TELA / 2 + 150, ALTURA_TELA - 700, ALLEGRO_ALIGN_CENTRE, "%d", p2.score);
        al_draw_filled_rectangle(p1.x, p1.y, p1.x+20, p1.y+120, white);
        al_draw_filled_rectangle(p2.x, p2.y, p2.x+20, p2.y+120, white);
        al_draw_filled_circle(bola.x, bola.y, bola.raio, red);
        al_flip_display();
        al_clear_to_color(al_map_rgb(68, 68, 68));
        al_rest(0.3);
      }

      bola.x += vel * bola.dir_x;
      bola.y += vel * bola.dir_y;

      p1.y -= keys[W] * 3;
      p2.y -= keys[UP] * 3;
      p1.y += keys[S] * 3;
      p2.y += keys[DOWN] * 3;

      if (p1.y >= 630)
      {
        p1.y = 630;
      }
      else if (p1.y <= 20)
      {
        p1.y = 20;
      }

      if (p2.y >= 630)
      {
        p2.y = 630;
      }
      else if (p2.y <= 20)
      {
        p2.y = 20;
      }

      if (bola.x >= p2.x - bola.raio && bola.y <= p2.y+120 && bola.y >= p2.y)
      {
        bola.dir_x = -1;
        bola.x = p2.x - bola.raio;
        al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        vel += 0.09;
      }
      else if (bola.x >= LARGURA_TELA - bola.raio)
      {
        p1.score++;
        vel = 1.5;
        al_play_sample(score, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        end = true;
      }

      if (bola.x <= bola.raio + 33 && bola.y <= p1.y + 120 && bola.y >= p1.y)
      {
        bola.dir_x = 1;
        bola.x = p1.x + 33;
        al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        vel += 0.09;
      }
      else if (bola.x <= bola.raio)
      {
        p2.score++;
        vel = 1.5;
        al_play_sample(score, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        end = true;
      }

      if (bola.y >= ALTURA_TELA - bola.raio)
      {
        bola.dir_y = -1;
        bola.y = ALTURA_TELA - bola.raio;
        vel += 0.09;
      }
      else if (bola.y <= bola.raio)
      {
        bola.dir_y = 1;
        bola.y = bola.raio;
        vel += 0.09;
      }

      al_rest(0.005);
      }

      if (sair)
      {
        al_destroy_audio_stream(musica);
        al_destroy_sample(sample);
        al_destroy_sample(score);
        al_destroy_sample(game_over_sample);
        al_destroy_sample(music);
        al_destroy_sample(sudden_death);
        al_destroy_sample_instance(songInstance);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_font(fonte);
        al_destroy_font(fonte2);

        return 0;
      }

    }
}

bool inicializar() {

    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }

    if (!al_reserve_samples(5))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }

    al_set_window_title(janela, "Pong");

    sample = al_load_sample("sounds/hit.ogg");
    score = al_load_sample("sounds/score.ogg");
    music = al_load_sample("sounds/jazz.ogg");
    sudden_death = al_load_sample("sounds/sudden_death.ogg");
    game_over_sample = al_load_sample("sounds/game_over.ogg");

    if (!sample)
    {
        fprintf(stderr, "Falha ao carregar hit.\n");
        al_destroy_display(janela);
        return false;
    }

    if (!score)
    {
        fprintf(stderr, "Falha ao carregar score.\n");
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }

    if (!music)
    {
        fprintf(stderr, "Falha ao carregar musica.\n");
        al_destroy_display(janela);
        al_destroy_sample(sample);
        al_destroy_sample(score);
        return false;
    }

    if (!sudden_death)
    {
        fprintf(stderr, "Falha ao carregar musica intensa.\n");
        al_destroy_display(janela);
        al_destroy_sample(sample);
        al_destroy_sample(score);
        al_destroy_sample(music);
        return false;
    }

    if (!game_over_sample)
    {
        fprintf(stderr, "Falha ao carregar musica game over.\n");
        al_destroy_display(janela);
        al_destroy_sample(sample);
        al_destroy_sample(score);
        al_destroy_sample(music);
        al_destroy_sample(sudden_death);
        return false;
    }

    songInstance = al_create_sample_instance(music);

    fonte = al_load_font("fonts/Comfortaa-Regular.ttf", 60, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        al_destroy_display(janela);
        return false;
    }

    fonte2 = al_load_font("fonts/AmaticSC-Regular.ttf", 48, 0);
    if (!fonte2)
    {
        fprintf(stderr, "Falha ao carregar fonte2.\n");
        al_destroy_display(janela);
        al_destroy_font(fonte);
        return false;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        al_destroy_font(fonte);
        al_destroy_font(fonte2);
        return false;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return true;

}
