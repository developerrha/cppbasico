/* 
 * File:   main.cpp
 * Author: homzode
 * 
 */

#include <iostream>
#include <fstream>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "QrCode.hpp"

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

static std::string filePath;
static std::string textQr;
static void doBasicDemo();
static std::string toSvgString(const QrCode &qr, int border);
static void writeToFile(const QrCode &qr, int border );
static void printQr(const QrCode &qr);


int main(int argc, char** argv) {
    filePath = argv[1];
    textQr = argv[2];
    std::cout << "filePath: "+filePath+" textQr: "+textQr;
    doBasicDemo();
    return 0;
}
// Creates a single QR Code, then prints it to the console.
static void doBasicDemo() {
	const char *text = textQr.c_str();              // User-supplied text
	const QrCode::Ecc errCorLvl = QrCode::Ecc::MEDIUM;  // Error correction level
	// Crea e imprime QR
	const QrCode qr = QrCode::encodeText(text, errCorLvl);
	printQr(qr);
        writeToFile(qr, 4);
	std::cout << toSvgString(qr, 4) << std::endl;
}


/*---- Utilities ----*/
/*
 *  Metodo que Escribe QR en archivo XML
 */
static void writeToFile(const QrCode &qr,int border){
    std::ofstream xmloutfile(filePath);
        xmloutfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	xmloutfile << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	xmloutfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	xmloutfile << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
	xmloutfile << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
	xmloutfile << "\t<path d=\"";
	for (int y = 0; y < qr.getSize(); y++) {
		for (int x = 0; x < qr.getSize(); x++) {
			if (qr.getModule(x, y)) {
				if (x != 0 || y != 0)
					xmloutfile << " ";
				xmloutfile << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
			}
		}
	}
	xmloutfile << "\" fill=\"#000000\"/>\n";
	xmloutfile << "</svg>\n";
        xmloutfile.close();
}

 /*Devuelve una cadena de código SVG para una imagen que representa el código QR dado, con el número dado
 de módulos fronterizos. La cadena siempre usa nuevas líneas de Unix (\n), independientemente de la plataforma.
  */

static std::string toSvgString(const QrCode &qr, int border) {
	if (border < 0)
		throw std::domain_error("Border must be non-negative");
	if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
		throw std::overflow_error("Border too large");
	std::ostringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
	sb << "\t<path d=\"";
	for (int y = 0; y < qr.getSize(); y++) {
		for (int x = 0; x < qr.getSize(); x++) {
			if (qr.getModule(x, y)) {
				if (x != 0 || y != 0)
					sb << " ";
				sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
			}
		}
	}
	sb << "\" fill=\"#000000\"/>\n";
	sb << "</svg>\n";
	return sb.str();
}


// Imprime el objeto QrCode dado en la consola.
static void printQr(const QrCode &qr) {
	int border = 4;
	for (int y = -border; y < qr.getSize() + border; y++) {
		for (int x = -border; x < qr.getSize() + border; x++) {
			std::cout << (qr.getModule(x, y) ? "##" : "  ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

