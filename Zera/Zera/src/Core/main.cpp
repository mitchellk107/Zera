#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

//This takes in a window object and it adjusts the size of the window 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//This processes input
void processInput(GLFWwindow* window);

//Screen resolution
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Vertex shader GLSL code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment shader GLSL code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // Setup that tells openGL what version and that we want to use modern OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // This creates a window object pointer and assigning it to the glfwcreate window output
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    // Error checking
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // This tells opengl what window we are working with
    glfwMakeContextCurrent(window);

    // opengl calls this to adjust the window size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // This function checks for GLAD errors
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // This is an unsigned int that holds the reference number (ID) for a shader object created by OpenGL.
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // This function takes in our vertexShader number and vertex shader code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //This compiles the shader into GPU code
    glCompileShader(vertexShader);

    // This int is to hold the value of a returned OpenGL error code
    int success;
    // This is a "String" in C as the OpenGL functions use C
    char infoLog[512];
    // This function loggs the compile status of the vertexShader, as specified in the second peramiter parameter
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // This checks to see if success is equal to 0
    if (!success)
    {
        // This gets the info log from open gl
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        // This prints the error log from the vertex shader
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //This is providing a reference number of the newly made fragment shader object
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //This provides openGL with the fragment shader code
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // This compiles the fragment shader into GPU code
    glCompileShader(fragmentShader);
    //This gets the compile status and success numnber for the fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //This checks if Success is equal to 0
    if (!success)
    {
        //This stores the fragment shader infoLog information
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        //Logs the information
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //This is an unsigned int that is provided a refrence number to a created program
    unsigned int shaderProgram = glCreateProgram();
    //This stages the already compiled vertex shader on to the GPU
    glAttachShader(shaderProgram, vertexShader);
    //This stages the already compiled fragment shader on to the GPU
    glAttachShader(shaderProgram, fragmentShader);
    //This links together all of the shader prgrams that are compiled on the GPU
    glLinkProgram(shaderProgram);
    //This gets the compile status of the shader program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //Checks success is equal to 0
    if (!success) {
        //Gets the program log info 
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        //This ouputs the log information to the console
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //This is cleanup for the vertex and fragment shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //This is an array of floats called vertices to draw our rectangle
    // ------------------------------------------------------------------
    float vertices[] = {     // X, Y, Z
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    //This is an array of unsigned ints called indices that I am going to struggle on XD
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    //Unsigned ints for the VBO, VAO, EBO;
    unsigned int VBO, VAO, EBO;

    //This is generating one vertex array object and storing its ID in VAO
    glGenVertexArrays(1, &VAO);
    //This generates a buffer object with memory we can work with on the gpu and stores its ID in VBO
    glGenBuffers(1, &VBO);
    //This generates a buffer object with memory we can work with on the gpu and stores its ID in EBO
    glGenBuffers(1, &EBO);

    //This function by "Binding the vertexarray" it basically activates the VAO making it the active current one (openGL is a state machine)
    glBindVertexArray(VAO);

    //This binds the buffer, basically enabling the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //??????????
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // -----
        processInput(window);

        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}