#include "graphics.h"

unsigned int asteroid_shader_program, bullet_shader_program;

void asteroid_model_matrix (asteroid_t* asteroid, mat4 matrix) {
    glm_mat4_identity(matrix);
    glm_translate(matrix, asteroid->location);
    glm_rotate(matrix, asteroid->angle, asteroid->axis);
}

void bullet_model_matrix(bullet_t* bullet, mat4 matrix) {
    glm_mat4_identity(matrix);
    glm_translate(matrix, bullet->location);
}

void render(GLFWwindow *window, asteroid_list_t* asteroids, bullet_list_t *bullets, ship_t *ship) {
    vec3 eye_dir;
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;

    glm_mat4_identity(model_matrix);

    float angle;
    vec3 axis;

    glm_vec3_cross(ship->direction, (vec3) {0.0f, 1.0f, 0.0f}, axis);
    angle = glm_vec3_angle(ship->direction, (vec3) {ship->direction[0], 0.0f, ship->direction[2]});
    if (ship->direction[1] < 0.0f) {
        angle *= -1;
    }
    glm_rotate(model_matrix, angle, axis);

    angle = glm_vec3_angle((vec3) {ship->direction[0], 0.0f, ship->direction[2]}, (vec3) {0.0f, 0.0f, -1.0f});
    if (ship->direction[0] < 0.0f) {
        angle *= -1;
    }
    glm_rotate(model_matrix, angle, (vec3) {0.0f, -1.0f, 0.0f});
    glm_vec3_copy(ship->direction, eye_dir);
    glm_vec3_scale(ship->direction, -16.0f, camera_location);
    camera_location[1] += 8.0f;
    vec3 eye = {camera_location[0], camera_location[1], camera_location[2]};
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_look(eye, eye_dir, up, view_matrix);

    glm_perspective(3.14159265358979323f/2.0f, 16.0f/9.0f, 1.0f, 100000.0f, projection_matrix);


    GLuint vbo;
    GLuint nbo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(asteroid_shader_program);

    unsigned int model_matrix_loc = glGetUniformLocation(asteroid_shader_program, "model_matrix");
    unsigned int view_matrix_loc = glGetUniformLocation(asteroid_shader_program, "view_matrix");
    unsigned int projection_matrix_loc = glGetUniformLocation(asteroid_shader_program, "projection_matrix");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, view_matrix[0]);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, projection_matrix[0]);

    // Draw ship
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*18, ship->vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*18, ship->normals, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, model_matrix[0]);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    // Draw asteroids
    asteroid_list_t *asteroids_head = asteroids;
    while (asteroids_head->this != NULL) {
        asteroid_t *asteroid = asteroids_head->this;

        asteroid_model_matrix(asteroids_head->this, model_matrix);

        glBindBuffer(GL_ARRAY_BUFFER, asteroid->vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, asteroid->nbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, model_matrix[0]);
        glDrawArrays(GL_TRIANGLES, 0, asteroid->vertices_length);

        asteroids_head = asteroids_head->next;
    }

    // Draw bullets
    glDisableVertexAttribArray(1);

    glUseProgram(bullet_shader_program);

    model_matrix_loc = glGetUniformLocation(bullet_shader_program, "model_matrix");
    view_matrix_loc = glGetUniformLocation(bullet_shader_program, "view_matrix");
    projection_matrix_loc = glGetUniformLocation(bullet_shader_program, "projection_matrix");

    bullet_list_t *bullets_head = bullets;
    while(bullets_head->next != NULL) {
        bullet_t *bullet = bullets_head->this;

        bullet_model_matrix(bullet, model_matrix);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*2, bullet->vertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, model_matrix[0]);
        glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, view_matrix[0]);
        glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, projection_matrix[0]);
        glDrawArrays(GL_LINES, 0, 2);

        bullets_head = bullets_head->next;
    }

    glDisableVertexAttribArray(0);
    glfwSwapBuffers(window);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &nbo);
}

void add_shaders() {
    int length;

    // Read vertex shader from file
    char *vertex_shader_source;
    FILE *vertex_shader_file = fopen(ASTEROID_VERTEX_SHADER_PATH, "rb");
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
    FILE *fragment_shader_file = fopen(ASTEROID_FRAGMENT_SHADER_PATH, "rb");
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
    asteroid_shader_program = glCreateProgram();
    glAttachShader(asteroid_shader_program, vertex_shader);
    glAttachShader(asteroid_shader_program, fragment_shader);
    glLinkProgram(asteroid_shader_program);

    if (!success) {
        glGetProgramInfoLog(asteroid_shader_program, 512, NULL, info_log);
        fprintf(stderr,"Shader program error: %s\n", info_log);
    }

    // Read vertex shader from file
    vertex_shader_file = fopen(BULLET_VERTEX_SHADER_PATH, "rb");
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
    vertex_shader_content = vertex_shader_source;

    // Compile vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_content, &length);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr,"Vertex shader compilation error: %s\n", info_log);
    }

    // Load fragment shader from file
    fragment_shader_file = fopen(BULLET_FRAGMENT_SHADER_PATH, "rb");
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
    fragment_shader_content = fragment_shader_source;

    // Compile fragment_shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_content, &length);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(stderr,"Fragment shader compilation error: %s\n", info_log);
    }

    // Link shader program
    bullet_shader_program = glCreateProgram();
    glAttachShader(bullet_shader_program, vertex_shader);
    glAttachShader(bullet_shader_program, fragment_shader);
    glLinkProgram(bullet_shader_program);

    if (!success) {
        glGetProgramInfoLog(bullet_shader_program, 512, NULL, info_log);
        fprintf(stderr,"Shader program error: %s\n", info_log);
    }

    glEnable(GL_DEPTH_TEST);
}


void resize_framebuffer(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int intialize_window(GLFWwindow **window) {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW failed to init\n");
        return 1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
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

    glEnable(GL_MULTISAMPLE);

    add_shaders();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return 0;
}
