#include <iostream>
#include <vector>


template<typename T>
class vector{
    public:
    //type definitions
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef T value_type;


    // Constructors, destructors, assignement operators
        vector(){
            allocate(2);
        }
        
        ~vector(){
            delete[] data;
        }

        vector& operator= (const vector& x){
            //copy parameters of x vector
            this->m_capacity = x.capacity();
            this->m_size = x.size();
            
            //creating vector with the same parameters
            T* newBlock = new T[this->m_capacity];

            //Copy values into new vector
            for(size_t i = 0; i < this->m_size; i++){
                newBlock[i] = x[i];
            }

            //delete old vector and assign new one
            delete[] this->data;
            this->data = newBlock;

            return *this;
        }


    // Iterators

    // Capacity
        size_t capacity() const{
            return m_capacity;
        }

        size_t size() const{
            return m_size;
        }

    // Element acess
        T& operator[] (size_t index) const{
            return data[index];
        }


    // Modifiers 
        void push_back(const T& value){
            // checking for free m_capacity, if not expanding array
            if(m_size >= m_capacity){
                allocate(m_capacity+m_capacity/2);
            }

            data[m_size] = value;
            m_size++;
        }

        void  pop_back(){
            //  remove last element of array
            //  after testing, std::vector seems to just decrease array size
            m_size -=1;
            
        }

        iterator insert(const_iterator position, const value_type& val){
            //insert one element into array

            //creating new array
            T* newBlock = new T[size+1];
            // geting index value of position
            int index = std::distance(data,position);
            for(size_t i = 0; i<size; i++){
                if(i > index) newBlock[i+1] = data[i];
                else if(i == index) newBlock[i] = val;
                else newBlock[i] = data[i];
            }
            
            //deleting old array
            delete[] data;
            //assing new array
            data = newBlock;
            //update size;
            size +=1; 
            
            return data[index];
            
        }

    // Allocator

    


    private:
    //members
        T* data = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
    // private methods
        void allocate(size_t newCapacity){
                //creating new block
                T* newBlock = new T[newCapacity]; 
                // copying previuos elements into new array
                if(newCapacity < m_size) m_size = newCapacity;
                for(size_t i=0; i < m_size; i++){
                    newBlock[i] = data[i];
                }
                //deleting old array and assinging new one
                delete[] data;
                data = newBlock;
                m_capacity = newCapacity;
        }
};

int main(){ 
    vector<int> container;
    vector<int> container2;
    int a = 1;
    container2.push_back(1);
    container2.push_back(2);
    container2.push_back(3);

    container = container2;
    // container2[1] = 4;

    container.insert(*container[1]; 9);

    // vector<std::string> stringContainer;
    // stringContainer.push_back("Abc");
    // stringContainer.push_back("EBC");

    // std::cout<<stringContainer[0]<<stringContainer[1]<<"\n";

    std::cout<<container[0]<<container[1]<<container[2]<<container[3]<<"\n";

}