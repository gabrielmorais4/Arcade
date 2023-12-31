/*
** EPITECH PROJECT, 2022
** a_star
** File description:
** discover_node
*/

#include "pathfinding.hpp"

void discover_node(t_par *par, int x, int y, t_node *parent)
{
    if (par->nodes[y][x].parent)
        return;
    par->nodes[y][x].parent = parent;
    par->nodes[y][x].visited = 0;
    par->nodes[y][x].x = x;
    par->nodes[y][x].y = y;
    get_hcost(par, x, y);
    get_gcost(par, x, y);
    get_fcosts(par, x, y);
    add_toopen(&par->open, x, y, par);
    par->nodes[0][0].parent = NULL;
}

int is_inopen(t_par *par, int x, int y)
{
    t_open **temp = &par->open;
    t_open *tmp = *temp;
    if (par->nodes[y][x].visited)
        return 0;
    if (par->nodes[y][x].opened)
        return 1;
    return 0;
}

void update_node(t_par *par, t_node *parent, int x, int y)
{
    par->nodes[y][x].parent = parent;
    get_hcost(par, x, y);
    get_gcost(par, x, y);
    get_fcosts(par, x, y);
}

void check_cost(t_par *par, t_node *parent, int x, int y)
{
    int tmp = parent->g_cost + 10;
    if (tmp > par->nodes[y][x].g_cost) {
        par->nodes[0][0].parent = NULL;
        update_node(par, parent, x, y);
    }
}
