#include <bits/stdc++.h>
using namespace std;

int M, N;
vector<string> grid;
vector<vector<int>> vis;
vector<pair<int,int>> path;

bool dfs(int r, int c){
    // base case: if out of bounds or cell is blocked or already visited
    if(r<0||r>=M||c<0||c>=N) return false;
    // if cell is blocked or already visited
    if(grid[r][c]=='X' || vis[r][c]) return false;
    vis[r][c]=1;
    // add current cell to path
    path.emplace_back(r,c);
    // if we reach the destination, return true
    if(r==M-1 && c==N-1) return true;
    // explore all 4 directions
    const int dr[4]={-1,1,0,0};
    const int dc[4]={0,0,-1,1};
    for(int k=0;k<4;k++){
        if(dfs(r+dr[k], c+dc[k])) return true;
    }
    // backtrack: remove current cell from path and mark as unvisited
    vis[r][c]=0;
    path.pop_back();
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if(!(cin>>M>>N)) return 0;
    grid.assign(M, string());
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            string s; if(!(cin>>s)) s="0";
            // accept X or 0/1
            if(s=="X" || s=="x") grid[i].push_back('X');
            else if(s=="1") grid[i].push_back('0'); // treat 1 as open
            else grid[i].push_back('0');
        }
    }
    vis.assign(M, vector<int>(N,0));
    if(dfs(0,0)){
        for(auto &p: path){
            cout<<p.first<<" "<<p.second<<"\n";
        }
    } else {
        cout<<"No path\n";
    }
    return 0;
}
