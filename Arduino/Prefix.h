

template<class T>
class Prefix{
    T* p;
public:
    Prefix(T* pp) :p(pp) { }
    T* operator->() { /* prefix code */ return p; }
};
