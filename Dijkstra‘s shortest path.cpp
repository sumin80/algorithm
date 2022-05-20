#include <iostream>
#include <vector>
#include <queue>
using namespace std;

using pii = pair<int, int>;

constexpr int INF = 0x3f3f3f3f;

class Graph {
public:
	int n;						// 정점의 개수
	vector<pii>* adj;	// 인접 리스트

	Graph(int _n) // 생성자
	{
		this->n = _n;
		this->adj = new vector<pii>[n]; // 동적할당
	}

	// 간선 추가 (양방향)
	void addEdge(int u, int v, int w)
	{
		this->adj[u].push_back({ v, w });
		this->adj[v].push_back({ u, w });
	}
};

class compare {
public:
	// 우선순위 큐를 위한 비교 함수
	bool operator()(pii a, pii b)
	{
		return a.second > b.second;
	}
};

vector<int>* dijkstra(Graph* g, int start) // 다익스트라 알고리즘
{
	// 최단 거리가 발견되면 true
	vector<bool> visited(g->n, false);
	// 해당 정점까지의 거리 (default 값 : INF)
	vector<int> dist(g->n, INF);
	// 최단거리가 업데이트 될 때 바로 이전에 방문하는 정점(경로 출력 위해)
	vector<int>* from = new vector<int>(g->n);

	// 가장 가까운 정점을 찾기 위한 우선순위큐 선언
	priority_queue<pii, vector<pii>, compare> pq;

	// 시작점의 최단거리 발견
	visited[start] = true;
	// 시작지까지의 거리
	dist[start] = 0;

	// 우선순위큐에 삽입
	pq.push({ start, 0 });

	while (!pq.empty())
	{
		// 가장 가까운 정점을 우선순위 큐에서 꺼냄
		int u = pq.top().first;
		pq.pop();

		visited[u] = true;
		for (int j = 0; j < g->adj[u].size(); j++)
		{
			auto v = g->adj[u][j];

			if (!visited[v.first])
			{
				if (dist[u] + v.second < dist[v.first])
				{
					dist[v.first] = dist[u] + v.second;
					(*from)[v.first] = u;

					pq.push({ v.first, dist[v.first] });
				}
			}
		}
	}
	return from;
}

void tracking(int s, int e, vector<int>* from)
{
	// 재귀 종료 조건
	if ((*from)[e] == s)
	{
		cout << char(s + 'A') << " -> ";
		return;
	}

	// 재귀 방식으로 end 전의 정점에 대한 경로 출력
	tracking(s, (*from)[e], from);

	// end 바로 이전의 정점 출력
	cout << char((*from)[e] + 'A') << " -> ";
}

void printPath(int s, int e, vector<int>* from)
{
	tracking(s, e, from);
	cout << char(e + 'A');
}

int main(int argc, char* argv[])
{
	Graph* g = new Graph(6);
	g->addEdge(0, 1, 5); // A -> B
	g->addEdge(0, 3, 2); // A -> D
	g->addEdge(0, 4, 4); // A -> E
	g->addEdge(1, 2, 3); // B -> C
	g->addEdge(1, 3, 3); // B -> D
	g->addEdge(2, 3, 3); // C -> D
	g->addEdge(2, 5, 2); // C -> F
	g->addEdge(2, 4, 4); // C -> E
	g->addEdge(3, 4, 1); // D -> E
	g->addEdge(4, 5, 2); // E -> F

	auto from = dijkstra(g, 0);

	// A부터 각 정점까지의 경로 출력
	for (int i = 0; i < g->n; i++)
	{
		cout << "A -> " << char(i + 'A') << " : ";
		printPath(0, i, from);
		cout << '\n';
	}
	
	return 0;
}
