#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>


#define CAMERA_SPEED 0.20f
#define CAMERA_ROTATION 0.10f

struct Camera{
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;
};

void init_camera(Camera *camera);

glm::mat4 calculate_view_matrix(Camera *camera);
