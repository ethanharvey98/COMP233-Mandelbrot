// Ethan Harvey ~ COMP 233 ~ Mandelbrot
// This program draws Mandelbrot images
// Code by Farouk Ounanes
// https://github.com/ChinksofLight/mandelbrot_cpp

#include <iostream>
#include <fstream>
#include <complex>
#include <omp.h>

using namespace std;

// Width and height of image
float width = 3000;
float height = 3000;

// Number of iterations
int ITERATIONS = 300;

// Function returns number of iterations or 0 if the pixel is a Mandelbrot point
int value(int x, int y) {
	complex<float> point((float)x / width - 1.5, (float)y / height - 0.5);
	complex<float> z(0, 0);
	int nb_iter = 0;
	while (abs(z) < 2 && nb_iter <= ITERATIONS) {
		z = z * z + point;
		nb_iter++;
	} // End while
	if (nb_iter < ITERATIONS) {
		// Return number of iterations
		return nb_iter;
	}
	// Mandelbrot point
	else {
		return 0;
	}
}

int main() {

	// Opening message
	cout << "Ethan Harvey ~ COMP 233 ~ Mandelbrot\n\n";

	// Fill count with 0
	int* count = new int[ITERATIONS];
	for (int i = 0; i < ITERATIONS; i++) {
		count[i] = 0;
	}

	// Histogram and U distribution
	int* histogram = new int[ITERATIONS];
	double* u = new double[ITERATIONS];

	// 2D array of iterations representing image
	int** iterations = new int* [height];
	for (int i = 0; i < height; i++) {
		iterations[i] = new int[width];
	}

	// Red, Green, and Blue color values
	int r, g, b;

	// String to print to file
	string stringToPrint = "";

	// File header
	stringToPrint += "P3\n" + std::to_string((int)width) + " " + std::to_string((int)height) + " 255\n";
	stringToPrint += "# Ethan Harvey ~ COMP 233 ~ Mandelbrot\n";
	stringToPrint += "# Code by Farouk Ounanes\n";

	// Set number of threads
	omp_set_num_threads(4);

#pragma omp for
	{
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {

				int val = value(row, col);
				iterations[row][col] = val;

#pragma omp atomic
				count[val]++;

			}
		}
	}

	// Fill histogram values
	histogram[0] = count[0];
	histogram[1] = count[1];
	for (int i = 2; i < ITERATIONS; i++) {
		histogram[i] = histogram[i - 1] + count[i];
	}

	// Fill U distribution values
	u[0] = 0;
	for (int i = 1; i < ITERATIONS; i++) {
		u[i] = (double)histogram[i] / histogram[ITERATIONS - 1];
	}

	ofstream my_Image("mandelbrot.ppm");

	if (my_Image.is_open()) {

		// Counter to only print out 5 pixels per line
		int counter = 0;

		// COLOR SCEME
		// White: RGB (255, 255, 255)
		// Yellow: RGB (255, 255, 0)
		// Pink: RGB (255, 0, 255)
		// Teal: RGB (0, 255, 255)
		
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				if (iterations[row][col] == 0) {
					stringToPrint += "0 0 0 ";
				}
				// White
				else if (0 <= u[iterations[row][col]] && u[iterations[row][col]] < 0.125) {
					// Set RGB values
					r = (int)((((u[iterations[row][col]] - 0) / 0.125) * -255) + 255);
					g = (int)((((u[iterations[row][col]] - 0) / 0.125) * -255) + 255);
					b = (int)((((u[iterations[row][col]] - 0) / 0.125) * -255) + 255);
					stringToPrint += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";

				}
				// Outer
				else if (0.125 <= u[iterations[row][col]] && u[iterations[row][col]] < 0.250) {
					// Set RGB values
					r = (int)((((u[iterations[row][col]] - 0.125) / 0.125) * 255) + 0);
					g = (int)((((u[iterations[row][col]] - 0.125) / 0.125) * 255) + 0);
					b = 0;
					stringToPrint += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";

				}
				// Outer
				else if (0.250 <= u[iterations[row][col]] && u[iterations[row][col]] < 0.375) {
					// Set RGB values
					r = (int)((((u[iterations[row][col]] - 0.250) / 0.125) * -255) + 255);
					g = (int)((((u[iterations[row][col]] - 0.250) / 0.125) * -255) + 255);
					b = 0;
					stringToPrint += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";
				}
				// Middle
				else if (0.375 <= u[iterations[row][col]] && u[iterations[row][col]] < 0.500) {
					// Set RGB values
					r = (int)((((u[iterations[row][col]] - 0.375) / 0.125) * 255) + 0);
					g = 0;
					b = (int)((((u[iterations[row][col]] - 0.375) / 0.125) * 255) + 0);
					stringToPrint += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";
				
				}
				// Middle
				else if (0.5 <= u[iterations[row][col]] && u[iterations[row][col]] < 0.625) {
					// Set RGB values
					r = (int)((((u[iterations[row][col]] - 0.5) / 0.125) * -255) + 255);
					g = 0;
					b = (int)((((u[iterations[row][col]] - 0.5) / 0.125) * -255) + 255);
					stringToPrint += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";

				}
				// Inner
				else if (0.625 <= u[iterations[row][col]] && u[iterations[row][col]] < 0.750) {
					// Set RGB values
					r = 0;
					g = (int)((((u[iterations[row][col]] - 0.625) / 0.125) * 255) + 0);
					b = (int)((((u[iterations[row][col]] - 0.625) / 0.125) * 255) + 0);
					stringToPrint += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";

				}
				// Inner
				else if (0.750 <= u[iterations[row][col]] && u[iterations[row][col]] < 0.875) {
					// Set RGB values
					r = 0;
					g = (int)((((u[iterations[row][col]] - 0.750) / 0.125) * -255) + 255);
					b = (int)((((u[iterations[row][col]] - 0.750) / 0.125) * -255) + 255);
					stringToPrint += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";

				}
				// White
				// (0.875 <= u[iterations[row][col]] && u[iterations[row][col]] < 1)
				else {
					// Set RGB values
					r = (int)((((u[iterations[row][col]] - 0.875) / 0.125) * 255) + 0);
					g = (int)((((u[iterations[row][col]] - 0.875) / 0.125) * 255) + 0);
					b = (int)((((u[iterations[row][col]] - 0.875) / 0.125) * 255) + 0);
					stringToPrint += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";

				}

				// Incrememnt pixels per line counter
				counter++;
				// Only print out 5 pixels per line
				if (counter % 5 == 0) {
					stringToPrint += "\n";
					// Write string to file
					my_Image << stringToPrint;
					// Reset string
					stringToPrint = "";
				}
			}
		}

		// Close file
		my_Image.close();
	}
	else {
		cout << "Could not open the file";
	}

	// Memory clean up
	delete[] count;
	delete[] histogram;
	delete[] u;
	for (int i = 0; i < height; i++) {
		delete[] iterations[i];
	}
	delete[] iterations;

	// Normal Termination
	cout << "\n\n<<< Normal Termination >>>\n\n";
	return 0;
}