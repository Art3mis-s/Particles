#include "Engine.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Engine Constructor

Engine::Engine()
{
    // Get the desktop screen resolution
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    // Create the RenderWindow using the desktop resolution in Fullscreen style
    m_Window.create(desktopMode, "Particle System", sf::Style::Fullscreen);

    // Seed the random number generator for particle creation
    std::srand(std::time(0));
}

// Main Game Loop
void Engine::run()
{
    // Clock object to track time differential (dt) per frame
    sf::Clock clock;

    // Run Unit Tests for Particle class
    std::cout << "Starting Particle unit tests..." << std::endl;

    // Construct a local Particle for testing, centered on the screen, with 4 points
    Particle p(m_Window, 4, sf::Vector2i((int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2));
    p.unitTests();

    std::cout << "Unit tests complete. Starting engine..." << std::endl;

    // Game Loop
    while (m_Window.isOpen())
    {
        // Restart the clock and get the elapsed time
        sf::Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();

        // Process input
        input();

        // Update game state
        update(dtAsSeconds);

        // Draw the scene
        draw();
    }
}

// Handle inputs
void Engine::input()
{
    sf::Event event;
    while (m_Window.pollEvent(event))
    {
        // Handle window closed event (clicking the 'X' button)
        if (event.type == sf::Event::Closed)
        {
            m_Window.close();
        }

        // Handle Escape key pressed to exit the program
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            m_Window.close();
        }

        // Handle left mouse button pressed to create particles
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            // Get the current mouse click position (pixel coordinates)
            sf::Vector2i clickPosition = { event.mouseButton.x, event.mouseButton.y };

            // Create 5 new particles at the click position
            for (int i = 0; i < 5; ++i)
            {
                // numPoints parameter: random number in the range [25:50]
                int numPoints = std::rand() % 26 + 25;

                // Construct Particle and add it to the m_particles vector
                m_particles.emplace_back(m_Window, numPoints, clickPosition);
            }
        }
    }
}

// Update game state
void Engine::update(float dtAsSeconds)
{
    // Use an iterator-based for-loop to safely remove elements while iterating

    for (auto it = m_particles.begin(); it != m_particles.end(); )
    {
        // Check if the particle's Time To Live (TTL) has not expired
        if (it->getTTL() > 0.0f)
        {
            // Particle is alive: call update, passing the time differential
            it->update(dtAsSeconds);

            // Increment the iterator to move to the next element
            ++it;
        }
        else
        {
            // Particle is expired: erase it from the vector
            // erase() returns an iterator to the element immediately following the deleted element
            it = m_particles.erase(it);

            // DO NOT increment the iterator here; the return value from erase() already points to the next element
        }
    }
}

// Draw scene
void Engine::draw()
{
    // Clear the window to prepare for the next frame
    m_Window.clear(sf::Color::Black);

    // Loop through each Particle in the vector
    for (const auto& particle : m_particles)
    {
        // Draw each element. This uses polymorphism to call Particle::draw()
        m_Window.draw(particle);
    }

    // Display the newly rendered frame to the screen
    m_Window.display();
}