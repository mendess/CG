#include "parser.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>





Parser::Parser(string f, string p, int t) : filename(f),patchfile(p),tessellation(t) {}






void Parser::parsePatchFile() {

  int numberPatches;  // numero de patches
  int positionLine;  // posição atual na string "line" declarada abaixo
  int positionPointLine; // posição atual na string "pointLine" declarada abaixo
  int index;  
  float pointCoords[3]; // Coordenadas de um ponto
  string currentLine;  // Linha de indices atual  
  string currentPointLine;  // Linha de pontos atual


  ifstream patch;
  patch.open(patchfile.c_str());



  if (patch.is_open()) {
    getline(patch, currentLine);
    numberPatches = atoi(currentLine.c_str());

    for (int i=0; i<numberPatches; i++) {
      getline(patch,currentLine);
      Patch* p = new Patch();
      patches.push_back(p);

      for (int j=0; j<16;j++) {
        positionLine = currentLine.find(',',0);
        index = atoi(currentLine.substr(0,positionLine).c_str());
        currentPointLine = lineNumberContent(numberPatches+3+index,patchfile);

        for (int k=0;k<3;k++) {
          positionPointLine = currentPointLine.find(',',0);
          pointCoords[k] = atof(currentPointLine.substr(0,positionPointLine).c_str());
          currentPointLine.erase(0,positionPointLine+1);
        }

        p->addControlPoint(new Point(pointCoords[0],pointCoords[1],pointCoords[2]));
        currentLine.erase(0,positionLine+1);
      }
    }

    patch.close();

  } else {
    cout << "Error opening file " << patchfile << endl;
  }
}



//returns a certain line from the file
string Parser::lineNumberContent(int lineNumber, string filename) {

  string content;

  ifstream patch;
  patch.open(filename.c_str());

  if (patch.is_open())
    for (int i=0; i<lineNumber; i++) getline(patch,content);

  else cout << "Error opening file " << patchfile << endl;

  return content;
}
