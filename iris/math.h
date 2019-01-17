 
#include <math.h>

#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

#define PI 3.14159

struct ivec2 {
    int x;
    int y;

    ivec2() {
        this->x = 0;
        this->y = 0;
    }
    
    ivec2(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator == (ivec2 rhs) {
        return (this->x == rhs.x) && (this->y == rhs.y);
    }

    bool operator != (ivec2 rhs) {
        return !(*this == rhs);
    }

    ivec2 operator / (int scalar) {
        return ivec2(this->x / scalar, this->y / scalar);
    }
    ivec2 operator / (ivec2 rhs) {
        return ivec2(this->x / rhs.x, this->y / rhs.y);
    }
};

inline ivec2
operator - (ivec2 lhs, ivec2 rhs) {
    ivec2 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}

struct vec2 {
    float x;
    float y;
    
    vec2() {
        this->x = 0.0f;
        this->y = 0.0f;
    }
    
    vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    bool operator == (vec2 rhs) {
        return (this->x == rhs.x) && (this->y == rhs.y);
    }

    bool operator != (vec2 rhs) {
        return !(*this == rhs);
    }

    bool operator > (vec2 rhs) {
        return sqrt(this->x * this->x + this->y * this->y) > sqrt(rhs.x * rhs.x + rhs.y * rhs.y);
    }
};

inline vec2
operator * (vec2 vector, float scalar) {
    vec2 result;
    result.x = vector.x * scalar;
    result.y = vector.y * scalar;
    return result;
}

inline vec2
operator * (float scalar, vec2 vector) {
    vec2 result;
    result.x = vector.x * scalar;
    result.y = vector.y * scalar;
    return result;
}

inline vec2
operator / (vec2 vector, float scalar) {
    vec2 result;
    result.x = vector.x / scalar;
    result.y = vector.y / scalar;
    return result;    
}

inline vec2
operator + (vec2 lhs, vec2 rhs) {
    vec2 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;    
}

inline vec2
operator - (vec2 lhs, vec2 rhs) {
    vec2 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;    
}

struct vec3 {
    float x;
    float y;
    float z;

    vec3() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }
    
    vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    vec3& operator += (const vec3 &rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    vec3& operator -= (const vec3 &rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }

    vec3& operator *= (const vec3 &rhs) {
        this->x *= rhs.x;
        this->y *= rhs.y;
        this->z *= rhs.z;
        return *this;
    }

    bool operator == (vec3 rhs) {
        return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z);
    }

    bool operator != (vec3 rhs) {
        return !(*this == rhs);
    }
};

inline vec3
operator * (vec3 vector, float scalar) {
    vec3 result;
    result.x = vector.x * scalar;
    result.y = vector.y * scalar;
    result.z = vector.z * scalar;
    return result;
}

inline vec3
operator * (float scalar, vec3 vector) {
    vec3 result;
    result.x = vector.x * scalar;
    result.y = vector.y * scalar;
    result.z = vector.z * scalar;
    return result;
}

inline vec3
operator * (vec3 vec1, vec3 vec2) {
    vec3 result;
    result.x = vec1.x * vec2.x;
    result.y = vec1.y * vec2.y;
    result.z = vec1.z * vec2.z;
    return result;
}

inline vec3
operator / (vec3 vector, float scalar) {
    vec3 result;
    result.x = vector.x / scalar;
    result.y = vector.y / scalar;
    result.z = vector.z / scalar;
    return result;    
}

inline vec3
operator / (vec3 lhs, vec3 rhs) {
    vec3 result;
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    result.z = lhs.z / rhs.z;
    return result;    
}

inline vec3
operator + (float scalar, vec3 vector) {
    vec3 result;
    result.x = scalar + vector.x;
    result.y = scalar + vector.y;
    result.z = scalar + vector.z;
    return result;    
}

inline vec3
operator + (vec3 lhs, vec3 rhs) {
    vec3 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    return result;    
}

inline vec3
operator - (vec3 lhs, vec3 rhs) {
    vec3 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    return result;    
}

inline vec3
operator - (vec3 vector) {
    vec3 result;
    result.x = -vector.x;
    result.y = -vector.y;
    result.z = -vector.z;
    return result;
}

struct vec4 {
    float x;
    float y;
    float z;
    float w;
    
    vec4() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 0.0f;
    }
    
    vec4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    vec4(vec3 vec, float w) {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
        this->w = w;    
    }
    bool operator == (vec4 rhs) {
        return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z) && (this->w == rhs.w);
    }

    bool operator != (vec4 rhs) {
        return !(*this == rhs);
    }
};


inline vec4
operator * (vec4 vector, float scalar) {
    vec4 result;
    result.x = vector.x * scalar;
    result.y = vector.y * scalar;
    result.z = vector.z * scalar;
    result.w = vector.w * scalar;
    return result;
}

inline vec4
operator * (float scalar, vec4 vector) {
    vec4 result;
    result.x = vector.x * scalar;
    result.y = vector.y * scalar;
    result.z = vector.z * scalar;
    result.w = vector.w * scalar;
    return result;
}

inline vec4
operator / (vec4 vector, float scalar) {
    vec4 result;
    result.x = vector.x / scalar;
    result.y = vector.y / scalar;
    result.z = vector.z / scalar;
    result.w = vector.w / scalar;
    return result;    
}

inline vec4
operator + (vec4 lhs, vec4 rhs) {
    vec4 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    result.w = lhs.w + rhs.w;
    return result;    
}

inline vec4
operator - (vec4 lhs, vec4 rhs) {
    vec4 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    result.w = lhs.w - rhs.w;
    return result;    
}

inline vec4
operator - (vec4 vector) {
    vec4 result;
    result.x = -vector.x;
    result.y = -vector.y;
    result.z = -vector.z;
    result.w = -vector.w;
    return result;
}

struct mat33 {
    float data[3][3];
    
    mat33() {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                this->data[i][j] = 0.0f;
            }
        }
    }
    
    mat33(float entry_11, float entry_12, float entry_13,
          float entry_21, float entry_22, float entry_23,
          float entry_31, float entry_32, float entry_33) {

        this->data[0][0] = entry_11;
        this->data[0][1] = entry_12;
        this->data[0][2] = entry_13;

        this->data[1][0] = entry_21;
        this->data[1][1] = entry_22;
        this->data[1][2] = entry_23;
        
        this->data[2][0] = entry_31;
        this->data[2][1] = entry_32;
        this->data[2][2] = entry_33;
    }
    
    mat33 operator = (mat33 rhs) {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                this->data[i][j] = rhs.data[i][j];
            }
        }
        return *this;
    }

    bool operator == (mat33 rhs) {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(this->data[i][j] != rhs.data[i][j])
                    return false;
            }
        }
        return true;
    }
};

inline vec3
operator * (vec3 vector, mat33 matrix) {
    vec3 result;
    result.x = vector.x * matrix.data[0][0] + vector.y * matrix.data[1][0] + vector.z * matrix.data[2][0];
    result.y = vector.x * matrix.data[0][1] + vector.y * matrix.data[1][1] + vector.z * matrix.data[2][1];
    result.z = vector.x * matrix.data[0][2] + vector.y * matrix.data[1][2] + vector.z * matrix.data[2][2];
    return result;
}

struct mat44 {
    float data[4][4];
    
    mat44() {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                this->data[i][j] = 0.0f;
            }
        }
    }
    
    mat44(float entry_11, float entry_12, float entry_13, float entry_14,
          float entry_21, float entry_22, float entry_23, float entry_24,
          float entry_31, float entry_32, float entry_33, float entry_34,
          float entry_41, float entry_42, float entry_43, float entry_44) {

        this->data[0][0] = entry_11;
        this->data[0][1] = entry_12;
        this->data[0][2] = entry_13;
        this->data[0][3] = entry_14;

        this->data[1][0] = entry_21;
        this->data[1][1] = entry_22;
        this->data[1][2] = entry_23;
        this->data[1][3] = entry_24;
        
        this->data[2][0] = entry_31;
        this->data[2][1] = entry_32;
        this->data[2][2] = entry_33;
        this->data[2][3] = entry_34;
        
        this->data[3][0] = entry_41;
        this->data[3][1] = entry_42;
        this->data[3][2] = entry_43;
        this->data[3][3] = entry_44;
    }
    
    mat44 operator = (mat44 rhs) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                this->data[i][j] = rhs.data[i][j];
            }
        }
        return *this;
    }

    bool operator == (mat44 rhs) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                if(this->data[i][j] != rhs.data[i][j])
                    return false;
            }
        }
        return true;
    }
};

inline mat44
operator * (mat44 lhs, mat44 rhs) {
    mat44 result;
    float ElementValue;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            ElementValue = 0;
            for(int k = 0; k < 4; ++k) {
                ElementValue += (lhs.data[i][k] * rhs.data[k][j]);
            }
            result.data[i][j] = ElementValue;
        }
    }
    return result;
}

inline void
operator *= (mat44 &lhs, mat44 rhs) {
    lhs = lhs * rhs;
}

inline mat44
operator * (mat44 matrix, float scalar) {
    mat44 result;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result.data[i][j] = matrix.data[i][j] * scalar;
        }
    }
    return result;
}

inline mat44
operator * (float scalar, mat44 matrix) {
    mat44 result;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result.data[i][j] = matrix.data[i][j] * scalar;
        }
    }
    return result;
}

// NOTE(Alex): Assumes the matrix's fourth column is set to (0, 0, 0, 1) for accurate results.
inline vec4
operator * (vec4 vector, mat44 matrix) {
    vec4 result;
    result.x = vector.x * matrix.data[0][0] + vector.y * matrix.data[1][0] +
        vector.z * matrix.data[2][0] + vector.w * matrix.data[3][0];
    result.y = vector.x * matrix.data[0][1] + vector.y * matrix.data[1][1] +
        vector.z * matrix.data[2][1] + vector.w * matrix.data[3][1];
    result.z = vector.x * matrix.data[0][2] + vector.y * matrix.data[1][2] +
        vector.z * matrix.data[2][2] + vector.w * matrix.data[3][2];
    result.w = vector.x * matrix.data[0][3] + vector.y * matrix.data[1][3] +
        vector.z * matrix.data[2][3] + vector.w * matrix.data[3][3];
    return result;
}

inline mat44
operator + (mat44 lhs, mat44 rhs) {
    mat44 result;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result.data[i][j] = lhs.data[i][j] + rhs.data[i][j];
        }
    }
    return result;
}

inline mat44
operator - (mat44 lhs, mat44 rhs) {
    mat44 result;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result.data[i][j] = lhs.data[i][j] - rhs.data[i][j];
        }
    }
    return result;
}


struct quat {
    float x;
    float y;
    float z;
    float w;

    quat() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 1.0f;
    }

    bool operator == (quat rhs) {
        if(this->x != rhs.x ||
           this->y != rhs.y ||
           this->z != rhs.z ||
           this->w != rhs.w) {
            return false;
        }
        return true;
    }
};

vec3 cross(vec3 lhs, vec3 rhs);
float dot(vec3 lhs, vec3 rhs);

inline quat
operator * (quat lhs, quat rhs) {
    quat result;
    vec3 complex1 = vec3(lhs.x, lhs.y, lhs.z);
    vec3 complex2 = vec3(rhs.x, rhs.y, rhs.z);
    vec3 complex = (complex1 * rhs.w) + (complex2 * lhs.w) + cross(complex1, complex2);

    result.x = complex.x;
    result.y = complex.y;
    result.z = complex.z;
    result.w = (lhs.w * rhs.w) - dot(complex1, complex2);
    return result;
}

inline quat
operator * (float scalar, quat rhs) {
    quat result;
    result.x = scalar * rhs.x;
    result.y = scalar * rhs.y;
    result.z = scalar * rhs.z;
    result.w = scalar * rhs.w;
    return result;
}

inline quat
operator * (quat lhs, float scalar) {
    quat result;
    result.x = lhs.x * scalar;
    result.y = lhs.y * scalar;
    result.z = lhs.z * scalar;
    result.w = lhs.w * scalar;
    return result;
}

inline quat
operator / (quat lhs, float scalar) {
    quat result;
    result.x = lhs.x / scalar;
    result.y = lhs.y / scalar;
    result.z = lhs.z / scalar;
    result.w = lhs.w / scalar;
    return result;
}

inline quat
operator + (quat lhs, quat rhs) {
    quat result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    result.w = lhs.w + rhs.w;
    return result;
}

inline float
magnitude(vec2 source) {
    return sqrtf(source.x * source.x + source.y * source.y);
}
                                                \
inline float
magnitude(vec3 source) {
    return sqrtf(source.x * source.x + source.y * source.y + source.z * source.z);
}

inline float
magnitude(vec4 source) {
    return sqrtf(source.x * source.x + source.y * source.y + source.z * source.z + source.w * source.w);
}

float magnitude(quat q) {
    return (float)sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

inline vec2
normalize(vec2 source) {
    if(source == vec2())
        return source;
    
    vec2 result;
    float source_length = magnitude(source);
    float multiplier = 1.0f / source_length;
    
    result.x = source.x * multiplier;
    result.y = source.y * multiplier;
    
    return result;
}

inline vec3
normalize(vec3 source) {
    if(source == vec3()) {
        return source;
    }
    vec3 result;
    float source_length = magnitude(source);
    float multiplier = 1.0f / source_length;
    
    result.x = source.x * multiplier;
    result.y = source.y * multiplier;
    result.z = source.z * multiplier;
    
    return result;
}

inline vec4
normalize(vec4 source) {
    if(source == vec4()) {
        return source;
    }
    vec4 result;
    float source_length = magnitude(source);
    float multiplier = 1.0f / source_length;
    
    result.x = source.x * multiplier;
    result.y = source.y * multiplier;
    result.z = source.z * multiplier;
    result.w = source.w * multiplier;
    
    return result;
}

inline quat
normalize(quat q) {
    if(q == quat()) {
        return q;
    }
    return q / magnitude(q);
}

inline float
dot(vec3 lhs, vec3 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline float
dot(vec4 lhs, vec4 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

inline float
dot(quat lhs, quat rhs) {
    return(lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w);
}

vec3
cross(vec3 lhs, vec3 rhs) {
    vec3 result;
    result.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
    result.y = (lhs.z * rhs.x) - (lhs.x * rhs.z);
    result.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
    return result;
}

vec4
cross(vec4 lhs, vec4 rhs) {
    vec4 result;
    result.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
    result.y = (lhs.z * rhs.x) - (lhs.x * rhs.z);
    result.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
    return result;
}

void
scale(vec3 *vector, float x, float y, float z) {
    vector->x *= x;
    vector->y *= y;
    vector->z *= z;
}

void
scale(vec4 *vector, float x, float y, float z, float w) {
    vector->x *= x;
    vector->y *= y;
    vector->z *= z;
    vector->w *= w;
}

void
rotate(vec3 *vector, float angle, int axis_of_rotation) {
    vec3 vec = *vector;
    switch(axis_of_rotation) {
        case X_AXIS: {
            vector->y = vec.y * (float)cos(angle) + vec.z * -1*(float)sin(angle);
            vector->z = vec.y * (float)sin(angle) + vec.z * (float)cos(angle);
        } break;
        
        case Y_AXIS: {
            vector->x = vec.x * (float)cos(angle) + vec.z * (float)sin(angle);
            vector->z = vec.x * -1*(float)sin(angle) + vec.z * (float)cos(angle);
        } break;
        
        case Z_AXIS: {
            vector->x = vec.x * (float)cos(angle) + vec.y * -1*(float)sin(angle);
            vector->y = vec.x * (float)sin(angle) + vec.y * (float)cos(angle);      
        } break;
    }
}

void 
rotate(vec3 *vector, float angle, vec3 point, vec3 line) { //rotates vector about "line" going through "point"
    vec3 vec = *vector;
    vector->x = (point.x*(line.y*line.y + line.z*line.z) - line.x*(point.y*line.y + point.z*line.z - line.x*vec.x - line.y*vec.y - line.z*vec.z)) *
        (1.0f - (float)cos(angle)) + vec.x*(float)cos(angle) + ((-point.z)*line.y + point.y*line.z - line.z*vec.y + line.y*vec.z) * (float)sin(angle);
    
    vector->y = (point.y*(line.x*line.x + line.z*line.z) - line.y*(point.x*line.x + point.z*line.z - line.x*vec.x - line.y*vec.y - line.z*vec.z)) *
        (1.0f - (float)cos(angle)) + vec.y*(float)cos(angle) + (point.z*line.x - point.x*line.z + line.z*vec.x - line.x*vec.z) * (float)sin(angle);
    
    vector->z = (point.z*(line.x*line.x + line.y*line.y) - line.z*(point.x*line.x + point.y*line.y - line.x*vec.x - line.y*vec.y - line.z*vec.z)) *
        (1.0f - (float)cos(angle)) + vec.z*(float)cos(angle) + ((-point.y)*line.x + point.x*line.y - line.y*vec.x + line.x*vec.y) * (float)sin(angle);
}

void
rotate(vec4 *vector, float angle, int axis_of_rotation) {
    switch(axis_of_rotation) {
        case X_AXIS: {
            vector->y = vector->y * (float)cos(angle) + vector->z * -1*(float)sin(angle);
            vector->z = vector->y * (float)sin(angle) + vector->z * (float)cos(angle);
        } break;
        
        case Y_AXIS: {
            vector->x = vector->x * (float)cos(angle) + vector->z * (float)sin(angle);
            vector->z = vector->x * -1*(float)sin(angle) + vector->z * (float)cos(angle);
        } break;
        
        case Z_AXIS: {
            vector->x = vector->x * (float)cos(angle) + vector->y * -1*(float)sin(angle);
            vector->y = vector->x * (float)sin(angle) + vector->y * (float)cos(angle);      
        } break;
    }
}

mat33
transpose(mat33 matrix) {
    mat33 result;
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            result.data[i][j] = matrix.data[j][i];
        }       
    }
    return result;
}

mat44
transpose(mat44 matrix) {
    mat44 result;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result.data[i][j] = matrix.data[j][i];
        }       
    }
    return result;
}

mat44
make_scaling_matrix(float x, float y, float z, float w) {
    return mat44(x, 0.0f, 0.0f, 0.0f,
                 0.0f, y, 0.0f, 0.0f,
                 0.0f, 0.0f, z, 0.0f,
                 0.0f, 0.0f, 0.0f, w);   
}

inline bool
ray_intersects_triangle(vec3 ro, vec3 rd, vec3 v0, vec3 v1, vec3 v2, vec3 n, float &t) {
    t = dot(n, v0 - ro) / dot(n, rd);
    vec3 Q = rd * t + ro;
    if(dot(n, (cross(Q - v0, v1 - v0))) < 0.0f ||
       dot(n, (cross(Q - v1, v2 - v1))) < 0.0f ||
       dot(n, (cross(Q - v2, v0 - v2))) < 0.0f) {
        return false;
    }
    return true;
}

inline float
find_scalar_multiple(vec3 a, vec3 b) {
    float EPSILON = 0.000001f;
    float cross_product_magnitude = magnitude(cross(a, b));
    if((cross_product_magnitude < -EPSILON) || (cross_product_magnitude > EPSILON)) {
        return 0.0f;
    }
    if(b.x != 0.0f) {
        return a.x / b.x;
    }
    if(b.y != 0.0f) {
        return a.y / b.y;
    }
    if(b.z != 0.0f) {
        return a.z / b.z;
    }
    return 0.0f;
}

quat
quat_from_euler_angles(vec3 euler_angles) {
    quat result;
    
    float cp = (float)cos(euler_angles.x * 0.5);
    float sp = (float)sin(euler_angles.x * 0.5);
    float cy = (float)cos(euler_angles.y * 0.5);
    float sy = (float)sin(euler_angles.y * 0.5);
    float cr = (float)cos(euler_angles.z * 0.5);
    float sr = (float)sin(euler_angles.z * 0.5);

    result.x = cy * sr * cp - sy * cr * sp;
    result.y = cy * cr * sp + sy * sr * cp;
    result.z = sy * cr * cp - cy * sr * sp;
    result.w = cy * cr * cp + sy * sr * sp;

    return result;
}

mat44
rotation_matrix(quat rotation) {
    return mat44(1.0f - 2.0f * rotation.y*rotation.y - 2.0f * rotation.z*rotation.z,
                 2.0f * rotation.x * rotation.y - 2.0f * rotation.z * rotation.w,
                 2.0f * rotation.x * rotation.z + 2.0f * rotation.y * rotation.w,
                 0.0f,
                 
                 2.0f * rotation.x * rotation.y + 2.0f * rotation.z * rotation.w,
                 1.0f - 2.0f * rotation.x*rotation.x - 2.0f * rotation.z*rotation.z,
                 2.0f * rotation.y * rotation.z - 2.0f * rotation.x * rotation.w,
                 0.0f,
                 
                 2.0f * rotation.x * rotation.z - 2.0f * rotation.y * rotation.w,
                 2.0f * rotation.y * rotation.z - 2.0f * rotation.x * rotation.w,
                 1.0f - 2.0f * rotation.x*rotation.x - 2.0f * rotation.y*rotation.y,
                 0.0f,

                 0.0f, 0.0f, 0.0f, 1.0f);
}

mat44
model_transform(vec3 world_pos, quat rotation) {
    mat44 translation = mat44(1.0f, 0.0f, 0.0f, world_pos.x,
                              0.0f, 1.0f, 0.0f, world_pos.y,
                              0.0f, 0.0f, 1.0f, world_pos.z,
                              0.0f, 0.0f, 0.0f, 1.0f);

    mat44 rot = mat44(1.0f - 2.0f * rotation.y*rotation.y - 2.0f * rotation.z*rotation.z,
                      2.0f * rotation.x * rotation.y - 2.0f * rotation.z * rotation.w,
                      2.0f * rotation.x * rotation.z + 2.0f * rotation.y * rotation.w,
                      0.0f,
                 
                      2.0f * rotation.x * rotation.y + 2.0f * rotation.z * rotation.w,
                      1.0f - 2.0f * rotation.x*rotation.x - 2.0f * rotation.z*rotation.z,
                      2.0f * rotation.y * rotation.z - 2.0f * rotation.x * rotation.w,
                      0.0f,
                 
                      2.0f * rotation.x * rotation.z - 2.0f * rotation.y * rotation.w,
                      2.0f * rotation.y * rotation.z - 2.0f * rotation.x * rotation.w,
                      1.0f - 2.0f * rotation.x*rotation.x - 2.0f * rotation.y*rotation.y,
                      0.0f,

                      0.0f, 0.0f, 0.0f, 1.0f);

    return translation * rot;
};

mat44
view_transform(vec3 position, vec3 direction, vec3 up) {
    up = normalize(up);
    vec3 forward = normalize(direction);
    vec3 right = normalize(cross(up, forward)); 

    
    mat44 translation = mat44(1.0f, 0.0f, 0.0f, -position.x,
                              0.0f, 1.0f, 0.0f, -position.y,
                              0.0f, 0.0f, 1.0f, -position.z,
                              0.0f, 0.0f, 0.0f,  1.0f);

    mat44 orientation = mat44(right.x,   right.y,   right.z,   0.0f,
                              up.x,      up.y,      up.z,      0.0f,
                              forward.x, forward.y, forward.z, 0.0f,
                              0.0f,      0.0f,      0.0f,      1.0f);

    return orientation * translation;
}

mat44
perspective(float FOV, float aspect, float z_near, float z_far) {
    mat44 result;
    float tan_half_FOV = (float)tan(FOV * 0.5 * (float)PI / 180.0f); 
    result.data[0][0] = 1.0f / (tan_half_FOV * aspect); 
    result.data[1][1] = 1.0f / tan_half_FOV; 
    result.data[2][2] = (z_far + z_near) / (z_far - z_near); 
    result.data[3][2] = -(2.0f * z_far * z_near) / (z_far - z_near);
    result.data[2][3] = 1.0f;
    return result;
}

inline float
sign(float t) {
    return (t < 0.0f)? -1.0f : 1.0f;
}

inline float
lerp(float a, float b, float t) {
    return t*b + (1.0f - t)*a;
}

inline vec2
lerp(vec2 a, vec2 b, float t) {
    return t*b + (1.0f - t)*a;
}

inline vec3
lerp(vec3 a, vec3 b, float t) {
    return t*b + (1.0f - t)*a;
}

inline quat
lerp(quat a, quat b, float t) {
    return normalize(t*b + (1.0f - t)*a);
}

inline quat
shortest_lerp(quat a, quat b, float t) {
    return normalize(sign(dot(a, b)) * (1.0f - t)*a + t*b);
}

inline int
absi(int x) {
    return (x < 0)? -x : x;
}

inline float
absf(float x) {
    return (x < 0)? -x : x;
}

inline float
pos_min(float x, float y) {
    if((x >= 0) && (x < y)) {
        return x;
    }
    if((y >= 0) && (y < x)) {
        return y;
    }
    return min(x, y);
}

inline float
dtr(float degrees) {
    return (degrees / 180.0f) * (float)PI;
}
