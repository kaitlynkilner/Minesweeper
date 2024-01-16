#include <iostream>
#include <fstream>
#include "welcome.h"

int main()
{
    string scols;
    string srows;
    string smines;
    int cols;
    int rows;
    int mines;
    int height;
    int width;
    ifstream config;
    config.open("files/board_config.cfg", ios::in);
    if(config.is_open()){
        getline(config, scols);
        getline(config, srows);
        getline(config, smines);
        cols = stoi(scols);
        rows = stoi(srows);
        mines = stoi(smines);
    }
    width = cols * 32;

    height = (rows * 32) +100;
    vector<vector<int>> vec(rows, vector<int> (cols, 0));
    Welcome welcomewindow;
    Board gamewindow;
    GameBoard gameboard;
    //gameboard.Images();
    //gamewindow.GenerateBoard(rows, cols);
    //gamewindow.GetMines(mines, rows, cols);
    //gamewindow.GetNumbers(mines, rows, cols);
    
   
    welcomewindow.OpenWindow(width, height);
    if(welcomewindow.ifclosed == false){
        gameboard.OpenBoard(cols, rows, mines, welcomewindow.displayname);}
    //gamewindow.PrintBoard(rows, cols);
    
    
    return 0;
}
