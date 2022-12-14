/* Hello Triangle - c?digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr?fico - Unisinos
 * Vers?o inicial: 7/4/2017
 * ?ltima atualiza??o em 01/08/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;


//// GLFW
//#include <GLFW/glfw3.h>
//
////GLM
//#include <glm/glm.hpp> 
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//
//#include "Shader.h"
//
//#include "stb_image.h"

#include "Tilemap.h"

//using namespace glm;


// Prot?tipo da fun??o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot?tipos das fun??es
int setupGeometry();
int setupSprite();
int setupSprite(int nFrames, int nAnimations, float &dx, float &dy);
int setupGeometry3D();

GLuint generateTexture(string filePath);

// Dimens?es da janela (pode ser alterado em tempo de execu??o)
const GLuint WIDTH = 800, HEIGHT = 600;

glm::vec3 posPersonagem; //em indice no mapa

Tilemap tilemap;
Tilemap coinmap;

// Fun??o MAIN
int main()
{
	// Inicializa??o da GLFW
	glfwInit();

	//Muita aten??o aqui: alguns ambientes n?o aceitam essas configura??es
	//Voc? deve adaptar para a vers?o do OpenGL suportada por sua placa
	//Sugest?o: comente essas linhas de c?digo para desobrir a vers?o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria??o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Tilemap!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun??o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun??es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa??es de vers?o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens?es da viewport com as mesmas dimens?es da janela da aplica??o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	


	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello_triangle.vs", "../shaders/hello_triangle.fs");



	tilemap.initialize("../maps/map2-regular.txt");
	coinmap.initialize("../maps/map3-regular.txt");
	tilemap.setShader(&shader);
	coinmap.setShader(&shader);

	//float dx, dy;
	//tilemap.initializePlayer("../textures/dinoanda.png", 5, 1, dx, dy);

	tilemap.initializePlayer("../textures/dinoanda.png", 5, 1);

	// Gerando um buffer simples, com a geometria de um tri?ngulo
	GLuint VAO = setupSprite();

	
	//GLuint VAODino = setupSprite(5, 1, dx, dy);

	//cout << dx << "  " << dy << endl;
	//GLuint texID3 = generateTexture("../textures/dinoanda.png");
	
	//Outros atributos do dino
	
	posPersonagem.x = 0;
	posPersonagem.y = 0;
	//int iFrame = 0;
	//int iAnimation = 0;

	
	//Ativa o shader que vai ser usado
	glUseProgram(shader.ID);

	//Associa a vari?vel que armazenar? o buffer de textura
	//glUniform1i(glGetUniformLocation(shader.ID, "texBuffer"), 0);
	shader.setInt("texBuffer", 0);

	//Matriz de proje??o
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.1f, 100.0f);
	shader.setMat4("projection", glm::value_ptr(projection));

	//Matriz de view
	glm::mat4 view = glm::mat4(1);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	shader.setMat4("view", glm::value_ptr(view));

	//Habilita teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);


	//Habilita um buffer de textura
	glActiveTexture(GL_TEXTURE0);

	//Habilita a transpar?ncia
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Loop da aplica??o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun??es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);


		//Viewport 
		glViewport(0, 0, width, height);

		//chama a atualiza??o do mapa
		//tilemap.update();
		coinmap.update();

		//chama o desenho do mapa
		tilemap.draw();
		coinmap.draw();
		tilemap.drawPlayer();


		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu??o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun??o de callback de teclado - s? pode ter uma inst?ncia (deve ser est?tica se
// estiver dentro de uma classe) - ? chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS) //norte
	{
		if (posPersonagem.y > 0 && tilemap.isWalkable(posPersonagem.y - 1, posPersonagem.x))
		{
			if (tilemap.isKillable(posPersonagem.y - 1, posPersonagem.x)) {
				coinmap.initialize("../maps/map3-regular.txt");
				posPersonagem.y = 0;
				posPersonagem.x = 0;
			}
			else {
				posPersonagem.y--;
			}
		}
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) //sul
	{
		if (posPersonagem.y < MAP_HEIGHT-1 && tilemap.isWalkable(posPersonagem.y + 1, posPersonagem.x))
		{
			if (tilemap.isKillable(posPersonagem.y + 1, posPersonagem.x)) {
				coinmap.initialize("../maps/map3-regular.txt");
				posPersonagem.y = 0;
				posPersonagem.x = 0;
			}
			else {
				posPersonagem.y++;
			}
		}

	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) //oeste
	{
		if (posPersonagem.x > 0 && tilemap.isWalkable(posPersonagem.y, posPersonagem.x - 1))
		{
			if (tilemap.isKillable(posPersonagem.y, posPersonagem.x - 1)) {
				coinmap.initialize("../maps/map3-regular.txt");
				posPersonagem.y = 0;
				posPersonagem.x = 0;
			}
			else {
				posPersonagem.x--;
			}
		}
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) //leste
	{
		if (posPersonagem.x < MAP_WIDTH - 1 && tilemap.isWalkable(posPersonagem.y, posPersonagem.x + 1))
		{
			if (tilemap.isKillable(posPersonagem.y, posPersonagem.x + 1)) {
				coinmap.initialize("../maps/map3-regular.txt");
				posPersonagem.y = 0;
				posPersonagem.x = 0;
			}
			else {
				posPersonagem.x++;
			}
		}
	}
	
	tilemap.setCharacterPos(posPersonagem);
	coinmap.setCharacterPos(posPersonagem);

}

// Esta fun??o est? bastante harcoded - objetivo ? criar os buffers que armazenam a 
// geometria de um tri?ngulo
// Apenas atributo coordenada nos v?rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun??o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri?ngulo e as armazenamos de forma
	// sequencial, j? visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v?rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO ?nico ou em VBOs separados
	GLfloat vertices[] = { //Ex3)
		-0.5, -0.5, 0.0, //v0
		 0.5, -0.5, 0.0, //v1
		 0.0, 0.5, 0.0,  //v2
		 //outro triangulo vai aqui
	};

	GLuint VBO, VAO;

	//Gera??o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex?o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera??o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v?rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza??o no shader * (a localiza??o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est? normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso ? permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v?rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran?a
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (? uma boa pr?tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}

int setupSprite()
{
	// Aqui setamos as coordenadas x, y e z do tri?ngulo e as armazenamos de forma
	// sequencial, j? visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v?rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO ?nico ou em VBOs separados
	GLfloat vertices[] = {
		//Base da pir?mide: 2 tri?ngulos
		//x    y    z    r    g    b	s     t
		-0.5, -0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0,
		 0.5, -0.5, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 
		-0.5,  0.5, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		 

		 0.5, -0.5, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0,
		 0.5,  0.5, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
		-0.5,  0.5, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};
	GLuint VBO, VAO;
	//Gera??o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex?o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Gera??o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v?rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza??o no shader * (a localiza??o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est? normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	//Atributo posi??o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//Atributo coordenada de textura (s,t)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	// Observe que isso ? permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v?rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran?a
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Desvincula o VAO (? uma boa pr?tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
	return VAO;
}

int setupSprite(int nFrames, int nAnimations, float &dx, float &dy)
{
	dx = 1.0f / (float) nFrames;
	dy = 1.0f / (float) nAnimations;

	GLfloat vertices[] = {
		//Base da pir?mide: 2 tri?ngulos
		//x    y    z    r    g    b	s     t
		-0.5, -0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0,
		 0.5, -0.5, 0.0, 1.0, 0.0, 1.0, dx, 0.0,
		-0.5,  0.5, 0.0, 0.0, 1.0, 1.0, 0.0, dy,


		 0.5, -0.5, 0.0, 1.0, 0.0, 1.0, dx, 0.0,
		 0.5,  0.5, 0.0, 0.0, 1.0, 1.0, dx, dy,
		-0.5,  0.5, 0.0, 0.0, 1.0, 1.0, 0.0, dy,
	};
	GLuint VBO, VAO;
	//Gera??o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex?o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Gera??o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v?rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Atributo posi??o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//Atributo coordenada de textura (s,t)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	// Observe que isso ? permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v?rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran?a
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Desvincula o VAO (? uma boa pr?tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
	return VAO;
}

int setupGeometry3D()
{
	// Aqui setamos as coordenadas x, y e z do tri?ngulo e as armazenamos de forma
	// sequencial, j? visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v?rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO ?nico ou em VBOs separados
	GLfloat vertices[] = {
		//Base da pir?mide: 2 tri?ngulos
		//x    y    z    r    g    b	s     t
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0, 0.0, 0.0,
		-0.5, -0.5,  0.5, 0.0, 1.0, 1.0, 0.0, 1.0,
		 0.5, -0.5, -0.5, 1.0, 0.0, 1.0, 1.0, 0.0,
		 -0.5, -0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 1.0,
		  0.5, -0.5,  0.5, 0.0, 1.0, 1.0,1.0, 1.0,
		  0.5, -0.5, -0.5, 1.0, 0.0, 1.0,1.0, 0.0,
		  //
		  -0.5, -0.5, -0.5, 1.0, 1.0, 0.0, 0.0, 0.0,
		   0.0,  0.5,  0.0, 1.0, 1.0, 0.0, 0.5, 1.0,
		   0.5, -0.5, -0.5, 1.0, 1.0, 0.0, 1.0, 0.0,

		  -0.5, -0.5, -0.5, 1.0, 0.0, 1.0, 0.0, 0.0,
		   0.0,  0.5,  0.0, 1.0, 0.0, 1.0, 0.5, 1.0,
		  -0.5, -0.5,  0.5, 1.0, 0.0, 1.0, 1.0, 0.0,

		  -0.5, -0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0,
		   0.0,  0.5, 0.0, 1.0, 1.0, 0.0, 0.5, 1.0,
		   0.5, -0.5, 0.5, 1.0, 1.0, 0.0, 1.0, 0.0,

		   0.5, -0.5, 0.5, 0.0, 1.0, 1.0, 0.0, 0.0,
		   0.0,  0.5,  0.0, 0.0, 1.0, 1.0,0.5, 1.0,
		   0.5, -0.5, -0.5, 0.0, 1.0, 1.0,1.0, 0.0
	};
	GLuint VBO, VAO;
	//Gera??o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex?o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Gera??o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v?rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza??o no shader * (a localiza??o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est? normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	//Atributo posi??o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//Atributo coordenada de textura (s,t)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	// Observe que isso ? permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v?rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran?a
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Desvincula o VAO (? uma boa pr?tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
	return VAO;
}

GLuint generateTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na mem?ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregamento do arquivo de imagem da textura
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	//Verifica se conseguiu carregar e manda a textura pro OpenGL
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //4 canais png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data); //liberando o array da imagem

	glBindTexture(GL_TEXTURE_2D, 0); // dar "unbind" 

	return texID;
}

