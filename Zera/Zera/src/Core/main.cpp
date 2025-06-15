#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

//Vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment shader source
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor; \n"

"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

//Frame buffer function that takes in the window object with two numbers and changes the window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//Proccessing user input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



//A sereies of floats that represents the triangle's verticies
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

int main(void)
{
    //init glfw
    glfwInit();

    //Gives the right contex for the openGL window 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creates the GLFW window object and sets the object to the create window function
    GLFWwindow* window = glfwCreateWindow(800, 600, "Zera", NULL, NULL);

    //Error handeling for window
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Assign the current context to the window 
    glfwMakeContextCurrent(window);

    //GLAD error handeling
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //This tells OPENGL parameters for rendering to the screen
    glViewport(0, 0, 800, 600);

    //Int to store the VAO
    unsigned int VAO;
    //Generates the Vertex Arrays
    glGenVertexArrays(1, &VAO);
    //Binds the VAO to the vertex array
    glBindVertexArray(VAO);


    //Creates a unsigned int for the Vertex array buffer
    unsigned int VBO;
    //This tells Opengl to generate a buffer that takes in the VBO int
    glGenBuffers(1, &VBO);
    //This binds the buffer to the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //This copies our data into the currently bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //This is the vertex shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //This attached the vertexShader source to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //This compiiles the shader
    glCompileShader(vertexShader);

    //Checks if ther shader Compiled
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //This is the fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //This attaches the fragment shader object to the fragment shader source
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //This compiles the shader 
    glCompileShader(fragmentShader);

    //Unsigned int to hold the shader program
    unsigned int shaderProgram;
    //This creates a shader program
    shaderProgram = glCreateProgram();

    //This attaches the shaders to the shader programs
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //Liks the shader program object to the program
    glLinkProgram(shaderProgram);

    //This checks to see if the shader prograqm compiled successfully
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    //This uses the currently binded shader program
    glUseProgram(shaderProgram);

    //This deletes the shaders 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //This is a wierd set of functions that tell OpenGL how it should interpret the vertex data (per vertex attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // function that takes in the GLFW window object and 2 ints for the dimensions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //The main render loop for the application
    while (!glfwWindowShouldClose(window))
    {

        //This function checks to see if a key is being pressesd
        processInput(window);

        glClearColor(0.3f, 0.0f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //This swaps the buffers on the provided window object
        glfwSwapBuffers(window);

        //Render input before the frame is called to render
        glClear(GL_COLOR_BUFFER_BIT);
        
        //This function proccesses user input
        glfwPollEvents();
    }
    
    //Terminates GLFW
    glfwTerminate();
    return 0;

}
