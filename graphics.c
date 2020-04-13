#include "graphics.h"

unsigned int asteroid_shader_program, bullet_shader_program, dust_shader_program, crosshair_shader_program;

unsigned int depth_map_fbo;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depth_map;

int screen_width, screen_height;

void asteroid_model_matrix (asteroid_t* asteroid, mat4 matrix) {
    glm_mat4_identity(matrix);
    glm_translate(matrix, asteroid->location);
    glm_rotate(matrix, asteroid->angle, asteroid->axis);
}

void bullet_model_matrix(bullet_t* bullet, mat4 matrix) {
    glm_mat4_identity(matrix);
    glm_translate(matrix, bullet->location);
}

void render_objects_with_shadow(world_t *world, mat4 view_matrix, mat4 projection_matrix) {
    // Set some world members to local variables for easier access
    asteroid_list_t* asteroids = world->asteroids;
    ship_t *ship = world->ship;
    bool running = world->running;

    GLuint vbo;
    GLuint nbo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);

    glUseProgram(asteroid_shader_program);

    unsigned int model_matrix_loc = glGetUniformLocation(asteroid_shader_program, "model_matrix");
    unsigned int view_matrix_loc = glGetUniformLocation(asteroid_shader_program, "view_matrix");
    unsigned int projection_matrix_loc = glGetUniformLocation(asteroid_shader_program, "projection_matrix");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, view_matrix[0]);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, projection_matrix[0]);

    mat4 model_matrix;
    glm_mat4_identity(model_matrix);

    // Rotate ship in xz-plane
    float angle;
    vec3 axis;
    glm_vec3_cross(ship->pointing_direction, (vec3) {0.0f, 1.0f, 0.0f}, axis);
    angle = glm_vec3_angle(ship->pointing_direction, (vec3) {ship->pointing_direction[0], 0.0f, ship->pointing_direction[2]});
    if (ship->pointing_direction[1] < 0.0f)
        angle *= -1;
    glm_rotate(model_matrix, angle, axis);

    // Rotate ship y-component
    angle = glm_vec3_angle((vec3) {ship->pointing_direction[0], 0.0f, ship->pointing_direction[2]}, (vec3) {0.0f, 0.0f, -1.0f});
    if (ship->pointing_direction[0] < 0.0f)
        angle *= -1;
    glm_rotate(model_matrix, angle, (vec3) {0.0f, -1.0f, 0.0f});


    if (running) {
        // Draw ship
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*18, ship->vertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, nbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*18, ship->normals, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, model_matrix[0]);
        glDrawArrays(GL_TRIANGLES, 0, 18);
    }

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
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &nbo);
}

void render_objects_without_shadow(world_t *world, mat4 view_matrix, mat4 projection_matrix) {
    // Set some world members to local variables for easier access
    bullet_list_t *bullets = world->bullets;
    int score = world->score;
    bool running = world->running;

    GLuint vbo;
    GLuint nbo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);

    // Draw bullets
    glDisableVertexAttribArray(1);

    glUseProgram(bullet_shader_program);

    unsigned int model_matrix_loc = glGetUniformLocation(bullet_shader_program, "model_matrix");
    unsigned int view_matrix_loc = glGetUniformLocation(bullet_shader_program, "view_matrix");
    unsigned int projection_matrix_loc = glGetUniformLocation(bullet_shader_program, "projection_matrix");

    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, view_matrix[0]);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, projection_matrix[0]);

    mat4 model_matrix;

    bullet_list_t *bullets_head = bullets;
    while(bullets_head->next != NULL) {
        bullet_t *bullet = bullets_head->this;

        bullet_model_matrix(bullet, model_matrix);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*2, bullet->vertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, model_matrix[0]);
        glDrawArrays(GL_LINES, 0, 2);

        bullets_head = bullets_head->next;
    }

    // Draw dust
    glUseProgram(dust_shader_program);
    view_matrix_loc = glGetUniformLocation(dust_shader_program, "view_matrix");
    projection_matrix_loc = glGetUniformLocation(dust_shader_program, "projection_matrix");
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, view_matrix[0]);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, projection_matrix[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*world->dust_cloud->vertices_length, world->dust_cloud->vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_POINTS, 0, world->dust_cloud->vertices_length);

    // Draw crosshair
    glUseProgram(crosshair_shader_program);
    vec3 crosshair_vertices[4] = {{0.025f, 0.0f, 0.0f},
                                  {-0.025f, 0.0f, 0.0f},
                                  {0.0f, 0.025f, 0.0f},
                                  {0.0f, -0.025f, 0.0f}};
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*4, crosshair_vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 4);

    glDisableVertexAttribArray(0);

    // Draw score and possibly game over
    gltInit();
    GLTtext *text = gltCreateText();
    char string[64];
    if (running)
        sprintf(string, "Score: %i\n", score);
    else
        sprintf(string, "Score: %i\nGame over", score);
    gltSetText(text, string);
    gltBeginDraw();

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(text, 0, 0, 1.0f);
    gltEndDraw();
    gltTerminate();

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &nbo);
}

void get_ship_perspective(world_t *world, mat4 view_matrix, mat4 projection_matrix) {
    // Set some world members to local variables for easier access
    ship_t *ship = world->ship;

    // Ship model and view direction
    vec3 eye_dir;

    // Set view direction to ship direction
    glm_vec3_copy(ship->pointing_direction, eye_dir);

    // Set camera to behind and above ship
    vec3 eye;
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_vec3_scale(ship->pointing_direction, -16.0f, eye);
    eye[1] += 8.0f;
    glm_look(eye, eye_dir, up, view_matrix);

    // Perspective matrix
    glm_perspective(3.14159265358979323f/2.0f, 16.0f/9.0f, 1.0f, 100000.0f, projection_matrix);
}

void get_sun_perspective(world_t *world, mat4 view_matrix, mat4 projection_matrix) {
    vec3 eye = {100000.0, 25000.0, 0.0};
    vec3 eye_dir;
    glm_vec3_scale(eye, -1.0f, eye_dir);
    glm_vec3_normalize(eye_dir);
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_look(eye, eye_dir, up, view_matrix);

    // Perspective matrix
    glm_perspective(3.14159265358979323f/128.0f, 1.0f, 1000.0f, 250000.0f, projection_matrix);
}


void setup_shadows() {
    glGenFramebuffers(1, &depth_map_fbo);
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH,
                 SHADOW_HEIGHT,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void render(GLFWwindow *window, world_t *world) {
    mat4 view_matrix;
    mat4 projection_matrix;

    // Compute shadows
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    get_sun_perspective(world, view_matrix, projection_matrix);
    render_objects_with_shadow(world, view_matrix, projection_matrix);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Render scene
    glViewport(0, 0, screen_width, screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    get_ship_perspective(world, view_matrix, projection_matrix);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    render_objects_with_shadow(world, view_matrix, projection_matrix);
    render_objects_without_shadow(world, view_matrix, projection_matrix);

    glfwSwapBuffers(window);
}

unsigned int compile_shader(char *shader_path, int shader_type) {
    int length;

    // Read shader from file
    char *shader_source;
    FILE *shader_file = fopen(shader_path, "rb");
    if (shader_file) {
        fseek(shader_file, 0, SEEK_END);
        length = (int) ftell(shader_file);
        fseek (shader_file, 0, SEEK_SET);
        shader_source = malloc (length);
        fread(shader_source, 1, length, shader_file);
        fclose(shader_file);
    } else {
        fprintf(stderr, "Could not open vertex shader file\n");
        exit(1);
    }
    const char *shader_content = shader_source;

    // Compile shader
    unsigned int shader;
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_content, &length);
    glCompileShader(shader);
    int  success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr,"Vertex shader compilation error: %s\n", info_log);
    }
    return shader;
}

void add_shader_program(char *vertex_shader_path,
                        char *fragment_shader_path,
                        unsigned int *shader_program_ptr) {
    // Compile shaders
    unsigned int vertex_shader = compile_shader(vertex_shader_path,
                                                GL_VERTEX_SHADER);
    unsigned int fragment_shader = compile_shader(fragment_shader_path,
                                                  GL_FRAGMENT_SHADER);

    // Link shader program
    *shader_program_ptr = glCreateProgram();
    glAttachShader(*shader_program_ptr, vertex_shader);
    glAttachShader(*shader_program_ptr, fragment_shader);
    glLinkProgram(*shader_program_ptr);
}

void resize_framebuffer(GLFWwindow* window, int width, int height) {
    screen_width = width;
    screen_height = height;
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
    glEnable(GL_DEPTH_TEST);

    add_shader_program(ASTEROID_VERTEX_SHADER_PATH,
                       ASTEROID_FRAGMENT_SHADER_PATH,
                       &asteroid_shader_program);
    add_shader_program(BULLET_VERTEX_SHADER_PATH,
                       BULLET_FRAGMENT_SHADER_PATH,
                       &bullet_shader_program);
    add_shader_program(DUST_VERTEX_SHADER_PATH,
                       DUST_FRAGMENT_SHADER_PATH,
                       &dust_shader_program);
    add_shader_program(CROSSHAIR_VERTEX_SHADER_PATH,
                       CROSSHAIR_FRAGMENT_SHADER_PATH,
                       &crosshair_shader_program);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    setup_shadows();

    return 0;
}
