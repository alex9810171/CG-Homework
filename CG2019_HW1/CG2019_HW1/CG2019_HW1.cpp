#include "pch.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include "float.h"
#include "hitable_list.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

using namespace std;

// pixel resolution
int width = 2560;
int height = 1440;
int sampleTime = 8;

// camera
vec3 lower_left_corner(-2, -9/8, -1);
vec3 origin(0, 0, 1);
vec3 horizontal(4, 0, 0);
vec3 vertical(0, 9/4, 0);

// light
vec3 lightPosition(-10, 10, 0);
vec3 lightIntensity(1.0, 1.0, 1.0);

vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.0, (numeric_limits<float>::max)(), rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 5 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
	fstream file;
	file.open("ray.ppm", ios::out);
	
	// initialize ball
	hitable *list[52];
	list[0] = new sphere(vec3(0, -100.5, -2), 100, new lambertian(vec3(0.6, 0.6, 0.6)));
	list[1] = new sphere(vec3(0, 0, -2), 0.5, new lambertian(vec3(0.3, 0.8, 0.8)));
	list[2] = new sphere(vec3(1, 0, -1.75), 0.5, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	list[3] = new sphere(vec3(-1, 0, -2.25), 0.5, new dielectric(1.5));
	srand(1234);
	for (int i = 0; i < 48; i++) {
		float choose_mat = drand48();
		float xr = ((float)rand() / (float)(RAND_MAX)) * 6.0f - 3.0f;
		float zr = ((float)rand() / (float)(RAND_MAX)) * 3.0f - 1.5f;
		if (choose_mat < 0.8) {  // diffuse
			list[i + 4] = new sphere(vec3(xr, -0.45, zr - 2), 0.05, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
		}
		else if (choose_mat < 0.95) { // metal
			list[i + 4] = new sphere(vec3(xr, -0.45, zr - 2), 0.05, new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
		}
		else {  // glass
			list[i + 4] = new sphere(vec3(xr, -0.45, zr - 2), 0.05, new dielectric(1.5));
		}
	}
	hitable *world = new hitable_list(list, 52);
	
	file << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			vec3 pixelColor(0, 0, 0);
			for (int s = 0; s < sampleTime; s++) {
				float random = rand() % (100) / (float)(100);
				float u = float(i + random) / float(width);
				float v = float(j + random) / float(height);
				ray r(origin, lower_left_corner + u * horizontal + v * vertical);
				pixelColor += color(r, world, 0);
			}
			pixelColor /= float(sampleTime);
			// gamma correction
			pixelColor = vec3(sqrt(pixelColor.e[0]), sqrt(pixelColor.e[1]), sqrt(pixelColor.e[2]));
			file << int(pixelColor.e[0] * 255) << " " << int(pixelColor.e[1] * 255) << " " << int(pixelColor.e[2] * 255) << "\n";
		}
	}
	return 0;
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的秘訣: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案

/*
float hit_sphere(const vec3 &center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0*a);
	}
}

		vec3 N = rec.normal;
		vec3 L = unit_vector(lightPosition - rec.p);
		vec3 I = vec3(lightIntensity);
		return I * max(float(0), dot(N, L));

*/
