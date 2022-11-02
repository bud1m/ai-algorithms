#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <queue>

#include "./include/node.h"
#include "button.h"

#define OLC_PGE_APPLICATION
#include "../headers/olcPixelGameEngine.h"

using namespace std;

// Override base class with your custom functionality
class aStarImplementation : public olc::PixelGameEngine
{
public:
    aStarImplementation()
    {
        // Name your application
        sAppName = "Search algorithms";
    }

private:
    sNode *nodes = nullptr;
    int nMapWidth = 16;
    int nMapHeight = 16;
    sNode *nodeStart = nullptr;
    sNode *nodeEnd = nullptr;

    vector<string> buttonsName{"DFS", "BFS", "A*"};
    sButton *buttons = nullptr;

public:
    bool
    OnUserCreate() override
    {
        // Init nodes
        nodes = new sNode[nMapWidth * nMapHeight];
        for (int x = 0; x < nMapWidth; x++)
        {
            for (int y = 0; y < nMapHeight; y++)
            {
                nodes[nMapWidth * y + x].x = x;
                nodes[nMapWidth * y + x].y = y;
                nodes[nMapWidth * y + x].bObstacle = false;
                nodes[nMapWidth * y + x].parent = nullptr;
                nodes[nMapWidth * y + x].bVisited = false;
            }
        }
        // Init buttons
        buttons = new sButton[buttonsName.size()];
        for (int i = 0; i < buttonsName.size(); i++)
        {
            buttons[i].height = 10;
            buttons[i].witdth = 10;
            buttons[i].x = nMapHeight + 10;
            buttons[i].y = nMapWidth + 10;
            buttons[i].text = buttonsName[i];
        }

        // Create connections
        for (int x = 0; x < nMapWidth; x++)
        {
            for (int y = 0; y < nMapHeight; y++)
            {
                if (y > 0)
                    nodes[y * nMapHeight + x].vecAdj.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
                if (y < nMapHeight - 1)
                    nodes[y * nMapHeight + x].vecAdj.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);

                if (x > 0)
                    nodes[y * nMapHeight + x].vecAdj.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
                if (x < nMapWidth - 1)
                    nodes[y * nMapHeight + x].vecAdj.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);
            }
        }

        nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + 1];
        nodeEnd = &nodes[(nMapHeight / 2) * nMapWidth + nMapWidth - 2];

        Solve_AStar();
        // Called once at the start, so create things here
        return true;
    }

    // bool Solve_Dfs()
    // {
    //     for (int x = 0; x < nMapWidth; x++)
    //     {
    //         for (int y = 0; y < nMapHeight; y++)
    //         {
    //             nodes[nMapWidth * y + x].bVisited = false;
    //             nodes[nMapWidth * y + x].fGlobalGoal = INFINITY;
    //             nodes[nMapWidth * y + x].fLocalGoal = INFINITY;
    //             nodes[nMapWidth * y + x].parent = nullptr;
    //         }
    //     }

    //     sNode *nodeCurrent = nodeStart;
    //     list<sNode *> visited;

    //     visited.push_back(nodeCurrent);

    //     while (nodeCurrent != nodeEnd || visited.empty())
    //     {
    //         nodeCurrent = visited.back();
    //         visited.pop_back();
    //         for (int i = 0; i < nodeCurrent->vecAdj.size(); i++)
    //         {
    //         }
    //     }
    // }
    bool Solve_AStar()
    {
        for (int x = 0; x < nMapWidth; x++)
        {
            for (int y = 0; y < nMapHeight; y++)
            {
                nodes[nMapWidth * y + x].bVisited = false;
                nodes[nMapWidth * y + x].fGlobalGoal = INFINITY;
                nodes[nMapWidth * y + x].fLocalGoal = INFINITY;
                nodes[nMapWidth * y + x].parent = nullptr;
            }
        }

        auto distance = [](sNode *a, sNode *b)
        {
            return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
        };

        auto heuristic = [distance](sNode *a, sNode *b)
        {
            return distance(a, b);
        };
  
        sNode *nodeCurrent = nodeStart;

        nodeStart->fLocalGoal = 0.0f;
        nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

        list<sNode *> listNotTesetedNodes;
        listNotTesetedNodes.push_back(nodeStart);

        while (!listNotTesetedNodes.empty() && nodeCurrent != nodeEnd)
        {
            listNotTesetedNodes.sort([](const sNode *lhs, const sNode *rhs)
                                     { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

            while (!listNotTesetedNodes.empty() && listNotTesetedNodes.front()->bVisited)
                listNotTesetedNodes.pop_front();

            if (listNotTesetedNodes.empty())
                break;

            nodeCurrent = listNotTesetedNodes.front();
            nodeCurrent->bVisited = true;

            for (auto adj : nodeCurrent->vecAdj)
            {
                if (!adj->bVisited && adj->bObstacle == 0)
                    listNotTesetedNodes.push_back(adj);

                float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, adj);

                if (fPossiblyLowerGoal < adj->fGlobalGoal)
                {
                    adj->parent = nodeCurrent;
                    adj->fLocalGoal = fPossiblyLowerGoal;

                    adj->fGlobalGoal = adj->fLocalGoal + heuristic(adj, nodeEnd);
                }
            }
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        int nNodeSize = 16;
        int nNodeBorder = 2;
        int nSize = nNodeSize - nNodeBorder;
        // Check for left mouse key pressedF
        int nSelectedNodeX = GetMouseX() / nNodeSize;
        int nSelectedNodeY = GetMouseY() / nNodeSize;

        int nButtonX = GetMouseX();
        int nButtonY = GetMouseY();

        if (GetMouse(olc::Mouse::LEFT).bReleased)
        {

            if (nSelectedNodeX >= 0 && nSelectedNodeX < nMapWidth)
            {
                if (nSelectedNodeY >= 0 && nSelectedNodeY < nMapHeight)
                {

                    if (GetKey(olc::Key::SHIFT).bHeld)
                    {
                        nodeStart = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
                    }

                    else if (GetKey(olc::Key::CTRL).bHeld)
                    {
                        nodeEnd = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
                    }
                    else
                        nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle = !nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle;
                    Solve_AStar();
                }
            }
            else if (nButtonX < nMapWidth)
            {
                if (nButtonY < nMapHeight)
                {
                }
            }
        }

        FillRect(0, 0, ScreenWidth(), ScreenHeight(), L' ');

        // Drawing buttons
        for (int x = 0; x < buttonsName.size(); x++)
        {
            FillRect(nNodeBorder + x * 50 + x * 10, nMapWidth * nNodeSize + 10, 50, 50, olc::GREEN);

            DrawString(nNodeBorder + x * 50, nMapWidth * nNodeSize + 10, buttonsName[x]);
        }
        // Debug drawing
        for (int i = 0; i < 10; i++)
        {
            DrawString(nMapWidth * nNodeSize, nNodeBorder, "Debug info:");
        }

        // Drawing before nodes
        for (int x = 0; x < nMapWidth; x++)
        {
            for (int y = 0; y < nMapHeight; y++)
            {
                for (auto n : nodes[y * nMapWidth + x].vecAdj)
                    DrawLine(x * nNodeSize + nNodeSize / 2, y * nNodeSize + nNodeSize / 2, n->x * nNodeSize + nNodeSize / 2, n->y * nNodeSize + nNodeSize / 2, olc::DARK_BLUE);
            }
        }
        for (int x = 0; x < nMapWidth; x++)
        {
            for (int y = 0; y < nMapHeight; y++)
            {

                FillRect(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, nSize, nSize, nodes[y * nMapWidth + x].bObstacle ? olc::GREY : olc::DARK_BLUE);

                if (nodes[y * nMapWidth + x].bVisited)
                    FillRect(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, nSize, nSize, olc::BLUE);

                if (&nodes[y * nMapWidth + x] == nodeStart)
                    FillRect(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, nSize, nSize, olc::GREEN);

                if (&nodes[y * nMapWidth + x] == nodeEnd)
                    FillRect(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, nSize, nSize, olc::RED);
            }
        }

        if (nodeEnd != nullptr)
        {
            sNode *p = nodeEnd;
            while (p->parent != nullptr)
            {
                p->bVisited = true;
                DrawLine(p->x * nNodeSize + nNodeSize / 2, p->y * nNodeSize + nNodeSize / 2, p->parent->x * nNodeSize + nNodeSize / 2, p->parent->y * nNodeSize + nNodeSize / 2, olc::YELLOW);

                p = p->parent;
            }
        }
        return true;
    }
};

int main()
{
    aStarImplementation game;
    game.Construct(450, 300, 2, 2);
    game.Start();

    return 0;
}
