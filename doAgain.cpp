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
        void resize(size_type n, const T& val = T{}){
            if( n > size() ){
                std::cout<<n-size()<<"\n";
                insert(end(), n-size(),val);
            }
            else if (n< size()){
                erase(begin()+n, end());
            }
        }
        size_type capacity() const noexcept{
            return limit-data;
        }
        bool empty() const noexcept{
            return data-dataEnd==0;
        }
        void reserve (size_type n){
            if(n > capacity()){
                grow(n);
            }
        }
        void shrink_to_fit(){
            grow(size());
        }


    //Element acess
        T& operator[](size_type i) { return data[i]; }
        const T& operator[](size_type i) const { return data[i]; }
    //--at
        T& at (size_type n){
            return data[n];
        }
        const T& at (size_type n) const{
            return data[n];
        }
    //--front 
        T& front () { return *data; }
        const T& front() const { return data; }
    //--back
        T& back() { return *(dataEnd-1);}
        const T& back() const { return *(dataEnd-1);}
    //--data
        T* data1() noexcept{
            return data;
        }
        const T* data1() const noexcept{
            return data;
        }

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
        iterator erase(iterator position){
            if( position <begin()|| position>end()) { return position;} //should add error?
            else{
                alloc.destroy(position);
                dataEnd = std::uninitialized_copy(position+1, end(), position); //copy all elements from position + to position
                return position;
            }
        }

        iterator erase (iterator first, iterator last){
            if (first<begin() || last>end()) { return first;} //error
            else{
                iterator it = last;
                while(it != first) alloc.destroy(--it); //delete elements in range [first,last)

                dataEnd = std::uninitialized_copy(last, end(), first); // copy elements from [last,end())

                return first+1; //return 1st element, behind deleted elements

            }
        }
    //--swap
        void swap (vector& x){
            iterator buff_data = x.data;
            iterator buff_limit = x.limit;
            iterator buff_dataEnd = x.dataEnd;

            x.data = data;
            x.dataEnd = dataEnd;
            x.limit = limit;

            data = buff_data;
            dataEnd = buff_dataEnd;
            limit = buff_limit;
        }
    //--clear
        void clear() noexcept{ // whats the diff between it and destructor?
            deallocate();
        }
    //--emplace
        template <class... Args>
        iterator emplace (iterator position, Args&&... args){ //Variadic function templates in C++
            if(position < begin() || position > end() ){ return position; }//error
            else{
                int index = std::distance(begin(),position);
                //patikrinam ar uztenka vietos
                if (dataEnd == limit) grow();
                position = data + index;
                
                for(iterator i = dataEnd()-1; i>=position; i--) alloc.construct(i+1,*i); //pajudinti visus elem i desine 
                
                dataEnd++; //update dataEnd

                alloc.construct(position,std::forward<Args>(args)...);

                return position;
            }
        }
        template <class... Args>
        void emplace_back (Args&&... args){
            alloc.construct(dataEnd++, std::forward<Args>(args)...);
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
        limit = dataEnd = std::uninitialized_copy(i,j,data); //nukopijoja elementus is intervalo
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

    void grow(size_type new_size){

        //allocate nauja vieta ir copy egz. elem.
        iterator new_data = alloc.allocate(new_size);
        iterator new_last = std::uninitialized_copy(data,dataEnd,new_data);

        //atlaisvinti sena vieta
        deallocate();

        //resetint rodykles i nauja vieta
        data = new_data;
        dataEnd = new_last;
        limit = data + new_size;
    }


    void unchecked_append(const T& val){
        alloc.construct(dataEnd++,val);
    }

};



int main(){
    vector<int> a;
    a.push_back(1);
    a.push_back(2);
    vector<int> b(a);
    // a[0]= 2;
    // a[1]= 7;
    // a.swap(b);
    // a.resize(10,3);
    // a.insert(a.end(), 7-a.size(),7);
    a.reserve(100);
    a.shrink_to_fit();
    std::cout<<a.capacity()<<"\n";
    std::cout<<a[0]<<a[1]<<a[2]<<a[3]<<a[4]<<a[5]<<""<<"\n";
    // std::cout<<b[0]<<b[1]<<b[2]<<a[3]<<a[4]<<a[5]<<""<<"\n";
    std::cout<<a.at(2);
}