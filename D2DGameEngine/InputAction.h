#pragma once

#include "IInputDevice.h"

class Deligator0
{
public:
    using CallBackFunction = std::function<void()>;
    using CallBcakMAp = std::map<uintptr_t, CallBackFunction>;

    template <typename Class>
    void RemoveFunction(Class* instance)
    {
        FuctionList.erase(reinterpret_cast<uintptr_t>(instance));
    }

    void InvokeFunction()
    {
        for (const auto& pair : FuctionList)
        {
            pair.second();
        }
    }

    template <typename Class, typename Function>
    void AddFunction(Class* _instance, Function&& _function)
    {
        CallBackFunction callback = [_instance, _function]()
            {
                (_instance->*_function)();
            };
        FuctionList[reinterpret_cast<uintptr_t>(_instance)] = callback;
    }

private:
    CallBcakMAp FuctionList;
};

// 커맨드는 마지막 Tap의 경우만 판단해서 된다.
   // 오직 KeyDown만 가능..?
   // 더블 클릭의 경우 클라이언트 사이드에서 {static bool같은 것으로 GetClock으로 비교해서 판단 할 수도..?}
   // ->람다를 이용해서 객체 내부에서 시간 확인 가능...? 쩔수임 시간 부족
enum InputState { KeyDown, KeyHold, KeyUp, None };

/// <summary>
/// 여러 키 리스트는 가능한데 더블클릭은 어케 할지?
/// 더블클릭은 나중에 따로 함수 매핑시 인자로 넣는 AddCommandLimitedTime(Class* instance, std::vector<int>& keyList??? int key, Function&& function, InputState KeyState, float deltaTime)을 만들어서 
///         bool                    PreveState[InputSize] = { 0 };를
///         std::pair<bool,float>   PreveState[InputSize] = { 0 };로
/// 관리 해야할 듯????
/// </summary>

/// <summary>
/// 지금은 1개의 함수만 매핑이 되는데
/// std::shared_ptr<Deligator0> ExcuteFunction[None] = { nullptr };를
/// 리스트로 만들면 여러개 매핑 가능? ㅇㅇ
/// </summary>
struct CommandNode
{
    int Command = -1;
    std::map<int, CommandNode*> NextCommandNode;
    InputState CurState = InputState::None;
    //Input의 경우 무조건 인자가 0
    std::shared_ptr<Deligator0> ExcuteFunction[None] = { nullptr };
};

class InputAction
{
public:
    static InputAction* GetInstance();
    static void DestroyInputAction();

public:
    void Update(const float& deltaTime);

    //Object가 커맨드리스트(단일도 가능)에 Function을 매핑 시키는 방법
    void AddInputKey(int key, BYTE Device = KeyBoardInput);

    // 한 커맨드에 한객체의 한 function만 가능
    // 같은 객체가 같은 커맨드에 함수를 추가할 경우 덮혀 씌워진다.
    template <typename Class, typename Function>
    void AddCommand(Class* instance, const std::vector<int>& keyList, Function&& function, InputState KeyState, BYTE Device = KeyBoardInput)
    {
        //Find & Make
        CommandNode* curNode = &HeadNode;
        for (int depth = keyList.size() - 1; depth >= 0; depth--)
        {
            std::map<int, CommandNode*>::iterator it = curNode->NextCommandNode.find(keyList[depth + Device]);
            if (it == curNode->NextCommandNode.end())
            {
                CommandNode* temp = new CommandNode();
                temp->Command = keyList[depth + Device];
                curNode->NextCommandNode.insert({ keyList[depth + Device], temp });
                curNode = temp;
            }
            else
            {
                curNode = it->second;
            }
        }

        //Function Mapping
        if (curNode->ExcuteFunction[KeyState] == nullptr)
        {
            curNode->ExcuteFunction[KeyState] = std::make_shared<Deligator0>();
        }
        curNode->ExcuteFunction[KeyState]->AddFunction(instance, function);
    }
    template <typename Class, typename Function>
    void AddCommand(Class* instance, int keyList, Function&& function, InputState KeyState, BYTE Device = KeyBoardInput)
    {
        //Find & Make
        CommandNode* curNode = &HeadNode;

        std::map<int, CommandNode*>::iterator it = curNode->NextCommandNode.find(keyList + Device);
        if (it == curNode->NextCommandNode.end())
        {
            CommandNode* temp = new CommandNode();
            temp->Command = keyList + Device;
            curNode->NextCommandNode.insert({ keyList + Device, temp });
            curNode = temp;
        }
        else
        {
            curNode = it->second;
        }


        //Function Mapping
        if (curNode->ExcuteFunction[KeyState] == nullptr)
        {
            curNode->ExcuteFunction[KeyState] = std::make_shared<Deligator0>();
        }
        curNode->ExcuteFunction[KeyState]->AddFunction(instance, function);
    }
    template <typename Class>
    void DeleteCommand(Class* instance, const std::vector<int>& keyList, InputState KeyState, BYTE Device = KeyBoardInput)
    {
        //Find
        CommandNode* curNode = &HeadNode;
        CommandNode* prevNode = nullptr;
        std::map<int, CommandNode*>::iterator it;
        int depth;
        //keyList.size()가 0이면 Error
        for (depth = keyList.size() - 1; depth >= 0; depth--)
        {
            it = curNode->NextCommandNode.find(keyList[depth + Device]);
            if (it == curNode->NextCommandNode.end())
            {
                return;
            }
            prevNode = curNode;
            curNode = it->second;
        }
        curNode->ExcuteFunction[KeyState]->RemoveFunction(instance);
        curNode->ExcuteFunction[KeyState] = nullptr;
        for (auto curFuction : curNode->ExcuteFunction)
        {
            if (curFuction != nullptr)
                return;
        }
        if (curNode->NextCommandNode.size() == 0)
        {
            if (nullptr != prevNode)
                prevNode->NextCommandNode.erase(keyList[depth + 1 + Device]);
            delete curNode;
        }
    }
    template <typename Class>
    void DeleteCommand(Class* instance, int keyList, InputState KeyState, BYTE Device = KeyBoardInput)
    {
        //Find
        CommandNode* curNode = &HeadNode;
        CommandNode* prevNode = curNode;
        std::map<int, CommandNode*>::iterator it;
        it = curNode->NextCommandNode.find(keyList + Device);
        if (it == curNode->NextCommandNode.end())
        {
            return;
        }
        curNode = it->second;
        curNode->ExcuteFunction[KeyState]->RemoveFunction(instance);
        curNode->ExcuteFunction[KeyState] = nullptr;
        for (auto curFuction : curNode->ExcuteFunction)
        {
            if (curFuction != nullptr)
                return;
        }
        if (curNode->NextCommandNode.size() == 0)
        {
            if (nullptr != prevNode)
                prevNode->NextCommandNode.erase(keyList + Device);
            delete curNode;
        }
    }

private:
    // 함수 실행
    void InvokeFunction(std::vector<int>& keyList, InputState KeyState);
    void InvokeFunction(int keyList, InputState KeyState);

    // Tap이 된 키 기준으로 눌려 있는 커맨드들을 확인해서 가장 빠르게 완성되는 커맨드를 실행
    bool FindHoldCommand(CommandNode*& curNode, std::vector<int>& TapCommandList, int size);

private:
    //키 입력 처리
    static constexpr size_t InputSize = 300;
    bool                    PreveState[InputSize] = { false };
    bool                    CurState[InputSize] = { false };

private:
    //Object가 커맨드리스트(단일도 가능)에 Function을 매핑 시키는 방법
    std::vector<int>    CommandStream;
    std::vector<int>    TapCommandStream;
    float               StreamTime = 0;
    float               MaxStreamTime = 0.2f;
    std::vector<int>    InputData;

    //Object가 커맨드리스트(단일도 가능)에 Function을 매핑 시키는 구조
    CommandNode    HeadNode;  //Command = -1, Excute는 없다. // 할당은 각각의 객체에서 해주고 딜리트는 한번에 하기

private:
    InputAction() {};
    ~InputAction() {};

    static InputAction* pInstance;
};
