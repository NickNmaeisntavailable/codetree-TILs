#include <iostream>

using namespace std;



int N, M, K;

int map[11][11];

int Rot_map[11][11];

pair<int, int> player[11];

pair<int, int> exits;

int ans;

int sx, sy, edge_length;

void MovePlayer() {
    for (int i = 1; i <= M; i++) {
        if (player[i].first == exits.first && player[i].second == exits.second)
            continue;

        if (player[i].first != exits.first) {
            int nx = player[i].first;
            int ny = player[i].second;

            if (exits.first > nx) nx++;
            else nx--;

            if (!map[nx][ny]) {
                player[i].first = nx;
                player[i].second = ny;
                ans++;
                continue;
            }
        }

        if (player[i].second != exits.second) {
            int nx = player[i].first;
            int ny = player[i].second;

            if (exits.second > ny) ny++;
            else ny--;

            if (!map[nx][ny]) {
                player[i].first = nx;
                player[i].second = ny;
                ans++;
                continue;
            }
        }
    }
}

void FindSquare() {
    for (int sz = 2; sz <= N; sz++) {
        for (int x1 = 1; x1 <= N - sz + 1; x1++) {
            for (int y1 = 1; y1 <= N - sz + 1; y1++) {
                int x2 = x1 + sz - 1;
                int y2 = y1 + sz - 1;

                if (!(x1 <= exits.first && exits.first <= x2 && y1 <= exits.second && exits.second <= y2)) continue;
         

                bool is_player_in = false;
                for (int l = 1; l <= M; l++) {
                    if (x1 <= player[l].first && player[l].first <= x2 && y1 <= player[l].second && player[l].second <= y2) {
                        if (!(player[l].first == exits.first && player[l].second == exits.second)) {
                            is_player_in = true;
                        }
                    }
                }

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

void RotateSquare() {
    for (int x = sx; x < sx + edge_length; x++)
        for (int y = sy; y < sy + edge_length; y++) {
            if (map[x][y]) map[x][y]--;
        }

    for (int x = sx; x < sx + edge_length; x++)
        for (int y = sy; y < sy + edge_length; y++) {
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = edge_length - ox - 1;
            Rot_map[rx + sx][ry + sy] = map[x][y];
        }

    for (int x = sx; x < sx + edge_length; x++)
        for (int y = sy; y < sy + edge_length; y++) {
            map[x][y] = Rot_map[x][y];
        }
}

void RotateTravelerAndExit() {
    for (int i = 1; i <= M; i++) {
        int x = player[i].first;
        int y = player[i].second;
        if (sx <= x && x < sx + edge_length && sy <= y && y < sy + edge_length) {
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = edge_length - ox - 1;
            player[i] = make_pair(rx + sx, ry + sy);
        }
    }

    int x = exits.first;
    int y = exits.second;
    if (sx <= x && x < sx + edge_length && sy <= y && y < sy + edge_length) {
        int ox = x - sx, oy = y - sy;
        int rx = oy, ry = edge_length - ox - 1;
        exits = make_pair(rx + sx, ry + sy);
    }
}

int main() {
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> map[i+1][j+1];

    for (int i = 0; i < M; i++) {
        cin >> player[i+1].first;
        cin >> player[i+1].second;
    }

    cin >> exits.first;
    cin >> exits.second;

    while (K--) {
        MovePlayer();

        bool is_all_escaped = true;
        for (int i = 1; i <= M; i++) {
            if (!(player[i].first == exits.first && player[i].second == exits.second)) {
                is_all_escaped = false;
            }
        }

        if (is_all_escaped) break;

        FindSquare();

        RotateSquare();
        RotateTravelerAndExit();
    }

    cout << ans << "\n";
    cout << exits.first << " " << exits.second;
}