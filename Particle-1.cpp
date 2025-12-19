#include "Particle.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace Matrices;
using namespace sf;
using namespace std;

bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }


    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}

    Particle::Particle(RenderTarget & target, int numPoints, Vector2i mouseClickPosition)
        : m_A(2, numPoints) // Initialize m_A with 2 rows (x,y) and numPoints columns
    {
        // Initialize member variables
        m_ttl = TTL;
        m_numPoints = numPoints;

        // Random angular velocity [0:PI]
        m_radiansPerSec = ((float)rand() / (RAND_MAX)) * M_PI;

        // Configure the Cartesian plane View
        m_cartesianPlane.setCenter(0, 0);
        // Invert the y-axis
        m_cartesianPlane.setSize(target.getSize().x, (-1.0f) * target.getSize().y);

        // Map mouse click position (pixel) to Cartesian coordinate (m_centerCoordinate)
        m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

        // Random initial velocities
        float initialSpeed = (float)(rand() % 401 + 100); // [100:500]
        m_vx = initialSpeed;
        if (rand() % 2 != 0) // Randomly flip sign
        {
            m_vx *= -1.0f;
        }

        initialSpeed = (float)(rand() % 401 + 100); // [100:500]
        m_vy = initialSpeed;
        if(rand() % 2 != 0) // Randomly flip sign
        {
            m_vy *= -1.0f;
        }

        // Assign colors
        m_color1 = Color::White; // Center color
        // Random color for edges
        m_color2 = Color(rand() % 256, rand() % 256, rand() % 256);

        // Randomly generate vertices
        float theta = ((float)rand() / (RAND_MAX)) * (M_PI / 2.0f); // [0: PI/2]
        float dTheta = 2.0f * M_PI / (numPoints - 1); // Amount to rotate per vertex

        for (int j = 0; j < numPoints; j++)
        {
            float r = (float)(rand() % 61 + 20); // Random radius [20:80]
            float dx = r * cos(theta);
            float dy = r * sin(theta);

            // Store Cartesian coordinates in m_A (shifted from center)
            m_A(0, j) = m_centerCoordinate.x + dx;
            m_A(1, j) = m_centerCoordinate.y + dy;

            theta += dTheta; // Increment angle for the next vertex
        }
    }

    void Particle::translate(double xShift, double yShift)
    {
        // Construct TranslationMatrix T (2 rows, m_numPoints columns)
        TranslationMatrix T(xShift, yShift, m_numPoints);

        // Apply translation: m_A = T + m_A
        m_A = T + m_A;

        // Update the particle's center coordinate
        m_centerCoordinate.x += xShift;
        m_centerCoordinate.y += yShift;
    }

    void Particle::rotate(double theta)
    {
        // 1. Store center
        Vector2f temp = m_centerCoordinate;

        // 2. Shift particle to origin (0, 0)
        translate(-temp.x, -temp.y);

        // 3. Construct RotationMatrix R
        RotationMatrix R(theta);

        // 4. Apply rotation (Left-multiply: m_A = R * m_A)
        m_A = R * m_A;

        // 5. Shift particle back to its original center
        translate(temp.x, temp.y);
    }

    void Particle::scale(double c)
    {
        // 1. Store center
        Vector2f temp = m_centerCoordinate;

        // 2. Shift particle to origin (0, 0)
        translate(-temp.x, -temp.y);

        // 3. Construct ScalingMatrix S
        ScalingMatrix S(c);

        // 4. Apply scaling (Left-multiply: m_A = S * m_A)
        m_A = S * m_A;

        // 5. Shift particle back to its original center
        translate(temp.x, temp.y);
    }

    void Particle::update(float dt)
	{
	    m_ttl -= dt;
	
	    rotate(dt * m_radiansPerSec);
	    scale(SCALE);
	
	    float dx = m_vx * dt;
	    m_vy -= G * dt;
	    float dy = m_vy * dt;
	
	    translate(dx, dy);
	
	    // ----- Aurora color blending -----
	    float lifeRatio = m_ttl / TTL;
	    if (lifeRatio < 0.f) lifeRatio = 0.f;
	    if (lifeRatio > 1.f) lifeRatio = 1.f;
	
	    sf::Color auroraColor(120, 190, 255);
	    sf::Color white(255, 255, 255);
	
	    float blend = sqrt(lifeRatio);
	
	    sf::Uint8 r = white.r * (1 - blend) + auroraColor.r * blend;
	    sf::Uint8 g = white.g * (1 - blend) + auroraColor.g * blend;
	    sf::Uint8 b = white.b * (1 - blend) + auroraColor.b * blend;
	    sf::Uint8 a = static_cast<sf::Uint8>(255 * lifeRatio);
	
	    m_color1 = sf::Color(r, g, b, a);
	    m_color2 = sf::Color(r, g, b, static_cast<sf::Uint8>(a * 0.8f));
	
	    // ----- Ground bounce -----
	    float groundY = -m_cartesianPlane.getSize().y / 2.0f;
	
	    if (m_centerCoordinate.y <= groundY)
	    {
	        float correction = groundY - m_centerCoordinate.y;
	        translate(0.0, correction);
	        m_vy = -m_vy * 0.6f;
	    }
	}


    void Particle::draw(RenderTarget& target, RenderStates states) const
    {
        // Construct VertexArray (TriangleFan primitive type)
        // Size is m_numPoints + 1 (to account for the center point)
        VertexArray lines(TriangleFan, m_numPoints + 1);

        // 1. Map center coordinate (Cartesian) to pixel coordinate (Monitor)
        Vector2f center = (Vector2f)target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);
        // 2. Assign center vertex (index 0)
        lines[0].position = center;
        lines[0].color = m_color1; // m_color1 (White) is the center color

        // 3. Assign all other vertices (indices 1 to m_numPoints)
        for (int j = 1; j <= m_numPoints; j++)
        {
            // Get Cartesian coordinates from m_A (column j - 1)
            Vector2f cartesianCoord((float)m_A(0, j - 1), (float)m_A(1, j - 1));

            // Map Cartesian coordinate to pixel coordinate
            Vector2f pixelCoord = (Vector2f)target.mapCoordsToPixel(cartesianCoord, m_cartesianPlane);
            // Assign vertex
            lines[j].position = pixelCoord;
            lines[j].color = m_color2; // m_color2 (Random) is the edge color
        }

        // 4. Draw the VertexArray
        target.draw(lines);
    }



