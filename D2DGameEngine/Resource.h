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
		Resource의 Base
 *		리소스를 상속받아 받느는 것은 Initialize에서 ResourceType를 지정해 주어야함
 */
template<typename T, typename ResourceType>
class Resource
	:public IResource
{
public:
	Resource() {};
	~Resource() {};


	/**
	 * @brief 리소스파일 키값 연결과 패스로 만드는 함수
	 * 모든 리소스 파일이 만들어져야 함
	 */
	virtual void LoadFile(std::wstring _key) = 0;

	/**
	 * @brief 리소스 데이터를 포인터로 가져오는 것
	 */
	ResourceType* GetResource(){ return resource; }
	virtual const std::type_index GetType() { return std::type_index(typeid(T)); }

protected:
	ResourceType* resource = nullptr;
};

