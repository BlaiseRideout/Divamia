#include "demo.hpp"

#include <GLFW/glfw3.h>
#include <time.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


Demo::Demo() : Game(), c(window, glm::vec3(3, 3, 0), glm::vec3(0, 10, 0), 0.005f) {
  initGraphics();
  initPhysics();
}

void Demo::initGraphics() {
  this->window.makeCurrent();
  FragmentShader f("res/screen.frag");
  VertexShader v("res/screen.vert");

  this->p = ShaderProgram(f, v);

  this->p["P"] = glm::perspective(45.0f, (float)this->window.width / (float)this->window.height, 0.1f, 1000.0f);
  this->p["V"] = c.viewMatrix();
  this->p["M"] = glm::scale(glm::translate(glm::mat4(1.0f), boxPosition), glm::vec3(0.001f));

  this->t = Texture("res/cubemap.png");
  this->p["tex"] = t;

  this->m = Model("res/cube.obj", this->p, "vertexPosition", "vertexNormal", "vertexUV");
}

void Demo::initPhysics() {
  this->broadphase = new btDbvtBroadphase();
  
  this->collisionConfiguration = new btDefaultCollisionConfiguration();
  this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);

  this->solver = new btSequentialImpulseConstraintSolver;

  this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration);
  this->dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));

  this->groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
  this->fallShape = new btBoxShape(btVector3(2.0f, 2.0f, 2.0f));


  this->groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

  btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
  this->groundRigidBody = new btRigidBody(groundRigidBodyCI);

  dynamicsWorld->addRigidBody(this->groundRigidBody);


  this->fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 1, 45), btVector3(0, 10, 0)));

  btScalar mass = 1;
  btVector3 fallInertia(1, 0, 0);
  fallShape->calculateLocalInertia(mass, fallInertia);

  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
  this->fallRigidBody = new btRigidBody(fallRigidBodyCI);

  dynamicsWorld->addRigidBody(this->fallRigidBody);
}

Demo::~Demo() {
  delete this->fallRigidBody;
  delete this->groundMotionState;

  delete this->groundRigidBody;
  delete this->groundMotionState;

  delete this->fallShape;
  delete this->groundShape;

  delete this->dynamicsWorld;
  delete this->solver;
  delete this->dispatcher;
  delete this->collisionConfiguration;
  delete this->broadphase;
}

void Demo::update(double time) {
  running = running && this->window.getKey(GLFW_KEY_ESCAPE) == GLFW_RELEASE;
  c.update();
  dynamicsWorld->stepSimulation(1 / 60.0f, 10);

  this->window.clearScreen();

  this->p["V"] = c.viewMatrix();
  
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);
  boxPosition.x = trans.getOrigin().getX();
  boxPosition.y = trans.getOrigin().getY();
  boxPosition.z = trans.getOrigin().getZ();

  btQuaternion q = trans.getRotation();
  btVector3 axis = q.getAxis();

  boxRotation.x = axis.getX();
  boxRotation.y = axis.getY();
  boxRotation.z = axis.getZ();
  boxRotation.w = q.getAngle();


  this->p["M"] = glm::translate(glm::rotate(glm::mat4(1.0f), boxRotation.w, glm::vec3(boxRotation)), boxPosition);
  
  this->p.use();
  this->m.draw();
}
