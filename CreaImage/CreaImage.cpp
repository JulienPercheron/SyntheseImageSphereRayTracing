#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>

#define PI 3.141592


using namespace cv;
using namespace std;

struct Ray {
	Vec3d origin;
	Vec3d direction;
};

struct Sphere {
	Vec3d origin;
	int rayon;
	Vec3d albedo;

	float intersect(const Ray& r) const
	{                // returns distance, 0 if nohit
		Vec3f op = origin - r.origin;        // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
		float t, eps = 1e-4, b = op.dot(r.direction), det =
			b * b - op.dot(op) + rayon * rayon;
		if (det < 0)
			return 0;
		else
			det = sqrt(det);
		return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
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

	
	Mat image = Mat::zeros(imgWidth, imgHeight, CV_8UC3);

	float sphereDistance = 255;

	Sphere murFond{ Vec3d{500, sphereDistance+500, 500}, 400, {1,0,0} };
	Sphere murGauche{ Vec3d{-10000, sphereDistance+30, 500}, 93230, {0,0,1} };
	Sphere murDroite{ Vec3d{11000, sphereDistance+30, 500}, 93230, {0,0,1} };
	Sphere murHaut{ Vec3d{500, sphereDistance + 30, -10000}, 93230, {0,1,0} };
	Sphere murBas{ Vec3d{500, sphereDistance + 30, 11000}, 93230, {0,1,0} };
	Sphere sphere{ Vec3d{350, sphereDistance, 500}, 100 , {0,1,1} };
	Sphere sphere2{ Vec3d{650, sphereDistance, 500}, 100, {1,1,0} };


	vector<Sphere> spheres = { sphere, sphere2, murGauche, murDroite, murHaut, murBas, murFond };

	LightSource lightSource{ Vec3d{500, sphereDistance-200, 300}, 70000000 };
	bool sphereFound = false;
	for (int i = 0; i < imgWidth; ++i) {
		for (int j = 0; j < imgHeight ; ++j) {
			
			Ray ray{ Vec3d{(float)i, 0, (float)j}, Vec3d{0, 1, 0} };
			for (Sphere s : spheres) {
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
					double f = w0.dot(normale) / PI;

					Vec3d couleur = (lightSource.color * f * s.albedo) / d2;

					image.at<Vec3b>({ i,j }) = couleur;
					sphereFound = true;
				} else {
					image.at<Vec3b>({ i,j }) = { 255,255,0 };
				}

			
			}		
			sphereFound = false;
		}
	}
	
	imshow("Display  Window", image);
	imwrite("sphere.jpg", image);
	waitKey(0);

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