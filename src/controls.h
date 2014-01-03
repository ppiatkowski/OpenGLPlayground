#ifndef _CONTROLS_H_
#define _CONTROLS_H_

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 700;

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif // _CONTROLS_H_