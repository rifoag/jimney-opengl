#ifndef JIMNEY_VERTEX
#define JIMNEY_VERTEX

float vertices[] = {
    // points             colors         texture coords
    0.92,   0.41,   0.00, 1.0, 1.0, 1.0, 0.0, 1.0,  // alas top right 0
    0.92,   -0.41,  0.00, 1.0, 1.0, 1.0, 1.0, 0.0,  // alas bottom right
    -0.92,  -0.41,  0.00, 1.0, 1.0, 1.0, 0.0, 1.0,  // alas bottom left
    -0.92,  0.41,   0.00, 1.0, 1.0, 1.0, 0.0, 0.0,  // alas top left
    0.92,   0.41,   0.25, 1.0, 1.0, 1.0, 1.0, 1.0,  // tengah top right 5     
    0.92,   -0.41,  0.25, 1.0, 1.0, 1.0, 1.0, 0.0,  // tengah top left
    -0.92,  -0.41,  0.25, 1.0, 1.0, 1.0, 0.2, 1.0,  // tengah bottom left      
    -0.92,  0.41,   0.25, 1.0, 1.0, 1.0, 0.2, 0.0,  // tengah bottom right
    0.00,   0.41,   0.25, 1.0, 1.0, 1.0, 0.5, 0.0,  // tengah middle right 9
    0.00,   -0.41,  0.25, 1.0, 1.0, 1.0, 0.5, 1.0,  // tengah middle left
    0.92,   0.41,   0.5,  1.0, 1.0, 1.0, 0.7, 0.0,  // atas top right
    0.92,   -0.41,  0.5,  1.0, 1.0, 1.0, 1.0, 0.0,  // atas top left
    0.00,   -0.41,  0.5,  1.0, 1.0, 1.0, 1.0, 1.0,  // atas bottom left
    0.00,   0.41,   0.5,  1.0, 1.0, 1.0, 0.7, 1.0,  // atas bottom right
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
    0, 3, 4,
    3, 4, 7,
    2, 3, 6,
    3, 6, 7,
    1, 2, 6,
    2, 5, 6,
    0, 1, 5,
    0, 4, 5,
    4, 5, 6,
    4, 6, 7,
    4, 10, 13,
    4, 8, 13,
    8, 12, 13,
    8, 9, 12,
    9, 11, 12,
    5, 9, 11,
    4, 5, 10,
    5, 10, 11,
    10, 11, 12,
    10, 12, 13,
    1, 2, 5
};

float light_sources[] = {
    1.0, 1.0, 1.0,
    1.0, 0.95, 1.0,
    0.95, 0.95, 1.0,
    0.95, 1.0, 1.0
};

unsigned int light_sources_indices[] = {
    0, 1, 2,
    0, 3, 2
};

#endif
