#pragma once

//enum ResourceType {Sprite, Sound, ReflectionData};

class IResource {

public:


	void SetKey(std::wstring _key) { key = _key; }

	const std::wstring GetKey() const { return key; }
	virtual const std::type_index GetType() = 0;

private:
	std::wstring	key;	// is equal to the path
};

/**
 * @brief
		Resource�� Base
 *		���ҽ��� ��ӹ޾� �޴��� ���� Initialize���� ResourceType�� ������ �־����
 */
template<typename T, typename ResourceType>
class Resource
	:public IResource
{
public:
	Resource() {};
	~Resource() {};


	/**
	 * @brief ���ҽ����� Ű�� ����� �н��� ����� �Լ�
	 * ��� ���ҽ� ������ ��������� ��
	 */
	virtual void LoadFile(std::wstring _key) = 0;

	/**
	 * @brief ���ҽ� �����͸� �����ͷ� �������� ��
	 */
	ResourceType* GetResource(){ return resource; }
	virtual const std::type_index GetType() { return std::type_index(typeid(T)); }

protected:
	ResourceType* resource = nullptr;
};

