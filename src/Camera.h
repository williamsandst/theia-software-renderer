#pragma once
#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#include "Eigen/Dense"

using namespace Eigen;

class Camera
{
private:
	Vector3f rotation = Vector3f(0, 0, 0);
	Vector3f translation = Vector3f(0, 0, 0);
	Affine3f createRotationMatrix(Vector3f r);

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	int height, width;

	float scale = 1;

	Matrix4f viewMatrix;
	Matrix4f projectionMatrix;
	Matrix4f viewPortMatrix;

	void calculateViewMatrix();
	void calculateProjectionMatrix();
	void updateMatrices();
	void setTranslation(Vector3f translation);
	void setRotation(Vector3f rotation);
	void setScale(float scale);
	void setViewport(int width, int height);

	void move(float x, float y, float z);
	void rotate(float xr, float yr, float zr);
	void changeScale(float x);

	Camera();
	~Camera();
};

