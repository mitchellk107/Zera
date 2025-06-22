#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

//This function decleration takes in a window object and it adjusts the size of the window 
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
//This functinon decleration processes input
void processInput(GLFWwindow* window);
//Screen resolution, width and height
const int screenWidth = 800;
const int screenHeight = 600;

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

int main() {
    // Setup that inits glfw, tells openGL what version and that we want to use modern OpenGL
    glfwInit();

    //This creates a window object pointer and assigning it to the glfwcreate window output
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Zera", 0, NULL);

      //This checks to see if there was an error in making the opengl window
      if (!window) 
      {
       std::cout << "Hey man your window is messed up" << std::endl;
       return 0;
      }

    //This tells opengl what window we are working with
    glfwMakeContextCurrent(window);
    //Opengl calls this to adjust the window size
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

     //This function checks for GLAD errors
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
       {
       std::cout << "Hey man your glad is messed up" << std::endl;
       return 0;
       }


    // This is an unsigned int vertex shader that holds the reference number (ID) for a shader object created by OpenGL.
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // This function takes in our vertexShader number and vertex shader source code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //This compiles the shader into GPU code
    glCompileShader(vertexShader);

    // This int is to hold the value of a returned OpenGL error code
    int success;
    // This is a "String" in C as the OpenGL functions use C
    char infoLog [512];
    // This function logs the compile status of the vertexShader, as specified in the second parameter
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
    //This stages the already compiled vertex and fragment shaders on to the GPU
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //This links together all of the shader prgrams that are compiled on the GPU
    glLinkProgram(shaderProgram);
    //This gets the compile status of the shader program
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
    //Checks success is equal to 0
    if (!success)
    {   
        //Gets the program log info 
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);

        //This ouputs the log information to the console
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << std::endl;
    }


    //This is cleanup for the vertex and fragment shaders
    glDeleteProgram(fragmentShader);    
    glDeleteProgram(vertexShader);


    //This is an array of floats called vertices to draw our rectangle
    // ------------------------------------------------------------------
    float vertices[]{
        // X, Y, Z
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f  // top left 
    };
    //This is an array of unsigned ints called indices that I am going to struggle on XD
    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
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
    //This tells opengl to allocate memory on the GPU for our vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //This binds the EBO Array buffer object and lets it be modifyable by openGL
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //THis tells opengl to allocate memory on the GPU for the indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //This tells opengl how to interperate the data 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //This assignes the vertex array to 0 so we don't accidentally modify it
    glBindVertexArray(0);
    //This assignes the buffer to 0, so we don't accidently write to it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //This is our main while loop that checks if the the glfw window should close
    // -----------
    while (!glfwWindowShouldClose(window))
    {


        //This processes the input of the current window object
        // -----
        processInput(window);

        //This clears the screen to make a color appear 
        // ------
        glClearColor(0.3f, 0.1f, 0.2f, 1.0f);
        //This fills in the color the previous glclearcolor provided
        glClear(GL_COLOR_BUFFER_BIT);
        //This tells opengl that we want to use the shader program
        glUseProgram(shaderProgram);
        //This binds the vao so we can access the memeory
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //This draws the elements of the 2 triangles
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //This swapps the buffers within the window object
        glfwSwapBuffers(window);
        // -------------------------------------------------------------------------------
        //This preforms and pending poll events
        glfwPollEvents();
    }

    //This deletes the shader program, VAO, VBO, EBO
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    //This terminates glfw
    glfwTerminate();
    // ------------------------------------------------------------------
    //returns 0
return 0;
}


//This function is for processing the input of our glfw window object
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    //This is the if statment for the glfwgetkey and checks for escape
    if (GLFW_KEY_ESCAPE)
    {
        //This closes the window
        glfwWindowShouldClose(window);
    }   
}
    


//This function framebuffer_size_callbeack is responsible for taking the window, width and height// ---------------------------------------------------------------------------------------------
    void frameBufferSizeCallback(GLFWwindow* window, int width, int height) 
    {
        glViewport(0,0, width, height);
    }
