#include <memory>
#include <iostream>

// template<class T>
// class allocator {
//     public:
//         T* allocate(size_t);          //isskirti raw atminti
//         void deallocate(T*, size_t);  // atlaisvinti atminti
//         void construct(T*, const T&); //sukonstruoti 1 objekta
//         void destroy(T*);             // sunaikinti 1 objekta
//         //-+
// };

template <class T>
class vector{
    public:
    //typedefs
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef size_t size_type;
        typedef T value_type;

    //rule of three
        vector(){ allocate(); }
        explicit vector(size_type n, const T& t = T{} ) { allocate (n,t); }
        vector(const vector& v ){ allocate(v.begin(), v.end()); }
        vector& operator=(const vector&);
        ~vector(){ deallocate(); }

    // iterators
        iterator begin() {return data; }
        const_iterator begin() const { return data; }
        iterator end() {return last; }
        const_iterator end() const {return last; }


    // Capacity
        size_type size();



    //Element acess
        T& operator[](size_type i) { return data[i]; }
        const T& operator[](size_type i) const { return data[i]; }


    // Modifiers
        void push_back(const T& val){
            if(last == limit) grow();
            unchecked_append(val);
        }

    private:
        iterator data;  // rodykle i pirma array elementa
        iterator last;  // rodykle i pask sukonstr. elementa
        iterator limit; // rodykle i allocated atminties gala

    // atminties isskyrimo valdymui
        std::allocator<T> alloc; //objektas atminties valdymui

    // isskirti atminti array ir ja incializuoti
    void allocate(){
        data = last = limit = nullptr;
    }

    void allocate(size_type n, const T& val){
        data = alloc.allocate(n); //isskyria raw atminti
        limit = last = data+n;
        unintialized_fill(data, last, val); // uzpildo visus elementus nuo data iki last su val
    }

    void allocate(const_iterator i, const_iterator j){
        data = alloc.allocate(j-i); //iskyria vietos
        limit = last = unintialized_copy(i,j,data); //nukopijoja elementus is intervalo
    }

    //sunaikinti array elementus ir atlaisvinti atminti
    void deallocate(){
        if (data) { 
         //sunaikinti atbuline tvarka sukonstruotus elementus
            iterator it = last;
            while(it != data){
                alloc.destroy(it);
                it--;
            }

            alloc.deallocate(data,limit - data);
        }
        data = limit = last = nullptr;
    }

    // pagalbines funkcijos
    void grow(){
        //dvigubai daugiau vietos
        size_type new_size = std::max(2 * (limit - data), ptrdiff_t(1));

        //allocate nauja vieta ir copy egz. elem.
        iterator new_data = alloc.allocate(new_size);
        iterator new_last = std::uninitialized_copy(data,last,new_data);

        //atlaisvinti sena vieta
        deallocate();

        //resetint rodykles i nauja vieta
        data = new_data;
        last = new_last;
        limit = data + new_size;

    };

    void unchecked_append(const T& val){
        alloc.construct(last++,val);
    }

};



int main(){
    vector<int> a;
    a.push_back(1);
    a.push_back(2);
    std::cout<<a[0]<<a[1]<<"\n";
}