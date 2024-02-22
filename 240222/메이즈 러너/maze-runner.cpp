#include <iostream>

using namespace std;



int N, M, K;//N은 가로 세로, M명의 플레이어,K초 만큼

int map[11][11];//배열 10X10이지만 1,1부터 시작이므로 

int Rot_map[11][11];// 돌렸을 때 임시 맵

pair<int, int> player[11];//플레이어의 좌표

pair<int, int> exits;//출구의 좌표

int ans;// 답

int sx, sy, edge_length;// 사각형 첫 x좌표 y 좌표 사각형 길이

void MovePlayer() {//플레이어 움직이는 함수
    for (int i = 1; i <= M; i++) {//1부터 M까지의 플레이어
        //만약 플레이어가 출구라면 continue
        if (player[i].first == exits.first && player[i].second == exits.second) continue;
        //만약 플레이어의 X좌표와 출구의 X좌표가 다르면
        if (player[i].first != exits.first) {
            //플레이어를 이동
            int nx = player[i].first;
            int ny = player[i].second;
            //만약 출구 X좌표가 다음 X좌표보다 크면 하나 증가
            if (exits.first > nx) nx++;
            //아니면 감소
            else nx--;
            //벽이 없다면 이동
            //가능하다면 행 이동, 다음 참가자로 넘어감
            if (!map[nx][ny]) {
                player[i].first = nx;
                player[i].second = ny;
                ans++;
                continue;
            }
        }
        //열 다르면 열 이동
        if (player[i].second != exits.second) {
            int nx = player[i].first;
            int ny = player[i].second;

            if (exits.second > ny) ny++;
            else ny--;
            //벽이 없다면 이동 >> 열 이동
            if (!map[nx][ny]) {
                player[i].first = nx;
                player[i].second = ny;
                ans++;
                continue;
            }
        }
    }
}
//가장 작은 정사각형 찾기
void FindSquare() {
    //가장 작은 정사각형 만들어보기
    for (int sz = 2; sz <= N; sz++) {
        //가장 윗 상단부터 행으로 검사
        for (int x1 = 1; x1 <= N - sz + 1; x1++) {
            //가장 윗 상단부터 열 검사
            for (int y1 = 1; y1 <= N - sz + 1; y1++) {
                int x2 = x1 + sz - 1;
                int y2 = y1 + sz - 1;
                //만약 출구가 정사각형 바깥이라면 continue
                if (!(x1 <= exits.first && exits.first <= x2 && y1 <= exits.second && exits.second <= y2)) continue;

                //정사각형 안에 플레이어가 한명 이상 있는지 확인
                bool is_player_in = false;
                for (int i = 1; i <= M; i++) {
                    if (x1 <= player[i].first && player[i].first <= x2 && y1 <= player[i].second && player[i].second <= y2) {
                        //출구에 있는 플레이어는 제외
                        if (!(player[i].first == exits.first && player[i].second == exits.second)) {
                            is_player_in = true;
                        }
                    }
                }
                //만약 한명 이상의 참가자라면 다시 검사 받기
                if (is_player_in) {
                    sx = x1;
                    sy = y1;
                    edge_length = sz;

                    return;
                }
            }
        }
    }
}
//정사각형 내부의 벽 회전
void RotateSquare() {
    //정사각형 내구도 1씩 감소
    for (int x = sx; x < sx + edge_length; x++)
        for (int y = sy; y < sy + edge_length; y++) {
            if (map[x][y]) map[x][y]--;
        }
    //정사각형 회전시키기
    for (int x = sx; x < sx + edge_length; x++)
        for (int y = sy; y < sy + edge_length; y++) {
            int ox = x - sx, oy = y - sy;//정사각형 첫 좌표를 (0,0)으로 변환
            int rx = oy, ry = edge_length - ox - 1;
            // 변환된 상태에서는 회전 이후의 좌표가 (x, y) -> (y, length - x - 1)
            Rot_map[rx + sx][ry + sy] = map[x][y];
            // 다시 (sx, sy)를 더해줍니다.
        }
    //Rot_map에 현재 map 정보 옮겨주기
    for (int x = sx; x < sx + edge_length; x++)
        for (int y = sy; y < sy + edge_length; y++) {
            map[x][y] = Rot_map[x][y];
        }
}

void RotatePlayerAndExit() {//모든 참가자와 출구 정보 회전
    for (int i = 1; i <= M; i++) {//M명의 참가자들 모두 확인
        int x = player[i].first;
        int y = player[i].second;
        //해당 참가자가 정사각형안에 있다면 회전
        if (sx <= x && x < sx + edge_length && sy <= y && y < sy + edge_length) {
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = edge_length - ox - 1;
            player[i] = make_pair(rx + sx, ry + sy);
            //회전 후 다시 첫 좌표 더해주기
        }
    }
    //출구도 회전
    int x = exits.first;
    int y = exits.second;
    if (sx <= x && x < sx + edge_length && sy <= y && y < sy + edge_length) {
        //(0.0)으로 변환
        int ox = x - sx, oy = y - sy;
        int rx = oy, ry = edge_length - ox - 1;
        exits = make_pair(rx + sx, ry + sy);
    }
}

int main() {
    cin >> N >> M >> K;//입력
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> map[i + 1][j + 1];

    for (int i = 0; i < M; i++) {
        cin >> player[i + 1].first;
        cin >> player[i + 1].second;
    }

    cin >> exits.first;//출구 정보
    cin >> exits.second;

    while (K--) {//K초만큼 반복
        MovePlayer();

        bool is_in = true;
        for (int i = 1; i <= M; i++) {
            if (!(player[i].first == exits.first && player[i].second == exits.second)) {
                is_in = false;
            }
        }

        if (is_in) break;

        FindSquare();

        RotateSquare();
        RotatePlayerAndExit();
    }

    cout << ans << endl;
    cout << exits.first << " " << exits.second;
}