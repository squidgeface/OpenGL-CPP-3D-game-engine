#pragma once

#include "prefab.h"

class CInput;

class CButton : public CPrefab
{
public:
	CButton() : CPrefab(), m_bIsBtn(false){};

	bool CheckHover(CInput* _object);
	void SetButton(bool state);
	void RenderShapes(GLuint program);

	bool GetShowing();

private:
	bool m_bIsBtn;
};

