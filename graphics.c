#include "graphics.h"

int asteroids_length;
int vertices_length;
vector3f_t *vertices;
int model_matrices_length;
mat4 *model_matrices;
unsigned int shader_program;

void render(GLFWwindow *window) {
    mat4 view_matrix;
    mat4 projection_matrix;

    vec3 eye = {camera_location.x, camera_location.y, camera_location.z};
    vec3 dir = {0.0f, 0.0f, 1.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_look(eye, dir, up, view_matrix);

    glm_perspective(3.14159265358979323f/4.0f, 16.0f/9.0f, 0.1f, 100.0f, projection_matrix);

    glClear(GL_COLOR_BUFFER_BIT);

    GLuint *vbos = malloc(sizeof(GLuint)*asteroids_length);
    glGenBuffers(asteroids_length, vbos);

    unsigned int model_matrix_loc = glGetUniformLocation(shader_program, "model_matrix");
    unsigned int view_matrix_loc = glGetUniformLocation(shader_program, "view_matrix");
    unsigned int projection_matrix_loc = glGetUniformLocation(shader_program, "projection_matrix");

    glEnableVertexAttribArray(0);
    for (int i = 0; i < asteroids_length; i++) {
        #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vector3f_t)*12, &(vertices[i*12]), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, model_matrices[i][0]);
        glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, view_matrix[0]);
        glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, projection_matrix[0]);
        glDrawArrays(GL_TRIANGLES, 0, vertices_length);
    }

    glDisableVertexAttribArray(0);
    glfwSwapBuffers(window);
    glDeleteBuffers(asteroids_length, vbos);
}

void asteroid_model_matrix (asteroid_t* asteroid, mat4 matrix) {
    glm_mat4_identity(matrix);
    glm_translate(matrix, (vec3) {asteroid->x, asteroid->y, asteroid->z});
}

void collect_vertices(asteroid_list_t* asteroids) {
    asteroids_length = 0;
    asteroid_list_t *asteroids_head = asteroids;
    while (asteroids_head->next != NULL){
        asteroids_length++;
        asteroids_head = asteroids_head->next;
    }

    vertices_length = asteroids_length * 12;
    vertices = malloc(sizeof(vector3f_t)*vertices_length);
    model_matrices = malloc(sizeof(mat4)*asteroids_length);

    asteroids_head = asteroids;
    int v = 0; // Index in vertex array
    int m = 0; // Index in matrix arrrays
    while (asteroids_head->this != NULL) {
        asteroid_t *asteroid = asteroids_head->this;

        vertices[v++] = asteroid->vertices[0];
        vertices[v++] = asteroid->vertices[1];
        vertices[v++] = asteroid->vertices[2];

        vertices[v++] = asteroid->vertices[0];
        vertices[v++] = asteroid->vertices[1];
        vertices[v++] = asteroid->vertices[3];

        vertices[v++] = asteroid->vertices[0];
        vertices[v++] = asteroid->vertices[2];
        vertices[v++] = asteroid->vertices[3];

        vertices[v++] = asteroid->vertices[1];
        vertices[v++] = asteroid->vertices[2];
        vertices[v++] = asteroid->vertices[3];

        asteroid_model_matrix(asteroids_head->this, model_matrices[m++]);

        asteroids_head = asteroids_head->next;
    }
}

void add_shaders() {
    int length;

    // Read vertex shader from file
    char *vertex_shader_source;
    FILE *vertex_shader_file = fopen(VERTEX_SHADER_PATH, "rb");
    if (vertex_shader_file) {
        fseek(vertex_shader_file, 0, SEEK_END);
        length = (int) ftell(vertex_shader_file);
        fseek (vertex_shader_file, 0, SEEK_SET);
        vertex_shader_source = malloc (length);
        fread(vertex_shader_source, 1, length, vertex_shader_file);
        fclose(vertex_shader_file);
    } else {
        fprintf(stderr, "Could not open vertex shader file\n");
        exit(1);
    }
    const char *vertex_shader_content = vertex_shader_source;

    // Compile vertex shader
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_content, &length);
    glCompileShader(vertex_shader);
    int  success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr,"Vertex shader compilation error: %s\n", info_log);
    }

    // Load fragment shader from file
    char *fragment_shader_source;
    FILE *fragment_shader_file = fopen(FRAGMENT_SHADER_PATH, "rb");
    if (fragment_shader_file) {
        fseek (fragment_shader_file, 0, SEEK_END);
        length = (int) ftell(fragment_shader_file);
        fseek (fragment_shader_file, 0, SEEK_SET);
        fragment_shader_source = malloc(length);
        fread(fragment_shader_source, 1, length, vertex_shader_file);
        fclose(fragment_shader_file);
    } else {
        fprintf(stderr, "Could not open fragment shader file\n");
        exit(10);
    }
    const char *fragment_shader_content = fragment_shader_source;

    // Compile fragment_shader
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_content, &length);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(stderr,"Fragment shader compilation error: %s\n", info_log);
    }

    // Link shader program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr,"Shader program error: %s\n", info_log);
    }

    glUseProgram(shader_program);
}


void resize_framebuffer(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int intialize_window(GLFWwindow **window) {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW failed to init\n");
        return 1;
    }

    *window = glfwCreateWindow(1920, 1080, "Particles", NULL, NULL);

    if (!*window) {
        fprintf(stderr, "Window failed to create\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, resize_framebuffer);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(res));
        return 1;
    }

    add_shaders();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return 0;
}
