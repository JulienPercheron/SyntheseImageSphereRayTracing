#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>



using namespace cv;
using namespace std;

struct Ray {
	Vec3d origin;
	Vec3d direction;
};

struct Sphere {
	Vec3d origin;
	double rayon;
	Vec3d albedo;

	float intersect(const Ray& r) const
	{                // returns distance, 0 if nohit
		Vec3d op = origin - r.origin;        // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
		float t, eps = 1e-4, b = op.dot(r.direction), det =
			b * b - op.dot(op) + rayon * rayon;
		if (det < 0)
			return 0;
		else
			det = sqrt(det);
		return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
	}
};

struct CubeEnglobant {
	Vec3d min;
	Vec3d max;

	vector<Sphere> listSpheres;

	bool intersect(const Ray& r) const
	{
		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		Vec3d invdir = { 1 / r.direction[0], 1 / r.direction[1], 1 / r.direction[2] };

		if (invdir[0] >= 0) {
			tmin = (min[0] - r.origin[0]) * invdir[0];
			tmax = (max[0] - r.origin[0]) * invdir[0];
		}
		else {
			tmin = (max[0] - r.origin[0]) * invdir[0];
			tmax = (min[0] - r.origin[0]) * invdir[0];
		}
		if (invdir[1] >= 0) {
			tymin = (min[1] - r.origin[1]) * invdir[1];
			tymax = (max[1] - r.origin[1]) * invdir[1];
		}
		else {
			tymin = (max[1] - r.origin[1]) * invdir[1];
			tymax = (min[1] - r.origin[1]) * invdir[1];
		}
		if (invdir[2] >= 0) {
			tzmin = (min[2] - r.origin[2]) * invdir[2];
			tzmax = (max[2] - r.origin[2]) * invdir[2];
		}
		else {
			tzmin = (max[2] - r.origin[2]) * invdir[2];
			tzmax = (min[2] - r.origin[2]) * invdir[2];
		}
		
		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		
		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}

	void spheresInCube(vector<Sphere> spheres) {
		double maxX = max[0];
		double maxY = max[1];
		double maxZ = max[2];

		double minX = min[0];
		double minY = min[1];
		double minZ = min[2];
		for (Sphere& s : spheres) {
			if (s.origin[0] > min[0] && s.origin[0] < max[0] && s.origin[1] > min[1] && s.origin[1] < max[1] && s.origin[2] > min[2] && s.origin[2] < max[2]) {
				listSpheres.push_back(s);

				if (s.origin[0] + s.rayon > maxX)
					maxX = s.origin[0] + s.rayon;
				if (s.origin[1] + s.rayon > maxY)
					maxY = s.origin[1] + s.rayon;
				if (s.origin[2] + s.rayon > maxZ)
					maxZ = s.origin[2] + s.rayon;
				
				if (s.origin[0] - s.rayon < minX)
					minX = s.origin[0] - s.rayon;
				if (s.origin[1] - s.rayon < minY)
					minY = s.origin[1] - s.rayon;
				if (s.origin[2] - s.rayon < minZ)
					minZ = s.origin[2] - s.rayon;

			}
		}

		min[0] = minX;
		min[1] = minY;
		min[2] = minZ;

		max[0] = maxX;
		max[1] = maxY;
		max[2] = maxZ;
	}


};


struct LightSource {
	Vec3d origin;

	float color;
	

};


int main()
{
	const int imgWidth = 1000;
	const int imgHeight = 1000;

	const float PI = 3.141592;

	//Modifier seconde coordonnée pour modifier l'angle de vue
	const Vec3d origine = { 500,-500, 500 };

	
	Mat image = Mat::zeros(imgWidth, imgHeight, CV_8UC3);


	float sphereDistance = 255;
	float tailleSphere = 40;

	Sphere sphere11{ Vec3d{100, sphereDistance, 100}, tailleSphere, {0,1,0} };
	Sphere sphere12{ Vec3d{200, sphereDistance, 100}, tailleSphere , {0,1,1} };
	Sphere sphere13{ Vec3d{300, sphereDistance, 100}, tailleSphere, {1,1,0} };
	Sphere sphere14{ Vec3d{400, sphereDistance, 100}, tailleSphere, {0,1,0} };
	Sphere sphere15{ Vec3d{500, sphereDistance, 100}, tailleSphere , {0,1,1} };
	Sphere sphere16{ Vec3d{600, sphereDistance, 100}, tailleSphere, {1,1,0} };
	Sphere sphere17{ Vec3d{700, sphereDistance, 100}, tailleSphere, {0,1,0} };
	Sphere sphere18{ Vec3d{800, sphereDistance, 100}, tailleSphere , {0,1,1} };
	Sphere sphere19{ Vec3d{900, sphereDistance, 100}, tailleSphere, {1,1,0} };
	Sphere sphere21{ Vec3d{100, sphereDistance, 200}, tailleSphere, {0,1,0} };
	Sphere sphere22{ Vec3d{200, sphereDistance, 200}, tailleSphere , {0,1,1} };
	Sphere sphere23{ Vec3d{300, sphereDistance, 200}, tailleSphere, {1,1,0} };
	Sphere sphere24{ Vec3d{400, sphereDistance, 200}, tailleSphere, {0,1,0} };
	Sphere sphere25{ Vec3d{500, sphereDistance, 200}, tailleSphere , {0,1,1} };
	Sphere sphere26{ Vec3d{600, sphereDistance, 200}, tailleSphere, {1,1,0} };
	Sphere sphere27{ Vec3d{700, sphereDistance, 200}, tailleSphere, {0,1,0} };
	Sphere sphere28{ Vec3d{800, sphereDistance, 200}, tailleSphere , {0,1,1} };
	Sphere sphere29{ Vec3d{900, sphereDistance, 200}, tailleSphere, {1,1,0} };
	Sphere sphere31{ Vec3d{100, sphereDistance, 300}, tailleSphere, {0,1,0} };
	Sphere sphere32{ Vec3d{200, sphereDistance, 300}, tailleSphere , {0,1,1} };
	Sphere sphere33{ Vec3d{300, sphereDistance, 300}, tailleSphere, {1,1,0} };
	Sphere sphere34{ Vec3d{400, sphereDistance, 300}, tailleSphere, {0,1,0} };
	Sphere sphere35{ Vec3d{500, sphereDistance, 300}, tailleSphere , {0,1,1} };
	Sphere sphere36{ Vec3d{600, sphereDistance, 300}, tailleSphere, {1,1,0} };
	Sphere sphere37{ Vec3d{700, sphereDistance, 300}, tailleSphere, {0,1,0} };
	Sphere sphere38{ Vec3d{800, sphereDistance, 300}, tailleSphere , {0,1,1} };
	Sphere sphere39{ Vec3d{900, sphereDistance, 300}, tailleSphere, {1,1,0} };
	Sphere sphere91{ Vec3d{100, sphereDistance, 900}, tailleSphere, {0,1,0} };
	Sphere sphere92{ Vec3d{200, sphereDistance, 900}, tailleSphere , {0,1,1} };
	Sphere sphere93{ Vec3d{300, sphereDistance, 900}, tailleSphere, {1,1,0} };
	Sphere sphere94{ Vec3d{400, sphereDistance, 900}, tailleSphere, {0,1,0} };
	Sphere sphere95{ Vec3d{500, sphereDistance, 900}, tailleSphere , {0,1,1} };
	Sphere sphere96{ Vec3d{600, sphereDistance, 900}, tailleSphere, {1,1,0} };
	Sphere sphere97{ Vec3d{700, sphereDistance, 900}, tailleSphere, {0,1,0} };
	Sphere sphere98{ Vec3d{800, sphereDistance, 900}, tailleSphere , {0,1,1} };
	Sphere sphere99{ Vec3d{900, sphereDistance, 900}, tailleSphere, {1,1,0} };


	//vector<Sphere> spheres = { sphere, sphere2, murGauche, murDroite, murHaut, murBas, murFond };
	vector<Sphere> spheres = { 
		sphere11, sphere12, sphere13, sphere14, sphere15, sphere16, sphere17, sphere18, sphere19,
		sphere21, sphere22, sphere23, sphere24, sphere25, sphere26, sphere27, sphere28, sphere29,
		sphere31, sphere32, sphere33, sphere34, sphere35, sphere36, sphere37, sphere38, sphere39,
		sphere91, sphere92, sphere93, sphere94, sphere95, sphere96, sphere97, sphere98, sphere99,
	};

	LightSource lightSource{ Vec3d{500, sphereDistance - 200, 300}, 70000000 };
	
	CubeEnglobant initialCube = {Vec3d{0, 1 ,0}, Vec3d{imgWidth, sphereDistance + 20, imgHeight} };

	vector<CubeEnglobant> cubes = { initialCube };

	vector<CubeEnglobant> tempCubes;
	for (int indice = 0; indice < 2; ++indice) {
		tempCubes.clear();
		for (CubeEnglobant& cube : cubes) {
			if (cube.max[0] - cube.min[0] > cube.max[1] - cube.min[1] && cube.max[0] - cube.min[0] > cube.max[2] - cube.min[2]) {
				CubeEnglobant cube1 = { Vec3d{cube.min[0], cube.min[1], cube.min[2]}, Vec3d{cube.max[0]/2, cube.max[1] ,cube.max[2]} };
				CubeEnglobant cube2 = { Vec3d{cube.max[0]/2, cube.min[1], cube.min[2]}, Vec3d{cube.max[0], cube.max[1] ,cube.max[2]} };
				 
				tempCubes.push_back(cube1);
				tempCubes.push_back(cube2);
			}
			else if (cube.max[1] - cube.min[1] > cube.max[0] - cube.min[0] && cube.max[1] - cube.min[1] > cube.max[2] - cube.min[2]){
				CubeEnglobant cube1 = { Vec3d{cube.min[0], cube.min[1], cube.min[2]}, Vec3d{cube.max[0], cube.max[1] / 2 ,cube.max[2]} };
				CubeEnglobant cube2 = { Vec3d{cube.min[0], cube.max[1] / 2, cube.min[2]}, Vec3d{cube.max[0], cube.max[1] ,cube.max[2]} };

				tempCubes.push_back(cube1);
				tempCubes.push_back(cube2);
			} 
			else {
				CubeEnglobant cube1 = { Vec3d{cube.min[0], cube.min[1], cube.min[2]}, Vec3d{cube.max[0], cube.max[1] ,cube.max[2]/2} };
				CubeEnglobant cube2 = { Vec3d{cube.min[0], cube.min[1], cube.max[2] / 2}, Vec3d{cube.max[0], cube.max[1] ,cube.max[2]} };

				tempCubes.push_back(cube1);
				tempCubes.push_back(cube2);
			}
		}
		cubes = tempCubes;
	}

	bool sphereFound = false;
	for (int i = 0; i < imgWidth; ++i) {
		for (int j = 0; j < imgHeight; ++j) {

			//Vec3d rayDirection = Vec3d{ (float)i, 0, (float)j }-origine;
			//double normRayDirection = cv::norm(rayDirection, NORM_L2);
			//rayDirection = rayDirection / normRayDirection;

			Vec3d rayDirection = Vec3d{ 0, 1, 0 };

			Ray ray{ Vec3d{(float)i, 0, (float)j}, rayDirection };

			
			for (CubeEnglobant cube : cubes) {
				if (cube.intersect(ray)) {
					for (Sphere& s : spheres) {
						if (sphereFound)
							break;

						float res = s.intersect(ray);


						if (res > 0) {
							Vec3d X = ray.origin + res * ray.direction;
							Vec3d directionLampe = lightSource.origin - X;
							double d2 = pow(directionLampe[0], 2) + pow(directionLampe[1], 2) + pow(directionLampe[2], 2);
							Vec3d w0 = directionLampe / sqrt(d2);



							Vec3d normale = X - s.origin;
							double normNormale = cv::norm(normale, NORM_L2);
							normale = normale / normNormale;
							double f = abs(w0.dot(normale) / PI);

							//cout << w0 << endl;
							Ray rayFromIntersec{ X, w0 };
							double smallestDist = -1;

							for (Sphere& s2 : spheres) {
								double dist = s2.intersect(rayFromIntersec);
								if (smallestDist == -1 && dist > 0)
									smallestDist = dist;
								else if (smallestDist > dist && dist > 0)
									smallestDist = dist;
							}

							float isClear = 1;
							//cout << abs(pow(smallestDist, 2) - d2)  << endl;
							if (smallestDist != -1) {
								if (pow(smallestDist, 2) > d2)
									isClear = 1;
								else
									isClear = 0;
							}


							float couleur = (lightSource.color * f * isClear) / d2;

							if (couleur > 255) {
								couleur = 255;
							}
							else if (couleur < 0) {
								couleur = 0;
							}

							image.at<Vec3b>({ i,j }) = couleur * s.albedo;
							sphereFound = true;
						}
						else {
							image.at<Vec3b>({ i,j }) = { 150,150,150 };
						}


					}

				}
				sphereFound = false;
			}
		}
	}
	
	cv::imshow("Display  Window", image);
	cv::imwrite("sphere.jpg", image);
	cv::waitKey(0);

	//Mat image = Mat::zeros(300, 600, CV_8UC3);
	//circle(image, Point(250, 150), 100, Scalar(0, 255, 128), -100);
	//circle(image, Point(350, 150), 100, Scalar(255, 255, 255), -100);


	//for (int r = 0; r < image.rows; ++r) {
	//	for (int c = 0; c < image.cols; ++c) {
	//		if()
	//		Vec3b& color = image.at<Vec3b>(r, c);
	//	}
	//}

	//imshow("Display Window", image);
	//imwrite("test.jpg", image);
	//waitKey(0);

	return 0;
}