
template <typename T>
struct array {
    T *Data;
    int Size;
    int Capacity;

    array() {
        Size = 0;
        Capacity = 1;
        Data = (T *)malloc(Capacity * sizeof(T));
    }
    
    T &operator[](int I) {
        return Data[I];
    }

    const T &operator[](int I) const {
        return Data[I];
    }    
    
    void PushBack(T Entry) {
        if(Size >= Capacity) {
            Data = (T *)realloc(Data, sizeof(T) * Capacity * 2);
            Capacity *= 2;
        }
        Data[Size++] = Entry;
    }

    bool Contains(T Entry) {
        for(int i = 0; i < Size; i++) {
            if(Data[i] == Entry) {
                return true;
            }
        }
        return false;
    }
};
