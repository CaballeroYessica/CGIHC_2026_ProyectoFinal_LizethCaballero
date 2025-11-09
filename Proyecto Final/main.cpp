/*
Proyeto final de Laboratorio de Computación Gráfica


Nombre: Caballero Garcia Yessica Lizeth


Fecha: 8 de noviembre de 2025
*/

#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;


glm::vec3 posLampara = glm::vec3(0.0f);

//VARIABLES PARA ANIMACIÓN



// Variables para animación del globo 
float angulovaria = 0.0f;
float movGlobo = 0.0f;
float movGloboOffset = 0.5f;
bool animacionGloboActiva = false;
float posXGlobo = 100.0f;
float posYGlobo = -1.0f;
float posZGlobo = -250.0f;
float rotGlobo = 0.0f;
float t = 0.0f;
float disminuir = 0.0f;


//Variables para animación de caminata James
float movPiernasJames = 0.0f;
float movBrazosJames = 0.0f;
float velocidadCaminata = 5.0f;
bool caminandoJames = false;
float anguloRodilla;
float moveSpeed;
float dirRadians;
float rotationSpeed;
float anguloCodo;


//Animación Eevee y Bulbasaur
float movPiernasEevee = 0.0f;
float movPiernasBulbasaur = 0.0f;
float velocidadAnimacionPokemon = 5.0;
float tiempoPokemon = 0.0f;
float radioCirculo = 8.0f;
float centroX = -90.0f;
float centroZ = 0.0f;
float velocidadAngular = 0.005f;
float anguloEevee;
float anguloBulbasaur;



//Animacion profesor oak
float oakSaludoTime = 0.0f;
bool oakAnimacionActiva = true;
float anguloSaludo = 0.0f;
float anguloCabeza = 0.0f;
float anguloCodoOak = 0.0f;


//Animacion entrenador pokemon
float movPiernasEntre = 0.0f;
float movBrazosEntre = 0.0f;
float velocidadCaminataEntre = 4.0f;
bool caminandoEntre = true;
float anguloRodillaEntre;
float anguloCodoEntre;
float posXEntre = -30.0f;
float posZEntre = -180.0f;
float direccionEntre = 90.0f;
float amplitudMovimiento = 10.0f;
float velocidadMovimiento = 0.02f;
float tiempoEntre = 0.0f;


//Animación puerta rejas
float rotRejaDer = 0.0f;
float desplazaRejaIzq = 0.0f;
bool animacionRejasActiva = false;
bool cicloCompletado = true;
float tiempoAbierto = 0.0f;
int faseAnimacion = 0;
float velocidadRotacion = 10.0f;
float velocidadDesplazamiento = 0.8f;
float tiempoPausa = 20.0f;


//Variables ciclo dia y noche
float cicloDiaNoche = 0.0f;
float velocidadCiclo = 0.1f; 
bool esDeNoche = false;
bool skyboxCambiado = false;
bool nuevoEstadoNoche;
std::vector<std::string> skyboxFacesDia;
std::vector<std::string> skyboxFacesNoche;



Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture pisoTexture;


Model pLuna_M;
Model pSol_M;
Model centroPoke_M;
Model globo_M;
Model ring_M;
Model mesa_M;
Model piedraTrueno_M;
Model lamPoke_M;
Model bandera_M;
Model anuncio_M;
Model luzSuelo_M;
Model puerta_M;
Model letrero_M;
Model rejaIzq_M;
Model rejaDer_M;
Model Bulbasaur_M;
Model derDelanBul_M;
Model derTrasBul_M;
Model izqDelanBul_M;
Model izqTrasBul_M;
Model bDerJames_M;
Model bIzqJames_M;
Model cabezaJames_M;
Model cuerpoJames_M;
Model mDerJames_M;
Model mIzqJames_M;
Model pDerJames_M;
Model pIzqJames_M;
Model zDerJames_M;
Model zIzqJames_M;
Model bDerOak_M;
Model bIzqOak_M;
Model cabezaOak_M;
Model cuerpoOak_M;
Model mDerOak_M;
Model mIzqOak_M;
Model pDerOak_M;
Model pIzqOak_M;
Model zDerOak_M;
Model zIzqOak_M;
Model Eevee_M;
Model cabezaEevee_M;
Model colaEevee_M;
Model izqDelanEevee_M;
Model izqTrasEevee_M;
Model derDelanEevee_M;
Model derTrasEevee_M;
Model cuerpoEntre_M;
Model bDerEntre_M;
Model bIzqEntre_M;
Model pDerEntre_M;
Model pIzqEntre_M;
Model zDerEntre_M;
Model zIzqEntre_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}







int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	pLuna_M = Model();
	pLuna_M.LoadModel("Models/pLuna.obj");
	pSol_M = Model();
	pSol_M.LoadModel("Models/pSol.obj");
	globo_M = Model();
	globo_M.LoadModel("Models/globo2.obj");
	ring_M = Model();
	ring_M.LoadModel("Models/ring.obj");
	mesa_M = Model();
	mesa_M.LoadModel("Models/mesaPicnic.obj");
	piedraTrueno_M = Model();
	piedraTrueno_M.LoadModel("Models/piedraTrueno.obj");

	centroPoke_M = Model();
	centroPoke_M.LoadModel("Models/centroPoke.obj");
	puerta_M = Model();
	puerta_M.LoadModel("Models/puerta.obj");
	rejaDer_M = Model();
	rejaDer_M.LoadModel("Models/rejaDer.obj");
	rejaIzq_M = Model();
	rejaIzq_M.LoadModel("Models/rejaIzq.obj");
	letrero_M = Model();
	letrero_M.LoadModel("Models/letrero.obj");
	Bulbasaur_M = Model();
	Bulbasaur_M.LoadModel("Models/Bulbasaur.obj");
	derDelanBul_M = Model();
	derDelanBul_M.LoadModel("Models/derDelanBul.obj");
	derTrasBul_M = Model();
	derTrasBul_M.LoadModel("Models/derTrasBul.obj");
	izqDelanBul_M = Model();
	izqDelanBul_M.LoadModel("Models/izqDelanBul.obj");
	izqTrasBul_M = Model();
	izqTrasBul_M.LoadModel("Models/izqTrasBul.obj");
	bDerJames_M = Model();
	bDerJames_M.LoadModel("Models/bDerJames.obj");
	bIzqJames_M = Model();
	bIzqJames_M.LoadModel("Models/bIzqJames.obj");
	cabezaJames_M = Model();
	cabezaJames_M.LoadModel("Models/cabezaJames.obj");
	cuerpoJames_M = Model();
	cuerpoJames_M.LoadModel("Models/cuerpoJames.obj");
	mDerJames_M = Model();
	mDerJames_M.LoadModel("Models/mDerJames.obj");
	mIzqJames_M = Model();
	mIzqJames_M.LoadModel("Models/mIzqJames.obj");
	pDerJames_M = Model();
	pDerJames_M.LoadModel("Models/pDerJames.obj");
	pIzqJames_M = Model();
	pIzqJames_M.LoadModel("Models/pIzqJames.obj");
	zDerJames_M = Model();
	zDerJames_M.LoadModel("Models/zDerJames.obj");
	zIzqJames_M = Model();
	zIzqJames_M.LoadModel("Models/zIzqJames.obj");
	bDerOak_M = Model();
	bDerOak_M.LoadModel("Models/bDer2Oak.obj");
	bIzqOak_M = Model();
	bIzqOak_M.LoadModel("Models/bIzq2Oak.obj");
	cabezaOak_M = Model();
	cabezaOak_M.LoadModel("Models/cabeza2Oak.obj");
	cuerpoOak_M = Model();
	cuerpoOak_M.LoadModel("Models/cuerpo2Oak.obj");
	mDerOak_M = Model();
	mDerOak_M.LoadModel("Models/mDerOak.obj");
	mIzqOak_M = Model();
	mIzqOak_M.LoadModel("Models/mIzq2Oak.obj");
	pDerOak_M = Model();
	pDerOak_M.LoadModel("Models/pDerOak.obj");
	pIzqOak_M = Model();
	pIzqOak_M.LoadModel("Models/pIzqOak.obj");
	zDerOak_M = Model();
	zDerOak_M.LoadModel("Models/zDerOak.obj");
	zIzqOak_M = Model();
	zIzqOak_M.LoadModel("Models/zIzqOak.obj");
	Eevee_M = Model();
	Eevee_M.LoadModel("Models/Eevee.obj");
	cabezaEevee_M = Model();
	cabezaEevee_M.LoadModel("Models/cabezaEevee.obj");
	colaEevee_M = Model();
	colaEevee_M.LoadModel("Models/colaEevee.obj");
	izqDelanEevee_M = Model();
	izqDelanEevee_M.LoadModel("Models/izqDelanEevee.obj");
	izqTrasEevee_M = Model();
	izqTrasEevee_M.LoadModel("Models/izqTrasEevee.obj");
	derDelanEevee_M = Model();
	derDelanEevee_M.LoadModel("Models/derDelanEevee.obj");
	derTrasEevee_M = Model();
	derTrasEevee_M.LoadModel("Models/derTrasEevee.obj");
	cuerpoEntre_M = Model();
	cuerpoEntre_M.LoadModel("Models/cuerpoEntre.obj");
	bDerEntre_M = Model();
	bDerEntre_M.LoadModel("Models/bDerEntre.obj");
	bIzqEntre_M = Model();
	bIzqEntre_M.LoadModel("Models/bIzqEntre.obj");
	pDerEntre_M = Model();
	pDerEntre_M.LoadModel("Models/pDerEntre.obj");
	pIzqEntre_M = Model();
	pIzqEntre_M.LoadModel("Models/pIzqEntre.obj");
	zDerEntre_M = Model();
	zDerEntre_M.LoadModel("Models/zDerEntre.obj");
	zIzqEntre_M = Model();
	zIzqEntre_M.LoadModel("Models/zIzqEntre.obj");
	lamPoke_M = Model();
	lamPoke_M.LoadModel("Models/lamPoke.obj");
	bandera_M = Model();
	bandera_M.LoadModel("Models/bandera.obj");
	anuncio_M = Model();
	anuncio_M.LoadModel("Models/anuncio.obj");
	luzSuelo_M = Model();
	luzSuelo_M.LoadModel("Models/luzSuelo.obj");



	// TEXTURAS SKYBOX DIA
	skyboxFacesDia.push_back("Textures/Skybox/miCielo_rt.png");
	skyboxFacesDia.push_back("Textures/Skybox/miCielo_lf.png");
	skyboxFacesDia.push_back("Textures/Skybox/miCielo_up.png");
	skyboxFacesDia.push_back("Textures/Skybox/miCielo_dn.png");
	skyboxFacesDia.push_back("Textures/Skybox/miCielo_bk.png");
	skyboxFacesDia.push_back("Textures/Skybox/miCielo_ft.png");

	// TEXTURAS SKYBOX NOCHE
	skyboxFacesNoche.push_back("Textures/Skybox/miCielo_night_rt.png");
	skyboxFacesNoche.push_back("Textures/Skybox/miCielo_night_lf.png");
	skyboxFacesNoche.push_back("Textures/Skybox/miCielo_night_up.png");
	skyboxFacesNoche.push_back("Textures/Skybox/miCielo_night_dn.png");
	skyboxFacesNoche.push_back("Textures/Skybox/miCielo_night_bk.png");
	skyboxFacesNoche.push_back("Textures/Skybox/miCielo_night_ft.png");

	// Inicializar con skybox de día
	skybox = Skybox(skyboxFacesDia);





	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);






	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,                                 // Intensidades
		0.0f, 0.0f, 0.0f,                       // Posición
		0.1f, 0.1f, 0.1f);                         // Atenuación
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,                                 // Intensidades
		0.0f, 0.0f, 0.0f,                       // Posición
		0.1f, 0.1f, 0.1f);                         // Atenuación
	pointLightCount++;

	pointLights[2] = PointLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,                                 // Intensidades
		0.0f, 0.0f, 0.0f,                       // Posición
		0.1f, 0.1f, 1.1f);                         // Atenuación
	pointLightCount++;

	pointLights[3] = PointLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,                                 // Intensidades
		0.0f, 0.0f, 0.0f,                       // Posición
		0.1f, 0.1f, 1.1f);                         // Atenuación
	pointLightCount++;










	unsigned int spotLightCount = 0;
	//Portico centro pokémon
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, 
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//Letreros lucha libre
	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	//Camara seguimiento
	glm::vec3 posicionJames = glm::vec3(0.0f, 4.20f, 0.0f);
	glm::vec3 frenteJames = glm::vec3(0.0f, 0.0f, -1.0f);


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		//Animación del globo aerostático

		angulovaria += 5.0f * deltaTime;

		if (mainWindow.inicioGlobo() && !animacionGloboActiva)
		{
			animacionGloboActiva = true;
			movGlobo = 0.0f;
			mainWindow.resetGlobo();
		}

		if (animacionGloboActiva)
		{
			movGlobo += movGloboOffset * deltaTime;

			if (movGlobo <= 100.0f)
			{
				posXGlobo = 100.0f + movGlobo * 0.5f;
				posZGlobo = -250.0f + movGlobo * 4.0f;
				posYGlobo = -1.0f + movGlobo * 1.2f;
				rotGlobo = movGlobo * 1.2f;

				posYGlobo += 8.0f * sin(angulovaria * 0.03f);
			}
			else if (movGlobo <= 200.0f)
			{
				t = movGlobo - 100.0f;

				posXGlobo = 150.0f - t * 0.5f;
				posZGlobo = 150.0f - t * 4.0f;
				posYGlobo = 119.0f - t * 1.2f;
				rotGlobo = 120.0f + t * (-1.2f);

				//Reducir la oscilación al final
				if (t > 90.0f) {
					disminuir = (100.0f - t) / 10.0f;
					posYGlobo += 8.0f * sin(angulovaria * 0.03f) * disminuir;
				}
				else {
					posYGlobo += 8.0f * sin(angulovaria * 0.03f);
				}
			}
			else
			{
				posXGlobo = 100.0f;
				posYGlobo = -1.0f;
				posZGlobo = -250.0f;
				rotGlobo = 0.0f;
				animacionGloboActiva = false;
			}
		}


		//Animacion de James
		// ROTACIÓN
		rotationSpeed = 5.0f;

		if (mainWindow.getRotateLeft())
		{
			mainWindow.setDireccionJames(mainWindow.getdireccionJames() + rotationSpeed * deltaTime);
		}

		if (mainWindow.getRotateRight())
		{
			mainWindow.setDireccionJames(mainWindow.getdireccionJames() - rotationSpeed * deltaTime);
		}

		// TRASLACIÓN
		moveSpeed = mainWindow.getMoveSpeed();
		dirRadians = mainWindow.getdireccionJames() * toRadians;

		glm::vec3 jamesFront = glm::vec3(sin(dirRadians), 0.0f, cos(dirRadians));
		glm::vec3 jamesRight = glm::vec3(cos(dirRadians), 0.0f, -sin(dirRadians));

		if (mainWindow.getMoveForward())
		{
			mainWindow.setPosXJames(mainWindow.getposXJames() - jamesFront.x * moveSpeed * deltaTime);
			mainWindow.setPosZJames(mainWindow.getposZJames() - jamesFront.z * moveSpeed * deltaTime);
		}

		if (mainWindow.getMoveBackward())
		{
			mainWindow.setPosXJames(mainWindow.getposXJames() + jamesFront.x * moveSpeed * deltaTime);
			mainWindow.setPosZJames(mainWindow.getposZJames() + jamesFront.z * moveSpeed * deltaTime);
		}

		if (mainWindow.getMoveLeft())
		{
			mainWindow.setPosXJames(mainWindow.getposXJames() - jamesRight.x * moveSpeed * deltaTime);
			mainWindow.setPosZJames(mainWindow.getposZJames() - jamesRight.z * moveSpeed * deltaTime);
		}

		if (mainWindow.getMoveRight())
		{
			mainWindow.setPosXJames(mainWindow.getposXJames() + jamesRight.x * moveSpeed * deltaTime);
			mainWindow.setPosZJames(mainWindow.getposZJames() + jamesRight.z * moveSpeed * deltaTime);
		}

		caminandoJames = mainWindow.getcaminandoJames();

		if (caminandoJames)
		{
			movPiernasJames += velocidadCaminata * deltaTime;
			movBrazosJames += velocidadCaminata * deltaTime;

			if (movPiernasJames > 360.0f) movPiernasJames -= 360.0f;
			if (movBrazosJames > 360.0f) movBrazosJames -= 360.0f;
		}
		else
		{
			movPiernasJames *= 0.9f;
			movBrazosJames *= 0.9f;
		}



		//Animación Eevee y Bulbasaur
		tiempoPokemon += deltaTime;
		anguloEevee = tiempoPokemon * velocidadAngular;
		anguloBulbasaur = anguloEevee + 3.14159f;

		mainWindow.setPosXEevee(centroX + cos(anguloEevee) * radioCirculo);
		mainWindow.setPosZEevee(centroZ + sin(anguloEevee) * radioCirculo);
		mainWindow.setDireccionEevee((-anguloEevee * 180.0f / 3.14159f) + 180.0f);

		mainWindow.setPosXBulbasaur(centroX + cos(anguloBulbasaur) * radioCirculo);
		mainWindow.setPosZBulbasaur(centroZ + sin(anguloBulbasaur) * radioCirculo);
		mainWindow.setDireccionBulbasaur((-anguloBulbasaur * 180.0f / 3.14159f) + 180.0f);

		// Animación de patas
		movPiernasEevee += velocidadAnimacionPokemon * deltaTime;
		movPiernasBulbasaur += velocidadAnimacionPokemon * deltaTime;

		if (movPiernasEevee > 360.0f) movPiernasEevee -= 360.0f;
		if (movPiernasBulbasaur > 360.0f) movPiernasBulbasaur -= 360.0f;




		//Animación Profesor Oak
		if (oakAnimacionActiva) {
			oakSaludoTime += deltaTime;
			anguloCodoOak = (sin(oakSaludoTime * 0.1f) + 1.0f) * 0.5f * 90.0f;
			anguloSaludo = sin(oakSaludoTime * 0.1f) * 90.0f;
			anguloCabeza = sin(oakSaludoTime * 0.1f) * 15.0f;
		}


		//Animación entrenador pokemon
		if (caminandoEntre)
		{
			movPiernasEntre += velocidadCaminataEntre * deltaTime;
			movBrazosEntre += velocidadCaminataEntre * deltaTime;

			if (movPiernasEntre > 360.0f) movPiernasEntre -= 360.0f;
			if (movBrazosEntre > 360.0f) movBrazosEntre -= 360.0f;

			tiempoEntre += deltaTime * velocidadMovimiento;

			posXEntre = -30.0f + sin(tiempoEntre) * amplitudMovimiento;

			if (cos(tiempoEntre) > 0) {
				direccionEntre = -90.0f; 
			}
			else {
				direccionEntre = 90.0f; 
			}
		}


		//Animación rejas

		if (mainWindow.getActivarAnimacionRejas()) {
			if (!animacionRejasActiva && cicloCompletado) {
				animacionRejasActiva = true;
				cicloCompletado = false;
				faseAnimacion = 1; // Empezar abriendo
				tiempoAbierto = 0.0f;
			}
			mainWindow.resetActivarAnimacionRejas();
		}

		if (animacionRejasActiva) {
			switch (faseAnimacion) {
			case 1: // ABRIENDO
				if (rotRejaDer < 90.0f) {
					rotRejaDer += velocidadRotacion * deltaTime;
					if (rotRejaDer > 90.0f) rotRejaDer = 90.0f;
				}
				if (desplazaRejaIzq < 4.0f) {
					desplazaRejaIzq += velocidadDesplazamiento * deltaTime;
					if (desplazaRejaIzq > 4.0f) desplazaRejaIzq = 4.0f;
				}

				if (rotRejaDer >= 90.0f && desplazaRejaIzq >= 4.0f) {
					faseAnimacion = 2; // Pasar a pausa
					tiempoAbierto = 0.0f;
				}
				break;

			case 2: // PAUSA
				tiempoAbierto += deltaTime;
				if (tiempoAbierto >= tiempoPausa) {
					faseAnimacion = 3; // Pasar a cerrar
				}
				break;

			case 3: // CERRANDO
				if (rotRejaDer > 0.0f) {
					rotRejaDer -= velocidadRotacion * deltaTime;
					if (rotRejaDer < 0.0f) rotRejaDer = 0.0f;
				}
				if (desplazaRejaIzq > 0.0f) {
					desplazaRejaIzq -= velocidadDesplazamiento * deltaTime;
					if (desplazaRejaIzq < 0.0f) desplazaRejaIzq = 0.0f;
				}

				if (rotRejaDer <= 0.0f && desplazaRejaIzq <= 0.0f) {
					animacionRejasActiva = false;
					cicloCompletado = true;
					faseAnimacion = 0;
				}
				break;
			}

		}



		//Camara seguimiento de James
		posicionJames = glm::vec3(mainWindow.getposXJames(), 4.20f, mainWindow.getposZJames());
		float dirRadians = mainWindow.getdireccionJames() * toRadians;
		frenteJames = glm::vec3(sin(dirRadians), 0.0f, cos(dirRadians));

		// Cambiar entre modos 
		// Camara seguimiento
		if (mainWindow.getModoCamaraSigue()) {
			if (!camera.estaSiguiendo()) {
				camera.establecerObjetivoSeguimiento(&posicionJames, &frenteJames, 8.0f, 3.0f);
			}

			camera.ajustarAngulosSeguimiento(mainWindow.getXChange() * 0.1f, mainWindow.getYChange() * 0.1f);


		}
		else {
			// Modo libre
			if (camera.estaSiguiendo()) {
				camera.establecerModoLibre();
			}
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}

		//Recibir eventos del usuario
		glfwPollEvents();


		////Recibir eventos del usuario
		//glfwPollEvents();
		//camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();



		//Ciclo dia y noche
		cicloDiaNoche += velocidadCiclo * deltaTime;
		if (cicloDiaNoche > 360.0f) {
			cicloDiaNoche -= 360.0f;
		}
		nuevoEstadoNoche = (cicloDiaNoche > 180.0f);

		if (nuevoEstadoNoche != esDeNoche) {
			esDeNoche = nuevoEstadoNoche;
			skyboxCambiado = false;
		}

		if (!skyboxCambiado) {
			if (esDeNoche) {
				skybox = Skybox(skyboxFacesNoche);
			}
			else {
				skybox = Skybox(skyboxFacesDia);
			}
			skyboxCambiado = true;
		}

		shaderList[0].SetDayNight(esDeNoche);





		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		////sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//Puerta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-230.0f, -0.7f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puerta_M.RenderModel();

		//Reja izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, 0.1f, -0.05f));
		model = glm::translate(model, glm::vec3(0.0F, 0.0f, -desplazaRejaIzq));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rejaIzq_M.RenderModel();

		//Reja Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, 2.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotRejaDer * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rejaDer_M.RenderModel();

		//Letrero
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.6f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		letrero_M.RenderModel();


		//Piramida de la luna
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 200.0));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pLuna_M.RenderModel();



		//Piramida del sol
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.6f, -40.0));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pSol_M.RenderModel();

		//Centro pokemon
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -250.0));
		spotLights[0].SetPos(glm::vec3(model[3].x, model[3].y + 25.0f, 35.0+ model[3].z));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		centroPoke_M.RenderModel();

		//Globo aerostatico
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posXGlobo, posYGlobo, posZGlobo));
		if (animacionGloboActiva)
		{
			model = glm::rotate(model, rotGlobo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		globo_M.RenderModel();
		glDisable(GL_BLEND);


		//Ring
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ring_M.RenderModel();

		//Mesa 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();

		//Mesa 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -1.0f, -8.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();

		//Mesa 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -1.0f, 8.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();

		//Anuncio 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, 30.0));
		spotLights[1].SetPos(glm::vec3(model[3].x, model[3].y + 14.0f, model[3].z));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		anuncio_M.RenderModel();

		//Anuncio 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, -30.0));
		spotLights[2].SetPos(glm::vec3(model[3].x, model[3].y + 14.0f, model[3].z));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		anuncio_M.RenderModel();


		//Piedra trueno
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 2.6f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piedraTrueno_M.RenderModel();

		//Lampara pokebola 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -1.0f, 90.0));
		pointLights[0].SetPos(glm::vec3(model[3].x, model[3].y + 21.0f, model[3].z)); //posicion de la luz de la lampara
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();


		////Lampara pokebola 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -1.0f, -120.0));
		pointLights[1].SetPos(glm::vec3(model[3].x, model[3].y + 21.0f, model[3].z)); 
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();

		//Lampara pokebola 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, -1.0f, -200.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();

		//Lampara pokebola 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, -1.0f, -200.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();

		//Lampara pokebola 5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -1.0f, 200.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();

		//Lampara pokebola 6
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-220.0f, -1.0f, 200.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();

		//Lampara pokebola 7
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-230.0f, -1.0f, 75.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();

		//Lampara pokebola 8
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-230.0f, -1.0f, 150.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();

		//Lampara pokebola 9
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-230.0f, -1.0f, -75.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();

		//Lampara pokebola 10
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-230.0f, -1.0f, -150.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamPoke_M.RenderModel();


		//Luz suelo 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		luzSuelo_M.RenderModel();
		glDisable(GL_BLEND);

		//Luz suelo 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, -1.0f, 0.0));

		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		luzSuelo_M.RenderModel();
		glDisable(GL_BLEND);

		//Luz suelo 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, -1.0f, -65.0));
		pointLights[2].SetPos(glm::vec3(model[3].x, model[3].y, model[3].z));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		luzSuelo_M.RenderModel();
		glDisable(GL_BLEND);

		//Luz suelo 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, -1.0f, -15.0));
		pointLights[3].SetPos(glm::vec3(model[3].x, model[3].y, model[3].z));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		luzSuelo_M.RenderModel();
		glDisable(GL_BLEND);


		//Bandera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -1.0f, 200.0));
		model = glm::rotate(model, -65 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bandera_M.RenderModel();




		//Instancia cuerpo Bulbasaur 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getPosXBulbasaur(), 4.85f, mainWindow.getPosZBulbasaur()));
		model = glm::rotate(model, mainWindow.getDireccionBulbasaur() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Bulbasaur_M.RenderModel();
		glDisable(GL_BLEND);

		//Pata izquierda delantera Bulbasaur
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.05f, -0.53f, -1.07f));
		model = glm::rotate(model, sin(movPiernasBulbasaur * toRadians) * 25.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		izqDelanBul_M.RenderModel();

		//Pata trasera izquierda Bulbasaur
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.1f, -0.53f, 0.83f));
		model = glm::rotate(model, -sin(movPiernasBulbasaur * toRadians) * 25.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		izqTrasBul_M.RenderModel();

		//Pata delantera derecha Bulbasaur
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.1f, -0.53f, -1.07f));
		model = glm::rotate(model, -sin(movPiernasBulbasaur * toRadians) * 25.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		derDelanBul_M.RenderModel();

		//Pata trasera derecha Bulbasaur
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.05f, -0.53f, 0.83f));
		model = glm::rotate(model, sin(movPiernasBulbasaur * toRadians) * 25.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		derTrasBul_M.RenderModel();

		//Instancia cuerpo Eevee 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getPosXEevee(), 4.7f, mainWindow.getPosZEevee()));
		model = glm::rotate(model, mainWindow.getDireccionEevee() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Eevee_M.RenderModel();

		//Cabeza Eevee
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, -0.75f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cabezaEevee_M.RenderModel();

		//Cola Eevee
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.08f, 0.8f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		colaEevee_M.RenderModel();

		//Pata izquierda delantera Eevee
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.3f, -0.18f, -0.4f));
		model = glm::rotate(model, sin(movPiernasEevee * toRadians) * 30.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		izqDelanEevee_M.RenderModel();

		//Pata trasera izquierda Eevee
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.3f, -0.05f, 0.5f));
		model = glm::rotate(model, -sin(movPiernasEevee * toRadians) * 30.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		izqTrasEevee_M.RenderModel();

		//Pata delantera derecha Eevee
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.3f, -0.18f, -0.4f));
		model = glm::rotate(model, -sin(movPiernasEevee * toRadians) * 30.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		derDelanEevee_M.RenderModel();

		//Pata trasera derecha Eevee
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.3f, -0.05f, 0.5f));
		model = glm::rotate(model, sin(movPiernasEevee * toRadians) * 30.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		derTrasEevee_M.RenderModel();




		// Instancia cuerpo James
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getposXJames(), 4.20f, mainWindow.getposZJames()));
		model = glm::rotate(model, mainWindow.getdireccionJames() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpoJames_M.RenderModel();

		// Cabeza James
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cabezaJames_M.RenderModel();

		// Brazo izquierdo James 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.8f, 1.13f, 0.15f));
		if (caminandoJames) {
			model = glm::rotate(model, sin(movBrazosJames * toRadians) * 25.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bIzqJames_M.RenderModel();

		// Codo y mano izquierda James 
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.7f, -0.89f, 0.04f));
		if (caminandoJames) {
			anguloCodo = (sin(movBrazosJames * toRadians) + 1.0f) * 0.5f * 50.0f;
			model = glm::rotate(model, anguloCodo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mIzqJames_M.RenderModel();

		// Brazo derecho James 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 1.13f, 0.15f));
		if (caminandoJames) {
			model = glm::rotate(model, -sin(movBrazosJames * toRadians) * 25.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bDerJames_M.RenderModel();

		// Codo y mano derecha James 
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.7f, -0.89f, 0.06f));
		if (caminandoJames) {
			anguloCodo = (sin((movBrazosJames + 180.0f) * toRadians) + 1.0f) * 0.5f * 50.0f;
			model = glm::rotate(model, anguloCodo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mDerJames_M.RenderModel();

		// Pierna izquierda James
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.21f, -0.7f, 0.10f));
		if (caminandoJames) {
			model = glm::rotate(model, sin(movPiernasJames * toRadians) * 35.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pIzqJames_M.RenderModel();

		// Rodilla y pie izquierdo James
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.25f, -2.15f, 0.15f));
		if (caminandoJames) {
			anguloRodilla = -(sin(movPiernasJames * toRadians) + 1.0f) * 0.5f * 50.0f;
			model = glm::rotate(model, anguloRodilla * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zIzqJames_M.RenderModel();

		// Pierna derecha James
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.21f, -0.7f, 0.10f));
		if (caminandoJames) {
			model = glm::rotate(model, -sin(movPiernasJames * toRadians) * 35.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pDerJames_M.RenderModel();

		// Rodilla y pie derecho James
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.25f, -2.15f, 0.15f));
		if (caminandoJames) {
			anguloRodilla = -(sin(movPiernasJames * toRadians) + 1.0f) * 0.5f * 50.0f;
			model = glm::rotate(model, anguloRodilla * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zDerJames_M.RenderModel();






		// Instancia cuerpo Entrenador
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posXEntre, 3.7f, posZEntre));
		model = glm::rotate(model, direccionEntre * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpoEntre_M.RenderModel();

		// Brazo derecho Entrenador 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.5f, 1.1f, -0.41f));
		model = glm::rotate(model, -sin(movBrazosEntre * toRadians) * 25.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bDerEntre_M.RenderModel();

		// Brazo izquierdo Entrenador 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 1.1f, 0.4f));
		model = glm::rotate(model, sin(movBrazosEntre * toRadians) * 25.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bIzqEntre_M.RenderModel();

		// Pierna derecha Entrenador
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.3f, -1.0f, 0.1f));
		model = glm::rotate(model, -sin(movPiernasEntre * toRadians) * 35.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pDerEntre_M.RenderModel();

		// Pie derecho Entrenador
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.65f, -1.5f, -0.75f));
		anguloRodillaEntre = -(sin(movPiernasEntre * toRadians) + 1.0f) * 0.5f * 50.0f;
		model = glm::rotate(model, anguloRodillaEntre * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zDerEntre_M.RenderModel();

		// Pierna izquierda Entrenador
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, -0.9f, 0.2f));
		model = glm::rotate(model, sin(movPiernasEntre * toRadians) * 35.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pIzqEntre_M.RenderModel();

		// Pie izquierdo Entrenador
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.8f, -1.6f, -0.02f));
		anguloRodillaEntre = -(sin(movPiernasEntre * toRadians) + 1.0f) * 0.5f * 50.0f;
		model = glm::rotate(model, anguloRodillaEntre * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zIzqEntre_M.RenderModel();






		//Instancia cuerpo Oak
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), -1.0f, 70.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpoOak_M.RenderModel();

		//Cabeza Oak
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 6.4f, 0.2f));
		model = glm::rotate(model, -anguloCabeza * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cabezaOak_M.RenderModel();

		//Brazo izquierdo Oak (hombro)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, 6.0f, 0.13f));
		model = glm::rotate(model, anguloSaludo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bIzqOak_M.RenderModel();

		//Brazo y mano Oak 
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.9f, -2.0f, 0.0f));
		model = glm::rotate(model, -anguloCodoOak * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mIzqOak_M.RenderModel();

		//Brazo derecho Oak
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.6f, 6.15f, 0.13f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bDerOak_M.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}