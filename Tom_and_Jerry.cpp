#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <string>

using namespace std;

//dfs
void printAllPathsUtil(int startX,int startY, int endX,int endY, bool** visited,
	pair<int,int> path[], int &path_index,int row,int col,int** ary,vector<string>& myVec)
{
	visited[startX][startY] = true;
	path[path_index] = make_pair(startX,startY);
	path_index++;


	if (startX == endX && startY==endY)
	{
		string str="";
		int previ;
		for (int i = 0; i < path_index; i++)
		{
			if (i > 0)
			{
				previ = i - 1;
				if (path[previ].first == path[i].first)
					if (path[previ].second < path[i].second)
					{
						cout << "E" << "->";
						str += "E";
					}
					else
					{
						cout << "W" << "->";
						str += "W";
					}
				else
					if (path[previ].first < path[i].first)
					{
						cout << "S" << "->";
						str += "S";
					}
					else
					{
						cout << "N" << "->";
						str += "N";
					}
			}
			else
				if (i == 0) cout << "T->";
		}
		myVec.push_back(str);
		cout << "J";
		cout << endl;
	}
	else
	{
		// Recur for all the vertices adjacent to current vertex
		
		for (int i = 0; i < 4; i++)
		{
			if (i == 0 && startX - 1 >= 0 && ary[startX - 1][startY] != 1 && !visited[startX - 1][startY])
				printAllPathsUtil(startX - 1, startY, endX, endY, visited, path, path_index, row, col,ary,myVec);
			if (i == 1 && startY + 1 <= col  && ary[startX][startY + 1] != 1 && !visited[startX][startY + 1])
				printAllPathsUtil(startX, startY + 1, endX, endY, visited, path, path_index, row, col,ary,myVec);
			if (i == 2 && startX + 1 <= row  && ary[startX + 1][startY] != 1 && !visited[startX + 1][startY])
				printAllPathsUtil(startX + 1, startY, endX, endY, visited, path, path_index, row, col,ary,myVec);
			if (i == 3 && startY - 1 >= 0 && ary[startX][startY - 1] != 1 && !visited[startX][startY - 1])
				printAllPathsUtil(startX, startY - 1, endX, endY, visited, path, path_index, row, col, ary,myVec);
		}
	}

	path_index--;
	visited[startX][startY] = false;
}

//helper function to determine furnitures geometry
bool hasN(string & str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == 'n')
			return true;
		i++;
	}
	return false;
}

//reading from file and assigning values to my variables
void assignValues(int& m, int& n, int& TomX, int& TomY, int& JerryX, int& JerryY,
	int& FurnitureCount, int& PaintableCount, ifstream& file,
	string*& furnContainer, pair<int, int>*& furnCoord,
	pair<int, int>*& paintableCoord)
{
	file >> m >> n >> JerryX >> JerryY >> TomX >> TomY >> FurnitureCount >> PaintableCount;
	furnCoord = new pair<int, int>[FurnitureCount];
	furnContainer = new string[FurnitureCount];
	int k = FurnitureCount;
	int i = 0;
	while (k > 0)
	{
		string firstRow;
		string secondRow;
		int tempX;
		int tempY;
		file >> tempX >> tempY;
		furnCoord[i].first = tempX;
		furnCoord[i].second = tempY;
		file.get();
		getline(file, firstRow);
		getline(file, secondRow);
		if (secondRow != "===")
		{
			//furnitures[][]
			furnContainer[i] = firstRow +'n' + secondRow;
			string equals;
			getline(file, equals);
		}
		else
		{
			furnContainer[i] = firstRow;
		}
		i++;
		k--;
	}
	paintableCoord = new pair<int, int>[PaintableCount];
	int t = PaintableCount;
	i = 0;
	while (t > 0)
	{
		int tempX;
		int tempY;
		file >> tempX >> tempY;
		paintableCoord[i].first = tempX;
		paintableCoord[i].second = tempY;
		i++;
		t--;
	}
}

//finds the vector element with the lowest size
//im using this to find the shortest path(s)
int findMindSizeCount(vector<string> myVec)
{
	int min = myVec[0].size();
	for (int i = 1; i < myVec.size(); i++)
	{
		if (myVec[i].size() < min)
			min = myVec[i].size();
	}
	return min;
}

//gives info about chosen path properties
void update(string path, int** ary, int& startX, int& startY)
{
	int painted = 0;
	int turns = 0;
	for (int i = 0; i < path.length()-1; i++)
	{
		if (path[i] == 'N' || path[i] == 'S')
		{
			if (path[i + 1] == 'W' || path[i + 1] == 'E')
				turns++;
		}
		else
			if (path[i + 1] == 'N' || path[i + 1] == 'S')
				turns++;
	}

	for (int i = 0; i < path.length(); i++)
	{
		switch (path[i])
		{
		case 'N':if (ary[startX--][startY] == 5) { cout << "The drone is going North and painting!\n"; painted++; }
				 else cout << "The drone is going North!\n"; break;
		case 'S':if (ary[startX++][startY] == 5) { cout << "The drone is going South and painting!\n"; painted++; }
				 else cout << "The drone is going South!\n"; break;
		case 'E':if (ary[startX][startY++] == 5) { cout << "The drone is going East and painting!\n"; painted++; }
				 else cout << "The drone is going East!\n"; break;
		case 'W':if (ary[startX][startY--] == 5) { cout << "The drone is going West and painting!\n"; painted++; }
				 else cout << "The drone is going West!\n"; break;
		}
	}
	cout << endl;
	cout << "Target achieved!\n" << "Painted " << painted << " spot(s)" << endl;
	cout << "Made " << turns << " turn(s)\n";
	cout << "Length " << path.size() << endl << endl;

}
int main()
{
	int m; //row
	int n; //col
	int TomX, TomY;
	int JerryX, JerryY;
	int FurnitureCount, PaintableCount;
	string* furnContainer;
	pair<int, int>* furnCoord;
	pair<int, int>* paintableCoord;
	cout << "Read from:\n> ";
	string info;
	cin >> info;
	cout << endl;
	ifstream read;
	read.open(info);
	assignValues(m, n, TomX, TomY, JerryX, JerryY, FurnitureCount,
				PaintableCount,read,furnContainer,furnCoord,paintableCoord);
	int newM = m + 1;
	int newN = n + 1;

	//dynamic array, representing the room
	int **ary = new int*[newN];
	for (int i = 0; i < newN; ++i)
		ary[i] = new int[newM];
	//assign 0's to the dynamic array
	for (int i = 0; i < newM; i++)
		for (int j = 0; j < newN; j++)
			ary[i][j] = 0;
	//assigning the array
	ary[TomX][TomY] = 2;
	ary[JerryX][JerryY] = 3;
	int i = 0;
	
	//assinging printable spots
	while (PaintableCount > 0)
	{
		ary[paintableCoord[i].first][paintableCoord[i].first]=5;
		i++;
		PaintableCount--;
	}
	

	//assingning furniture
	i = 0;
	while (FurnitureCount > 0)
	{
		if (hasN(furnContainer[i]))
		{
			int j = 0;
			int temporX = furnCoord[i].first;
			int temporY = furnCoord[i].second;
			int l = j;
			while (furnContainer[i][j])
			{
				if (furnContainer[i][j] == '1')
				{
					ary[temporX][temporY + l] = 1;
				}
				else if (furnContainer[i][j] == ' ')
				{
					ary[temporX][temporY + l] = 0;
				}
				else if (furnContainer[i][j] == 'n')
				{
					temporX++;
					l = -1;
				}
				l++;
				j++;
			}
		}
		else
		{
			int j = 0;
			while (furnContainer[i][j])
			{
				if (furnContainer[i][j] == '1')
				{
					ary[furnCoord[i].first][furnCoord[i].second + j] = 1;
				}
				j++;
			}
		}
		i++;
		FurnitureCount--;
	}


	//Testing if Containers are filled correctly :)
	cout << "The current situation in the room: " << endl;
	for (int i = 0; i < newM; i++)
	{
		for (int j = 0; j < newN; j++)
		{
			if (ary[i][j] == 2) cout << " T ";
			else if (ary[i][j] == 3) cout << " J ";
			else if (ary[i][j] == 1) cout << " F ";
			else if (ary[i][j] == 5) cout << " P ";
			else cout << " "<<ary[i][j]<<" ";
		}
		cout << endl;
	}
	cout << "\nF - Furniture\n" << "J - Jerry\n" << "T - Tom\n" << "P - Paintable Spots\n";
	cout << "\nTom! Quickly, press S to see all available paths for your drone!\n> ";
	string press;
	cin >> press;
	cout << endl;
	while (press != "S")
	{
		cout << "\nYou sly cat, you didn't press S!\nOr maybe CAPS LOCK wasn't on ;)?\nTry again !\n> ";
		cin >> press;
	}
	
	//path for dfs
	pair<int, int>* path;
	path = new pair<int,int>[newN*newM];

	//visited array for dfs
	bool **visited = new bool*[newN];
	for (int i = 0; i < newN; ++i)
		visited[i] = new bool[newM];

	for (int i = 0; i < newM; i++)
	{
		for (int j = 0; j < newN; j++)
			visited[i][j] = false;
	}

	vector<string> pathContainer;
	int pathIndex = 0;

	//dfs
	printAllPathsUtil(TomX, TomY, JerryX, JerryY,visited,path,pathIndex, m, n,ary,pathContainer);
	
	cout << "\nTom! Quickly, press B to see the best routes for your drone!\n> ";
	cin >> press;
	cout << endl;
	while (press != "B")
	{
		cout << "\nYou sly cat, you didn't press B !\nOr maybe wasn't CAPS LOCK was on ;)?\nTry again!\n> ";
		cin >> press;
		cout << endl;
	}

	//shortest paths
	vector<string> minimumPathCont;
	for (int i = 0; i < pathContainer.size(); i++)
		if (pathContainer[i].size() == findMindSizeCount(pathContainer))
			minimumPathCont.push_back(pathContainer[i]);
	for (int i = 0; i < minimumPathCont.size(); i++)
	{
		cout << "Number " << i + 1 << " : ";
		for (int j = 0; j < minimumPathCont[i].size(); j++)
			if (j == minimumPathCont[i].size() - 1)
				cout << minimumPathCont[i][j];
			else
				cout << minimumPathCont[i][j] << " -> ";
		cout << endl;
	}
	cout << "Enter the number of your choice, pussy cat!\n> ";
	int choice;
	cin >> choice;
	cout << endl;
	cout << "Your choice is path number " << choice << ": ";
	for (int j = 0; j < minimumPathCont[choice-1].size(); j++)
		if (j == minimumPathCont[i].size() - 1)
			cout << minimumPathCont[i][j];
		else
			cout << minimumPathCont[i][j] << " -> ";
	cout << endl;
	
	int tempX = TomX;
	int tempY = TomY;
	cout << endl;

	//final info
	update(minimumPathCont[choice - 1], ary, tempX, tempY);

    return 0;
}

