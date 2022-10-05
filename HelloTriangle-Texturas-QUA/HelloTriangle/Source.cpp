/* Noah Game
 *
 * Adaptado por Luan e Suelen
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 05/10/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#define GLFW_KEY_DOWN 264
#define GLFW_KEY_UP 265
#define GLFW_KEY_SPACE 32

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB_IMAGE
#include "stb_image.h"

#include "Shader.h"


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupSprite();
int setupSprite(int nAnimations, int nFrames, float& dx, float& dy);

GLuint createTexture(string filePath);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Função MAIN
int main()
{
	srand((int)glfwGetTime());
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Noah Game", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;



	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um triângulo

	float dx, dy;
	GLuint VAO_Sea = setupSprite(1, 1, dx, dy);
	GLuint VAO_Shark = setupSprite(1, 4, dx, dy);
	GLuint VAO_Noah = setupSprite(1, 8, dx, dy);
	GLuint VAO_GameOver = setupSprite(1, 1, dx, dy);


	int iAnimation = 0;
	int iFrame = 0;
	int changeFrame = 0;
	int numberFramesNoah = 8;
	const float speed = 0.02;
	int velShark = 0;

	float xShark = 400.0f, yShark = 700.0f;
	float xNoah = 100.0f, yNoah = 170.0f;
	float xGameOver = 800.0f, yGameOver = 600.0f;
	float xScalaShark = 150.0f, yScalaShark = 70.0f;
	float xScalaNoah = 117.916f, yScalaNoah = 58.66f;
	bool gameOver = false;

	GLuint texID_Fundo = createTexture("../textures/1u3w_fsob_190604.jpg");
	GLuint texID_Noah = createTexture("../textures/noah.png");
	GLuint texID_Shark = createTexture("../textures/shark.png");
	GLuint texID_gameOver = createTexture("../textures/game-over.gif");

	//Matriz de projeção
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);


	glUseProgram(shader.ID);

	shader.setMat4("projection", glm::value_ptr(projection));

	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer"), 0);


	glActiveTexture(GL_TEXTURE0);

	//Habilitando o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Habilitando a transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);


		//----------------Background---------------------
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1080.0f, 600.0f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		shader.setVec2("offsets", 1, 1);
		if (!gameOver) {
			glBindVertexArray(VAO_Sea);
			glBindTexture(GL_TEXTURE_2D, texID_Fundo);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//---------------Noah-------------------------
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(xNoah, yNoah, 0.0f));
		model = glm::scale(model, glm::vec3(xScalaNoah, yScalaNoah, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		shader.setVec2("offsets", iFrame * dx, iAnimation * dy);

		if (changeFrame % 100 == 0) {
			iFrame = (iFrame + 1) % numberFramesNoah;
		}

		changeFrame++;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && yNoah <= 500) {
			yNoah += 50.0f * 0.02f;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && yNoah >= 50) {
			yNoah -= 50.0f * 0.02f;
		}

		if (!gameOver) {
			glBindVertexArray(VAO_Noah);
			glBindTexture(GL_TEXTURE_2D, texID_Noah);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//--------------Shark--------------------------
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(xShark, yShark, 0.0f));
		model = glm::scale(model, glm::vec3(xScalaShark, yScalaShark, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		shader.setVec2("offsets", 1, 1);

		if (!gameOver) {
			glBindVertexArray(VAO_Shark);
			glBindTexture(GL_TEXTURE_2D, texID_Shark);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (xShark <= 0.0)
		{
			yShark = 64 + rand() % 500;
			xShark = 850;
		}

		if (velShark > 10000) {
			xShark -= 10;
		}

		else if (velShark >= 5000 && velShark < 10000) {
			xShark -= 5;
		}

		else if (velShark >= 3000 && velShark < 4000) {
			xShark -= 4;
		}

		else if (velShark >= 2000 && velShark < 3000) {
			xShark -= 3;
		}

		else if (velShark >= 1000 && velShark < 2000) {
			xShark -= 2;
		}

		else if (velShark < 1000) {
			xShark -= 1;
		}


		cout << velShark << endl;

		velShark += 1;

		// ----------------- Colisão -----------------
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
		model = glm::scale(model, glm::vec3(800.0f, 600.0f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		shader.setVec2("offsets", 1, 1);

		float xDiagonalSuperiorNoah = xNoah + xScalaNoah / 2;
		float yDiagonalSuperiorNoah = yNoah + yScalaNoah / 2;
		float xDiagonalSuperiorShark = xShark + xScalaShark / 2;
		float yDiagonalSuperiorShark = yShark + yScalaShark / 2;

		bool colisaoX = xDiagonalSuperiorNoah >= xShark && xDiagonalSuperiorShark >= xNoah;
		bool colisaoY = yDiagonalSuperiorNoah >= yShark && yDiagonalSuperiorShark >= yNoah;


		if (colisaoX && colisaoY) {
			glBindVertexArray(VAO_GameOver);
			glBindTexture(GL_TEXTURE_2D, texID_gameOver);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			gameOver = true;
		}

		if (gameOver || (colisaoX && colisaoY)) {
			glBindVertexArray(VAO_GameOver);
			glBindTexture(GL_TEXTURE_2D, texID_gameOver);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			velShark = 0;
			gameOver = false;
		}


		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO_Noah);
	glDeleteVertexArrays(1, &VAO_Sea);

	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint createTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregando a imagem do filePath e armazenando em data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);



	return texID;
}

int setupSprite()
{
	GLfloat vertices[] = {
		//x   y     z    s    t
		//Primeiro triângulo
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5, -0.5, 0.0, 1.0, 0.0, //v1
		 0.5,  0.5, 0.0, 1.0, 1.0,  //v2
		 //outro triangulo vai aqui
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5,  0.5, 0.0, 1.0, 1.0,  //v2
		-0.5,  0.5, 0.0, 0.0, 1.0, //v3
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo coord de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int setupSprite(int nAnimations, int nFrames, float& dx, float& dy)
{
	dx = 1.0 / nFrames;
	dy = 1.0 / nAnimations;

	GLfloat vertices[] = {
		//x   y     z    s    t
		//Primeiro triângulo
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5, -0.5, 0.0, dx, 0.0, //v1
		 0.5,  0.5, 0.0, dx, dy,  //v2
		 //outro triangulo vai aqui
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5,  0.5, 0.0, dx, dy,  //v2
		-0.5,  0.5, 0.0, 0.0, dy, //v3
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo coord de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
