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
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        

    //rule of three
        //default
        vector(){ allocate(); }
        // sukuria n didumo vektoriu su t reiskme, nezinant t reiskmes sukuria n tusciu
        explicit vector(size_type n, const T& t = T{} ) { allocate (n,t); }
        // sukuria nauja vektoriu darydamas kito vekt copy
        vector(const vector& v ){ allocate(v.begin(), v.end()); }

        vector& operator=(const vector& rhs){
            //patikriname ar ne lygu
            if(&rhs != this){
                //atlaisviname seno this objekto atminti
                deallocate();
                //copy elementus is rhs i lhs
                allocate(rhs.begin(),rhs.end());
            }
            return *this;
        }
        ~vector(){ deallocate(); }

    // iterators
        iterator begin() {return data; }
        const_iterator begin() const { return data; }

        iterator end() {return dataEnd; }
        const_iterator end() const { return dataEnd; }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator  rbegin() const { return const_reverse_iterator(end()); }
        
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const const_reverse_iterator rend() const { return reverse_iterator(begin()); }


    // Capacity
        size_type size(){ return dataEnd - data; }
        size_type max_size(){ return alloc.max_size; }
        // void resize(size_type n, const T& = T{}){
        //     if( n<size()){}
        //     else if(n>size()) {

        //     }
        // }



    //Element acess
        T& operator[](size_type i) { return data[i]; }
        const T& operator[](size_type i) const { return data[i]; }


    // Modifiers
    //--assign
        void assign(size_type n, const T& val){
            deallocate();
            allocate(n,val);
        }

        template <class InputIterator>
        void assign (InputIterator first, InputIterator last){
            deallocate();
            allocate(first,last);
        }

    //--push_back
        void push_back(const T& val){
            if(dataEnd == limit) grow();
            unchecked_append(val);
        }
    //--pop_back
        void pop_back(){
            alloc.destroy(dataEnd--);
        }
    //--insert
        iterator insert(iterator position, const T& val){
            if(position< data || position > dataEnd){ return data; } //should add error
            else{
                int index = std::distance(data,position);
                if(dataEnd == limit) grow(); //changes pointer
                push_back(*(dataEnd-1));
                for(size_t i= size()-1; i>index; i-- ){
                    data[i]= data[i-1];
                }
                data[index] = val;

                return &data[index];
            }
            
        }
        iterator insert (iterator position, size_type n, const T& val){
            for(size_t i = 0; i< n; i++){
                position = insert(position,val);
            }
            return position;
        }
    //--erase
        iterator erase(const_iterator position){
            
        }


    private:
        iterator data;  // rodykle i pirma array elementa
        iterator dataEnd;  // rodykle i pask sukonstr. elementa
        iterator limit; // rodykle i allocated atminties gala

    // atminties isskyrimo valdymui
        std::allocator<T> alloc; //objektas atminties valdymui

    // isskirti atminti array ir ja incializuoti
    void allocate(){
        data = dataEnd = limit = nullptr;
    }

    void allocate(size_type n, const T& val){
        data = alloc.allocate(n); //isskyria raw atminti
        limit = dataEnd = data+n;
        unintialized_fill(data, dataEnd, val); // uzpildo visus elementus nuo data iki last su val
    }

    void allocate(const_iterator i, const_iterator j){
        data = alloc.allocate(j-i); //iskyria vietos
        limit = dataEnd = unintialized_copy(i,j,data); //nukopijoja elementus is intervalo
    }

    //sunaikinti array elementus ir atlaisvinti atminti
    void deallocate(){
        if (data) { 
         //sunaikinti atbuline tvarka sukonstruotus elementus
            iterator it = dataEnd;
            while(it != data){
                alloc.destroy(it);
                it--;
            }

            alloc.deallocate(data,limit - data);
        }
        data = limit = dataEnd = nullptr;
    }

    // pagalbines funkcijos
    void grow(){
        //dvigubai daugiau vietos
        size_type new_size = std::max(2 * (limit - data), ptrdiff_t(1));

        //allocate nauja vieta ir copy egz. elem.
        iterator new_data = alloc.allocate(new_size);
        iterator new_last = std::uninitialized_copy(data,dataEnd,new_data);

        //atlaisvinti sena vieta
        deallocate();

        //resetint rodykles i nauja vieta
        data = new_data;
        dataEnd = new_last;
        limit = data + new_size;

    };

    void unchecked_append(const T& val){
        alloc.construct(dataEnd++,val);
    }

};



int main(){
    vector<int> a;
    a.push_back(1);
    a.push_back(2);
    // a.pop_back();
    // a.push_back(3);
    // a.insert(a.begin()+1,9);
    a.insert(a.begin(),9);
    // a.insert(a.begin(),9);
    // a.insert(a.begin(),9);
    std::cout<<*(a.insert(a.begin()+2,2,7))<<"\n";
    std::cout<<a[0]<<a[1]<<a[2]<<a[3]<<a[4]<<a[5]<<""<<"\n";

}