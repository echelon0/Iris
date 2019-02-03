
int ScanWord(char *str, char *substr) { //returns chars read
    int count = 0;
    while(*str && (*str != ' ') && (*str != '\n')) {
        *substr++ = *str++;
        count++;
    }
    *substr = '\0';
    return count;
}

model
LoadObj(char *FileName) {
    model LoadedModel = {};
    
    char FolderPath[] = "../models/";
    int FolderPathLength = StrLen(FolderPath);
    
    char *FilePath = (char *)malloc((FolderPathLength + StrLen(FileName) + 1) * sizeof(char));
    StrCopy(FilePath, FolderPath);
    StrCat(FilePath, FileName);
        
    FILE *objFileHandle = fopen(FilePath, "r");
    if(!objFileHandle) {
        char *msg = (char *)calloc(StrLen("Cannot open .obj file: "), sizeof(char));
        StrCat(msg, "Cannot open .obj file: ");
        StrCat(msg, FileName);
        LOG_ERROR("ERROR", msg);
        return LoadedModel;
    }

    //mtl file path
    int FilePathLength = StrLen(FilePath) - 4;
    char mtlFileExtension[] = ".mtl";
    for(int i = FilePathLength, j = 0; i < FilePathLength + 4, j < 4; i++, j++) {
        FilePath[i] = mtlFileExtension[j];
    }

    FILE *mtlFileHandle = fopen(FilePath, "r");
    if(!mtlFileHandle) {
        char errorMsg[] = "material file not found for ";
        char *fullErrorMsg = (char *)malloc((StrLen(errorMsg) + StrLen(FileName) + 1) * sizeof(char));
        StrCopy(fullErrorMsg, errorMsg);
        StrCat(fullErrorMsg, FileName);
        LOG_ERROR("Message", fullErrorMsg);
        delete fullErrorMsg;
        return LoadedModel;
    }

    char lineID[2048];
    struct MaterialWithID {
        char name[1024];
        bool recorded;
        material Mat;
    };
    array<MaterialWithID> Materials;
    while(fscanf(mtlFileHandle, "%s", lineID) != EOF) {
        if(strcmp(lineID, "newmtl") == 0) {
            MaterialWithID Mat = {};
            fscanf(mtlFileHandle, "%s", Mat.name);
            Materials.PushBack(Mat);
                        
        } else if(strcmp(lineID, "Kd") == 0) { //Diffuse
            fscanf(mtlFileHandle, "%f%f%f\n",
                   &Materials[Materials.Size - 1].Mat.Diffuse.x,
                   &Materials[Materials.Size - 1].Mat.Diffuse.y,
                   &Materials[Materials.Size - 1].Mat.Diffuse.z);
            
        } else if(strcmp(lineID, "Ks") == 0) { //Specular
            fscanf(mtlFileHandle, "%f%f%f\n",
                   &Materials[Materials.Size - 1].Mat.Specular.x,
                   &Materials[Materials.Size - 1].Mat.Specular.y,
                   &Materials[Materials.Size - 1].Mat.Specular.z);

        } else if(strcmp(lineID, "Ns") == 0) { //Specular Exponent
            fscanf(mtlFileHandle, "%f", &Materials[Materials.Size - 1].Mat.Exponent);

        } else if(strcmp(lineID, "d") == 0) { //dissolve
            fscanf(mtlFileHandle, "%f\n", &Materials[Materials.Size - 1].Mat.Transparency);
            Materials[Materials.Size - 1].Mat.Transparency = 1.0f - Materials[Materials.Size - 1].Mat.Transparency;
            
        } else if(strcmp(lineID, "Tr") == 0) {
            fscanf(mtlFileHandle, "%f\n", &Materials[Materials.Size - 1].Mat.Transparency);
            
        }
    }
    
    array<vec3> Positions;
    array<vec3> Normals;
    array<vec2> TexCoords;
    
    vec3 position;
    vec3 Normal;
    vec2 TexCoord;

    bool has_v = false;
    bool has_vt = false;
    bool has_vn = false;

    int currentMatIndex = -1;
    while(fscanf(objFileHandle, "%s", lineID) != EOF) {
        if(strcmp(lineID, "v") == 0) { //Vertex position
            fscanf(objFileHandle, "%f%f%f\n", &position.x, &position.y, &position.z);
            Positions.PushBack(position);
            has_v = true;

        } else if(strcmp(lineID, "vt") == 0) { //TexCoord
            fscanf(objFileHandle, "%f%f\n", &TexCoord.x, &TexCoord.y);
            TexCoords.PushBack(TexCoord);
            has_vt = true;
            
        } else if(strcmp(lineID, "vn") == 0) { //Normal
            fscanf(objFileHandle, "%f%f%f\n", &Normal.x, &Normal.y, &Normal.z);
            Normals.PushBack(Normal);
            has_vn = true;

        } else if(strcmp(lineID, "usemtl") == 0) {
            char MatName[1024];
            fscanf(objFileHandle, "%s", MatName);
            for(int i = 0; i < Materials.Size; i++) {
                if(strcmp(Materials[i].name, MatName) == 0) {
                    currentMatIndex = i;
                }
            }
            
        } else if(strcmp(lineID, "f") == 0) {
            int v1=0, v2=0, v3=0;
            int vt1=0, vt2=0, vt3=0;
            int vn1=0, vn2=0, vn3=0;

            if(has_v && has_vt && has_vn) { // v/vt/vn
                fscanf(objFileHandle, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
            } else if(has_v && has_vt && !has_vn) { // v/vt
                fscanf(objFileHandle, "%d/%d %d/%d %d/%d\n", &v1, &vt1, &v2, &vt2, &v3, &vt3);
            } else if(has_v && !has_vt && has_vn) { // v//vn
                fscanf(objFileHandle, "%d//%d %d//%d %d//%d\n", &v1, &vn1, &v2, &vn2, &v3, &vn3);
            } else if(has_v && !has_vt && !has_vn) { // v
                fscanf(objFileHandle, "%d %d %d\n", &v1, &v2, &v3);
            } else {
                LOG_ERROR("ERROR", "Cannot open .obj");
            }
            
            vertex_attribute TempVertices[3];
            TempVertices[0].Position = Positions[v1-1];
            TempVertices[0].TexCoord = TexCoords[vt1-1];
            TempVertices[1].Position = Positions[v2-1];
            TempVertices[1].TexCoord = TexCoords[vt2-1];
            TempVertices[2].Position = Positions[v3-1];
            TempVertices[2].TexCoord = TexCoords[vt3-1];
            TempVertices[0].Normal = Normals[vn1 - 1];
            TempVertices[1].Normal = Normals[vn1 - 1];
            TempVertices[2].Normal = Normals[vn1 - 1];                                                                        
//            TempVertices[0].Normal = normalize(cross(normalize(TempVertices[2].Position - TempVertices[1].Position), 
//                                                     normalize(TempVertices[0].Position - TempVertices[1].Position)));
//            TempVertices[1].Normal = TempVertices[0].Normal;
//            TempVertices[2].Normal = TempVertices[0].Normal;
            for(int i = 0; i < 3; i++) {
                if(!Materials[currentMatIndex].recorded) {
                    material finalMat = {};
                    finalMat.Diffuse = Materials[currentMatIndex].Mat.Diffuse;
                    finalMat.Specular = Materials[currentMatIndex].Mat.Specular;
                    finalMat.Exponent = Materials[currentMatIndex].Mat.Exponent;
                    finalMat.Transparency = Materials[currentMatIndex].Mat.Transparency;
                    
                    LoadedModel.Materials.PushBack(finalMat);
                    LoadedModel.MaterialBases.PushBack(LoadedModel.VertexAttributes.Size);
                    if(LoadedModel.Materials.Size >= 2) {
                        int PrevMatSize = LoadedModel.MaterialBases[LoadedModel.MaterialBases.Size - 1] - LoadedModel.MaterialBases[LoadedModel.MaterialBases.Size - 2];
                        LoadedModel.MaterialSizes.PushBack(PrevMatSize);
                    }
                    Materials[currentMatIndex].recorded = true;
                }
                LoadedModel.VertexAttributes.PushBack(TempVertices[i]);
            }
        }
    }
    
    if(LoadedModel.Materials.Size >= 2) {
        int PrevMatSize = LoadedModel.VertexAttributes.Size - LoadedModel.MaterialBases[LoadedModel.MaterialBases.Size - 2];
        LoadedModel.MaterialSizes.PushBack(PrevMatSize);
    } else if(LoadedModel.Materials.Size == 1) {
        LoadedModel.MaterialSizes.PushBack(LoadedModel.VertexAttributes.Size);
    }

    StrCopy(LoadedModel.StrName, FileName);
    
    delete FilePath;
    delete Positions.Data;
    delete Normals.Data;
    delete TexCoords.Data;

    fclose(objFileHandle);
    fclose(mtlFileHandle);
    
    return LoadedModel;
}
