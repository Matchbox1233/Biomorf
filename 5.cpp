#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <complex>
#include <cmath>

double oMAXX = 0.7;
double oMINX = -2.0;
double oMAXY = 1.35;
double oMINY = -1.35;

double MAXX = 0.7;
double MINX = -2.0;
double MAXY = 1.35;
double MINY = -1.35;

const double D_X=0.01;

double X = 1;
void compute(std::vector<sf::Vertex> &v,const int &w)
{
    double dx = (MAXX - MINX)  / w;
    double dy = (MAXY - MINY)  / w;

    std::complex<double> C, Z, Zuj;
    int iteracio;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < w; j++)
        {
            C.real(MINX + j * dx);
            C.imag(MAXY - i * dy);

            Z = 0;
            iteracio = 0;

            while(abs(Z) < 2 && iteracio++ < 255)
            {
                Zuj = Z*Z+C;
                Z = Zuj;
            }

            v[i*w+j].color.r = (256 - iteracio)%200;
            v[i*w+j].color.g = (256 - iteracio)%90;
            v[i*w+j].color.b = (256 - iteracio)%256;
        }
    }
}

double scale_x(float m, int w)
{

    return m/w*(MAXX-MINX);
}
double scale_y(float m, int w)
{

    return m/w*(MAXY-MINY);
}

void set_boundaries( float m_x, float m_y, float w, int delta=0)
{

    double DstX = MAXX-MINX;
    double DstY = MAXY-MINY;

    double AvgX = (MAXX+MINX)/2;
    double AvgY = (MAXY+MINY)/2;

    double mid_x = m_x/w * DstX; 
    double mid_y = m_y/w * DstY;

    //std::cout <<DstX<< " "<<DstY<< " "<< mid_x << " "<< mid_y<<std::endl;
    
    std::cout << MINX+scale_x(m_x,w)<< " " << MAXY-scale_y(m_y,w) << std::endl;

    MINX = MINX+scale_x(m_x,w)-DstX/2;
    MAXX = MINX+scale_x(m_x,w)+DstX/2;
    MINY = MAXY-scale_y(m_y,w)-DstX/2;
    MAXY = MAXY-scale_y(m_y,w)+DstX/2;

    std::cout <<MAXX<< " "<<MINX<< " "<< MAXY << " "<< MINY<<std::endl;

}

int main(int argc, char **argv)
{
    if(argc !=2)
    {
        std::cout<< "Használd: ./mandel szélesség"<<std::endl;
        return -1;
    }
    int width;
    std::istringstream ss1(argv[1]);
    if(!(ss1 >> width))
    {
        std::cout<<"Szélesség nem érvényes szám."<<std::endl;
        return -2;
    }
    
    sf::RenderWindow window(sf::VideoMode(width, width), "Mandelbrot ZOOM");
    window.setFramerateLimit(15);

    std::vector<sf::Vertex> gfx;
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<width;j++)
        {
            gfx.push_back(sf::Vertex(sf::Vector2f(j,i),sf::Color::Black));
        }
    }

    float k=1; 
    int prev_delta=1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                std::cout<<"Pressed:"<<std::endl;
                std::cout<<"x: "<<event.mouseButton.x<<std::endl;
                std::cout<<"y: "<<event.mouseButton.y<<std::endl;
                std::cout<<"Button: "<<(event.mouseButton.button == sf::Mouse::Left? "Left" : "Right")<<std::endl;
                set_boundaries(event.mouseButton.x, event.mouseButton.y, width);
            }
            /*else if (event.type == sf::Event::MouseButtonReleased)
            {
                std::cout<<"Released:"<<std::endl;
                std::cout<<"x: "<<event.mouseButton.x<<std::endl;
                std::cout<<"y: "<<event.mouseButton.y<<std::endl;
                std::cout<<"Button: "<<(event.mouseButton.button == sf::Mouse::Left? "Left" : "Right")<<std::endl;
            }*/
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                std::cout<<"Delta: "<<event.mouseWheel.delta<<std::endl;
                std::cout<<"x: "<<event.mouseWheel.x-width/2<<std::endl;
                std::cout<<"y: "<< -(event.mouseWheel.y-width/2)<<std::endl;
                //X+= event.mouseWheel.delta*D_X/2;
                k-=event.mouseWheel.delta;
                
#define F 1.0005
                if(prev_delta!=event.mouseWheel.delta)
                {
                    k = -k;
                }
                //set_boundaries(event.mouseWheel.x, event.mouseWheel.y, width);
                MAXX = MAXX*pow(F,k);
                MINX = MINX*pow(F,k);

                MAXY = MAXY*pow(F,k);
                MINY = MINY*pow(F,k);

                //sf::Mouse::setPosition(sf::Vector2i(width/2,width/2),window);
                prev_delta = event.mouseWheel.delta;
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::R)
                {
                    MAXX = oMAXX;
                    MINX = oMINX;

                    MAXY = oMAXY;
                    MINY = oMINY;
                    k=1;
                }
                if(event.key.code == sf::Keyboard::E)
                {
                    window.close();
                }
            }
        }


        compute(gfx,width);
        window.clear();
        for(int i=0;i<gfx.size();i++)
        {
            window.draw(&gfx[i],1,sf::Points);
        }
        window.display();
        
    }

    
    return 0;
}