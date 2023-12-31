/*
**  ______   ______     ______     __    __     ______     __   __
** /\  == \ /\  __ \   /\  ___\   /\ "-./  \   /\  __ \   /\ "-.\ \
** \ \  _-/ \ \  __ \  \ \ \____  \ \ \-./\ \  \ \  __ \  \ \ \-.  \
**  \ \_\    \ \_\ \_\  \ \_____\  \ \_\ \ \_\  \ \_\ \_\  \ \_\\"\_\
**   \/_/     \/_/\/_/   \/_____/   \/_/  \/_/   \/_/\/_/   \/_/ \/_/
**
*/

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <memory>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#pragma once

struct node {
    int g_cost;
    int f_cost;
    int h_cost;
    int visited;
    struct node *parent;
    int x;
    int y;
    int has_parent;
    int opened;
    int initialize;
} typedef t_node;

struct open {
    int x;
    int y;
    struct open *next;
    struct open *prev;
} typedef t_open;

struct my_open {
    int x;
    int y;
} typedef t_myopen;

struct par {
    t_node **nodes;
    t_node best_node;
    t_open *open;
    t_open *tail;
    t_open *best_open;
    t_myopen *my_open;
    void *pool;
    char *map;
    char **arr;
    int arr_len;
    int str_len;
    int end_x;
    int end_y;
    int current_x;
    int current_y;
    int list_len;
    int is_possible;
    int lowest_fcost;
    int counter;
} typedef t_par;

int get_lines_str(char *str);

char *get_map(char *path);

int get_line_lenght(char *map);

char **map_to_arr(char *map);

int my_arrlen(char **arr);

int my_strlen (char const *str);

int get_size(char *path);

void get_endpos(t_par *par);

void malloc_list(t_par *par);

void get_hcost(t_par *par, int x, int y);

void get_gcost(t_par *par, int x, int y);

void get_fcosts(t_par *par, int x, int y);

void add_toopen (t_open **open, int x, int y, t_par *par);

int is_inopen(t_par *par, int x, int y);

int is_accessible(t_par *par, int x, int y);

int get_pos(t_par *par, int x, int y);

void remove_open (t_par *par, t_open *prev, t_open *old, int position);

void discover_node(t_par *par, int x, int y, t_node *parent);

void check_cost(t_par *par, t_node *parent, int x, int y);

void check_right(int x, int y, t_par *par);

void check_left(int x, int y, t_par *par);

void check_up(int x, int y, t_par *par);

void check_down(int x, int y, t_par *par);

void check_allsquares(t_par *par, int x, int y);

void pick_square(t_par *par);

char **str_to_array (char *str);

void write_tomap(t_par *par);

int check_errors(int ac, char **av);

int is_a_file(const char *path);

int list_len (t_par *par);

void is_possible(t_par *par);

void free_list(t_par *par);

void initialize_nodes(t_par *par);

void free_arr(t_par *par);

void free_linked(t_par *par);

void free_nodes(t_par *par);

t_myopen newNode(int x, int y);

t_myopen send_next_pos(t_par *par);
