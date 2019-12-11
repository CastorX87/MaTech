#pragma once
#include "stdafx.h"
#include "BaseScenario.h"
#include "Background.h"
#include "BaseButton.h"
#include "RealObject.h"

class MenuScenario :
	public BaseScenario
{
private:
	Font* mFont;

public:
	MenuScenario(const Vector2i& viewportSize);
	virtual ~MenuScenario();
	virtual void HandleEvents(const Event& windowEvent);
};

int OnClick_Quit(const Event& event, void* scenario)
{
	//Transform T = BaseDrawable::GetScreenTransform(Vector2f(bb->GetPos().x, bb->GetPos().y), Vector2f(bb->GetSize().x, bb->GetSize().y), bb->GetAngle(), bb->GetSize() * 0.5f, currScenario->GetCamera());
	//Vector2f TP = ScreenPosToHPos(Vector2i(event.mouseButton.x, event.mouseButton.y), currScenario->GetCamera().ViewportSize, currScenario->GetCamera().ZoomFactor);
	//Transform T_G2L = BaseDrawable::GetGlobalToLocalTransform(bb->GetPos(), bb->GetAngle());
	//TP = T_G2L.transformPoint(TP);

	printf("BUTTON\n");
	
	return 0;
}
int X = 0;
int Y = 0;
int HandleEvents(const Event& event, void* scenario)
{
	MenuScenario* ms = (MenuScenario*)scenario;
	Scene* scene = ms->GetScene();
	RealObject* obj1 = dynamic_cast<RealObject*>(scene->GetObject("X0"));
	RealObject* obj2 = dynamic_cast<RealObject*>(scene->GetObject("GRAB"));

	if (event.type == Event::EventType::KeyPressed)
	{
		Vector2f TP = ScreenPosToHPos(Vector2i(X, Y), ms->GetCamera().ViewportSize, ms->GetCamera().ZoomFactor);
		obj2->SetHPosition(TP);
		printf("%f %f", TP.x, TP.y);
		
		Transform T_G2L = BaseDrawable::GetGlobalToLocalTransform(obj1->GetHPosition(), obj1->GetAngle());
		TP = T_G2L.transformPoint(TP);

		b2DistanceJointDef bj;
		bj.bodyA = obj1->GetBody();
		bj.bodyB = obj2->GetBody();
		bj.collideConnected = false;
		bj.length = 0.02;
		bj.dampingRatio = 0.99;
		bj.frequencyHz = 5;
		bj.localAnchorA = b2Vec2(TP.x, TP.y);
		bj.localAnchorB = b2Vec2(0, 0);
		bj.type = b2JointType::e_distanceJoint;
		bj.userData = nullptr;

		b2MotorJoint* djoint = (b2MotorJoint*)ms->GetWorld()->CreateJoint(&bj);
		bool active = djoint->IsActive();
	}
	if (event.type == Event::EventType::MouseMoved)
	{
		X = event.mouseMove.x;
		Y = event.mouseMove.y;
		Vector2f TP = ScreenPosToHPos(Vector2i(event.mouseMove.x, event.mouseMove.y), ms->GetCamera().ViewportSize, ms->GetCamera().ZoomFactor);
		obj2->SetHPosition(TP);
	}
	return 0;
}

MenuScenario::MenuScenario(const Vector2i& viewportSize) :
	BaseScenario(viewportSize)
{
	mFont = new Font();
	mFont->loadFromFile("Resources\\Fonts\\Amiko-Regular.ttf");
	//mScene->AddObject(new Background("Resources\\Textures\\pngtree_test.png"));
	
	//BaseButton* btnQuit = new BaseButton("Resources\\Textures\\crate.jpg", L"Kilépés", *mFont, 0, 0, 0, Vector2f(0, 0), Vector2f(0.2, 0.2), 0, &mCamera);
	BaseEventHandler* beh = new BaseEventHandler(::HandleEvents, this);
	mScene->AddObject("EventHandler", beh);
	//mScene->AddObject("MouseMoveHandler", new BaseEventHandler(HandleEvents, this));

	BaseButton* btnQuit = new BaseButton("Resources\\Textures\\ButtonBackgrountTest.png", L"Kilépés", *mFont, 12, 0, 0, Vector2f(0, 0), Vector2f(0.2, 0.2), 0, &mCamera);
	btnQuit->RegisterOnClick(OnClick_Quit, this); 
	mScene->AddObject("btnQuit", btnQuit);

	Background* bg = new Background("Resources\\Textures\\ButtonBackgrountTest.png");
	mScene->AddObject("Background", bg);

	// Add ground
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2BodyType::b2_staticBody;
		bodyDef.position.Set(0.0f, -0.3f);


		vector<b2FixtureDef> fictureDefs;

		b2PolygonShape groundBox;
		groundBox.SetAsBox(20.0f, 0.1f);
		b2FixtureDef fd;
		fd.shape = &groundBox;
		fd.density = 0.4f;
		fd.friction = 0.3f;
		
		fictureDefs.push_back(fd);
		
		BasePhysicalBody* physBody = new BasePhysicalBody(this->mWorld, bodyDef, fictureDefs);
		
		mScene->AddObject("ground", physBody);
	}
	{
		b2BodyDef bodyDef;
		
		bodyDef.type = b2BodyType::b2_kinematicBody;
		bodyDef.position.Set(0.0f, 0.4f);
		bodyDef.angle = 0;

		vector<b2FixtureDef> fictureDefs;

		b2PolygonShape groundBox;
		groundBox.SetAsBox(0.2f, 0.1f);
		b2FixtureDef fd;
		fd.shape = &groundBox;
		fd.density = 0.0f;
		fd.friction = 0.3f;
		fd.restitution = 0.3f;
		fictureDefs.push_back(fd);
		fd.isSensor = true;
		RealObject* realObj = new RealObject("Resources\\Textures\\crate.jpg", "XXX", *mFont, 72, 1, 0, Vector2f(0.4, 0.2), &mCamera, this->mWorld, bodyDef, fictureDefs);
		mScene->AddObject("GRAB", realObj);
	}
	// Add object
	for(int i = 0; i < 20; i++)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2BodyType::b2_dynamicBody;
		bodyDef.position.Set(0.0f + i * 0.2, 1.0f + i * 2);
		bodyDef.angle = 10;
		bodyDef.angularDamping = 1;
		bodyDef.linearDamping = 0.99;

		vector<b2FixtureDef> fictureDefs;

		b2PolygonShape groundBox;
		groundBox.SetAsBox(0.25f, 0.25f);
		b2FixtureDef fd;
		
		fd.shape = &groundBox;
		fd.density = 0.2f;
		fd.friction = 0.3f;
		fd.restitution = 0.3f;
		fictureDefs.push_back(fd);

		RealObject* realObj = new RealObject("Resources\\Textures\\crate.jpg", "Hello", *mFont, 12, 1, 0, Vector2f(0.525, 0.525), &mCamera, this->mWorld, bodyDef, fictureDefs);
		mScene->AddObject("X" + to_string(i), realObj);
	}

}

MenuScenario::~MenuScenario()
{
	SafeDelete(mFont);
}

void MenuScenario::HandleEvents(const Event& windowEvent)
{

}