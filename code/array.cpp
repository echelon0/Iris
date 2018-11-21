
template <typename T>
struct Array {
    T *data;
    int size;
    int capacity;

    Array() {
        size = 0;
        capacity = 1;
        data = (T *)calloc(capacity, sizeof(T));
    }
    
    T &operator[](int i) {
        return data[i];
    }
    
    void PushBack(T entry) {
        if(size >= capacity) {
            data = (T *)realloc(data, sizeof(T) * capacity * 2);
            capacity *= 2;
        }
        data[size++] = entry;
    }

    bool Contains(T entry) {
        for(int i = 0; i < size; i++) {
            if(data[i] == entry) {
                return true;
            }
        }
        return false;
    }
};
