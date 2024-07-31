# 게임인재원 6기 2쿼터 팀 프로젝트
---------------------------------------------
### 코드 컨벤션

```
// 글로벌 변수 절대 X
// Tab은 4칸!
class MyClass {
    const int MY_CONST_VARIABLE;  // 상수는 대문자 + 스네이크(_)

    int myPrivateVariable;  // 프라이빗 변수는 camelCase
    void MyPrivateFunction() // 프라이빗 함수는 PascalCase
    {
        int a{ 0 };  // 로컬 변수는 본인 편한대로!
        return;
    }

public:
    int myPublicVariable;  // 퍼블릭 변수는 camelCase
    // 포인터는 포인터라고 이름에 명시 안함
    OtherObject* pOtherObj, otherObjPointer;
    // 참조(Reference)(`&`)는 참조라고 이름에 명시 안함
    OtherObject& rOtherObj, otherObjRef;

    // 기본 변수 이니셜라이징(초기화)은 Uniform 이니셜라이져 `{}` 사용
    MyClass() : _myPrivateVariable{ 0 }, myPublicVariable{ 0 }, pOtherObj{ nullptr } {}
    // 이니셜라이징 리스트가 많으면 짤라서!
    MyClass(
        int myPrivateVar,
        int myPublicVar,
        OtherObject* pObj,
        OtherObject& rObj)
        :   _myPrivateVariable{ myPrivateVar },
            myPublicVariable{ myPublicVar },
            pOtherObj{ pObj }, rOtherObj{ rObj } 
    {}

    void MyPublicFunction() 
    {  // 퍼블릭 함수는 PascalCase
        int a{ 0 };  // 로컬 변수는 본인 편한대로!
        return;
    }

    bool MyFunctionWithLongParameterList(  // 함수 파라미터가 많은면 짤라서!
        int a,            // a
        double d,         // d
        Object* pObj,     // pObj
        void* pReserved   // pReserved
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

+) Commit시 로그 잘 쓰기!
+) 주석 잘 쓰기!
---------------------------------------------
## 참고 하면 좋은 사이트

### Behavior Tree
  : https://maplestoryworlds-creators.nexon.com/ko/docs?postId=562
