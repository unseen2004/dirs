#include <iostream>
#include <array>
#include <limits>
#include <cassert>
#include <random>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


constexpr int g_consoleLines{ 25 };

class Direction
{
public:
    enum Type
    {
        up,
        down,
        right,
        left,
        max_type
    };

    Direction(Type direction)
    : m_actual_direction {direction}
    {}

    Type getDirection() const
    {
        return m_actual_direction;
    }

    Direction operator-() const
    {
        switch (m_actual_direction)
        {
        case up:    return Direction{ down };
        case down:  return Direction{ up };
        case left:  return Direction{ right };
        case right: return Direction{ left };
        default:    break;
        }

        assert(0 && "Unsupported direction was passed!");
        return Direction{ up };
    }

    static Direction getRandomDirection()
    {
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution getrandom{ 0, static_cast<int>(Type::max_type) - 1};
        Type random{ static_cast<Type>(getrandom(mt))};
        return Direction{ random };
    }

private:
    Type m_actual_direction {};    
};

std::ostream& operator<< (std::ostream& out, const Direction direction)
{
    switch(direction.getDirection())
    {
        case Direction::up: return (out << "up");
        case Direction::down: return (out << "down");
        case Direction::left: return (out << "left");
        case Direction::right: return (out << "right");
        default:            break;
    }
    assert(0 && "Unsupported direction was passed!");
    return (out << "unknown direction");
}

namespace UserInput 
{
    void ignoreLine()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    char getCommandFromUser()
    {
        char input {};
        while(true)
        {
            std::cin >> input;
            ignoreLine();
            switch(input)
            {
                case 'a':
                case 's':
                case 'd':
                case 'w':
                case 'q':
                    return input;
                default:
                    continue;
            }
        }
    }

    Direction charToDirection(char ch)
    {
        switch (ch)
        {
        case 'w': return Direction{ Direction::up };
        case 's': return Direction{ Direction::down };
        case 'a': return Direction{ Direction::left };
        case 'd': return Direction{ Direction::right };
        }

        assert(0 && "Unsupported direction was passed!");
        return Direction{ Direction::up };
    }
    

}

class Tile 
{
private:
    int m_number {0};
public:
    Tile() = default;
    explicit Tile(int number)
    : m_number{number}
    {}

    bool isEmpty() const { return m_number==0; }
    int getNum()   const { return m_number; }

    //friend std::ostream& operator<< (std::ostream& out, const Tile& tile);

};

std::ostream& operator<< (std::ostream& out, const Tile& tile)
{
    if (tile.getNum() > 9) // if two digit number
        out << " " << tile.getNum() << " ";
    else if (tile.getNum() > 0) // if one digit number
        out << "  " << tile.getNum() << " ";
    else if (tile.getNum() == 0) // if empty spot
        out << "    ";
    return out;
}

struct Point
{
    int x {};
    int y {};

    friend bool operator==(const Point p1, const Point p2) { return (p1.x == p2.x && p1.y == p2.y);}
    friend bool operator!=(const Point p1, const Point p2) { return !operator==(p1, p2); }
    

    Point getAdjacentPoint(Direction direction) const
    {
        switch(direction.getDirection())
        {
            case Direction::up: return Point{x, y-1};
            case Direction::down: return Point{x, y+1};
            case Direction::left: return Point{x-1, y};
            case Direction::right: return Point{x+1, y};
            default: break;
        }
        assert(0 && "Unsupported direction was passed!");
        return *this;
    }
};

class Board
{
private:
    static constexpr int m_grid_size {4};
    std::array<std::array<Tile, m_grid_size>, m_grid_size> m_tiles{{
        std::array<Tile, m_grid_size>{Tile{ 1 }, Tile { 2 }, Tile { 3 }, Tile { 4 }},
        std::array<Tile, m_grid_size>{Tile { 5 }, Tile { 6 }, Tile { 7 }, Tile { 8 }},
        std::array<Tile, m_grid_size>{Tile { 9 }, Tile { 10 }, Tile { 11 }, Tile { 12 }},
        std::array<Tile, m_grid_size>{Tile { 13 }, Tile { 14 }, Tile { 15 }, Tile { 0 }}
    }};

public:
    Board() = default;
    friend std::ostream& operator<< (std::ostream& out, const Board& board);

    bool isValid(Point p){ return p.x < m_grid_size && p.x >= 0 && p.y < m_grid_size && p.y >= 0; }
    Point emptyIndex() 
    { 
        // Flatten the grid into a 1D container of iterators to tiles
        auto begin = m_tiles.front().begin();
        auto end = m_tiles.back().end();

        // Use std::min_element with a custom comparator to find the tile with number 0
        auto it = std::min_element(begin, end, [](const Tile& a, const Tile& b) {
            return a.getNum() < b.getNum(); // 0 is the smallest
        });

        // Get the index of the empty tile
        int idx = std::distance(begin, it);
        return Point{ idx % m_grid_size, idx / m_grid_size };
    }
    void swap(Point& p1, Point& p2)
    {
        std::swap(m_tiles[p1.y][p1.x], m_tiles[p2.y][p2.x]);
    }
    bool moveTile(Direction dir)
    {
        Point empty_tail {emptyIndex() };
        Point adj {empty_tail.getAdjacentPoint(-dir) };
        
        if(isValid(adj))
        {
            swap(adj, empty_tail);
            return true;
        }
        return false;
    }

    void randomize()
    {
        constexpr int iterations {1000};
        for (int i{0}; i<iterations;)
        {
            if (moveTile(Direction::getRandomDirection()))
                ++i;
        }
    }

    friend bool operator==(const Board& b1, const Board& b2)
    {
        for (int y = 0; y < m_grid_size; ++y)
            for (int x = 0; x < m_grid_size; ++x)
                if (b1.m_tiles[y][x].getNum() != b2.m_tiles[y][x].getNum())
                    return false;

        return true;
    }
    

    bool playerWon() const
    {
        static Board s_solved{};  
        return s_solved == *this; 
    }
    
};

std::ostream& operator<< (std::ostream& out, const Board& board)
{
    for (int i{0}; i < g_consoleLines; ++i)
        std::cout<<'\n';
    for (int y{0}; y < board.m_grid_size; ++y)
    {
        for (int x{0}; x < board.m_grid_size; ++x)
            out << board.m_tiles[y][x];
        out << '\n';
    }
    return out;
}

void wait()
{
    for (int i : {3, 2, 1})
    {
        std::cout<<i<<"\n";
        sleep(1);
    }
    
}

int main()
{
    Board board{};
    board.randomize();
    std::cout<<"move: w,a,s,d\nquit: q\n";
    wait();
    std::cout << board;
    
    do
    {
        char input {UserInput::getCommandFromUser()};
        if (input == 'q')
        {
            std::cout<<"\n\nBye!\n\n";
            return 0;
        }

        Direction dir{ UserInput::charToDirection(input) };  

        bool userMoved { board.moveTile(dir) };
        if (userMoved)
            std::cout << board;
        else
            std::cout<<"You can't move there! \n";

    }while(!board.playerWon());

    
    std::cout<<"\n\nYou won!\n\n";
    return 0;
}