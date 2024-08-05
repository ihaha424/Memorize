#include "framework.h"
#include "InputAction.h"

InputAction* InputAction::pInstance = nullptr;
InputAction* InputAction::GetInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new InputAction();
	}
	return pInstance;
}
void InputAction::DestroyInputAction()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

void InputAction::Update(const float& deltaTime)
{
	CommandStream.clear();
	TapCommandStream.clear();

	//CommandStream.push_back();

	//인풋을 한 0.2초 마다 초기화 키셔주면서 Tap -> 초기화 Tap ->초기화 반복하면 될듯? 개소리야 시발
	StreamTime += deltaTime;
	for (int i = 0; i < InputSize; i++)
	{
		if (CurState[i])
		{
			if (PreveState[i])
				CommandStream.push_back(i);		//Hold는 그냥 추가
			else
				TapCommandStream.push_back(i);	//KeyDown
		}
		else
			if (PreveState[i])
				InvokeFunction(i, KeyUp);		//KeyUp은 바로 실행 떈거니까?
	}

	// KeyDown(Tap) 실행
	int CommandStreamSize = CommandStream.size();
	for (auto tapKey : TapCommandStream)
	{
		CommandNode* curNode = &HeadNode;
		std::map<int, CommandNode*>::iterator it = curNode->NextCommandNode.find(tapKey);
		if (it == curNode->NextCommandNode.end())
		{
			continue; //Tap된 입력이 없다는 것.
		}
		curNode = it->second;
		std::vector<int> TapCommandList;
		TapCommandList.push_back(tapKey);
		if (FindHoldCommand(curNode, TapCommandList, CommandStreamSize))
			InvokeFunction(TapCommandList, KeyDown);
	}

	// KeyHold 실행
	for (auto holdKey : CommandStream)
		InvokeFunction(holdKey, KeyHold);

	//마지막에 넘겨서 초기화 시키고 다음 Update때 정보를 비교 && 처리
	memmove(PreveState, CurState, InputSize);
	memset(CurState, 0, InputSize);
}

// -> CommandNode의 CurState로 눌렸는지 체크하면 훨씬 쉽게 가능한데???? 아 진짜 너무 늦게 꺠달았다.
// 굳이 눌려있는 키들 받아서 확인 할 필요없이 현재 노드가 눌려있는지 확인하면...? -> 근데 이 경우에상태를 모든 맵에 다 업데이트해야하는 문제가 있기는 함.
// 잘 하면 될것 같기도 하지만 기각
bool InputAction::FindHoldCommand(CommandNode*& curNode, std::vector<int>& TapCommandList, int size)
{
	CommandNode* tempNode = curNode;
	std::map<int, bool> CheckRepeate;
	for (auto n : TapCommandList)
		CheckRepeate[n] = true;
	for (int i = 0; i < size; i++)
	{
		if (CheckRepeate[CommandStream[i]]) // TapCommandList의 중복 체크 Ex> a e a e의 커맨드 방지
			continue;
		std::map<int, CommandNode*>::iterator it = curNode->NextCommandNode.find(CommandStream[i]);
		if (it != curNode->NextCommandNode.end())
		{
			TapCommandList.push_back(CommandStream[i]);
			curNode = it->second;
			if (FindHoldCommand(curNode, TapCommandList, size))
				return true;
			else
			{
				curNode = tempNode;
				TapCommandList.pop_back();
			}
		}
	}
	if (curNode->ExcuteFunction[KeyDown] != nullptr)
		return true;
	return false;
}

// 비트로 자리수 올려서 입력이 어디서 되었는지 구분하기 - 마무스, 키보드
// Update에서 스트림 관리하기?
void InputAction::AddInputKey(int key, BYTE Device)
{
	CurState[key + Device] = true;
}

void InputAction::InvokeFunction(std::vector<int>& keyList, InputState KeyState)
{
	//Find
	CommandNode* curNode = &HeadNode;
	for (int depth = 0; depth < keyList.size(); depth++)
	{
		std::map<int, CommandNode*>::iterator it = curNode->NextCommandNode.find(keyList[depth]);
		if (it == curNode->NextCommandNode.end())
		{
			return;
		}
		curNode = it->second;
	}

	if (curNode->ExcuteFunction[KeyState] != nullptr)
		curNode->ExcuteFunction[KeyState]->InvokeFunction();
}

void InputAction::InvokeFunction(int keyList, InputState KeyState)
{
	//Find
	CommandNode* curNode = &HeadNode;
	std::map<int, CommandNode*>::iterator it = curNode->NextCommandNode.find(keyList);
	if (it == curNode->NextCommandNode.end())
	{
		return;
	}
	curNode = it->second;

	if (curNode->ExcuteFunction[KeyState] != nullptr)
		curNode->ExcuteFunction[KeyState]->InvokeFunction();
}