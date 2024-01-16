#include <SFML/Graphics.hpp>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <sstream>
using namespace std;
#pragma once


int Random(int min, int max, mt19937 &random_mt){
    uniform_int_distribution<int> dist(min, max);
    return dist(random_mt);}


struct Board{
    //0 means no mines next to it
    //-5 means mine
    //any other number is the count
    
    vector<vector<int>> board;
    vector<vector<int>> mine_loc;
    
    void GenerateBoard(int rows, int cols){
        board.resize(rows, vector<int> (cols, 0));
    }
    
    void GetMines(int mines, int row, int col, int rownum, int colnum){
        mt19937 random_mt (time(nullptr));
        mine_loc.resize(mines, vector<int> (2, 0));
        for(int i = 0; i < mines; i++){
            // find out way to check for repeats
            int randcol = Random(0, col-1, random_mt);
            int randrow = Random(0, row-1,  random_mt);
            while(randrow == rownum && randcol == colnum){
                randcol = Random(0, col-1, random_mt);
                randrow = Random(0, row-1, random_mt);
            }
            for(int j = 0; j < mine_loc.size(); j++){
                if(mine_loc[j][0] == randrow && mine_loc[j][1] == randcol){
                    while(mine_loc[j][0] == randrow && mine_loc[j][1] == randcol){
                        int tempcol = Random(0, col-1,  random_mt);
                        int temprow = Random(0, row-1,  random_mt);
                        if(tempcol != colnum && temprow != rownum){
                            randcol = tempcol;
                            randrow = temprow;}
                    }
                }            }
            
            if(board[randrow][randcol] == -5){
                while(board[randrow][randcol] == -5){
                    int tempcol = Random(0, col-1, random_mt);
                    int temprow = Random(0, row-1,  random_mt);
                    if(tempcol != colnum && temprow != rownum){
                        randcol =tempcol;
                        randrow = temprow;}
                }
            }
            
            mine_loc[i][0] = randrow;
            mine_loc[i][1] = randcol;
            board[randrow][randcol] = -5;
            //-5 represents bomb
        }
    }
    void GetNumbers(int mines, int row, int col){
        //iterate through mine location, add 1 to surrounding numbers on board
        for(int i = 0; i < mines; i++){
                int rownum = mine_loc[i][0];
                int colnum = mine_loc[i][1];
                if(mine_loc[i][0] -1 >= 0 && mine_loc[i][1] -1 >= 0 && board[rownum -1][colnum-1] != -5){
                    board[rownum-1][colnum-1] += 1;
                }
                if(mine_loc[i][1] -1 >= 0  && board[rownum][colnum-1] != -5 ){
                    board[rownum][colnum-1] += 1;
                }
                if(mine_loc[i][0] +1 < row && mine_loc[i][1] -1 >= 0 && board[rownum+1][colnum-1] != -5){
                    board[rownum+1][colnum-1] += 1;
                    
                }
                if(mine_loc[i][0] +1 < row && board[rownum+1][colnum] != -5){
                    board[rownum+1][colnum] += 1;
                }
                if(mine_loc[i][0] + 1 < row && mine_loc[i][1] + 1 < col && board[rownum+1][colnum+1] != -5){
                    board[rownum+1][colnum+1] += 1;
                }
                if(mine_loc[i][1] + 1 < col && board[rownum][colnum+1] != -5 ){
                    board[rownum][colnum+1] += 1;
                }
                if(mine_loc[i][1] + 1 < col && mine_loc[i][0] -1 >= 0 && board[rownum-1][colnum+1] != -5 ){
                    board[rownum-1][colnum+1] += 1;
                }
                if(mine_loc[i][0] - 1 >= 0  && board[rownum-1][colnum] != -5 ){
                    board[rownum-1][colnum] += 1;
                }
        }}
    
};


class GameBoard{
    map<string, sf::Sprite> images;
    Board boardobj;
    vector<vector<int>> emptytiles;
    vector<vector<int>> numtiles;
    int arr[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};
    vector<vector<int>> zeros;
    bool ifdebug = false;
    vector<vector<int>> flags;
    int minecount;
    bool iflose = false;
    bool ifwin = false;
    int wincount;
    float totaltime;
    bool ifpause = false;
    int pausecount = 1;
    int leadercount = 1;
    chrono::high_resolution_clock::time_point start;
    chrono::high_resolution_clock::time_point end;
    int pausetime = 0;
    int temptime;
    bool iftemptime = false;
    chrono::high_resolution_clock::time_point startpause;
    chrono::high_resolution_clock::time_point endpause;
    int wintime;
    int winnum = 0;
    bool ifleaderboard = false;
    
    

public:
    
    bool CheckMine(int row, int col){
        if(boardobj.board[row][col] == -5){
            return true;
        }
        if(boardobj.board[row][col] != -5){
            return false;
        }
    }
    
    int OpenTile(int row, int col, int rownum, int colnum){
        if(boardobj.board[rownum][colnum] != 0 && zeros.size() != 0){
            bool ifinvec = false;
            for(int i = 0; i < numtiles.size(); i++){
                if(numtiles[i][0] == rownum && numtiles[i][1] == colnum){
                    ifinvec = true;
                }
            }
            bool ifflagged = false;
            for(int i = 0; i < flags.size(); i++){
                if(flags[i][0] == rownum && flags[i][1] == colnum){
                    ifflagged = true;
                }}
            
            if(ifinvec == false && ifflagged == false){
            vector<int> temp;
            temp.push_back(rownum);
            temp.push_back(colnum);
            numtiles.push_back(temp);
            vector<int> vec = zeros.back();
            zeros.pop_back();
            wincount ++;
            OpenTile(row, col, vec[0], vec[1]);
            }}
        if(boardobj.board[rownum][colnum] != 0 && zeros.size() == 0){
            vector<int> temp1;
            bool ifinvec = false;
            for(int i = 0; i < numtiles.size(); i++){
                if(numtiles[i][0] == rownum && numtiles[i][1] == colnum){
                    ifinvec = true;
                }
            }
            bool ifflagged = false;
            for(int i = 0; i < flags.size(); i++){
                if(flags[i][0] == rownum && flags[i][1] == colnum){
                    ifflagged = true;
                }}
            if(ifinvec == false && ifflagged == false){
            temp1.push_back(rownum);
            temp1.push_back(colnum);
            numtiles.push_back(temp1);
                wincount++;
            }}
        if(boardobj.board[rownum][colnum] == 0){
            bool ifinvec = false;
            for(int i = 0; i < emptytiles.size(); i++){
                if(emptytiles[i][0] == rownum && emptytiles[i][1] == colnum){
                    ifinvec = true;
                }
            }
            bool ifflagged = false;
            for(int i = 0; i < flags.size(); i++){
                if(flags[i][0] == rownum && flags[i][1] == colnum){
                    ifflagged = true;
                }}
            if(ifinvec == false && ifflagged == false){
            vector<int> temp2;
            temp2.push_back(rownum);
            temp2.push_back(colnum);
                emptytiles.push_back(temp2);
                wincount++;
            }
            if(rownum -1 >= 0 && colnum -1 >= 0){
                vector<int> temp3;
                temp3.push_back(rownum-1);
                temp3.push_back(colnum-1);
                bool ifin = false;
                for(int i = 0; i < zeros.size(); i++){
                    if(zeros[i][0] == temp3[0] && zeros[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < emptytiles.size(); i++){
                    if(emptytiles[i][0] == temp3[0] && emptytiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < numtiles.size(); i++){
                    if(numtiles[i][0] == temp3[0] && numtiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < flags.size(); i++){
                    if(flags[i][0] == rownum && flags[i][1] == colnum){
                        ifin = true;
                    }}
                if(ifin == false){
                    zeros.push_back(temp3);}
            }
            if(rownum-1 >= 0){
                vector<int> temp3;
                temp3.push_back(rownum-1);
                temp3.push_back(colnum);
                bool ifin = false;
                for(int i = 0; i < zeros.size(); i++){
                    if(zeros[i][0] == temp3[0] && zeros[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < emptytiles.size(); i++){
                    if(emptytiles[i][0] == temp3[0] && emptytiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < numtiles.size(); i++){
                    if(numtiles[i][0] == temp3[0] && numtiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < flags.size(); i++){
                    if(flags[i][0] == rownum && flags[i][1] == colnum){
                        ifin = true;
                    }}
                if(ifin == false){
                    zeros.push_back(temp3);}
            }
            if(rownum - 1 >= 0 && colnum + 1 < col){
                vector<int> temp3;
                temp3.push_back(rownum-1);
                temp3.push_back(colnum+1);
                bool ifin = false;
                for(int i = 0; i < zeros.size(); i++){
                    if(zeros[i][0] == temp3[0] && zeros[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < emptytiles.size(); i++){
                    if(emptytiles[i][0] == temp3[0] && emptytiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < numtiles.size(); i++){
                    if(numtiles[i][0] == temp3[0] && numtiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < flags.size(); i++){
                    if(flags[i][0] == rownum && flags[i][1] == colnum){
                        ifin = true;
                    }}
                if(ifin == false){
                    zeros.push_back(temp3);}
            }
            if(colnum + 1 < col){
                vector<int> temp3;
                temp3.push_back(rownum);
                temp3.push_back(colnum+1);
                bool ifin = false;
                for(int i = 0; i < zeros.size(); i++){
                    if(zeros[i][0] == temp3[0] && zeros[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < emptytiles.size(); i++){
                    if(emptytiles[i][0] == temp3[0] && emptytiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < numtiles.size(); i++){
                    if(numtiles[i][0] == temp3[0] && numtiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < flags.size(); i++){
                    if(flags[i][0] == rownum && flags[i][1] == colnum){
                        ifin = true;
                    }}
                if(ifin == false){
                    zeros.push_back(temp3);}
            }
            if(rownum + 1 < row && colnum+1 < col){
                vector<int> temp3;
                temp3.push_back(rownum+1);
                temp3.push_back(colnum+1);
                bool ifin = false;
                for(int i = 0; i < zeros.size(); i++){
                    if(zeros[i][0] == temp3[0] && zeros[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < emptytiles.size(); i++){
                    if(emptytiles[i][0] == temp3[0] && emptytiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < numtiles.size(); i++){
                    if(numtiles[i][0] == temp3[0] && numtiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < flags.size(); i++){
                    if(flags[i][0] == rownum && flags[i][1] == colnum){
                        ifin = true;
                    }}
                if(ifin == false){
                    zeros.push_back(temp3);}
            }
            if(rownum + 1 < row){
                vector<int> temp3;
                temp3.push_back(rownum+1);
                temp3.push_back(colnum);
                bool ifin = false;
                for(int i = 0; i < zeros.size(); i++){
                    if(zeros[i][0] == temp3[0] && zeros[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < emptytiles.size(); i++){
                    if(emptytiles[i][0] == temp3[0] && emptytiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < numtiles.size(); i++){
                    if(numtiles[i][0] == temp3[0] && numtiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < flags.size(); i++){
                    if(flags[i][0] == rownum && flags[i][1] == colnum){
                        ifin = true;
                    }}
                if(ifin == false){
                    zeros.push_back(temp3);}
            }
            if(rownum + 1 < row && colnum - 1 >= 0){
                vector<int> temp3;
                temp3.push_back(rownum+1);
                temp3.push_back(colnum-1);
                bool ifin = false;
                for(int i = 0; i < zeros.size(); i++){
                    if(zeros[i][0] == temp3[0] && zeros[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < emptytiles.size(); i++){
                    if(emptytiles[i][0] == temp3[0] && emptytiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < numtiles.size(); i++){
                    if(numtiles[i][0] == temp3[0] && numtiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < flags.size(); i++){
                    if(flags[i][0] == rownum && flags[i][1] == colnum){
                        ifin = true;
                    }}
                if(ifin == false){
                    zeros.push_back(temp3);}
            }
            if(colnum - 1 >= 0){
                vector<int> temp3;
                temp3.push_back(rownum);
                temp3.push_back(colnum-1);
                bool ifin = false;
                for(int i = 0; i < zeros.size(); i++){
                    if(zeros[i][0] == temp3[0] && zeros[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < emptytiles.size(); i++){
                    if(emptytiles[i][0] == temp3[0] && emptytiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < numtiles.size(); i++){
                    if(numtiles[i][0] == temp3[0] && numtiles[i][1] == temp3[1]){
                        ifin = true;
                    }
                }
                for(int i = 0; i < flags.size(); i++){
                    if(flags[i][0] == rownum && flags[i][1] == colnum){
                        ifin = true;
                    }}
                if(ifin == false){
                    zeros.push_back(temp3);}
            }
            for(int i = 0; i < zeros.size(); i++){
                int nrow = zeros[i][0];
                int ncol = zeros[i][1];
                if(boardobj.board[nrow][ncol] != 0){
                    vector<int> newvec;
                    newvec.push_back(nrow);
                    newvec.push_back(ncol);
                    numtiles.push_back(newvec);
                    wincount++;
                    zeros.erase(zeros.begin()+i);
                }
            }
            if(zeros.size()!= 0){
            vector<int> vec = zeros.back();
            zeros.pop_back();
                OpenTile(row, col, vec[0], vec[1]);}
        }
        
        
        
    }
    void OpenLeaderBoard(int cols, int rows, string displayname, int totaltime){
        ifstream leaderboard;
        ofstream updatedboard;
        leaderboard.open("files/leaderboard.txt", ios::in);
        vector<string> names;
        vector<string> times;
        string singleline;
        if(leaderboard.is_open()){
            while(getline(leaderboard,singleline)){
                string stime;
                string name;
                istringstream stream(singleline);
                getline(stream, stime, ',');
                getline(stream, name, ',');
                names.push_back(name);
                times.push_back(stime);
                         }}
        int ifbigger = 6;
        int minutes;
        int seconds;
        minutes = (totaltime/420)/60;
        seconds = abs(totaltime/420)-(minutes*60);
        string sminutes = to_string(minutes);
        string sseconds = to_string(seconds);
        string stotaltime = sminutes + sseconds;
        int finaltime = stoi(stotaltime);
        
        if(ifwin){
            winnum++;
            for(int i = 0; i < times.size(); i++){
                string temptimes = times.at(i);
                temptimes.erase(remove(temptimes.begin(), temptimes.end(), ':'), temptimes.end());
                int inttime = stoi(temptimes);
                if(finaltime < inttime){
                    ifbigger = i;
                    break;
                }
            }
        
        if(ifbigger != 6 && winnum < 3){
            if(stoi(sseconds) <10){
                sseconds = "0"+sseconds;
            }
            times.insert(times.begin()+ifbigger, sminutes+":"+sseconds);
            names.insert(names.begin()+ifbigger, displayname);
            times.pop_back();
            names.pop_back();
        }
        updatedboard.open("files/leaderboard.txt", ios::out);
        for(int i = 0; i < names.size(); i++){
            updatedboard << times.at(i) << "," << names.at(i) << endl;
        }}
        string newstring;
        for(int i = 0; i < 5; i++){
            if(names.at(i) == displayname){
                newstring += to_string(i+1)+"."+"\t"+times.at(i)+"\t"+names.at(i)+"*";
                newstring += "\n\n";
            }
            else{
                newstring += to_string(i+1)+"."+"\t"+times.at(i)+"\t"+names.at(i);
                newstring += "\n\n";}
        }
        sf::Font font;
        font.loadFromFile("files/font.ttf");
        sf::Text text2;
        text2.setFont(font);
        text2.setString(newstring);
        text2.setCharacterSize(18);
        text2.setStyle(sf::Text::Bold);
        sf::FloatRect textrect2 = text2.getLocalBounds();
        text2.setOrigin(textrect2.left + textrect2.width/2.0f, textrect2.top + textrect2.height/2.0f);
        text2.setPosition(sf::Vector2f((cols*16)/2.0f ,(((rows*16)+50)/2.0f)+20));
        
        sf::RenderWindow leader(sf::VideoMode(cols*16, (rows*16)+50),"Minesweeper", sf::Style::Close);
        sf::RectangleShape screen(sf::Vector2f(cols*16, (rows*16)+50));
        screen.setFillColor(sf::Color::Blue);
        sf::Text text;
        text.setFont(font);
        text.setString("LEADERBOARD");
        text.setCharacterSize(20);
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
        sf::FloatRect textrect = text.getLocalBounds();
        text.setOrigin(textrect.left + textrect.width/2.0f, textrect.top + textrect.height/2.0f);
        text.setPosition(sf::Vector2f((cols*16)/2.0f ,((rows*16+50)/2.0f)-120));
        while(leader.isOpen()){
            if(ifwin == false){
                start = chrono::high_resolution_clock::now();}
            sf::Event playere;
            while(leader.pollEvent(playere)){
                if(playere.type == playere.Closed){
                    if(ifwin == false &&iflose == false){
                        leadercount++;
                        //ifpause = false;
                        ifleaderboard = false;
                        iftemptime = false;}
                    leader.close();}}
                
            
                leader.clear();
                leader.draw(screen);
                leader.draw(text);
                leader.draw(text2);
                leader.display();
            if(ifwin == false){
                end = chrono::high_resolution_clock::now();
                std::chrono::duration<float, milli> runtime = end - start;
                totaltime -= runtime.count();
                totaltime += runtime.count();}
        }
        
    }
    
    
    void OpenBoard(int cols, int rows, int mines, string displayname){
        minecount = mines;
        int debugcount = 1;
        sf::RenderWindow game(sf::VideoMode(cols*32, (rows*32)+100),"Minesweeper", sf::Style::Close);
        sf::RectangleShape screen(sf::Vector2f(cols*32, 100));
        screen.setPosition(0, rows*32);
        screen.setFillColor(sf::Color::White);
        sf::Texture texture;
        sf::Texture texture2;
        sf::Texture texture3;
        sf::Texture texture4;
        sf::Texture texture5;
        sf::Texture texture6;
        sf::Texture texture7;
        sf::Texture texture8;
        sf::Texture texture9;
        sf::Texture texture10;
        sf::Texture texture11;
        sf::Texture texture12;
        sf::Texture texture13;
        sf::Texture texture14;
        sf::Texture texture15;
        sf::Texture texture16;
        sf::Texture texture17;
        sf::Texture texture18;
        sf::Texture texture19;
        sf::Texture texture20;
        sf::Sprite sprite;
        sf::Sprite sprite2;
        sf::Sprite sprite3;
        sf::Sprite sprite4;
        sf::Sprite sprite5;
        sf::Sprite sprite6;
        sf::Sprite sprite7;
        sf::Sprite sprite8;
        sf::Sprite sprite9;
        sf::Sprite sprite10;
        sf::Sprite sprite11;
        sf::Sprite sprite12;
        sf::Sprite sprite13;
        sf::Sprite sprite14;
        sf::Sprite sprite15;
        sf::Sprite sprite16;
        sf::Sprite sprite17;
        sf::Sprite sprite18;
        sf::Sprite sprite19;
        sf::Sprite sprite20;
       if(!texture.loadFromFile("files/images/tile_hidden.png")){
            cout << "file did not load" << endl;
        }
        sprite.setTexture(texture);
        images["tile_hidden"] = sprite;
        if(!texture2.loadFromFile("files/images/face_happy.png")){
            cout << "file did not load";
        }
        sprite2.setTexture(texture2);
        images["face_happy"] = sprite2;
        texture3.loadFromFile("files/images/tile_revealed.png");
        sprite3.setTexture(texture3);
        images["tile_revealed"] = sprite3;
        texture4.loadFromFile("files/images/number_1.png");
        sprite4.setTexture(texture4);
        images["1"] = sprite4;
        texture5.loadFromFile("files/images/number_2.png");
        sprite5.setTexture(texture5);
        images["2"] = sprite5;
        texture6.loadFromFile("files/images/number_3.png");
        sprite6.setTexture(texture6);
        images["3"] = sprite6;
        texture7.loadFromFile("files/images/number_4.png");
        sprite7.setTexture(texture7);
        images["4"] = sprite7;
        texture8.loadFromFile("files/images/number_5.png");
        sprite8.setTexture(texture8);
        images["5"] = sprite8;
        texture9.loadFromFile("files/images/number_6.png");
        sprite9.setTexture(texture9);
        images["6"] = sprite9;
        texture10.loadFromFile("files/images/number_7.png");
        sprite10.setTexture(texture10);
        images["7"] = sprite10;
        texture11.loadFromFile("files/images/number_8.png");
        sprite11.setTexture(texture11);
        images["8"] = sprite11;
        texture12.loadFromFile("files/images/mine.png");
        sprite12.setTexture(texture12);
        images["mine"] = sprite12;
        texture13.loadFromFile("files/images/debug.png");
        sprite13.setTexture(texture13);
        images["debug"] = sprite13;
        texture14.loadFromFile("files/images/flag.png");
        sprite14.setTexture(texture14);
        images["flag"] = sprite14;
        texture15.loadFromFile("files/images/face_lose.png");
        sprite15.setTexture(texture15);
        images["lose_face"] = sprite15;
        texture16.loadFromFile("files/images/face_win.png");
        sprite16.setTexture(texture16);
        images["win_face"] = sprite16;
        texture17.loadFromFile("files/images/digits.png");
        sprite17.setTexture(texture17);
        images["digits"] = sprite17;
        texture18.loadFromFile("files/images/leaderboard.png");
        sprite18.setTexture(texture18);
        images["leaderboard"] = sprite18;
        texture19.loadFromFile("files/images/pause.png");
        sprite19.setTexture(texture19);
        images["pause"] = sprite19;
        texture20.loadFromFile("files/images/play.png");
        sprite20.setTexture(texture20);
        images["play"] = sprite20;
        
        
        
        
        int rownum = 0;
        int colnum = 0;
        while(game.isOpen()){
            game.clear();
            if(iftemptime == false && ifwin == false && iflose == false){
                start = chrono::high_resolution_clock::now();}
            if(ifpause == false && leadercount % 2 != 0 && ifleaderboard == false){
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        int x = j*32;
                        int y = i*32;
                        images["tile_hidden"].setPosition(x, y);
                        game.draw(images["tile_hidden"]);
                    }}
            if(emptytiles.size() != 0){
                for(int i = 0; i < emptytiles.size(); i++){
                    for(int j = 0; j < 1; j++){
                        int x = emptytiles[i][1]*32;
                        int y = emptytiles[i][0]*32;
                        images["tile_revealed"].setPosition(x, y);
                        game.draw(images["tile_revealed"]);
                    }
                }}
            if(numtiles.size() != 0){
                for(int i = 0; i < numtiles.size(); i++){
                        int x = numtiles[i][0];
                        int y = numtiles[i][1];
                        string num = to_string(boardobj.board[x][y]);
                        images[num].setPosition(y*32, x*32);
                    game.draw(images[num]);
                }}
            if(flags.size() != 0){
                for(int i = 0; i < flags.size(); i++){
                        int x = flags[i][0];
                        int y = flags[i][1];
                        images["flag"].setPosition(y*32, x*32);
                    game.draw(images["flag"]);
                }}
                if(ifdebug){
                    for(int i = 0; i < boardobj.mine_loc.size(); i++){
                        int x = boardobj.mine_loc[i][0]*32;
                        int y = boardobj.mine_loc[i][1]*32;
                        images["mine"].setPosition(y, x);
                        game.draw(images["mine"]);
                    }
                }
            }
            else{
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        int x = j*32;
                        int y = i*32;
                        images["tile_revealed"].setPosition(x, y);
                        game.draw(images["tile_revealed"]);
                    }}
                if(leadercount %2 == 0 && ifwin == false && iflose == false){
                    OpenLeaderBoard(cols, rows, displayname, totaltime);
                }
            }
            if(ifwin && winnum==1){
                OpenLeaderBoard(cols, rows, displayname, wintime);
                //winnum++;
            }
            sf::Event playere;
            while(game.pollEvent(playere)){
                if(playere.type == playere.Closed){
                    game.close();}
                if(iflose || ifwin){
                    if(playere.type == sf::Event::MouseButtonPressed){
                        //cout << sf::Mouse::getPosition(game).x << ", " << sf::Mouse::getPosition(game).y << endl;
                        start = chrono::high_resolution_clock::now();
                        int mousex = sf::Mouse::getPosition(game).x;
                        int mousey = sf::Mouse::getPosition(game).y;
                        auto mousecoords = sf::Mouse::getPosition(game);
                        rownum = mousey/32;
                        colnum = mousex/32;
                        if(images["leaderboard"].getGlobalBounds().contains(mousex, mousey)){
                            //do some stuff
                            temptime = totaltime;
                            iftemptime = true;
                            OpenLeaderBoard(cols, rows, displayname, temptime);
                        }
                        if(ifwin){
                            if(images["win_face"].getGlobalBounds().contains(mousex, mousey)){
                                boardobj.board.clear();
                                boardobj.mine_loc.clear();
                                emptytiles.clear();
                                numtiles.clear();
                                flags.clear();
                                minecount = mines;
                                debugcount = 1;
                                ifdebug = false;
                                totaltime = 0;
                                temptime = 0;
                                winnum = 0;
                                ifwin = false;
                                leadercount = 1;
                                iftemptime = false;
                                wintime = 0;
                            }
                        }
                        if(iflose){
                            if(images["lose_face"].getGlobalBounds().contains(mousex, mousey)){
                                boardobj.board.clear();
                                boardobj.mine_loc.clear();
                                emptytiles.clear();
                                numtiles.clear();
                                flags.clear();
                                minecount = mines;
                                debugcount = 1;
                                ifdebug = false;
                                totaltime = 0;
                                temptime = 0;
                                winnum = 0;
                                iflose = false;
                                leadercount = 1;
                                iftemptime = false;
                            }
                        }
                        std::chrono::duration<float, milli> runtime = end - start;
                        totaltime -= runtime.count();
                        totaltime += runtime.count();
                    }}
                if(iflose == false && ifwin == false){
                if(playere.type == sf::Event::MouseButtonPressed){
                    //cout << sf::Mouse::getPosition(game).x << ", " << sf::Mouse::getPosition(game).y << endl;
                    int mousex = sf::Mouse::getPosition(game).x;
                    int mousey = sf::Mouse::getPosition(game).y;
                    auto mousecoords = sf::Mouse::getPosition(game);
                    rownum = mousey/32;
                    colnum = mousex/32;
                    //cout << rownum << ", " << colnum << endl;
                    if(ifpause == false){
                    if(rownum < rows && colnum < cols){
                        bool ifflag = false;
                        if(playere.mouseButton.button == sf::Mouse::Right){
                            for(int i = 0; i < flags.size(); i++){
                                if(rownum == flags[i][0] && colnum == flags[i][1]){
                                    flags.erase(flags.begin() + i);
                                    ifflag = true;
                                    minecount += 1;
                                }
                            }
                            if(ifflag == false){
                            vector<int> vec;
                            vec.push_back(rownum);
                            vec.push_back(colnum);
                            flags.push_back(vec);
                                minecount -=1;
                            }}
                        else{
                            bool ifflagged = false;
                            for(int i = 0; i < flags.size(); i++){
                                if(flags[i][0] == rownum && flags[i][1] == colnum){
                                    ifflagged = true;
                                }
                            }
                    if(boardobj.board.size() == 0){
                        if(ifflagged == false){
                            boardobj.GenerateBoard(rows, cols);
                            boardobj.GetMines(mines, rows, cols, rownum, colnum);
                            boardobj.GetNumbers(mines, rows, cols);
                        }}
                    
                        if(CheckMine(rownum, colnum) == false){
                            if(ifflagged == false){
                                OpenTile(rows, cols, rownum, colnum);}
                        }
                        if(CheckMine(rownum, colnum)){
                            if(ifflagged==false){
                                ifdebug = true;
                                iflose = true;
                            }}
                            
                        }
                        int flagnum=0;
                        for(int i = 0; i < flags.size(); i++){
                            for(int j = 0; j < boardobj.mine_loc.size(); j++){
                                if(flags[i][0] == boardobj.mine_loc[j][0] && flags[i][1] == boardobj.mine_loc[j][1]){
                                    flagnum+=1;
                                }
                            }}
                        if(emptytiles.size()+numtiles.size() == (rows*cols)-mines){
                            ifwin = true;
                            winnum++;
                            wintime = totaltime;
                            flags.clear();
                            for(int i = 0; i < boardobj.mine_loc.size(); i++){
                                vector<int> newvec;
                                newvec.push_back(boardobj.mine_loc[i][0]);
                                newvec.push_back(boardobj.mine_loc[i][1]);
                                flags.push_back(newvec);
                            }
                            if(flags.size() != 0){
                                for(int i = 0; i < flags.size(); i++){
                                        int x = flags[i][0];
                                        int y = flags[i][1];
                                        images["flag"].setPosition(y*32, x*32);
                                    game.draw(images["flag"]);
                                }}
                        }
                        
                    }}
                    auto coords = game.mapPixelToCoords(mousecoords);
                    if(images["debug"].getGlobalBounds().contains(mousex, mousey)){
                        debugcount++;
                        if(debugcount%2==0){
                            ifdebug = true;}
                        if(debugcount%2 != 0){
                            ifdebug = false;
                        }
                        
                    }
                    if(images["face_happy"].getGlobalBounds().contains(mousex, mousey)){
                        boardobj.board.clear();
                        boardobj.mine_loc.clear();
                        emptytiles.clear();
                        numtiles.clear();
                        flags.clear();
                        minecount = mines;
                        ifdebug = false;
                        totaltime = 0;
                        temptime = 0;
                        debugcount = 1;
                        leadercount = 1;
                    }
                    if(images["leaderboard"].getGlobalBounds().contains(mousex, mousey)){
                        //do some stuff
                        //ifpause = true;
                        ifleaderboard = true;
                        leadercount++;
                        temptime = totaltime;
                        iftemptime = true;
                        game.draw(images["play"]);
                        images["play"].setPosition(((cols) * 32) - 240, 32 * ((rows)+0.5f));
                        for(int i = 0; i < rows; i++){
                            for(int j = 0; j < cols; j++){
                                int x = j*32;
                                int y = i*32;
                                images["tile_revealed"].setPosition(x, y);
                                game.draw(images["tile_revealed"]);
                            }}
                    }
                    if(images["pause"].getGlobalBounds().contains(mousex, mousey)){
                        pausecount++;
                        if(pausecount%2 ==0){
                            ifpause = true;}
                        if(pausecount%2 != 0){
                            ifpause = false;
                        }
                        
                    }
                    
                    if(images["play"].getGlobalBounds().contains(mousex, mousey)){
                        if(pausecount%2 ==0){
                            ifpause = true;}
                        if(pausecount%2 != 0){
                            ifpause = false;
                        }
                    }

                }}
                    
                    }
            game.draw(screen);
            if(ifpause == false && iftemptime == false && ifwin == false && iflose == false){
                end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float, milli> runtime = end - start;
                totaltime += runtime.count();
            }
            if(ifpause && iftemptime == false){
                end = chrono::high_resolution_clock::now();
                std::chrono::duration<float, milli> runtime = end - start;
                totaltime -= runtime.count();
                totaltime += runtime.count();
            }
            if(iftemptime){
                totaltime = temptime;
            }
            //cout << totaltime << endl;
            int minutes = 0;
            int seconds = 0;
            if((totaltime/420)/60 >= 1){
                minutes = (totaltime/420)/60;
                seconds = abs(totaltime/420)-(minutes*60);
            }
            else{
                seconds = abs(totaltime/420);

            }
            //cout <<  seconds <<endl;
            string stringmins = to_string(minutes);
            string stringsecs = to_string(seconds);
            if(minutes == 0){
                images["digits"].setTextureRect(sf::IntRect(0, 0, 21, 32));
                game.draw(images["digits"]);
                images["digits"].setPosition(((cols)*32)-97, 32*((rows)+0.5f)+16);
                game.draw(images["digits"]);
                images["digits"].setPosition((((cols)*32)-97)+21, 32*((rows)+0.5f)+16);
            }
            if(10 > minutes > 0 ){
                images["digits"].setTextureRect(sf::IntRect(0, 0, 21, 32));
                game.draw(images["digits"]);
                images["digits"].setPosition(((cols)*32)-97, 32*((rows)+0.5f)+16);
                sf::Sprite tempsprite;
                tempsprite.setTexture(texture17);
                tempsprite.setTextureRect(sf::IntRect(21*(int(stringmins[0]-48)), 0, 21, 32));
                tempsprite.setPosition((((cols)*32)-97)+21, 32*((rows)+0.5f)+16);
                game.draw(tempsprite);
            }
            if(minutes >= 10){
                sf::Sprite tempsprite;
                tempsprite.setTexture(texture17);
                tempsprite.setTextureRect(sf::IntRect(21*(int(stringmins[0]-48)), 0, 21, 32));
                tempsprite.setPosition(((cols)*32)-97, 32*((rows)+0.5f)+16);
                game.draw(tempsprite);
                sf::Sprite tempsprite2;
                tempsprite2.setTexture(texture17);
                tempsprite2.setTextureRect(sf::IntRect(21*(int(stringmins[0]-48)), 0, 21, 32));
                tempsprite2.setPosition((((cols)*32)-97)+21, 32*((rows)+0.5f)+16);
                game.draw(tempsprite2);
            }
            
            //cout << stringsecs << endl;
            
            if(10 > seconds > 0){
                images["digits"].setTextureRect(sf::IntRect(0, 0, 21, 32));
                game.draw(images["digits"]);
                images["digits"].setPosition((((cols)*32)-54)+21, 32*((rows)+0.5f)+16);
                images["digits"].setTextureRect(sf::IntRect(21*(int(stringsecs[0]-48)), 0, 21, 32));
                game.draw(images["digits"]);
                images["digits"].setPosition(((cols)*32)-54, 32*((rows)+0.5f)+16);
            }
            else if(seconds == 0){
                images["digits"].setTextureRect(sf::IntRect(0, 0, 21, 32));
                game.draw(images["digits"]);
                images["digits"].setPosition(((cols)*32)-54, 32*((rows)+0.5f)+16);
                game.draw(images["digits"]);
                images["digits"].setPosition((((cols)*32)-54)+21, 32*((rows)+0.5f)+16);
                game.draw(images["digits"]);
            }
            else if(seconds >= 10){
                sf::Sprite tempsprite;
                tempsprite.setTexture(texture17);
                tempsprite.setTextureRect(sf::IntRect(21*(int(stringsecs[0]-48)), 0, 21, 32));
                tempsprite.setPosition((((cols)*32)-54), 32*((rows)+0.5f)+16);
                game.draw(tempsprite);
                sf::Sprite tempsprite2;
                tempsprite2.setTexture(texture17);
                tempsprite2.setTextureRect(sf::IntRect(21*(int(stringsecs[1]-48)), 0, 21, 32));
                tempsprite2.setPosition((((cols)*32)-54)+21, 32*((rows)+0.5f)+16);
                game.draw(tempsprite2);
            }
            
            
            game.draw(images["leaderboard"]);
            images["leaderboard"].setPosition(((cols) * 32) - 176, 32 * ((rows)+0.5f));
            if(ifpause == false){
                game.draw(images["pause"]);
                images["pause"].setPosition(((cols) * 32) - 240, 32 * ((rows)+0.5f));}
            if(ifpause || ifleaderboard){
                game.draw(images["play"]);
                images["play"].setPosition(((cols) * 32) - 240, 32 * ((rows)+0.5f));}
            if(iflose){
                images["lose_face"].setPosition(((cols / 2.0) * 32) - 32,32 *(rows+0.5f));
                game.draw(images["lose_face"]);
            }
            if(ifwin){
                images["win_face"].setPosition(((cols / 2.0) * 32) - 32,32 *(rows+0.5f));
                game.draw(images["win_face"]);
            }
            if(ifwin == false && iflose == false){
                game.draw(images["face_happy"]);
                images["face_happy"].setPosition(((cols / 2.0) * 32) - 32,32 *(rows+0.5f));}
            game.draw(images["debug"]);
            images["debug"].setPosition(((cols) * 32) - 304, 32 * ((rows)+0.5f));
            if(minecount < 0){
            sf::Sprite tempsprite;
                tempsprite.setTexture(texture17);
                tempsprite.setTextureRect(sf::IntRect(210, 0, 21, 32));
                tempsprite.setPosition(12, 32 * ((rows)+0.5f)+16);
                game.draw(tempsprite);}
            string stringcount = to_string(minecount);
            
            images["digits"].setTextureRect(sf::IntRect(0, 0, 21, 32));
            game.draw(images["digits"]);
            images["digits"].setPosition(33, 32 * ((rows)+0.5f)+16);
            game.draw(images["digits"]);
            images["digits"].setPosition(33+21, 32 * ((rows)+0.5f)+16);
            game.draw(images["digits"]);
            images["digits"].setPosition(33+(21*2), 32 * ((rows)+0.5f)+16);
            if(minecount < 0){
                stringcount.erase(stringcount.begin());
            }
            for(int i = (stringcount.size()-1); i >= 0; i--){
                images["digits"].setTextureRect(sf::IntRect(21*(int(stringcount[i]-48)), 0, 21, 32));
                game.draw(images["digits"]);
                images["digits"].setPosition(54 + 21*((stringcount.size()-1)-i), 32 * ((rows)+0.5f)+16);
                }
            
            game.display();
            
        }}
};


class Welcome{
    string playername;
    GameBoard gameboard;
public:
    bool ifclosed = false;
    string displayname;
    void OpenWindow(int cols, int rows){
        sf::RenderWindow window(sf::VideoMode(cols, rows),"Minesweeper", sf::Style::Close);
        float fcols = cols;
        float frows = rows;
        sf::RectangleShape screen(sf::Vector2f(fcols, frows));
        screen.setFillColor(sf::Color::Blue);
        sf::Font font;
        font.loadFromFile("files/font.ttf");
        sf::Text text;
        sf::Text text2;
        text2.setFont(font);
        text.setFont(font);
        text.setString("WELCOME TO MINESWEEPER!");
        text2.setString("Enter your name:");
        text2.setCharacterSize(20);
        text.setCharacterSize(24);
        text2.setStyle(sf::Text::Bold);
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
        sf::FloatRect textrect = text.getLocalBounds();
        sf::FloatRect textrect2 = text2.getLocalBounds();
        text.setOrigin(textrect.left + textrect.width/2.0f, textrect.top + textrect.height/2.0f);
        text2.setOrigin(textrect2.left + textrect2.width/2.0f, textrect2.top + textrect2.height/2.0f);
        text.setPosition(sf::Vector2f(fcols/2.0f ,(frows/2.0f)-150));
        text2.setPosition(sf::Vector2f(fcols/2.0f ,(frows/2.0f)-75));
        sf::Text name;
        sf::String playerinp;
        name.setFont(font);
        name.setString("|");
        name.setColor(sf::Color::Yellow);
        name.setCharacterSize(18);
        name.setStyle(sf::Text::Bold);
        sf::FloatRect namerect = text.getLocalBounds();
        name.setOrigin(namerect.left + namerect.width/2.0f, namerect.top + namerect.height/2.0f);
        name.setPosition(sf::Vector2f(fcols/2.0f ,(frows/2.0f)-45));
        while(window.isOpen()){
            sf::Event playere;
            while(window.pollEvent(playere)){
                if(playere.type == playere.Closed){
                    ifclosed = true;
                    window.close();}
                if(playere.type == sf::Event::TextEntered){
                    if(isalpha(playere.text.unicode)){
                        if(playername.size() < 10){
                        playerinp += playere.text.unicode;
                        name.setString(playerinp + "|");
                        playername += playere.text.unicode;
                        }}
                    if(playere.text.unicode == '\b'){
                        if(playername.size() != 0){
                        playername.erase(playername.end()-1);
                        playerinp.erase(playerinp.getSize()-1, 1);
                        name.setString(playerinp + "|");
                        }}
                    if(playere.text.unicode == '\n' && playername.size() != 0){
                        char playerchar;
                        playerchar = (char)toupper(playername[0]);
                        displayname += playerchar;
                        for(int i = 1; i < playername.length(); i++){
                             playerchar = (char)tolower(playername[i]);
                            displayname += playerchar;}
                        window.close();
                    }
                }
                }
            window.draw(screen);
            window.draw(text);
            window.draw(text2);
            window.draw(name);
            window.display();
        }  // end of first while loop
    } // end of func def
    
    
};
