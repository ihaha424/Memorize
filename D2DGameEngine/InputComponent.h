#pragma once
#include "IComponent.h"
#include "InputAction.h"

class IInput
{
public:
	virtual void InitializeInput() = 0;
	virtual void DestroyInput() = 0;
};

class InputComponent : public IComponent
{
public:
	InputComponent(Actor* _owner) : IComponent(_owner) {};
	virtual ~InputComponent() {};

	template <typename Class, typename Function>
	void ActionBinding(Class* _instance, int _key, Function&& _function, InputState _keyState, BYTE _device = KeyBoardInput)
	{
		InputAction::GetInstance()->AddCommand(_instance, _key, _function, _keyState, _device);
	}

	template <typename Class, typename Function>
	void ActionBinding(Class* _instance, const std::vector<int>& _keyList, Function&& _function, InputState _keyState, BYTE _device = KeyBoardInput)
	{
		InputAction::GetInstance()->AddCommand(_instance, _keyList, _function, _keyState, KeyBoardInput);
	}


	template <typename Class>
	void DeleteCommand(Class* _instance, int _key, InputState _keyState, BYTE _device = KeyBoardInput)
	{
		InputAction::GetInstance()->DeleteCommand(_instance, _key, _keyState, _device);
	}

	template <typename Class>
	void DeleteCommand(Class* _instance, const std::vector<int>& _keyList, InputState _keyState, BYTE _device = KeyBoardInput)
	{
		InputAction::GetInstance()->DeleteCommand(_instance, _keyList, _keyState, _device);
	}
};

template<class T>
concept InputComponentType = std::is_base_of<InputComponent, T>::value;


// Use Case InputKey Ex>
// DIK_Q
// DIK_W
// DIK_E
// DIK_R
// DIK_T
// DIK_Y
// DIK_U
// DIK_I
// DIK_O
// DIK_P
