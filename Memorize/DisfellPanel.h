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
	class ID2D1Bitmap* Qbm_off;
	class ID2D1Bitmap* Wbm_off;
	class ID2D1Bitmap* Ebm_off;
	class ID2D1Bitmap* Rbm_off;

	bool ending = false;
	float elapsedTime = 0.f;
public:
	DisfellPanel(World* _world);
	virtual ~DisfellPanel();

	virtual void Update(float _dt) override;

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
	void CommandImageOff(int index, int command);
};

