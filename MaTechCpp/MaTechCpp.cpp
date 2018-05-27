// MaTechCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MaTechApp.h"

int main()
{
	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);
	
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	{
		b2PolygonShape groundBox;
		groundBox.SetAsBox(50.0f, 10.0f);
		groundBody->CreateFixture(&groundBox, 0.0f);
	}
	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	{
		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f, 1.0f);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);
	}
	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for (int32 i = 0; i < 6000; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}

	auto app = unique_ptr<MaTechApp>(new MaTechApp("Hello", 640, 480));
	app->Run();
	app.reset();

	return 0;
}

