#include "camera.hpp"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

void init_camera(Camera *camera){
    camera->pos = glm::vec3(0.0f, 0.0f, 3.0f);
    camera->dir = glm::vec3(0.0f, 0.0f, -1.0f);
    camera->up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 calculate_view_matrix(Camera *camera){
    return glm::lookAt(camera->pos, camera->pos + camera->dir, camera->up);
}
