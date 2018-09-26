#pragma once

//智能指针
#define PTR(T) shared_ptr<T>
#define MKPTR(T) make_shared<T>
#define WPTR weak_ptr<T>
#define TOPTR(T1, T2) static_pointer_cast<T1>(T2)

//动态绑定
#ifndef BIND
#define BIND(p1, p2, ...)  bind(&p1, p2, ##__VA_ARGS__)
#define BIND1(p1, p2, ...) bind(&p1, p2, placeholders::_1, ##__VA_ARGS__)
#define BIND2(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, ##__VA_ARGS__)
#define BIND3(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, ##__VA_ARGS__)
#define BIND4(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, ##__VA_ARGS__)
#define BIND5(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, ##__VA_ARGS__)
#endif  //BIND

//静态绑定
#ifndef BIND_S
#define BIND_S(p1, ...)  bind(p1, ##__VA_ARGS__)
#define BIND1_S(p1, ...) bind(p1, placeholders::_1, ##__VA_ARGS__)
#define BIND2_S(p1, ...) bind(p1, placeholders::_1, placeholders::_2, ##__VA_ARGS__)
#define BIND3_S(p1, ...) bind(p1, placeholders::_1, placeholders::_2, placeholders::_3, ##__VA_ARGS__)
#define BIND4_S(p1, ...) bind(p1, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, ##__VA_ARGS__)
#define BIND5_S(p1, ...) bind(p1, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, ##__VA_ARGS__)
#endif //SBIND


