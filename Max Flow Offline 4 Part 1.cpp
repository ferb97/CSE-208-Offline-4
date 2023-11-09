#include<bits/stdc++.h>
#define INF 1000000005
using namespace std;

int n,m,i,j,k,l;
int rem_games[105][105];
vector<bool> visited;

struct Team
{
    string name;
    int win;
    int lose;
    int rem;
};

struct Node
{
    int id;
    int first;
    int second;
    int weight;

    Node(int i,int f,int s,int w){
        id=i;
        first=f;
        second=s;
        weight=w;
    }
};

Team teams[105];
vector<Node> adj[5005];
vector<Node> v;
vector<int> parent;
int start_id=0,sink_id;
int capacity[5005][5005];


void construct_graph(int x)
{

    v.clear();

    sink_id=n+(n-1)*(n-2)/2;
    for(i=0;i<=sink_id;i++){
        for(j=0;j<=sink_id;j++){
            capacity[i][j]=0;
        }
    }

    for(i=0;i<=sink_id;i++){
        adj[i].clear();
    }

    Node start(0,-1,-1,-1);
    v.push_back(start);

    int node_id[n+3];
    int id=1;

    for(i=0;i<n;i++){
        if(i==x)
          continue;

        Node temp(id,i,-1,INF);
        node_id[i]=id;
        v.push_back(temp);
        id++;

    }

    for(i=0;i<n;i++){
        if(i==x)
          continue;
        for(j=i+1;j<n;j++){
            if(j==x)
              continue;

            Node temp1(id,i,j,rem_games[i][j]);
            v.push_back(temp1);

            adj[0].push_back(temp1);
            capacity[0][id]=rem_games[i][j];

            Node temp4(node_id[i],i,-1,INF);
            adj[id].push_back(temp4);
            capacity[id][node_id[i]]=INF;

            Node temp5(node_id[j],j,-1,INF);
            adj[id].push_back(temp5);
            capacity[id][node_id[j]]=INF;

            id++;

        }
    }

    Node sink(id,-1,-1,-1);
    sink_id=id;
    v.push_back(sink);

    int h=teams[x].win+teams[x].rem;

    for(i=0;i<n;i++){
        if(i==x)
          continue;

        Node temp3(sink_id,-1,-1,h-teams[i].win);
        adj[node_id[i]].push_back(temp3);
        capacity[node_id[i]][sink_id]=h-teams[i].win;
    }
}

int bfs()
{
    parent.assign(n+(n-1)*(n-2)/2+3,-1);
    parent[0]=-2;

    queue<pair<Node,int> > q;
    q.push({v[0],INF});

    while(!q.empty()){

        Node temp=q.front().first;
        int flow=q.front().second;
        q.pop();

        for(i=0;i<=sink_id;i++){
            if(parent[next.id]==-1&&capacity[temp.id][next.id]>0){
                parent[next.id]=temp.id;
                int new_flow=min(flow,capacity[temp.id][next.id]);

                if(next.id==sink_id)
                  return new_flow;
                q.push({next,new_flow});

            }
        }
    }

    return 0;
}


void dfs(int y)
{
    for(Node n1:adj[y]){
        if(capacity[y][n1.id]){
           if(n1.first!=-1&&n1.second==-1)
             visited[n1.first]=true;
           dfs(n1.id);
        }
    }
}


bool is_eliminated()
{
    int flow=0;
    int new_flow;

    while(new_flow=bfs()){
        flow+=new_flow;
        int cur=sink_id;

        while (cur!=0) {
            int prev=parent[cur];
            capacity[prev][cur]-=new_flow;
            capacity[cur][prev]+=new_flow;
            cur=prev;
        }
    }

    visited.assign(n,false);
    bool eliminated=false;

    for(i=n;i<n+(n-1)*(n-2)/2;i++){
        if(capacity[0][i]){
           eliminated=true;
           break;
//           if(!visited[v[i].first])
//              visited[v[i].first]=true;
//           if(!visited[v[i].second])
//              visited[v[i].second]=true;

        }
    }

    if(eliminated){
        dfs(0);
    }

    return eliminated;

}
int main()
{
    int mx_win=0;
    cin>>n;

    string mx_win_team;
    for(i=0;i<n;i++){
        cin>>teams[i].name>>teams[i].win>>teams[i].lose>>teams[i].rem;

        if(mx_win<teams[i].win){
           mx_win=teams[i].win;
           mx_win_team=teams[i].name;
        }

        for(j=0;j<n;j++){
            cin>>rem_games[i][j];
        }

    }
    for(l=0;l<n;l++){

        if(teams[l].win+teams[l].rem<mx_win){
           cout<<teams[l].name<<" is eliminated."<<endl;
           cout<<"They can win at most "<<teams[l].win<<" + "<<teams[l].rem<<" = "<<teams[l].win+teams[l].rem<<" games."<<endl;
           cout<<mx_win_team<<" has won a total of "<<mx_win<<" games."<<endl;
           cout<<"They play each other 0 times."<<endl;
           cout<<"So on average, each of the teams wins "<<mx_win<<"/"<<"1 = "<<mx_win<<" games."<<endl;
           cout<<endl;
           continue;
        }

        construct_graph(l);

        bool eliminated=is_eliminated();

        if(eliminated){
//           for(m=n;m<n+(n-1)*(n-2)/2;m++){
//               cout<<capacity[0][m]<<" ";
//           }
//           cout<<endl;
           vector<int> v1;
           int tot_win=0,tot_rem=0;
           for(m=0;m<n;m++){
              if(visited[m]){
                cout<<m<<" ";
                v1.push_back(m);
                tot_win+=teams[m].win;
                tot_rem+=teams[m].rem;
              }
           }
           cout<<endl;
           cout<<teams[l].name<<" is eliminated."<<endl;
           cout<<"They can win at most "<<teams[l].win<<" + "<<teams[l].rem<<" = "<<teams[l].win+teams[l].rem<<" games."<<endl;
           for(m=0;m<v1.size()-1;m++){
              cout<<teams[v1[m]].name<<" and ";
           }
           cout<<teams[v1[v1.size()-1]].name<<" have won a total of "<<tot_win<<" games."<<endl;
           //cout<<teams[p1.first].name<<" and "<<teams[p1.second].name<<" have won a total of "<<teams[p1.first].win+teams[p1.second].win<<" games."<<endl;
           cout<<"They play each other "<<tot_rem<<" times."<<endl;
           cout<<"So on average, each of the teams wins "<<tot_win+tot_rem<<"/"<<v1.size()<<" = "<<(tot_win+tot_rem)/(v1.size()*1.0)<<" games."<<endl;
           cout<<endl;
        }

    }
}

