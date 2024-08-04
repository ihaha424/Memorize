# 게임인재원 6기 2쿼터 팀 프로젝트
---------------------------------------------
### 코드 컨벤션

```cpp
// 글로벌 변수 절대 X
// Tab은 4칸!
class MyClass {
    const int MY_CONST_VARIABLE;  // 상수는 대문자 + 스네이크(_)

    int privateVariable;  // 프라이빗 변수는 camelCase
    void PrivateFunction() // 프라이빗 함수는 PascalCase
    {
        int a{ 0 };  // 로컬 변수는 본인 편한대로!
        return;
    }

public:
    int myPublicVariable;  // 퍼블릭 변수는 camelCase
    // 포인터는 포인터라고 이름에 명시 안함
    OtherObject* otherObj;
    // 참조(Reference)(`&`)는 참조라고 이름에 명시 안함
    OtherObject& otherObj;

    // 기본 변수 이니셜라이징(초기화)은 Uniform 이니셜라이져 `{}` 사용
    MyClass() : privateVariable{ 0 }, publicVariable{ 0 }, otherObj{ nullptr } {}
    // 이니셜라이징 리스트가 많으면 짤라서!
    MyClass(                //함수 인자는 _ + camelCase
        int _privateVar,
        int _publicVar,
        OtherObject* _obj,
        OtherObject& _obj)
        :   privateVariable{ _privateVar },
            publicVariable{ _publicVar },
            otherObj{ _obj }, otherObj{ _obj } 
    {}

    void MyPublicFunction() 
    {  // 퍼블릭 함수는 PascalCase
        int a{ 0 };  // 로컬 변수는 본인 편한대로!
        return;
    }

    bool MyFunctionWithLongParameterList(  // 함수 파라미터가 많은면 짤라서!
        int _a,            // a
        double _d,         // d
        Object* _obj,     // obj
        void* _reserved   // reserved
    )
    {
        int localA{ a };    // 로컬 변수는 본인 편한대로!
        double local_d{ d } // 로컬 변수는 본인 편한대로!

            // 라인이 길어지면 짤라서!
            return  (a == 1) &&
                    (d == 0.7) &&
                    (pObj != nullptr) &&
                    (pReserved == nullptr);
    }
};
```



---------------------------------------------

+) Commit시 로그 잘 쓰기!<br></br>
+) 주석 잘 쓰기!
---------------------------------------------
## 참고 하면 좋은 사이트

### Behavior Tree
  : https://maplestoryworlds-creators.nexon.com/ko/docs?postId=562<br></br>
  : https://www.gamedeveloper.com/programming/behavior-trees-for-ai-how-they-work
  
### FSM
  : https://www.aleksandrhovhannisyan.com/blog/implementing-a-finite-state-machine-in-cpp/#approach-3-finite-state-machine<br></br>
  : https://gameprogrammingpatterns.com/state.html
