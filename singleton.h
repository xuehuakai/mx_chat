#ifndef SINGLETON_H
#define SINGLETON_H
#include"global.h"
/**
 * @brief 单例类
 * /

/*类的static成员初始化：模板类需要在cpp中   非模板类需要在.h中*/


template<typename T>
class Singleton{
protected:
    Singleton() = default;
    Singleton(const Singleton<T> & ) = delete; //拷贝构造
    Singleton& operator = (const Singleton<T>& st) = delete; //拷贝赋值
    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag s_flag;
        std::call_once(s_flag,[&](){
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    void PrintAddress(){
        std::cout<<_instance.get()<<std::endl;
    }

    ~Singleton(){
        std::cout<<"This is singleton destruct ." <<std::endl;
    }

};

template<typename T>
std::shared_ptr<T>  Singleton<T>::_instance = nullptr;


#endif // SINGLETON_H
