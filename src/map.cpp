#include "map.h"

/*Map::~Map()
{
 Clear(true);
}*/

void Map::Clear(bool _delete,bool _delete_all)
{
 if(_delete && _delete_all)
    {
     //map_textures[20].Clear();
     //map_textures[0].Clear();
     for(auto i:map_textures)
         i.second.Clear(true);
     map_textures.clear();
    }
 else
    {
     for(auto i:map_textures)
         i.second.Clear(false);
     map_textures.clear();
    }
 for(int k=0;k<number_of_layers;k++)
     for(int i=0;i<=1;i++)
         for(int j=0;j<=1;j++)
             {
              if(map_image[k][i][j]!=NULL && _delete)
                 {
                  Destroy_Texture(map_image[k][i][j]);
                  map_image[k][i][j]=NULL;
                 }
              std::vector<std::pair<int,int> >().swap(fast_access_map_textures_animations[k][i][j]);
              std::vector<std::pair<int,int> >().swap(fast_access_clues_map_textures[i][j]);
             }
 std::vector<std::pair<int,int> >().swap(fast_access_special_clues_map_textures);
 for(int k=0;k<number_of_layers;k++)
     for(int i=0;i<number_of_lines;i++)
         for(int j=0;j<number_of_columns;j++)
             {
              map_textures_ids[k][i][j].Clear();
              clues_map_textures_ids[i][j].Clear();
              special_clues_map_textures_ids[i][j].Clear();
             }
 if(_delete)
    {
     Destroy_Texture(name_image);
     name_image=NULL;
     name_image_opaque_time=NAME_IMAGE_OPAQUE_TIME;
     name_image_alpha=SDL_ALPHA_OPAQUE;
    }
}

void Map::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Map::Set_map_texture_id(int x,int y,Interactive_map_texture _map_texture_id)
{
 std::vector<Map_texture_id> textures_ids;
 _map_texture_id.Get_all_textures_ids(textures_ids);
 for(std::vector<Map_texture_id>::iterator i=textures_ids.begin();i!=textures_ids.end();i++)
     if(map_textures[i->Get_id()].Get_id()==0 || map_textures[i->Get_id()].Get_id()==20)
        {
         map_textures[i->Get_id()].Set_id(i->Get_id());
         map_textures[i->Get_id()].Load();
        }
 if(map_textures[_map_texture_id.Get_texture_id()].Get_power()>map_textures[map_textures_ids[0][x][y].Get_texture_id()].Get_power())
    map_textures_ids[0][x][y]=_map_texture_id;
}

bool Map::Is_obstacle(int x,int y)
{
 if(y>=number_of_columns || x>=number_of_lines)
    return false;
 if((clues_map_textures_ids[x][y].Get_id()!=0 && map_textures[clues_map_textures_ids[x][y].Get_interactive_map_texture()->Get_texture_id()].Is_obstacle()) ||
    (special_clues_map_textures_ids[x][y].Get_id()!=0 && map_textures[special_clues_map_textures_ids[x][y].Get_interactive_map_texture()->Get_texture_id()].Is_obstacle()))
    return true;
 return (map_textures[map_textures_ids[0][x][y].Get_texture_id()].Is_obstacle());
}

bool Map::Is_obstacle_pixel(int x,int y)
{
 return map_obstacles[x][y];
}

bool Map::Is_light(int x,int y)
{
 if(x>=number_of_lines || x<0 || y<0 || y>=number_of_columns)
    return false;
 return map_textures[map_textures_ids[0][x][y].Get_texture_id()].Is_light();
}

bool Map::Is_Trigger(int x,int y)
{
 if(x>=number_of_lines || x<0 || y<0 || y>=number_of_columns)
    return false;
 return map_textures[map_textures_ids[0][x][y].Get_texture_id()].Is_trigger();
}

char *Map::Get_name()
{
 return name;
}

int Map::Get_type(int x,int y)
{
 return map_textures[map_textures_ids[0][x][y].Get_texture_id()].Get_type();
}

int Map::Get_map_texture_type(int _texture_position,int x,int y)
{
 return map_textures[map_textures_ids[0][x][y].Get_texture_id(_texture_position)].Get_type();
}

int Map::Get_number_of_lines()
{
 return number_of_lines;
}

int Map::Get_number_of_columns()
{
 return number_of_columns;
}

Buff Map::Get_map_texture_Buff(int x,int y)
{
 return map_textures[map_textures_ids[0][x][y].Get_texture_id()].Get_Buff();
}

Interactive_map_texture Map::Get_map_texture_id(int x,int y)
{
 return map_textures_ids[0][x][y];
}

int Map::Get_map_texture_type(int x,int y)
{
 return map_textures[map_textures_ids[0][x][y].Get_texture_id()].Get_type();
}

int Map::Get_map_texture_player_pos_x(int x,int y)
{
 return map_textures_ids[0][x][y].Get_texture_player_map_pos_x();
}

int Map::Get_map_texture_player_pos_y(int x,int y)
{
 return map_textures_ids[0][x][y].Get_texture_player_map_pos_y();
}

char *Map::Get_map_texture_map_name(int x,int y)
{
 return map_textures_ids[0][x][y].Get_texture_map_name();
}

char *Map::Get_map_texture_puzzle_name(int x,int y)
{
 return map_textures_ids[0][x][y].Get_texture_puzzle_name();
}

int Map::Get_map_texture_key_id(int x,int y)
{
 return map_textures_ids[0][x][y].Get_key_id();
}

void Map::Copy(int x,int y,Map *source)
{
 for(int i=0;i<source->Get_number_of_lines();i++)
     for(int j=0;j<source->Get_number_of_columns();j++)
         {
          if(x+i<0 || y+j<0 || x+i>=Get_number_of_lines() || y+j>=Get_number_of_columns())
             continue;
          Set_map_texture_id(x+i,y+j,source->Get_map_texture_id(i,j));
         }
}

void Map::Decrease_map_textures_ids_remaining_time()
{
 for(int i=0;i<=1;i++)
     for(int j=0;j<=1;j++)
         for(int k=0;k<number_of_layers;k++)
             {
              Decrease_map_texture_id_remaining_time(k,i,j);
             }
}

void Map::Decrease_map_texture_id_remaining_time(int _layer,bool before_player,bool lights)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_map_textures_animations[_layer][before_player][lights].begin();it!=fast_access_map_textures_animations[_layer][before_player][lights].end();it++)
     {
      if(!map_textures_ids[_layer][it->first][it->second].Is_done())
         {
          if(map_textures[map_textures_ids[_layer][it->first][it->second].Get_texture_id()].Is_passer() &&
             map_textures_ids[_layer][it->first][it->second].Get_texture_frame()==map_textures[map_textures_ids[_layer][it->first][it->second].Get_texture_id()].Get_number_of_frames()-1)
             {
              if(map_textures[map_textures_ids[_layer][it->first][it->second].Get_texture_id()].Get_number_of_frames()==1)
                 {
                  if(map_textures_ids[_layer][it->first][it->second].Get_texture_frame_change_elapsed()>=map_textures[map_textures_ids[_layer][it->first][it->second].Get_texture_id()].Get_frame_change_delay()-1)
                     {
                      map_textures_ids[_layer][it->first][it->second].Trigger();
                     }
                 }
              else
                 {
                  map_textures_ids[_layer][it->first][it->second].Trigger();
                 }
             }
         }
     }
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_map_textures_animations[_layer][before_player][lights].begin();it!=fast_access_map_textures_animations[_layer][before_player][lights].end();it++)
     {
      if(!map_textures_ids[_layer][it->first][it->second].Is_done())
         {
          map_textures_ids[_layer][it->first][it->second].Decrease_remaining_duration();
          map_textures_ids[_layer][it->first][it->second].Update_texture_frame(map_textures[map_textures_ids[_layer][it->first][it->second].Get_texture_id()].Get_number_of_frames(),map_textures[map_textures_ids[_layer][it->first][it->second].Get_texture_id()].Get_frame_change_delay());
         }
     }
}

void Map::Decrease_clues_map_texture_id_remaining_time(bool before_player,bool lights)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_clues_map_textures[before_player][lights].begin();it!=fast_access_clues_map_textures[before_player][lights].end();it++)
     {
      if(!map_textures_ids[0][it->first][it->second].Is_done())
         {
          clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Decrease_remaining_duration();
          clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Update_texture_frame(map_textures[clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_id()].Get_number_of_frames(),map_textures[clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_id()].Get_frame_change_delay());
          if(map_textures[clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_id()].Is_passer() && clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_frame()==map_textures[clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_id()].Get_number_of_frames()-1)
             {
              clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Trigger();
             }
         }
     }
}

void Map::Decrease_clues_map_textures_ids_remaining_time()
{
 for(int i=0;i<=1;i++)
     for(int j=0;j<=1;j++)
         {
          Decrease_clues_map_texture_id_remaining_time(i,j);
         }
}

void Map::Decrease_special_clues_map_texture_id_remaining_time()
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_special_clues_map_textures.begin();it!=fast_access_special_clues_map_textures.end();it++)
     {
      if(!map_textures_ids[0][it->first][it->second].Is_done())
         {
          special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Decrease_remaining_duration();
          special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Update_texture_frame(map_textures[special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_id()].Get_number_of_frames(),map_textures[special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_id()].Get_frame_change_delay());
          if(map_textures[special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_id()].Is_passer() && special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_frame()==map_textures[special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Get_texture_id()].Get_number_of_frames()-1)
             {
              special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Trigger();
             }
         }
     }
}


void Map::Update_all_frames()
{
 current_number_of_updates++;
 Decrease_map_textures_ids_remaining_time();
 Decrease_clues_map_textures_ids_remaining_time();
 Decrease_special_clues_map_texture_id_remaining_time();
}

bool Map::Is_done()
{
 return (current_number_of_updates>number_of_updates && number_of_updates>0);
}

void Map::Load(std::bitset<NUMBER_OF_MAX_KEYS> *_keys)
{
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"maps/");
 strcat(path,name);
 strcat(path,".pwm");
 FILE *where=fopen(path,"r");
 int _is_interest_point,_is_static;
 fscanf(where,"%d %d %d %d %d %d ",&_is_interest_point,&_is_static,&number_of_updates,&number_of_lines,&number_of_columns,&number_of_layers);
 is_interest_point=_is_interest_point;
 is_static=_is_static;

 if(is_interest_point)
    {
     TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
     SDL_Color color={255,255,255};
     Texture *image=Create_TTF_Texture(font,name,color);
     Texture *aux=Load_Transparent_Texture("images/game/map_name_background.png");
     name_image=Create_Transparent_Texture(aux->w,aux->h);
     Apply_Texture(0,0,aux,name_image);
     Destroy_Texture(aux);
     Apply_Texture((name_image->w-image->w)/2,(name_image->h-image->h)/2,image,name_image);
     Destroy_Texture(image);
     TTF_CloseFont(font);
     Set_Texture_Blend_Mode(name_image,SDL_BLENDMODE_BLEND);
     name_image_opaque_time=NAME_IMAGE_OPAQUE_TIME;
     name_image_alpha=SDL_ALPHA_OPAQUE;
    }

 for(int k=0;k<number_of_layers;k++)
     for(int i=0;i<=1 && is_static;i++)
         for(int j=0;j<=1;j++)
             {
              map_image[k][i][j]=Create_Transparent_Texture(50*PIXELS_PER_INGAME_UNIT,50*PIXELS_PER_INGAME_UNIT);
             }

 int number_of_locks=0;
 fscanf(where,"%d ",&number_of_locks);
 for(int i=0;i<number_of_locks;i++)
     {
      int x,y,key_id,texture_id;
      fscanf(where,"%d %d %d %d ",&x,&y,&key_id,&texture_id);
      if((*_keys)[key_id] && map_textures_ids[0][x][y].Get_id()==0)
         map_textures_ids[0][x][y].Set_id(texture_id);
     }

 std::vector<Map_texture_id> textures_ids;

 for(int k=0;k<number_of_layers;k++)
     {
      for(int i=0;i<number_of_lines;i++)
          {
           for(int j=0;j<number_of_columns;j++)
               {
                int __id;
                fscanf(where,"%d ",&__id);
                if(map_textures_ids[k][i][j].Get_id()!=0)
                   map_textures_ids[k][i][j].Load();
                else
                   map_textures_ids[k][i][j].Load(__id);

                map_textures_ids[k][i][j].Get_all_textures_ids(textures_ids);

                for(std::vector<Map_texture_id>::iterator it=textures_ids.begin();it!=textures_ids.end();it++)
                    {
                     if(map_textures.count(it->Get_id())==0 && it->Get_id()!=0)
                        {
                         map_textures[it->Get_id()].Set_id(it->Get_id());
                         map_textures[it->Get_id()].Load();
                        }
                    }

                if(map_textures[map_textures_ids[k][i][j].Get_texture_id()].Is_animation())
                   fast_access_map_textures_animations[k][map_textures[map_textures_ids[k][i][j].Get_texture_id()].Get_print_before_player()][map_textures[map_textures_ids[k][i][j].Get_texture_id()].Is_light()].push_back(std::make_pair(i,j));
                else
                   Print_image(j*PIXELS_PER_INGAME_UNIT,i*PIXELS_PER_INGAME_UNIT,map_image[k][map_textures[map_textures_ids[k][i][j].Get_texture_id()].Get_print_before_player()][map_textures[map_textures_ids[k][i][j].Get_texture_id()].Is_light()],&map_textures_ids[k][i][j]);
               }
          }
     }

 const int dirx[]={0,0,1,0,-1,1,1,-1,-1};
 const int diry[]={0,1,0,-1,0,1,-1,1,-1};
 std::map<std::pair<int,int>,bool> viz;
 std::vector<std::pair<int,int> > new_elements[2];
 for(int k=0;k<NUMBER_OF_LAYERS_MAX;k++)
     for(int before_player=0;before_player<=1;before_player++)
         for(std::vector<std::pair<int,int> >::iterator it=fast_access_map_textures_animations[k][before_player][true].begin();it!=fast_access_map_textures_animations[k][before_player][true].end();it++)
             {
              int i=it->first,j=it->second;
              for(int dir=0;dir<9;dir++)
                  {
                   map_textures_ids[k][i+dirx[dir]][j+diry[dir]].Get_all_textures_ids(textures_ids);
                   for(std::vector<Map_texture_id>::iterator itt=textures_ids.begin();itt!=textures_ids.end();itt++)
                       {
                        if(viz.count(std::make_pair(i+dirx[dir],j+diry[dir]))==0)
                           {
                            new_elements[map_textures[itt->Get_id()].Get_print_before_player()].push_back(std::make_pair(i+dirx[dir],j+diry[dir]));
                            viz[std::make_pair(i+dirx[dir],j+diry[dir])]=true;
                           }
                       }
                  }
             }
 for(int before_player=0;before_player<=1;before_player++)
     {
      for(std::vector<std::pair<int,int> >::iterator it=new_elements[before_player].begin();it!=new_elements[before_player].end();it++)
          for(int k=0;k<number_of_layers;k++)
              {
               map_textures_ids[k][it->first][it->second].Get_all_textures_ids(textures_ids);
               for(std::vector<Map_texture_id>::iterator itt=textures_ids.begin();itt!=textures_ids.end();itt++)
                   {
                    if(map_textures[itt->Get_id()].Is_animation())
                       {
                        for(std::vector<std::pair<int,int> >::iterator _it=fast_access_map_textures_animations[k][map_textures[itt->Get_id()].Get_print_before_player()][map_textures[itt->Get_id()].Is_light()].begin();_it!=fast_access_map_textures_animations[k][map_textures[itt->Get_id()].Get_print_before_player()][map_textures[itt->Get_id()].Is_light()].end();_it++)
                            {
                             if(_it->first==it->first && _it->second==it->second)
                                {
                                 fast_access_map_textures_animations[k][map_textures[itt->Get_id()].Get_print_before_player()][map_textures[itt->Get_id()].Is_light()].erase(_it);
                                 break;
                                }
                            }
                       }
                   }
               fast_access_map_textures_animations[k][before_player][true].push_back(*it);
              }
      std::vector<std::pair<int,int> >().swap(new_elements[before_player]);
     }
 viz.clear();

 Read_Commands(where,_keys);
 fclose(where);
}

const int number_of_commands=6;
const char *map_command_names[number_of_commands]={"trigger","level_changer","key","clue","special_clue","puzzle"};

int Map::Get_command_id(char *_command_name)
{
 for(int i=0;i<number_of_commands;i++)
     if(strcmp(map_command_names[i],_command_name)==0)
        return i;
 return -1;
}

void Map::Read_Commands(FILE *where,std::bitset<NUMBER_OF_MAX_KEYS> *_keys)
{
 while(true)
       {
        char command_name[TEXT_LENGTH_MAX];
        fscanf(where,"%s",command_name);
        if(feof(where))
           break;
        switch(Get_command_id(command_name))
               {
                case 0: Read_trigger_command(where);
                        break;
                case 1: Read_level_changer_command(where);
                        break;
                case 2: Read_key_command(where);
                        break;
                case 3: Read_clue_command(where,_keys);
                        break;
                case 4: Read_special_clue_command(where,_keys);
                        break;
                case 5: Read_puzzle_command(where);
                        break;
               }
       }
}

void Map::Read_trigger_command(FILE *where)
{
 int x,y,number_of_targets;
 fscanf(where,"%d %d %d ",&x,&y,&number_of_targets);
 for(int i=0;i<number_of_targets;i++)
   {
    int x1,y1;
    fscanf(where,"%d %d ",&x1,&y1);
    map_textures_ids[0][x][y].Add_target(x1,y1);
   }
}

void Map::Read_level_changer_command(FILE *where)
{
 int x,y,x1,y1,_texture;
 char map_name[TEXT_LENGTH_MAX];
 fscanf(where,"%d %d %d %d %d ",&_texture,&x,&y,&x1,&y1);
 fgets(map_name,sizeof map_name,where);
 if(map_name[strlen(map_name)-1]=='\n')
    map_name[strlen(map_name)-1]=NULL;
 map_textures_ids[0][x][y].Set_texture_player_map_pos(_texture,x1,y1);
 if(Get_map_texture_type(_texture,x,y)!=6 && strcmp(map_name,"~")!=0)
    map_textures_ids[0][x][y].Set_texture_map_name(_texture,map_name);
}

void Map::Read_key_command(FILE *where)
{
 int x,y,key_id,_texture;
 fscanf(where,"%d %d %d %d ",&_texture,&x,&y,&key_id);
 map_textures_ids[0][x][y].Set_key_id(_texture,key_id);
}

void Map::Read_clue_command(FILE *where,std::bitset<NUMBER_OF_MAX_KEYS> *_keys)
{
 int id,x,y,n_keys;
 fscanf(where,"%d %d %d %d ",&id,&x,&y,&n_keys);
 bool locked=false;
 for(int j=0;j<n_keys;j++)
     {
      int __key;
      fscanf(where,"%d ",&__key);
      if(!(*_keys)[__key])
         locked=true;
     }
 std::vector<Map_texture_id> textures_ids;
 if(!locked && clues_map_textures_ids[x][y].Get_id()==0)
    {
     clues_map_textures_ids[x][y].Load(id);
     clues_map_textures_ids[x][y].Get_interactive_map_texture()->Get_all_textures_ids(textures_ids);
     for(std::vector<Map_texture_id>::iterator it=textures_ids.begin();it!=textures_ids.end();it++)
         {
          if(map_textures.count(it->Get_id())==0 && it->Get_id()!=0)
             {
              map_textures[it->Get_id()].Set_id(it->Get_id());
              map_textures[it->Get_id()].Load();
             }
         }
     fast_access_clues_map_textures[map_textures[clues_map_textures_ids[x][y].Get_interactive_map_texture()->Get_texture_id()].Get_print_before_player()][map_textures[clues_map_textures_ids[x][y].Get_interactive_map_texture()->Get_texture_id()].Is_light()].push_back(std::make_pair(x,y));
    }
}

void Map::Read_special_clue_command(FILE *where,std::bitset<NUMBER_OF_MAX_KEYS> *_keys)
{
 int id,x,y,n_keys;
 fscanf(where,"%d %d %d %d ",&id,&x,&y,&n_keys);
 bool locked=false;
 for(int i=0;i<n_keys;i++)
     {
      int __key;
      fscanf(where,"%d ",&__key);
      if(!(*_keys)[__key])
         locked=true;
     }
 std::vector<Map_texture_id> textures_ids;
 if(!locked && special_clues_map_textures_ids[x][y].Get_id()==0)
    {
     special_clues_map_textures_ids[x][y].Load(id);
     special_clues_map_textures_ids[x][y].Get_interactive_map_texture()->Get_all_textures_ids(textures_ids);
     for(std::vector<Map_texture_id>::iterator it=textures_ids.begin();it!=textures_ids.end();it++)
         {
          if(map_textures.count(it->Get_id())==0 && it->Get_id()!=0)
             {
              map_textures[it->Get_id()].Set_id(it->Get_id());
              map_textures[it->Get_id()].Load();
             }
         }
     fast_access_special_clues_map_textures.push_back(std::make_pair(x,y));
    }
}

void Map::Read_puzzle_command(FILE *where)
{
 int x,y,_texture;
 char puzzle_name[TEXT_LENGTH_MAX];
 fscanf(where,"%d %d %d ",&_texture,&x,&y);
 fgets(puzzle_name,sizeof puzzle_name,where);
 if(puzzle_name[strlen(puzzle_name)-1]=='\n')
    puzzle_name[strlen(puzzle_name)-1]=NULL;
 map_textures_ids[0][x][y].Set_texture_puzzle_name(_texture,puzzle_name);
}

const int MAP_IMAGE_WEIGHT=840,MAP_IMAGE_HEIGHT=680;

void Map::Print_Layer(int screen_x,int screen_y,int map_x,int map_y,Texture *_screen,int _layer,bool before_player,bool lights)
{
 Apply_Texture(map_x*PIXELS_PER_INGAME_UNIT,map_y*PIXELS_PER_INGAME_UNIT,screen_x,screen_y,std::min(MAP_IMAGE_WEIGHT,number_of_columns*PIXELS_PER_INGAME_UNIT),std::min(MAP_IMAGE_HEIGHT,number_of_lines*PIXELS_PER_INGAME_UNIT),map_image[_layer][before_player][lights],_screen);
}

void Map::Print_Layer_Animations(int screen_x,int screen_y,int map_x,int map_y,Texture *_screen,int _layer,bool before_player,bool lights)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_map_textures_animations[_layer][before_player][lights].begin();it!=fast_access_map_textures_animations[_layer][before_player][lights].end();it++)
     {
      if(!map_textures_ids[_layer][it->first][it->second].Is_done() && it->second>=map_x && it->first>=map_y && it->first<map_y+MAP_IMAGE_HEIGHT/PIXELS_PER_INGAME_UNIT && it->second<map_x+MAP_IMAGE_WEIGHT/PIXELS_PER_INGAME_UNIT)
         Print_image((it->second-map_x)*PIXELS_PER_INGAME_UNIT+screen_x,(it->first-map_y)*PIXELS_PER_INGAME_UNIT+screen_y,_screen,&map_textures_ids[_layer][it->first][it->second]);
     }
}

void Map::Print(int screen_x,int screen_y,int map_x,int map_y,Texture *_screen,bool before_player,bool lights)
{
 for(int k=number_of_layers-1;k>=0;k--)
     {
      Print_Layer(screen_x,screen_y,map_x,map_y,_screen,k,before_player,lights);
      Print_Layer_Animations(screen_x,screen_y,map_x,map_y,_screen,k,before_player,lights);
     }
}

void Map::Print_image(int screen_x,int screen_y,Texture *_screen,Interactive_map_texture *source)
{
 map_textures[source->Get_texture_id()].Print_image(screen_x,screen_y,_screen,source->Get_texture_frame());
}

void Map::Trigger(int x,int y)
{
 std::vector<std::pair<int,int> > target_map_position;
 for(int k=0;k<number_of_layers;k++)
     map_textures_ids[k][x][y].Trigger();
 map_textures_ids[0][x][y].Get_all_targets_map_positions(target_map_position);
 for(std::vector<std::pair<int,int> >::iterator it=target_map_position.begin();it!=target_map_position.end();it++)
     Trigger(it->first,it->second);
}

void Map::Print_name_image(Texture *_screen)
{
 if(!is_interest_point)
    return;
 Apply_Texture((RESOLUTION_W-name_image->w)/2,PIXELS_PER_INGAME_UNIT,name_image,_screen);
}

void Map::Update_name_image()
{
 if(name_image_alpha==0 || name_image_opaque_time-->0)
    return;
 name_image_alpha-=10;
 if(name_image_alpha<0)
    name_image_alpha=0;
 Set_Texture_Alpha(name_image,name_image_alpha);
}

void Map::Print_Clues(int screen_x,int screen_y,int map_x,int map_y,Texture *_screen,bool before_player,bool lights)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_clues_map_textures[before_player][lights].begin();it!=fast_access_clues_map_textures[before_player][lights].end();it++)
     {
      if(!clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Is_done() && it->second>=map_x && it->first>=map_y && it->first<map_y+MAP_IMAGE_HEIGHT/PIXELS_PER_INGAME_UNIT && it->second<map_x+MAP_IMAGE_WEIGHT/PIXELS_PER_INGAME_UNIT)
         Print_image((it->second-map_x)*PIXELS_PER_INGAME_UNIT+screen_x,(it->first-map_y)*PIXELS_PER_INGAME_UNIT+screen_y,_screen,clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture());
     }
}

void Map::Print_Special_Clues(int screen_x,int screen_y,int map_x,int map_y,Texture *_screen)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_special_clues_map_textures.begin();it!=fast_access_special_clues_map_textures.end();it++)
     {
      if(!special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture()->Is_done() && it->second>=map_x && it->first>=map_y && it->first<map_y+MAP_IMAGE_HEIGHT/PIXELS_PER_INGAME_UNIT && it->second<map_x+MAP_IMAGE_WEIGHT/PIXELS_PER_INGAME_UNIT)
         Print_image((it->second-map_x)*PIXELS_PER_INGAME_UNIT+screen_x,(it->first-map_y)*PIXELS_PER_INGAME_UNIT+screen_y,_screen,special_clues_map_textures_ids[it->first][it->second].Get_interactive_map_texture());
     }
}

Clue_map_texture *Map::Get_Clue_map_texture(int x,int y)
{
 return &clues_map_textures_ids[x][y];
}

Clue_map_texture *Map::Get_Special_Clue_map_texture(int x,int y)
{
 return &special_clues_map_textures_ids[x][y];
}
