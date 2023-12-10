//BUG:移動邏輯，未考慮邊界和障礙物
#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<vector>
using namespace std;

class Weapon{
protected:
  string name;//武器名
  int attack;//攻擊力
  int level;//武器等級
public:
  Weapon(string name,int attack,int level)
    : name(name),attack(attack),level(level){}
};


class Character{
protected:
  string name;//角色名
  int health;//生命值
  int positionX;//角色位置x
  int positionY;//角色位置y
  Weapon *weapon;//武器
public:
  //設定角色名字 和 位置 其他都有預設值
  Character(string n,int x,int y) : name(n),health(100),positionX(x),positionY(y){};

  void move(int step){//實現移動邏輯 要考慮障礙物和地圖邊界
    // 簡單的移動邏輯，不考慮邊界和障礙物
    positionX += step;
    positionY += step;
    cout << name << " moved " << step << " steps.\n";
  }
  virtual void attack(Character& enemy){//實現攻擊邏輯 
    // 簡單的攻擊邏輯
    enemy.health -= 10;
    cout << name << " attacked " << enemy.name << ". "
      << enemy.name << "'s health: " << enemy.health << "\n";
  }
  void displayInfo() {//顯示角色資料
    // 顯示角色信息
    cout << "Name: " << name << ", Position: (" << positionX 
      << ", " << positionY << "), Health: " << health << "\n";
  }
  string getName () const {
    return name ;
  }
  int getHealth () const {
    return health ;
  }
  int getX() const{
    return positionX ;
  }
  int getY() const{
    return positionY ;
  }

};

class GameMap{
protected:
  int size;
  vector<vector<int>> obstacles; // 1表示障礙物
public:
  void generateObstacles(){// 要實現隨機生成障礙物的邏輯
    // 簡單的隨機生成障礙物的邏輯
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            obstacles[i][j] = rand() % 2; 
          // 隨機生成0或1，表示有無障礙物
        }
    }
  }

  GameMap(int size) : size(size){//初始化map大小 要實現障礙物
    obstacles.resize(size, vector<int>(size, 0));
    generateObstacles();
  }
  void displayMap(const vector<Character>& characters) {// 顯示地圖，包括角色位置和障礙物
    // 顯示地圖，包括角色位置和障礙物
    cout << "Game Map:\n";
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            // 簡單表示角色和障礙物
            if(i == characters[0].getX() && j == characters[0].getY()){
              cout << "A ";
            } else if(i == characters[1].getX() && j == characters[1].getY()){
              cout << "B ";
            } else if (obstacles[i][j] == 1) {
                cout << "X ";
            }else {
                cout << ". ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
  }

  
  int getSize() const {
    return size;
  }
};

class Game{
private:
  GameMap map;
  vector<Character> players;
public:
  void initializePlayers(){//雙方選擇角色
    for (int i = 1; i <= 2; ++i) {
        string name = "Player" + to_string(i);
        int startX = rand() % map.getSize();
        int startY = rand() % map.getSize();
        Character player(name, startX, startY);
        players.push_back(player);
    }
  }
  Game(int mapSize) : map(mapSize){//設定地圖
     initializePlayers();
  }
  void startGame() {//開始遊戲
    while (true) {
      // 顯示地圖和玩家信息
      map.displayMap(players);
      for (auto &player : players) {
        player.displayInfo();
      }

      // 玩家操作
      for (auto &player : players) {
        int steps = rand() % 3 + 1; // 簡單的步數生成邏輯
        player.move(steps);

        // 簡單的攻擊邏輯，隨機選擇攻擊對手
        int targetIndex = rand() % players.size();
        if (targetIndex != &player - &players[0]) {
          player.attack(players[targetIndex]);
        }

        // 簡單的勝負判斷，假設血量為0時為失敗
        if (players[targetIndex].getHealth() <= 0) {
          cout << players[targetIndex].getName() << " has been defeated! " 
            << player.getName() << " wins!\n";
          return;
        }
      }
    }
  }
};

int main(){
    srand(time(0));
    Game game(10);
    game.startGame();
    return 0;
}

