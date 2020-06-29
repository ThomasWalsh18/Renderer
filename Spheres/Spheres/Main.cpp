#define _CRT_SECURE_NO_WARNINGS
#include "Sphere.h"
#include "objloader.h"
#include "Triangle.h"
#include "Plane.h"
#include "Shape.h"
#include <glm.hpp>
#include <fstream>
#include "Sphere.h"
#include <vector>
#include "AreaLight.h"
#include "light.h"
#include <chrono>
#include <ctime>
#include <SDL.h>
#include <SDL_main.h>
#include "AxisAlignedBounding.h"
#include <thread>
#include <mutex>


using namespace std;
using namespace glm;


const int WIDTH = 640;
const int HEIGHT = 480;
vec3 image[WIDTH][HEIGHT];
int primatives = 0;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool loadModel = false;

AreaLight* light1 = new AreaLight(vec3(0, 20, 0), vec3(0.5f, 0.5f, 0.5f), vec3(1, 1, 1), 5, 9);
light* light2 = new light(vec3(0, 20, 0), vec3(0.5f, 0.5f, 0.5f), vec3(1, 1, 1));
bool isSoft = true;

//vec3 RayOrigin(0, 0, 0);

vector<AxisAlignedBounding*> Meshes;
float FOV = radians(90.0);
float IAR = WIDTH / HEIGHT;
bool isMultiThread = false;
mutex m;

void SDLStart()
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("322", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (window == NULL)
	{
		printf("could not create window: %s\n", SDL_GetError());
	}
}
vec3 PCalcSphere(Shape* object, int MinOrMax) {
	float radi = object->getRadi();
	if (MinOrMax == 0) {
		//min
		return vec3(object->pos.x -radi, object->pos.y - radi, object->pos.z - radi);
	}
	else
	{
		//max
		return vec3(object->pos.x + radi, object->pos.y + radi, object->pos.z + radi);
	}

}
void addObjects(vector<AxisAlignedBounding*>& Meshes) {
	vector <Shape*> temp;
	
	Shape* Red = new Sphere(vec3(0, 0, -20), 4, vec3(1.0f, 0.0f, 0.0f), 128);
	Red->isReflective = true;
	temp.push_back(Red);
	AxisAlignedBounding* RedBox = new AxisAlignedBounding(PCalcSphere(Red, 0), PCalcSphere(Red, 1), temp);
	Meshes.push_back(RedBox);
	primatives++;
	temp.clear();
	
	Shape* Yellow = new Sphere(vec3(5, -1, -15), 2, vec3(0.9, 0.76, 0.46), 128);
	//	Yellow->isReflective = true;
	temp.push_back(Yellow);
	AxisAlignedBounding* YellowBox = new AxisAlignedBounding(PCalcSphere(Yellow, 0), PCalcSphere(Yellow, 1), temp);
	Meshes.push_back(YellowBox);
	primatives++;
	temp.clear();
	
	Shape* lightBlue = new Sphere(vec3(5, 0, -25), 3, vec3(0.65, 0.77, 0.97), 128);
	temp.push_back(lightBlue);
	AxisAlignedBounding* LightBlueBox = new AxisAlignedBounding(PCalcSphere(lightBlue, 0), PCalcSphere(lightBlue, 1), temp);
	Meshes.push_back(LightBlueBox);
	primatives++;
	temp.clear();

	Shape* grey = new Sphere(vec3(-5.5, 0, -15), 3, vec3(0.0, 0.0, 1.0), 128);
	temp.push_back(grey);
	AxisAlignedBounding* GreyBox = new AxisAlignedBounding(PCalcSphere(grey, 0), PCalcSphere(grey, 1), temp);
	Meshes.push_back(GreyBox);
	primatives++;
	temp.clear();
	
	Shape* floor = new Plane(vec3(-10, -5, -10), vec3(0, 1, 0), vec3(0.2, 0.2, 0.2), 128);
	floor->isReflective = true;
	temp.push_back(floor);
	AxisAlignedBounding* floorBox = new AxisAlignedBounding(vec3(-INFINITY, -5.0f, -INFINITY), vec3(INFINITY,4.0f,INFINITY), temp);
	Meshes.push_back(floorBox);
	primatives++;
	temp.clear();
	
	
	Triangle* tri = new Triangle(vec3(0, 3, -3), vec3(-1.9, 1, -3), vec3(1.6, 1.5, -3),
	vec3(0.0, 0.6, 1.0), vec3(-0.4, -0.4, 1.0),	vec3(0.4, -0.4, 1.0),
	vec3(0.0f,1.0f, 0.0f), 
	128
	);
	temp.push_back(tri);
	vec3 Pmin = vec3(0.0f);
	vec3 Pmax = vec3(0.0f);
	for (int i = 0; i < 4; i++) {
		Pmax = vec3(max(tri->a.x, Pmax.x), max(tri->a.y, Pmax.y), max(tri->a.z, Pmax.z));
		Pmin = vec3(min(tri->a.x, Pmin.x), min(tri->a.y, Pmin.y), min(tri->a.z, Pmin.z));
	}
	AxisAlignedBounding* triBox = new AxisAlignedBounding(Pmin,Pmax,temp);
	Meshes.push_back(triBox);
	primatives++;
	temp.clear();
	
}
void addModel(vector<AxisAlignedBounding*>& Meshes, vec3 offset) {

	vector<vec3> vertices;
	vector<vec3> normals;
	vector<Shape*> model;
	vec3 minimum = vec3(INFINITY);
	vec3 maximum = vec3(-INFINITY);
	if (offset.y == 6) { // quick add I could use a string to swap what model is loaded
		bool res = loadOBJ("teapot_smooth.obj", vertices, normals);
	}
	else {
		bool res = loadOBJ("teapot_simple.obj", vertices, normals);

	}
	for (int i = 0; i < vertices.size(); i += 3) {
		Shape* temp = new Triangle(
			vec3(vertices[i].x + offset.x, vertices[i].y + offset.y, vertices[i].z + offset.z),
			vec3(vertices[i + 1].x + offset.x, vertices[i + 1].y + offset.y, vertices[i + 1].z + offset.z),
			vec3(vertices[i + 2].x + offset.x, vertices[i + 2].y + offset.y, vertices[i + 2].z + offset.z),
			vec3(normals[i].x, normals[i].y, normals[i].z),
			vec3(normals[i + 1].x, normals[i + 1].y, normals[i + 1].z),
			vec3(normals[i + 2].x, normals[i + 2].y, normals[i + 2].z),
			vec3(0.1, 1, 0.5),
			128
		);
		model.push_back(temp);
	}
	
	for (int j = 0; j < vertices.size(); j++) {
		maximum = vec3(max(vertices[j].x, maximum.x), max(vertices[j].y, maximum.y), max(vertices[j].z, maximum.z)) + offset;
		minimum = vec3(min(vertices[j].x, minimum.x), min(vertices[j].y, minimum.y), min(vertices[j].z, minimum.z)) + offset;
	
	}
	/*
	for (int j = 0; j < vertices.size(); j++) {
		if (vertices[j].x > maximum.x && vertices[j].y > maximum.y && (vertices[j].z - 10) > maximum.z) {
			maximum = vec3(vertices[j].x, vertices[j].y, vertices[j].z - 10);
		}
		else if (vertices[j].x < minimum.x && vertices[j].y < minimum.y && (vertices[j].z - 10) < minimum.z) {
			minimum = vec3(vertices[j].x, vertices[j].y, vertices[j].z - 10);
		}
	}*/
	//std::cout << maximum.x << " " << maximum.y << " " << maximum.z << std::endl;
	//std::cout << minimum.x << " " << minimum.y << " " << minimum.z << std::endl;
	AxisAlignedBounding* Model = new AxisAlignedBounding(minimum, maximum, model);
	Meshes.push_back(Model);
	loadModel = true;
}

vec3 RayDir(vec3 RayOrigin, float IAR, float FOV, int i, int j) {
	float pixelNormalizedx = (i + 0.5f) / WIDTH;
	float pixelNormalizedy = (j + 0.5f) / HEIGHT;
	float pixelRemappedx = (2 * pixelNormalizedx - 1) * IAR;
	float pixelRemappedy = 1 - 2 * pixelNormalizedy;
	float pixelCamerax = pixelRemappedx * tan(FOV * 0.5);
	float pixelCameray = pixelRemappedy * tan(FOV * 0.5);
	vec3 PcameraSpace(pixelCamerax, pixelCameray, -1);
	vec3 RayDirection = normalize(PcameraSpace - RayOrigin);
	return RayDirection;
}

void CalcRayTracer(int start, int remaining) {
	vec3 RayOrigin(0.0f);
	//lock_guard<mutex> lk(m);
	for (int i = start; i < remaining; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			vec3 RayDirection = RayDir(RayOrigin, IAR, FOV, i, j);

			HitInfo info;
			int objectIndex = -1;
			int MeshHit = -1;
			float closestValue = 999999.0f;
			for (int k = 0; k < Meshes.size(); k++)
			{
				if (Meshes[k]->intersection(RayOrigin, RayDirection))
				{
					for (int q = 0; q < Meshes[k]->objects.size(); q++)
					{
						
						if (Meshes[k]->objects[q]->intersection(RayOrigin, RayDirection, info))
						{
							
							if (info.hit == true && info.t < closestValue)
							{
								closestValue = info.t;
								objectIndex = q;
								MeshHit = k;
							}
						}
					}
				}
			}
			
			vec3 colour = vec3(0.0f);
			if (objectIndex != -1 && MeshHit != -1)
			{
				if (isSoft) {
					colour = light1->lightCalc(RayOrigin, Meshes, RayDirection, objectIndex, MeshHit, 0);
				}
				else {
					colour = light2->lightCalc(RayOrigin ,Meshes, RayDirection, objectIndex, MeshHit, 0);
				}

			}
			m.lock();
			image[i][j] = colour;
			m.unlock();
		}
	}
}
void render(SDL_Surface* surface) {
	auto start = std::chrono::system_clock::now();
	

	if (isMultiThread) {
		CalcRayTracer(0, 80);
		thread thread1(CalcRayTracer, 80, 160);
		thread thread2(CalcRayTracer, 160, 240);
		thread thread3(CalcRayTracer, 240, 320);
		thread thread4(CalcRayTracer, 320, 400);
		thread thread5(CalcRayTracer, 400, 480);
		thread thread6(CalcRayTracer, 480, 560);
		thread thread7(CalcRayTracer, 560, 640);
		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();
		thread5.join();
		thread6.join();
		thread7.join();
	}
	else {
		CalcRayTracer(0, 640);
	}

	SDL_LockSurface(surface);

	Uint32* pixels = (Uint32*)surface->pixels;

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			vec3 pixel = image[x][y] * 255.0f;

			pixels[(y * surface->w) + x] = SDL_MapRGB(surface->format,
				(Uint8)min(pixel.x, 255.0f),
				(Uint8)min(pixel.y, 255.0f),
				(Uint8)min(pixel.z, 255.0f));
		}
	}
	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
	auto end = std::chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "Time taken: " << elapsed_seconds.count() << endl;
}

void updateSamplePoints(AreaLight*& light1, vec3 change) {
	for (int i = 0; i < light1->SamplePoints.size(); i++) {
		light1->SamplePoints[i]= light1->SamplePoints[i] - change;
	}
}

int main(int argc, char* argv[])
{
	SDLStart();
	bool quit = true;
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	addObjects(Meshes);	
	if (loadModel) {
		//addModel(Meshes, vec3(0.0f, 0.0f, -10.0f));
		//addModel(Meshes, vec3(0.0f, 6.0f, -10.0f));
	}
	render(surface);
	
	while (quit)
	{
		SDL_PollEvent(&event);
		
		if (event.type == SDL_QUIT)
		{
			for (int i = 0; i < Meshes.size(); i++) {
				delete Meshes[i];
			}
			quit = false;
			light1->~AreaLight();
			light2->~light();
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
			SDL_Quit();

		}
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				light1->pos = light1->pos - vec3(0, -1, 0);
				light2->pos = light1->pos - vec3(0, -1, 0);
				updateSamplePoints(light1, vec3(0, -1, 0));
				render(SDL_GetWindowSurface(window));
				break;

			case SDLK_DOWN:
				light1->pos = light1->pos - vec3(0, 1, 0);
				light2->pos = light1->pos - vec3(0, 1, 0);
				updateSamplePoints(light1, vec3(0, 1, 0));
				render(SDL_GetWindowSurface(window));
				break;

			case SDLK_LEFT:
				light1->pos = light1->pos + vec3(1, 0, 0);
				light2->pos = light1->pos + vec3(1, 0, 0);
				updateSamplePoints(light1, vec3(-1, 0, 0));
				render(SDL_GetWindowSurface(window));
				break;

			case SDLK_RIGHT:
				light1->pos = light1->pos - vec3(1, 0, 0);
				light2->pos = light1->pos - vec3(1, 0, 0);
				updateSamplePoints(light1, vec3(1, 0, 0));
				render(SDL_GetWindowSurface(window));
				break;
			case SDLK_w:
				FOV = FOV + radians(1.0f);
				render(SDL_GetWindowSurface(window));
				break;
			case SDLK_s:
				FOV = FOV - radians(1.0f);	
				render(SDL_GetWindowSurface(window));
				break;
			case SDLK_h:
				isSoft = !isSoft;
				render(SDL_GetWindowSurface(window));
				break;
			case SDLK_0:
				if (Meshes.size() >= 0)
				{
					Meshes[0]->objects[0]->isReflective = !Meshes[0]->objects[0]->isReflective;
				}
				render(SDL_GetWindowSurface(window));
				break;
			case SDLK_1:
				if (Meshes.size() > 1)
				{
					Meshes[1]->objects[0]->isReflective = !Meshes[1]->objects[0]->isReflective;
				}
				render(SDL_GetWindowSurface(window));
				break;
			case SDLK_2:
				if (Meshes.size() > 2)
				{
					Meshes[2]->objects[0]->isReflective = !Meshes[2]->objects[0]->isReflective;
				}
				render(SDL_GetWindowSurface(window));
				break;
			case SDLK_3:
				if (Meshes.size() > 3)
				{
					Meshes[3]->objects[0]->isReflective = !Meshes[3]->objects[0]->isReflective;
				}
				render(SDL_GetWindowSurface(window));
				break;	
			case SDLK_4:
				if (Meshes.size() > 4)
				{
					Meshes[4]->objects[0]->isReflective = !Meshes[4]->objects[0]->isReflective;
				}
				render(SDL_GetWindowSurface(window));
				break;	
			case SDLK_5:
				if (Meshes.size() > 5)
				{
					Meshes[5]->objects[0]->isReflective = !Meshes[5]->objects[0]->isReflective;
				}
				render(SDL_GetWindowSurface(window));
				break;	
			case SDLK_t:
				isMultiThread = !isMultiThread;
				render(SDL_GetWindowSurface(window));
				break;
			case SDLK_m:
				if (loadModel == true) {
					loadModel = false;
					std::cout << "Deleting Model" << endl;
					for (int i = 0; i < Meshes[primatives]->objects.size(); i++) {
						Meshes[primatives]->objects[i]->~Shape();
					}
					for (int i = 0; i < Meshes[primatives+1]->objects.size(); i++) {
						Meshes[primatives+1]->objects[i]->~Shape();
					}
					Meshes[primatives]->~AxisAlignedBounding();
				}
				else {
					loadModel = true;
					addModel(Meshes, vec3(0.0f, 0.0f, -10.0f));
					addModel(Meshes, vec3(0.0f, 6.0f, -10.0f));
				}
				render(SDL_GetWindowSurface(window));
				break;

			default:
				break;
			}
		}
	}
		return 0;
	}

