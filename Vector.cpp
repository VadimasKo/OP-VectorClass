
#include <iostream>


template<typename T>
class Vector{
    public: //constructors, destructors, assing operators
        Vector(){
            allocate(2);
            std::cout<<"Construct \n";
        }

        ~Vector(){
            delete[] data;
        }


    public: //capacity
        size_t size() const{
            return size;
        }


    
    public: //modifiers
        void push_back(const T& value){
            std::cout<<"pushback \n";

            // checking for free capacity, if not expanding array
            if(container_size >= capacity){
                allocate(capacity+capacity/2);
            }

            data[container_size] = value;
            container_size++;
        }
    
    public: //element acess
        T& operator[] (size_t index) const{
            return data[index];
        }


    private: 
        void allocate(size_t newCapacity){
            std::cout<<"Allocate \n";

            //creating new block
            T* newBlock = new T[newCapacity]; 
            // copying previuos elements into new array
            if(newCapacity < container_size) container_size = newCapacity;
            for(size_t i=0; i < container_size; i++){
                newBlock[i] = data[i];
            }
            //deleting old array and assinging new one
            delete[] data;
            data = newBlock;
            capacity = newCapacity;
        }

    private:
        T* data = nullptr;
        size_t container_size = 0;
        size_t capacity = 0;
};




int main(){ 
    Vector<int> container;
    int a = 1;
    container.push_back(1);
    container.push_back(2);
    container.push_back(3);
    std::cout<<container[0]<<container[1]<<container[2];

}