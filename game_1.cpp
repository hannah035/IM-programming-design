#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;



///---------------------------

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
                        cout << " 1 "; break;
                    case WATER:
                        cout << " 2 "; break;
                    case ROCK:
                        cout << " 3 "; break;
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

int mapArray[16][16] = {0};

///---------------------------

int cnt_UpDown = 0;
int cnt_LeftRight = 0;

int cnt_UpDown_player2 = 100;
int cnt_LeftRight_player2 = 100;

int p1_x = 1, p1_y = 1, p1_moves = 3;
int p2_x = 15, p2_y = 15, p2_moves = 3;

bool isInitialDrawDone = false;

int currentPlayer = 1;

int arr[15][15] = {0};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    const wchar_t* text;
    //HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    //SelectObject(hdc, hFont);
    text = L"Please choose small (press 1), medium (press 2), or large (press 3).";
    TextOutW(hdc, 10, 10, text, wcslen(text));
    ReleaseDC(hwnd, hdc);  // 釋放裝置內文

    /// 方框
    hdc = GetDC(hwnd);

    switch (uMsg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
            HBRUSH grassBrush = CreateSolidBrush(RGB(0, 201, 87));
            HBRUSH waterBrush = CreateSolidBrush(RGB(0, 255, 255));
            HBRUSH rockBrush = CreateSolidBrush(RGB(192, 192, 192));

            // Check if initial drawing is done
            if (!isInitialDrawDone) {
                // Draw the black rectangles here
                for (int i = 1; i <= 15; i++) {
                    for (int j = 1; j <= 15; j++) {
                        RECT rect = {i * 50, j * 50, (i + 1) * 50, (j + 1) * 50};
                        if(mapArray[i][j] == 1) FillRect(hdc, &rect, grassBrush);
                        else if(mapArray[i][j] == 2) FillRect(hdc, &rect, waterBrush);
                        else if(mapArray[i][j] == 3) FillRect(hdc, &rect, rockBrush);
                    }
                }
                RECT rect;
                rect = {1 * 50, 1 * 50, (1 + 1) * 50, (1 + 1) * 50};
                FillRect(hdc, &rect, redBrush);

                rect = {15 * 50, 15 * 50, (15 + 1) * 50, (15 + 1) * 50};
                FillRect(hdc, &rect, blueBrush);

                isInitialDrawDone = true;
            }

            EndPaint(hwnd, &ps);
            break;
        }
        default:
            break;
    }


    wchar_t msg[32];
    switch (uMsg)
    {
        case WM_KEYDOWN:
        {
            swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
            OutputDebugStringW(msg);
            cout << "WM_KEYDOWN" << endl;

            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

            RECT rect;

            rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
            FillRect(hdc, &rect, redBrush);

            rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
            FillRect(hdc, &rect, blueBrush);


            if (currentPlayer == 1)
            {
                switch (wParam)
                {
                    case VK_UP:
                        if(p1_y > 1 && arr[p1_x][p1_y - 1] == 0)
                        {
                            cout << "up" << endl;
                            p1_y -= 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            arr[p1_x][p1_y] = 1;
                            break;
                        }
                        else break;


                    case VK_DOWN:
                        if(p1_y < 15 && arr[p1_x][p1_y + 1] == 0)
                        {
                            cout << "down" << endl;
                            p1_y += 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            arr[p1_x][p1_y] = 1;
                            break;
                        }
                        else break;


                    case VK_LEFT:
                        if(p1_x > 1 && arr[p1_x - 1][p1_y] == 0)
                        {
                            cout << "left" << endl;
                            p1_x -= 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            arr[p1_x][p1_y] = 1;
                            break;
                        }
                        else break;


                    case VK_RIGHT:
                        if(p1_x < 15 && arr[p1_x + 1][p1_y] == 0)
                        {
                            cout << "right" << endl;
                            p1_x += 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            arr[p1_x][p1_y] = 1;
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                if(p1_moves != 3)
                {
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, whiteBrush);
                }
                if(p1_moves == 0)
                {
                    currentPlayer = 2;
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, redBrush);
                    rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, whiteBrush);
                }
            }
            else if (currentPlayer == 2)
            {
                switch (wParam)
                {
                    case 'W':
                        if(p2_y > 1)
                        {
                            cout << "W pressed" << endl;
                            p2_y -= 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            break;
                        }
                        else break;

                    case 'S':
                        if(p2_y < 15)
                        {
                            cout << "S pressed" << endl;
                            p2_y += 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            break;
                        }
                        else break;


                    case 'A':
                        if(p2_x > 1)
                        {
                            cout << "A pressed" << endl;
                            p2_x -= 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            break;
                        }
                        else break;


                    case 'D':
                        if(p2_x < 15)
                        {
                            cout << "D pressed" << endl;
                            p2_x += 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                FillRect(hdc, &rect, whiteBrush);
                if(p2_moves == 0)
                {
                    // Switch turns after a move
                    isInitialDrawDone = 0;
                    // Reset moves for the next player
                    p1_moves = 3;
                    p2_moves = 3;
                    // Draw the black rectangles here
                    for (int i = 1; i <= 15; i++) {
                        for (int j = 1; j <= 15; j++) {
                            RECT rect = {i * 50, j * 50, (i + 1) * 50, (j + 1) * 50};
                            FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
                        }
                    }
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, redBrush);

                    rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, blueBrush);
                    currentPlayer = 1;
                }
            }



            if (p1_x == p2_x && p1_y == p2_y)
            {
                PostQuitMessage(0); // 結束程序
            }
            ReleaseDC(hwnd, hdc);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main()
{
    ///--------------------------------
    //建立地圖
    TileMap myTileMap(16, 16);

    for (int i = 1; i <= 15; i++) {
        for (int j = 1; j <= 15; j++) {
            myTileMap.setTileType(j, i, GRASS);
        }
    }

    //加入湖水
    int lakeCnt = generateRandomNumber(static_cast<int>(225 * 0.02),
                                       static_cast<int>(225 * 0.04));
    //cout << lakeCnt << endl;
    for (int i = 1; i <= lakeCnt; i++)
        myTileMap.generateWaterMass();

    //加入岩石
    int rockCnt = generateRandomNumber(static_cast<int>(225 * 0.05),
                                       static_cast<int>(225 * 0.1));
    //cout << rockCnt << endl;
    myTileMap.randomRocks(rockCnt);

    for (int i = 1; i <= 15; i++){
        for (int j = 1; j <= 15; j++){
            if (i == 1 || i == 15)
                myTileMap.setTileType(j, i, GRASS);
            if (j == 1 || j == 15)
                myTileMap.setTileType(j, i, GRASS);
        }
    }

    myTileMap.printTileMap();

    for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                switch (myTileMap.getTileType(j, i)) {
                    case GRASS:
                        mapArray[i][j] = 1;
                        break;
                    case WATER:
                        mapArray[i][j] = 2;
                        break;
                    case ROCK:
                        mapArray[i][j] = 3;
                        break;
                }
            }
        }
    for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                cout << mapArray[i][j] << " ";
            }
            cout << endl;
        }
    ///----------------------------------

    // 註冊視窗
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = L"SampleWindowClass";
    RegisterClassW(&wc);

    // 創建視窗
    HWND hwnd = CreateWindowExW(0, L"SampleWindowClass", L"Sample Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, NULL, GetModuleHandleW(NULL), NULL);

    if (hwnd == NULL)
    {
        cerr << "視窗創建失敗！" << endl;
        return 1;
    }

    // 顯示視窗
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    // 進入消息循環
    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 恢復編譯器警告設定
    #pragma GCC diagnostic pop

    return 0;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;



///---------------------------

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
                        cout << " 1 "; break;
                    case WATER:
                        cout << " 2 "; break;
                    case ROCK:
                        cout << " 3 "; break;
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

int mapArray[16][16] = {0};

///---------------------------

int cnt_UpDown = 0;
int cnt_LeftRight = 0;

int cnt_UpDown_player2 = 100;
int cnt_LeftRight_player2 = 100;

int p1_x = 1, p1_y = 1, p1_moves = 3;
int p2_x = 15, p2_y = 15, p2_moves = 3;

bool isInitialDrawDone = false;

int currentPlayer = 1;

int arr[15][15] = {0};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    const wchar_t* text;
    //HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    //SelectObject(hdc, hFont);
    text = L"Please choose small (press 1), medium (press 2), or large (press 3).";
    TextOutW(hdc, 10, 10, text, wcslen(text));
    ReleaseDC(hwnd, hdc);  // 釋放裝置內文

    /// 方框
    hdc = GetDC(hwnd);

    switch (uMsg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
            HBRUSH grassBrush = CreateSolidBrush(RGB(0, 201, 87));
            HBRUSH waterBrush = CreateSolidBrush(RGB(0, 255, 255));
            HBRUSH rockBrush = CreateSolidBrush(RGB(192, 192, 192));

            // Check if initial drawing is done
            if (!isInitialDrawDone) {
                // Draw the black rectangles here
                for (int i = 1; i <= 15; i++) {
                    for (int j = 1; j <= 15; j++) {
                        RECT rect = {i * 50, j * 50, (i + 1) * 50, (j + 1) * 50};
                        if(mapArray[i][j] == 1) FillRect(hdc, &rect, grassBrush);
                        else if(mapArray[i][j] == 2) FillRect(hdc, &rect, waterBrush);
                        else if(mapArray[i][j] == 3) FillRect(hdc, &rect, rockBrush);
                    }
                }
                RECT rect;
                rect = {1 * 50, 1 * 50, (1 + 1) * 50, (1 + 1) * 50};
                FillRect(hdc, &rect, redBrush);

                rect = {15 * 50, 15 * 50, (15 + 1) * 50, (15 + 1) * 50};
                FillRect(hdc, &rect, blueBrush);

                isInitialDrawDone = true;
            }

            EndPaint(hwnd, &ps);
            break;
        }
        default:
            break;
    }


    wchar_t msg[32];
    switch (uMsg)
    {
        case WM_KEYDOWN:
        {
            swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
            OutputDebugStringW(msg);
            cout << "WM_KEYDOWN" << endl;

            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

            RECT rect;

            rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
            FillRect(hdc, &rect, redBrush);

            rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
            FillRect(hdc, &rect, blueBrush);


            if (currentPlayer == 1)
            {
                switch (wParam)
                {
                    case VK_UP:
                        if(p1_y > 1 && arr[p1_x][p1_y - 1] == 0)
                        {
                            cout << "up" << endl;
                            p1_y -= 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            arr[p1_x][p1_y] = 1;
                            break;
                        }
                        else break;


                    case VK_DOWN:
                        if(p1_y < 15 && arr[p1_x][p1_y + 1] == 0)
                        {
                            cout << "down" << endl;
                            p1_y += 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            arr[p1_x][p1_y] = 1;
                            break;
                        }
                        else break;


                    case VK_LEFT:
                        if(p1_x > 1 && arr[p1_x - 1][p1_y] == 0)
                        {
                            cout << "left" << endl;
                            p1_x -= 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            arr[p1_x][p1_y] = 1;
                            break;
                        }
                        else break;


                    case VK_RIGHT:
                        if(p1_x < 15 && arr[p1_x + 1][p1_y] == 0)
                        {
                            cout << "right" << endl;
                            p1_x += 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            arr[p1_x][p1_y] = 1;
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                if(p1_moves != 3)
                {
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, whiteBrush);
                }
                if(p1_moves == 0)
                {
                    currentPlayer = 2;
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, redBrush);
                    rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, whiteBrush);
                }
            }
            else if (currentPlayer == 2)
            {
                switch (wParam)
                {
                    case 'W':
                        if(p2_y > 1)
                        {
                            cout << "W pressed" << endl;
                            p2_y -= 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            break;
                        }
                        else break;

                    case 'S':
                        if(p2_y < 15)
                        {
                            cout << "S pressed" << endl;
                            p2_y += 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            break;
                        }
                        else break;


                    case 'A':
                        if(p2_x > 1)
                        {
                            cout << "A pressed" << endl;
                            p2_x -= 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            break;
                        }
                        else break;


                    case 'D':
                        if(p2_x < 15)
                        {
                            cout << "D pressed" << endl;
                            p2_x += 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                FillRect(hdc, &rect, whiteBrush);
                if(p2_moves == 0)
                {
                    // Switch turns after a move
                    isInitialDrawDone = 0;
                    // Reset moves for the next player
                    p1_moves = 3;
                    p2_moves = 3;
                    // Draw the black rectangles here
                    for (int i = 1; i <= 15; i++) {
                        for (int j = 1; j <= 15; j++) {
                            RECT rect = {i * 50, j * 50, (i + 1) * 50, (j + 1) * 50};
                            FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
                        }
                    }
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, redBrush);

                    rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, blueBrush);
                    currentPlayer = 1;
                }
            }



            if (p1_x == p2_x && p1_y == p2_y)
            {
                PostQuitMessage(0); // 結束程序
            }
            ReleaseDC(hwnd, hdc);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main()
{
    ///--------------------------------
    //建立地圖
    TileMap myTileMap(16, 16);

    for (int i = 1; i <= 15; i++) {
        for (int j = 1; j <= 15; j++) {
            myTileMap.setTileType(j, i, GRASS);
        }
    }

    //加入湖水
    int lakeCnt = generateRandomNumber(static_cast<int>(225 * 0.02),
                                       static_cast<int>(225 * 0.04));
    //cout << lakeCnt << endl;
    for (int i = 1; i <= lakeCnt; i++)
        myTileMap.generateWaterMass();

    //加入岩石
    int rockCnt = generateRandomNumber(static_cast<int>(225 * 0.05),
                                       static_cast<int>(225 * 0.1));
    //cout << rockCnt << endl;
    myTileMap.randomRocks(rockCnt);

    for (int i = 1; i <= 15; i++){
        for (int j = 1; j <= 15; j++){
            if (i == 1 || i == 15)
                myTileMap.setTileType(j, i, GRASS);
            if (j == 1 || j == 15)
                myTileMap.setTileType(j, i, GRASS);
        }
    }

    myTileMap.printTileMap();

    for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                switch (myTileMap.getTileType(j, i)) {
                    case GRASS:
                        mapArray[i][j] = 1;
                        break;
                    case WATER:
                        mapArray[i][j] = 2;
                        break;
                    case ROCK:
                        mapArray[i][j] = 3;
                        break;
                }
            }
        }
    for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                cout << mapArray[i][j] << " ";
            }
            cout << endl;
        }
    ///----------------------------------

    // 註冊視窗
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = L"SampleWindowClass";
    RegisterClassW(&wc);

    // 創建視窗
    HWND hwnd = CreateWindowExW(0, L"SampleWindowClass", L"Sample Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, NULL, GetModuleHandleW(NULL), NULL);

    if (hwnd == NULL)
    {
        cerr << "視窗創建失敗！" << endl;
        return 1;
    }

    // 顯示視窗
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    // 進入消息循環
    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 恢復編譯器警告設定
    #pragma GCC diagnostic pop

    return 0;
}
