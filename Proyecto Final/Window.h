#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }

	//Animación del globo aerostático
	bool inicioGlobo() { return globoAnimation; }
	void resetGlobo() { globoAnimation = false; }


	//Animación caminata James
	GLboolean getcaminandoJames() { return caminandoJames; }
	GLfloat getdireccionJames() { return direccionJames; }
	GLfloat getposXJames() { return posXJames; }
	GLfloat getposZJames() { return posZJames; }
	GLfloat getMoveSpeed() { return 0.1f; }
	bool getMoveForward() { return moveForward; }
	bool getMoveBackward() { return moveBackward; }
	bool getMoveLeft() { return moveLeft; }
	bool getMoveRight() { return moveRight; }
	bool getRotateLeft() { return rotateLeft; }
	bool getRotateRight() { return rotateRight; }
	void setPosXJames(GLfloat x) { posXJames = x; }
	void setPosZJames(GLfloat z) { posZJames = z; }
	void setDireccionJames(GLfloat dir) { direccionJames = dir; }


	//Camara seguimiento
	bool getModoCamaraSigue() { return modoCamaraSigue; }
	void alternarModoCamara() { modoCamaraSigue = !modoCamaraSigue; }


	//Animación Eevee y Bulbasaur
	GLfloat getPosXEevee() { return posXEevee; }
	GLfloat getPosZEevee() { return posZEevee; }
	GLfloat getDireccionEevee() { return direccionEevee; }
	GLfloat getPosXBulbasaur() { return posXBulbasaur; }
	GLfloat getPosZBulbasaur() { return posZBulbasaur; }
	GLfloat getDireccionBulbasaur() { return direccionBulbasaur; }
	void setPosXEevee(GLfloat x) { posXEevee = x; }
	void setPosZEevee(GLfloat z) { posZEevee = z; }
	void setDireccionEevee(GLfloat dir) { direccionEevee = dir; }
	void setPosXBulbasaur(GLfloat x) { posXBulbasaur = x; }
	void setPosZBulbasaur(GLfloat z) { posZBulbasaur = z; }
	void setDireccionBulbasaur(GLfloat dir) { direccionBulbasaur = dir; }


	//Animación Entrenador Pokémon
	GLfloat getMovPiernasEntre() { return movPiernasEntre; }
	GLfloat getMovBrazosEntre() { return movBrazosEntre; }
	GLboolean getCaminandoEntre() { return caminandoEntre; }


	//Animación puerta
	bool getActivarAnimacionRejas() { return activarAnimacionRejas; }
	void resetActivarAnimacionRejas() { activarAnimacionRejas = false; }




	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool mouseFirstMoved;

	//Animación del globo aerostático
	bool globoAnimation;


	//Animación caminata James
	GLfloat movPiernasJames;
	GLfloat movBrazosJames;
	GLboolean caminandoJames;
	GLfloat direccionJames;
	GLfloat posXJames;
	GLfloat posZJames;

	bool moveForward;
	bool moveBackward;
	bool moveLeft;
	bool moveRight;

	bool rotateLeft;
	bool rotateRight;



	//Camara seguimiento
	bool modoCamaraSigue = false;




	//Animación Eevee y Bulbasaur
	GLfloat posXEevee;
	GLfloat posZEevee;
	GLfloat direccionEevee;
	GLfloat posXBulbasaur;
	GLfloat posZBulbasaur;
	GLfloat direccionBulbasaur;


	//Animación Entrenador Pokémon
	GLfloat movPiernasEntre;
	GLfloat movBrazosEntre;
	GLboolean caminandoEntre;


	//Animación puerta
	bool activarAnimacionRejas;



	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

