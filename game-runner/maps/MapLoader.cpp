#include "MapLoader.h"
#include "../graphics/gfx.h"
#include "../FSM/GameState.h"

using namespace std;

int MapLoader::loadFileList()
{
    struct dirent *d;
    DIR *dr;
    dr = opendir("maps");

    if (dr != NULL)
    {
        for (d=readdir(dr); d!=NULL; d=readdir(dr))
        {
            string str(d->d_name);
            if (str.find(".csv") != string::npos)
            {   
                fileList.push_back(str.substr(0, (str.length() - 4)));
            }
        }
        closedir(dr);
    }

    if (fileList.size() / 4 == 0)
    {
        return ceil(fileList.size() / 4);
    } else {
        return ceil(fileList.size() / 4) + 1;
    }

}

vector<string> MapLoader::getFileList()
{
    return fileList;
}

Marple* MapLoader::loadMapFile(int fileIndex, Canvas *c)
{
    ifstream file("maps/" + fileList[fileIndex] + ".csv");

    vector<vector<string>> mapData;
    vector<string> row;
    string line, word;

    while (getline(file, line))
    {
        row.clear();

        stringstream str(line);

        while (getline(str, word, ',')) 
        {
            row.push_back(word);
        }

        mapData.push_back(row);
    }

    file.close();

    //

    vector<Wall*> walls;
    vector<Hole*> holes;
    int w = 0;
    int h = 0;

    Marple *m;

    // For every row
    for (int y=0; y<64; y++)
    {   
        // For every cell
        for (int x=0; x<64; x++)
        {
            // Is a Wall here?
            if (mapData[y][x][0] == 'W')
            {   

                // Check for 2x2 square
                if (y < 63 && x < 63 && 
                    mapData[y+1][x][0] == 'W' && 
                    mapData[y][x+1][0] == 'W' &&
                    mapData[y+1][x+1][0] == 'W') {

                    // Clear redundant data for wall piece
                    mapData[y+1][x] = "";
                    mapData[y+1][x+1] = "";

                    // Check for 3x3 square
                    if (x < 62 && y < 62 &&
                        mapData[y][x+2][0] == 'W' &&
                        mapData[y+1][x+2][0] == 'W' &&
                        mapData[y+2][x+2][0] == 'W' &&
                        mapData[y+2][x][0] == 'W' &&
                        mapData[y+2][x+1][0] == 'W')
                    {

                        // Clear redundant data for wall piece
                        mapData[y+1][x+2] = "";
                        mapData[y+2][x] = "";
                        mapData[y+2][x+1] = "";
                        mapData[y+2][x+2] = "";

                        // Add new wall of size 3x3
                        walls.push_back(new Wall(x, y, 3));
                        walls[w]->setColour(getColour(mapData[y][x].substr(2, 6)));
                        w++;

                        //cout << "Wall found at " << x << "," << y << endl; 

                        x += 2;
                    }
                    else
                    {
                        // Add new wall of size 2x2
                        walls.push_back(new Wall(x, y, 2));
                        walls[w]->setColour(getColour(mapData[y][x].substr(2, 6)));
                        w++;

                        //cout << "Wall found at " << x << "," << y << endl;

                        x++;
                    }
                }
                else
                {

                    // Add new wall of size 1x1
                    walls.push_back(new Wall(x, y, 1));
                    walls[w]->setColour(getColour(mapData[y][x].substr(2, 6)));
                    w++;

                    //cout << "Wall found at " << x << "," << y << endl;
                }
            }
            else if (mapData[y][x][0] == 'S')
            {
                m = new Marple(x, y, 3, new Home(x, y, 3));
                x+=2;
                //Marple marple(x, y, 2);
                //marple.setColour(getColour(mapData[y][x].substr(2,6)));
            }
            else if (mapData[y][x][0] == 'E')
            {
                // Marple marple(x, y, 2);
                drawZone(x, y, 3, c, clock());
                x+=2;
            }
            else if (mapData[y][x][0] == 'H')
            {
                holes.push_back(new Hole(x, y, 3));
                h++;
                x+=2;
            }
        }
    }
    return m;
}

vector<int> getColour(string hexString)
{
    int x;
    stringstream ss;
    vector<int> colours;

    for (int i=0; i<6; i+=2)
    {   
        ss.clear();
        x=0;

        ss << hex << hexString.substr(i, 2);
        ss >> x;
        colours.push_back(x);
    }

    return colours;
}