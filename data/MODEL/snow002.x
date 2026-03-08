xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 20;
 -33.17543;0.00000;-17.26608;,
 -39.00000;0.00000;-11.00000;,
 -25.00000;0.00000;13.00000;,
 -17.88808;5.40543;3.48941;,
 -9.72620;6.83918;-6.50283;,
 -27.00000;0.00000;-23.50000;,
 -12.00000;0.00000;34.00000;,
 -3.43867;6.43519;24.08909;,
 5.99806;9.43040;12.98818;,
 12.00000;0.00000;37.00000;,
 18.76541;0.00000;32.80563;,
 25.50000;0.00000;28.50000;,
 -21.01856;0.00000;-29.83501;,
 -1.67193;4.61164;-16.33307;,
 6.00000;0.00000;-25.50000;,
 -15.00000;0.00000;-36.00000;,
 15.09208;6.74839;1.48948;,
 23.00000;0.00000;-9.00000;,
 32.28348;0.00000;24.25121;,
 39.00000;0.00000;20.00000;;
 
 12;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;3,2,6,7;,
 4;3,7,8,4;,
 4;7,6,9,10;,
 4;7,10,11,8;,
 4;12,5,4,13;,
 4;12,13,14,15;,
 4;13,4,8,16;,
 4;13,16,17,14;,
 4;16,8,11,18;,
 4;16,18,19,17;;
 
 MeshMaterialList {
  2;
  12;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   25.000000;
   0.500000;0.500000;0.500000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\source\\DirectX\\3DActionGame_000\\DirectX_003\\data\\TEXTURE\\snow000.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   25.000000;
   0.500000;0.500000;0.500000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\snow004.png";
   }
  }
 }
 MeshNormals {
  20;
  -0.332446;0.932993;0.137852;,
  -0.171059;0.931188;0.321913;,
  0.053325;0.917712;0.393650;,
  0.287100;0.957655;-0.021682;,
  0.278340;0.945505;-0.168958;,
  0.013106;0.971498;-0.236685;,
  -0.274524;0.960680;0.041606;,
  0.139553;0.950650;-0.277111;,
  -0.173585;0.968832;-0.176727;,
  -0.118940;0.985413;-0.121712;,
  0.075632;0.993070;0.089953;,
  0.209786;0.947819;0.240060;,
  -0.249537;0.967367;-0.043949;,
  -0.270067;0.961602;0.048836;,
  -0.091273;0.961028;0.260951;,
  0.108853;0.924454;0.365427;,
  -0.055480;0.971459;-0.230628;,
  0.053958;0.969236;-0.240146;,
  0.220282;0.970875;-0.094226;,
  0.271079;0.960676;0.060143;;
  12;
  4;12,6,0,13;,
  4;12,13,9,8;,
  4;13,0,1,14;,
  4;13,14,10,9;,
  4;14,1,2,15;,
  4;14,15,11,10;,
  4;16,8,9,17;,
  4;16,17,7,5;,
  4;17,9,10,18;,
  4;17,18,4,7;,
  4;18,10,11,19;,
  4;18,19,3,4;;
 }
 MeshTextureCoords {
  20;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
