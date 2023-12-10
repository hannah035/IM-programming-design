#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//方格的種類
enum TileType {
    GRASS,
    WATER,
    ROCK
};
struct Vector{
    int x;
    int y;
};

int generateRandomNumber(int minValue, int maxValue) {
    return rand() % (maxValue - minValue + 1) + minValue;
}

//這是個別的方格
class Tile {
private:
    TileType type;
public:
    Tile(TileType type){
        this->type = type;
    }
    Tile(){}
    TileType getTile(){
        return type;
    }
    void setTile(TileType type){
        this->type = type;
        return;
    }
};

//生成地圖
class TileMap {
private:
    int width;
    int length;
    int obstacleCnt;
    Tile*** TileMapObject;
public:
    TileMap(int width, int length){
        this->width = width;
        this->length = length;
        this->obstacleCnt = 0.1*(length*width);
        
        TileMapObject = new Tile** [length];
        for (int i = 0; i < length; i++){
            TileMapObject[i] = new Tile* [width];
            for (int j = 0; j < width; j++){
                TileMapObject[i][j] = new Tile();
            }
        }
        srand(static_cast<unsigned>(time(0)));
    }
    ~TileMap() {
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                delete TileMapObject[i][j];
            }
            delete[] TileMapObject[i];
        }
        delete[] TileMapObject;
    }
    
    TileType getTileType(int x, int y) const {
            return TileMapObject[y][x]->getTile();
        }
    
    void setTileType(int x, int y, TileType type) {
            TileMapObject[y][x]->setTile(type);
        }
    
    void printTileMap() const {
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                switch (getTileType(j, i)) {
                    case GRASS:
                        cout << " 🌳 "; break;
                    case WATER:
                        cout << " 💧 "; break;
                    case ROCK:
                        cout << " 🪨 "; break;
                    }
                }
                cout << endl;
            }
        }
    void generateWaterMass() {
        int waterMassWidth = rand() % 3 + 2; // 在[2,4]之間取一個隨機數字當水池寬度
        int waterMassLength = rand() % 3 + 2;

        int startX = rand() % (width - waterMassWidth + 1);
        int startY = rand() % (length - waterMassLength + 1);

        int endX = startX + waterMassWidth - 1;
        int endY = startY + waterMassLength - 1;

        for (int i = startY; i <= endY; i++) {
            for (int j = startX; j <= endX; j++) {
                setTileType(j, i, WATER);
            }
        }
    }
    void randomRocks(int rockCnt) {
        for (int i = 0; i < rockCnt; i++) {
            int x = rand() % width;
            int y = rand() % length;

            //檢查這個方格是不是GRASS 是的話才設定為ROCK
            if (getTileType(x, y) == GRASS) {
                setTileType(x, y, ROCK);
            } else {
                i--;
            }
        }
    }
};


int main(){
    //請使用者輸入地圖的長與寬（最小地圖長寬為5x5）
    int width, length;
    cout << "The smallest generatable map is 5x5, please enter valid parameters!" << endl;
    cout << "Enter the width of your map:" << endl;
    cin >> width;
    cout << "Enter the length of your map:" << endl;
    cin >> length;
    
    if (length < 5 || width < 5){
        cout << "Invalid map parameters, please restart the game and try again!" << endl;
        return 0;
    }
        
    
    //建立地圖
    TileMap myTileMap(width, length);
    
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            myTileMap.setTileType(j, i, GRASS);
        }
    }
    
    //加入湖水
    int lakeCnt = generateRandomNumber(static_cast<int>((length * width) * 0.02),
                                       static_cast<int>((length * width) * 0.04));
    //cout << lakeCnt << endl;
    for (int i = 0; i < lakeCnt; i++)
        myTileMap.generateWaterMass();
    
    //加入岩石
    int rockCnt = generateRandomNumber(static_cast<int>((length * width) * 0.05),
                                       static_cast<int>((length * width) * 0.1));
    //cout << rockCnt << endl;
    myTileMap.randomRocks(rockCnt);
    
    myTileMap.printTileMap();
    
    return 0;
}
