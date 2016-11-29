#include "menu.h"
#include "settings.h"
#include "script_interpreter.h"
#include "shop.h"
#include "level.h"
#include "load_audio_effects.h"
#include "puzzle.h"
#include<cstdio>
SDL_Surface *screen;

Script_interpreter script_interpreter;

Shop_Screen shop_screen;

Level level;

Mix_Music *launcher_background_music=NULL;

int main( int argc, char* args[] )
{
 if(SDL_Init(SDL_INIT_EVERYTHING)<0)
    {
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"SDL_Init() failed : %s ",SDL_GetError());
     fclose(log_file);
     exit(-1);
    }
 if(TTF_Init()==-1)
    {
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"TTF_Init() failed : %s ",TTF_GetError());
     fclose(log_file);
     exit(-2);
    }
 MUSIC_MODULE_INIT=true;
 if(Mix_Init(MIX_INIT_MP3)&(MIX_INIT_MP3)!=MIX_INIT_MP3)
    {
     MUSIC_MODULE_INIT=false;
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"Mix_Init() failed : %s ",Mix_GetError());
     fclose(log_file);
    }
 SDL_WM_SetCaption("Pixel World",NULL);
 Set_icon("images/icon.bmp");
 Load_Settings();
 screen=SDL_SetVideoMode(RESOLUTION_X,RESOLUTION_Y,32,DISPLAY_MODE);
 MUSIC_MODULE_INIT=true;
 if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1)
    {
     MUSIC_MODULE_INIT=false;
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"Mix_OpenAudio failed : %s ",Mix_GetError());
     fclose(log_file);
    }
 SDL_Flip(screen);
 Menu main_menu,gamemode_menu,story_menu,duel_menu;
 Load_all_images();
 Load_Duel_Mode_effects();
 main_menu.Load("menu/main_menu.pwm");
 gamemode_menu.Load("menu/gamemode.pwm");
 story_menu.Load("menu/story_menu.pwm");
 duel_menu.Load("menu/duel_menu.pwm");
 LAUNCHER_BBACKGROUND.Update_size();
 LAUNCHER_BBACKGROUND.Load_Logo();
 launcher_background_music=Mix_LoadMUS("audio/Hallowed Be Thy Name.mp3");
 if(MUSIC_MODULE_INIT)
    {
     Mix_Volume(-1,MIX_MAX_VOLUME*VOLUME/100);
     Mix_Volume(2,MIX_MAX_VOLUME*VOLUME/100);
     Mix_Volume(3,MIX_MAX_VOLUME*(VOLUME/10)/100);
     Mix_Volume(4,MIX_MAX_VOLUME*(VOLUME/10)/100);
     Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
     Mix_PlayMusic(launcher_background_music,-1);
    }
 Puzzle test;
 test.Set_name("1");
 test.Load();
 if(false)
    {
     bool ret=test.Start(screen);
     if(ret==false && false)
        exit(666013);
    }
 int option=-1;
 while(option!=-2)
       {
        option=main_menu.Start(screen);
        switch(option)
               {
                case -2:{return 0;};
                case -1:{break;};
                case 0:{
                        int _option=-1;
                        while(_option!=-2)
                              {
                               _option=gamemode_menu.Start(screen);
                               option=-2;
                               switch(_option)
                                      {
                                       case -2:{return 0;};
                                       case -1:{break;};
                                       case 0:{
                                               //Story Mode
                                               int __option=-1;
                                               while(__option!=-2)
                                                     {
                                                      __option=story_menu.Start(screen);
                                                      switch(__option)
                                                             {
                                                              case -2:{return 0;};
                                                              case -1:{break;};
                                                              case 0:{//Launch Story Mode
                                                                      if(MUSIC_MODULE_INIT)
                                                                         Mix_HaltMusic();
                                                                      level.Set_screen(screen);
                                                                      level.Setup("The Stables Western Exit");
                                                                      level.Start(screen);
                                                                      if(MUSIC_MODULE_INIT)
                                                                         Mix_PlayMusic(launcher_background_music,-1);
                                                                      _option=-2;
                                                                      break;
                                                                     };
                                                              case 1:{
                                                                      if(shop_screen.Start(screen,"Shop","Timy")==-1)
                                                                         return 0;
                                                                      break;
                                                                     };
                                                              case 2:{
                                                                      _option=-1;
                                                                      __option=-2;
                                                                      break;
                                                                     };
                                                             }
                                                     }
                                               break;
                                              };
                                       case 1:{//Launch Duel Mode
                                               int __option=-1;
                                               while(__option!=-2)
                                                     {
                                                      __option=duel_menu.Start(screen);
                                                      switch(__option)
                                                             {
                                                              case -2:{return 0;};
                                                              case -1:{break;};
                                                              case 0:{//Launch Duel Mode
                                                                      if(MUSIC_MODULE_INIT)
                                                                         Mix_HaltMusic();
                                                                      level.Set_screen(screen);
                                                                      level.Setup("Duel Mode");
                                                                      level.Start(screen);
                                                                      if(MUSIC_MODULE_INIT)
                                                                         Mix_PlayMusic(launcher_background_music,-1);
                                                                      _option=-2;
                                                                      break;
                                                                     };
                                                              case 1:{
                                                                      if(shop_screen.Start(screen,"Shop","Player1")==-1)
                                                                         return 0;
                                                                      if(shop_screen.Start(screen,"Shop","Player2")==-1)
                                                                         return 0;
                                                                      break;
                                                                     };
                                                              case 2:{
                                                                      _option=-1;
                                                                      __option=-2;
                                                                      break;
                                                                     };
                                                             }
                                                     }
                                               break;
                                              };
                                       case 2://Back
                                              option=-1;
                                              _option=-2;
                                              break;
                                      }
                              }
                        /*Run the game*/break;};
                case 1:{Graphic_Settings(screen);
                        Save_Settings();
                        LAUNCHER_BBACKGROUND.Update_size();
                        main_menu.Load("menu/main_menu.pwm");
                        gamemode_menu.Load("menu/gamemode.pwm");
                        story_menu.Load("menu/story_menu.pwm");
                        duel_menu.Load("menu/duel_menu.pwm");
                        if(MUSIC_MODULE_INIT)
                           {
                            Mix_Volume(-1,MIX_MAX_VOLUME*VOLUME/100);
                            Mix_Volume(2,MIX_MAX_VOLUME*VOLUME/100);
                            Mix_Volume(3,MIX_MAX_VOLUME*(VOLUME/10)/100);
                            Mix_Volume(4,MIX_MAX_VOLUME*(VOLUME/10)/100);
                            Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
                           }
                        break;
                       };
                case 2:{
                        script_interpreter.Start(screen,"Credits");
                        //script_interpreter.Start(screen,"Test");
                        break;
                       };
                case 3:{
                        option=-2;
                        break;
                       };
               }
       }
 return 0;
}
