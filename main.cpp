// Use a vers�o do GLFW compativel com 64 bits.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Usado para escrever no console com C++
#include <iostream>

// Declara��o de fun��es deve ocorrer antes do Main.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void inputTeclas(GLFWwindow* window);
void compilaVertexShader(int vs);
void compilaFragmentShader(int fs);
void vinculaProgramShader(int ps);

// Declara��o da resolu��o da janela
const unsigned int larguraJanela = 800;
const unsigned int alturaJanela = 600;

// Buffer com os vertices do tri�ngulo. (Vertex Buffer Object - VBO)
unsigned int VBO;

// Buffer com os arrays de vertices. (Vertex Array Object - VAO)
unsigned int VAO;

// Declarando e compilando um vertex shader.
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

// Declarando e compilando um fragment shader.
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";

// Declarando a variavel que ser� utilizada pelo vertex shader vazio.
unsigned int vertexShader;

// Declarando a variavel que ser� utilizada pelo fragment shader vazio.
unsigned int fragmentShader;

// Declarando a variavel que ser� utilizada na jun��o dos shaders (Vertex + Fragment).
// Resulta num ProgramShader
unsigned int shaderProgram;

// Coordenadas do triangulo de definidas em "x", "y" e "z".
   // Janela possui intervalos de -1.0  a 1.0. (Normalized Device Coordinates - NDC)
float vertices[] = {
   -0.5f, -0.5f,  0.0f, // Lateral esquerda
    0.5f, -0.5f,  0.0f, // Laterial direita
    0.0f, 0.5f,  0.0f  // Centro Superior
};


int main()
{
    // Fun��o responsavel por inicializar o GLFW.
    glfwInit();

    // Fun��o responsavel por definir a vers�o a ser executada. (3.x)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    // Fun��o responsavel por definir a vers�o a ser executada. (x.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Fun��o resposavel por definir o perfil do glfw
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Definindo ponteiro da janela.
    GLFWwindow* JanelaPrincipal = glfwCreateWindow(larguraJanela, alturaJanela, "Apendendo OpenGL", NULL, NULL);

    // Tratamento de erro na cria��o da janela.
    if (JanelaPrincipal == NULL)
    {
        // Mensagem de erro ao criar janela.
        std::cout << "Erro ao criar janela com OpenGL" << std::endl;

        // Finalizar cria��o de janela com glfw;
        glfwTerminate();
    }

    glfwMakeContextCurrent(JanelaPrincipal);
    glfwSetFramebufferSizeCallback(JanelaPrincipal, framebuffer_size_callback);

    // Passa para o Glad os ponteiros do OpenGL.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Erro ao inicializar o Glad" << std::endl;
    }

    // VERTEX SHADER
    // Criando o Vertex Shader vazio.
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Envia o codigo-fonte do shared compilado ao objeto de shader.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // Compila o shader fornecido.
    glCompileShader(vertexShader);

    // Fun��o que testa se ocorreu erro durante compila��o.
    compilaVertexShader(vertexShader);

    // FRAGMENT SHADER
    // Criando o Fragment Shader vazio.
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Envia o codigo-fonte do shared compilado ao objeto de shader.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // Compila o shader fornecido.
    glCompileShader(fragmentShader);

    // Fun��o que testa se ocorreu erro durante compila��o.
    compilaFragmentShader(fragmentShader);

    // PROGRAM SHADER
    // ProgramShader vazio (Utilizado na jun��o do vertex e fragment).
    shaderProgram = glCreateProgram();

    // Fun��o que associa um shader a programas de shader
    glAttachShader(shaderProgram, vertexShader);

    // Fun��o que associa um shader a programas de shader
    glAttachShader(shaderProgram, fragmentShader);

    // Vincula todos os shaders no programa.
    glLinkProgram(shaderProgram);

    // Fun��o que testa se ocorreu erro durante vincula��o de shader em um programa.
    vinculaProgramShader(shaderProgram);

    // Comando para exluir os shader depois que vinculados a um programa.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Fun��o que gera o buffer.
    glGenVertexArrays(1, &VAO);  // VAO - Vertex Array Object

    // Fun��o que gera o buffer (Quantidade e refer�ncia).
    glGenBuffers(1, &VBO);  // VBO - Vertex Buffer Object

    // O objetos de vetor de vertices � usado para automatizar o envio de objetos para o desenho.
    // Fun��o que gera o buffer.
    glBindVertexArray(VAO);  // VAO - Vertex Array Object

    // Fun��o que vinculo o objeto de buffer ao proprio buffer do OpenGL (VBO � enviado para GL_ARRAY_BUFFER).
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Fun��o que aloca memoria e envia os dados ao buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Comando especifica ao OpenGL como ele deve ler o Array dos vertices ao desenhar o triangulo.
    // (x, y, z), (x, y, z), (x, y, z). 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Especifica ao OpenGL como ele deve interpretar dos dados do buffer (Array).
    glEnableVertexAttribArray(0);

    // 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 
    glBindVertexArray(0);

    // Exibe janela at� clicar no de frechar (sem que se feche automatica).
    // Loop de renderiza��o.
    while (!glfwWindowShouldClose(JanelaPrincipal))
    {
        // Fun��o de detectar tecla ESC.
        processInput(JanelaPrincipal);

        // Fun��o de detectar teclas "asd".
        inputTeclas(JanelaPrincipal);

        // Define o buffer preto por padr�o.
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        // Comando para usar o objeto de programa.
        glUseProgram(shaderProgram);


        // O objetos de vetor de vertices � usado para automatizar o envio de objetos para o desenho.
        // Fun��o que gera o buffer.
        glBindVertexArray(VAO);  // VAO - Vertex Array Object

        // 
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Responsavel por manipular o buffer da janela.
        glfwSwapBuffers(JanelaPrincipal);

        // Responsavel por atualizar o status da janela.
        glfwPollEvents();

        // VSync de 60 Quadros por Segundo (via GLFW).
        glfwSwapInterval(1);

    }

    // Comandos opcionais para desalocar memoria.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Finalizar cria��o de janela com glfw;
    glfwTerminate();

    return 0;
}

// Cria um processo para detectar input de teclas dentro do loop de renderiza��o.
void inputTeclas(GLFWwindow* window) {

    // Le se a tecla escape (ESC) foi pressionada.
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    }
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    }
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    }
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    }
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    }
    glClear(GL_COLOR_BUFFER_BIT);
}

// Cria um processo para detectar input de teclas dentro do loop de renderiza��o.
void processInput(GLFWwindow* window) {
    // Le se a tecla escape (ESC) foi pressionada.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Redimensiona janela.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void compilaVertexShader(int vs) {
    // Tratando erros durante a compila��o de shaders
    int  successoCompilacaoVertex;
    char infoLogVertex[512];

    glGetShaderiv(vs, GL_COMPILE_STATUS, &successoCompilacaoVertex);

    if (!successoCompilacaoVertex) {

        // Envia mensagem para o log
        glGetShaderInfoLog(vs, 512, NULL, infoLogVertex);

        // Imprime mensagem de erro de compila��o e informa��es do log.
        std::cout << "Erro durante a compila��o do Vertex Shader \n" << infoLogVertex << std::endl;
    }
}

void compilaFragmentShader(int fs) {
    // Tratando erros durante a compila��o de shaders
    int  successoCompilacaoFragment;
    char infoLogFragment[512];

    glGetShaderiv(fs, GL_COMPILE_STATUS, &successoCompilacaoFragment);

    if (!successoCompilacaoFragment) {

        // Envia mensagem para o log
        glGetShaderInfoLog(fs, 512, NULL, infoLogFragment);

        // Imprime mensagem de erro de compila��o e informa��es do log.
        std::cout << "Erro durante a compila��o do Fragment Shader \n" << infoLogFragment << std::endl;
    }
}

void vinculaProgramShader(int ps) {
    // Tratando erros durante a vincula��o de shaders
    int  successoPrograma;
    char infoLogPrograma[512];

    glGetProgramiv(ps, GL_LINK_STATUS, &successoPrograma);
    if (!successoPrograma) {

        // Envia mensagem para o log
        glGetProgramInfoLog(ps, 512, NULL, infoLogPrograma);

        // Imprime mensagem de erro vincula��o e informa��es do log.
        std::cout << "Erro durante a vincula��o do Program Shader \n" << infoLogPrograma << std::endl;
    }
}

//---------------------------------------------------------
// Mal sei C++ e estou escrevendo
// Ler sobre: 
// VBO, VAO, Vertex Shader, Fragment Shader, Program Shader, Normalized Device Coordinates

