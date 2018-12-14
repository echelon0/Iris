
int ScanWord(char *str, char *substr) { //returns chars read
    int count = 0;
    while(*str && (*str != ' ') && (*str != '\n')) {
        *substr++ = *str++;
        count++;
    }
    *substr = '\0';
    return count;
}

Model LoadObj(char *fileName) {
    Model loadedModel = {};
    
    char folderPath[] = "../models/";
    int folderPathLength = StrLen(folderPath);
    
    char *filePath = (char *)malloc((folderPathLength + StrLen(fileName) + 1) * sizeof(char));
    StrCopy(filePath, folderPath);
    StrCat(filePath, fileName);
        
    FILE *objFileHandle = fopen(filePath, "r");
    if(!objFileHandle) {
        char *msg = (char *)calloc(StrLen("Cannot open .obj file: "), sizeof(char));
        StrCat(msg, "Cannot open .obj file: ");
        StrCat(msg, fileName);
        LOG_ERROR("ERROR", msg);
        return loadedModel;
    }

    //mtl file path
    int filePathLength = StrLen(filePath) - 4;
    char mtlFileExtension[] = ".mtl";
    for(int i = filePathLength, j = 0; i < filePathLength + 4, j < 4; i++, j++) {
        filePath[i] = mtlFileExtension[j];
    }

    FILE *mtlFileHandle = fopen(filePath, "r");
    if(!mtlFileHandle) {
        char errorMsg[] = "Material file not found for ";
        char *fullErrorMsg = (char *)malloc((StrLen(errorMsg) + StrLen(fileName) + 1) * sizeof(char));
        StrCopy(fullErrorMsg, errorMsg);
        StrCat(fullErrorMsg, fileName);
        LOG_ERROR("Message", fullErrorMsg);
        delete fullErrorMsg;
        return loadedModel;
    }

    char lineID[2048];
    struct MaterialWithID {
        char name[1024];
        bool recorded;
        Material mat;
    };
    Array<MaterialWithID> materials;
    while(fscanf(mtlFileHandle, "%s", lineID) != EOF) {
        if(strcmp(lineID, "newmtl") == 0) {
            MaterialWithID mat = {};
            fscanf(mtlFileHandle, "%s", mat.name);
            materials.PushBack(mat);
                        
        } else if(strcmp(lineID, "Kd") == 0) { //diffuse
            fscanf(mtlFileHandle, "%f%f%f\n",
                   &materials[materials.size - 1].mat.diffuse.x,
                   &materials[materials.size - 1].mat.diffuse.y,
                   &materials[materials.size - 1].mat.diffuse.z);
            
        } else if(strcmp(lineID, "Ks") == 0) { //specular
            fscanf(mtlFileHandle, "%f%f%f\n",
                   &materials[materials.size - 1].mat.specular.x,
                   &materials[materials.size - 1].mat.specular.y,
                   &materials[materials.size - 1].mat.specular.z);

        } else if(strcmp(lineID, "Ns") == 0) { //specular exponent
            fscanf(mtlFileHandle, "%f", &materials[materials.size - 1].mat.exponent);

        } else if(strcmp(lineID, "d") == 0) { //dissolve
            fscanf(mtlFileHandle, "%f\n", &materials[materials.size - 1].mat.transparency);
            materials[materials.size - 1].mat.transparency = 1.0f - materials[materials.size - 1].mat.transparency;
            
        } else if(strcmp(lineID, "Tr") == 0) {
            fscanf(mtlFileHandle, "%f\n", &materials[materials.size - 1].mat.transparency);
            
        }  else if(strcmp(lineID, "illum") == 0) { //illumination model
            fscanf(mtlFileHandle, "%d\n", &materials[materials.size - 1].mat.illumModel);
        }
    }
    
    Array<vec3> positions;
    Array<vec3> normals;
    Array<vec2> texcoords;
    
    vec3 position;
    vec3 normal;
    vec2 texcoord;

    bool has_v = false;
    bool has_vt = false;
    bool has_vn = false;

    int currentMatIndex = -1;
    while(fscanf(objFileHandle, "%s", lineID) != EOF) {
        if(strcmp(lineID, "v") == 0) { //vertex position
            fscanf(objFileHandle, "%f%f%f\n", &position.x, &position.y, &position.z);
            positions.PushBack(position);
            has_v = true;

        } else if(strcmp(lineID, "vt") == 0) { //texcoord
            fscanf(objFileHandle, "%f%f\n", &texcoord.x, &texcoord.y);
            texcoords.PushBack(texcoord);
            has_vt = true;
            
        } else if(strcmp(lineID, "vn") == 0) { //normal
            fscanf(objFileHandle, "%f%f%f\n", &normal.x, &normal.y, &normal.z);
            normals.PushBack(normal);
            has_vn = true;

        } else if(strcmp(lineID, "usemtl") == 0) {
            char matName[1024];
            fscanf(objFileHandle, "%s", matName);
            for(int i = 0; i < materials.size; i++) {
                if(strcmp(materials[i].name, matName) == 0) {
                    currentMatIndex = i;
                }
            }
            
        } else if(strcmp(lineID, "f") == 0) {
            int v1=0, v2=0, v3=0;
            int vt1=0, vt2=0, vt3=0;
            int vnPlaceholder=0;

            if(has_v && has_vt && has_vn) { // v/vt/vn
                fscanf(objFileHandle, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &vt1, &vnPlaceholder, &v2, &vt2, &vnPlaceholder, &v3, &vt3, &vnPlaceholder);
            } else if(has_v && has_vt && !has_vn) { // v/vt
                fscanf(objFileHandle, "%d/%d %d/%d %d/%d\n", &v1, &vt1, &v2, &vt2, &v3, &vt3);
            } else if(has_v && !has_vt && has_vn) { // v//vn
                fscanf(objFileHandle, "%d//%d %d//%d %d//%d\n", &v1, &vnPlaceholder, &v2, &vnPlaceholder, &v3, &vnPlaceholder);
            } else if(has_v && !has_vt && !has_vn) { // v
                fscanf(objFileHandle, "%d %d %d\n", &v1, &v2, &v3);
            } else {
                LOG_ERROR("ERROR", "Cannot open .obj");
            }
            
            VertexAttribute tempVertices[3];
            tempVertices[0].position = positions[v1-1];
            tempVertices[0].texcoord = texcoords[vt1-1];
            tempVertices[1].position = positions[v2-1];
            tempVertices[1].texcoord = texcoords[vt2-1];
            tempVertices[2].position = positions[v3-1];
            tempVertices[2].texcoord = texcoords[vt3-1];

            tempVertices[0].normal = cross(tempVertices[1].position - tempVertices[0].position,
                                            tempVertices[2].position - tempVertices[0].position);
            tempVertices[1].normal = tempVertices[0].normal;
            tempVertices[2].normal = tempVertices[0].normal;
            for(int i = 0; i < 3; i++) {
                if(!materials[currentMatIndex].recorded) {
                    Material finalMat = {};
                    finalMat.diffuse = materials[currentMatIndex].mat.diffuse;
                    finalMat.specular = materials[currentMatIndex].mat.specular;
                    finalMat.exponent = materials[currentMatIndex].mat.exponent;
                    finalMat.transparency = materials[currentMatIndex].mat.transparency;
                    finalMat.illumModel = materials[currentMatIndex].mat.illumModel;
                    
                    loadedModel.materials.PushBack(finalMat);
                    loadedModel.materialBases.PushBack(loadedModel.vertexAttributes.size);
                    if(loadedModel.materials.size >= 2) {
                        int prevMatSize = loadedModel.materialBases[loadedModel.materialBases.size - 1] - loadedModel.materialBases[loadedModel.materialBases.size - 2];
                        loadedModel.materialSizes.PushBack(prevMatSize);
                    }
                    materials[currentMatIndex].recorded = true;
                }
                loadedModel.vertexAttributes.PushBack(tempVertices[i]);
            }
        }
    }
    
    if(loadedModel.materials.size >= 2) {
        int prevMatSize = loadedModel.vertexAttributes.size - loadedModel.materialBases[loadedModel.materialBases.size - 2];
        loadedModel.materialSizes.PushBack(prevMatSize);
    } else if(loadedModel.materials.size == 1) {
        loadedModel.materialSizes.PushBack(loadedModel.vertexAttributes.size);
    }

    StrCopy(loadedModel.strName, fileName);
    
    delete filePath;
    delete positions.data;
    delete normals.data;
    delete texcoords.data;

    fclose(objFileHandle);
    fclose(mtlFileHandle);
    
    return loadedModel;
}
