//努力生出交代給隊友們
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
  Weapon(string name,int attack,int level) : name(name),attack(attack),level(level){}
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
  
  void move(int step);//實現移動邏輯 要考慮障礙物和地圖邊界
  void attack(Character& enemy);//實現攻擊邏輯 
  void displayInfo() ;//顯示角色資料
};

class GameMap{
protected:
  int size;
  vector<vector<int>> obstacles; // 1表示障礙物
public:
  void generateObstacles();// 要實現隨機生成障礙物的邏輯
  GameMap(int size) : size(size){//初始化map大小 要實現障礙物
     generateObstacles();
  }
  void displayMap() ;// 顯示地圖，包括角色位置和障礙物
};

class Game{
private:
  GameMap map;
  vector<Character> players;
public:
  void initializePlayers();//雙方選擇角色
  Game(int mapSize);//設定地圖
  void startGame() ;//開始遊戲
};

int main(){
    srand(time(0));
    Game game(10);
    game.startGame();

    return 0;
}
