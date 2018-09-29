#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>

//智能指针
#define PTR(T) std::shared_ptr<T>
#define MKPTR(T) std::make_shared<T>
#define WPTR std::weak_ptr<T>
#define TOPTR(T1, T2) std::static_pointer_cast<T1>(T2)

//动态绑定
#ifndef BIND
#define BIND(p1, p2, ...)  std::bind(&p1, p2, ##__VA_ARGS__)
#define BIND1(p1, p2, ...) std::bind(&p1, p2, std::placeholders::_1, ##__VA_ARGS__)
#define BIND2(p1, p2, ...) std::bind(&p1, p2, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define BIND3(p1, p2, ...) std::bind(&p1, p2, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
#define BIND4(p1, p2, ...) std::bind(&p1, p2, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)
#define BIND5(p1, p2, ...) std::bind(&p1, p2, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, ##__VA_ARGS__)
#endif  //BIND

//静态绑定
#ifndef BIND_S
#define BIND_S(p1, ...)  std::bind(p1, ##__VA_ARGS__)
#define BIND1_S(p1, ...) std::bind(p1, std::placeholders::_1, ##__VA_ARGS__)
#define BIND2_S(p1, ...) std::bind(p1, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define BIND3_S(p1, ...) std::bind(p1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
#define BIND4_S(p1, ...) std::bind(p1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)
#define BIND5_S(p1, ...) std::bind(p1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, ##__VA_ARGS__)
#endif //SBIND

//MAP
#ifndef MAP
#if TEST
//测试状态下用map，因为用unordered_map的话，在Linux下有static对象的时候,valgrind会报内存泄露
#define MAP std::map
#else
#define MAP std::unordered_map
#endif
#endif
