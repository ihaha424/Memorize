#pragma once
#include "D2DGameEngine/UIPanel.h"

class DisfellPanel : public UIPanel
{
	std::vector<class UIImage*> disfellCommands;
	int commandMaxCount = 8;

	class GPlayerController* playerController;

	class ID2D1Bitmap* Qbm;
	class ID2D1Bitmap* Wbm;
	class ID2D1Bitmap* Ebm;
	class ID2D1Bitmap* Rbm;

public:
	DisfellPanel(World* _world);
	virtual ~DisfellPanel();

	/**
	 * @brief index��° Ŀ�ǵ� �̹����� �����մϴ�.
	 * @param index ������ Ŀ�ǵ� ��°��
	 * @param command Ŀ�ǵ� ���� ����
	 */
	void SetCommandImage(int index, int command);

	/**
	 * @brief index��° Ŀ�ǵ� �̹����� ����ϴ�. 
	 * @param index ���� Ŀ�ǵ� ��°��
	 */
	void HideCommandImage(int index);
};

