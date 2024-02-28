#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

// Estructura para representar puntos en el juego
struct Point
{
    int x, y;
} a[4], b[4];

// Definición de las figuras posibles en el juego
int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

// Función para verificar si se puede mover una figura a la nueva posición
bool check()
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return false; // La figura está fuera del tablero
        else if (field[a[i].y][a[i].x])
            return false; // La posición está ocupada por otra figura
    }
    return true;
}

int main()
{
    srand(time(0)); // Inicializar la semilla aleatoria

    RenderWindow window(VideoMode(320, 480), "Tetris");

    // Cargar texturas para los elementos del juego
    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");

    // Crear sprites para los elementos del juego
    Sprite s(t1), background(t2), frame(t3);

    int dx = 0; 
    bool rotate = false; 
    int colorNum = 1;
    float timer = 0, delay = 0.3;

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Up)
                    rotate = true;
                else if (e.key.code == Keyboard::Left)
                    dx = -1;
                else if (e.key.code == Keyboard::Right)
                    dx = 1;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;

        // Mover la figura horizontalmente
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check()) // Si no se puede mover horizontalmente, revertir el movimiento
        {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }

        // Rotar la figura si es necesario
        if (rotate)
        {
            Point p = a[1]; // Centro de rotación
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x + x;
                a[i].y = p.y - y;
            }
            if (!check()) // Si no se puede rotar, revertir la rotación
            {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }
        }

        // Hacer descender la figura
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check()) // Si no se puede mover hacia abajo, colocar la figura en su lugar
            {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }

            timer = 0;
        }

        // Eliminar líneas completas y desplazar las demás hacia abajo
        int k = M - 1;
        for (int i = M - 1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < N; j++)
            {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N)
                k--;
        }

        dx = 0; 
        rotate = false; 
        delay = 0.3;

        // Dibujar los elementos del juego en la ventana
        window.clear(Color::White);
        window.draw(background);

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0)
                    continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18, i * 18);
                s.move(28, 31); // Offset
                window.draw(s);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            s.move(28, 31); // Offset
            window.draw(s);
        }

        window.draw(frame);
        window.display();
    }

    return 0;
}