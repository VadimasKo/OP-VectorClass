# OP-VectorClass
Recreating std::vector CPP
reference - https://www.cplusplus.com/reference/vector/vector/

## Spartos tyrimai
   

| Konteineris/dydis | 10 000      | 100 000    | 1 000 000 |  10 000 000 | 100 000 000 |
|-------------------|-------------|------------|-----------|-------------|-------------|
| std::vector       | 0.000637811 | 0.00555732 | 0.0409649 | 0.224621    | 2.01388     |
| vector            | 0.000629882 | 0.00595285 | 0.0195125 | 0.230119    | 2.06794     |

## Funkciju pavyzdziai
* void push_back(const T& val)
    * Funkcija prideda val reiksme i array konteinerio pabaiga
    * Jeigu capacity = size, funkcija isskyria daugiau vietos
    * funkcijos testas:
    ```
    int main(){
        vector<int> test;
        test.push_back(1);
        test.push_back(2);
        test.push_back(3);
        std::cout<<test[0]<<test[1]<<test[2]<<'\n';
    }
    ```
    * rezultatai:
    ```
        [broom@broom-80wk OP-VectorClass]$ g++ vector.cpp 
        [broom@broom-80wk OP-VectorClass]$ ./a.out 
        123
    ```
* void pop_back()
    * Funkcija pasalina paskutini elementa,
    * efektyviai sumazina konteinerio dydi 1
    * funkcijos testas:
    ```
    int main(){
        vector<int> test;
        test.push_back(1);
        test.push_back(2);
        test.pop_back();
        test.push_back(5);
        std::cout<<test[0]<<test[1]<<test[2]<<'\n';
    }
    ```
    * rezultatai:
    ``` 
        [broom@broom-80wk OP-VectorClass]$ g++ vector.cpp 
        [broom@broom-80wk OP-VectorClass]$ ./a.out 
        150
    ```

* iterator insert (const_iterator position, const value_type& val)
    * Funkcija iterpia val reiskmia i nurodyta pozicija,
    * veikia jeigu nurodyta pozicija pabaiga>pos>pradzia
    * funkcijos testas:
    ```
    int main(){
        vector<int> test;
        test.push_back(1);
        test.push_back(2);
        test.push_back(3);
        std::cout<<test[0]<<test[1]<<test[2]<<test[3]<<'\n';
        test.insert(test.begin()+1,9);
        std::cout<<test[0]<<test[1]<<test[2]<<test[3]<<'\n';
    }
    ```
    * rezultatai:
    ``` 
        [broom@broom-80wk OP-VectorClass]$ g++ vector.cpp 
        [broom@broom-80wk OP-VectorClass]$ ./a.out 
        1230
        1923
    ```

* iterator insert (iterator position, size_type n, const T& val)
    * Funkcija iterpia n kieki val reiskmiu i nurodyta pozicija,
    * veikia jeigu nurodyta pozicija pabaiga>pos>pradzia
    * funkcijos testas:
    ```
    int main(){
        vector<int> test;
        test.push_back(1);
        test.push_back(2);
        test.push_back(3);
        std::cout<<test[0]<<test[1]<<test[2]<<test[3]<<'\n';
        test.insert(test.begin()+1,3,9);
        std::cout<<test[0]<<test[1]<<test[2]<<test[3]<<test[4]<<test[5]<<test[6]<<'\n';
    }
    ```
    * rezultatai:
    ``` 
        [broom@broom-80wk OP-VectorClass]$ g++ vector.cpp 
        [broom@broom-80wk OP-VectorClass]$ ./a.out 
        1230
        1999230
    ```
* T& operator[](size_type i)
    * Funkcija grazina reiksme saugoma konteinerio i vietoje
    * funkcijos testas:
    ```
    int main(){
        int main(){
        vector<int> test;
        test.push_back(1);
        test.push_back(2);
        test.push_back(3);
        std::cout<<test[0]<<test[1]<<test[2]<<'\n';
    }
    ```
    * rezultatai:
    ``` 
        [broom@broom-80wk OP-VectorClass]$ g++ vector.cpp 
        [broom@broom-80wk OP-VectorClass]$ ./a.out 
        123
    ```

