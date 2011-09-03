////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <sstream>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

using namespace std;

namespace Pong
{
class Ball
{

private:
    const int radius, speed;
    sf::Color color;
    int pos_x, pos_y;
    float vel_x, vel_y;
    sf::Shape ball;
public:
    Ball ( ) :pos_x(390), pos_y(290), vel_x(1), vel_y(1),color( sf::Color::White), radius(5), speed(2)
    {
        Update();
    };
    void Update()
    {
        ball = sf::Shape::Circle( pos_x, pos_y, radius, color);
    };

    void resetBall()
    {
        pos_x = 390;
        pos_y = 290;
    };

    void yCollissionHandler()
    {
        vel_y = vel_y * -1;
    };

    void xCollissionHandler()
    {
        vel_x = vel_x * -1;
    };

    void Move()
    {
        //detectCollision();
        pos_x += vel_x;
        pos_y += vel_y;
        Update();
    };

    int getX1()
    {
        return(pos_x);
    };

    int getX2()
    {
        return(pos_x + radius*2 );
    };

    int getY1()
    {
        return(pos_y);
    };

    int getY2()
    {
        return(pos_y + radius*2);
    };

    sf::Shape getShape()
    {
        return ball;
    };

    /*void detectCollision()
    {
    	table.Collision(this);
    };*/
};

class Player
{
private:
    sf::String ScoreBoard;
    string name;
    int score;
public:
    Player(string name, int pos_x, int pos_y ) :score(0), name(name), ScoreBoard( "0", sf::Font::GetDefaultFont(), 30)
    {
        ScoreBoard.SetPosition(pos_x, pos_y);
        ScoreBoard.SetColor(sf::Color::Red);
    };

    void addPoint()
    {
        this->score++;
        ScoreBoard.SetText( this->getScoreAsString() );
//					cout << this->name << " has " << this->score << " points.\n";
    };

    string getName()
    {
        return(name);
    };

    string getScoreAsString()
    {
        std::ostringstream scoreAsString;
        scoreAsString << this->score;
        return scoreAsString.str();
    };

    sf::String &getScoreBoard()
    {
        return( this->ScoreBoard );
    }
};

class Paddle
{

private:
    const int size_x, size_y, yposStart, yposEnd, speed;
    sf::Color color;
    int pos_x, pos_y;
    sf::Shape paddle;
public:
    Paddle ( int pos_x ) :pos_x(pos_x), pos_y(270),size_x(10), size_y(60), yposStart(0), yposEnd(100), speed(2),paddle(sf::Shape::Rectangle( pos_x, pos_y, pos_x + size_x, pos_y + size_y , sf::Color::White)){};


    int getX1()
    {
        return(pos_x);
    };

    int getX2()
    {
        return(pos_x + size_x);
    };

    int getY1()
    {
        return(pos_y);
    };

    int getY2()
    {
        return(pos_y + size_y);
    };

    void topCollissionHandler()
    {
        this->MoveDown();
    };

    void bottomCollissionHandler()
    {
        this->MoveUp();
    };

    void Collision( Ball &b )
    {
        if((pos_x <= b.getX2() && pos_x + size_x >= b.getX1() && pos_y <= b.getY1() && pos_y + size_y >= b.getY2()))
        {
            b.xCollissionHandler(); //
        };
    };

    virtual void MoveUp()
    {
        pos_y -= speed;
        paddle.Move(0, -speed);
    };

    void MoveDown()
    {
        pos_y += speed;
        paddle.Move(0, speed);
    };

    sf::Shape getShape()
    {
        return paddle;
    };
};

class Table
{
    private:
        int size_x, size_y, pos_x, pos_y;
        sf::Shape table;
    public:
        Table (int width, int height) :size_y(height), size_x(width), pos_x(10), pos_y(10), table(sf::Shape::Rectangle( pos_x, pos_y, pos_x + size_x, pos_y + size_y , sf::Color::Black, 5, sf::Color::White)) {};

        sf::Shape getShape()
        {
            return(table);
        };

        void Collision(Ball &b, Player &p1, Player &p2)
        {
            if((pos_y >= b.getY1()) || (pos_y + size_y <= b.getY2()))
            {
                b.yCollissionHandler(); //
            }
            else if (pos_x >= b.getX2())
            {
                b.resetBall();
                b.xCollissionHandler();
                p2.addPoint();
            }
            else if (pos_x + size_x <= b.getX1())
            {
                b.resetBall();
                b.xCollissionHandler();
                p1.addPoint();
            };
        };

        void Collision(Paddle &p)
        {
            if(pos_y >= p.getY1())
            {
                p.topCollissionHandler(); //
            }
            else if(pos_y + size_y <= p.getY2())
            {
                p.bottomCollissionHandler();
            }
        };
    };
};

int main()
{
    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML Shapes");

    Pong::Table table(780, 580);
    Pong::Paddle pad1(20);
    Pong::Paddle pad2(770);
    Pong::Ball ball;
    Pong::Player player1("Player 1", 70, 20);
    Pong::Player player2("Player 2", 720, 20);


    sf::Event Event;
    sf::Clock Clock;

    while (App.IsOpened())
    {
        float Time = Clock.GetElapsedTime();
        Clock.Reset();

        if (App.GetEvent(Event))
        {
            if ( Event.Type == sf::Event::Closed )
            {
                App.Close();
            }
        }

        if ( App.GetInput().IsKeyDown(sf::Key::W) )
        {
            pad1.MoveUp();
        }
        if ( App.GetInput().IsKeyDown(sf::Key::S) )
        {
            pad1.MoveDown();
        }
        if ( App.GetInput().IsKeyDown(sf::Key::Up) )
        {
            pad2.MoveUp();
        }
        if ( App.GetInput().IsKeyDown(sf::Key::Down) )
        {
            pad2.MoveDown();
        }

        App.Clear();

        table.Collision(ball, player1, player2);

        table.Collision(pad1);
        table.Collision(pad2);

        pad1.Collision(ball);
        pad2.Collision(ball);

        ball.Move();

        App.Draw( table.getShape() );
        App.Draw( ball.getShape() );
        App.Draw( pad1.getShape() );
        App.Draw( pad2.getShape() );
        App.Draw( player1.getScoreBoard() );
        App.Draw( player2.getScoreBoard() );
        App.Display();

        sf::Sleep(.005F);
    }
    return EXIT_SUCCESS;
}
