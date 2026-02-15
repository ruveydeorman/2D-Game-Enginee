#include"PhysicsSystem.h"
#include<iostream>


bool PhysicsSystem::Init(float ppm, float gX, float gY) {
	m_PPM = ppm;
	m_World = new b2World({ gX,gY });
	return true;
}
void PhysicsSystem::Shutdown() {
	delete m_World;
	m_World = nullptr;
}
void PhysicsSystem::Step(float dt) {
	if (!m_World)return;
	m_World->Step(dt, 8, 3);
}
b2Body* PhysicsSystem::CreateBox(float px, float py, float w, float h, bool dynamic, float density, float rest) {
	//temel özellikleri
	b2BodyDef bd;
	bd.type = dynamic ? b2_dynamicBody : b2_staticBody;//dinamic olup olmadýðýna göre cismin türünü belirler
	bd.position.Set(px / m_PPM, py / m_PPM); //cismin baþlayacað yerin koordinatlarýný alýr pixel türüne göre çevirme yapýlarak.
	//þekil öxellikleri
	b2PolygonShape shape;
	shape.SetAsBox((w / 2) / m_PPM, (h / 2) / m_PPM);//cismin þekil iþlemleri için geniþlik v eyüksekliði piksel cinsinden yarýsýný alarark iþlem yapýlýr
	//fiziksel özellikleri
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = density;
	fd.restitution = rest; //cismin momentumunu alýr yani çarpýþma durumunda hangi olçekte tepki vermesini ayarlamak için 


	b2Body* body = m_World->CreateBody(&bd);
	body->CreateFixture(&fd);
	return body;
}

b2Body* PhysicsSystem::CreateCircle(float px, float py, float radius, bool dynamic, float density, float rest) {
	b2BodyDef bd;
	bd.type = dynamic ? b2_dynamicBody : b2_staticBody;
	bd.position.Set(px / m_PPM, py / m_PPM);


	b2CircleShape shape;
	shape.m_radius = radius / m_PPM;

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = density;
	fd.restitution = rest;

	b2Body* body = m_World->CreateBody(&bd);
	body->CreateFixture(&fd);
	return body;
}
