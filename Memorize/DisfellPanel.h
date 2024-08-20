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
	 * @brief index번째 커맨드 이미지를 설정합니다.
	 * @param index 설정할 커맨드 번째수
	 * @param command 커맨드 원소 종류
	 */
	void SetCommandImage(int index, int command);

	/**
	 * @brief index번째 커맨드 이미지를 숨깁니다. 
	 * @param index 숨길 커맨드 번째수
	 */
	void CommandImageOff(int index, int command);
};

